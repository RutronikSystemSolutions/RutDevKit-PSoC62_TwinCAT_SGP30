/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 5.05.02 (2018-08-30)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.59.01 (2018-05-17)                                     **
**                                                                            */
/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2013 HMS Industrial Networks AB                 **
**                                                                            **
** This code is the property of HMS Industrial Networks AB.                   **
** The source code may not be reproduced, distributed, or used without        **
** permission. When used together with a product from HMS, permission is      **
** granted to modify, reproduce and distribute the code in binary form        **
** without any restrictions.                                                  **
**                                                                            **
** THE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. HMS DOES NOT    **
** WARRANT THAT THE FUNCTIONS OF THE CODE WILL MEET YOUR REQUIREMENTS, OR     **
** THAT THE OPERATION OF THE CODE WILL BE UNINTERRUPTED OR ERROR-FREE, OR     **
** THAT DEFECTS IN IT CAN BE CORRECTED.                                       **
********************************************************************************
********************************************************************************
** File Description:
** Implementation of message segmentation support
********************************************************************************
********************************************************************************
*/

#include "abcc_drv_cfg.h"
#include "abcc_td.h"
#include "abp.h"
#include "abcc.h"
#include "abcc_debug_err.h"
#include "abcc_port.h"
#include "abcc_seg.h"

/*******************************************************************************
** Constants
********************************************************************************
*/
#ifndef ABCC_NUM_SEGMENTATION_SESSIONS
#define ABCC_NUM_SEGMENTATION_SESSIONS 1
#endif

#define ABCC_MSG_HEADER_TYPE_SIZEOF 12

#define ABCC_SIZE_SEG_SIZE ABCC_CFG_MAX_MSG_SIZE

/********************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** To determine if a command belongs to an existing segmentation session the
** the members of this struct must match the command that started the session.
**------------------------------------------------------------------------------
**   bDestObj  - Destination object.
**   iInstance - Instance.
**   bCmd      - Command.
**   bCmdExt0  - Command extension 0.
**------------------------------------------------------------------------------
*/
typedef struct abcc_SegSessionIdentifiers
{
   UINT8 bDestObj;
   UINT16 iInstance;
   UINT8 bCmd;
   UINT8 bCmdExt0;
}
abcc_SegSessionIdentifiersType;

/*------------------------------------------------------------------------------
** Segmentations session type
**------------------------------------------------------------------------------
**   pxData        - Pointer to abcc message payload
**   pnDone        - Function to be called when segmentation session is done
**   pxObject      - User defined object. Supplied as parameter in pnDone function.
**   lBytesLeft    - Number of bytes left to be sent.
**   lTotalMsgSize - Size of entire payload to be sent
**   bRspCmdExt0   - Command extension 0 value to use in response segments.
**   sIdentifiers  - Used to match commands with existing sessions.
**   fInUse        - Indicates if the session is in use.
**------------------------------------------------------------------------------
*/
typedef struct abcc_SegSession
{
   const void* pxData;
   const void* pxObject;
   ABCC_SegMsgHandlerDoneFuncType pnDone;
   ABCC_SegMsgHandlerNextBlockFuncType pnNext;
   UINT32 lBytesLeft;
   UINT32 lDataBlockSize;
   UINT8 bRspCmdExt0;
   abcc_SegSessionIdentifiersType sIdentifiers;
   BOOL  fInUse;
}
abcc_SegSessionType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Number of segmentation sessions in use.
**------------------------------------------------------------------------------
*/
static UINT8 abcc_bSegNumUsedInst;

/*------------------------------------------------------------------------------
** Place holder for segmentation sessions
**------------------------------------------------------------------------------
*/
static abcc_SegSessionType abcc_sSegSession[ ABCC_NUM_SEGMENTATION_SESSIONS ];

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Allocate segmentations instance
**------------------------------------------------------------------------------
** Arguments:
**    None
** Returns:
**    Pointer to abcc_SegInstanceType. NULL if no free resource is found
**------------------------------------------------------------------------------
*/
static abcc_SegSessionType* AllocSegmentationSession( void )
{
   UINT8 bSession;
   abcc_SegSessionType* psSegSession = NULL;
   ABCC_PORT_UseCritical();

   ABCC_PORT_EnterCritical();

   for( bSession = 0; bSession < ABCC_NUM_SEGMENTATION_SESSIONS; bSession++ )
   {
      if( !abcc_sSegSession[ bSession ].fInUse )
      {
         abcc_bSegNumUsedInst++;
         psSegSession = &abcc_sSegSession[ bSession ];
         psSegSession->fInUse = TRUE;
         break;
      }
   }

   ABCC_PORT_ExitCritical();

   return( psSegSession );
}

