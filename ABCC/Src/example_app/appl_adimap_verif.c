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
** Example of an ADI setup covering all data types and structures.
********************************************************************************
********************************************************************************
*/

#include "appl_adi_config.h"
#include "abcc.h"
#include "app_obj.h"
#include "abp_asm.h"
#include "ad_obj.h"

#if ( APPL_ACTIVE_ADI_SETUP == 0xFFFF )

#if !ABCC_CFG_STRUCT_DATA_TYPE
   #error ABCC_CFG_STRUCT_DATA_TYPE in abcc_drv_cfg.h must be enabled
#endif

#if !ABCC_CFG_ADI_GET_SET_CALLBACK
   #error ABCC_CFG_ADI_GET_SET_CALLBACK in abcc_drv_cfg.h must be enabled
#endif

#if !ABCC_CFG_REMAP_SUPPORT_ENABLED
   #error ABCC_CFG_REMAP_SUPPORT_ENABLED in abcc_drv_cfg.h must be enabled
#endif

#if !AD_IA_MIN_MAX_DEFAULT_ENABLE
   #error AD_IA_MIN_MAX_DEFAULT_ENABLE in abcc_drv_cfg.h must be enabled
#endif

#if !MQTT_OBJ_ENABLE
   #error MQTT_OBJ_ENABLE in abcc_obj_cfg.h must be enabled
#endif

#if !MQTT_IA_MODE_ENABLE
   #error MQTT_IA_MODE_ENABLE in abcc_obj_cfg.h must be enabled
#endif

#if( MQTT_IA_MODE_VALUE != 0x01 )
   #error MQTT_IA_MODE_VALUE in abcc_obj_cfg.h must be 1
#endif

#if( APP_ON_DATA_NOTIFICATION_REQ_CBF != APP_VerifOnDataNotificationReqCfn )
   #error APP_ON_DATA_NOTIFICATION_REQ_CBF must be set to APP_VerifOnDataNotificationReqCfn
#endif

#if ABCC_CFG_CHECK_STACK_USAGE
extern UINT32 APPL_MaxStackUsage( void );
#endif

/*******************************************************************************
** Constants
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Access descriptor for the ADIs
**------------------------------------------------------------------------------
*/
#define APPL_READ_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_READ_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_SET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_READ_PD )

#define APPL_WRITE_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |          \
                                          ABP_APPD_DESCR_MAPPABLE_WRITE_PD )

#define APPL_NOT_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS )

#define APPL_NOT_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_SET_ACCESS )

#define APPL_ALL_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |                     \
                               ABP_APPD_DESCR_SET_ACCESS |                     \
                               ABP_APPD_DESCR_MAPPABLE_READ_PD |               \
                               ABP_APPD_DESCR_MAPPABLE_WRITE_PD )

/*******************************************************************************
** Typedefs
********************************************************************************
*/
typedef struct APPL_MappableStruct
{
   UINT32  lUint32;
   INT32   lInt32;
   UINT16  iUint16;
   INT16   iInt16;
   UINT16  iBits16;
   UINT8   bUint8;
   INT8    bInt8;
   UINT8   bBits8;
   UINT8   bBitTypes[ 4 ];
   UINT16  iBitTypes[ 2 ];
}
APPL_MappableStructType;

typedef struct APPL_NotMappableStruct
{
   char  acCharString[ 6 ];
   UINT8 abOctetString[ 10 ];
}
APPL_NotMappableStructType;

typedef struct APPL_AdiMinMaxVerif
{
   UINT8   bUint8;
   INT8    bInt8;
   UINT16  iUint16;
   INT16   iInt16;
   UINT32  lUint32;
   INT32   lInt32;
   FLOAT32 rFloat;
   UINT8   bOctet;
   UINT8   eEnum;
}
APPL_AdiMinMaxVerifType;


/*******************************************************************************
** Private Globals
********************************************************************************
*/
#if ABCC_CFG_CHECK_STACK_USAGE
static void GetStackUsage( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex );
static UINT32 appl_lStackUsage = 0;
#endif

static UINT16 appl_iDataNotifResources = 0;
static void AdiChanged( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex );

/*------------------------------------------------------------------------------
** Variable data sources
**------------------------------------------------------------------------------
*/
static BOOL8                   appl_fBool8 = TRUE;
static INT8                    appl_bInt8 = 0x62;
static INT16                   appl_iInt16 = 1893;
static INT32                   appl_lInt32 = 124076833;
static UINT8                   appl_bUint8 = 0x15;
static UINT16                  appl_iUint16 = 0x1234;
static UINT32                  appl_lUint32 = 0x12345678;
static char                    appl_cChar   = 'a';
static UINT8                   appl_bEnum = 5;
static UINT8                   appl_bBits8 = 0;
static UINT16                  appl_iBits16 = 0;
static UINT32                  appl_lBits32 = 0;
static UINT8                   appl_bOctet = 0xAF;
static FLOAT32                 appl_rFloat = 59.09347152709961;
/* PAD0-16 have no variables */
static BOOL                    appl_fBool1 = FALSE;
static UINT8                   appl_bBit1 = 0x1;
static UINT8                   appl_bBit2 = 0x2;
static UINT8                   appl_bBit3 = 0x1;
static UINT8                   appl_bBit4 = 0x5;
static UINT8                   appl_bBit5 = 0x15;
static UINT8                   appl_bBit6 = 0x36;
static UINT8                   appl_bBit7 = 0x42;

