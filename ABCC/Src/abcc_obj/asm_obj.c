/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.05.02 (2018-08-30)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.59.01 (2018-05-17)                                     **
**    ABCC Driver    5.05.02 (2018-08-30)                                     **
**                                                                            */
/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2017 HMS Industrial Networks AB                 **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorized company official.                                         **
********************************************************************************
********************************************************************************
** Implementation of the Assembly Mapping Object.
**
** Dynamic assemblies are not supported!
**
** In abcc_drv_cfg.h make sure that the following definitions are set to:
** ABCC_CFG_REMAP_SUPPORT_ENABLED ( TRUE )
**
** -----------------------------------------------------------------------------
** EtherNet/IP specific requirements
** -----------------------------------------------------------------------------
** If there is at least one mappable assembly mapping instance defined, make
** sure that the following definition is set in abcc_obj_cfg.h:
** EIP_OBJ_ENABLE              ( TRUE )
**
** If there is at least one write mappable assembly mapping instance defined,
** make sure that the following definitions are set in abcc_obj_cfg.h
** EIP_IA_PROD_INSTANCE_ENABLE ( TRUE )
**
** The array defined by EIP_IA_PROD_INSTANCE_VALUE must have the same size as
** the number of process data mappable write assemblies.
** EIP_IA_PROD_INSTANCE_ARRAY_SIZE must be set to this size.
**
** If there is at least one read mappable assembly mapping instance defined,
** make sure that the following definitions are set in abcc_obj_cfg.h
** EIP_IA_CONS_INSTANCE_ENABLE ( TRUE )
**
** The array defined by EIP_IA_CONST_INSTANCE_VALUE must have the same size as
** the number of process data mappable read assemblies.
** EIP_IA_CONS_INSTANCE_ARRAY_SIZE must be set to this size.
********************************************************************************
********************************************************************************
*/

#include "abcc_obj_cfg.h"

#if ASM_OBJ_ENABLE

#include "abcc.h"
#include "abp_asm.h"
#include "appl_adi_config.h"
#include "abcc_ad_if.h"
#include "ad_obj.h"
#include "asm_obj.h"

#if !ABCC_CFG_REMAP_SUPPORT_ENABLED
   #error "ABCC_CFG_REMAP_SUPPORT_ENABLED must be set to TRUE to use the assembly mapping ojbect"
