
#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"

#include "../SW_BOARD/gpio.h"
#include "keyboard_v4.h"

#include "../SW_TIMERS/sw_soft_timers.h"



static uint8_t keybEventFlag = 0;
static T_KEYB keyboard;

T_KEYB * return_keyboard( void ) {
	return &keyboard;
}

/************ Przykład dla funkcji zwracającej wartość ***************/
static void ( *keyboard_event_callback )(void);
void register_keyboard_event_callback( void (*callback)(void)) {
	keyboard_event_callback = callback;
}
/**********************************************************************/

void sw_keyboard_init( void ) {
// Ustawiamy pin jako wejście z pullup
	gpio_pin_cfg( BTN1_PORT, BTN1_PIN, gpio_mode_in_PU );

#if defined BTN2_PIN
	gpio_pin_cfg( BTN2_PORT, BTN2_PIN, gpio_mode_in_PU );
#endif
#if defined BTN3_PIN
	gpio_pin_cfg( BTN3_PORT, BTN3_PIN, gpio_mode_in_PU );
#endif

	GPIOA->ODR = PA10|PA11|PA12;
}

#if MATRIX == 0
/********************************* Obsługa kilku klawiszy  *******************************/
static INLINE uint8_t sw_get_single_key( void ) {
	if ( learn_key1_pressed() )
		return KEY1_PRESSED;									// Przycisk 1 wciśnięty
#ifdef BTN2_PIN
	else
	if ( learn_key2_pressed() )
		return KEY2_PRESSED;									// Przycisk 2 wciśnięty
#endif
#ifdef BTN3_PIN
	else
	if ( learn_key3_pressed() )
		return KEY3_PRESSED;									// Przycisk 3 wciśnięty
#endif
		return 0;												// Przyciski zwolnione
}
/*****************************************************************************************/
#else	// MATRIX == 1

/*************************** Obsługa klawiatury matrycowej *******************************/
static uint8_t sw_get_matrix_key( void ) {
// Na razie pusto :(
	return 0;
}
/*****************************************************************************************/
#endif


#if KEYB_ENHANCED_MODE == 1
/*
static uint8_t get_alternate_code(uint8_t keyCode) {
  if      (keyCode == HID_KEY_ENTER)	return HID_KEY_ESC;		//'Key OK' to 'Key esc'
  else if (keyCode == HID_KEY_RIGHT)	return HID_KEY_P;		//'Menu right' to 'Play'
  else if (keyCode == HID_KEY_DOT)		return HID_KEY_F;		//'Next track' to "Fastforward"
  else if (keyCode == HID_KEY_COMMA)	return HID_KEY_R;		//'Previous track' to "Rewind"
  else if (keyCode == HID_KEY_Y)		return HID_KEY_R;		//'Previous track' to "Rewind"
  else return 0;
}
*/
/*************************************** Analizujemy czas naciśnięcia przycisku *********************************************/
static INLINE void sw_press_time_analyze( void ) {
	if ( pressTimer == 0 && keyboard.switchPressType == SHORT_KEY_PRESS ) {	// Minęło SHORT_TIME, krótki czas naciśnięcia
		keyboard.switchPressType = MEDIUM_KEY_PRESS;
		pressTimer					 = TIME2;									// Ustawiamy timer na średnio długi czas naciśnięcia
	}
	if ( pressTimer == 0 && keyboard.switchPressType == MEDIUM_KEY_PRESS )	{	// Minął kolejny interwał, średnio długi czas
		keyboard.switchPressType = LONG_KEY_PRESS;							// Zaczyna się długi okres czasu

#if defined XLONG_KEY_PRESS
		pressTimer 		 = TIME3;
	}
	if ( pressTimer == 0 && switchPressType == LONG_KEY_PRESS ) {  		// Minął kolejny interwał, długi okres czasu
		switchPressType = XLONG_KEY_PRESS;								// Rozpoczyna sie najdłuższy okres czasu
		pressTimer 		= XLONG_TIME;
#endif
	}
}
#endif
/***************************************************************************************************************************/

/********************************************** Obsługa naciśniętego przycisku **********************************************/
void sw_keyboard_manage( void  ) {
#if DEBOUNCE_MODE==COUNTER
	static uint16_t counter;
#endif
	static uint8_t keyTmp;

#if MATRIX == 0
	uint8_t keyKeyboardCode	= sw_get_single_key();						// Tryb obsługi kilku klawiszy
#else
	uint8_t keyKeyboardCode	= sw_get_matrix_key();						// Tryb obsługi wielu klawiszy, klawiatura matrycowa
#endif

#ifdef REPEAT_MODE
	if ( keyboard.switchPressType == LONG_KEY_PRESS ) {
		if ( debounceTimer == 0 && keyboard.keyState == KEY_PRESSED  ) {
			keybEventFlag 			= 1;								// Odebraliśmy dane z klawiszy
			keyboard.keysDecoded[0] = keyTmp;
			debounceTimer 			= REPEAT_TIME;						// Ustawiamy odliczanie czasu przy repetycji klawisza
		}
	}
#endif
	// ---------------------------------------------------------------------
	if ( keyKeyboardCode > 0 ) {							// Przycisk NACIŚNIETY, pierwsze "wykryte" "zbocze opadające"
		if ( keyboard.keyState == KEY_RELEASED ) {			// Jeśli był zwolniony, to analizujemy dalej
			keyTmp						= keyKeyboardCode;	// Zapamiętujemy tymczasowo kod klawisza
			keyboard.keyState			= KEY_PRESSED;		// Ustawiamy flagę: przycisk wciśnięty
			keyboard.switchPressType	= SHORT_KEY_PRESS;	// Tryb podstawowy analizy, "krótkie naciśnięcie"
			pressTimer					= TIME1;			// Odliczamy 0.4 sekundy, maksymalny czas "zwykłego nacisniecia"
		} else
		if ( keyboard.keyState == KEY_PRESSED ) {			// Nadal naciśnięty, mierzymy czas naciśnięcia
			sw_press_time_analyze();						// Sprawdzamy czas trzymania klawisza
		}
	} // keyKeyboardCode = 0
	else if ( keyboard.keyState == KEY_PRESSED  ) {
		keyboard.keyState = KEY_UNSTABLE;					// Wchodzimy w stan, gdzie mogą nastąpić drgania styków
		keyboard.keysDecoded[0] 		= keyTmp;
#if DEBOUNCE_MODE==COUNTER
		counter = 10;
#else
		pressTimer	= DEBOUNCE_TIME;						// debounceTimer
#endif
	}

#if DEBOUNCE_MODE==COUNTER
	if ( keyboard.keyState==KEY_UNSTABLE && --counter == 0 ) {
#else
	if ( keyboard.keyState == KEY_UNSTABLE && pressTimer == 0 ) {
#endif
		keyboard.keyState = KEY_RELEASED;
		keyTmp			  = 0;
		keybEventFlag	  = 1;								// Odebraliśmy dane z klawiszy
	}
}
/**************************************************************************************************************************************/



void SW_KEYBOARD_EVENT( void ) {
	if ( keybEventFlag ) {
		if( keyboard_event_callback ) {
			keyboard_event_callback();
		}
		keybEventFlag = 0;
	}
}
