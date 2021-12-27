#ifndef LIBS_KEYBOARD_V4_H_
#define LIBS_KEYBOARD_V4_H_



/**************** Definiujemy parametry biblioteki ******************************************/
#define MATRIX				0
#define KEYB_ENHANCED_MODE	1			// Włącza rozszerzony tryb obsługi klawiatury
										// z obsugą
#define COUNTER				1
#define TIMER				2

#define DEBOUNCE_MODE		TIMER
#define REPEAT_MODE

#define BTN1_PORT			PORTA
#define BTN1_PIN			PA10
#define learn_key1_pressed()	 !( BTN1_PORT->IDR & BTN1_PIN )

#define BTN2_PORT			PORTA						// Uncomment if second key activated
#if defined BTN2_PORT
	#define BTN2_PIN  		PA11
	#define learn_key2_pressed() !( BTN2_PORT->IDR & BTN2_PIN )
#endif
#define BTN3_PORT			PORTA						// Uncomment if third key activated
#if defined BTN3_PORT
	#define BTN3_PIN  		PA12
	#define learn_key3_pressed() !( BTN3_PORT->IDR & BTN3_PIN )
#endif

// Definiujemy stany w jakich znajduje sie naciśnięty przycisk
#define KEY_RELEASED		0
#define KEY_PRESSED			1
#define KEY_UNSTABLE		2

// Definiujemy interwały czasowe
#define TIME1			300
#define TIME2			700
#define TIME3			10000
#define REPEAT_TIME		1000
#define DEBOUNCE_TIME	50
#define XLONG_TIME		4000

// Definiujemy typu zdarzeń, po przytrzymaniu wciśniętego przycisku
#define SHORT_KEY_PRESS		1
#define MEDIUM_KEY_PRESS	2
#define LONG_KEY_PRESS		3
//#define XLONG_KEY_PRESS		4				// Zakomentować jeśli nie potrzeba dodatkowego interwału czasowego

// Definiujemy porty i warunki dla trybu podstawowego "single keys"
#define KEY1_PRESSED		1
#define KEY2_PRESSED		2
#define KEY3_PRESSED		3

/**** Definicja struktury ******/
typedef struct {
	uint8_t keyState;
	uint8_t switchPressType;
	uint8_t keysDecoded[1];				// Obecnie możliwość wykrycia tylko 1 przycisku
} T_KEYB;
/***********************************/

T_KEYB * return_keyboard( void );
/***********************************/

void sw_keyboard_init( void );
void register_keyboard_event_callback( void (*callback)(void) );
void sw_keyboard_manage( void  );
void SW_KEYBOARD_EVENT( void );

#endif /* LIBS_KEYBOARD_V4_H_ */