#endif

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define ASM_OA_NAME_VALUE           "Assembly mapping"
#define ASM_OA_REV_VALUE            1

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure describing an Assembly mapping object.
**------------------------------------------------------------------------------
*/
typedef struct asm_Object
{
   const  char* pcName;
   UINT8  bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
   /* Write PD Instance list is generated in runtime */
   /* Read PD Instance list is generated in runtime */
}
asm_ObjectType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Declare the Assembly mapping object instance.
**------------------------------------------------------------------------------
*/
static asm_ObjectType asm_sObject =
{
   ASM_OA_NAME_VALUE,       /* Name.                                          */
   ASM_OA_REV_VALUE,        /* Revision.                                      */
   0,                       /* Number of instances. (assigned in runtime)     */
   0                        /* Highest instance number. (assigned in runtime) */
};

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Processes commands to the Assembly mapping Instance
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage )
{
   const ASM_InstanceType* psInstance;
   UINT16                  iMapSize;
   UINT16                  iOctetOffset;

   if( ABCC_GetMsgInstance( psNewMessage ) > APPL_GetNumAsmInstances() )
   {
      /*
      ** The requested instance does not exist.
      ** Respond with an error.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );

      return;
   }

   psInstance = APPL_GetAsmInstances()[ ABCC_GetMsgInstance( psNewMessage ) - 1 ];

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_ASM_IA_DESCRIPTOR:
      {
         /*
         ** The 'descriptor' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData32( psNewMessage,
                            psInstance->lDescriptor,
                            0 );

         ABP_SetMsgResponse( psNewMessage, ABP_UINT32_SIZEOF );

         break;
      }
#if ASM_IA_NAME_ENABLE
      case ABP_ASM_IA_NAME:
      {
         /*
         ** The 'name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( psInstance->pacName );
         ABCC_SetMsgString( psNewMessage, psInstance->pacName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );

         break;
      }
#endif /* #if ASM_IA_NAME_ENABLE */
      default:
      {
         /*
         ** The 'ADI Map X' attribute or any other attribute is requested, verify
         ** if the requested attribute is a valid 'ADI MAP X' attribute..
         */
         UINT16 iAdisPerAttribute;
         UINT16 iNumAdisInMap;
         UINT16 iNumAdisInAttribute;
         UINT16 iIndex;
         UINT8  bAttribute;
         UINT8  bAdiMapNum;

         if( ABCC_GetMessageChannelSize() == ABP_MAX_MSG_255_DATA_BYTES )
         {
            iAdisPerAttribute = 62;
         }
         else
         {
            iAdisPerAttribute = 380;
         }

         iNumAdisInMap = AD_GetNumAdisInMap( psInstance->psMap );
         bAttribute = ABCC_GetMsgCmdExt0( psNewMessage );

         if( ( bAttribute > 12 ) ||
             ( bAttribute > ( ( iNumAdisInMap / iAdisPerAttribute ) + ABP_ASM_IA_ADI_MAP_XX ) ) )
         {
            /*
            ** This attribute does not exists.
            */
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );

            break;
         }

         bAdiMapNum = bAttribute - ABP_ASM_IA_ADI_MAP_XX;

         if( iNumAdisInMap >= ( ( bAdiMapNum + 1 ) * iAdisPerAttribute ) )
         {
            iNumAdisInAttribute = iAdisPerAttribute;
         }
         else
         {
            iNumAdisInAttribute = iNumAdisInMap - ( bAdiMapNum * iAdisPerAttribute );
         }

         if( ( iNumAdisInAttribute * ABP_BITS32_SIZEOF ) > ABCC_CFG_MAX_MSG_SIZE )
         {
            ABCC_ERROR( ABCC_SEV_WARNING, ABCC_EC_BAD_ASSEMBLY_INSTANCE, (UINT32)ABCC_GetMsgInstance( psNewMessage ) );
            ABCC_DEBUG_ERR( ( "Too many ADI's in this assembly mapping instance (%d) for the current configuration of " \
                              "ABCC_CFG_MAX_MSG_SIZE (%d)\n" \
                              "Minimum message size required: %d\n",
                              ABCC_GetMsgInstance( psNewMessage ),
                              ABCC_CFG_MAX_MSG_SIZE,
                              iNumAdisInAttribute * ABP_BITS32_SIZEOF ) );

            break;
         }

         iOctetOffset = 0;

         for( iIndex = bAdiMapNum * iAdisPerAttribute;
              ( iIndex < iNumAdisInMap ) && ( iIndex < ( ( bAdiMapNum + 1 ) * iAdisPerAttribute ) );
              iIndex++ )
         {
            UINT8 bNumElem;
            UINT8 bStartIndex;

            if( psInstance->psMap[ iIndex ].bNumElem == AD_MAP_ALL_ELEM )
            {
               /*
               ** Convert internal representation for all elements to the
               ** actual number of elements.
               */
               bNumElem = AD_GetAdiInstEntry( psInstance->psMap[ iIndex ].iInstance )->bNumOfElements;
               bStartIndex = 0;
            }
            else
            {
               bNumElem = psInstance->psMap[ iIndex ].bNumElem;
               bStartIndex = psInstance->psMap[ iIndex ].bElemStartIndex;
            }

            ABCC_SetMsgData32( psNewMessage,
                               psInstance->psMap[ iIndex ].iInstance |
                               ( bStartIndex << 16 ) |
                               ( bNumElem << 24 ),
                               iOctetOffset );

            iOctetOffset += ABP_BITS32_SIZEOF;
         }

         ABP_SetMsgResponse( psNewMessage, iOctetOffset );

         break;
      }
      } /* end of switch( ABCC_GetMsgCmdExt0( psNewMessage ) ) */

      break;
   }
   case ABP_ASM_CMD_WRITE_ASSEMBLY_DATA:
   {
      iMapSize = AD_GetMapSizeOctets( psInstance->psMap );

      if( ABCC_GetMsgDataSize( psNewMessage ) < iMapSize )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_NOT_ENOUGH_DATA );

         break;
      }
      else if( ABCC_GetMsgDataSize( psNewMessage ) > iMapSize )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );

         break;
      }

      iOctetOffset = 0;

      AD_WritePdMapFromBuffer( psInstance->psMap,
                               ABCC_GetMsgDataPtr( psNewMessage ),
                               &iOctetOffset );

      ABP_SetMsgResponse( psNewMessage, 0 );

      break;
   }
   case ABP_ASM_CMD_READ_ASSEMBLY_DATA:
   {
      iMapSize = AD_GetMapSizeOctets( psInstance->psMap );

      if( ABCC_CFG_MAX_MSG_SIZE < iMapSize )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_MSG_CHANNEL_TOO_SMALL );

         break;
      }

      iOctetOffset = 0;

      AD_WriteBufferFromPdMap( ABCC_GetMsgDataPtr( psNewMessage ),
                               &iOctetOffset,
                               psInstance->psMap );

      ABP_SetMsgResponse( psNewMessage, iOctetOffset );

      break;
   }
   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );

      break;
   }
}

