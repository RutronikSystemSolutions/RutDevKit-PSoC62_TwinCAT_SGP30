/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the RutDevKit-TwinCAT_SGP30
*              Application for ModusToolbox.
*
* Related Document: See README.md
*
*
*  Created on: 2021-06-02
*  Company: Rutronik Elektronische Bauelemente GmbH
*  Address: Jonavos g. 30, Kaunas 44262, Lithuania
*  Author: GDR
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*
* Rutronik Elektronische Bauelemente GmbH Disclaimer: The evaluation board
* including the software is for testing purposes only and,
* because it has limited functions and limited resilience, is not suitable
* for permanent use under real conditions. If the evaluation board is
* nevertheless used under real conditions, this is done at one’s responsibility;
* any liability of Rutronik is insofar excluded
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

/* ABCC Includes */
#include "abcc_td.h"
#include "abcc.h"
#include "abcc_sys_adapt.h"
#include "ad_obj.h"
#include "appl_abcc_handler.h"
#include "abcc_versions.h"
#include "appl_adi_config.h"

/* APPL_ADI_SETUP_SGP30 */
#if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_SGP30 )
#include "sensirion_common.h"
#include "sgp30.h"
#endif

/* ABCC defines */
#define APPL_TIMER_MS	60
#define USE_TIMER_INTERRUPT 0

void handle_error(void);
static cy_rslt_t gpio_init(void);

int main(void)
{
    cy_rslt_t result;
    APPL_AbccHandlerStatusType eAbccHandlerStatus = APPL_MODULE_NO_ERROR;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
    	handle_error();
    }

    result = cy_retarget_io_init( KITPROG_TX, KITPROG_RX, CY_RETARGET_IO_BAUDRATE);
    if (result != CY_RSLT_SUCCESS)
    {
        handle_error();
    }

    result = gpio_init();
    if (result != CY_RSLT_SUCCESS)
    {
        handle_error();
    }

    __enable_irq();

#if ( APPL_ACTIVE_ADI_SETUP == APPL_ADI_SETUP_SGP30 )
    while(sgp_probe() != STATUS_OK)
    {
	  /*Sensor not ready*/
	  CyDelay(500);
  	  cyhal_gpio_toggle(LED1);
    }

    /*SGP30 Initialization*/
    if(sgp_iaq_init() == STATUS_OK)
    {
    	cyhal_gpio_write(LED2, true);
    	cyhal_gpio_write(LED1, false);
    }
#endif

    /*ABCC Module Initialization*/
	if (ABCC_HwInit() != ABCC_EC_NO_ERROR)
	{
		handle_error();
	}

	while(eAbccHandlerStatus == APPL_MODULE_NO_ERROR)
	{

		eAbccHandlerStatus = APPL_HandleAbcc();

	#if(!USE_TIMER_INTERRUPT)
		ABCC_RunTimerSystem(APPL_TIMER_MS);
		CyDelay(APPL_TIMER_MS);
	#endif

		switch(eAbccHandlerStatus)
		{
		case APPL_MODULE_RESET:
		{
	    	cyhal_gpio_write(LED2, false);
	    	cyhal_gpio_write(LED1, true);
	    	CyDelay(2000);
		}
			break;
		default:
		{
			cyhal_gpio_toggle(LED1);
		}
			break;
		}
	 }
}

void handle_error(void)
{
     /* Disable all interrupts. */
    __disable_irq();

    cyhal_gpio_write(LED2, true);
    cyhal_gpio_write(LED1, true);
    while(true)
    {
    	cyhal_gpio_toggle(LED2);
    	cyhal_gpio_toggle(LED1);
    	CyDelay(100);
    }
}

static cy_rslt_t gpio_init(void)
{
	cy_rslt_t result;

    /*Initialize LEDs*/
    result = cyhal_gpio_init( LED1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    if (result != CY_RSLT_SUCCESS)
    {return result;}
    result = cyhal_gpio_init( LED2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    if (result != CY_RSLT_SUCCESS)
    {return result;}

    /*Initialize Module ID Inputs*/
    result = cyhal_gpio_init( ARDU_IO5, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
    if (result != CY_RSLT_SUCCESS)
    {return result;}
    result = cyhal_gpio_init( ARDU_IO8, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
    if (result != CY_RSLT_SUCCESS)
    {return result;}

    /*Initialize Operation Mode Outputs*/
    result = cyhal_gpio_init( ARDU_ADC1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_PULLUP, false);
    if (result != CY_RSLT_SUCCESS)
    {return result;}
    result = cyhal_gpio_init( ARDU_ADC2, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_PULLUP, false);
    if (result != CY_RSLT_SUCCESS)
    {return result;}
    result = cyhal_gpio_init( ARDU_ADC3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_PULLUP, false);
    if (result != CY_RSLT_SUCCESS)
    {return result;}
    result = cyhal_gpio_init( POT_ADC, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_PULLUP, false);
    if (result != CY_RSLT_SUCCESS)
    {return result;}

    /*Initialize Module RESET Control Pin*/
    result = cyhal_gpio_init( ARDU_IO7, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);
    if (result != CY_RSLT_SUCCESS)
    {return result;}

    /*Initialize Module Detect Pin*/
    result = cyhal_gpio_init( ARDU_IO3, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);
    if (result != CY_RSLT_SUCCESS)
    {return result;}

    return result;
}

/* [] END OF FILE */
