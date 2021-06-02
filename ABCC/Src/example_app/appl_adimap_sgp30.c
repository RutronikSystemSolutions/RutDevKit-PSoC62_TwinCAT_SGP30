#include "appl_adi_config.h"
#include "abcc.h"
#include "sensirion_common.h"
#include "sgp30.h"

#if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_SGP30 )

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

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Private Globals
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Data holder for the ADI instances
**------------------------------------------------------------------------------
*/
static UINT16 tvoc_ppb;
static UINT16 co2_eq_ppm;
static UINT16 scaled_ethanol_signal;
static UINT16 scaled_h2_signal;

/*------------------------------------------------------------------------------
** Min, max and default value for appl_aiUint16
**------------------------------------------------------------------------------
*/
static AD_UINT16Type appl_sUint16Prop = { { 0, 0xFFFF, 0 } };

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*-------------------------------------------------------------------------------------------------------------
** 1. iInstance | 2. pabName | 3. bDataType | 4. bNumOfElements | 5. bDesc | 6. pxValuePtr | 7. pxValuePropPtr
**--------------------------------------------------------------------------------------------------------------
*/
const AD_AdiEntryType APPL_asAdiEntryList[] =
{
   {  0x1,  "TVOC_PPB",  ABP_UINT16,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, { { &tvoc_ppb,    &appl_sUint16Prop } } },
   {  0x2,  "CO2_EQ_PPM", ABP_UINT16,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, { { &co2_eq_ppm, &appl_sUint16Prop } } },
   {  0x3,  "SCALED_ETHANOL_SIGNAL", ABP_UINT16,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, { { &scaled_ethanol_signal, &appl_sUint16Prop } } },
   {  0x4,  "SCALED_H2_SIGNAL", ABP_UINT16,   1, APPL_WRITE_MAP_READ_ACCESS_DESC, { { &scaled_h2_signal, &appl_sUint16Prop } } }
};

/*------------------------------------------------------------------------------
** Map all adi:s in both directions
**------------------------------------------------------------------------------
** 1. AD instance | 2. Direction | 3. Num elements | 4. Start index |
**------------------------------------------------------------------------------
*/
const AD_MapType APPL_asAdObjDefaultMap[] =
{
   { 1, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { 2, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { 3, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { 4, PD_WRITE, AD_MAP_ALL_ELEM, 0 },
   { AD_MAP_END_ENTRY }
};

/*******************************************************************************
** Private Services
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/
UINT16 APPL_GetNumAdi( void )
{
   return( sizeof( APPL_asAdiEntryList ) / sizeof( AD_AdiEntryType ) );
}

void APPL_CyclicalProcessing( void )
{
   if( ABCC_AnbState() == ABP_ANB_STATE_PROCESS_ACTIVE )
   {
	   printf("ABP_ANB_STATE_PROCESS_ACTIVE\r\n");

	   if(sgp_measure_signals_blocking_read(&scaled_ethanol_signal, &scaled_h2_signal) == STATUS_OK){
		   printf("tVOC Concentration: %5d [ppb]     CO2eq Concentration: %5d [ppm]\r\n", tvoc_ppb, co2_eq_ppm);
	   }
	   else
		   printf("Measurement Error\r\n");

	   if(sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm) == STATUS_OK){
		   printf("tVOC Concentration: %5d [ppb]     CO2eq Concentration: %5d [ppm]\r\n", tvoc_ppb, co2_eq_ppm);
	   }
	   else
		   printf("Measurement Error\r\n");
   }
   else
   {
	   printf("ABP_ANB_STATE_PROCESS_INACTIVE");

	   //SPS is inactive, Sensor communication inactive
	   //Reset sensor values

	   tvoc_ppb = 0;
	   co2_eq_ppm = 0;
	   scaled_ethanol_signal = 0;
	   scaled_h2_signal = 0;
   }
}

#endif //APPL_ADI_SETUP_SGP30
