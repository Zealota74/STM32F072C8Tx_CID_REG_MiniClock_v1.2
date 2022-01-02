/*
 * sw_led_blink.h
 *
 *  Created on: 05.07.2017
 *      Author: simon
 */

#ifndef SW_LED_BLINK_H_
#define SW_LED_BLINK_H_

#include "sw_STM32F072C8Tx_mini_clock.h"


#ifdef NUCLEO_STM32F303RE
#define DEBUG_PORT3		PORTA
#define DEBUG_PIN3		PA12
#define DEBUG_PORT2		D9_PORT
#define DEBUG_PIN2		D9
#define DEBUG_PORT1		D8_PORT
#define DEBUG_PIN1		D8
#endif

#ifdef KA_NUCLEO
#define DEBUG_PORT0		D14_PORT
#define DEBUG_PIN0		D14
#define DEBUG_PORT1		D15_PORT
#define DEBUG_PIN1		D15
#define DEBUG_PORT2		PORTA
#define DEBUG_PIN2		PA15
#endif

#ifdef STM32F303CBTx_mini
#define DEBUG_PORT0		PORTC
#define DEBUG_PIN0		PC13
#define DEBUG_PORT1		PORTC
#define DEBUG_PIN1		PC14
#define DEBUG_PORT2		PORTA
#define DEBUG_PIN2		PA15
#endif

#ifdef STM32F072C8Tx_MINICLOCK
#define DEBUG_PORT0		JP1_PORT
#define DEBUG_PIN0		JP1_PIN
#define DEBUG_PORT1		JP2_PORT
#define DEBUG_PIN1		JP2_PIN

#define DEBUG_PORT2		BACK2_PORT
#define DEBUG_PIN2		BACK2_PIN
#define DEBUG_PORT3		BACK3_PORT
//#define DEBUG_PIN3		BACK3_PIN
#endif


extern volatile uint32_t ledEventFlag;

extern void sw_led_debug_init( void );
extern void sw_led_on(  void );
extern void sw_led_off( void );
extern void sw_led_blink( void );

extern void register_led_event_callback( void (*callback)(void));
extern void SW_LED_EVENT( void );

#endif /* SW_LED_BLINK_H_ */
