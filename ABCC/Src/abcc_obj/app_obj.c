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
** COPYRIGHT NOTIFICATION (c) 2015 HMS Industrial Networks AB                 **
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
** Source file for the APP object, containing the Application Object and
** Application Instance.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abp.h"
#include "abcc.h"
#include "abcc_obj_cfg.h"
#include "app_obj.h"
#include "string.h"
#include "appl_abcc_handler.h"
#include "abcc_port.h"
#include "ad_obj.h"
#include "asm_obj.h"

#if APP_OBJ_ENABLE

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define APP_OA_NAME_VALUE                          "Application"
#define APP_OA_REV_VALUE                           2
#define APP_OA_NUM_INST_VALUE                      1
#define APP_OA_HIGHEST_INST_VALUE                  1

/*------------------------------------------------------------------------------
** Number of bits in an octet
**------------------------------------------------------------------------------
*/
#define APP_OCTET_NUM_BITS                         8

/*------------------------------------------------------------------------------
** Size of message header
**------------------------------------------------------------------------------
*/
#define ABCC_MSG_HEADER_TYPE_SIZEOF                12

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure describing an Application Object.
**------------------------------------------------------------------------------
*/
typedef struct app_Object
{
  const char* pcName;
  UINT8  bRevision;
  UINT16 iNumberOfInstances;
  UINT16 iHighestInstanceNo;
}
app_ObjectType;

/*------------------------------------------------------------------------------
** Structure describing an Application Data Instance.
**------------------------------------------------------------------------------
*/
#if APP_IA_CONFIGURED_ENABLE ||   \
    APP_IA_SUP_LANG_ENABLE ||     \
    APP_IA_PAR_CRTL_SUM_ENABLE || \
    APP_IA_HW_CONF_ADDR_ENABLE
typedef struct app_Instance
{
#if APP_IA_CONFIGURED_ENABLE
   BOOL   fConfigured;
#endif
#if APP_IA_SUP_LANG_ENABLE
   UINT8  abSupportedLanguages[ APP_IA_SUP_LANG_ARRAY_SIZE ];
#endif
#if APP_IA_PAR_CRTL_SUM_ENABLE
   UINT8  abParameterControlSum[ 16 ];
#endif
#if APP_IA_HW_CONF_ADDR_ENABLE
   BOOL   fHardwareConfigurableAddress;
#endif
}
app_InstanceType;
#endif

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

#if APP_IA_SUP_LANG_ENABLE
static const char* app_aacLanguages[ 5 ] =
{
   "English",
   "Deutsch",
   "Español",
   "Italiano",
   "Français"
};
#endif

static const app_ObjectType app_sObject =
{
   APP_OA_NAME_VALUE,                           /* Name.                                              */
   APP_OA_REV_VALUE,                            /* Revision.                                          */
   APP_OA_NUM_INST_VALUE,                       /* Number of instances.                               */
   APP_OA_HIGHEST_INST_VALUE                    /* Highest instance number.                           */
};

#if APP_IA_CONFIGURED_ENABLE ||   \
    APP_IA_SUP_LANG_ENABLE ||     \
    APP_IA_PAR_CRTL_SUM_ENABLE || \
    APP_IA_HW_CONF_ADDR_ENABLE
static app_InstanceType app_sInstance =
{
#if APP_IA_CONFIGURED_ENABLE
   APP_IA_CONFIGURED_VALUE,                     /* Configured.                                        */
#endif
#if APP_IA_SUP_LANG_ENABLE
   APP_IA_SUP_LANG_VALUE,                       /* Supported languages.                               */
#endif
#if APP_IA_PAR_CRTL_SUM_ENABLE
   APP_IA_PAR_CRTL_SUM_VALUE,                   /* Parameter control sum                              */
#endif
#if APP_IA_HW_CONF_ADDR_ENABLE
   APP_IA_HW_CONF_ADDR_VALUE,                   /* Hardware configurable address                      */
#endif
};

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
static UINT16 app_iSuppDataNotifCapabilities;
static ABP_MsgHeaderType app_sDataNotifHeader;
static UINT16 app_iDataNotifCounter = 0;
static BOOL app_fInhibitRespMsg;
#endif

