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
** User configuration of the Anybus objects implementation.
**
** All configurations in this file should be possible to override if defined
** in abcc_platform_cfg.h. This allows the target platform to adapt the
** product configuration to realize its implementation for specific needs.
********************************************************************************
********************************************************************************
** Services:
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_OBJ_CFG_H_
#define ABCC_OBJ_CFG_H_

#include "abcc_platform_cfg.h"
#include "abcc_drv_cfg.h"


/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Supported host-side network objects - Set to TRUE to include and enable the
** handler for the corresponding network object.
**------------------------------------------------------------------------------
*/
#ifndef CFN_OBJ_ENABLE
   #define CFN_OBJ_ENABLE                          FALSE
#endif
#ifndef EPL_OBJ_ENABLE
   #define EPL_OBJ_ENABLE                          FALSE
#endif
#ifndef ECT_OBJ_ENABLE
   #define ECT_OBJ_ENABLE                          TRUE
#endif
#ifndef PRT_OBJ_ENABLE
   #define PRT_OBJ_ENABLE                          FALSE
#endif
#ifndef CCL_OBJ_ENABLE
   #define CCL_OBJ_ENABLE                          FALSE
#endif
#ifndef EIP_OBJ_ENABLE
   #define EIP_OBJ_ENABLE                          FALSE
#endif
#ifndef MOD_OBJ_ENABLE
   #define MOD_OBJ_ENABLE                          FALSE
#endif
#ifndef COP_OBJ_ENABLE
   #define COP_OBJ_ENABLE                          FALSE
#endif
#ifndef DEV_OBJ_ENABLE
   #define DEV_OBJ_ENABLE                          FALSE
#endif
#ifndef DPV1_OBJ_ENABLE
   #define DPV1_OBJ_ENABLE                         FALSE
#endif

/*------------------------------------------------------------------------------
** Supported host objects - Set to TRUE to enable
**------------------------------------------------------------------------------
*/
#ifndef SAFE_OBJ_ENABLE
   #define SAFE_OBJ_ENABLE                         FALSE
#endif
#ifndef SYNC_OBJ_ENABLE
   #define SYNC_OBJ_ENABLE                         ABCC_CFG_SYNC_ENABLE
#endif
#ifndef ETN_OBJ_ENABLE
   #define ETN_OBJ_ENABLE                          FALSE
#endif
#ifndef OPCUA_OBJ_ENABLE
   #define OPCUA_OBJ_ENABLE                        FALSE
#endif
#ifndef MQTT_OBJ_ENABLE
   #define MQTT_OBJ_ENABLE                         FALSE
#endif
#ifndef ASM_OBJ_ENABLE
   #define ASM_OBJ_ENABLE                          FALSE
#endif

/*------------------------------------------------------------------------------
** MQTT Object (0xE2)
**------------------------------------------------------------------------------
*/
#if MQTT_OBJ_ENABLE
/*
** Attribute 1: MQTT mode (UINT8 - 0:Disabled/1:Enabled with JSON data format)
*/
#ifndef MQTT_IA_MODE_ENABLE
   #define MQTT_IA_MODE_ENABLE                     FALSE
   #define MQTT_IA_MODE_VALUE                      0x00
#endif

/*
** Attribute 2: MQTT last will configuration
** Struct of:
** UINT8          - QoS level (0/1/2)
** BOOL           - Retain bit (TRUE/FALSE)
** UINT16         - Topic string length (0-128)
** Array of CHAR  - Topic string (0x00-0xFF),
**                  length of array must match "Topic string length"
** UINT16         - Message data length (0-256)
** Array of OCTET - Message data (0x00-0xFF),
**                  length of array must match "Message data length"
*/
#ifndef MQTT_IA_LAST_WILL_ENABLE
   #define MQTT_IA_LAST_WILL_ENABLE                FALSE
   #define MQTT_IA_LAST_WILL_QOS                   0
   #define MQTT_IA_LAST_WILL_RETAIN                FALSE
   #define MQTT_IA_LAST_WILL_TOPIC_STR             "UserDefinedTopic"
   #define MQTT_IA_LAST_WILL_MESSAGE_STR           "UserDefinedMessage"
#endif

/*
** Object specific command: Publish Configuration (0x10)
**
** If enabled, MQTT_GetPublishConfigurationCfn() must be ported to respond with
** host application specific data.
*/
#ifndef MQTT_CMD_PUBLISH_CFG_ENABLE
   #define MQTT_CMD_PUBLISH_CFG_ENABLE             FALSE
#endif

#endif /* #if MQTT_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** OPC UA Object (0xE3)
**------------------------------------------------------------------------------
*/
#if OPCUA_OBJ_ENABLE
/*
** Attribute 1: OPC UA Model (UINT8 - 0:Disabled/1:CompactCom 40 model)
*/
#ifndef OPCUA_IA_MODEL_ENABLE
   #define OPCUA_IA_MODEL_ENABLE                   FALSE
   #define OPCUA_IA_MODEL_VALUE                    0x00
#endif

/*
** Attribute 2: Application/Local namespace URI (Array of CHAR - {0x00-0xFF})
** A function returning a string may be used to generate a unique string
*/
#ifndef OPCUA_IA_APPLICATION_URI_ENABLE
   #define OPCUA_IA_APPLICATION_URI_ENABLE         FALSE
   #define OPCUA_IA_APPLICATION_URI_VALUE          "UniqueApplicationUriPerUnit"
#endif

/*
** Attribute 3: Vendor namespace URI (Array of CHAR - {0x00-0xFF})
*/
#ifndef OPCUA_IA_VENDOR_NAMESPACE_URI_ENABLE
   #define OPCUA_IA_VENDOR_NAMESPACE_URI_ENABLE    FALSE
   #define OPCUA_IA_VENDOR_NAMESPACE_URI_VALUE     "UniqueVendorNamespaceUri"
#endif

/*
** Attribute 4: DeviceType name (Array of CHAR - {0x00-0xFF))
*/
#ifndef OPCUA_IA_DEVICE_TYPE_NAME_ENABLE
   #define OPCUA_IA_DEVICE_TYPE_NAME_ENABLE        FALSE
   #define OPCUA_IA_DEVICE_TYPE_NAME_VALUE         "VendorDeviceType"
#endif

/*
** Attribute 5: Device instance name (Array of CHAR - {0x00-0xFF))
*/
#ifndef OPCUA_IA_DEVICE_INST_NAME_ENABLE
   #define OPCUA_IA_DEVICE_INST_NAME_ENABLE        FALSE
   #define OPCUA_IA_DEVICE_INST_NAME_VALUE         "VendorDeviceInstance"
#endif

/*
**Attribute 6: Product URI (Array of CHAR - {0x00-0xFF))
*/
#ifndef OPCUA_IA_PRODUCT_URI_ENABLE
   #define OPCUA_IA_PRODUCT_URI_ENABLE             FALSE
   #define OPCUA_IA_PRODUCT_URI_VALUE              "UniqueProductUri"
#endif

