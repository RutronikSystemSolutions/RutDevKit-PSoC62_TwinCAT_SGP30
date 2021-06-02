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
** Example of an ADI setup with assembly mapping instances.
**
** Dynamic assemblies are not supported. Some networks have additional
** requirements when enabling the assembly mapping object, see asm_obj.c for
** more details.
**
** In abcc_drv_cfg.h make sure that the following definitions are set to:
** ABCC_CFG_STRUCT_DATA_TYPE     ( FALSE )
** ABCC_CFG_ADI_GET_SET_CALLBACK ( FALSE )
**
** In abcc_obj_cfg.h make sure that the following definitions are set to:
** ASM_OBJ_ENABLE                ( TRUE )
** ASM_IA_NAME_ENABLE            ( TRUE )
********************************************************************************
********************************************************************************
*/

#include "appl_adi_config.h"

#if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_ASM )

#if (  ABCC_CFG_STRUCT_DATA_TYPE || ABCC_CFG_ADI_GET_SET_CALLBACK )
   #error ABCC_CFG_ADI_GET_SET_CALLBACK must be set to FALSE and ABCC_CFG_STRUCT_DATA_TYPE set to FALSE in order to run this example
#endif

#include "abp_asm.h"
#include "asm_obj.h"

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

#define APPL_NOT_MAP_READ_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |            \
                                        ABP_APPD_DESCR_MAPPABLE_WRITE_PD )

#define APPL_NOT_MAP_WRITE_ACCESS_DESC ( ABP_APPD_DESCR_GET_ACCESS |           \
                                         ABP_APPD_DESCR_SET_ACCESS )

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Example ADI's and properties.
**------------------------------------------------------------------------------
*/
static UINT32 appl_lUint32 = 0x12345678;
static INT32  appl_lInt32 = 0x87654321;
static UINT16 appl_iUint16 = 0x1234;
static INT16  appl_iInt16 = 0x4321;

static AD_UINT32Type  appl_sUint32Prop  = { { 1, 0xFFFFFFF0, 0x12345678 } };
static AD_SINT32Type  appl_sSint32Prop  = { { -0x000000FF, 0x0FFFFFFF, 0x07654321 } };
static AD_UINT16Type  appl_sUint16Prop  = { { 2, 0xFFF0, 0x1234 } };
static AD_SINT16Type  appl_sSint16Prop  = { { -0x00FF, 0x0FFF, 0x0765 } };

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
   { 0x1,  "ABP_UINT32",     ABP_UINT32,  1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_lUint32,     &appl_sUint32Prop } } },
   { 0x2,  "ABP_SINT32",     ABP_SINT32,  1,  APPL_READ_MAP_WRITE_ACCESS_DESC, { { &appl_lInt32,      &appl_sSint32Prop } } },
   { 0x3,  "ABP_UINT16",     ABP_UINT16,  1,  APPL_WRITE_MAP_READ_ACCESS_DESC, { { &appl_iUint16,     &appl_sUint16Prop } } },
   { 0x4,  "ABP_SINT16",     ABP_SINT16,  1,  APPL_READ_MAP_WRITE_ACCESS_DESC, { { &appl_iInt16,      &appl_sSint16Prop } } }
};

/*
** Default map (AD_MapType):
**-------------------------------------------------------------
** | 1. AdiIndex | 2. Direction | 3. NumElem  | 4. StartIndex |
**-------------------------------------------------------------
*/
const AD_MapType APPL_asAdObjDefaultMap[] =
{
   { AD_MAP_END_ENTRY }
};

/*------------------------------------------------------------------------------
** Example write maps.
**------------------------------------------------------------------------------
*/
const AD_MapType APPL_asAsmObjWriteMap1[] =
{
   { 1, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { 3, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};

const AD_MapType APPL_asAsmObjWriteMap2[] =
{
   { 1, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};

const AD_MapType APPL_asAsmObjWriteMap3[] =
{
   { 3, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};

/*------------------------------------------------------------------------------
** Example read maps.
**------------------------------------------------------------------------------
*/
const AD_MapType APPL_asAsmObjReadMap1[] =
{
   { 2, PD_READ, AD_MAP_ALL_ELEM, 0 },
   { 4, PD_READ, AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};

const AD_MapType APPL_asAsmObjReadMap2[] =
{
   { 2, PD_READ, AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};

/*------------------------------------------------------------------------------
** Assembly mapping instances.
**------------------------------------------------------------------------------
*/
const ASM_InstanceType APPL_sAsmWriteMapInst1 =
{
   ABP_ASM_IA_DESC_WRITE | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   APPL_asAsmObjWriteMap1,
   "Write mappable assembly 1"
};

const ASM_InstanceType APPL_sAsmWriteMapInst2 =
{
   ABP_ASM_IA_DESC_WRITE | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   APPL_asAsmObjWriteMap2,
   "Write mappable assembly 2"
};

const ASM_InstanceType APPL_sAsmWriteMapInst3 =
{
   ABP_ASM_IA_DESC_WRITE | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   APPL_asAsmObjWriteMap3,
   "Write mappable assembly 3"
};

const ASM_InstanceType APPL_sAsmWriteNonMapInst =
{
   ABP_ASM_IA_DESC_WRITE | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_NON_PD_MAPPABLE,
   APPL_asAsmObjWriteMap2,
   "Write non-mappable assembly"
};

const ASM_InstanceType APPL_sAsmReadMapInst1 =
{
   ABP_ASM_IA_DESC_READ | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   APPL_asAsmObjReadMap1,
   "Read mappable assembly 1"
};

const ASM_InstanceType APPL_sAsmReadMapInst2 =
{
   ABP_ASM_IA_DESC_READ | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_PD_MAPPABLE,
   APPL_asAsmObjReadMap2,
   "Read mappable assembly 2"
};

const ASM_InstanceType APPL_sAsmReadNonMapInst =
{
   ABP_ASM_IA_DESC_READ | ABP_ASM_IA_DESC_STATIC | ABP_ASM_IA_DESC_NON_PD_MAPPABLE,
   APPL_asAsmObjReadMap2,
   "Read non-mappable assembly"
};

/*------------------------------------------------------------------------------
** Array of all assembly mapping instances.
**------------------------------------------------------------------------------
*/
const ASM_InstanceType* APPL_aasAsmInstances[] =
{
   &APPL_sAsmWriteMapInst1,   /* Instance 1 */
   &APPL_sAsmWriteMapInst2,   /* Instance 2 */
   &APPL_sAsmWriteMapInst3,   /* Instance 3 */
   &APPL_sAsmWriteNonMapInst, /* Instance 4 */
   &APPL_sAsmReadMapInst1,    /* Instance 5 */
   &APPL_sAsmReadMapInst2,    /* Instance 6 */
   &APPL_sAsmReadNonMapInst   /* Instance 7 */
};

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

const ASM_InstanceType** APPL_GetAsmInstances( void )
{
   return( APPL_aasAsmInstances );
}

UINT16 APPL_GetNumAsmInstances( void )
{
   return( sizeof( APPL_aasAsmInstances ) / sizeof( ASM_InstanceType* ) );
}

UINT16 APPL_GetNumAdi( void )
{
   return( sizeof( APPL_asAdiEntryList ) / sizeof( AD_AdiEntryType ) );
}

void APPL_CyclicalProcessing( void )
{
   /*
   ** This function is called when read and write data have been updated. It
   ** could for example be used for operations on the ADI data.
   ** Not used in this example.
   */
}

/*******************************************************************************
** Tasks
********************************************************************************
*/

#endif /* end of #if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_ASM ) */
