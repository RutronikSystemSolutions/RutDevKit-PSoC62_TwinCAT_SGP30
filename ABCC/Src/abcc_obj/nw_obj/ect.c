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
** Source file for the EtherCAT Object.
********************************************************************************
********************************************************************************
*/

#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "abcc_obj_cfg.h"
#include "ect.h"
#include "abp.h"
#include "abp_ect.h"
#include "string.h"
#include "appl_abcc_handler.h"
#include "abcc_port.h"

#if ECT_OBJ_ENABLE

/*******************************************************************************
** Defines
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Object attribute values
**------------------------------------------------------------------------------
*/
#define ECT_OA_NAME_VALUE                          "EtherCAT"
#define ECT_OA_REV_VALUE                           1
#define ECT_OA_NUM_INST_VALUE                      1
#define ECT_OA_HIGHEST_INST_VALUE                  1

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure describing the EtherCAT Instance 1 attribute 13 ADI Translation
**------------------------------------------------------------------------------
*/
#if ECT_IA_ADI_TRANS_ENABLE
typedef struct ect_Instance1AdiTranslation
{
   UINT16 iADIInstanceNumber;
   UINT16 iADIObjectIndex;
}
ect_Instance1AdiTranslation;
#endif

/*------------------------------------------------------------------------------
** Structure describing the EtherCAT Instance 1 attribute 15 Object Subindex
** Translation
**------------------------------------------------------------------------------
*/
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
typedef struct ect_Instance1ObjSubTrans
{
   UINT16 iADIInstanceNumber;
   UINT16 iADIObjectIndex;
   UINT8  bADIObjectIndexSubindex;
}
ect_Instance1ObjSubTrans;
#endif

/*------------------------------------------------------------------------------
** Structure describing the EtherCAT Instance 1 attributes.
**------------------------------------------------------------------------------
*/
#if( ECT_IA_ENUM_ADIS_ENABLE || ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE || ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE || ECT_IA_ADI_TRANS_ENABLE || ECT_IA_OBJ_SUB_TRANS_ENABLE || ECT_IA_ENABLE_FOE_ENABLE || ECT_IA_ENABLE_EOE_ENABLE || ECT_IA_SET_DEV_ID_AS_CSA_ENABLE || ECT_IA_STATE_TIMEOUTS_ENABLE || ECT_IA_COMP_IDENT_LISTS_ENABLE || ECT_IA_CLEAR_IDENT_AL_STS_ENABLE )
typedef struct ect_Instance
{
#if ECT_IA_ENUM_ADIS_ENABLE
   UINT16 aiEnumADIs[ ECT_IA_ENUM_ADIS_ARRAY_SIZE ];
#endif
#if ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE
   UINT16 aiWrPdAssyInstTrans[ ECT_IA_WR_PD_ASSY_INST_TRANS_SIZE ];
#endif
#if ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE
   UINT16 aiRdPdAssyInstTrans[ ECT_IA_RD_PD_ASSY_INST_TRANS_SIZE ];
#endif
#if ECT_IA_ADI_TRANS_ENABLE
   ect_Instance1AdiTranslation asADITrans[ ECT_IA_ADI_TRANS_SIZE ];
#endif
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
   ect_Instance1ObjSubTrans asObjSubTrans[ ECT_IA_OBJ_SUB_TRANS_SIZE ];
#endif
#if ECT_IA_ENABLE_FOE_ENABLE
   BOOL fEnableFoE;
#endif
#if ECT_IA_ENABLE_EOE_ENABLE
   BOOL fEnableEoE;
#endif
#if ECT_IA_SET_DEV_ID_AS_CSA_ENABLE
   BOOL fSetDevIDAsCSA;
#endif
#if ECT_IA_STATE_TIMEOUTS_ENABLE
   UINT32 alStateTransTimeouts[ 4 ];
#endif
#if ECT_IA_COMP_IDENT_LISTS_ENABLE
   BOOL fCompIDLists;
#endif
#if ECT_IA_CLEAR_IDENT_AL_STS_ENABLE
   BOOL fClearIdAlSts;
#endif
}
ect_InstanceType;
#endif /* if( ECT_IA_ENUM_ADIS_ENABLE... */