#endif

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Called to check if the requested reset is permitted by the application.
**------------------------------------------------------------------------------
** Arguments:
**    bResetType           - Type of reset, see ABP_RESET_XXX defines.
**
** Returns:
**    BOOL                 - TRUE: Reset request is allowed.
**                           FALSE: Reset request NOT allowed.
**------------------------------------------------------------------------------
*/
static BOOL IsResetRequestAllowed( UINT8 bResetType )
{
   switch( (ABP_ResetType)bResetType )
   {
   case ABP_RESET_POWER_ON:
   case ABP_RESET_FACTORY_DEFAULT:
   case ABP_RESET_POWER_ON_FACTORY_DEFAULT:

      return( TRUE );

   default:

      return( FALSE );
   }
}

/*------------------------------------------------------------------------------
** This function will restore NVS parameters to their default values
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void RestoreToDefault( void )
{
   /*
   ** Todo: PORTING ALERT!
   ** Restore parameters stored in NVS to their default values
   */
}

/*------------------------------------------------------------------------------
** Function to set whether firmware is available or not in the candidate area.
** This function sets the value to a NVS.
**------------------------------------------------------------------------------
** Arguments:
**    fFirmwareAvailable    - TRUE/FALSE
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void SetCandidateFirmwareAvailable( BOOL fFirmwareAvailable )
{
   /*
   ** Todo: PORTING ALERT!
   ** Store value to NVS.
   */
   (void)fFirmwareAvailable;
}

/*------------------------------------------------------------------------------
** The function that processes the commands to the Application Instance.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage )
{
   BOOL8  fFirmwareAvailable;
   UINT16 iIndex;

   fFirmwareAvailable = 0;
   iIndex = 0;

   (void)(fFirmwareAvailable);
   (void)(iIndex);

   if( ABCC_GetMsgInstance( psNewMessage ) != 1 )
   {
      /*
      ** The requested instance does not exist.
      ** Respond with a error.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );

      return;
   }

   switch ( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if APP_IA_CONFIGURED_ENABLE
      case ABP_APP_IA_CONFIGURED:

         /*
         ** The 'configured' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, app_sInstance.fConfigured, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_APP_IA_CONFIGURED_DS );
         break;
#endif
#if APP_IA_SUP_LANG_ENABLE
      case ABP_APP_IA_SUP_LANG:

         /*
         ** The 'supported languages' attribute is requested.
         ** Copy the attribute to a response message.
         */
         for( iIndex = 0; iIndex < APP_IA_SUP_LANG_ARRAY_SIZE; iIndex++ )
         {
            ABCC_SetMsgData8( psNewMessage, app_sInstance.abSupportedLanguages[ iIndex ], iIndex );
         }
         ABP_SetMsgResponse( psNewMessage, APP_IA_SUP_LANG_ARRAY_SIZE );
         break;
#endif
#if APP_IA_SER_NUM_ENABLE
      case ABP_APP_IA_SER_NUM:

         /*
         ** The 'serial number' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData32( psNewMessage, APP_IA_SER_NUM_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_APP_IA_SER_NUM_DS );
         break;
#endif
#if APP_IA_PAR_CRTL_SUM_ENABLE
      case ABP_APP_IA_PAR_CRTL_SUM:

         /*
         ** The 'parameter control sum' attribute is requested.
         ** Copy the attribute to a response message.
         */
         for( iIndex = 0; iIndex < 16; iIndex++ )
         {
            ABCC_SetMsgData8( psNewMessage, app_sInstance.abParameterControlSum[ iIndex ], iIndex );
         }
         ABP_SetMsgResponse( psNewMessage, ABP_APP_IA_PAR_CRTL_SUM_DS );
         break;
#endif
#if APP_IA_FW_AVAILABLE_ENABLE
      case ABP_APP_IA_FW_AVAILABLE:

         /*
         ** The 'candidate firmware available' attribute is requested.
         ** Copy the attribute to a response message.
         */
         fFirmwareAvailable = APP_GetCandidateFwAvailable();
         ABCC_SetMsgData8( psNewMessage, fFirmwareAvailable, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_APP_IA_FW_AVAILABLE_DS );
         break;