/*------------------------------------------------------------------------------
** Array data sources
**------------------------------------------------------------------------------
*/
#ifdef ABCC_SYS_16_BIT_CHAR
static UINT16                  appl_afBool8Array[ 4 ] = { 0x0001, 0x0001, 0x0001, 0x0001 };
#else
static BOOL8                   appl_afBool8Array[ 8 ] = { TRUE, FALSE, TRUE, FALSE, TRUE, FALSE, TRUE, FALSE };
#endif
#ifdef ABCC_SYS_16_BIT_CHAR
static INT16                   appl_abInt8Array[ 4 ] = { 0xF162, 0xF162, 0xF162, 0xF162 };
#else
static INT8                    appl_abInt8Array[ 8 ] = { 0x62, -0x0F, 0x62, -0x0F, 0x62, -0x0F, 0x62, -0x0F };
#endif
static INT16                   appl_aiInt16Array[ 8 ] ={ 1893, -0x00FF, 1893, -0x00FF, 1893, -0x00FF, 1893, -0x00FF };
static INT32                   appl_alInt32Array[ 8 ] = { 124076833, 80080899, 124076833, 80080899, 124076833, 80080899, 124076833, 80080899 };
#ifdef ABCC_SYS_16_BIT_CHAR
static UINT16                  appl_abUint8Array[ 4 ] = { 0xF015, 0xF015, 0xF015, 0xF015 };
#else
static UINT8                   appl_abUint8Array[ 8 ] = { 0x15, 0xF0, 0x15, 0xF0, 0x15, 0xF0, 0x15, 0xF0 };
#endif
static UINT16                  appl_aiUint16Array[ 8 ] = { 0x1234, 0x0010, 0x1234, 0x0010, 0x1234, 0x0010, 0x1234, 0x0010 };
static UINT32                  appl_alUint32Array[ 8 ] = { 0x12345678, 0x87654321, 0x12345678, 0x87654321, 0x12345678, 0x87654321, 0x12345678, 0x87654321 };
#ifdef ABCC_SYS_16_BIT_CHAR
static char                    appl_acCharArray[ 8 ] = { 0x6554, 0x7473, 0x7453, 0x3172 };
#else
static char                    appl_acCharArray[ 8 + 1 ] = "TestStr1";
#endif
#ifdef ABCC_SYS_16_BIT_CHAR
static UINT16                  appl_abEnumArray[ 4 ] = { 0x0705, 0x0705, 0x0705, 0x0705 };
#else
static UINT8                   appl_abEnumArray[ 8 ] = { 5, 7, 5, 7, 5, 7, 5, 7 };
#endif
#ifdef ABCC_SYS_16_BIT_CHAR
static UINT16                  appl_abBits8Array[ 4 ] = { 0xFF00, 0xFF00, 0xFF00, 0xFF00 };
#else
static UINT8                   appl_abBits8Array[ 8 ] = { 0, 255, 0, 255, 0, 255, 0, 255 };
#endif
static UINT16                  appl_aiBits16Array[ 8 ] = { 0, 65535, 0, 65535, 0, 65535, 0, 65535 };
static UINT32                  appl_alBits32Array[ 8 ] = { 0, 0xFCFCFAFA, 0, 0xFCFCFAFA, 0, 0xFCFCFAFA, 0, 0xFCFCFAFA };
#ifdef ABCC_SYS_16_BIT_CHAR
static UINT16                  appl_abOctetArray[ 4 ] = { 0x0AAF, 0x0AAF, 0x0AAF, 0x0AAF };
#else
static UINT8                   appl_abOctetArray[ 8 ] = { 0xAF, 0x0A, 0xAF, 0x0A, 0xAF, 0x0A, 0xAF, 0x0A };
#endif
static FLOAT32                 appl_arFloatArray[ 8 ] = { 59.09347152709961, -43.454322814941406, 59.09347152709961, -43.454322814941406, 59.09347152709961, -43.454322814941406, 59.09347152709961, -43.454322814941406 };
/* PAD0-16 have no array variables */
static BOOL                    appl_afBool1Array = 0xAA;
static UINT8                   appl_abBit1Array = 0xAA;
static UINT16                  appl_aiBit2Array = 0xCCCC;
static UINT32                  appl_alBit3Array = 0xE38E38;
static UINT32                  appl_alBit4Array = 0xF0F0F0F0;
static UINT16                  appl_aiBit5Array[ 3 ] = { 0x83E0, 0x3E0F, 0x00F8 };
static UINT16                  appl_aiBit6Array[ 3 ] = { 0x0FC0, 0xC0FC, 0xFC0F };
static UINT16                  appl_aiBit7Array[ 4 ] = { 0x3F80, 0x0FE0, 0x03F8, 0x00FE };

/*------------------------------------------------------------------------------
** Variables with subelements data sources
**------------------------------------------------------------------------------
*/
#ifdef ABCC_SYS_16_BIT_CHAR
static UINT16 appl_abOctetString[] = { 0x0505, 0x0505, 0x0505, 0x0505, 0x0505 };
#else
static UINT8 appl_abOctetString[ 10 ] = { 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05 };
#endif
#ifdef ABCC_SYS_16_BIT_CHAR
static UINT16 appl_acCharString[] =  { 0x6e41,0x6279,0x7375, 0 };
#else
static char appl_acCharString[ 6 ] = "Anybus";
#endif

/*------------------------------------------------------------------------------
** Structure data sources
**------------------------------------------------------------------------------
*/
static APPL_MappableStructType appl_sStruct1 = { 4, 0, 6, 0, 1, 1, 0, 0, { 0, 0, 0, 0 }, { 0, 0 } };
static APPL_MappableStructType appl_sStruct2 = { 4, 0, 6, 0, 1, 1, 0, 0, { 0, 0, 0, 0 }, { 0, 0 } };
#ifdef ABCC_SYS_16_BIT_CHAR
static APPL_NotMappableStructType appl_sStruct3 = { { 0x6e41, 0x6279, 0x7375, 0 }, { 0x0201, 0x0403, 0x0605, 0x0807, 0x0a09, 0, 0, 0, 0, 0 } };
#else
static APPL_NotMappableStructType appl_sStruct3 = { "Anybus", { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
#endif
static APPL_AdiMinMaxVerifType     appl_sAdi4 = { 7, -0x0E, 5, -0x00DD, 2, -0x000000A1, -30.0, 0x10, 7 };
static APPL_AdiMinMaxVerifType     appl_sAdi5 = { 7, -0x0E, 5, -0x00DD, 2, -0x000000A1, -30.0, 0x10, 7 };
static UINT8                   appl_sAdi6[ 2 ] = { 0, 0 };

static AD_ENUMStrType          appl_asEnumStrings[ 3 ] = { { 0, "String0" }, { 1, "String1" }, { 2, "String2" } };


static AD_BOOL8Type   appl_sBool8Prop   = { { 0, 1, 1 } };
static AD_SINT8Type   appl_sSint8Prop   = { { -0x0F, 0x7E, 0x62 } };
static AD_SINT16Type  appl_sSint16Prop  = { { -0x00FF, 0x0FFF, 0x0765 } };
static AD_SINT32Type  appl_sSint32Prop  = { { -0x000000FF, 0x0FFFFFFF, 0x07654321 } };
static AD_UINT8Type   appl_sUint8Prop   = { { 0x05, 0xF0, 0x15 } };
static AD_UINT16Type  appl_sUint16Prop  = { { 2, 0xFFF0, 0x1234 } };
static AD_UINT32Type  appl_sUint32Prop  = { { 1, 0xFFFFFFF0, 0x12345678 } };
static AD_ENUMType    appl_sEnumProp    = { { 2, 7, 5 }, 3, appl_asEnumStrings };
/* BITS8/16/32 doesn't have properties */
static AD_OctetType   appl_sOctetProp   = { { 0x05, 0xFE, 0x05 } };
static AD_FLOAT32Type appl_sFloat32Prop = { { -43.454322814941406, 345.83782958984375, 59.09347152709961 } };
/* PAD0-16, BOOL1, BIT1-7 doesn't have properties */

/*
** A write to this ADI will trigger a data notification with the following parameters
** appl_bDataSet               - Data set to publish
** appl_iDataNotifInst         - Instance of data set
** appl_DataNotifTmoLow/High  - Timestamp
*/
static UINT8 appl_bDataSet = 0;
static UINT16 appl_iDataNotifInst = 0;
static UINT32 appl_lDataNotifTimestampLow = 0;
static UINT32 appl_lDataNotifTimestampHigh = 0;

static const AD_StructDataType appl_DataNotifDataStruct[] =
{
   /* Index: 0 */  { "DataSet",       ABP_UINT8,  1, APPL_NOT_MAP_WRITE_ACCESS_DESC, 0, { { &appl_bDataSet, NULL } } },
   /* Index: 1 */  { "Instance",      ABP_UINT16, 1, APPL_NOT_MAP_WRITE_ACCESS_DESC, 0, { { &appl_iDataNotifInst, NULL } } },
   /* Index: 2 */  { "TimestampLow",  ABP_UINT32, 1, APPL_NOT_MAP_WRITE_ACCESS_DESC, 0, { { &appl_lDataNotifTimestampLow, NULL } } },
   /* Index: 3 */  { "TimestampHigh", ABP_UINT32, 1, APPL_NOT_MAP_WRITE_ACCESS_DESC, 0, { { &appl_lDataNotifTimestampHigh, NULL } } }
};


/*
** Structured ADI #1 (AD_StructDataType):
**-------------------------------------------------------------------------------------------------
** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
**-------------------------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_MappableStruct1[] =
{
   /* Index: 0 */  { "A", ABP_UINT32, 1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.lUint32,        NULL } } },
   /* Index: 1 */  { "B", ABP_SINT32, 1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.lInt32,         NULL } } },
   /* Index: 2 */  { "C", ABP_UINT16, 1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.iUint16,        NULL } } },
   /* Index: 3 */  { "D", ABP_SINT16, 1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.iInt16,         NULL } } },
   /* Index: 4 */  { "E", ABP_BITS16, 1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.iBits16,        NULL, } } },
   /* Index: 5 */  { "F", ABP_UINT8,  1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.bUint8,         NULL } } },
   /* Index: 6 */  { "G", ABP_SINT8,  1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.bInt8,          NULL } } },
   /* Index: 7 */  { "H", ABP_BITS8,  1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.bBits8,         NULL } } },
   /* Index: 8 */  { "I", ABP_PAD8,   1, APPL_ALL_ACCESS_DESC,  0,  { { NULL,                          NULL } } },
