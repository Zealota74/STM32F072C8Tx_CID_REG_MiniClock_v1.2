/*
 * sw_i2c.c
 *
 *  Created on: 02.09.2018
 *      Author: simon
 */

#include <string.h>

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"

#include "../SW_BOARD/gpio.h"

#include "sw_i2c.h"


/************************************ PRIVATE VARIABLES ******************************************/
static I2C_TypeDef * i2c 	= I2C1;
static bool i2cAutoEnd 		= 1;
static bool i2cErrorOcurred = 0;

static bool i2cTransmitted 	= 0;
static bool i2cReceived 	= 0;

/*************************************************************************************************/

//----------------------------------- PRIVATE FUNCTIONS -----------------------------------------//
// 	I2C configured in master mode to transmit
static INLINE void sw_i2c_master_transmit_mode( I2C_TypeDef *i2c, uint8_t slaveAddr, uint8_t sizeBuff ) {
	i2c->CR2 = (i2c->CR2 &											// Clear actual values
			  ~(I2C_CR2_RD_WRN | I2C_CR2_NBYTES | I2C_CR2_SADD) )
			 														// Set values
			  |(uint32_t)sizeBuff  << I2C_CR2_NBYTES_Pos			// Bytes to send
			  |(uint32_t)slaveAddr << I2C_7BIT_ADDRESS				// Set slave address
//			  |I2C_CR2_AUTOEND
			  ;
}
/*************************************************************************************************/

/*************************************************************************************************/
// 	I2C configured in master mode to receive code example
static INLINE void sw_i2c_master_receive_mode( I2C_TypeDef *i2c, uint8_t slaveAddr, uint8_t sizeBuff  ) {
	i2c->CR2 = (i2c->CR2 & 									// Clear actual values
			  ~(I2C_CR2_NBYTES | I2C_CR2_SADD) )
			 	 	 	 	 	 	 	 	 	 	 	 	// Set values
			 |I2C_CR2_RD_WRN								// Receive mode
			 |(uint32_t)sizeBuff  << I2C_CR2_NBYTES_Pos		// Bytes to send
			 |(uint32_t)slaveAddr << I2C_7BIT_ADDRESS		// Set slave address
			 |I2C_CR2_AUTOEND
// Repeated start need "CR2_START" in this place, with all the rest parameters
			 |I2C_CR2_START
			 ;
	i2cAutoEnd = 1;
}
/*************************************************************************************************/

/***********************************************************************/
static INLINE void sw_i2c_gpio_init( I2C_TypeDef * i2c ) {
//	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	if (i2c == I2C1) {
//		gpio_pin_cfg( PORTA, PA15, gpio_mode_AF4_OD_HS );	// I2C_SCL
//		gpio_pin_cfg( PORTA, PA14, gpio_mode_AF4_OD_HS );	// I2C_SDA
		gpio_pin_cfg( PORTB, PB6, gpio_mode_AF1_OD_HS );	// I2C_SCL
		gpio_pin_cfg( PORTB, PB7, gpio_mode_AF1_OD_HS );	// I2C_SDA
	}
	else
	if (i2c == I2C2) {
		gpio_pin_cfg( PORTA, PA9, gpio_mode_AF4_OD_HS );	// I2C_SCL
		gpio_pin_cfg( PORTA, PA10, gpio_mode_AF4_OD_HS );	// I2C_SDA
	}

	/* Enable the peripheral clock i2c */
	if (i2c == I2C1)
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	else
	if (i2c == I2C2)
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
}
/***********************************************************************/

