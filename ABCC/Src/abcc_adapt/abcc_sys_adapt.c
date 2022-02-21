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
*/

#include "abcc_drv_cfg.h"
#include "abcc_port.h"
#include "abcc_sys_adapt.h"
#include "abcc_sys_adapt_spi.h"
#include "abcc_sys_adapt_par.h"
#include "abcc_sys_adapt_ser.h"

/*PSoC62 Header Files*/
#include "cycfg_pins.h"
#include "cyhal.h"

#if(ABCC_CFG_INT_ENABLED)
extern void(*ABCC_ISR)(void);
static void hms_irq_handler(void *handler_arg, cyhal_gpio_event_t event);
static cy_rslt_t hms_irq_init(void);
static void hms_irq_deinit(void);
#endif

/* Defines */
#define ABCC_SYS_OM0              	ARDU_ADC1
#define ABCC_SYS_OM1              	ARDU_ADC2
#define ABCC_SYS_OM2              	ARDU_ADC3
#define ABCC_SYS_OM3              	POT_ADC
#define ABCC_SYS_OM3_POR          	CYHAL_PORT_10
#define ABCC_SYS_OM3_PIN          	(5u)

#define ABCC_SYS_nRESET				ARDU_IO7
#define ABCC_SYS_MI0				ARDU_IO5
#define ABCC_SYS_MI1				ARDU_IO8
#define ABCC_SYS_MD					ARDU_IO3

#define ABCC_SYS_IRQ_PIN			ARDU_IO4
#define ABCC_IRQ_PRIORITY			(7u)

#define ABCC_SYS_SPI_HANDLE			hms_spi_obj
#define ABCC_SYS_SPI_NSS			ARDU_CS
#define ABCC_SYS_SPI_NSS_POR		CYHAL_PORT_8
#define ABCC_SYS_SPI_NSS_PIN		(3u)
#define ABCC_SYS_SPI_NSS_ON			cyhal_gpio_write(ABCC_SYS_SPI_NSS, false)
#define ABCC_SYS_SPI_NSS_OFF		cyhal_gpio_write(ABCC_SYS_SPI_NSS, true)
#define ABCC_SYS_SPI_MOSI			ARDU_MOSI
#define ABCC_SYS_SPI_MISO			ARDU_MISO
#define ABCC_SYS_SPI_CLK			ARDU_CLK
#define ABCC_SYS_SPI_FREQ			10000000UL

#define ABCC_SYS_SER_UART_HANDLE	hms_uart_obj
#define ABCC_SYS_SER_UART_RX		ARDU_RX
#define ABCC_SYS_SER_UART_TX		ARDU_TX

#if(ABCC_CFG_DRV_SPI)
/* Callback for SPI data ready indication */
static ABCC_SYS_SpiDataReceivedCbfType pnDataReadyCbf;
#endif

#if( ABCC_CFG_DRV_SERIAL )
/*
** Callback for Serial data ready indication.
*/
static ABCC_SYS_SerDataReceivedCbfType pnSerDataReadyCbf;
#endif

/*PSoC62 Global Variables*/
cyhal_uart_t hms_uart_obj;
cyhal_spi_t hms_spi_obj;
cyhal_uart_cfg_t hms_uart_config =
		{
				.data_bits = 8,
				.stop_bits = 1,
				.parity = CYHAL_UART_PARITY_NONE,
				.rx_buffer = NULL,
				.rx_buffer_size = 0,
		};

cyhal_gpio_callback_data_t hms_irq_data =
{
		.callback = hms_irq_handler,
		.callback_arg = NULL,

};

#if( ABCC_CFG_POLL_ABCC_IRQ_PIN )
_Bool irq_active = false;
#endif