#endif
#if APP_IA_HW_CONF_ADDR_ENABLE
      case ABP_APP_IA_HW_CONF_ADDR:

         /*
         ** The 'hardware configurable address' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, app_sInstance.fHardwareConfigurableAddress, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_APP_IA_HW_CONF_ADDR_DS );
         break;
#endif
      default:

         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;
   }
#if APP_IA_FW_AVAILABLE_ENABLE
   case ABP_CMD_SET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_APP_IA_FW_AVAILABLE:

         /*
         ** Set the 'candidate firmware available' attribute
         */
          ABCC_GetMsgData8( psNewMessage, &fFirmwareAvailable, 0 );
         SetCandidateFirmwareAvailable( fFirmwareAvailable );
         ABP_SetMsgResponse( psNewMessage, 0 );
         break;

      default:

         /*
         ** The attribute does not exist, or the attribute is not settable.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;
   }
#endif
#if APP_IA_SUP_LANG_ENABLE
   case ABP_CMD_GET_ENUM_STR:
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_APP_IA_SUP_LANG:
      {
         BOOL8  fLanguageSupported = FALSE;
         UINT16 iStrLength;

         for( iIndex = 0; iIndex < APP_IA_SUP_LANG_ARRAY_SIZE; iIndex++ )
         {
            if( app_sInstance.abSupportedLanguages[ iIndex ] == ABCC_GetMsgCmdExt1( psNewMessage ) )
            {
               /*
               ** Copy the ENUM STR to a message.
               */
               fLanguageSupported = TRUE;
               iStrLength = (UINT16)strlen( app_aacLanguages[ ABCC_GetMsgCmdExt1( psNewMessage ) ] );
               ABCC_SetMsgString( psNewMessage,
                                  app_aacLanguages[ ABCC_GetMsgCmdExt1( psNewMessage ) ],
                                  iStrLength,
                                  0 );
               ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
            }
         }

         if( !fLanguageSupported )
         {
            /*
            ** The ENUM value was out of range.
            */
            ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_1 );
         }
         break;
      }
      default:

         /*
         ** The attribute does not exist, or the attribute is not an ENUM.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;
      }
      break;
#endif

   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End switch( command number ) */
}

/*------------------------------------------------------------------------------
** The function that processes the commands to the Application Object
** (instance 0).
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void ObjectCommand( ABP_MsgType* psNewMessage )
{
   UINT16 iStrLength;
#if MQTT_OBJ_ENABLE
   ABP_MsgErrorCodeType eRspCode;
#endif

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
         iStrLength = (UINT16)strlen( app_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, app_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;

      case ABP_OA_REV:

         /*
         ** The 'revision' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, app_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** The 'Number of Instances' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, app_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** The 'Highest Instance Number' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, app_sObject.iHighestInstanceNo, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_HIGHEST_INST_DS );
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
   case ABP_APP_CMD_RESET_REQUEST:
   {
      /*
      ** Request a command reset.
      */
      if( !IsResetRequestAllowed( ABCC_GetMsgCmdExt1( psNewMessage ) ) )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_STATE );
      }
      else
      {
         ABP_SetMsgResponse( psNewMessage, 0 );
      }
      break;
   }
   case ABP_CMD_RESET:
   {
      /*
      ** Perform a reset.
      */
      APP_ProcResetRequest( ABCC_GetMsgCmdExt1( psNewMessage ) );
      ABP_SetMsgResponse( psNewMessage, 0 );
      break;
   }
   case ABP_APP_CMD_CHANGE_LANG_REQUEST:

      /*
      ** Optionally implement the Change Language Request service here.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   case ABP_APP_CMD_RESET_DIAGNOSTIC:

      /*
      ** Optionally implement the Reset Diagnostic service here.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
   case ABP_APP_CMD_GET_DATA_NOTIF:

      /*
      ** Get data notification service
      */

      /*
      ** Get the supported NotificationEntry capabilities
      ** and copy the message header.
      */
      ABCC_GetMsgData16( psNewMessage, &app_iSuppDataNotifCapabilities, 0 );
      ABCC_MemCpy( &app_sDataNotifHeader, &psNewMessage->sHeader, ABCC_MSG_HEADER_TYPE_SIZEOF );

      app_iDataNotifCounter++;
      eRspCode = APP_ON_DATA_NOTIFICATION_REQ_CBF( app_iDataNotifCounter );

      if( eRspCode == ABP_ERR_NO_ERROR )
      {
         /*
         ** Segmentation routine has sent response message,
         ** or decided to keep the request for future use.
         */
         app_fInhibitRespMsg = TRUE;
      }
      else
      {
         /*
         ** No data to publish or out of resources
         */
         app_fInhibitRespMsg = FALSE;
         ABP_SetMsgErrorResponse( psNewMessage, 1, eRspCode );
      }
      break;