#ifdef ABCC_SYS_16_BIT_CHAR
   /* Index: 9 */  { "J", ABP_BIT1,   1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.iBitTypes[ 0 ], NULL } } },
   /* Index: 10 */ { "K", ABP_BIT2,   1, APPL_ALL_ACCESS_DESC,  1,  { { &appl_sStruct1.iBitTypes[ 0 ], NULL } } },
   /* Index: 11 */ { "L", ABP_BIT3,   1, APPL_ALL_ACCESS_DESC,  3,  { { &appl_sStruct1.iBitTypes[ 0 ], NULL } } },
   /* Index: 12 */ { "M", ABP_BIT4,   1, APPL_ALL_ACCESS_DESC,  6,  { { &appl_sStruct1.iBitTypes[ 0 ], NULL } } },
   /* Index: 13 */ { "N", ABP_BIT5,   1, APPL_ALL_ACCESS_DESC,  10, { { &appl_sStruct1.iBitTypes[ 0 ], NULL } } },
   /* Index: 14 */ { "O", ABP_BIT6,   1, APPL_ALL_ACCESS_DESC,  15, { { &appl_sStruct1.iBitTypes[ 0 ], NULL } } },
   /* Index: 15 */ { "P", ABP_BIT7,   1, APPL_ALL_ACCESS_DESC,  5,  { { &appl_sStruct1.iBitTypes[ 1 ], NULL } } },
   /* Index: 16 */ { "Q", ABP_PAD4,   1, APPL_ALL_ACCESS_DESC,  4,  { { NULL,                          NULL } } }
#else
   /* Index: 9 */  { "J", ABP_BIT1,   1, APPL_ALL_ACCESS_DESC,  0,  { { &appl_sStruct1.bBitTypes[ 0 ], NULL } } },
   /* Index: 10 */ { "K", ABP_BIT2,   1, APPL_ALL_ACCESS_DESC,  1,  { { &appl_sStruct1.bBitTypes[ 0 ], NULL } } },
   /* Index: 11 */ { "L", ABP_BIT3,   1, APPL_ALL_ACCESS_DESC,  3,  { { &appl_sStruct1.bBitTypes[ 0 ], NULL } } },
   /* Index: 12 */ { "M", ABP_BIT4,   1, APPL_ALL_ACCESS_DESC,  6,  { { &appl_sStruct1.bBitTypes[ 0 ], NULL } } },
   /* Index: 13 */ { "N", ABP_BIT5,   1, APPL_ALL_ACCESS_DESC,  2,  { { &appl_sStruct1.bBitTypes[ 1 ], NULL } } },
   /* Index: 14 */ { "O", ABP_BIT6,   1, APPL_ALL_ACCESS_DESC,  7,  { { &appl_sStruct1.bBitTypes[ 1 ], NULL } } },
   /* Index: 15 */ { "P", ABP_BIT7,   1, APPL_ALL_ACCESS_DESC,  5,  { { &appl_sStruct1.bBitTypes[ 2 ], NULL } } },
   /* Index: 16 */ { "Q", ABP_PAD4,   1, APPL_ALL_ACCESS_DESC,  4,  { { NULL ,                         NULL } } }
#endif
};

/*
** Structured ADI #2 (AD_StructDataType):
**-------------------------------------------------------------------------------------------------
** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
**-------------------------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_MappableStruct2[] =
{
   /* Index: 0 */  { "A", ABP_UINT32, 1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.lUint32,        NULL } } },
   /* Index: 1 */  { "B", ABP_SINT32, 1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.lInt32,         NULL } } },
   /* Index: 2 */  { "C", ABP_UINT16, 1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.iUint16,        NULL } } },
   /* Index: 3 */  { "D", ABP_SINT16, 1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.iInt16,         NULL } } },
   /* Index: 4 */  { "E", ABP_BITS16, 1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.iBits16,        NULL } } },
   /* Index: 5 */  { "F", ABP_UINT8,  1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.bUint8,         NULL } } },
   /* Index: 6 */  { "G", ABP_SINT8,  1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.bInt8,          NULL } } },
   /* Index: 7 */  { "H", ABP_BITS8,  1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.bBits8,         NULL } } },
   /* Index: 8 */  { "I", ABP_PAD8,   1, APPL_ALL_ACCESS_DESC, 0,  { { NULL,                          NULL } } },