/*------------------------------------------------------------------------------
** Structure describing an EtherCAT Object.
**------------------------------------------------------------------------------
*/
typedef struct ect_Object
{
    const  char* pcName;
    UINT8  bRevision;
    UINT16 iNumberOfInstances;
    UINT16 iHighestInstanceNo;
}
ect_ObjectType;

/*******************************************************************************
** Private Globals
********************************************************************************
*/
#if ECT_IA_ETHERCAT_STATE_ENABLE
   static UINT8 ect_bEtherCATState;
#endif

#if ECT_IA_FSOE_STATUS_IND_ENABLE
   static UINT8 ect_eFSoEStatusIndicator;
#endif

static const ect_ObjectType ect_sObject =
{
   ECT_OA_NAME_VALUE,                           /* Name.                                              */
   ECT_OA_REV_VALUE,                            /* Revision.                                          */
   ECT_OA_NUM_INST_VALUE,                       /* Number of instances.                               */
   ECT_OA_HIGHEST_INST_VALUE                    /* Highest instance number.                           */
};

#if( ECT_IA_ENUM_ADIS_ENABLE || ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE || ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE || ECT_IA_ADI_TRANS_ENABLE || ECT_IA_OBJ_SUB_TRANS_ENABLE || ECT_IA_ENABLE_FOE_ENABLE || ECT_IA_ENABLE_EOE_ENABLE || ECT_IA_SET_DEV_ID_AS_CSA_ENABLE || ECT_IA_STATE_TIMEOUTS_ENABLE || ECT_IA_COMP_IDENT_LISTS_ENABLE || ECT_IA_CLEAR_IDENT_AL_STS_ENABLE )
static const ect_InstanceType ect_sInstance =
{
#if ECT_IA_ENUM_ADIS_ENABLE
   ECT_IA_ENUM_ADIS_VALUE,
#endif
#if ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE
   ECT_IA_WR_PD_ASSY_INST_TRANS_VALUE,
#endif
#if ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE
   ECT_IA_RD_PD_ASSY_INST_TRANS_VALUE,
#endif
#if ECT_IA_ADI_TRANS_ENABLE
   ECT_IA_ADI_TRANS_VALUE,
#endif
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
   ECT_IA_OBJ_SUB_TRANS_VALUE,
#endif
#if ECT_IA_ENABLE_FOE_ENABLE
   ECT_IA_ENABLE_FOE_VALUE,
#endif
#if ECT_IA_ENABLE_EOE_ENABLE
   ECT_IA_ENABLE_EOE_VALUE,
#endif
#if ECT_IA_SET_DEV_ID_AS_CSA_ENABLE
   ECT_IA_SET_DEV_ID_AS_CSA_VALUE,
#endif
#if ECT_IA_STATE_TIMEOUTS_ENABLE
   ECT_IA_STATE_TIMEOUTS_VALUE,
#endif
#if ECT_IA_COMP_IDENT_LISTS_ENABLE
   ECT_IA_COMP_IDENT_LISTS_VALUE,
#endif
#if ECT_IA_CLEAR_IDENT_AL_STS_ENABLE
   ECT_IA_CLEAR_IDENT_AL_STS_VALUE
#endif
};

