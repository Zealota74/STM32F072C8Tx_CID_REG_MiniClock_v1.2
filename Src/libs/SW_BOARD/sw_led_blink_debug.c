/*
 * led_blink.c
 *
 *  Created on: 05.07.2017
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"
#include "gpio.h"

#include "../SW_TIMERS/sw_soft_timers.h"
#include "../SW_BOARD/sw_led_blink_debug.h"


volatile uint32_t ledEventFlag = 0;


#ifdef BSRR_REG
OPTIMISE("O0") void sw_led_off( void ) {
	LED1_PORT->BSRR = LED1_PIN;
}
OPTIMISE("O0") void sw_led_on( void ) {
	LED1_PORT->BRR = LED1_PIN;
}
#else
	OPTIMISE("O0") void sw_led_off( GPIO_TypeDef *ledPort, T_GPIO_PIN ledPin ) {
		ledPort->BSRRL = ledPin;
	}
	vOPTIMISE("O0") oid sw_led_on( GPIO_TypeDef *ledPort, T_GPIO_PIN ledPin ) {
		ledPort->BSRRH = ledPin;
	}
#endif
	OPTIMISE("O0") void sw_led_xor( GPIO_TypeDef *ledPort, T_GPIO_PIN ledPin ) {
	ledPort->ODR ^= ledPin;
}


OPTIMISE("O0") void sw_led_blink( void ) {
	sw_led_xor( LED1_PORT, LED1_PIN );
}

void sw_led_debug_init( void ) {

#ifdef KA_NUCLEO
	// tryb wyjściowy, push-pull, 2MHz dla pinu PB1
	gpio_pin_cfg( LED1_PORT, LED1_PIN, 		gpio_mode_output_PP_HS );

//	gpio_pin_cfg( LED2_PORT, LED2_PIN_BLUE,	gpio_mode_output_PP_HS );
	gpio_pin_cfg( LED2_PORT, LED2_PIN_RED,	gpio_mode_output_PP_HS );
//	gpio_pin_cfg( LED2_PORT, LED2_PIN_GREEN,gpio_mode_output_PP_HS );

	// Wyłączamy wszystkie diody
	sw_led_off( LED1_PORT, LED1_PIN );

	sw_led_off( LED2_PORT, LED2_PIN_BLUE );
	sw_led_off( LED2_PORT, LED2_PIN_RED );
	sw_led_off( LED2_PORT, LED2_PIN_GREEN );
#else
	gpio_pin_cfg( LED1_PORT, LED1_PIN, 	gpio_mode_output_OD_LS );
	sw_led_off();
#endif

#ifdef	DEBUG_PIN0
	gpio_pin_cfg( DEBUG_PORT0, DEBUG_PIN0, gpio_mode_output_PP_HS );
#endif
#ifdef	DEBUG_PIN1
	gpio_pin_cfg( DEBUG_PORT1, DEBUG_PIN1, gpio_mode_output_PP_HS );
#endif
#ifdef	DEBUG_PIN2
	gpio_pin_cfg( DEBUG_PORT2, DEBUG_PIN2, gpio_mode_output_PP_HS );
#endif
#ifdef	DEBUG_PIN3
	gpio_pin_cfg( DEBUG_PORT3, DEBUG_PIN3, gpio_mode_output_PP_HS );
#endif
}


// Funkcje zdarzeniowe
/************ Przykład dla funkcji zwracającej wartość ***************/
static void ( *led_event_callback )(void);
void register_led_event_callback( void (*callback)(void)) {
	led_event_callback = callback;
}

static inline void proc_led( void ) {
	if ( !softTimer5 ) {
		softTimer5 = 1000;
		ledEventFlag = 1;
	}
	else {
		ledEventFlag = 0;
	}
}

void SW_LED_EVENT( void ) {
	proc_led();
	if ( ledEventFlag ) {
		if( led_event_callback) {
			led_event_callback();
		}
		ledEventFlag = 0;
	}
}
/**********************************************************************/

//gpio_pin_HI( DEBUG_PORT0, DEBUG_PIN0 );
//gpio_pin_LO( DEBUG_PORT0, DEBUG_PIN0 );