#ifdef ABCC_SYS_16_BIT_CHAR
   /* Index: 9 */  { "J", ABP_BIT1,   1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.iBitTypes[ 0 ], NULL } } },
   /* Index: 10 */ { "K", ABP_BIT2,   1, APPL_ALL_ACCESS_DESC, 1,  { { &appl_sStruct2.iBitTypes[ 0 ], NULL } } },
   /* Index: 11 */ { "L", ABP_BIT3,   1, APPL_ALL_ACCESS_DESC, 3,  { { &appl_sStruct2.iBitTypes[ 0 ], NULL } } },
   /* Index: 12 */ { "M", ABP_BIT4,   1, APPL_ALL_ACCESS_DESC, 6,  { { &appl_sStruct2.iBitTypes[ 0 ], NULL } } },
   /* Index: 13 */ { "N", ABP_BIT5,   1, APPL_ALL_ACCESS_DESC, 10, { { &appl_sStruct2.iBitTypes[ 0 ], NULL } } },
   /* Index: 14 */ { "O", ABP_BIT6,   1, APPL_ALL_ACCESS_DESC, 15, { { &appl_sStruct2.iBitTypes[ 0 ], NULL } } },
   /* Index: 15 */ { "P", ABP_BIT7,   1, APPL_ALL_ACCESS_DESC, 5,  { { &appl_sStruct2.iBitTypes[ 1 ], NULL } } },
   /* Index: 16 */ { "Q", ABP_PAD4,   1, APPL_ALL_ACCESS_DESC, 4,  { { NULL,                          NULL } } }
#else
   /* Index: 9 */  { "J", ABP_BIT1,   1, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct2.bBitTypes[ 0 ], NULL } } },
   /* Index: 10 */ { "K", ABP_BIT2,   1, APPL_ALL_ACCESS_DESC, 1,  { { &appl_sStruct2.bBitTypes[ 0 ], NULL } } },
   /* Index: 11 */ { "L", ABP_BIT3,   1, APPL_ALL_ACCESS_DESC, 3,  { { &appl_sStruct2.bBitTypes[ 0 ], NULL } } },
   /* Index: 12 */ { "M", ABP_BIT4,   1, APPL_ALL_ACCESS_DESC, 6,  { { &appl_sStruct2.bBitTypes[ 0 ], NULL } } },
   /* Index: 13 */ { "N", ABP_BIT5,   1, APPL_ALL_ACCESS_DESC, 2,  { { &appl_sStruct2.bBitTypes[ 1 ], NULL } } },
   /* Index: 14 */ { "O", ABP_BIT6,   1, APPL_ALL_ACCESS_DESC, 7,  { { &appl_sStruct2.bBitTypes[ 1 ], NULL } } },
   /* Index: 15 */ { "P", ABP_BIT7,   1, APPL_ALL_ACCESS_DESC, 5,  { { &appl_sStruct2.bBitTypes[ 2 ], NULL } } },
   /* Index: 16 */ { "Q", ABP_PAD4,   1, APPL_ALL_ACCESS_DESC, 4,  { { NULL ,                         NULL } } }
#endif
};

/*
** Structured ADI #3 (AD_StructDataType):
**-------------------------------------------------------------------------------------------------
** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
**-------------------------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_NonMappableStruct3[] =
{
   /* Index: 0 */  { "A", ABP_CHAR,  6,  APPL_ALL_ACCESS_DESC, 0,  { { appl_sStruct3.acCharString,   NULL } } },
   /* Index: 1 */  { "B", ABP_OCTET, 10, APPL_ALL_ACCESS_DESC, 0,  { { &appl_sStruct3.abOctetString, NULL } } }
};

/*
** Structured ADI #4 (AD_StructDataType):
**-------------------------------------------------------------------------------------------------
** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
**-------------------------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_Adi4Struct[] =
{
   /* Index: 0 */  { "A", ABP_UINT8,  1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi4.bUint8,  &appl_sUint8Prop   } } },
   /* Index: 1 */  { "B", ABP_SINT8,  1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi4.bInt8,   &appl_sSint8Prop   } } },
   /* Index: 2 */  { "C", ABP_UINT16, 1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi4.iUint16, &appl_sUint16Prop  } } },
   /* Index: 3 */  { "D", ABP_SINT16, 1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi4.iInt16,  &appl_sSint16Prop  } } },
   /* Index: 4 */  { "E", ABP_UINT32, 1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi4.lUint32, &appl_sUint32Prop  } } },
   /* Index: 5 */  { "F", ABP_SINT32, 1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi4.lInt32,  &appl_sSint32Prop  } } },
   /* Index: 6 */  { "G", ABP_FLOAT,  1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi4.rFloat,  &appl_sFloat32Prop } } },
   /* Index: 7 */  { "H", ABP_OCTET,  1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi4.bOctet,  &appl_sOctetProp   } } },
   /* Index: 8 */  { "I", ABP_ENUM,   1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi4.eEnum,   &appl_sEnumProp    } } }
};

/*
** Structured ADI #5 (AD_StructDataType):
**-------------------------------------------------------------------------------------------------
** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
**-------------------------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_Adi5Struct[] =
{
   /* Index: 0 */  { "A",  ABP_UINT8,  1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi5.bUint8,  NULL } } },
   /* Index: 1 */  { "B",  ABP_SINT8,  1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi5.bInt8,   NULL } } },
   /* Index: 2 */  { "",   ABP_UINT16, 1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi5.iUint16, NULL } } },
   /* Index: 3 */  { "D",  ABP_SINT16, 1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi5.iInt16,  NULL } } },
   /* Index: 4 */  { NULL, ABP_UINT32, 1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi5.lUint32, NULL } } },
   /* Index: 5 */  { NULL, ABP_SINT32, 1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi5.lInt32,  NULL } } },
   /* Index: 6 */  { "G",  ABP_FLOAT,  1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi5.rFloat,  NULL } } },
   /* Index: 7 */  { "H",  ABP_OCTET,  1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi5.bOctet,  NULL } } },
   /* Index: 8 */  { "I",  ABP_ENUM,   1, APPL_ALL_ACCESS_DESC, 0, { { &appl_sAdi5.eEnum,   NULL } } }
};