#endif /* #if OPCUA_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** Functional Safety object (0xE8)
**------------------------------------------------------------------------------
*/
#if SAFE_OBJ_ENABLE
/*
** Attribute 1: Safety Enabled (BOOL - TRUE/FALSE)
**
** PORTING ALERT!
**
** If the ABCCs serial application interface is used, please double-check that
** the application hardware is correctly set up before enabling the interface
** for the safety hardware via the 'Safety Enabled' attribute. When a serial
** Operation Mode is used the serial communication signals for the safety
** hardware uses the same I/O pins on the ABCC as the A12 and A13 address bus
** signals, and they must then not be driven from any other application-side
** hardware in this case.
**
** Also note that the value of the 'Safety Enabled' attribute does not have to
** be initialized to TRUE in this file. It can be set during system startup via
** the "SAFE_SetSafetyEnable()" function in "safe_obj.c" before the ABCC driver
** itself is started.
*/
#ifndef SAFE_IA_SAFETY_ENABLED_ENABLE
   #define SAFE_IA_SAFETY_ENABLED_ENABLE           FALSE
   #define SAFE_IA_SAFETY_ENABLED_VALUE            FALSE
#endif

/*
** Attribute 2: Baud rate (UINT32 - 625000, 1000000 or 1020000)
*/
#ifndef SAFE_IA_BAUD_RATE_ENABLE
   #define SAFE_IA_BAUD_RATE_ENABLE                FALSE
   #define SAFE_IA_BAUD_RATE_VALUE                 1020000L
#endif

/*
** Attribute 3: IO Configuration (Array of UINT8 - {0x00-0xFF})
*/
#ifndef SAFE_IA_IO_CONFIGURATION_ENABLE
   #define SAFE_IA_IO_CONFIGURATION_ENABLE         FALSE
   #define SAFE_IA_IO_CONFIGURATION_VALUE          { 0x00 }
   #define SAFE_IA_IO_CONFIGURATION_ARRAY_SIZE     1
#endif

/*
** Attribute 4: Cycle time (UINT8 - 2, 4, 8 or 16)
*/
#ifndef SAFE_IA_CYCLE_TIME_ENABLE
#define SAFE_IA_CYCLE_TIME_ENABLE                  FALSE
#define SAFE_IA_CYCLE_TIME_VALUE                   2
#endif

#endif /* #if SAFE_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** Assembly mapping Object (0xEB)
**------------------------------------------------------------------------------
*/
#if ASM_OBJ_ENABLE
/*
** Attribute 13: Name (Array of CHAR - {0x00-0xFF))
*/
#ifndef ASM_IA_NAME_ENABLE
   #define ASM_IA_NAME_ENABLE                      FALSE
#endif
#endif /* #if ASM_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** Sync Object (0xEE)
**------------------------------------------------------------------------------
*/
#if SYNC_OBJ_ENABLE
/*
** Attribute 1: Cycle time (UINT32 - 0x00000000-0xFFFFFFFF)
*/
#ifndef SYNC_IA_CYCLE_TIME_ENABLE
   #define SYNC_IA_CYCLE_TIME_ENABLE               TRUE
   #define SYNC_IA_CYCLE_TIME_VALUE                0L
#endif

/*
** Attribute 2: Output valid (UINT32 - 0x00000000-0xFFFFFFFF)
*/
#ifndef SYNC_IA_OUTPUT_VALID_ENABLE
   #define SYNC_IA_OUTPUT_VALID_ENABLE             TRUE
   #define SYNC_IA_OUTPUT_VALID_VALUE              0
#endif

/*
** Attribute 3: Input capture (UINT32 - 0x00000000-0xFFFFFFFF)
*/
#ifndef SYNC_IA_INPUT_CAPTURE_ENABLE
   #define SYNC_IA_INPUT_CAPTURE_ENABLE            TRUE
   #define SYNC_IA_INPUT_CAPTURE_VALUE             0
#endif

/*
** Attribute 4: Output processing (UINT32 - 0x00000000-0xFFFFFFFF)
*/
#ifndef SYNC_IA_OUTPUT_PROCESSING_ENABLE
   #define SYNC_IA_OUTPUT_PROCESSING_ENABLE        TRUE
   #define SYNC_IA_OUTPUT_PROCESSING_VALUE         1000L
#endif

/*
** Attribute 5: Input processing (UINT32 - 0x00000000-0xFFFFFFFF)
*/
#ifndef SYNC_IA_INPUT_PROCESSING_ENABLE
   #define SYNC_IA_INPUT_PROCESSING_ENABLE         TRUE
   #define SYNC_IA_INPUT_PROCESSING_VALUE          1000L
#endif

/*
** Attribute 6: Min cycle time (UINT32 - 0x00000000-0xFFFFFFFF)
*/
#ifndef SYNC_IA_MIN_CYCLE_TIME_ENABLE
   #define SYNC_IA_MIN_CYCLE_TIME_ENABLE           TRUE
   #define SYNC_IA_MIN_CYCLE_TIME_VALUE            1L
#endif

/*
** Attribute 7: Sync mode (UINT16 - 0: Nonsynchronous operation
**                                  1: Synchronous operation)
*/
#ifndef SYNC_IA_SYNC_MODE_ENABLE
   #define SYNC_IA_SYNC_MODE_ENABLE                TRUE
   #define SYNC_IA_SYNC_MODE_VALUE                 0x0000
#endif

/*
** Attribute 8: Supported sync modes (UINT16 - Bit 0: 1 = Nonsynchronous mode supported
**                                             Bit 1: 1 = Synchronous mode supported
**                                             Bit 2-15: Reserved)
*/
#ifndef SYNC_IA_SUPPORTED_SYNC_MODES_ENABLE
   #define SYNC_IA_SUPPORTED_SYNC_MODES_ENABLE     TRUE
   #define SYNC_IA_SUPPORTED_SYNC_MODES_VALUE      0x0003
#endif

#endif /* #if SYNC_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** EtherCAT Object (0xF5)
**------------------------------------------------------------------------------
*/
#if ECT_OBJ_ENABLE

/*------------------------------------------------------------------------------
** ECT_SUPPORT_SYNC_ERR_COUNTERS
**
** Setting this configuration option to TRUE will enable the synchronization
** related error counters on the sync manager parameter objects (0x1C32 and
** 0x1C33).
** The following object entries will then be accessible by the EtherCAT master:
** 0x1C32:11                 -  SM event missed counter
** 0x1C32:12 and 0x1C33:12   -  Cycle time too small
** 0x1C32:13                 -  Shift time too short
** 0x1C32:32                 -  Sync error
**
** Note: These object entries are specified in the EtherCAT specification
** ETG.1020, Protocol enhancements. See that document for more information.
**
** Note: These object entries are mandatory to support when synchronization
** is supported by the EtherCAT slave. They are not used when not operating
** synchronously. Therefore the recommendation is to set this configuration
** option to TRUE if bit 1 in SYNC_IA_SUPPORTED_SYNC_MODES_VALUE is 1 and
** the EtherCAT network is supported.
**------------------------------------------------------------------------------
*/
#ifndef ECT_SUPPORT_SYNC_ERR_COUNTERS
   #define ECT_SUPPORT_SYNC_ERR_COUNTERS  FALSE
#endif

