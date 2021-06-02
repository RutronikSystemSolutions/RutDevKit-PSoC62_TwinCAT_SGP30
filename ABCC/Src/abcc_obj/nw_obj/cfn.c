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
** Source file for the CC-Link IE Field Object.
********************************************************************************
********************************************************************************
*/

#include "abcc_td.h"
#include "abcc.h"
#include "abcc_obj_cfg.h"
#include "cfn.h"
#include "abp.h"
#include "abp_cfn.h"
#include "string.h"
#include "appl_abcc_handler.h"
#include "abcc_port.h"

#if CFN_OBJ_ENABLE

/*******************************************************************************
** Defines
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define CFN_OA_NAME_VALUE                          "ÅCC-Link IE Field Network"
#define CFN_OA_REV_VALUE                           1
#define CFN_OA_NUM_INST_VALUE                      1
#define CFN_OA_HIGHEST_INST_VALUE                  1

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure describing an CC-Link IE Field Object.
**------------------------------------------------------------------------------
*/
typedef struct cfn_Object
{
   const  char* pcName;
   UINT8  bRevision;
   UINT16 iNumberOfInstances;
   UINT16 iHighestInstanceNo;
}
cfn_ObjectType;

/*------------------------------------------------------------------------------
** Forward declarations
**------------------------------------------------------------------------------
*/
static void InstanceCommand( ABP_MsgType* psNewMessage );
static void ObjectCommand( ABP_MsgType* psNewMessage );

/*******************************************************************************
** Private Globals
********************************************************************************
*/

static const cfn_ObjectType cfn_sObject =
{
   CFN_OA_NAME_VALUE,                           /* Name.                                              */
   CFN_OA_REV_VALUE,                            /* Revision.                                          */
   CFN_OA_NUM_INST_VALUE,                       /* Number of instances.                               */
   CFN_OA_HIGHEST_INST_VALUE                    /* Highest instance number.                           */
};

/*******************************************************************************
** Public Services
********************************************************************************
*/

void CFN_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the CC-Link IE Field Object and it's Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** CC-Link IE Field object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** CC-Link IE Field instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Processes commands to CFN Instances
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
   /*
   ** This function processes commands to the CC-Link IE Field Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) != 1 )
   {
      /*
      ** The Instance does not exist.
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
#if CFN_IA_VENDOR_CODE_ENABLE
      case ABP_CFN_IA_VENDOR_CODE:

         /*
         ** Copy the 1st Instance 1 attribute (Vendor Code) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CFN_IA_VENDOR_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CFN_IA_VENDOR_CODE_DS );
         break;
#endif
#if CFN_IA_VENDOR_NAME_ENABLE
      case ABP_CFN_IA_VENDOR_NAME:
      {
         UINT16 iStrLength;

         /*
         ** Copy the 2nd Instance 1 attribute (Vendor Name) to the message.
         */
         iStrLength = (UINT16)strlen( CFN_IA_VENDOR_NAME_VALUE );
         ABCC_SetMsgString( psNewMessage, CFN_IA_VENDOR_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
#endif
#if CFN_IA_MODEL_TYPE_ENABLE
      case ABP_CFN_IA_MODEL_TYPE:

         /*
         ** Copy the 3rd Instance 1 attribute (Model Type) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, CFN_IA_MODEL_TYPE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CFN_IA_MODEL_TYPE_DS );
         break;
#endif
#if CFN_IA_MODEL_NAME_ENABLE
      case ABP_CFN_IA_MODEL_NAME:
      {
         UINT16 iStrLength;

         /*
         ** Copy the 4th Instance 1 attribute (Model Name) to the message.
         */
         iStrLength = (UINT16)strlen( CFN_IA_MODEL_NAME_VALUE );
         ABCC_SetMsgString( psNewMessage, CFN_IA_MODEL_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
#endif
#if CFN_IA_MODEL_CODE_ENABLE
      case ABP_CFN_IA_MODEL_CODE:

         /*
         ** Copy the 5th Instance 1 attribute (Model code) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, CFN_IA_MODEL_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CFN_IA_MODEL_CODE_DS );
         break;
#endif
#if CFN_IA_SW_VERSION_ENABLE
      case ABP_CFN_IA_SW_VERSION:

         /*
         ** Copy the 6th Instance 1 attribute (Version) to the message.
         */
         ABCC_SetMsgData8( psNewMessage, CFN_IA_SW_VERSION_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_CFN_IA_SW_VERSION_DS );
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

   } /* End of switch( Command number ) */
}

/*------------------------------------------------------------------------------
** Processes commands to the CFN Object (Instance 0)
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
   /*
   ** This function processes commands to the CC-Link IE Field Object (Instance 0).
   */
   switch ( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         /*
         ** Copy the attribute to a message.
         */
         iStrLength = (UINT16)strlen( cfn_sObject.pcName );
         ABCC_SetMsgString( psNewMessage, cfn_sObject.pcName, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }

      case ABP_OA_REV:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData8( psNewMessage, cfn_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_REV_DS );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, cfn_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_NUM_INST_DS );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, cfn_sObject.iHighestInstanceNo, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_OA_HIGHEST_INST_DS );
         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

      } /* End of switch( Attribute number ) */

      break;
   }
   case ABP_CFN_CMD_BUF_SIZE_NOTIF:

      /*
      ** Optionally implement the Buffer_Data_Size_Notification service here.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   default:

      /*
      ** Unsupported command.
      */
      ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_UNSUP_CMD );
      break;

   } /* End of switch( Command number ) */
}

#endif /* CFN_OBJ_ENABLE */