/*
** Structured ADI #6 (AD_StructDataType):
**-------------------------------------------------------------------------------------------------
** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
**-------------------------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_Adi6Struct[] =
{
   /* Index: 0 */  { NULL, ABP_UINT8, 1, ABP_APPD_DESCR_GET_ACCESS,      0, { { &appl_sAdi6[ 0 ], NULL } } },
   /* Index: 1 */  { NULL, ABP_UINT8, 1, ABP_APPD_DESCR_SET_ACCESS,      0, { { &appl_sAdi6[ 0 ], NULL } } },
   /* Index: 2 */  { NULL, ABP_UINT8, 1, APPL_NOT_MAP_WRITE_ACCESS_DESC, 0, { { &appl_sAdi6[ 1 ], NULL } } }
};

/*
** ABP_OCTET_String (AD_StructDataType):
**-------------------------------------------------------------------------------------------------
** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
**-------------------------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_sOctetString[] = { { NULL, ABP_OCTET, 10, APPL_ALL_ACCESS_DESC, 0, { { appl_abOctetString, NULL } } } };

/*
** ABP_CHAR_String (AD_StructDataType):
**-------------------------------------------------------------------------------------------------
** | 1. pacElementName | 2. bDataType | 3. iNumSubElem | 4. bDesc | 5. bBitOffset | 6. pxValuePtr | 7. pxValuePropPtr |
**-------------------------------------------------------------------------------------------------
*/
static const AD_StructDataType appl_sCharString[] = { { NULL, ABP_CHAR, 6, APPL_ALL_ACCESS_DESC, 0, { { appl_acCharString, NULL } } } };

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*
** The ADI entry list (AD_AdiEntryType):
** ----------------------------------------------------------------------------------------------------------------------------
** | 1. iInstance | 2. pabName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 6.pxValuePtr | 7. pxValuePropPtr | 8. psStruct |
** ----------------------------------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType APPL_asAdiEntryList[] =
{
   /* Variables all data types */
   { 0x1,  "ABP_BOOL",       ABP_BOOL,    1,  APPL_ALL_ACCESS_DESC, { { &appl_fBool8,      &appl_sBool8Prop } },      NULL, NULL, NULL },
   { 0x2,  "ABP_SINT8",      ABP_SINT8,   1,  APPL_ALL_ACCESS_DESC, { { &appl_bInt8,       &appl_sSint8Prop } },      NULL, NULL, NULL },
   { 0x3,  "ABP_SINT16",     ABP_SINT16,  1,  APPL_ALL_ACCESS_DESC, { { &appl_iInt16,      &appl_sSint16Prop } },     NULL, NULL, NULL },
   { 0x4,  "ABP_SINT32",     ABP_SINT32,  1,  APPL_ALL_ACCESS_DESC, { { &appl_lInt32,      &appl_sSint32Prop } },     NULL, NULL, NULL },
   { 0x5,  "ABP_UINT8",      ABP_UINT8,   1,  APPL_ALL_ACCESS_DESC, { { &appl_bUint8,      &appl_sUint8Prop } },      NULL, NULL, NULL },
   { 0x6,  "ABP_UINT16",     ABP_UINT16,  1,  APPL_ALL_ACCESS_DESC, { { &appl_iUint16,     &appl_sUint16Prop } },     NULL, NULL, NULL },
   { 0x7,  "ABP_UINT32",     ABP_UINT32,  1,  APPL_ALL_ACCESS_DESC, { { &appl_lUint32,     &appl_sUint32Prop } },     NULL, NULL, NULL },
   { 0x8,  "ABP_CHAR",       ABP_CHAR,    1,  APPL_ALL_ACCESS_DESC, { { &appl_cChar,       NULL } },                  NULL, NULL, NULL },
   { 0x9,  "ABP_ENUM",       ABP_ENUM,    1,  APPL_ALL_ACCESS_DESC, { { &appl_bEnum,       &appl_sEnumProp } },       NULL, NULL, NULL },
   { 0xA,  "ABP_BITS8",      ABP_BITS8,   1,  APPL_ALL_ACCESS_DESC, { { &appl_bBits8,      NULL } },                  NULL, NULL, NULL },
   { 0xB,  "ABP_BITS16",     ABP_BITS16,  1,  APPL_ALL_ACCESS_DESC, { { &appl_iBits16,     NULL } },                  NULL, NULL, NULL },
   { 0xC,  "ABP_BITS32",     ABP_BITS32,  1,  APPL_ALL_ACCESS_DESC, { { &appl_lBits32,     NULL } },                  NULL, NULL, NULL },
   { 0xD,  "ABP_OCTET",      ABP_OCTET,   1,  APPL_ALL_ACCESS_DESC, { { &appl_bOctet,      &appl_sOctetProp } },      NULL, NULL, NULL },
     /* UINT64 and SINT64 not included */
   { 0x10, "ABP_FLOAT",      ABP_FLOAT,   1,  APPL_ALL_ACCESS_DESC, { { &appl_rFloat,      &appl_sFloat32Prop } },    NULL, NULL, NULL },
   { 0x11, "ABP_PAD0",       ABP_PAD0,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x12, "ABP_PAD1",       ABP_PAD1,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x13, "ABP_PAD2",       ABP_PAD2,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x14, "ABP_PAD3",       ABP_PAD3,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x15, "ABP_PAD4",       ABP_PAD4,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x16, "ABP_PAD5",       ABP_PAD5,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x17, "ABP_PAD6",       ABP_PAD6,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x18, "ABP_PAD7",       ABP_PAD7,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x19, "ABP_PAD8",       ABP_PAD8,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x1A, "ABP_PAD9",       ABP_PAD9,    1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x1B, "ABP_PAD10",      ABP_PAD10,   1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x1C, "ABP_PAD11",      ABP_PAD11,   1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x1D, "ABP_PAD12",      ABP_PAD12,   1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x1E, "ABP_PAD13",      ABP_PAD13,   1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x1F, "ABP_PAD14",      ABP_PAD14,   1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x20, "ABP_PAD15",      ABP_PAD15,   1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x21, "ABP_PAD16",      ABP_PAD16,   1,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  NULL, NULL, NULL },
   { 0x22, "ABP_BOOL1",      ABP_BOOL1,   1,  APPL_ALL_ACCESS_DESC, { { &appl_fBool1,      NULL } },                  NULL, NULL, NULL },
   { 0x23, "ABP_BIT1",       ABP_BIT1,    1,  APPL_ALL_ACCESS_DESC, { { &appl_bBit1,       NULL } },                  NULL, NULL, NULL },
   { 0x24, "ABP_BIT2",       ABP_BIT2,    1,  APPL_ALL_ACCESS_DESC, { { &appl_bBit2,       NULL } },                  NULL, NULL, NULL },
   { 0x25, "ABP_BIT3",       ABP_BIT3,    1,  APPL_ALL_ACCESS_DESC, { { &appl_bBit3,       NULL } },                  NULL, NULL, NULL },
   { 0x26, "ABP_BIT4",       ABP_BIT4,    1,  APPL_ALL_ACCESS_DESC, { { &appl_bBit4,       NULL } },                  NULL, NULL, NULL },
   { 0x27, "ABP_BIT5",       ABP_BIT5,    1,  APPL_ALL_ACCESS_DESC, { { &appl_bBit5,       NULL } },                  NULL, NULL, NULL },
   { 0x28, "ABP_BIT6",       ABP_BIT6,    1,  APPL_ALL_ACCESS_DESC, { { &appl_bBit6,       NULL } },                  NULL, NULL, NULL },
   { 0x29, "ABP_BIT7",       ABP_BIT7,    1,  APPL_ALL_ACCESS_DESC, { { &appl_bBit7,       NULL } },                  NULL, NULL, NULL },
   /* Arrays all data types */
   { 0x50, "ABP_BOOL_ARRAY",       ABP_BOOL,    8,  APPL_ALL_ACCESS_DESC, { { appl_afBool8Array,  &appl_sBool8Prop } },      NULL, NULL, NULL },
   { 0x51, "ABP_SINT8_ARRAY",      ABP_SINT8,   8,  APPL_ALL_ACCESS_DESC, { { appl_abInt8Array,   &appl_sSint8Prop } },      NULL, NULL, NULL },
   { 0x52, "ABP_SINT16_ARRAY",     ABP_SINT16,  8,  APPL_ALL_ACCESS_DESC, { { appl_aiInt16Array,  &appl_sSint16Prop } },     NULL, NULL, NULL },
   { 0x53, "ABP_SINT32_ARRAY",     ABP_SINT32,  8,  APPL_ALL_ACCESS_DESC, { { appl_alInt32Array,  &appl_sSint32Prop } },     NULL, NULL, NULL },
   { 0x54, "ABP_UINT8_ARRAY",      ABP_UINT8,   8,  APPL_ALL_ACCESS_DESC, { { appl_abUint8Array,  &appl_sUint8Prop } },      NULL, NULL, NULL },
   { 0x55, "ABP_UINT16_ARRAY",     ABP_UINT16,  8,  APPL_ALL_ACCESS_DESC, { { appl_aiUint16Array, &appl_sUint16Prop } },     NULL, NULL, NULL },
   { 0x56, "ABP_UINT32_ARRAY",     ABP_UINT32,  8,  APPL_ALL_ACCESS_DESC, { { appl_alUint32Array, &appl_sUint32Prop } },     NULL, NULL, NULL },
   { 0x57, "ABP_CHAR_ARRAY",       ABP_CHAR,    8,  APPL_ALL_ACCESS_DESC, { { appl_acCharArray,   NULL } },                  NULL, NULL, NULL },
   { 0x58, "ABP_ENUM_ARRAY",       ABP_ENUM,    8,  APPL_ALL_ACCESS_DESC, { { appl_abEnumArray,   &appl_sEnumProp } },       NULL, NULL, NULL },
   { 0x59, "ABP_BITS8_ARRAY",      ABP_BITS8,   8,  APPL_ALL_ACCESS_DESC, { { appl_abBits8Array,  NULL } },                  NULL, NULL, NULL },
   { 0x5A, "ABP_BITS16_ARRAY",     ABP_BITS16,  8,  APPL_ALL_ACCESS_DESC, { { appl_aiBits16Array, NULL } },                  NULL, NULL, NULL },
   { 0x5B, "ABP_BITS32_ARRAY",     ABP_BITS32,  8,  APPL_ALL_ACCESS_DESC, { { appl_alBits32Array, NULL } },                  NULL, NULL, NULL },
   { 0x5C, "ABP_OCTET_ARRAY",      ABP_OCTET,   8,  APPL_ALL_ACCESS_DESC, { { appl_abOctetArray,  &appl_sOctetProp } },      NULL, NULL, NULL },
     /* UINT64 and SINT64 not included */
   { 0x5F, "ABP_FLOAT_ARRAY",      ABP_FLOAT,   8,  APPL_ALL_ACCESS_DESC, { { &appl_arFloatArray, &appl_sFloat32Prop } },    NULL, NULL, NULL },
   { 0x71, "ABP_BOOL1_ARRAY",      ABP_BOOL1,   8,  APPL_ALL_ACCESS_DESC, { { &appl_afBool1Array, NULL } },                  NULL, NULL, NULL },
   { 0x72, "ABP_BIT1_ARRAY",       ABP_BIT1,    8,  APPL_ALL_ACCESS_DESC, { { &appl_abBit1Array,  NULL } },                  NULL, NULL, NULL },
   { 0x73, "ABP_BIT2_ARRAY",       ABP_BIT2,    8,  APPL_ALL_ACCESS_DESC, { { &appl_aiBit2Array,  NULL } },                  NULL, NULL, NULL },
   { 0x74, "ABP_BIT3_ARRAY",       ABP_BIT3,    8,  APPL_ALL_ACCESS_DESC, { { &appl_alBit3Array,  NULL } },                  NULL, NULL, NULL },
   { 0x75, "ABP_BIT4_ARRAY",       ABP_BIT4,    8,  APPL_ALL_ACCESS_DESC, { { &appl_alBit4Array,  NULL } },                  NULL, NULL, NULL },
   { 0x76, "ABP_BIT5_ARRAY",       ABP_BIT5,    8,  APPL_ALL_ACCESS_DESC, { { appl_aiBit5Array,   NULL } },                  NULL, NULL, NULL },
   { 0x77, "ABP_BIT6_ARRAY",       ABP_BIT6,    8,  APPL_ALL_ACCESS_DESC, { { appl_aiBit6Array,   NULL } },                  NULL, NULL, NULL },
   { 0x78, "ABP_BIT7_ARRAY",       ABP_BIT7,    8,  APPL_ALL_ACCESS_DESC, { { appl_aiBit7Array,   NULL } },                  NULL, NULL, NULL },
   /* Variables with subelements */
   { 0xA0, "ABP_OCTET_String",     DONT_CARE,   1,  APPL_ALL_ACCESS_DESC, { { NULL,  NULL } },          appl_sOctetString, NULL, NULL },
   { 0xA1, "ABP_CHAR_String",      DONT_CARE,   1,  APPL_ALL_ACCESS_DESC, { { NULL,  NULL } },          appl_sCharString,  NULL, NULL },
   /* Structures */
   { 0xF0, "Struct1",        DONT_CARE,   17, APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  appl_MappableStruct1, NULL, NULL },
   { 0xF1, "Struct2",        DONT_CARE,   17, APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  appl_MappableStruct2, NULL, NULL },
   { 0xF2, "Struct3_no_map", DONT_CARE,   2,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  appl_NonMappableStruct3, NULL, NULL },
   { 0xF3, "Struct4",        DONT_CARE,   9,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  appl_Adi4Struct, NULL, NULL },
   { 0xF4, "Struct5",        DONT_CARE,   9,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  appl_Adi5Struct, NULL, NULL },
   { 0xF5, "Struct6",        DONT_CARE,   3,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  appl_Adi6Struct, NULL, NULL },
#if ABCC_CFG_CHECK_STACK_USAGE
   { 0x100, "Stack usage",   ABP_UINT32,  1,  APPL_ALL_ACCESS_DESC, { { &appl_lStackUsage, NULL} }, NULL, GetStackUsage , NULL },
#endif
   { 0x200, "DataNotifCtrl",        DONT_CARE,   4,  APPL_ALL_ACCESS_DESC, { { NULL,              NULL } },                  appl_DataNotifDataStruct, NULL, AdiChanged }
};