#if ECT_SUPPORT_SYNC_ERR_COUNTERS
   /*
   ** Sub-index translation functionality must be enabled when the sync
   ** error counters are enabled.
   */
   #ifndef ECT_IA_OBJ_SUB_TRANS_ENABLE
      #define ECT_IA_OBJ_SUB_TRANS_ENABLE       TRUE
   #endif

   /*
   ** ADIs used for the sub-index translation
   */
   #ifndef ECT_SUB_TRANS_1C32_11
      #define ECT_SUB_TRANS_1C32_11 0xF20B
   #endif
   #ifndef ECT_SUB_TRANS_1C32_12
      #define ECT_SUB_TRANS_1C32_12 0xF20C
   #endif
   #ifndef ECT_SUB_TRANS_1C32_13
      #define ECT_SUB_TRANS_1C32_13 0xF20D
   #endif
   #ifndef ECT_SUB_TRANS_1C32_32
      #define ECT_SUB_TRANS_1C32_32 0xF220
   #endif
   #ifndef ECT_SUB_TRANS_1C33_12
      #define ECT_SUB_TRANS_1C33_12 0xF30C
   #endif

   /*
   ** Example value for the sub-index translation attribute.
   */
   #ifndef ECT_IA_OBJ_SUB_TRANS_VALUE
      /*
      ** The ADIs created for sync error counters are translated into entries
      ** on the 0x1C32 and 0x1C33 objects.
      */
      #define ECT_IA_OBJ_SUB_TRANS_VALUE        {  { ECT_SUB_TRANS_1C32_11, 0x1C32, 0x0B }, \
                                                   { ECT_SUB_TRANS_1C32_12, 0x1C32, 0x0C }, \
                                                   { ECT_SUB_TRANS_1C32_13, 0x1C32, 0x0D }, \
                                                   { ECT_SUB_TRANS_1C32_32, 0x1C32, 0x20 }, \
                                                   { ECT_SUB_TRANS_1C33_12, 0x1C33, 0x0C }  \
                                                }
   #endif
   #ifndef ECT_IA_OBJ_SUB_TRANS_SIZE
      #define ECT_IA_OBJ_SUB_TRANS_SIZE         5
   #endif
#endif /* #if ECT_SUPPORT_SYNC_ERR_COUNTERS */

/*
** Attribute 9: ENUM ADIs (Array of UINT16 - {0x0001-0xFFFF})
*/
#ifndef ECT_IA_ENUM_ADIS_ENABLE
   #define ECT_IA_ENUM_ADIS_ENABLE                 FALSE
   #define ECT_IA_ENUM_ADIS_VALUE                  { 0x1357 }
   #define ECT_IA_ENUM_ADIS_ARRAY_SIZE             1
#endif

/*
** Attribute 11: Write PD assembly instance translation
** (Array of UINT16 - {0x1A00-0x1BFF})
*/
#ifndef ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE
   #define ECT_IA_WR_PD_ASSY_INST_TRANS_ENABLE     FALSE
   #define ECT_IA_WR_PD_ASSY_INST_TRANS_VALUE      { 0x1BFF }
   #define ECT_IA_WR_PD_ASSY_INST_TRANS_SIZE       1
#endif

/*
** Attribute 12: Read PD assembly instance translation
** (Array of UINT16 - {0x1600-0x17FF})
*/
#ifndef ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE
   #define ECT_IA_RD_PD_ASSY_INST_TRANS_ENABLE     FALSE
   #define ECT_IA_RD_PD_ASSY_INST_TRANS_VALUE      { 0x17FF }
   #define ECT_IA_RD_PD_ASSY_INST_TRANS_SIZE       1
#endif

/*
** Attribute 13: ADI translation (Array of (Struct of {UINT16, UINT16}))
*/
#ifndef ECT_IA_ADI_TRANS_ENABLE
   #define ECT_IA_ADI_TRANS_ENABLE                 FALSE
   #define ECT_IA_ADI_TRANS_VALUE                  { { 0xE000, 0x10F8 } }
   #define ECT_IA_ADI_TRANS_SIZE                   1
#endif

/*
** Attribute 15: Object subindex translation
** (Array of (Struct of {UINT16, UINT16, UINT8 }))
*/
#ifndef ECT_IA_OBJ_SUB_TRANS_ENABLE
   #define ECT_IA_OBJ_SUB_TRANS_ENABLE             FALSE
   #define ECT_IA_OBJ_SUB_TRANS_VALUE              { { 0xE000, 0x10F3, 0x01 } }
   #define ECT_IA_OBJ_SUB_TRANS_SIZE               1
#endif

/*
** Attribute 16: Enable FoE (BOOL - TRUE/FALSE)
*/
#ifndef ECT_IA_ENABLE_FOE_ENABLE
   #define ECT_IA_ENABLE_FOE_ENABLE                FALSE
   #define ECT_IA_ENABLE_FOE_VALUE                 TRUE
#endif

/*
** Attribute 17: Enable EoE (BOOL - TRUE/FALSE)
*/
#ifndef ECT_IA_ENABLE_EOE_ENABLE
   #define ECT_IA_ENABLE_EOE_ENABLE                FALSE
   #define ECT_IA_ENABLE_EOE_VALUE                 TRUE
#endif

/*
** Attribute 19: Set Device ID as Configured Station Alias (BOOL - TRUE/FALSE)
*/
#ifndef ECT_IA_SET_DEV_ID_AS_CSA_ENABLE
   #define ECT_IA_SET_DEV_ID_AS_CSA_ENABLE         FALSE
   #define ECT_IA_SET_DEV_ID_AS_CSA_VALUE          FALSE
#endif

/*
** Attribute 20: EtherCAT state
*/
#ifndef ECT_IA_ETHERCAT_STATE_ENABLE
   #define ECT_IA_ETHERCAT_STATE_ENABLE            FALSE
#endif

/*
** Attribute 21: State Transition Timeouts (Array of UINT32)
*/
#ifndef ECT_IA_STATE_TIMEOUTS_ENABLE
   #define ECT_IA_STATE_TIMEOUTS_ENABLE            FALSE
   #define ECT_IA_STATE_TIMEOUTS_VALUE             { 1000, 5000, 1000, 200 }
#endif

/*
** Attribute 22: Compare Identity Lists (BOOL - TRUE/FALSE)
*/
#ifndef ECT_IA_COMP_IDENT_LISTS_ENABLE
   #define ECT_IA_COMP_IDENT_LISTS_ENABLE          FALSE
   #define ECT_IA_COMP_IDENT_LISTS_VALUE           TRUE
#endif

/*
** Attribute 23: FSoE Status Indicator
*/
#ifndef ECT_IA_FSOE_STATUS_IND_ENABLE
   #define ECT_IA_FSOE_STATUS_IND_ENABLE           FALSE
#endif

/*
** Attribute 24: Clear IdentALSts (BOOL - TRUE/FALSE)
*/
#ifndef ECT_IA_CLEAR_IDENT_AL_STS_ENABLE
   #define ECT_IA_CLEAR_IDENT_AL_STS_ENABLE        FALSE
   #define ECT_IA_CLEAR_IDENT_AL_STS_VALUE         FALSE
#endif
#endif /* if ECT_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** PROFINET IO Object (0xF6)
**------------------------------------------------------------------------------
*/

#if PRT_OBJ_ENABLE
/*
** Attribute 4: MaxAr (UINT32 - 0x00000000-0x00000003)
*/
#ifndef PRT_IA_MAX_AR_ENABLE
   #define PRT_IA_MAX_AR_ENABLE                    FALSE
   #define PRT_IA_MAX_AR_VALUE                     0x00000000
#endif

/*
** Attribute 7: Record data Transparent Mode (UINT8 - 0x00-0xFF)
*/
#ifndef PRT_IA_RTM_ENABLE
   #define PRT_IA_RTM_ENABLE                       FALSE
   #define PRT_IA_RTM_VALUE                        0x00
#endif