#if( ABCC_CFG_DRV_SERIAL )
static void ABCC_SYS_UartInit( UINT8 bOpmode )
{
	cy_rslt_t result;

	result = cyhal_uart_init(&ABCC_SYS_SER_UART_HANDLE, ABCC_SYS_SER_UART_TX, ABCC_SYS_SER_UART_RX, NULL, &hms_uart_config);
	if(result != CY_RSLT_SUCCESS)
	{
		 CY_ASSERT(0);
	}

   /* Ensure that OM3 is configured as an Input */
   if( bOpmode == ABP_OP_MODE_SERIAL_19_2 )
   {
      result = cyhal_uart_set_baud(&ABCC_SYS_SER_UART_HANDLE, 19200, NULL);
      if(result != CY_RSLT_SUCCESS)
      {
    	  CY_ASSERT(0);
      }
   }
   else if( bOpmode == ABP_OP_MODE_SERIAL_57_6 )
   {
      result = cyhal_uart_set_baud(&ABCC_SYS_SER_UART_HANDLE, 57600, NULL);
      if(result != CY_RSLT_SUCCESS)
      {
    	  CY_ASSERT(0);
      }
   }
   else if( bOpmode == ABP_OP_MODE_SERIAL_115_2 )
   {
      result = cyhal_uart_set_baud(&ABCC_SYS_SER_UART_HANDLE, 115200, NULL);
      if(result != CY_RSLT_SUCCESS)
      {
    	  CY_ASSERT(0);
      }
   }
   else if (bOpmode == ABP_OP_MODE_SERIAL_625)
   {
      result = cyhal_uart_set_baud(&ABCC_SYS_SER_UART_HANDLE, 625000, NULL);
      if(result != CY_RSLT_SUCCESS)
      {
    	  CY_ASSERT(0);
      }
   }
}
#endif

#if(ABCC_CFG_DRV_SPI)
static void ABCC_SYS_SpiInit(void)
{
	cy_rslt_t result;

	result = cyhal_spi_init(&ABCC_SYS_SPI_HANDLE,
			ABCC_SYS_SPI_MOSI,
			ABCC_SYS_SPI_MISO,
			ABCC_SYS_SPI_CLK,
			ABCC_SYS_SPI_NSS,
			NULL,
			8,
			CYHAL_SPI_MODE_00_MSB,
			false);
    if(result != CY_RSLT_SUCCESS)
    {
  	  CY_ASSERT(0);
    }

    result = cyhal_spi_set_frequency(&ABCC_SYS_SPI_HANDLE, ABCC_SYS_SPI_FREQ);
    if(result != CY_RSLT_SUCCESS)
    {
  	  CY_ASSERT(0);
    }
}
#endif

BOOL ABCC_SYS_HwInit( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
#if(ABCC_CFG_INT_ENABLED)
	{
		ABCC_SYS_AbccInterruptDisable();
	}
#endif

	//No further actions, option board is powered without MCU
	ABCC_SYS_HWReset();
	CyDelay(5);
	return TRUE;
}


BOOL ABCC_SYS_Init( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
	return TRUE;
}


void ABCC_SYS_Close( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}


#if( ABCC_CFG_OP_MODE_SETTABLE )
void ABCC_SYS_SetOpmode( UINT8 bOpmode )
{
   cyhal_gpio_write(ABCC_SYS_OM0, false);
   cyhal_gpio_write(ABCC_SYS_OM1, false);
   cyhal_gpio_write(ABCC_SYS_OM2, false);
   cyhal_gpio_write(ABCC_SYS_OM3, false);

   /* OM0 */
   if( ( bOpmode & 0x01 ) == 0x01 )
   {
      cyhal_gpio_write(ABCC_SYS_OM0, true);
   }

   /* OM1 */
   if( ( bOpmode & 0x02 ) == 0x02 )
   {
      cyhal_gpio_write(ABCC_SYS_OM1, true);
   }

   /* OM2 */
   if( ( bOpmode & 0x04 ) == 0x04 )
   {
      cyhal_gpio_write(ABCC_SYS_OM2, true);
   }

   /* OM3 */
   if( ( bOpmode & 0x08 ) == 0x08 )
   {
      cyhal_gpio_write(ABCC_SYS_OM3, true);
   }

   /* Peripherals will be re-initialized according to operating mode */
   if( bOpmode >= ABP_OP_MODE_SERIAL_19_2 )
   {
#if( ABCC_CFG_DRV_SERIAL )
      /* Serial op mode selected. Configure serial port */
      ABCC_SYS_UartInit( bOpmode );
#endif
   }
   else
   {
	   if(ABCC_SYS_SER_UART_HANDLE.base != NULL)
	   {
		   cyhal_uart_free(&ABCC_SYS_SER_UART_HANDLE);
	   }
   }

   if( bOpmode == ABP_OP_MODE_SPI )
   {
#if( ABCC_CFG_DRV_SPI )
      ABCC_SYS_SpiInit();
#endif
   }
   else
   {
	   if(ABCC_SYS_SPI_HANDLE.base != NULL)
	   {
		   cyhal_spi_free( &ABCC_SYS_SPI_HANDLE );
	   }
   }

   if( ( ( bOpmode != ABP_OP_MODE_16_BIT_PARALLEL ) &&
         ( bOpmode != ABP_OP_MODE_8_BIT_PARALLEL ) ) )
   {

   }

   if( ( bOpmode == ABP_OP_MODE_16_BIT_PARALLEL ) ||
       ( bOpmode == ABP_OP_MODE_8_BIT_PARALLEL ) )
   {
      /* Initialize parallel controller */
#if( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 )
      ABCC_SYS_FsmcInit( bOpmode );
#endif
   }
   CyDelay(5);
}
#endif


