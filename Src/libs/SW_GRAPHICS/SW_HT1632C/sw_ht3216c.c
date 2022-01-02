/*
 * ht3216c.c
 *
 *  Created on: 30.06.2018
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"

#include "gpio.h"
#include "sw_spi.v2.h"

#include "../../SW_BOARD/sw_led_blink_debug.h"

#include "../fb.h"
#include "sw_ht3216c.h"


static uint16_t command 			= 0;
volatile static bool scrollEvent 	= 0;
volatile static uint8_t bright 		= 0;

const uint8_t gammaBright[] = {
	0, 1, 2, 3, 6, 9, 12, 15
};

static const SPI_param_t spi2_ht3216c_Param = { MSBFIRST, CPOL_HIGH, CPHA_LEADING };

/**************************** Basic display function ***************************/
static void HT1632C_command( uint16_t data ) {
#if SPI_TRANSFER_MODE == SPI_SW_MODE
	HT1632C_send( &command, ONE_TRANSMIT, 12 );
#elif SPI_TRANSFER_MODE == SPI_HW_MODE
	sw_spi_hw_write_read( data, 12 );
#endif
}
static void HT1632C_write_buff( uint8_t chooseBuffer ) {
	HT_CS_LO;
	command = HT1632C_WRITE;
	sw_spi_hw_send_buff16( &command, ONE_TRANSMIT, 10 );

	if ( chooseBuffer == 0 ) {
		sw_spi_hw_send_buff8( (uint8_t *)fb_get_active_buffer(buffer1), FRAMEBUFFER_ALL, 8 );
		fb_switch_buff(buffer1);
	} else {
		sw_spi_hw_send_buff8( (uint8_t *)fb_get_active_buffer(buffer2), FRAMEBUFFER_ALL, 8 );
		fb_switch_buff(buffer2);
	}
	HT_CS_HI;
}
/*******************************************************************************/

/*******************************************************************************/
static uint8_t HT1632C_breath_calc( uint8_t dynaBright ) {
	if ( dynaBright == DYNA_BRIGHT_OFF ) return 5;

	static uint8_t i  = 0, bright = 0;
	static int8_t dir = 1;

	if ( i++%2 == 0 ) {
		if (dir == 1) {
			bright++;
			if ( bright == HT1632C_BRIGHT_MAX ) dir =-1;	// DOWN
		}
		else {
			bright--;
			if ( bright == HT1632C_BRIGHT_MIN ) dir = 1;	// UP
		}
	}
	return bright;
}
/*******************************************************************************/

/*******************************************************************************/
static void HT1632C_dyna_bright( uint8_t dynaBright ) {
	uint8_t brightness = 0;

	if( bright == DYNAMIC_BRIGHT ) {
		if ( dynaBright )	brightness = gammaBright[ 7 ];
		else 		 		brightness = HT1632C_breath_calc( DYNA_BRIGHT_ON );
	} else {
		if ( dynaBright )	brightness = gammaBright[ bright ];
		else		 		brightness = gammaBright[ 1 ];
	}
	HT1632C_command( HT1632C_SET_BRIGHT + ((brightness & 15) << 1) );
}
/*******************************************************************************/

/*************************** Send buffer to display ****************************/
static void HT1632C_scroll( uint8_t scrollSpeed ) {
	if ( scrollSpeed > 4 ) return;

	volatile static uint8_t i = 0;
	if ( (++i % (scrollSpeed)) == 0 ) {
		scrollEvent = 1;
	}
}
/*******************************************************************************/



#ifdef HARDWARE_SYNC
/*******************************************************************************/
void EXTI4_15_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR8) {
		EXTI->PR = EXTI_PR_PR8;		// This bit is cleared by writing a 1 to the bit.

		volatile static uint8_t cnt;
		cnt++;

		HT1632C_dyna_bright( cnt%2 );
		HT1632C_write_buff ( cnt%2 );
		HT1632C_scroll( 5 );
	}
}
/*******************************************************************************/
#else
/*******************************************************************************/
 void TIM2_IRQHandler( void ) {
	volatile static uint8_t cnt = 0;
	if (TIM2->SR & TIM_SR_UIF ) { 		// Update interrupt Flag
		TIM2->SR &= ~TIM_SR_UIF;
		if (cnt++%2){ActualBuffer = Buffer1; bright = 3;}
		else		{ActualBuffer = Buffer2; bright = 5;}
		if ( syncFlag == 1 ) {
			return;
		}
		HT1632C_send_screen( bright );
		gpio_pin_HI( DEBUG_PORT1, DEBUG_PIN1);
		gpio_pin_LO( DEBUG_PORT1, DEBUG_PIN1);
	}
}
/*******************************************************************************/
#endif