/*
** Attribute 12: IM Revision counter (UINT16 - 0x0000-0xFFFF)
*/
#ifndef PRT_IA_IM_REV_CNT_ENABLE
   #define PRT_IA_IM_REV_CNT_ENABLE                FALSE
   #define PRT_IA_IM_REV_CNT_VALUE                 0xFFFF
#endif

/*
** Attribute 13: IM Profile ID (UINT16 - 0x0000-0xFFFF)
*/
#ifndef PRT_IA_IM_PROFILE_ID_ENABLE
   #define PRT_IA_IM_PROFILE_ID_ENABLE             FALSE
   #define PRT_IA_IM_PROFILE_ID_VALUE              0xFFFF
#endif

/*
** Attribute 14: IM Profile specific type (UINT16 - 1-6))
*/
#ifndef PRT_IA_IM_PROFILE_SPEC_TYPE_ENABLE
   #define PRT_IA_IM_PROFILE_SPEC_TYPE_ENABLE      FALSE
   #define PRT_IA_IM_PROFILE_SPEC_TYPE_VALUE       0x0004
#endif

/*
** Attribute 15: IM Version (Array[2] of UINT8). Reserved in 40-series.
*/
#ifndef PRT_IA_IM_VER_ENABLE
   #define PRT_IA_IM_VER_ENABLE                    FALSE
   #define PRT_IA_IM_VER_MAJOR_VALUE               99
   #define PRT_IA_IM_VER_MINOR_VALUE               99
#endif

/*
** Attribute 16: IM Supported (UINT16 - 0x0000-0xFFFF). Reserved in 40-series.
*/
#ifndef PRT_IA_IM_SUPPORTED_ENABLE
   #define PRT_IA_IM_SUPPORTED_ENABLE              FALSE
   #define PRT_IA_IM_SUPPORTED_VALUE               0xFFFF
#endif

/*
** Attribute 17: Port 1 MAC Address (Array[6] of UINT8)
*/
#ifndef PRT_IA_PORT1_MAC_ADDRESS_ENABLE
   #define PRT_IA_PORT1_MAC_ADDRESS_ENABLE         FALSE
   #define PRT_IA_PORT1_MAC_ADDRESS_VALUE          "\x00\x00\x00\x00\x00\x00"
#endif

/*
** Attribute 18: Port 2 MAC Address (Array[6] of UINT8)
*/
#ifndef PRT_IA_PORT2_MAC_ADDRESS_ENABLE
   #define PRT_IA_PORT2_MAC_ADDRESS_ENABLE         FALSE
   #define PRT_IA_PORT2_MAC_ADDRESS_VALUE          "\x00\x00\x00\x00\x00\x00"
#endif

/*
** Attribute 20: Interface Description (Array of CHAR). Reserved in 40-series.
*/
#ifndef PRT_IA_INTERFACE_DESCRIPTION_ENABLE
   #define PRT_IA_INTERFACE_DESCRIPTION_ENABLE     FALSE
   #define PRT_IA_INTERFACE_DESCRIPTION_VALUE      "Interface Description"
#endif

/*
** Attribute 21: Module Id Assignment mode (UINT8 - 0x00-0xFF). Reserved in 40-series.
*/
#ifndef PRT_IA_MOD_ID_ASSIGN_MODE_ENABLE
   #define PRT_IA_MOD_ID_ASSIGN_MODE_ENABLE        FALSE
   #define PRT_IA_MOD_ID_ASSIGN_MODE_VALUE         0
#endif

/*
** Attribute 23: PROFIenergy functionality (UINT8 - 0x00-0xFF). Reserved in 40-series.
*/
#ifndef PRT_IA_PROFIENERGY_FUNC_ENABLE
   #define PRT_IA_PROFIENERGY_FUNC_ENABLE          FALSE
   #define PRT_IA_PROFIENERGY_FUNC_VALUE           0
#endif

/*
** Attribute 25: IM5 Module Order ID (Array of CHAR).
*/
#ifndef PRT_IA_IM_MODULE_ORDER_ID_ENABLE
   #define PRT_IA_IM_MODULE_ORDER_ID_ENABLE        FALSE
   #define PRT_IA_IM_MODULE_ORDER_ID_VALUE         "ARTICLE_ABCD_1234_PIR"
#endif

/*
** Attribute 26: IM5 Annotation (Array of CHAR).
*/
#ifndef PRT_IA_IM_ANNOTATION_ENABLE
   #define PRT_IA_IM_ANNOTATION_ENABLE             FALSE
   #define PRT_IA_IM_ANNOTATION_VALUE              "ExampleCompany PROFINET IRT Adapter"
#endif

/*
** Attribute 27: IM5 Enabled (BOOL - TRUE/FALSE).
*/
#ifndef PRT_IA_IM5_ENABLED_ENABLE
   #define PRT_IA_IM5_ENABLED_ENABLE               FALSE
   #define PRT_IA_IM5_ENABLED_VALUE                TRUE
#endif

#endif /* #if PRT_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** CC-Link Object (0xF7)
**------------------------------------------------------------------------------
*/
#if CCL_OBJ_ENABLE

/*
** Attribute 4: Network Setting (Array[3] of UINT8)
*/
#ifndef CCL_IA_NETWORK_SETTING_ENABLE
   #define CCL_IA_NETWORK_SETTING_ENABLE           TRUE
   #define CCL_IA_CCLINK_VERSION_VALUE             0x02
   #define CCL_IA_NUM_OF_OCCUPIED_STATION_VALUE    0x02
   #define CCL_IA_NUM_OF_EXTENSION_CYCLES_VALUE    0x04
#endif

/*
** Attribute 5: System Area Hander (SINT16 - -1:Host application/0...880:Module)
*/
#ifndef CCL_IA_SYSTEM_AREA_HANDLER_ENABLE
   #define CCL_IA_SYSTEM_AREA_HANDLER_ENABLE       FALSE
   #define CCL_IA_SYSTEM_AREA_HANDLER_VALUE        (-1)
#endif

/*
** Attribute 6: Output Hold/Clear (UINT8 - 0:Clear/1:Hold)
*/
#ifndef CCL_IA_OUTPUT_HOLD_CLEAR_ENABLE
   #define CCL_IA_OUTPUT_HOLD_CLEAR_ENABLE         TRUE
   #define CCL_IA_OUTPUT_HOLD_CLEAR_VALUE          0x00
#endif

#endif /* #if CCL_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** EtherNet/IP Object (0xF8)
**------------------------------------------------------------------------------
*/

#if EIP_OBJ_ENABLE
/*
** Attribute 7: Producing instance number (Array of UINT16 - {0x0001-0xFFFF})
*/
#ifndef EIP_IA_PROD_INSTANCE_ENABLE
   #define EIP_IA_PROD_INSTANCE_ENABLE             FALSE
   #define EIP_IA_PROD_INSTANCE_VALUE              { 0x0064 }
   #define EIP_IA_PROD_INSTANCE_ARRAY_SIZE         1
#endif

/*
** Attribute 8: Consuming instance number (Array of UINT16 - {0x0001-0xFFFF})
*/
#ifndef EIP_IA_CONS_INSTANCE_ENABLE
   #define EIP_IA_CONS_INSTANCE_ENABLE             FALSE
   #define EIP_IA_CONS_INSTANCE_VALUE              { 0x0096 }
   #define EIP_IA_CONS_INSTANCE_ARRAY_SIZE         1
#endif