#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

   default:

      /*
      ** Unsupported command.
      ** Respond with an error.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End switch( command number ) */

}

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE

/*------------------------------------------------------------------------------
** Handles callback supported by ABCC_StartServerRespSegmentationSession() for
** sending data notifications.
** The data notification is divided into three parts. The data before the value (head),
** the value data and the data after the value (tail).
** Each part is placed in a separate buffer in the APP_DataNotifDescType initiated by
** the corresponding prepare function. A pointer to an APP_DataNotifDescType instance
** is provided in the pxObject argument.
**
** This function implements a state machine controlling what part of the data
** notification should be provided at each call.
**
** State: APP_DATA_NOTIF_HEAD
**    Provides the head part i.e. the part before the value entry. This
**    buffer is a part of the APP_DataNotifDescType and initiated by the corresponding
**    DataNotifPrepare function.
**
** State: APP_DATA_NOTIF_VALUE
**    Provides the value entry buffer. The original buffer pointer is supplied by the
**    user and saved in the APP_DataNotifDescType.
**
** State: APP_DATA_NOTIF_TAIL
**    Provides the tail part i.e. payload after the value entry. This
**    buffer is a part of the APP_DataNotifDescType and initiated by the corresponding
**    DataNotifPrepare function.
**------------------------------------------------------------------------------
** Arguments:
**     See description of the ABCC_SegMsgHandlerNextBlockFuncType (abcc.h)
** Returns:
**     See description of the ABCC_SegMsgHandlerNextBlockFuncType (abcc.h)
**------------------------------------------------------------------------------
*/
static UINT8* GetNextDataBlock( void* pxObject, UINT32* plSize )
{
   UINT8 *pbDataBlock = NULL;
   APP_DataNotifDescType* psDataNotifDesc = (APP_DataNotifDescType*)pxObject;

   switch( psDataNotifDesc->eState )
   {
   case APP_DATA_NOTIF_HEAD:

      pbDataBlock = psDataNotifDesc->abHead;
      *plSize = psDataNotifDesc->lHeadSize;
      psDataNotifDesc->eState = APP_DATA_NOTIF_VALUE;
   break;

   case APP_DATA_NOTIF_VALUE:

      pbDataBlock = psDataNotifDesc->pbNotifEntryValue;
      *plSize = psDataNotifDesc->lNotifEntryValueSize;

      if( psDataNotifDesc->lTailSize == 0 )
      {
         psDataNotifDesc->eState = APP_DATA_NOTIF_DONE;
      }
      else
      {
         psDataNotifDesc->eState = APP_DATA_NOTIF_TAIL;
      }
      break;

   case APP_DATA_NOTIF_TAIL:

      pbDataBlock = psDataNotifDesc->abTail;
      *plSize = psDataNotifDesc->lTailSize;
      psDataNotifDesc->eState = APP_DATA_NOTIF_DONE;
      break;

   case APP_DATA_NOTIF_DONE:
   default:

      *plSize = 0;
      break;

   }
   return( pbDataBlock );
}

