/*
 * sw_irmanage.c
 *
 *  Created on: 01.03.2018
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"


#include "IRMP/irmp.h"
#include "sw_irmanage.h"
#include "memory_data.h"

#include "../../SW_BOARD/sw_led_blink_debug.h"
#include "../../SW_TIMERS/sw_soft_timers.h"


static uint16_t  irDecoded 			= NO_IR_CODE;
static uint16_t  globalIrAddress 	= IR_SIRC_ADDR;

static IRMP_DATA irmpData;



uint16_t sw_ir_return_code( void ) {
	return irDecoded;
}



/************** Timer4 init *****************************/
static INLINE void timer14_init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

	TIM14->PSC = ( (SystemCoreClock / F_INTERRUPTS) / 8 ) - 1;
	TIM14->ARR = 7;

	TIM14->DIER	= TIM_DIER_UIE;		// Update Event Interrupt

	TIM14->CR1 = TIM_CR1_CEN;		// Counter start
	NVIC_EnableIRQ( TIM14_IRQn );
}
void TIM14_IRQHandler(void) {	// Timer2 Interrupt Handler
	if ( TIM14->SR & TIM_SR_UIF ) {
		TIM14->SR &= ~TIM_SR_UIF;		// Update interrupt Flag
		(void) irmp_ISR();				// call IRMP ISR
	}
}
/********************************************************/

/********************************************************/
void sw_ir_init(void) {
	irmp_init();					// initialize irmp
	timer14_init();					// initialize timer2
}
/********************************************************/

/**************************** IR procedure *****************************/
static INLINE uint16_t sw_get_ir_data( uint8_t repeat_on_off ) {
	static uint16_t irRepeatCounter = 0;
	uint8_t repeatFlag;

	if ( irmp_get_data(&irmpData) && repeatTimer == 0 ) {
		if ( irmpData.address != globalIrAddress )
			return 0;

		repeatFlag = irmpData.flags & IRMP_FLAG_REPETITION;
		if ( repeat_on_off == AUTOREPEAT_OFF && ( repeatFlag ) ) {
			return NO_IR_CODE;
		}

		if ( repeatFlag ) irRepeatCounter++;
		else			  irRepeatCounter = 0;

		if ( irRepeatCounter > IR_OMIT || irRepeatCounter == 0 )
			repeatTimer = 100;
		else
			return NO_IR_CODE;

		return irmpData.command;
	}
	return NO_IR_CODE;
}
/**********************************************************************/

/******** Funkcje callbackowe i zdarzeniowe *************/
static void ( *irDecoded_event_callback )( void );
void register_irDecoded_event_callback(
		void (*callback)( void ) ) {
	irDecoded_event_callback = callback;
}
/********************************************************/


/**********************************************************************/
void SW_IR_DECODED_EVENT( void ) {
	irDecoded = sw_get_ir_data( AUTOREPEAT_ON );
	if ( irDecoded != NO_IR_CODE ) {
		if ( irDecoded_event_callback ) {
			irDecoded_event_callback();
		}
	}
}
/**********************************************************************/