/*
** Attribute 9: Enable communication settings from net (BOOL - TRUE/FALSE)
*/
#ifndef EIP_IA_COMM_SETTINGS_FROM_NET_ENABLE
   #define EIP_IA_COMM_SETTINGS_FROM_NET_ENABLE    FALSE
   #define EIP_IA_COMM_SETTINGS_FROM_NET_VALUE     TRUE
#endif

/*
** Attribute 11: Enable CIP request forwarding (BOOL - TRUE/FALSE)
*/
#ifndef EIP_IA_ENABLE_APP_CIP_OBJECTS_ENABLE
   #define EIP_IA_ENABLE_APP_CIP_OBJECTS_ENABLE    FALSE
   #define EIP_IA_ENABLE_APP_CIP_OBJECTS_VALUE     FALSE
#endif

/*
** Attribute 12: Enable Parameter object (BOOL - TRUE/FALSE)
*/
#ifndef EIP_IA_ENABLE_PARAM_OBJECT_ENABLE
   #define EIP_IA_ENABLE_PARAM_OBJECT_ENABLE       FALSE
   #define EIP_IA_ENABLE_PARAM_OBJECT_VALUE        TRUE
#endif

/*
** Attribute 13: Input only heartbeat instance number (UINT16 - 0x0000-0xFFFF)
*/
#ifndef EIP_IA_INPUT_INSTANCE_OBJECT_ENABLE
   #define EIP_IA_INPUT_INSTANCE_OBJECT_ENABLE     FALSE
   #define EIP_IA_INPUT_INSTANCE_OBJECT_VALUE      0x0003
#endif

/*
** Attribute 14: Listen only heartbeat instance number (UINT16 - 0x0000-0xFFFF)
*/
#ifndef EIP_IA_LISTEN_INSTANCE_OBJECT_ENABLE
   #define EIP_IA_LISTEN_INSTANCE_OBJECT_ENABLE    FALSE
   #define EIP_IA_LISTEN_INSTANCE_OBJECT_VALUE     0x0004
#endif

/*
** Attribute 15: Assembly object Configuration instance number (UINT16 - 0x0000-0xFFFF)
*/
#ifndef EIP_IA_CONFIG_INSTANCE_ENABLE
   #define EIP_IA_CONFIG_INSTANCE_ENABLE           FALSE
   #define EIP_IA_CONFIG_INSTANCE_VALUE            0x0005
#endif

/*
** Attribute 16: Disable Strict IO match (BOOL - TRUE/FALSE)
*/
#ifndef EIP_IA_DISABLE_STRICT_IO_MATCH_ENABLE
   #define EIP_IA_DISABLE_STRICT_IO_MATCH_ENABLE   FALSE
   #define EIP_IA_DISABLE_STRICT_IO_MATCH_VALUE    FALSE
#endif

/*
** Attribute 17: Enable unconnected routing (BOOL - TRUE/FALSE)
*/
#ifndef EIP_IA_ENABLE_UNCONNECTED_SEND_ENABLE
   #define EIP_IA_ENABLE_UNCONNECTED_SEND_ENABLE   FALSE
   #define EIP_IA_ENABLE_UNCONNECTED_SEND_VALUE    FALSE
#endif

/*
** Attribute 18: Input only extended heartbeat instance number (UINT16 - 0x0000-0xFFFF)
*/
#ifndef EIP_IA_INPUT_EXT_INSTANCE_OBJECT_ENABLE
   #define EIP_IA_INPUT_EXT_INSTANCE_OBJECT_ENABLE FALSE
   #define EIP_IA_INPUT_EXT_INSTANCE_OBJECT_VALUE  0x0006
#endif

/*
** Attribute 19: Listen only extended heartbeat instance number (UINT16 - 0x0000-0xFFFF)
*/
#ifndef EIP_IA_LISTEN_EXT_INSTANCE_OBJECT_ENABLE
   #define EIP_IA_LISTEN_EXT_INSTANCE_OBJECT_ENABLE   FALSE
   #define EIP_IA_LISTEN_EXT_INSTANCE_OBJECT_VALUE    0x0007
#endif

/*
** Attribute 20: Interface label port 1 (Array of CHAR)
*/
#ifndef EIP_IA_IF_LABEL_PORT_1_ENABLE
   #define EIP_IA_IF_LABEL_PORT_1_ENABLE           FALSE
   #define EIP_IA_IF_LABEL_PORT_1_VALUE            "Port 1"
#endif

/*
** Attribute 21: Interface label port 2 (Array of CHAR)
*/
#ifndef EIP_IA_IF_LABEL_PORT_2_ENABLE
   #define EIP_IA_IF_LABEL_PORT_2_ENABLE           FALSE
   #define EIP_IA_IF_LABEL_PORT_2_VALUE            "Port 2"
#endif

/*
** Attribute 22: Interface label internal port (Array of CHAR)
*/
#ifndef EIP_IA_IF_LABEL_PORT_INT_ENABLE
   #define EIP_IA_IF_LABEL_PORT_INT_ENABLE         FALSE
   #define EIP_IA_IF_LABEL_PORT_INT_VALUE          "Internal"
#endif

/*
** Attribute 23: Enable CIP request forwarding extended (BOOL - TRUE/FALSE)
** Attribute not used in the 40-series
*/
#ifndef EIP_IA_ENABLE_APP_CIP_OBJECTS_EXT_ENABLE
   #define EIP_IA_ENABLE_APP_CIP_OBJECTS_EXT_ENABLE   FALSE
   #define EIP_IA_ENABLE_APP_CIP_OBJECTS_EXT_VALUE    FALSE
#endif

/*
** Attribute 24: Prepend producing profile instance (UINT16 - 0x0000-0xFFFF)
** Attribute not used in the 40-series
*/
#ifndef EIP_IA_PREPEND_PRODUCING_ENABLE
   #define EIP_IA_PREPEND_PRODUCING_ENABLE         FALSE
   #define EIP_IA_PREPEND_PRODUCING_VALUE          0x0000
#endif

/*
** Attribute 25: Prepend consuming profile instance (UINT16 - 0x0000-0xFFFF)
** Attribute not used in the 40-series
*/
#ifndef EIP_IA_PREPEND_CONSUMING_ENABLE
   #define EIP_IA_PREPEND_CONSUMING_ENABLE         FALSE
   #define EIP_IA_PREPEND_CONSUMING_VALUE          0x0000
#endif

/*
** Attribute 26: Enable EtherNet/IP QuickConnect (BOOL - TRUE/FALSE)
*/
#ifndef EIP_IA_ENABLE_EIP_QC_ENABLE
   #define EIP_IA_ENABLE_EIP_QC_ENABLE             FALSE
   #define EIP_IA_ENABLE_EIP_QC_VALUE              FALSE
#endif

/*
** Attribute 27: Producing Instance Map (Array of eip_InstanceMapType)
** Attribute not used in the 40-series
*/
#ifndef EIP_IA_PROD_INSTANCE_MAP_ENABLE
   #define EIP_IA_PROD_INSTANCE_MAP_ENABLE         FALSE
   #define EIP_IA_PROD_INSTANCE_MAP_VALUE          { {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1} }
   #define EIP_IA_PROD_INSTANCE_MAP_ARRAY_SIZE     6
#endif

/*
** Attribute 28: Consuming Instance Map (Array of eip_InstanceMapType)
** Attribute not used in the 40-series
*/
#ifndef EIP_IA_CONS_INSTANCE_MAP_ENABLE
   #define EIP_IA_CONS_INSTANCE_MAP_ENABLE         FALSE
   #define EIP_IA_CONS_INSTANCE_MAP_VALUE          { {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1}, {0, 1} }
   #define EIP_IA_CONS_INSTANCE_MAP_ARRAY_SIZE     6