/*------------------------------------------------------------------------------
** Writes the dataset header including dataset identifier and network channels
** bitfield to a dataset buffer.
**------------------------------------------------------------------------------
** Arguments:
**    pbDatasetHeader    - Pointer to the beginning of the dataset buffer
**    piOctetOffset      - Pointer to octet offset in pbDatasetBuffer where the
**                         notification entry header shall be written. Will be
**                         incremented with the data size added to
**                         pbDatasetBuffer.
**    iDatasetIdentifier - Dataset identifier to write to the header
**    iNetworkChannels   - Network channels to write to the header (bitfield of
**                         ABP_APP_NW_CHANNELS_ definitions)
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
static void SetDatasetHeader( UINT8*  pbDatasetBuffer,
                              UINT16* piOctetOffset,
                              UINT16  iDatasetIdentifier,
                              UINT16  iNetworkChannels )
{
   /*
   ** Dataset identifier
   */
   ABCC_SetData16( pbDatasetBuffer, iDatasetIdentifier, *piOctetOffset );
   *piOctetOffset += ABP_UINT16_SIZEOF;

   /*
   ** Network channel
   */
   ABCC_SetData16( pbDatasetBuffer, iNetworkChannels, *piOctetOffset );
   *piOctetOffset += ABP_UINT16_SIZEOF;
}
#endif /* #if APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
/*------------------------------------------------------------------------------
** Writes a notification entry header to a notification entry buffer.
**------------------------------------------------------------------------------
** Arguments:
**    pbDatasetBuffer - Pointer to the dataset buffer.
**    piOctetOffset   - Pointer to octet offset in pbDatasetBuffer where the
**                      notification entry header shall be written. Will be
**                      incremented with the data size added to pbDatasetBuffer.
**    fSubIdentifier  - Set to true if sub identifier should be included.
**    fValue          - Set to true if a value should be included.
**    fTimestamp      - Set to true if a timestamp should be included.
**
** Returns:
**    The actual notification entry header written to piNotifEntryHeader after
**    masking requested bits with bits supported by the module. Only the bits
**    set in this bitfield are allowed to be written to the notification entry.
**------------------------------------------------------------------------------
*/
static UINT16 SetNotifEntryHeader( UINT8* pbDatasetBuffer,
                                   UINT16* piOctetOffset,
                                   BOOL fSubIdentifier,
                                   BOOL fValue,
                                   BOOL fTimestamp )
{
   UINT16 iNotifEntryHeader = 0;

   if( ( app_iSuppDataNotifCapabilities & ABP_APP_NOTIFENTRY_SUBIDENT_BIT ) && fSubIdentifier )
   {
      iNotifEntryHeader |= ABP_APP_NOTIFENTRY_SUBIDENT_BIT;
   }

   if( ( app_iSuppDataNotifCapabilities & ABP_APP_NOTIFENTRY_VALUE_BIT ) && fValue )
   {
      iNotifEntryHeader |= ABP_APP_NOTIFENTRY_VALUE_BIT;
   }

   if( ( app_iSuppDataNotifCapabilities & ABP_APP_NOTIFENTRY_TIMESTAMP_BIT ) && fTimestamp )
   {
      iNotifEntryHeader |= ABP_APP_NOTIFENTRY_TIMESTAMP_BIT;
   }

   /*
   ** Dataset identifier
   */
   ABCC_SetData16( pbDatasetBuffer, iNotifEntryHeader, *piOctetOffset );
   *piOctetOffset += ABP_UINT16_SIZEOF;

   return( iNotifEntryHeader );
}
#endif /* #if APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

/*******************************************************************************
** Public Services
********************************************************************************
*/

void APP_HwConfAddress( BOOL fhwConfAddress )
{
#if APP_IA_HW_CONF_ADDR_ENABLE
   app_sInstance.fHardwareConfigurableAddress = fhwConfAddress;
#endif
}

BOOL APP_GetCandidateFwAvailable( void )
{
   /*
   ** Todo: PORTING ALERT!
   ** Read value from NVS.
   */
   return( FALSE );
}

void APP_ProcResetRequest( UINT8 bResetType )
{
   switch( bResetType )
   {
   case ABP_RESET_FACTORY_DEFAULT:
      RestoreToDefault();
      break;

   case ABP_RESET_POWER_ON_FACTORY_DEFAULT:
      RestoreToDefault();
      APPL_Reset();
      break;

   case ABP_RESET_POWER_ON:
      APPL_Reset();
      break;

   default:
      break;
   }
}

void APP_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** Process the Application data Object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Process the Application data Instance Command.
      */
      InstanceCommand( psNewMessage );
   }

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
   if( app_fInhibitRespMsg == FALSE )
   {
      ABCC_SendRespMsg( psNewMessage );
   }
   else
   {
      /*
      ** Reset flag to send normal response for next command
      */
      app_fInhibitRespMsg = FALSE;
   }
