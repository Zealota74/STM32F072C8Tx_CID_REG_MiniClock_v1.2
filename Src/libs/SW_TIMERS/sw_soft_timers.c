/*
 * sw_soft_timers.c
 *
 *  Created on: 05.07.2017
 *      Author: simon
 */

#include "sw_mcu_conf.h"
//#include "gpio.h"

#include "../SW_BOARD/sw_led_blink_debug.h"

//#include "../SW_INPUT/KEYBOARD/keyboard.h"

#include "sw_soft_timers.h"



/**** Inicjujemy SysTick pod timery programowe *****/
void sw_softTimers_init( uint32_t timeBase_ms ) {
	SysTick_Config( SystemCoreClock/1000/timeBase_ms );					// Co jedną milisekundę
}
/***************************************************/

/******************************************************************************/
volatile uint16_t pressTimer, debounceTimer, repeatTimer;
volatile uint32_t riseTimer, fallTimer, peakTimer, fallAvgTimer, peakAvgTimer;
volatile uint16_t showMenuTimer;
volatile uint16_t softTimer1, softTimer2, softTimer5, softTimer3, whileTimer ;

volatile static uint16_t delayTimer;
volatile static uint32_t milis = 0;
void SysTick_Handler( void ) {
	milis++;
	if (pressTimer) 	pressTimer--;
	if (debounceTimer)	debounceTimer--;
	if (repeatTimer)	repeatTimer--;
	if (delayTimer)		delayTimer--;
	if (showMenuTimer) 	showMenuTimer--;
	if (softTimer1) 	softTimer1--;
	if (softTimer2) 	softTimer2--;
	if (softTimer3) 	softTimer3--;
	if (softTimer5) 	softTimer5--;
	if (whileTimer) 	whileTimer--;

	if (showMenuTimer == 0) {
		showMenuTimer = 10;
		input_event();
	}
}
/******************************************************************************/

/*********************************************************/
void delay_ms( uint16_t ms ) {
	softTimer5 = ms;
	while ( softTimer5 ) {}
}
/*********************************************************/

uint32_t millis(void) {
	return milis;
}