#endif

/*
** Attribute 29: Ignore Sequence Count Check (BOOL -TRUE/FALSE)
*/
#ifndef EIP_IA_IGNORE_SEQ_COUNT_CHECK_ENABLE
   #define EIP_IA_IGNORE_SEQ_COUNT_CHECK_ENABLE    FALSE
   #define EIP_IA_IGNORE_SEQ_COUNT_CHECK_VALUE     FALSE
#endif

/*
** Attribute 30: ABCC ADI Object number (UINT16 - 0x0064-0x00C7, 0x0300-0x04FF)
*/
#ifndef EIP_IA_ABCC_ADI_OBJECT_ENABLE
   #define EIP_IA_ABCC_ADI_OBJECT_ENABLE           FALSE
   #define EIP_IA_ABCC_ADI_OBJECT_VALUE            0x00A2
#endif

/*
** Attribute 31: Enable DLR (BOOL - TRUE/FALSE)
*/
#ifndef EIP_IA_ABCC_ENABLE_DLR_ENABLE
   #define EIP_IA_ABCC_ENABLE_DLR_ENABLE           FALSE
   #define EIP_IA_ABCC_ENABLE_DLR_VALUE            TRUE
#endif

#endif /* #if EIP_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** Ethernet Object (0xF9)
**------------------------------------------------------------------------------
*/

#if ETN_OBJ_ENABLE
/*
** Attribute 2: Enable Host IP Configuration Protocol  (BOOL - TRUE/FALSE)
*/
#ifndef ETN_IA_ENABLE_HICP_ENABLE
   #define ETN_IA_ENABLE_HICP_ENABLE               FALSE
   #define ETN_IA_ENABLE_HICP_VALUE                TRUE
#endif

/*
** Attribute 3: Enable Web Server  (BOOL - TRUE/FALSE)
*/
#ifndef ETN_IA_ENABLE_WEB_ENABLE
   #define ETN_IA_ENABLE_WEB_ENABLE                FALSE
   #define ETN_IA_ENABLE_WEB_VALUE                 TRUE
#endif

/*
** Attribute 4: Enable ModbusTCP  (BOOL - TRUE/FALSE). Reserved in 40-series.
*/
#ifndef ETN_IA_ENABLE_MOD_TCP_ENABLE
   #define ETN_IA_ENABLE_MOD_TCP_ENABLE            FALSE
   #define ETN_IA_ENABLE_MOD_TCP_VALUE             TRUE
#endif

/*
** Attribute 5: Enable Web ADI access  (BOOL - TRUE/FALSE)
*/
#ifndef ETN_IA_ENABLE_WEB_ADI_ACCESS_ENABLE
   #define ETN_IA_ENABLE_WEB_ADI_ACCESS_ENABLE     FALSE
   #define ETN_IA_ENABLE_WEB_ADI_ACCESS_VALUE      TRUE
#endif

/*
** Attribute 6: Enable FTP server  (BOOL - TRUE/FALSE)
*/
#ifndef ETN_IA_ENABLE_FTP_ENABLE
   #define ETN_IA_ENABLE_FTP_ENABLE                FALSE
   #define ETN_IA_ENABLE_FTP_VALUE                 TRUE
#endif

/*
** Attribute 7: Enable admin mode  (BOOL - TRUE/FALSE)
*/
#ifndef ETN_IA_ENABLE_ADMIN_MODE_ENABLE
   #define ETN_IA_ENABLE_ADMIN_MODE_ENABLE         FALSE
   #define ETN_IA_ENABLE_ADMIN_MODE_VALUE          FALSE
#endif

/*
** Attribute 8: Network status  (UINT16)
*/
#ifndef ETN_IA_NETWORK_STATUS_ENABLE
   #define ETN_IA_NETWORK_STATUS_ENABLE            FALSE
#endif

/*
** Attribute 9: Port 1 MAC Address (Array[6] of UINT8)
*/
#ifndef ETN_IA_PORT1_MAC_ADDRESS_ENABLE
   #define ETN_IA_PORT1_MAC_ADDRESS_ENABLE         TRUE
   #define ETN_IA_PORT1_MAC_ADDRESS_VALUE          "\x00\x30\x11\x00\x00\x01"
#endif

/*
** Attribute 10: Port 2 MAC Address (Array[6] of UINT8)
*/
#ifndef ETN_IA_PORT2_MAC_ADDRESS_ENABLE
   #define ETN_IA_PORT2_MAC_ADDRESS_ENABLE         TRUE
   #define ETN_IA_PORT2_MAC_ADDRESS_VALUE          "\x00\x30\x11\x00\x00\x02"
#endif

/*
** Attribute 11: Enable Address Conflict Detection   (BOOL - TRUE/FALSE)
*/
#ifndef ETN_IA_ENABLE_ACD_ENABLE
   #define ETN_IA_ENABLE_ACD_ENABLE                FALSE
   #define ETN_IA_ENABLE_ACD_VALUE                 TRUE
#endif

/*
** Attribute 12: Enable Port 1 state   (ENUM)
*/
#ifndef ETN_IA_PORT1_STATE_ENABLE
   #define ETN_IA_PORT1_STATE_ENABLE               FALSE
   #define ETN_IA_PORT1_STATE_VALUE                ABP_ETN_IA_PORT_STATE_ENABLE
#endif

/*
** Attribute 13: Enable Port 2 state   (ENUM)
*/
#ifndef ETN_IA_PORT2_STATE_ENABLE
   #define ETN_IA_PORT2_STATE_ENABLE               FALSE
   #define ETN_IA_PORT2_STATE_VALUE                ABP_ETN_IA_PORT_STATE_ENABLE
#endif

/*
** Attribute 14: Enable web update  (BOOL - TRUE/FALSE).
*/
#ifndef ETN_IA_ENABLE_WEB_UPDATE_ENABLE
   #define ETN_IA_ENABLE_WEB_UPDATE_ENABLE         FALSE
   #define ETN_IA_ENABLE_WEB_UPDATE_VALUE          TRUE
#endif

/*
** Attribute 15: Enable HICP reset (BOOL - TRUE/FALSE)
*/
#ifndef ETN_IA_ENABLE_HICP_RESET_ENABLE
   #define ETN_IA_ENABLE_HICP_RESET_ENABLE         FALSE
   #define ETN_IA_ENABLE_HICP_RESET_VALUE          FALSE
#endif

/*
** Attribute 16: IP Configuration  (Struct of {UINT32, UINT32, UINT32})
*/
#ifndef ETN_IA_IP_CONFIGURATION_ENABLE
   #define ETN_IA_IP_CONFIGURATION_ENABLE          FALSE
#endif

/*
** Attribute 17: IP address byte 0-2 (Array[3] of UINT8)
*/
#ifndef ETN_IA_IP_ADDRESS_BYTE_0_2_ENABLE
   #define ETN_IA_IP_ADDRESS_BYTE_0_2_ENABLE       FALSE
   #define ETN_IA_IP_ADDRESS_BYTE_0_2_VALUE        "\xC0\xA8\x00"
#endif