#else
   ABCC_SendRespMsg( psNewMessage );
#endif
}

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
void APP_PrepareDataNotifSingleAdi( APP_DataNotifDescType* psDataNotifDesc,
                                    UINT8* pbAdiValueBuffer,
                                    UINT16 iValueSize,
                                    UINT16 iNwChannel,
                                    UINT16 iInstance,
                                    UINT8 bStartIndex,
                                    UINT8 bNumElements,
                                    UINT32 lTimestampLow,
                                    UINT32 lTimestampHigh )
{
   UINT16 iOctetOffset = 0;
   UINT16 iSuppNotifEntryHeaderBits;

   psDataNotifDesc->eState = APP_DATA_NOTIF_HEAD;
   psDataNotifDesc->eDataset = ABP_APP_DATASET_SINGLEADI;
   psDataNotifDesc->pbNotifEntryValue = pbAdiValueBuffer;
   psDataNotifDesc->lNotifEntryValueSize = iValueSize;

   SetDatasetHeader( psDataNotifDesc->abHead, &iOctetOffset,
                     iInstance, iNwChannel );

   /*
   ** NotificationEntry - Header
   ** Bit 0  - Sub identifier
   ** Bit 1  - Value
   ** Bit 2  - Timestamp
   */
   iSuppNotifEntryHeaderBits = SetNotifEntryHeader( psDataNotifDesc->abHead,
                                                    &iOctetOffset,
                                                    ( bNumElements > 0 ),
                                                    TRUE,
                                                    ( lTimestampLow | lTimestampHigh ) != 0 );

   /*
   ** NotificatioNEntry - Sub identifier
   */
   if( iSuppNotifEntryHeaderBits & ABP_APP_NOTIFENTRY_SUBIDENT_BIT )
   {
      ABCC_SetData8( psDataNotifDesc->abHead, bStartIndex, iOctetOffset );
      iOctetOffset += ABP_UINT8_SIZEOF;
      ABCC_SetData8( psDataNotifDesc->abHead, bNumElements, iOctetOffset );
      iOctetOffset += ABP_UINT8_SIZEOF;
   }

   /*
   ** NotificationEntry - Value length
   */
   if( iSuppNotifEntryHeaderBits & ABP_APP_NOTIFENTRY_VALUE_BIT )
   {
      ABCC_SetData32( psDataNotifDesc->abHead, psDataNotifDesc->lNotifEntryValueSize, iOctetOffset );
      iOctetOffset += ABP_UINT32_SIZEOF;
   }

   psDataNotifDesc->lHeadSize = iOctetOffset;
   iOctetOffset = 0;
   /*
   ** NotificationEntry - Timestamp
   */
   if( iSuppNotifEntryHeaderBits & ABP_APP_NOTIFENTRY_TIMESTAMP_BIT )
   {
      ABCC_SetData32( psDataNotifDesc->abTail, lTimestampLow, iOctetOffset);
      iOctetOffset +=  ABP_UINT32_SIZEOF;
      ABCC_SetData32( psDataNotifDesc->abTail, lTimestampHigh, iOctetOffset);
      iOctetOffset +=  ABP_UINT32_SIZEOF;
   }
   psDataNotifDesc->lTailSize = iOctetOffset;
}