/*------------------------------------------------------------------------------
** Find segmentation session with matching source id.
**------------------------------------------------------------------------------
** Arguments:
**    psMsg - Pointer to ABCC message.
**
** Returns:
**    Pointer to abcc_SegInstanceType. NULL if no match is found.
**------------------------------------------------------------------------------
*/
static abcc_SegSessionType* FindSegmentationSession( const ABP_MsgType* psMsg )
{
   UINT8 bSession;
   abcc_SegSessionType* psSegSession = NULL;
   ABCC_PORT_UseCritical();

   ABCC_PORT_EnterCritical();

   for( bSession = 0; bSession < ABCC_NUM_SEGMENTATION_SESSIONS; bSession++ )
   {
      if( abcc_sSegSession[ bSession ].fInUse &&
         ( abcc_sSegSession[ bSession ].sIdentifiers.bDestObj == ABCC_GetMsgDestObj( psMsg ) ) &&
         ( abcc_sSegSession[ bSession ].sIdentifiers.iInstance == ABCC_GetMsgInstance( psMsg ) ) &&
         ( abcc_sSegSession[ bSession ].sIdentifiers.bCmd == ABCC_GetMsgCmdBits( psMsg ) ) &&
         ( abcc_sSegSession[ bSession ].sIdentifiers.bCmdExt0 == ABCC_GetMsgCmdExt0( psMsg ) ) )
      {
         psSegSession = &abcc_sSegSession[ bSession ];
         break;
      }
   }
   ABCC_PORT_ExitCritical();

   return( psSegSession );
}

/*------------------------------------------------------------------------------
** Free segmentation session resource
**------------------------------------------------------------------------------
** Arguments:
**    psSegSession - Pointer to segmentation instance
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
static void FreeSegmentationSession( abcc_SegSessionType* psSegSession )
{
   ABCC_PORT_UseCritical();

   ABCC_PORT_EnterCritical();

   psSegSession->fInUse = FALSE;
   abcc_bSegNumUsedInst--;

   ABCC_PORT_ExitCritical();
}

/*------------------------------------------------------------------------------
** Get next data block from callback if available
**------------------------------------------------------------------------------
** Arguments:
**    psSegSession - Pointer to segmentation instance
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
static void GetNextDataBlock( abcc_SegSessionType* psSegSession )
{
   if( psSegSession->pnNext != NULL )
   {
      psSegSession->pxData = psSegSession->pnNext( (void*)psSegSession->pxObject,
                                                   &psSegSession->lDataBlockSize );

      if( psSegSession->pxData == NULL )
      {
         psSegSession->lDataBlockSize = 0;
      }

      psSegSession->lBytesLeft = psSegSession->lDataBlockSize;
   }
}

/*------------------------------------------------------------------------------
** Copy next segment to the abcc message and set the required segmentation bits.
**------------------------------------------------------------------------------
** Arguments:
**    psSegSession - Pointer to segmentation instance
**    psMsg        - Pointer to abcc message
**    fFirstSeg    - Indicates first segment
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
static void PrepareAndSendSegmentationRespMsg( abcc_SegSessionType* psSegSession, ABP_MsgType* psMsg, BOOL fFirstSeg )
{
   UINT8  bCmdExt1 = 0;
   UINT16 iDataSize = 0;
   UINT16 iPayloadOffset;
   UINT16 iNumOctetsToCopy;

   if( fFirstSeg )
   {
      bCmdExt1 = ABP_MSG_CMDEXT1_SEG_FIRST;

      /*
      ** If no payload pointer is provided, check if the user has implemented the callback
      */
      if( psSegSession->pxData == NULL )
      {
         GetNextDataBlock( psSegSession );
      }
   }

   /*
   ** Fill the payload until the segment is full or until no more data is
   ** provided by the user.
   */
   while( ( iDataSize != ABCC_SIZE_SEG_SIZE ) && ( psSegSession->lBytesLeft != 0 ) )
   {
      /*
      ** Store current payload offset
      */
      iPayloadOffset = iDataSize;

      if( ( iDataSize + psSegSession->lBytesLeft ) > ABCC_SIZE_SEG_SIZE )
      {
         iDataSize = ABCC_SIZE_SEG_SIZE;
      }
      else
      {
         iDataSize += psSegSession->lBytesLeft;
      }

      iNumOctetsToCopy = iDataSize - iPayloadOffset;

      /*
      ** Copy payload from user buffer to response message buffer
      */
      ABCC_PORT_CopyOctets( ABCC_GetMsgDataPtr( psMsg ),
                            iPayloadOffset,
                            psSegSession->pxData,
                            psSegSession->lDataBlockSize - psSegSession->lBytesLeft,
                            iNumOctetsToCopy );

      psSegSession->lBytesLeft -= iNumOctetsToCopy;

      /*
      ** If the whole block is copied, check if the user has more data to send.
      */
      if( psSegSession->lBytesLeft == 0 )
      {
         GetNextDataBlock( psSegSession );
      }
   }

   if( psSegSession->lBytesLeft == 0 )
   {
      bCmdExt1 |= ABP_MSG_CMDEXT1_SEG_LAST;
   }

   /*
   ** Set segmentation bits
   */
   ABCC_SetMsgCmdExt1( psMsg, bCmdExt1 );
   ABP_SetMsgResponse( psMsg, iDataSize );

   /*
   ** Set command extension 0 for every segment.
   */
   ABCC_SetMsgCmdExt0( psMsg, psSegSession->bRspCmdExt0 );

   ABCC_SendRespMsg( psMsg );

   /*
   ** Check if last segment has been sent
   */
   if( psSegSession->lBytesLeft == 0 )
   {
      if( psSegSession->pnDone )
      {
         psSegSession->pnDone( (void*)psSegSession->pxObject );
      }
      FreeSegmentationSession( psSegSession );
   }
}

