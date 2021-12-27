#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"

#include "libs/SW_BOARD/gpio.h"
#include "libs/SW_BOARD/sw_led_blink_debug.h"

#include "libs/SW_INPUT/sw_keyboard.h"

#include "libs/SW_GRAPHICS/SW_HT1632C/sw_ht3216c.h"
#include "libs/SW_GRAPHICS/text.h"
#include "libs/SW_GRAPHICS/SW_OSD/sw_osd_fx.h"

#include "libs/SW_TIMERS/sw_soft_timers.h"
#include "libs/SW_RTC/sw_rtc_manage.h"
#include "libs/SW_INPUT/SW_IR/sw_irmanage.h"

#include "libs/SW_STM_PERIPHS/sw_spi.v2.h"
#include "libs/SW_STM_PERIPHS/sw_tim.h"

#include "libs/SW_GRAPHICS/SW_OSD/sw_osd.h"

void SystemClock_Config(void);

/*******************************************************************************/
static uint8_t refresh = 50;
void sw_tim2_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = (uint32_t)SystemCoreClock/1000 - 1;	// 24MHz/24000 = 1000 Hz
	TIM2->ARR = (uint32_t)1000/refresh - 1;			// 1000 Hz / 5 = 200 Hz -> 5 ms
	TIM2->DIER |= 									// TIM DMA/interrupt enable register
				  TIM_DIER_UIE						// Update interrupt enable
				;
	TIM2->CR1 |= TIM_CR1_CEN
				;
	NVIC_EnableIRQ( TIM2_IRQn );
}
/*******************************************************************************/

int main(void) {
	SystemClock_Config();

	nvic_priority();

/********************************** Hardware layer *****************************************/
	RCC_gpio_init();
	sw_led_debug_init();

	sw_keyboard_init();
//	sw_ir_init();

	HT1632C_init();
	clock_init( FALSE );

/*******************************************************************************************/
//	gpio_pin_HI( DEBUG_PORT0, DEBUG_PIN0);
//	gpio_pin_LO( DEBUG_PORT0, DEBUG_PIN0);
/********************************** Hardware abstraction layer ****************************/
	osd_init();
	scroll_text_param( (FONT_INFO *)&Arial6pt8FontInfo_var );

//	register_irDecoded_event_callback( osd_ir_manage );
	register_scroll_event_callback( scroll_text_display );

	register_datetime_event_callback( rtc_get_date_time );
	register_osd_event_callback( osd_manage_screens );

	SysTick_Config( (uint32_t)SystemCoreClock / 1000 );		// Systick on 1 ms
	while (1) {
		SW_OSD_EVENT( 0 );
		SW_DATETIME_EVENT();

//		SW_IR_DECODED_EVENT();

		SW_SCROLL_EVENT( (uint16_t *)&softTimer1, TRUE );

		if (softTimer2 == 0) {
//			softTimer2 = 100;
		}
	}
}
/*******************************************************************************/
void TIM2_IRQHandler( void ) {
	if (TIM2->SR & TIM_SR_UIF ) { 		// Update interrupt Flag
		TIM2->SR = ~TIM_SR_UIF;

		gpio_pin_HI( DEBUG_PORT0, DEBUG_PIN0);
		gpio_pin_LO( DEBUG_PORT0, DEBUG_PIN0);
	}
}
/*******************************************************************************/