#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE && ASM_OBJ_ENABLE
void APP_PrepareDataNotifAsmInst( APP_DataNotifDescType* psDataNotifDesc,
                                  UINT8* pabAsmValueBuffer,
                                  UINT16 iValueSize,
                                  UINT16 iNwChannel,
                                  UINT16 iInstance,
                                  UINT32 lTimestampLow,
                                  UINT32 lTimestampHigh )
{
   UINT16 iSuppNotifEntryHeaderBits;
   UINT16 iOctetOffset = 0;

   psDataNotifDesc->eState = APP_DATA_NOTIF_HEAD;
   psDataNotifDesc->eDataset = ABP_APP_DATASET_ASSEMBLYMAPPING;
   psDataNotifDesc->pbNotifEntryValue = pabAsmValueBuffer;
   psDataNotifDesc->lNotifEntryValueSize = iValueSize;

   SetDatasetHeader( psDataNotifDesc->abHead, &iOctetOffset,
                     iInstance, iNwChannel );

   iSuppNotifEntryHeaderBits = SetNotifEntryHeader( psDataNotifDesc->abHead,
                                                    &iOctetOffset,
                                                    FALSE,
                                                    TRUE,
                                                    ( ( lTimestampLow | lTimestampHigh ) != 0 ) );

   if( iSuppNotifEntryHeaderBits | ABP_APP_NOTIFENTRY_VALUE_BIT )
   {

      ABCC_SetData32( psDataNotifDesc->abHead,
                      iValueSize,
                      iOctetOffset );
      iOctetOffset += ABP_UINT32_SIZEOF;

   }

   psDataNotifDesc->lHeadSize = iOctetOffset;
   iOctetOffset = 0;

   if( iSuppNotifEntryHeaderBits | ABP_APP_NOTIFENTRY_TIMESTAMP_BIT )
   {
      ABCC_SetData32( psDataNotifDesc->abTail, lTimestampLow, iOctetOffset );
      iOctetOffset += ABP_UINT32_SIZEOF;
      ABCC_SetData32( psDataNotifDesc->abTail, lTimestampHigh, iOctetOffset );
      iOctetOffset += ABP_UINT32_SIZEOF;
   }

   psDataNotifDesc->lTailSize = iOctetOffset;
}
#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
void APP_PrepareDataNotifTransparent( APP_DataNotifDescType* psDataNotifDesc,
                                      UINT8* pxTransparentPayload,
                                      UINT16 iPayloadSize,
                                      UINT16 iNwChannel,
                                      UINT16 iIdentifierTransparent )
{

   UINT16 iSuppNotifEntryHeaderBits;
   UINT16 iOctetOffset = 0;

   psDataNotifDesc->eState = APP_DATA_NOTIF_HEAD;
   psDataNotifDesc->eDataset = ABP_APP_DATASET_TRANSPARENT;
   psDataNotifDesc->pbNotifEntryValue = pxTransparentPayload;
   psDataNotifDesc->lNotifEntryValueSize = iPayloadSize;

   SetDatasetHeader( psDataNotifDesc->abHead, &iOctetOffset,
                     iIdentifierTransparent, iNwChannel );

   /*
   ** NotificationEntry - Header
   ** Bit 1  - Value
   */
   iSuppNotifEntryHeaderBits = SetNotifEntryHeader( psDataNotifDesc->abHead,
                                                    &iOctetOffset,
                                                    FALSE,
                                                    TRUE,
                                                    FALSE );

   /*
   ** NotificationEntry - Value
   */
   if( iSuppNotifEntryHeaderBits & ABP_APP_NOTIFENTRY_VALUE_BIT )
   {
      /*
      ** NotificationEntry - Value length
      */
      ABCC_SetData32( psDataNotifDesc->abHead, iPayloadSize, iOctetOffset );
      iOctetOffset += ABP_UINT32_SIZEOF;
   }

   psDataNotifDesc->lHeadSize = iOctetOffset;
   psDataNotifDesc->lTailSize = 0;
}
#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE

INT16  APP_SendDataNotification( APP_DataNotifDescType* psDataNotifDesc,
                                 ABCC_SegMsgHandlerDoneFuncType pnDone )
{
   ABCC_ErrorCodeType eRspCode;
   /*
   ** Check available Data Notification resources
   */
   if( app_iDataNotifCounter > 0 )
   {
      app_iDataNotifCounter--;
   }
   else
   {
      return( -1 );
   }

   /*
   ** Send a segmented response message to the ABCC
   */
   eRspCode = ABCC_StartServerRespSegmentationSession( &app_sDataNotifHeader,
                                                       psDataNotifDesc->eDataset,
                                                       NULL,
                                                       0,
                                                       &GetNextDataBlock,
                                                       pnDone,
                                                       psDataNotifDesc );

   if( eRspCode != ABCC_EC_NO_ERROR )
   {
      /*
      ** Segmentation routine failed
      */
      return( -1 );
   }

   return( app_iDataNotifCounter );
}
#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
UINT8* APP_GetDataNotifValuePtr( const APP_DataNotifDescType* psDataNotifDesc )
{
   return( psDataNotifDesc->pbNotifEntryValue );
}
#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */
#endif /* APP_OBJ_ENABLE */