/*******************************************************************************
** Public Services
********************************************************************************
*/

void ABCC_SegmentationInit( void )
{
   UINT8 bSession;

   abcc_bSegNumUsedInst = 0;
   for( bSession = 0; bSession < ABCC_NUM_SEGMENTATION_SESSIONS; bSession++ )
   {
      abcc_sSegSession[ bSession ].fInUse = FALSE;
   }
}

EXTFUNC ABCC_ErrorCodeType ABCC_StartServerRespSegmentationSession( const ABP_MsgHeaderType* psReqMsgHeader,
                                                                    UINT8 bRspCmdExt0,
                                                                    const void* pxData,
                                                                    UINT32 lSize,
                                                                    ABCC_SegMsgHandlerNextBlockFuncType pnNext,
                                                                    ABCC_SegMsgHandlerDoneFuncType pnDone,
                                                                    const void* pxObject )
{
   ABP_MsgType* psMsg;
   abcc_SegSessionType* psSegSession;

   if( ( pxData == NULL ) && ( pnNext == NULL ) )
   {
      return( ABCC_EC_UNEXPECTED_NULL_PTR );
   }

   psSegSession = AllocSegmentationSession();

   if( psSegSession == NULL )
   {
      ABCC_ERROR( ABCC_SEV_WARNING,
                  ABCC_EC_NO_RESOURCES,
                  0 );

      return( ABCC_EC_NO_RESOURCES );
   }

   psSegSession->lBytesLeft = lSize;
   psSegSession->lDataBlockSize = lSize;
   psSegSession->pxData = pxData;
   psSegSession->pnDone = pnDone;
   psSegSession->pnNext = pnNext;
   psSegSession->pxObject = pxObject;
   psSegSession->bRspCmdExt0 = bRspCmdExt0;
   psSegSession->sIdentifiers.bDestObj = ABCC_GetMsgDestObj( psReqMsgHeader );
   psSegSession->sIdentifiers.iInstance = ABCC_GetMsgInstance( psReqMsgHeader );
   psSegSession->sIdentifiers.bCmd = ABCC_GetMsgCmdBits( psReqMsgHeader );
   psSegSession->sIdentifiers.bCmdExt0 = ABCC_GetMsgCmdExt0( psReqMsgHeader );

   /*
   ** Get message buffer. Will be converted to response later.
   */
   psMsg = ABCC_GetCmdMsgBuffer();

   if( psMsg == NULL )
   {
      FreeSegmentationSession( psSegSession );

      ABCC_ERROR( ABCC_SEV_WARNING,
                  ABCC_EC_NO_RESOURCES,
                  0 );

      return( ABCC_EC_NO_RESOURCES );
   }

   /*
   ** Copy the header from the request
   */
   ABCC_PORT_MemCpy( psMsg, psReqMsgHeader, ABCC_MSG_HEADER_TYPE_SIZEOF );

   PrepareAndSendSegmentationRespMsg( psSegSession, psMsg, TRUE );

   return( ABCC_EC_NO_ERROR );
}

BOOL ABCC_HandleSegmentAck( ABP_MsgType* psMsg )
{
   abcc_SegSessionType* psSegSession;
   UINT8 bSourceId;

   if( abcc_bSegNumUsedInst == 0 )
   {
      return( FALSE );
   }

   bSourceId = ABCC_GetMsgSourceId( psMsg );

   psSegSession = FindSegmentationSession( psMsg );
   if( psSegSession == NULL )
   {
      return( FALSE );
   }

   if( ABCC_GetMsgCmdExt1( psMsg ) & ABP_MSG_CMDEXT1_SEG_ABORT )
   {
      ABCC_ERROR( ABCC_SEV_WARNING,
                  ABCC_EC_ERROR_RESP_SEGMENTATION,
                  bSourceId );
      /*
      ** Abort segmentation by clearing number of bytes left and clear callback pointer
      */
      psSegSession->lBytesLeft = 0;
      psSegSession->pnNext = NULL;
   }

   PrepareAndSendSegmentationRespMsg( psSegSession, psMsg, FALSE );

   return( TRUE );
}
