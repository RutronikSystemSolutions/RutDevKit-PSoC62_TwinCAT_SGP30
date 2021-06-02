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
** Source file for the OPC UA object.
********************************************************************************
********************************************************************************
*/
#include "abcc_td.h"
#include "abcc.h"
#include "abcc_obj_cfg.h"
#include "appl_abcc_handler.h"
#include "abcc_port.h"
#include "abp_opcua.h"

#if OPCUA_OBJ_ENABLE

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define OPCUA_OA_NAME_VALUE                        "OPC UA"
#define OPCUA_OA_REV_VALUE                         1
#define OPCUA_OA_NUM_INST_VALUE                    1
#define OPCUA_OA_HIGHEST_INST_VALUE                1

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure describing an OPC UA object.
**------------------------------------------------------------------------------
*/
typedef struct opcua_Object
{
   const char* pcName;
   UINT8  bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
}
opcua_ObjectType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

static const opcua_ObjectType opcua_sObject =
{
   OPCUA_OA_NAME_VALUE,                            /* Name.                                      */
   OPCUA_OA_REV_VALUE,                             /* Revision.                                  */
   OPCUA_OA_NUM_INST_VALUE,                        /* Number of instances.                       */
   OPCUA_OA_HIGHEST_INST_VALUE                     /* Highest instance number.                   */
};

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** The function that processes the commands to the OPC UA Instance.
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
#if OPCUA_IA_APPLICATION_URI_ENABLE        ||                                  \
    OPCUA_IA_VENDOR_NAMESPACE_URI_ENABLE   ||                                  \
    OPCUA_IA_DEVICE_TYPE_NAME_ENABLE       ||                                  \
    OPCUA_IA_DEVICE_INST_NAME_ENABLE       ||                                  \
    OPCUA_IA_PRODUCT_URI_ENABLE
   char*  pcStr;
   UINT16 iStrLength;
#endif

   if ( ABCC_GetMsgInstance( psNewMessage ) != 1 )
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
      switch ( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if OPCUA_IA_MODEL_ENABLE
      case ABP_OPCUA_IA_MODEL:

         /*
         ** The 'OPC UA model' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, OPCUA_IA_MODEL_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OPCUA_IA_MODEL_DS );
         break;
#endif
#if OPCUA_IA_APPLICATION_URI_ENABLE
      case ABP_OPCUA_IA_APPLICATION_URI:

         /*
         ** The 'Application/Local namespace URI' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_APPLICATION_URI_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_APPLICATION_URI_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
#endif
#if OPCUA_IA_VENDOR_NAMESPACE_URI_ENABLE
      case ABP_OPCUA_IA_VENDOR_NAMESPACE_URI:

         /*
         ** The 'Vendor namespace URI' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_VENDOR_NAMESPACE_URI_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_VENDOR_NAMESPACE_URI_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
#endif
#if OPCUA_IA_DEVICE_TYPE_NAME_ENABLE
      case ABP_OPCUA_IA_DEVICE_TYPE_NAME:

         /*
         ** The 'DeviceType name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_DEVICE_TYPE_NAME_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_DEVICE_TYPE_NAME_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
#endif
#if OPCUA_IA_DEVICE_INST_NAME_ENABLE
      case ABP_OPCUA_IA_DEVICE_INST_NAME:

         /*
         ** The 'Device instance name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_DEVICE_INST_NAME_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_DEVICE_INST_NAME_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;
#endif
#if OPCUA_IA_PRODUCT_URI_ENABLE
      case ABP_OPCUA_IA_PRODUCT_URI:

         /*
         ** The 'Product URI' attribute is requested.
         ** Copy the attribute to a response message.
         */
         pcStr = OPCUA_IA_PRODUCT_URI_VALUE;
         iStrLength = (UINT16)strnlen( pcStr, ABP_OPCUA_IA_PRODUCT_URI_MAX_DS );

         ABCC_SetMsgString( psNewMessage, pcStr, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
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
** The function that processes the commands to the OPC UC Object (instance 0).
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

   switch ( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch ( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:

         /*
         ** The 'name' attribute is requested.
         ** Copy the attribute to a response message.
         */
         iStrLength = (UINT16)strlen( opcua_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, opcua_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, iStrLength );
         break;

      case ABP_OA_REV:

         /*
         ** The 'revision' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, opcua_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** The 'Number of Instances' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, opcua_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** The 'Highest Instance Number' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData16( psNewMessage, opcua_sObject.iHighestInstanceNo, 0 );
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

void OPCUA_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   if ( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** Process the OPC UAC Object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** Process the OPC UA Instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

#endif /* OPCUA_OBJ_ENABLE */