#endif /* #if( ECT_IA_ENUM_ADIS_ENABLE...*/

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Processes commands to ECT Instances
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
   ** This function processes commands to the EtherCAT Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) != 1 )
   {
      /*
      ** The Instance does not exist.
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
#if ECT_IA_VENDOR_ID_ENABLE
      case ABP_ECT_IA_VENDOR_ID:

         /*
         ** Copy the 1st Instance 1 attribute (Vendor ID) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, ECT_IA_VENDOR_ID_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_VENDOR_ID_DS );
         break;
#endif
#if ECT_IA_PRODUCT_CODE_ENABLE
      case ABP_ECT_IA_PRODUCT_CODE:

         /*
         ** Copy the 2nd Instance 1 attribute (Product code) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, ECT_IA_PRODUCT_CODE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_PRODUCT_CODE_DS );
         break;
#endif
#if ECT_IA_MAJOR_REV_ENABLE
      case ABP_ECT_IA_MAJOR_REV:

         /*
         ** Copy the 3rd Instance 1 attribute (Revision high word) to the
         ** message.
         */
         ABCC_SetMsgData16( psNewMessage, ECT_IA_REVISION_HW_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_MAJOR_REV_DS );
         break;
#endif
#if ECT_IA_MINOR_REV_ENABLE
      case ABP_ECT_IA_MINOR_REV:

         /*
         ** Copy the 4th Instance 1 attribute (Revision low word) to the message.
         */
         ABCC_SetMsgData16( psNewMessage, ECT_IA_REVISION_LW_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_MINOR_REV_DS );
         break;
#endif
#if ECT_IA_SERIAL_NUMBER_ENABLE
      case ABP_ECT_IA_SERIAL_NUMBER:

         /*
         ** Copy the 5th Instance 1 attribute (Serial number) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, ECT_IA_SERIAL_NUMBER_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_SERIAL_NUMBER_DS );
         break;
#endif
#if ECT_IA_MANF_DEVICE_NAME_ENABLE
      case ABP_ECT_IA_MANF_DEV_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ECT_IA_MANF_DEVICE_NAME_VALUE );

         /*
         ** Copy the 6th Instance 1 attribute (Manufacturer device name) to the
         ** message.
         */
         ABCC_SetMsgString( psNewMessage, ECT_IA_MANF_DEVICE_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
#endif
#if ECT_IA_MANF_HW_VERSION_ENABLE
      case ABP_ECT_IA_MANF_HW_VER:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ECT_IA_MANF_HW_VERSION_VALUE );

         /*
         ** Copy the 7th Instance 1 attribute (Manufacturer hardware version) to
         ** the message.
         */
         ABCC_SetMsgString( psNewMessage, ECT_IA_MANF_HW_VERSION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
      }
      break;
#endif
#if ECT_IA_MANF_SW_VERSION_ENABLE
      case ABP_ECT_IA_MANF_SW_VER:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ECT_IA_MANF_SW_VERSION_VALUE );

         /*
         ** Copy the 8th Instance 1 attribute (Manufacturer software version) to
         ** the message.
         */
         ABCC_SetMsgString( psNewMessage, ECT_IA_MANF_SW_VERSION_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
      }
      break;
#endif
#if ECT_IA_ENUM_ADIS_ENABLE
      case ABP_ECT_IA_ENUM_ADIS:
      {
         UINT16  iIndex;

         /*
         ** Copy the 9th Instance 1 attribute (ENUM ADIs) to the message.
         */
         for( iIndex = 0; iIndex < ECT_IA_ENUM_ADIS_ARRAY_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.aiEnumADIs[ iIndex ],
                               ( ABP_UINT16_SIZEOF * iIndex ) );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_ENUM_ADIS_ARRAY_SIZE * ABP_UINT16_SIZEOF ) );
         break;
      }
#endif
#if ECT_IA_DEVICE_TYPE_ENABLE
      case ABP_ECT_IA_DEVICE_TYPE:

         /*
         ** Copy the 10th Instance 1 attribute (Device type) to the message.
         */
         ABCC_SetMsgData32( psNewMessage, ECT_IA_DEVICE_TYPE_VALUE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_DEVICE_TYPE_DS );
         break;
#endif
#if ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE
      case ABP_ECT_IA_WR_PD_ASSY_INST_TRANS:
      {
         UINT16  iIndex;

         /*
         ** Copy the 11th Instance 1 attribute (Write PD assembly instance
         ** translation) to the message.
         */
         for( iIndex = 0; iIndex < ECT_IA_WR_PD_ASSY_INST_TRANS_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.aiWrPdAssyInstTrans[ iIndex ],
                               ( ABP_UINT16_SIZEOF * iIndex ) );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_WR_PD_ASSY_INST_TRANS_SIZE * ABP_UINT16_SIZEOF ) );
         break;
      }
