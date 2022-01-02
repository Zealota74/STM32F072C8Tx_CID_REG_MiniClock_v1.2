/*
 * sw_tim.c
 *
 *  Created on: 28.02.2018
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "gpio.h"

#include "sw_tim.h"


/************************ TIM7 configuration ****************************/
void sw_tim7_init( void ) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;	// Włączamy zegar licznika TIM7

	TIM7->PSC	= 1800 - 1;				// 72 MHz / 1800 = 40 KHz
	TIM7->ARR	= 40 - 1;				// 40 KHz / 40 = 1000 Hz
	TIM7->DIER	= TIM_DIER_UIE;			// Update Event Interrupt

	NVIC_EnableIRQ( TIM7_IRQn );
}
/************************************************************************/


/************************ Interrupt priority*******************************/
void nvic_priority(void) {

	NVIC_SetPendingIRQ( TIM14_IRQn );		// IR priority
	NVIC_SetPriority  ( TIM14_IRQn, 0 );	//
	NVIC_SetPendingIRQ( EXTI4_15_IRQn );		// Display refresh IRQ priority
	NVIC_SetPriority  ( EXTI4_15_IRQn, 1 );
//	NVIC_SetPendingIRQ( TIM3_IRQn );		// Display refresh IRQ priority
//	NVIC_SetPriority  ( TIM3_IRQn, 1 );
	NVIC_SetPendingIRQ( SysTick_IRQn );		// Systick IRQ priority
	NVIC_SetPriority  ( SysTick_IRQn, 3 );	//

}
/************************************************************************/