#if( ABCC_CFG_OP_MODE_GETTABLE )
UINT8 ABCC_SYS_GetOpmode( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
	return ABP_OP_MODE_SPI;
	//return ABP_OP_MODE_SERIAL_115_2;
}
#endif


void ABCC_SYS_HWReset( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
	cyhal_gpio_write(ABCC_SYS_nRESET, false);
	CyDelay(1);
}


void ABCC_SYS_HWReleaseReset( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
	cyhal_gpio_write(ABCC_SYS_nRESET, true);
	CyDelay(1);
}


#ifndef ABCC_CFG_ABCC_MODULE_ID
UINT8 ABCC_SYS_ReadModuleId( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
	UINT8 bRet = 0x00;
	if(cyhal_gpio_read(ABCC_SYS_MI0) == true)
	{
		bRet |= 0x01;
	}

	if(cyhal_gpio_read(ABCC_SYS_MI1) == true)
	{
		bRet |= 0x02;
	}

	return bRet;
}
#endif


#if( ABCC_CFG_MOD_DETECT_PINS_CONN )
BOOL ABCC_SYS_ModuleDetect( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
	if(cyhal_gpio_read(ABCC_SYS_MD) == false)
	{
		return (TRUE);
	}
	else
	{
		return (FALSE);
	}
}
#endif

#if( ABCC_CFG_SYNC_ENABLE && ABCC_CFG_USE_ABCC_SYNC_SIGNAL )
void ABCC_SYS_SyncInterruptEnable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif

#if( ABCC_CFG_SYNC_ENABLE && ABCC_CFG_USE_ABCC_SYNC_SIGNAL )
void ABCC_SYS_SyncInterruptDisable( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
}
#endif


#if( ABCC_CFG_INT_ENABLED )
void ABCC_SYS_AbccInterruptEnable( void )
{
	cy_rslt_t result;

	result = hms_irq_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
}


void ABCC_SYS_AbccInterruptDisable( void )
{
	hms_irq_deinit();
}
#endif


#if( ABCC_CFG_POLL_ABCC_IRQ_PIN )
BOOL ABCC_SYS_IsAbccInterruptActive( void )
{
   /*
   ** Implement according to abcc_sys_adapt.h
   */
	if(irq_active)
	{
		irq_active = false;
		return (TRUE);
	}
	else
	return (FALSE);
}
#endif


#if( ABCC_CFG_DRV_SPI )
void ABCC_SYS_SpiRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   /*
   ** Implement according to abcc_sys_adapt_spi.h
   */
	pnDataReadyCbf = pnDataReceived;
}


void ABCC_SYS_SpiSendReceive( void* pxSendDataBuffer, void* pxReceiveDataBuffer, UINT16 iLength)
{
   /*
   ** Implement according to abcc_sys_adapt_spi.h
   */
	cy_rslt_t result;

	result = cyhal_spi_transfer(&ABCC_SYS_SPI_HANDLE, pxSendDataBuffer, iLength, pxReceiveDataBuffer, iLength, 0xFF);

    if(result != CY_RSLT_SUCCESS)
    {
  	  CY_ASSERT(0);
    }
	pnDataReadyCbf();
}
#endif