#endif
#if ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE
      case ABP_ECT_IA_RD_PD_ASSY_INST_TRANS:
      {
         UINT16  iIndex;

         /*
         ** Copy the 12th Instance 1 attribute (Read PD assembly instance
         ** translation) to the message.
         */
         for( iIndex = 0; iIndex < ECT_IA_RD_PD_ASSY_INST_TRANS_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.aiRdPdAssyInstTrans[ iIndex ],
                               ( ABP_UINT16_SIZEOF * iIndex ) );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_RD_PD_ASSY_INST_TRANS_SIZE * ABP_UINT16_SIZEOF ) );
         break;
      }
#endif
#if ECT_IA_ADI_TRANS_ENABLE
      case ABP_ECT_IA_ADI_TRANS:
      {
         UINT16  iIndex;

         /*
         ** Copy the 13th Instance 1 attribute (ADI translation) to the message.
         */
         for( iIndex = 0; iIndex < ECT_IA_ADI_TRANS_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.asADITrans[ iIndex ].iADIInstanceNumber,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF ) * iIndex ) );
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.asADITrans[ iIndex ].iADIObjectIndex,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF ) * iIndex ) + ABP_UINT16_SIZEOF );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_ADI_TRANS_SIZE * ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF ) ) );
         break;
      }
#endif
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
      case ABP_ECT_IA_OBJ_SUB_TRANS:
      {
         UINT16  iIndex;

         /*
         ** Copy the 13th Instance 1 attribute (ADI translation) to the message.
         */
         for( iIndex = 0; iIndex < ECT_IA_OBJ_SUB_TRANS_SIZE; ++iIndex )
         {
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.asObjSubTrans[ iIndex ].iADIInstanceNumber,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF + ABP_UINT8_SIZEOF ) * iIndex ) );
            ABCC_SetMsgData16( psNewMessage,
                               ect_sInstance.asObjSubTrans[ iIndex ].iADIObjectIndex,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF + ABP_UINT8_SIZEOF ) * iIndex ) + ABP_UINT16_SIZEOF );
            ABCC_SetMsgData8( psNewMessage,
                               ect_sInstance.asObjSubTrans[ iIndex ].bADIObjectIndexSubindex,
                               ( ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF + ABP_UINT8_SIZEOF ) * iIndex ) + ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF );
         }

         ABP_SetMsgResponse( psNewMessage, ( ECT_IA_OBJ_SUB_TRANS_SIZE * ( ABP_UINT16_SIZEOF + ABP_UINT16_SIZEOF + ABP_UINT8_SIZEOF ) ) );
         break;
      }
