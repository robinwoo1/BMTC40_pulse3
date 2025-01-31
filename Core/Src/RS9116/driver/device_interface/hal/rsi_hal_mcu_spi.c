/*******************************************************************************
* @file  rsi_hal_mcu_spi.c
* @brief 
*******************************************************************************
* # License
* <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/
/**
 * @file       rsi_hal_mcu_spi.c
 * @version    0.1
 * @date       18 sept 2015
 *
 *
 *
 * @brief: HAL SPI API
 *
 * @Description:
 * This file Contains all the API's related to HAL 
 *
 */

#define DMA_ENABLED 1
/**
 * Includes
 */
 #include "stm32f7xx_hal.h"
#include "rsi_driver.h"
#include "variable.h"

#ifdef RSI_SPI_INTERFACE
extern SPI_HandleTypeDef hspi4;
extern uint8_t receive_completed,transmit_completed;
volatile uint8_t  dma_tx_rx_completed;
/**
 * Global Variables
 */

void cs_enable()
{
	// HAL_GPIO_WritePin(SPI4_CS_GPIO_Port, SPI4_CS_Pin, GPIO_PIN_RESET);
}

void cs_disable()
{
	// HAL_GPIO_WritePin(SPI4_CS_GPIO_Port, SPI4_CS_Pin, GPIO_PIN_SET);
}

/*==================================================================*/
/**
 * @fn         int16 rsi_spi_cs_deassert(void)
 * @param[in]  None
 * @param[out] None
 * @return     None
 * @description 
 * This API is used to deassert the SPI chip select for SPI interface.
 */


void rsi_pwrsave_host_gpio_set(void)
{
  //! Make SPI CS HIGH
	// HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);
}

/*==================================================================*/
/**
 * @fn         int16 rsi_spi_cs_assert(void)
 * @param[in]  None
 * @param[out] None
 * @return     None
 * @description 
 * This API is used to assert the SPI chip select for SPI interface.
 */
void rsi_pwrsave_host_gpio_reset(void)
{
  //! Make SPI CS LOW
	// HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET);
	
}



/*==================================================================*/
/**
 * @fn         int16_t rsi_spi_transfer(uint8_t *ptrBuf,uint16_t bufLen,uint8_t *valBuf,uint8_t mode)
 * @param[in]  uint8_t *tx_buff, pointer to the buffer with the data to be transfered
 * @param[in]  uint8_t *rx_buff, pointer to the buffer to store the data received
 * @param[in]  uint16_t transfer_length, Number of bytes to send and receive
 * @param[in]  uint8_t mode, To indicate mode 8 BIT/32 BIT mode transfers.
 * @param[out] None
 * @return     0, 0=success
 * @section description  
 * This API is used to tranfer/receive data to the Wi-Fi module through the SPI interface.
 */
int16_t rsi_spi_transfer(uint8_t *tx_buff, uint8_t *rx_buff, uint16_t transfer_length,uint8_t mode)
{
	uint32_t timeout = HAL_GetTick();

	UNUSED_PARAMETER(mode);//This statement is added only to resolve compilation warning, value is unchanged

	if(tx_buff != NULL)
	{
		memcpy(ethTxdummyBuf, tx_buff, transfer_length);
	}
	if(rx_buff != NULL)
	{
		memcpy(ethRxdummyBuf, rx_buff, transfer_length);
	}

	// if(tx_buff == NULL)
	// {
	// 	tx_buff = (uint8_t *)&ethTxdummyBuf;
	// }
	// else if(rx_buff == NULL)
	// {
	// 	rx_buff = (uint8_t *)&ethRxdummyBuf;
	// }			
	//! enable CS PIN - HARDWARE로 제어
	// cs_enable();
					
	#if DMA_ENABLED
		dma_tx_rx_completed = 0;

		HAL_SPI_TransmitReceive_DMA(&hspi4, ethTxdummyBuf, ethRxdummyBuf, transfer_length);
		// HAL_SPI_TransmitReceive_DMA(&hspi4, tx_buff, rx_buff, transfer_length);
		while(dma_tx_rx_completed == 0)
		{
			if(HAL_GetTick() - timeout > 300 && HAL_GetTick() > 100)
				break;
		}

		if(rx_buff != NULL)
			memcpy(rx_buff, ethRxdummyBuf, transfer_length);

		if(tx_buff != NULL)
			memset(ethTxdummyBuf, 0, transfer_length);

	#else
		HAL_SPI_TransmitReceive(&hspi4, tx_buff, rx_buff, transfer_length, 100);
	#endif

	//! disable CS PIN
	// cs_disable();
			
	return 0;
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	UNUSED_PARAMETER(hspi);//This statement is added only to resolve compilation warning, value is unchanged
	if (hspi->Instance == SPI4)
	{
		dma_tx_rx_completed = 1;
	}
}

#endif