/*------------------------------------------------------------------------------
** Processes commands to the Assembly mapping Object (Instance 0)
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void ObjectCommand( ABP_MsgType* psNewMessage )
{
   UINT16                   iStrLength;
   UINT16                   iIndex;
   UINT16                   iRspSize;
   const ASM_InstanceType** ppsInstance;

   /*
   ** This function processes commands to the Assembly Mapping Object (Instance 0).
   */
   switch ( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:

         /*
         ** The 'name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         iStrLength = (UINT16)strlen( asm_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, asm_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );

         break;

      case ABP_OA_REV:

         /*
         ** The 'revision' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, asm_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );

         break;

      case ABP_OA_NUM_INST:

         /*
         ** The 'Number of Instances' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, asm_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );

         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** The 'Highest Instance Number' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, asm_sObject.iHighestInstanceNo, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_HIGHEST_INST_DS );

         break;

      case ABP_ASM_OA_WRITE_PD_INST_LIST:

         /*
         ** The 'Write PD Instance list' attribute is requested.
         ** Copy the attribute to a response message.
         */

         ppsInstance = APPL_GetAsmInstances();
         iRspSize = 0;

         for( iIndex = 0; iIndex < APPL_GetNumAsmInstances(); iIndex++ )
         {
            if( ( (*ppsInstance)->lDescriptor & ( ABP_ASM_IA_DESC_WRITE_READ_MASK | ABP_ASM_IA_DESC_PD_MAPPABLE_MASK ) ) ==
                ( ABP_ASM_IA_DESC_WRITE | ABP_ASM_IA_DESC_PD_MAPPABLE ) )
            {
               ABCC_SetMsgData16( psNewMessage,
                                  iIndex + 1,
                                  iRspSize );

               iRspSize += ABP_UINT16_SIZEOF;
            }

            ppsInstance++;
         }

         ABP_SetMsgResponse( psNewMessage,
                             iRspSize );

         break;

      case ABP_ASM_OA_READ_PD_INST_LIST:

         /*
         ** The 'Read PD Instance list' attribute is requested.
         ** Copy the attribute to a response message.
         */

         ppsInstance = APPL_GetAsmInstances();
         iRspSize = 0;

         for( iIndex = 0; iIndex < APPL_GetNumAsmInstances(); iIndex++ )
         {
            if( ( (*ppsInstance)->lDescriptor & ( ABP_ASM_IA_DESC_WRITE_READ_MASK | ABP_ASM_IA_DESC_PD_MAPPABLE_MASK ) ) ==
                ( ABP_ASM_IA_DESC_READ | ABP_ASM_IA_DESC_PD_MAPPABLE ) )
            {
               ABCC_SetMsgData16( psNewMessage,
                                  iIndex + 1,
                                  iRspSize );

               iRspSize += ABP_UINT16_SIZEOF;
            }

            ppsInstance++;
         }

         ABP_SetMsgResponse( psNewMessage,
                             iRspSize );

         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );

         break;
      }
      break;
   }
   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );

      break;

   } /* End of switch( Command number ) */
}

/*******************************************************************************
** Public Services
********************************************************************************
*/

EXTFUNC BOOL ASM_GetData( UINT16 iInstance,
                          void* pxBuffer,
                          UINT16* piOctetOffset )
{
   const ASM_InstanceType* psInstance;

   if( ( iInstance == 0 ) ||
       ( iInstance > APPL_GetNumAsmInstances() ) )
   {
      return( FALSE );
   }

   psInstance = APPL_GetAsmInstances()[ iInstance - 1 ];

   AD_WriteBufferFromPdMap( pxBuffer,
                            piOctetOffset,
                            psInstance->psMap );

   return( TRUE );
}

void ASM_Init( void )
{
   const ASM_InstanceType** ppsInstance;
   UINT16 iIndex;

   asm_sObject.iNumberOfInstances = APPL_GetNumAsmInstances();
   asm_sObject.iHighestInstanceNo = APPL_GetNumAsmInstances();

   ppsInstance = APPL_GetAsmInstances();

   /*
   ** Verify that there are no dynamic assemblies.
   */
   for( iIndex = 0; iIndex < APPL_GetNumAsmInstances(); iIndex++ )
   {
      if( ( (*ppsInstance)->lDescriptor & ( ABP_ASM_IA_DESC_STATIC_DYNAMIC_MASK ) ) ==
          ABP_ASM_IA_DESC_DYNAMIC )
      {
         ABCC_ERROR( ABCC_SEV_FATAL, ABCC_EC_BAD_ASSEMBLY_INSTANCE, (UINT32)iIndex );
      }

      ppsInstance++;
   }
}

void ASM_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** Process the Assembly mapping object command
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Process the Assembly mapping instance command
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

/*******************************************************************************
** Tasks
********************************************************************************
*/

#endif /* ASM_OBJ_ENABLE */