#if( ( ABCC_CFG_DRV_PARALLEL || ABCC_CFG_DRV_PARALLEL_30 ) && !ABCC_CFG_MEMORY_MAPPED_ACCESS )
void ABCC_SYS_ParallelRead( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}


#if( ABCC_CFG_DRV_PARALLEL_30 )
UINT8 ABCC_SYS_ParallelRead8( UINT16 iMemOffset )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


#if( ABCC_CFG_DRV_PARALLEL )
UINT16 ABCC_SYS_ParallelRead16( UINT16 iMemOffset )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


void ABCC_SYS_ParallelWrite( UINT16 iMemOffset, void* pxData, UINT16 iLength )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}


#if( ABCC_CFG_DRV_PARALLEL_30 )
void ABCC_SYS_ParallelWrite8( UINT16 iMemOffset, UINT8 pbData )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


#if( ABCC_CFG_DRV_PARALLEL )
void ABCC_SYS_ParallelWrite16( UINT16 iMemOffset, UINT16 piData )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif

void* ABCC_SYS_ParallelGetRdPdBuffer( void )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}


void* ABCC_SYS_ParallelGetWrPdBuffer( void )
{
   /*
   ** Implement according to abcc_sys_adapt_par.h
   */
}
#endif


#if( ABCC_CFG_DRV_SERIAL )
void ABCC_SYS_SerRegDataReceived( ABCC_SYS_SpiDataReceivedCbfType pnDataReceived  )
{
   /*
   ** Implement according to abcc_sys_adapt_ser.h
   */
	pnSerDataReadyCbf = pnDataReceived;
}


void ABCC_SYS_SerSendReceive( void* pxTxDataBuffer, void* pxRxDataBuffer, UINT16 iTxSize, UINT16 iRxSize )
{
   /* Perform a blocking transmission */
   if( cyhal_uart_write( &ABCC_SYS_SER_UART_HANDLE, pxTxDataBuffer, (size_t*)&iTxSize ) != CY_RSLT_SUCCESS )
   {
      /* Error */
      return;
   }

   /* Perform a blocking reception */
   if( cyhal_uart_read( &ABCC_SYS_SER_UART_HANDLE, pxRxDataBuffer, (size_t*)&iRxSize ) != CY_RSLT_SUCCESS )
   {
      /* Error */
      return;
   }

   if( pnSerDataReadyCbf )
   {
      pnSerDataReadyCbf();
   }
}


void ABCC_SYS_SerRestart( void )
{
   /*
   ** Implement according to abcc_sys_adapt_ser.h
   */
}
#endif

#if(ABCC_CFG_INT_ENABLED)
static void hms_irq_handler(void *handler_arg, cyhal_gpio_event_t event)
{

#if( ABCC_CFG_POLL_ABCC_IRQ_PIN )
	irq_active = true;
#endif

	ABCC_ISR();
}

/* Initializes the interrupt pin and registers a callback */
static cy_rslt_t hms_irq_init(void)
{
	cy_rslt_t result;

    /* Initialize the IRQ pin */
    result = cyhal_gpio_init(ABCC_SYS_IRQ_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);

    /* IRQ initialization failed. */
    if (result != CY_RSLT_SUCCESS)
    {
    	return result;
    }

    /* Configure GPIO interrupt */
    cyhal_gpio_register_callback(ABCC_SYS_IRQ_PIN, &hms_irq_data);
    cyhal_gpio_enable_event(ABCC_SYS_IRQ_PIN, CYHAL_GPIO_IRQ_FALL, ABCC_IRQ_PRIORITY, true);

	return result;
}

/* Initializes the interrupt pin and registers a callback */
static void hms_irq_deinit(void)
{
	/* De-initialize the IRQ pin */
	cyhal_gpio_free(ABCC_SYS_IRQ_PIN);

    /* Configure GPIO interrupt */
    cyhal_gpio_register_callback(ABCC_SYS_IRQ_PIN, &hms_irq_data);
    cyhal_gpio_enable_event(ABCC_SYS_IRQ_PIN, CYHAL_GPIO_IRQ_FALL, ABCC_IRQ_PRIORITY, false);
}
#endif