/***********************************************************************/
static INLINE void sw_i2c_master_init( I2C_TypeDef *i2c ) {

#if defined (STM32F3)
#	if I2C_SPEED == I2C_SPEED_100kHz
	i2c->TIMINGR = (uint32_t) 0x10C07DB6;	// 64 MHz clock
#	elif I2C_SPEED == I2C_SPEED_400kHz
//	i2c->TIMINGR = (uint32_t) 0x00E02168;	// 64 MHz clock, master, fast mode: 400 KHz
	i2c->TIMINGR = (uint32_t) 0x00E02579;	// 72 MHz clock, master, fast mode: 400 KHz
#	endif
#elif defined (STM32F0)
	i2c->TIMINGR = (uint32_t) 0x00B01A4B;	// 48 MHz clock
#endif
	i2c->CR1 =   I2C_CR1_PE					// Peripheral enable
//				|I2C_CR1_ERRIE				// Errors interrupt enable
				;
//	NVIC_EnableIRQ( I2C1_ER_IRQn );
}
void I2C1_ER_IRQHandler( void ) {
	if (i2c->ISR & I2C_ISR_ARLO ) {
		i2c->ICR = I2C_ICR_ARLOCF;
		i2c->CR1 &= ~I2C_CR1_PE;		// I2C off
	}
	if (i2c->ISR & I2C_ISR_BERR ) {
		i2c->ICR = I2C_ICR_BERRCF;
		i2c->CR1 &= ~I2C_CR1_PE;		// I2C off
	}
	if (i2c->ISR & I2C_ISR_OVR ) {
		i2c->ICR = I2C_ICR_OVRCF;
		i2c->CR1 &= ~I2C_CR1_PE;		// I2C off
	}
	i2cErrorOcurred = 1;
}
/***********************************************************************/

/***********************************************************************/
static INLINE void sw_i2c_dma_init( I2C_TypeDef *i2c ){
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	// DMA transmit config
	DMA1_Channel6->CPAR 	= (uint32_t)( &i2c->TXDR );
	DMA1_Channel6->CCR =
					  DMA_CCR_DIR
					 |DMA_CCR_TCIE	// Transfer complete interrupt enable
					 |DMA_CCR_MINC	// Memory increment mode
					 ;
//	NVIC_EnableIRQ( DMA_Channel6_IRQn );

	// DMA receive config
	DMA1_Channel7->CPAR = (uint32_t)( &i2c->RXDR );
	DMA1_Channel7->CCR =
						 DMA_CCR_TCIE	// Transfer complete interrupt enable
						|DMA_CCR_MINC	// Memory increment mode
					;
//	NVIC_EnableIRQ( DMA1_Channel7_IRQn );
	NVIC_EnableIRQ( DMA1_Channel4_5_6_7_IRQn );

	i2c->CR1 |=
				 I2C_CR1_TXDMAEN			// DMA transmission requests enable
				|I2C_CR1_RXDMAEN			// DMA reception requests enable
				;
}
/***********************************************************************/

//--------------------------- PUBLIC FUNCTIONS ------------------------//
/***********************************************************************/
void sw_i2c_init( void ) {
	sw_i2c_gpio_init( i2c );
	sw_i2c_master_init( i2c );
	sw_i2c_dma_init( i2c );
}
/***********************************************************************/

/***************** I2C TX buffer transmitted ***************************/
void DMA1_Channel6_IRQHandler( void ) {
	if ( DMA1->ISR & DMA_ISR_TCIF6 ) {
		DMA1->IFCR = DMA_IFCR_CTCIF6; // Channel 4 Transfer Complete clear

		DMA1_Channel6->CCR &= ~DMA_CCR_EN;
		i2cTransmitted = 1;
	}
}
/***********************************************************************/

/******************** I2C RX buffer received ***************************/
void DMA1_Channel7_IRQHandler( void ) {
	if ( DMA1->ISR & DMA_ISR_TCIF7 ) {
		DMA1->IFCR = DMA_IFCR_CTCIF7; // Channel 4 Transfer Complete clear

		DMA1_Channel7->CCR &= ~DMA_CCR_EN;

		i2cReceived = 1;
	}
}
/************************************************************************/

