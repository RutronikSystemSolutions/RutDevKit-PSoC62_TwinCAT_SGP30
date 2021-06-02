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
** COPYRIGHT NOTIFICATION (c) 2018 HMS Industrial Networks AB                 **
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
** User implementations of callback functions of Anybus objects implementations.
********************************************************************************
********************************************************************************
*/

#include "abcc.h"
#include "abcc_drv_cfg.h"
#include "abcc_obj_cfg.h"
#include "appl_adi_config.h"
#include "mqtt_obj.h"
#include "app_obj.h"
#include "ad_obj.h"
#include "abp.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Data Notification defines
**------------------------------------------------------------------------------
*/
#define APP_DATANOTIF_HEADER_DATA_SIZE_MAX   ( 18 )

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/
#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
static BOOL appl_fNoDataToPublish = FALSE;
static INT16 app_iDataNotifResources = 0;
static char app_acTransparentPayload[] = "Transparent payload";
#endif

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/
#if MQTT_OBJ_ENABLE && MQTT_CMD_PUBLISH_CFG_ENABLE
ABP_MsgErrorCodeType MQTT_GetPublishConfigurationCfn( ABP_AppDatasetType eDatasetType,
                                                      UINT16 iDatasetIdentifier,
                                                      BOOL* pfRetainBit,
                                                      const char** ppcTopicString )
{
   /*
   ** PORTING ALERT!
   ** This is a simple example implementation of this callback.
   ** If MQTT_CMD_PUBLISH_CFG_ENABLE is enabled this function should be ported
   ** by user.
   */

   static const char* const pacTopicStrings[ ABP_APP_DATASET_TRANSPARENT + 1 ] = { "singleadi/usertopic",
                                                                                   "modulardevice/usertopic",
                                                                                   "transparent/usertopic" };

   *pfRetainBit = FALSE;

   switch( eDatasetType )
   {
   case ABP_APP_DATASET_SINGLEADI:

      if( iDatasetIdentifier == APPL_asAdiEntryList[ 0 ].iInstance )
      {
         *ppcTopicString = pacTopicStrings[ eDatasetType ];
      }
      else
      {
         return( ABP_ERR_OUT_OF_RANGE );
      }

      break;

   case ABP_APP_DATASET_ASSEMBLYMAPPING:

      return( ABP_ERR_INV_CMD_EXT_0 );
      break;

   case ABP_APP_DATASET_TRANSPARENT:

      if( iDatasetIdentifier == 1 )
      {
         *ppcTopicString = pacTopicStrings[ eDatasetType ];
      }
      else
      {
         return( ABP_ERR_OUT_OF_RANGE );
      }

      break;

   default:

      ABCC_ERROR( ABCC_SEV_FATAL, ABP_ERR_OUT_OF_RANGE, eDatasetType );
      break;
   }

   return( ABP_ERR_NO_ERROR );
}
#endif /* #if MQTT_OBJ_ENABLE && MQTT_CMD_PUBLISH_CFG_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
ABP_MsgErrorCodeType APP_OnDataNotificationReqCfn( UINT16 iDataNotifCounter )
{
   /*
   ** PORTING ALERT!
   ** This is a simple example implementation of this callback.
   ** If APP_CMD_GET_DATA_NOTIFICATION_ENABLE is enabled
   ** this function should be ported by user.
   **
   ** The provided example code will do three data notification
   ** publications, one for each dataset type (excluding assembly data). Further, the data
   ** notification response buffer is populated within this function,
   ** this could typically be done outside of this function
   ** e.g. triggered by an updated ADI value.
   */

   static UINT16 aiValueBuffer[ 256 >> 1 ];
   static APP_DataNotifDescType sDataNotifDesc;
   static ABP_AppDatasetType eExample = ABP_APP_DATASET_SINGLEADI;
   const struct AD_AdiEntry* psAdiEntry;
   UINT16 iNetworkChannel;
   UINT16 iInstance;
   UINT16 iBufferSize = 0;
   UINT8 bStartIndex;
   UINT8 bNumElements;
   UINT32 lTimestampLow;
   UINT32 lTimestampHigh;
   INT16 iIdentifierTransparent;

   app_iDataNotifResources = iDataNotifCounter;

   if( appl_fNoDataToPublish )
   {
      /*
      ** No data to publish and no support to hold the request
      */
      return( ABP_ERR_DATA_NOT_AVAILABLE );
   }

   if( eExample == ABP_APP_DATASET_SINGLEADI )
   {
      /*
      ** EXAMPLE: Publish the first ADI instance together with timestamp
      */
      iNetworkChannel = ABP_APP_NW_CHANNELS_MQTT_BIT;
      iInstance = APPL_asAdiEntryList[ 0 ].iInstance;

      psAdiEntry = AD_GetAdiInstEntry( iInstance );

      if( psAdiEntry != NULL )
      {
         AD_GetAdiValue( psAdiEntry,
                         (void*)aiValueBuffer,
                         psAdiEntry->bNumOfElements,
                         0,
                         &iBufferSize,
                         TRUE );


         bStartIndex = 0x0;
         bNumElements = 0x0;
         lTimestampLow = 0x5d8ce800;
         lTimestampHigh = 0x57dc8;

         APP_PrepareDataNotifSingleAdi( &sDataNotifDesc,
                                        (UINT8*)aiValueBuffer,
                                        ( iBufferSize + 7 ) / 8,
                                        iNetworkChannel,
                                        iInstance,
                                        bStartIndex,
                                        bNumElements,
                                        lTimestampLow,
                                        lTimestampHigh );
      }

      eExample = ABP_APP_DATASET_TRANSPARENT;
   }
   else if( eExample == ABP_APP_DATASET_TRANSPARENT )
   {
      /*
      ** EXAMPLE: Publish transparent payload
      */
      iNetworkChannel = ABP_APP_NW_CHANNELS_MQTT_BIT;
      iIdentifierTransparent = 0x0001;
      iBufferSize = 19;

      /*
      ** Copy the string to pack it properly on 16 bit char platforms.
      */
      ABCC_SetString( aiValueBuffer, app_acTransparentPayload, iBufferSize, 0 );

      APP_PrepareDataNotifTransparent( &sDataNotifDesc,
                                       (void*)aiValueBuffer,
                                       iBufferSize,
                                       iNetworkChannel,
                                       iIdentifierTransparent );

      /*
      ** Only run the examples once
      */
      eExample = (ABP_AppDatasetType)0xFF;
   }
   else
   {
      eExample = (ABP_AppDatasetType)0xFF;
   }

   /*
   ** Send data notification
   */
   if( iBufferSize != 0 )
   {
      app_iDataNotifResources = APP_SendDataNotification( &sDataNotifDesc, NULL );

      if( app_iDataNotifResources < 0 )
      {
         /*
         ** Data notification failed
         */
         return( ABP_ERR_NO_RESOURCES );
      }
      else
      {
         return( ABP_ERR_NO_ERROR );
      }
   }
   else
   {
      return( ABP_ERR_DATA_NOT_AVAILABLE );
   }
}

#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

/*******************************************************************************
** Tasks
********************************************************************************
*/