/*
** Attribute 18: Ethernet PHY Configuration (Array of BITS16)
*/
#ifndef ETN_IA_ETH_PHY_CONFIG_ENABLE
   #define ETN_IA_ETH_PHY_CONFIG_ENABLE            FALSE
   #define ETN_IA_ETH_PHY_CONFIG_PORT1_VALUE       0x0001
   #define ETN_IA_ETH_PHY_CONFIG_PORT2_VALUE       0x0001
#endif

/*
** Attribute 20: SNMP read-only community string (Array of CHAR - {0x00-0xFF})
*/
#ifndef ETN_IA_SNMP_READ_ONLY_ENABLE
   #define ETN_IA_SNMP_READ_ONLY_ENABLE            FALSE
   #define ETN_IA_SNMP_READ_ONLY_VALUE             "public"
#endif

/*
** Attribute 21: SNMP read-write community string (Array of CHAR - {0x00-0xFF})
*/
#ifndef ETN_IA_SNMP_READ_WRITE_ENABLE
   #define ETN_IA_SNMP_READ_WRITE_ENABLE           FALSE
   #define ETN_IA_SNMP_READ_WRITE_VALUE            "private"
#endif

/*
** Attribute 22: DHCP Option 61 source (ENUM)
*/
#ifndef ETN_IA_DHCP_OPTION_61_SOURCE_ENABLE
   #define ETN_IA_DHCP_OPTION_61_SOURCE_ENABLE     FALSE
   #define ETN_IA_DHCP_OPTION_61_SOURCE_VALUE      ABP_ETN_IA_DHCP_OPTION_61_SOURCE_DISABLE
#endif

/*
** Attribute 23: DHCP Option 61 generic string (Array of UINT8)
*/
#ifndef ETN_IA_DHCP_OPTION_61_GENERIC_STR_ENABLE
   #define ETN_IA_DHCP_OPTION_61_GENERIC_STR_ENABLE       FALSE
   #define ETN_IA_DHCP_OPTION_61_GENERIC_STR_VALUE        "\x01\x00\x30\x11\x00\x00\x01"
   #define ETN_IA_DHCP_OPTION_61_GENERIC_STR_ARRAY_SIZE   7
#endif

/*
** Attribute 24: Enable DHCP Client (BOOL - TRUE/FALSE)
*/
#ifndef ETN_IA_ENABLE_DHCP_CLIENT_ENABLE
   #define ETN_IA_ENABLE_DHCP_CLIENT_ENABLE        FALSE
   #define ETN_IA_ENABLE_DHCP_CLIENT_VALUE         TRUE
#endif

#ifndef ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK
   #define ETN_OBJ_USE_SET_ATTR_SUCCESS_CALLBACK   FALSE
#endif

#endif /* #if ETN_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** Modbus Object (0xFA)
**------------------------------------------------------------------------------
*/

#if MOD_OBJ_ENABLE
/*
** Attribute 9: No. of ADI indexing bits (UINT8 - 0x00-0xFF)
*/
#ifndef MOD_IA_ADI_INDEXING_BITS_ENABLE
   #define MOD_IA_ADI_INDEXING_BITS_ENABLE         FALSE
   #define MOD_IA_ADI_INDEXING_BITS_VALUE          0x00
#endif

/*
** Attribute 10: Enable Modbus Message forwarding (BOOL - TRUE/FALSE)
*/
#ifndef MOD_IA_MESSAGE_FORWARDING_ENABLE
   #define MOD_IA_MESSAGE_FORWARDING_ENABLE        FALSE
   #define MOD_IA_MESSAGE_FORWARDING_VALUE         FALSE
#endif

/*
** Attribute 11: Modbus read/write registers command offset (Array of SINT16 - {0x0000-0xFFFF})
*/
#ifndef MOD_IA_RW_OFFSET_ENABLE
   #define MOD_IA_RW_OFFSET_ENABLE                 FALSE
   #define MOD_IA_RW_OFFSET_READ_VALUE             0x0000
   #define MOD_IA_RW_OFFSET_WRITE_VALUE            0x0000
#endif

#endif /* #if MOD_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** DeviceNet Object (0xFC)
**------------------------------------------------------------------------------
*/

#if DEV_OBJ_ENABLE
/*
** Attribute 7: Producing instance number (UINT16 - 0x0001-0xFFFF)
*/
#ifndef DEV_IA_PROD_INSTANCE_ENABLE
   #define DEV_IA_PROD_INSTANCE_ENABLE             FALSE
   #define DEV_IA_PROD_INSTANCE_VALUE              0x0064
#endif

/*
** Attribute 8: Consuming instance number (UINT16 - 0x0001-0xFFFF)
*/
#ifndef DEV_IA_CONS_INSTANCE_ENABLE
   #define DEV_IA_CONS_INSTANCE_ENABLE             FALSE
   #define DEV_IA_CONS_INSTANCE_VALUE              0x0096
#endif

/*
** Attribute 9: Enable address from net (BOOL - TRUE/FALSE)
*/
#ifndef DEV_IA_ADDRESS_FROM_NET_ENABLE
   #define DEV_IA_ADDRESS_FROM_NET_ENABLE          FALSE
   #define DEV_IA_ADDRESS_FROM_NET_VALUE           FALSE
#endif

/*
** Attribute 10: Enable baud rate from net (BOOL - TRUE/FALSE)
*/
#ifndef DEV_IA_BAUD_RATE_FROM_NET_ENABLE
   #define DEV_IA_BAUD_RATE_FROM_NET_ENABLE        FALSE
   #define DEV_IA_BAUD_RATE_FROM_NET_VALUE         FALSE
#endif

/*
** Attribute 11: Enable CIP request forwarding (BOOL - TRUE/FALSE)
*/
#ifndef DEV_IA_ENABLE_APP_CIP_OBJECTS_ENABLE
   #define DEV_IA_ENABLE_APP_CIP_OBJECTS_ENABLE    FALSE
   #define DEV_IA_ENABLE_APP_CIP_OBJECTS_VALUE     FALSE
#endif

/*
** Attribute 12: Enable Parameter object (BOOL - TRUE/FALSE)
*/
#ifndef DEV_IA_ENABLE_PARAM_OBJECT_ENABLE
   #define DEV_IA_ENABLE_PARAM_OBJECT_ENABLE       FALSE
   #define DEV_IA_ENABLE_PARAM_OBJECT_VALUE        FALSE
#endif

/*
** Attribute 13: Enable Quick Connect (BOOL - TRUE/FALSE)
*/
#ifndef DEV_IA_ENABLE_QUICK_CONNECT_ENABLE
   #define DEV_IA_ENABLE_QUICK_CONNECT_ENABLE      FALSE
   #define DEV_IA_ENABLE_QUICK_CONNECT_VALUE       FALSE
#endif

/*
** Attribute 18: Prepend producing profile instance (UINT16 - 0x0000-0xFFFF)
*/
#ifndef DEV_IA_PREPEND_PRODUCING_ENABLE
   #define DEV_IA_PREPEND_PRODUCING_ENABLE         FALSE
   #define DEV_IA_PREPEND_PRODUCING_VALUE          0x0000
#endif

/*
** Attribute 19: Prepend consuming profile instance (UINT16 - 0x0000-0xFFFF)
*/
#ifndef DEV_IA_PREPEND_CONSUMING_ENABLE
   #define DEV_IA_PREPEND_CONSUMING_ENABLE         FALSE
   #define DEV_IA_PREPEND_CONSUMING_VALUE          0x0000
#endif