#ifdef HARDWARE_SYNC
/*******************************************************************************/
static INLINE void Configure_EXTI(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	gpio_pin_cfg( PORTA, PA8, gpio_mode_in_PU );

	SYSCFG->EXTICR[1] 	|= SYSCFG_EXTICR3_EXTI8_PA;
// Falling trigger event configuration bit of line 8
	EXTI->FTSR 			|= EXTI_FTSR_TR8;
// Rising trigger event configuration bit of line 8
//	EXTI->RTSR 			|= EXTI_RTSR_TR8;
	EXTI->IMR 			|= EXTI_IMR_MR8;			// Interrupt Mask on line 8

	NVIC_EnableIRQ( EXTI4_15_IRQn );
}
/*******************************************************************************/
#elif
/*******************************************************************************/
static uint8_t refresh = 60;
void sw_tim2_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = (uint32_t)SystemCoreClock/1000 - 1;	// 24MHz/24000 = 1000 Hz
	TIM2->ARR = (uint32_t)1000/refresh - 1;			// 1000 Hz / 5 = 200 Hz -> 5 ms
	TIM2->DIER |= 							// TIM DMA/interrupt enable register
				  TIM_DIER_UIE						// Update interrupt enable
				;
	TIM2->CR1 |= TIM_CR1_CEN
				;
	NVIC_EnableIRQ( TIM2_IRQn );
}
/*******************************************************************************/
#endif
/*******************************************************************************/
void HT1632C_init( void ) {					// setting up the display
	gpio_pin_cfg( HT_CS_PORT, 	HT_CS_PIN,	 gpio_mode_output_PP_HS);

#if SPI_TRANSFER_MODE == SPI_SW_MODE
	gpio_pin_cfg( HT_WR_PORT, 	HT_WR_PIN,	 gpio_mode_output_PP_HS);
	gpio_pin_cfg( HT_DATA_PORT, HT_DATA_PIN, gpio_mode_output_PP_HS);
#elif SPI_TRANSFER_MODE == SPI_HW_MODE
	sw_spi_hw_init( &spi2_ht3216c_Param );
#endif
	HT1632C_command( HT1632C_START_SYS );
	HT1632C_command( HT1632C_LED_ON );
	HT1632C_command( HT1632C_SET_CLOCK );
	HT1632C_command( HT1632C_SET_LEYOUT );
	HT1632C_command( HT1632C_BLINK_OFF );
	HT1632C_command( HT1632C_SET_BRIGHT + (6 << 1) );
#ifdef HARDWARE_SYNC
	Configure_EXTI();
#else
	sw_tim2_init();
#endif
}
void INLINE HT1632C_brightness( uint16_t brightness ) {
	bright = brightness;
	if ( bright < AUTO_BRIGHT ) {
		brightness = gammaBright[ bright ];
//		HT1632C_command( HT1632C_SET_BRIGHT + ((brightness & 15) << 1) );
	}
}
void HT1632C_led_on ( void ) {
	HT1632C_command( HT1632C_LED_ON );
}
void HT1632C_led_off( void ) {
	HT1632C_command( HT1632C_LED_OFF );
}
/*******************************************************************************/

/******** Funkcje callback'owe i zdarzeniowe *************/
static void ( *scroll_event_callback )( uint8_t );
void register_scroll_event_callback(
		void (*callback)( uint8_t aa ) ) {
	scroll_event_callback = callback;
}
/********************************************************/
/**********************************************************************/
void SW_SCROLL_EVENT( uint16_t * softTimer, uint8_t scrollOn ) {
	if ( scrollEvent && scrollOn ) {
		if ( scroll_event_callback ) {
			scroll_event_callback( 0 );
			scrollEvent = 0;
		}
	}
}
/**********************************************************************/

/******************* Clear display ********************
void HT1632C_fill_screen( uint8_t pattern ) {
	HT_CS_LO;

	command = HT1632C_WRITE;

#if SPI_TRANSFER_MODE == SPI_SW_MODE
	HT1632C_send( &command, ONE_TRANSMIT, 10 );
	for (uint8_t i=0; i<SCREEN_WIDTH; i++) {
		command = (uint16_t)pattern;
		HT1632C_send( &command, ONE_TRANSMIT, 8 );
	}
#elif SPI_TRANSFER_MODE == SPI_HW_MODE
#endif
	HT_CS_HI;
}
******************************************************/
/*******************************************************************************
void ht1632c_display_refresh( uint8_t refresh ) {
}

*******************************************************************************/
