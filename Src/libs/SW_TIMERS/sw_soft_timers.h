/*
 * sw_soft_timers.h
 *
 *  Created on: 05.07.2017
 *      Author: simon
 */

#ifndef SW_SOFT_TIMERS_H_
#define SW_SOFT_TIMERS_H_


/****************************** Wyliczenia dla timera programowego **********************************/
#define time_base_s(x)	(x*1000)	// Podstawiamy za 'x' ilość sekund, wynikiem będzie wartość soft timera
#define time_base_ms(x)	(x)			// Podstawiamy za 'x' ilość milisekund, wynikiem będzie wartość soft timera
#define time_base_us(x)	(x)			// Podstawiamy za 'x' ilość mikrosekund, wynikiem będzie wartość soft timera
/****************************************************************************************************/



void sw_softTimers_init( uint32_t timeBase_ms );

extern volatile uint16_t softTimer1, softTimer2, softTimer5, adcTimer;
extern volatile uint16_t pressTimer, debounceTimer, repeatTimer, adcCaptureTimer;
extern volatile uint32_t riseTimer, fallTimer, peakTimer, fallAvgTimer, peakAvgTimer;
extern volatile uint16_t showMenuTimer, softTimer3, whileTimer;

extern void delay_ms( uint16_t ms );

extern uint32_t millis(void);

#endif /* SW_SOFT_TIMERS_H_ */