/*
** Attribute 20: ABCC ADI Object number (UINT16 - 0x0064-0x00C7, 0x0300-0x04FF)
*/
#ifndef DEV_IA_ABCC_ADI_OBJECT_ENABLE
   #define DEV_IA_ABCC_ADI_OBJECT_ENABLE           FALSE
   #define DEV_IA_ABCC_ADI_OBJECT_VALUE            0x00A2
#endif

#endif /* #if DEV_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** PROFIBUS DPV1 Object (0xFD)
**------------------------------------------------------------------------------
*/

#if DPV1_OBJ_ENABLE
/*
** Attribute 2: Parameterization data (Array of UINT8)
*/
#ifndef DPV1_IA_PRM_DATA_ENABLE
   #define DPV1_IA_PRM_DATA_ENABLE                 FALSE
   #define DPV1_IA_PRM_DATA_VALUE                  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
   #define DPV1_IA_PRM_DATA_ARRAY_SIZE             10
#endif

/*
** Attribute 3: Expected configuration (Array of UINT8)
*/
#ifndef DPV1_IA_EXPECTED_CFG_DATA_ENABLE
   #define DPV1_IA_EXPECTED_CFG_DATA_ENABLE        FALSE
   #define DPV1_IA_EXPECTED_CFG_DATA_VALUE         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
   #define DPV1_IA_EXPECTED_CFG_DATA_ARRAY_SIZE    10
#endif

/*
** Attribute 4: SSA enabled (BOOL - TRUE/FALSE)
*/
#ifndef DPV1_IA_SSA_ENABLED_ENABLE
   #define DPV1_IA_SSA_ENABLED_ENABLE              FALSE
   #define DPV1_IA_SSA_ENABLED_VALUE               FALSE
#endif

/*
** Attribute 13: Revision counter (UINT16 - 0x0000-0xFFFF)
*/
#ifndef DPV1_IA_REV_COUNTER_ENABLE
   #define DPV1_IA_REV_COUNTER_ENABLE              FALSE
   #define DPV1_IA_REV_COUNTER_VALUE               0xFFFF
#endif

/*
** Attribute 14: Profile ID (UINT16 - 0x0000-0xFFFF)
*/
#ifndef DPV1_IA_PROFILE_ID_ENABLE
   #define DPV1_IA_PROFILE_ID_ENABLE               FALSE
   #define DPV1_IA_PROFILE_ID_VALUE                0xFFFF
#endif

/*
** Attribute 15: Profile specific type (UINT16 - 1-6)
*/
#ifndef DPV1_IA_PROFILE_SPEC_TYPE_ENABLE
   #define DPV1_IA_PROFILE_SPEC_TYPE_ENABLE        FALSE
   #define DPV1_IA_PROFILE_SPEC_TYPE_VALUE         0x0004
#endif

/*
** Attribute 18: IM header (Array of UINT8)
*/
#ifndef DPV1_IA_IM_HEADER_ENABLE
   #define DPV1_IA_IM_HEADER_ENABLE                FALSE
   #define DPV1_IA_IM_HEADER_VALUE                 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
   #define DPV1_IA_IM_HEADER_ARRAY_SIZE            10
#endif

#endif /* #if DPV1_OBJ_ENABLE */

/*------------------------------------------------------------------------------
** Application data Object (0xFE)
**------------------------------------------------------------------------------
*/

/*
** These defines shall be set to the max number of process data mapping entries
** that will be required by the implementation.
** Note that each mapping entry represents a 'range' of elements from one ADI,
** meaning that if only some elements from a multi-element ADI are to be mapped
** it will require as many mapping entries as there are separate and non-
** continuous ranges of elements to map.
** Do not forget to consider remap scenarios if ABCC_CFG_REMAP_SUPPORT_ENABLED
** is enabled in abcc_drv_cfg.h.
*/
#ifndef AD_MAX_NUM_WRITE_MAP_ENTRIES
   #define AD_MAX_NUM_WRITE_MAP_ENTRIES             ( 64 )
#endif
#ifndef AD_MAX_NUM_READ_MAP_ENTRIES
   #define AD_MAX_NUM_READ_MAP_ENTRIES              ( 64 )
#endif

/*
** Attributes 5, 6, 7: Min, max and default attributes  - (BOOL - TRUE/FALSE)
**
** Enabling this will also enable and include functions that performs runtime
** min/max range checks for 'SetAttribute' operations targeting ADI elements,
** which will increase code ROM consumption.
** If disabled no range checks will be made, and the min/max will be the full
** range of each data type.
*/
#ifndef AD_IA_MIN_MAX_DEFAULT_ENABLE
   #define AD_IA_MIN_MAX_DEFAULT_ENABLE            ( FALSE )
#endif

/*------------------------------------------------------------------------------
** Application Object (0xFF)
**------------------------------------------------------------------------------
*/
#ifndef APP_OBJ_ENABLE
   #define APP_OBJ_ENABLE                          TRUE
#endif

#if APP_OBJ_ENABLE
/*
** Attribute 1: Configured (BOOL - TRUE/FALSE)
*/
#ifndef APP_IA_CONFIGURED_ENABLE
   #define APP_IA_CONFIGURED_ENABLE                TRUE
   #define APP_IA_CONFIGURED_VALUE                 FALSE
#endif

/*
** Attribute 2: Supported languages (Array of ENUM - {ABP_LANG_*})
*/
#ifndef APP_IA_SUP_LANG_ENABLE
   #define APP_IA_SUP_LANG_ENABLE                  TRUE
   #define APP_IA_SUP_LANG_VALUE                   { ABP_LANG_ENG }
   #define APP_IA_SUP_LANG_ARRAY_SIZE              1
#endif

/*
** Attribute 3: Serial number (UINT32 - function/variable/0x00000000-0xFFFFFFFF)
*/
#ifndef APP_IA_SER_NUM_ENABLE
   #define APP_IA_SER_NUM_ENABLE                   FALSE
   #define APP_IA_SER_NUM_VALUE                    0
#endif

/*
** Attribute 4: Parameter control sum (Array[16] of UINT8, {0x00-0xFF})
*/
#ifndef APP_IA_PAR_CRTL_SUM_ENABLE
   #define APP_IA_PAR_CRTL_SUM_ENABLE              FALSE
   #define APP_IA_PAR_CRTL_SUM_VALUE               { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#endif

/*
** Attribute 5: Candidate firmware available (BOOL - TRUE/FALSE)
*/
#ifndef APP_IA_FW_AVAILABLE_ENABLE
   #define APP_IA_FW_AVAILABLE_ENABLE              TRUE
   #define APP_IA_FW_AVAILABLE_VALUE               FALSE
#endif

/*
** Attribute 6: Hardware configurable address (BOOL - TRUE/FALSE)
*/
#ifndef APP_IA_HW_CONF_ADDR_ENABLE
   #define APP_IA_HW_CONF_ADDR_ENABLE              TRUE
   #define APP_IA_HW_CONF_ADDR_VALUE               FALSE
#endif

/*
** Application Object command support: "Get Data Notification", 0x13, (BOOL - TRUE/FALSE)
*/
#ifndef APP_CMD_GET_DATA_NOTIFICATION_ENABLE
#if MQTT_OBJ_ENABLE
   #define APP_CMD_GET_DATA_NOTIFICATION_ENABLE    TRUE
#else
   #define APP_CMD_GET_DATA_NOTIFICATION_ENABLE    FALSE
#endif
#endif

#endif /* #if APP_OBJ_ENABLE */


/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

#include "abcc_identification.h"

#endif  /* inclusion lock */