/*
** Default map (AD_MapType):
**-------------------------------------------------------------
** | 1. Adi number | 2. Direction | 3. NumElem  | 4. StartIndex |
**-------------------------------------------------------------
*/

#define APPL_DEFAULT_READ_MAP    { 0xF0, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { 0xF1, PD_READ,  1,                0  }, \
                                 { 0xF1, PD_READ,  1,                1  }, \
                                 { 0xF1, PD_READ,  1,                2  }, \
                                 { 0xF1, PD_READ,  1,                3  }, \
                                 { 0xF1, PD_READ,  1,                4  }, \
                                 { 0xF1, PD_READ,  1,                5  }, \
                                 { 0xF1, PD_READ,  1,                6  }, \
                                 { 0xF1, PD_READ,  1,                7  }, \
                                 { 0xF1, PD_READ,  1,                8  }, \
                                 { 0xF1, PD_READ,  1,                9  }, \
                                 { 0xF1, PD_READ,  1,                10 }, \
                                 { 0xF1, PD_READ,  1,                11 }, \
                                 { 0xF1, PD_READ,  1,                12 }, \
                                 { 0xF1, PD_READ,  1,                13 }, \
                                 { 0xF1, PD_READ,  1,                14 }, \
                                 { 0xF1, PD_READ,  1,                15 }, \
                                 { 0xF1, PD_READ,  1,                16 }, \
                                 { 0x07, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { 0x04, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { 0x06, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { 0x03, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { 0x0B, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { 0x05, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { 0x02, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { 0x19, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { 0x29, PD_READ,  AD_MAP_ALL_ELEM,  0  }, \
                                 { AD_MAP_PAD_ADI, PD_READ, 1,       0  }, \
                                 { 0x73, PD_READ,  4,                0  }, \
                                 { 0x10, PD_READ,  AD_MAP_ALL_ELEM,  0  }

#define APPL_DEFAULT_WRITE_MAP      { 0xF0, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { 0xF1, PD_WRITE, 1,                8  }, \
                                    { 0xF1, PD_WRITE, 1,                7  }, \
                                    { 0xF1, PD_WRITE, 1,                6  }, \
                                    { 0xF1, PD_WRITE, 1,                5  }, \
                                    { 0xF1, PD_WRITE, 1,                4  }, \
                                    { 0xF1, PD_WRITE, 1,                3  }, \
                                    { 0xF1, PD_WRITE, 1,                2  }, \
                                    { 0xF1, PD_WRITE, 1,                1  }, \
                                    { 0xF1, PD_WRITE, 1,                0  }, \
                                    { 0xF1, PD_WRITE, 1,                9  }, \
                                    { 0xF1, PD_WRITE, 1,                11 }, \
                                    { 0xF1, PD_WRITE, 1,                13 }, \
                                    { 0xF1, PD_WRITE, 1,                15 }, \
                                    { 0xF1, PD_WRITE, 1,                10 }, \
                                    { 0xF1, PD_WRITE, 1,                12 }, \
                                    { 0xF1, PD_WRITE, 1,                14 }, \
                                    { 0xF1, PD_WRITE, 1,                16 }, \
                                    { 0x29, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { AD_MAP_PAD_ADI, PD_WRITE, 1,      0  }, \
                                    { 0x19, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { 0x02, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { 0x05, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { 0x0B, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { 0x03, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { 0x06, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { 0x04, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { 0x07, PD_WRITE, AD_MAP_ALL_ELEM,  0  }, \
                                    { 0x10, PD_WRITE,  AD_MAP_ALL_ELEM, 0  }, \
                                    { 0x73, PD_WRITE,  4,               0  }

const AD_MapType APPL_asAdObjDefaultMap[] =
{
   APPL_DEFAULT_READ_MAP,
   APPL_DEFAULT_WRITE_MAP,
   { AD_MAP_END_ENTRY }
};

#if ASM_OBJ_ENABLE
/*******************************************************************************
** Create map items to test different levels of full map attributes.
********************************************************************************
*/
#define APPL_ASM_ADI_MAP_UINT8        { 0x05, PD_READ, AD_MAP_ALL_ELEM, 0 }
#define APPL_ASM_ADI_MAP_UINT8_X10    APPL_ASM_ADI_MAP_UINT8, APPL_ASM_ADI_MAP_UINT8, APPL_ASM_ADI_MAP_UINT8, APPL_ASM_ADI_MAP_UINT8, APPL_ASM_ADI_MAP_UINT8, \
                                      APPL_ASM_ADI_MAP_UINT8, APPL_ASM_ADI_MAP_UINT8, APPL_ASM_ADI_MAP_UINT8, APPL_ASM_ADI_MAP_UINT8, APPL_ASM_ADI_MAP_UINT8
#define APPL_ASM_ADI_MAP_UINT8_X100   APPL_ASM_ADI_MAP_UINT8_X10, APPL_ASM_ADI_MAP_UINT8_X10, APPL_ASM_ADI_MAP_UINT8_X10, APPL_ASM_ADI_MAP_UINT8_X10, APPL_ASM_ADI_MAP_UINT8_X10, \
                                      APPL_ASM_ADI_MAP_UINT8_X10, APPL_ASM_ADI_MAP_UINT8_X10, APPL_ASM_ADI_MAP_UINT8_X10, APPL_ASM_ADI_MAP_UINT8_X10, APPL_ASM_ADI_MAP_UINT8_X10

/*------------------------------------------------------------------------------
** One full ADI map (380 map items).
**------------------------------------------------------------------------------
*/
const AD_MapType appl_asAsmMapOneFullMap[ 380 + 1 ] =
{
   APPL_ASM_ADI_MAP_UINT8_X100,
   APPL_ASM_ADI_MAP_UINT8_X100,
   APPL_ASM_ADI_MAP_UINT8_X100,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   { AD_MAP_END_ENTRY }
};

const ASM_InstanceType appl_sAsmReadOneFullMap =
{
   ABP_ASM_IA_DESC_READ | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   appl_asAsmMapOneFullMap,
};

/*------------------------------------------------------------------------------
** One full ADI map plus one item (381 map items).
**------------------------------------------------------------------------------
*/
const AD_MapType appl_asAsmMapOneFullMapPlusOne[ 381 + 1 ] =
{
   APPL_ASM_ADI_MAP_UINT8_X100,
   APPL_ASM_ADI_MAP_UINT8_X100,
   APPL_ASM_ADI_MAP_UINT8_X100,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8_X10,
   APPL_ASM_ADI_MAP_UINT8,
   { AD_MAP_END_ENTRY }
};

const ASM_InstanceType appl_sAsmReadOneFullMapPlusOne =
{
   ABP_ASM_IA_DESC_READ | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   appl_asAsmMapOneFullMapPlusOne,
};

/*------------------------------------------------------------------------------
** Create an assembly mapping using the default write map.
**------------------------------------------------------------------------------
*/
const AD_MapType APPL_asAsmObjDefaultWriteMap[] =
{
   APPL_DEFAULT_WRITE_MAP,
   { AD_MAP_END_ENTRY }
};

const ASM_InstanceType appl_sAsmDefaultWriteMap =
{
   ABP_ASM_IA_DESC_WRITE | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   APPL_asAsmObjDefaultWriteMap,
};

/*------------------------------------------------------------------------------
** Create an assembly mapping using the default read map.
**------------------------------------------------------------------------------
*/
const AD_MapType APPL_asAsmObjDefaultReadMap[] =
{
   APPL_DEFAULT_READ_MAP,
   { AD_MAP_END_ENTRY }
};

const ASM_InstanceType appl_sAsmDefaultReadMap =
{
   ABP_ASM_IA_DESC_READ | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   APPL_asAsmObjDefaultReadMap,
};


/*------------------------------------------------------------------------------
** Array of all assembly mapping instances.
**------------------------------------------------------------------------------
*/
const ASM_InstanceType* appl_aasAsmInstances[] =
{
   &appl_sAsmDefaultWriteMap,       /* Instance 1 */
   &appl_sAsmDefaultReadMap,        /* Instance 2 */
   &appl_sAsmReadOneFullMap,        /* Instance 3 */
   &appl_sAsmReadOneFullMapPlusOne, /* Instance 4 */
};
#endif

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Get current maximum stack usage
**------------------------------------------------------------------------------
*/
#if ABCC_CFG_CHECK_STACK_USAGE
static void GetStackUsage( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex )
{
   (void)psAdiEntry;
   (void)bNumElements;
   (void)bStartIndex;

   appl_lStackUsage = APPL_MaxStackUsage();
}
#endif


/*******************************************************************************
** Public Services
********************************************************************************
*/
#if ASM_OBJ_ENABLE
const ASM_InstanceType** APPL_GetAsmInstances( void )
{
   return( appl_aasAsmInstances );
}

UINT16 APPL_GetNumAsmInstances( void )
{
   return( sizeof( appl_aasAsmInstances ) / sizeof( ASM_InstanceType* ) );
}
#endif

UINT16 APPL_GetNumAdi( void )
{
   return( sizeof( APPL_asAdiEntryList ) / sizeof( AD_AdiEntryType ) );
}

void APPL_CyclicalProcessing( void )
{

}

ABP_MsgErrorCodeType APP_VerifOnDataNotificationReqCfn( UINT16 iDataNotifCounter )
{
    appl_iDataNotifResources = iDataNotifCounter;
    return( ABP_ERR_NO_ERROR );
}

static void AdiChanged( const struct AD_AdiEntry* psAdiEntry, UINT8 bNumElements, UINT8 bStartIndex )
{
   static UINT16 aiValueBuffer[ 4086 >> 1 ];
   static APP_DataNotifDescType sDataNotifDesc;
   UINT16 iValueSize = 0;
   UINT8 bIndex;
   UINT16 iIndex;

   struct TranspPayloadTest
   {
      UINT16 iIdentifier;
      UINT8 bData;
      UINT16 iDataLen;
   }
   sTranspPayloadTestArray[] = { { 1, 0x01, 100 },
                                 { 2, 0x02, 244 },
                                 { 3, 0x03, 245 },
                                 { 4, 0x04, 246 },
                                 { 5, 0x05, 247 },
                                 { 65535, 0x01, 4086 } };
   (void)bNumElements;
   (void)bStartIndex;

   if( appl_iDataNotifResources == 0 )
   {
      return;
   }

   switch( appl_bDataSet )
   {
   case ABP_APP_DATASET_SINGLEADI:

      if( appl_iDataNotifResources > 0 )
      {
         psAdiEntry = AD_GetAdiInstEntry( appl_iDataNotifInst );

         AD_GetAdiValue( psAdiEntry,
                         (void*)aiValueBuffer,
                         psAdiEntry->bNumOfElements,
                         0,
                         &iValueSize,
                         TRUE );

         APP_PrepareDataNotifSingleAdi( &sDataNotifDesc,
                                        aiValueBuffer,
                                        ( iValueSize + 7 ) / 8,
                                        ABP_APP_NW_CHANNELS_MQTT_BIT,
                                        appl_iDataNotifInst,
                                        0,
                                        0,
                                        appl_lDataNotifTimestampLow,
                                        appl_lDataNotifTimestampHigh );
      }

      break;

   case ABP_APP_DATASET_TRANSPARENT:

      for( bIndex = 0;
           bIndex < ( sizeof( sTranspPayloadTestArray ) / sizeof( struct TranspPayloadTest ) );
           bIndex++ )
      {
         if( appl_iDataNotifInst == sTranspPayloadTestArray[ bIndex ].iIdentifier )
         {
            for( iIndex = 0;
                 iIndex < ( sTranspPayloadTestArray[ bIndex ].iDataLen );
                 iIndex++ )
            {
               ABCC_PORT_Copy8( aiValueBuffer, iIndex, &sTranspPayloadTestArray[ bIndex ].bData, 0 );
            }

            APP_PrepareDataNotifTransparent( &sDataNotifDesc,
                                             aiValueBuffer,
                                             sTranspPayloadTestArray[ bIndex ].iDataLen,
                                             ABP_APP_NW_CHANNELS_MQTT_BIT,
                                             appl_iDataNotifInst );

            break;
         }
      }

      break;

   default:

      break;
   }

   appl_iDataNotifResources = APP_SendDataNotification( &sDataNotifDesc, NULL );
}

/*******************************************************************************
** Tasks
********************************************************************************
*/
#endif
