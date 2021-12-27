/*
 * keyboard_keys.h
 *
 *  Created on: 24 sie 2021
 *      Author: simon
 */

#ifndef SRC_LIBS_KEYBOARD_KEYS_H_
#define SRC_LIBS_KEYBOARD_KEYS_H_

#define KEYS_SIMPLE
//#define KEYS_MULTI
//#define KEYS_MATRIX


#ifdef 	KEYS_SIMPLE
#	define NUM_KEYS				3
#	define BTN1_PORT			PORTA
#	define BTN1_PIN				PA10
#	define BTN2_PORT			PORTA
#	define BTN2_PIN				PA11
#	define BTN3_PORT			PORTA
#	define BTN3_PIN				PA12
#	define learn_key1_pressed()	gpio_pin_READ( BTN1_PORT, BTN1_PIN )
#	define learn_key2_pressed()	gpio_pin_READ( BTN2_PORT, BTN2_PIN )
#	define learn_key3_pressed()	gpio_pin_READ( BTN3_PORT, BTN3_PIN )

/********************************* Obsługa kilku klawiszy  *******************************/
static inline uint8_t sw_get_keys_simple( void ) {
	if ( learn_key1_pressed() )
		return 1;									// Przycisk 1 wciśnięty
	else
	if ( learn_key2_pressed() )
		return 2;
	else
	if ( learn_key3_pressed() )
		return 3;
	return 0;
}
/*****************************************************************************************/
#endif

#ifdef KEY_MULTI
#	define NUM_KEYS	10

#endif





#ifdef KEY_MATRIX
//TO DO
#endif

#endif /* SRC_LIBS_KEYBOARD_KEYS_H_ */