#endif
#if ECT_IA_ENABLE_FOE_ENABLE
      case ABP_ECT_IA_ENABLE_FOE:
      {
         /*
         ** The 'FoE enable' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fEnableFoE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_ENABLE_FOE_DS );
         break;
      }
#endif
#if ECT_IA_ENABLE_EOE_ENABLE
      case ABP_ECT_IA_ENABLE_EOE:
      {
         /*
         ** The 'EoE' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fEnableEoE, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_ENABLE_EOE_DS );
         break;
      }
#endif
#if ECT_IA_SET_DEV_ID_AS_CSA_ENABLE
      case ABP_ECT_IA_SET_DEV_ID_AS_CSA:
      {
         /*
         ** The 'Set Device ID as Configured State Alias' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fSetDevIDAsCSA, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_SET_DEV_ID_AS_CSA_DS );
         break;
      }
#endif
#if ECT_IA_ETHERCAT_STATE_ENABLE
      case ABP_ECT_IA_ETHERCAT_STATE:
      {
         /*
         ** The 'EtherCAT state' attribute is requested.
         ** Attribute is 'Set only'. Return error code 'Attribute not gettable'.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_GETABLE );
         break;
      }
#endif
#if ECT_IA_STATE_TIMEOUTS_ENABLE
      case ABP_ECT_IA_STATE_TIMEOUTS:
      {
         /*
         ** The 'State Transitions Timeout' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData32( psNewMessage, ect_sInstance.alStateTransTimeouts[ 0 ], 0 );
         ABCC_SetMsgData32( psNewMessage, ect_sInstance.alStateTransTimeouts[ 1 ], 4 );
         ABCC_SetMsgData32( psNewMessage, ect_sInstance.alStateTransTimeouts[ 2 ], 8 );
         ABCC_SetMsgData32( psNewMessage, ect_sInstance.alStateTransTimeouts[ 3 ], 12 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_STATE_TIMEOUTS_DS );
         break;
      }
#endif
#if ECT_IA_COMP_IDENT_LISTS_ENABLE
      case ABP_ECT_IA_COMP_IDENT_LISTS:
      {
         /*
         ** The 'Compare Identity Lists' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fCompIDLists, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_COMP_IDENT_LISTS_DS );
         break;
      }
#endif
#if ECT_IA_FSOE_STATUS_IND_ENABLE
      case ABP_ECT_IA_FSOE_STATUS_IND:
      {
         /*
         ** The 'FsOE status indicator' attribute is requested.
         ** Attribute is 'Set only'. Return error code 'Attribute not gettable'.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_GETABLE );
         break;
      }
#endif
#if ECT_IA_CLEAR_IDENT_AL_STS_ENABLE
      case ABP_ECT_IA_CLEAR_IDENT_AL_STS:
      {
         /*
         ** The 'Clear IdentALSts' attribute is requested.
         ** Copy the attribute to a response message.
         */
         ABCC_SetMsgData8( psNewMessage, ect_sInstance.fClearIdAlSts, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_ECT_IA_CLEAR_IDENT_AL_STS_DS );
         break;
      }
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
   case ABP_CMD_SET_ATTR:
   {
      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
#if ECT_IA_ETHERCAT_STATE_ENABLE
      case ABP_ECT_IA_ETHERCAT_STATE:
      {
         /*
         ** Save the new state in the local variable.
         */
         ABCC_GetMsgData8( psNewMessage, &ect_bEtherCATState, 0 );
         ABP_SetMsgResponse( psNewMessage, 0 );
         break;
      }
#endif
#if ECT_IA_FSOE_STATUS_IND_ENABLE
      case ABP_ECT_IA_FSOE_STATUS_IND:
      {
         /*
         ** Save the new state of the FsOE status indicator.
         */
         ABCC_GetMsgData8( psNewMessage, &ect_eFSoEStatusIndicator, 0 );
         ABP_SetMsgResponse( psNewMessage, 0 );
         break;
      }
#endif
#if( ECT_IA_VENDOR_ID_ENABLE || ECT_IA_PRODUCT_CODE_ENABLE || ECT_IA_MAJOR_REV_ENABLE || ECT_IA_MINOR_REV_ENABLE || ECT_IA_SERIAL_NUMBER_ENABLE || ECT_IA_MANF_DEVICE_NAME_ENABLE || ECT_IA_MANF_HW_VERSION_ENABLE || ECT_IA_MANF_SW_VERSION_ENABLE || ECT_IA_ENUM_ADIS_ENABLE || ECT_IA_DEVICE_TYPE_ENABLE || ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE || ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE || ECT_IA_ADI_TRANS_ENABLE || ECT_IA_OBJ_SUB_TRANS_ENABLE || ECT_IA_ENABLE_FOE_ENABLE || ECT_IA_ENABLE_EOE_ENABLE || ECT_IA_SET_DEV_ID_AS_CSA_ENABLE || ECT_IA_STATE_TIMEOUTS_ENABLE || ECT_IA_COMP_IDENT_LISTS_ENABLE || ECT_IA_CLEAR_IDENT_AL_STS_ENABLE )
#if ECT_IA_VENDOR_ID_ENABLE
      case ABP_ECT_IA_VENDOR_ID:
#endif
#if ECT_IA_PRODUCT_CODE_ENABLE
      case ABP_ECT_IA_PRODUCT_CODE:
#endif
#if ECT_IA_MAJOR_REV_ENABLE
      case ABP_ECT_IA_MAJOR_REV:
#endif
#if ECT_IA_MINOR_REV_ENABLE
      case ABP_ECT_IA_MINOR_REV:
#endif
#if ECT_IA_SERIAL_NUMBER_ENABLE
      case ABP_ECT_IA_SERIAL_NUMBER:
#endif
#if ECT_IA_MANF_DEVICE_NAME_ENABLE
      case ABP_ECT_IA_MANF_DEV_NAME:
#endif
#if ECT_IA_MANF_HW_VERSION_ENABLE
      case ABP_ECT_IA_MANF_HW_VER:
#endif
#if ECT_IA_MANF_SW_VERSION_ENABLE
      case ABP_ECT_IA_MANF_SW_VER:
#endif
#if ECT_IA_ENUM_ADIS_ENABLE
      case ABP_ECT_IA_ENUM_ADIS:
#endif
#if ECT_IA_DEVICE_TYPE_ENABLE
      case ABP_ECT_IA_DEVICE_TYPE:
#endif
#if ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE
      case ABP_ECT_IA_WR_PD_ASSY_INST_TRANS:
#endif
#if ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE
      case ABP_ECT_IA_RD_PD_ASSY_INST_TRANS:
#endif
#if ECT_IA_ADI_TRANS_ENABLE
      case ABP_ECT_IA_ADI_TRANS:
#endif
#if ECT_IA_OBJ_SUB_TRANS_ENABLE
      case ABP_ECT_IA_OBJ_SUB_TRANS:
#endif
#if ECT_IA_ENABLE_FOE_ENABLE
      case ABP_ECT_IA_ENABLE_FOE:
#endif
#if ECT_IA_ENABLE_EOE_ENABLE
      case ABP_ECT_IA_ENABLE_EOE:
#endif
#if ECT_IA_SET_DEV_ID_AS_CSA_ENABLE
      case ABP_ECT_IA_SET_DEV_ID_AS_CSA:
#endif
#if ECT_IA_STATE_TIMEOUTS_ENABLE
      case ABP_ECT_IA_STATE_TIMEOUTS:
#endif
#if ECT_IA_COMP_IDENT_LISTS_ENABLE
      case ABP_ECT_IA_COMP_IDENT_LISTS:
#endif
#if ECT_IA_CLEAR_IDENT_AL_STS_ENABLE
      case ABP_ECT_IA_CLEAR_IDENT_AL_STS:
#endif
      {
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_ATTR_NOT_SETABLE );
         break;
      }
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
   }
}

