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
** Source file for the MQTT object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc.h"
#include "abcc_obj_cfg.h"
#include "appl_abcc_handler.h"
#include "abcc_port.h"
#include "abp_mqtt.h"
#include "mqtt_obj.h"

#if MQTT_OBJ_ENABLE

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define MQTT_OA_NAME_VALUE                        "MQTT"
#define MQTT_OA_REV_VALUE                         1
#define MQTT_OA_NUM_INST_VALUE                    1
#define MQTT_OA_HIGHEST_INST_VALUE                1

/*------------------------------------------------------------------------------
** Publish configuration
**------------------------------------------------------------------------------
*/
#define MQTT_TOPIC_STR_BUFFER_SIZE                40

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure describing an MQTT object.
**------------------------------------------------------------------------------
*/
typedef struct mqtt_Object
{
   const char* pcName;
   UINT8  bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
}
mqtt_ObjectType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

static const mqtt_ObjectType mqtt_sObject =
{
   MQTT_OA_NAME_VALUE,                            /* Name.                                      */
   MQTT_OA_REV_VALUE,                             /* Revision.                                  */
   MQTT_OA_NUM_INST_VALUE,                        /* Number of instances.                       */
   MQTT_OA_HIGHEST_INST_VALUE                     /* Highest instance number.                   */
};

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** The function that processes the commands to the MQTT Instance.
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
#if MQTT_IA_LAST_WILL_ENABLE
   char*  pcStr;
   UINT16 iTopicStrLength;
   UINT16 iMessageStrLength;
   UINT16 iMsgRespSize;
#endif

   if( ABCC_GetMsgInstance( psNewMessage ) != 1 )
   {
      /*
      ** The requested instance does not exist.
      ** Respond with a error.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_INST );

      return;
   }

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if MQTT_IA_MODE_ENABLE
      case ABP_MQTT_IA_MODE:

         /*
         ** The 'MQTT mode' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, MQTT_IA_MODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_MQTT_IA_MODE_DS );
         break;
#endif

#if MQTT_IA_LAST_WILL_ENABLE
      case APB_MQTT_IA_LAST_WILL:

         /*
         ** The 'MQTT last will' attribute is requested.
         ** Copy the attribute to a response message
         */

         /*
         ** Last will QoS and retain
         */
         ABCC_SetMsgData8( psNewMessage, MQTT_IA_LAST_WILL_QOS, ABP_MQTT_LASTWILL_QOS_OFFSET );
         ABCC_SetMsgData8( psNewMessage, MQTT_IA_LAST_WILL_RETAIN, ABP_MQTT_LASTWILL_RETAIN_OFFSET );

         /*
         ** Last will topic
         */
         pcStr = MQTT_IA_LAST_WILL_TOPIC_STR;
         iTopicStrLength = (UINT16)strnlen( pcStr, 128 );
         ABCC_SetMsgData16( psNewMessage, iTopicStrLength, ABP_MQTT_LASTWILL_TOPIC_LENGTH_OFFSET );
         ABCC_SetMsgString( psNewMessage, pcStr, iTopicStrLength, ABP_MQTT_LASTWILL_TOPIC_STR_OFFSET );

         /*
         ** Last will message
         */
         pcStr = MQTT_IA_LAST_WILL_MESSAGE_STR;
         iMessageStrLength = (UINT16)strnlen( pcStr, 256 );
         ABCC_SetMsgData16( psNewMessage, iMessageStrLength, ABP_MQTT_LASTWILL_MESSAGE_LENGTH_BASE_OFFSET + iTopicStrLength );
         ABCC_SetMsgString( psNewMessage, pcStr, iMessageStrLength, ABP_MQTT_LASTWILL_MESSSAGE_STR_BASE_OFFSET + iTopicStrLength );

         iMsgRespSize = ABP_UINT8_SIZEOF + ABP_BOOL_SIZEOF + ABP_UINT16_SIZEOF + iTopicStrLength + ABP_UINT16_SIZEOF + iMessageStrLength;
         ABP_SetMsgResponse( psNewMessage, iMsgRespSize );
         break;
#endif
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
   } /* End switch( command number ) */
}

/*------------------------------------------------------------------------------
** The function that processes the commands to the MQTT Object (instance 0).
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
#if MQTT_CMD_PUBLISH_CFG_ENABLE
   UINT16 iDatasetIdentifier;
   BOOL   fRetainBit;
   const char*  pcTopicString;
   ABP_AppDatasetType eDatasetType;
   ABP_MsgErrorCodeType eRspCode;
#endif

   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
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
         iStrLength = (UINT16)strlen( mqtt_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, mqtt_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;

      case ABP_OA_REV:

         /*
         ** The 'revision' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, mqtt_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** The 'Number of Instances' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, mqtt_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** The 'Highest Instance Number' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, mqtt_sObject.iHighestInstanceNo, 0 );
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
#if MQTT_CMD_PUBLISH_CFG_ENABLE
   case ABP_MQTT_CMD_GET_PUBLISH_CONFIGURATION:
   {
      if( ( eDatasetType = ABCC_GetMsgCmdExt0( psNewMessage ) ) > ABP_APP_DATASET_TRANSPARENT )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
      }
      else if( ABCC_GetMsgDataSize( psNewMessage ) < ABP_UINT16_SIZEOF )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_NOT_ENOUGH_DATA );
      }
      else if( ABCC_GetMsgDataSize( psNewMessage ) > ABP_UINT16_SIZEOF )
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_TOO_MUCH_DATA );
      }
      else
      {
         ABCC_GetMsgData16( psNewMessage, &iDatasetIdentifier, 0 );
         eRspCode = MQTT_GetPublishConfigurationCfn( eDatasetType,
                                                     iDatasetIdentifier,
                                                     &fRetainBit,
                                                     &pcTopicString );

         if( eRspCode == ABP_ERR_NO_ERROR )
         {
            ABP_SetMsgErrorResponse( psNewMessage, 1, eRspCode );
         }
         else
         {
            ABCC_SetMsgData8( psNewMessage, fRetainBit, ABP_MQTT_GETPUBCONF_RETAIN_OFFSET );
            ABCC_SetMsgString( psNewMessage, pcTopicString, (UINT16)strlen( pcTopicString ),
                               ABP_MQTT_GETPUBCONF_TOPIC_OFFSET );
            ABP_SetMsgResponse( psNewMessage, 1 + (UINT16)strlen( pcTopicString ) );
         }
      }

      break;
   }
#endif /* #if MQTT_CMD_PUBLISH_CFG_ENABLE */
   default:

      /*
      ** Unsupported command.
      ** Respond with an error.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;
   } /* End switch( command number ) */
}

/*******************************************************************************
** Public Services
********************************************************************************
*/

void MQTT_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** Process the MQTT Object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Process the MQTT Instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

#endif /* MQTT_OBJ_ENABLE */