/*********************************************************************************/
void sw_i2c_dma_send_buff ( uint8_t slaveAddr, uint8_t *buffPtr, uint16_t sizeBuff ) {
	if (i2cErrorOcurred) return;

	sw_i2c_master_transmit_mode( i2c, slaveAddr, sizeBuff );
	if (i2cAutoEnd == 1)
		i2c->CR2 |= I2C_CR2_AUTOEND;						// Auto STOP at the end of frame
	else
		i2c->CR2 &= ~I2C_CR2_AUTOEND;						// No STOP at the end of frame


	DMA1_Channel6->CMAR	 = (uint32_t)( (uint8_t *)buffPtr );
	DMA1_Channel6->CNDTR = sizeBuff;

//	TX transfer start
	i2c->CR2 |= ( sizeBuff << I2C_CR2_NBYTES_Pos);

	DMA1_Channel6->CCR  |= DMA_CCR_EN;
	i2c->CR2 			|= I2C_CR2_START;
}
/*********************************************************************************/

/*********************************************************************************/
void sw_i2c_dma_get_buff ( uint8_t slaveAddr, uint8_t *buffPtr, uint16_t sizeBuff, uint8_t wordLength ) {
	if (i2cErrorOcurred) return;

// Send address to slave -------------------
	i2cAutoEnd = 0;
	sw_i2c_dma_send_buff( slaveAddr, buffPtr, wordLength );
	i2cAutoEnd = 1;
// -----------------------------------------
	uint32_t delay = 10000;
	while( (i2c->ISR & I2C_ISR_TC) == 0 ) {
		if (delay-- == 0 ) 	{
			i2cErrorOcurred = 1;
			return;
		}
	}

	DMA1_Channel7->CNDTR = (uint32_t)sizeBuff;
	DMA1_Channel7->CMAR	 = (uint32_t)( (uint8_t *)buffPtr + wordLength );
	DMA1_Channel7->CCR  |= DMA_CCR_EN;

	sw_i2c_master_receive_mode( i2c, slaveAddr, sizeBuff );
}
/*********************************************************************************/

/***********************************************************************/
bool sw_i2c_test_slave( uint8_t slaveAddr, uint8_t testData ) {

	uint8_t test = 0x00;
	sw_i2c_dma_send_buff( slaveAddr, (uint8_t *)&test, 1 );

	uint32_t delay = 10000;								// For 72MHz only
	while( (i2c->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY) {	// Transmit data register empty
			if (delay-- == 0 ) 	{
				i2c->CR1 &= ~I2C_CR1_PE;				// I2C disable
				i2cErrorOcurred = 1;
				return I2C_ERROR;
			}
		}
	if ( (i2c->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF)
		return I2C_NACK;

// Test wypadł pomyślnie
	i2c->ICR = 0xFFFF;
	i2cErrorOcurred = 0;

	return I2C_OK;
}

/***********************************************************************/

/***********************************************************************
static void (*i2cTX_event_callback )( uint8_t * );
static void (*i2cRX_event_callback )( void );
static void (*i2cOVF_event_callback)( void );
******************* Kod dotyczący funkcji zwrotnych *******************
void sk_register_I2cTX_event_callback( void (*callback)( uint8_t * )) {
	i2cTX_event_callback = callback;
}
void sk_register_I2cRX_event_callback( void (*callback)( void )) {
	i2cRX_event_callback = callback;
}
void sk_register_I2cOVF_event_callback( void (*callback)( void )) {
	i2cOVF_event_callback = callback;
}
void SW_I2C_EVENTS(void) {
	if ( i2cReceived && i2cTransmitted==0 ) {
		if ( i2cRX_event_callback )
			i2cRX_event_callback();

//		for (uint8_t i =0 ;i<7; i++ ) {
//			ShowDateTime.bytes[i] = 0;//sw_bcd2dec( i2cRxBuff.data[i] );
//		}

		i2cReceived = 0;
	}
	if ( i2cTransmitted && i2cReceived==0 ) {
//		if ( i2cTX_event_callback )
//			i2cTX_event_callback();
		i2cTransmitted = 0;
	}
}

void sk_register_I2cTX_event_callback( void (*callback)( uint8_t * ));
void sk_register_I2cRX_event_callback( void (*callback)( void ));
void sk_register_I2cOVF_event_callback( void (*callback)( void ));

void SW_I2C_EVENTS(void);

***********************************************************************/