/*------------------------------------------------------------------------------
** Processes commands to the ECT Object (Instance 0)
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
   ** This function processes commands to the EtherCAT Object
   ** (Instance 0).
   */
   switch( ABCC_GetMsgCmdBits( psNewMessage ) )
   {
   case ABP_CMD_GET_ATTR:

      switch( ABCC_GetMsgCmdExt0( psNewMessage ) )
      {
      case ABP_OA_NAME:
      {
         UINT16 iStrLength;

         iStrLength = (UINT16)strlen( ECT_OA_NAME_VALUE );

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgString( psNewMessage, ECT_OA_NAME_VALUE, iStrLength, 0 );
         ABP_SetMsgResponse( psNewMessage, (UINT8)iStrLength );
         break;
      }
      case ABP_OA_REV:

         /*
         ** Copy the attribute to a message.
         */

         ABCC_SetMsgData8( psNewMessage, ect_sObject.bRevision, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT8_SIZEOF );
         break;

      case ABP_OA_NUM_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, ect_sObject.iNumberOfInstances, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      case ABP_OA_HIGHEST_INST:

         /*
         ** Copy the attribute to a message.
         */
         ABCC_SetMsgData16( psNewMessage, ect_sObject.iHighestInstanceNo, 0 );
         ABP_SetMsgResponse( psNewMessage, ABP_UINT16_SIZEOF );
         break;

      default:

         /*
         ** Unsupported attribute.
         */
         ABP_SetMsgErrorResponse( psNewMessage, 1, ABP_ERR_INV_CMD_EXT_0 );
         break;

      } /* End of switch( Attribute number ) */

      break;

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

void ECT_ProcessCmdMsg( ABP_MsgType* psNewMessage )
{
   /*
   ** This function processes commands to the EtherCAT Object and its Instance.
   */
   if( ABCC_GetMsgInstance( psNewMessage ) == ABP_INST_OBJ )
   {
      /*
      ** EtherCAT object Command.
      */
      ObjectCommand( psNewMessage );
   }
   else
   {
      /*
      ** EtherCAT instance Command.
      */
      InstanceCommand( psNewMessage );
   }

   ABCC_SendRespMsg( psNewMessage );
}

#endif