/****************** Funkcja szukaj??ca powtarzaj??ce si?? kody **************************************************
static uint8_t sw_check_duplicates( uint16_t irCode, uint16_t * tablePtr, uint8_t tabLength) {
	while ( (--tabLength) ) {
		if ( *(tablePtr+tabLength) == irCode ) {
			return 1;
		}
	}
	return 0;
}
**************************************************************************************************************/
/****************************** Funkcja rejestruj??ca kody pilota **********************************************
static void sw_send_usb_massage( uint8_t a1, const char * string ) {
}

#define LED_OK		1
#define LED_ERROR	3
void sw_ir_learn( uint8_t usbCheck ) {
	uint16_t irAddressRAMtmp	= 0;
	uint16_t menuDownKey		= 0xFFFF;
	uint16_t menuExitKey		= 0xFFFF;
	uint16_t irCode;
//	uint8_t ledState			= 0;
//	sw_led_on();
	sw_send_usb_massage( ENTER_1, initString );							// Wy??wietlamy "Wlaczono tryb uczenia"
	sw_send_usb_massage( ENTER_2, inviteString );						// Wy??wietlamy "Naci??nij dowolny klawisz by zacz???? uczenie"


	irAddressRAMtmp = irmpData.address;									// Zapami??tujemy adres pilota, po odebranu pierwszego kodu
	for (uint8_t i=0; i<TABLENGTH; i++) {

		char *ptr = (char *)buttonLabels + i;							// Pobieramy z FLASH kolejny wska??nik do nazwy kodu
		sw_send_usb_massage( ENTER_NO,  waitString );					// Wy??wietlamy "Czekam na kod: "
		sw_send_usb_massage( ENTER_1, ptr );							// Wy??wietlamy "KOD" + 2*ENTER

		while ( sw_get_ir_data( AUTOREPEAT_OFF ) == NO_IR_CODE ) {		// Czekamy na odebranie kodu z pilota
//			usbPoll();													// Utrzymujemy transmisj?? USB podczas czekania na kod
//			wdt_reset();												// Resetujemy watchdog'a	podczas czekania
			// Czekamy na naci??ni??cie klawisza
//			sw_led_blink( &softTimer2, &ledState);
		}// P??tla while zako??czona, odebrali??my kod.
//		ledState = LED_OK;
		irCode = irmpData.command;
//		delay_ms(100);													// Zw??oka zabezpieczj??ca przed pobraniem niepotrzebnego kodu

		if ( sw_check_duplicates( irCode, irManageRAM.irCodesTAB, i )
		  &&(irCode != menuExitKey)&&(i>0)&&(irCode!=menuDownKey) ) {	// Odebrali??my TEN SAM kod, nic nie robimy
			sw_send_usb_massage( ENTER_1, duplicateString );			// "Blad: powtorny kod, podaj inny" + ENTER
			i--; irCode=0;												// Zmniejszamy index tablicy o 1
			continue;													// Przechodzimy do nast. iteracji p??tli
		}
		if ( (irAddressRAMtmp==irmpData.address)&&(irCode) ){			// Sprawdzamy zgodno???? adresu pilota
			//Zapisujemy do tablicy
			if ( (irCode != menuDownKey) &&								// Sprawdzamy czy nie powinni??my pomin??c klawisza
				 (irCode != menuExitKey) ) {							// Sprawdzamy czy nie powinni??my pomin??c klawisza
				*(irManageRAM.irCodesTAB + i) = irCode;
				sw_send_usb_massage( ENTER_NO, gotIrString );			// Wy??wietlamy "Zarejestrowano kod: "
				sw_send_usb_massage( ENTER_2, ptr );					// Wy??wietlamy "KOD" + 2*ENTER
			} else {
				*(irManageRAM.irCodesTAB + i) = NO_IR_CODE;				// Zerujemy pomini??ty kod
				sw_send_usb_massage( ENTER_NO, leaveString );			// Wy??wietlamy "Pomini??to kod:
				sw_send_usb_massage( ENTER_2, ptr );					// Wy??wietlamy "KOD" + 2*ENTER
			}
			if ( (irCode == menuExitKey) ) {							// Sprawdzamy czy nie powinni??my zako??czy?? uczenia
				for (uint8_t j=i; j<TABLENGTH; j++) {
					*(irManageRAM.irCodesTAB + j) = NO_IR_CODE;			// "Czy??cimy pozosta??e w tablicy kody
				}
				break;
			}
		}
		else {
			sw_send_usb_massage( ENTER_2, adresString );		// Wy??wietlamy "Adres" + 2*ENTER
			i--;														// Skaczemy do poprzedniego indeksu, powtarzamy uczenie
		}
		irCode = 0;
		if (i == 2) {													// Sprawdzamy
			menuDownKey = irmpData.command;								// Zapami??tujemy kod s??u????cy do pomijania klawiszy
		}
		if (i == 6) {													// Sprawdzamy
			menuExitKey = irmpData.command;								// Zapami??tujemy kod s??u????cy do pomijania klawiszy
		}
	} //end of FOR loop

	irManageRAM.irAddress = irAddressRAMtmp;							// Zapisujemy nowy adres pilota

//	sw_copy_ram_to_eeprom( &irManageRAM, &irManageEEM, EEMEM_BLOCK );	// Gromadzimy aktualne dane w pami??ci EEPROM
//	eeprom_write_byte ( &eememChk, EEMEM_CHK );							// Zapisujemy do EEPROM znacznik poprawno??ci zapisu
//	sw_send_usb_massage( ENTER_1, endString,	 usbCheck );			// Wy??wietlamy "Zako??czono tryb uczenia"

//	sw_led_off();
}
**************************************************************************************************************/


