/*
 * keyboard_defs.h
 *
 *  Created on: 1 sty 2022
 *      Author: simon
 */

#ifndef LIBS_SW_INPUT_KEYBOARD_KEYBOARD_DEFS_H_
#define LIBS_SW_INPUT_KEYBOARD_KEYBOARD_DEFS_H_

// ------------------ Constant definitions -------------------
#define TEST_KEYBOARD	0

#define DEBOUNCE_TIME		10
#define REPEAT_TIME			100
#define SHORT_MAX_TIME		400
#define SHORT_MIN_TIME		100
#define MEDIUM_MAX_TIME		500
#define VLONG_TIME_CNT		20		// 20 * 100 ms = 2s
#define MULTI_PRESS_TIME	500
#define EDGE_COUNTER		4		// 2 fall and 2 rise edges
#define NO_KEY				0
#define KEY1				1
#define KEY2				2
#define KEY3				3

typedef enum {
	NO_EVENT 	= 0, SHORT_PRESS  = 1, MEDIUM_PRESS = 2, LONG_PRESS = 3,
	VLONG_PRESS = 4, DOUBLE_PRESS = 5, KEY_REPEAT 	= 6
} KEY_EVENT_t;
typedef enum {
	action_idle, action_debounce, action_check, action_react, action_repeat
} KEY_ACTION_t;

enum {
	stateFALL = 1, stateRISE = 2, stateLOW = 3, stateHIGH = 4
};
enum {
	keyRELEASED = 0, keyPRESSED = 1
};
// -----------------------------------------------------------

// ------------------- Types definitions ---------------------
typedef struct {
	KEY_EVENT_t keyEvent;
	uint8_t 	keyCoded;
} KEYB_t;
typedef struct {
	GPIO_TypeDef *  port;
	T_GPIO_PIN 		pin;
	void ( *keyboard_callback )(void);
	KEYB_t keyb;
} T_KEYB_OUT;
// ----------------------------------------------------------


#endif /* LIBS_SW_INPUT_KEYBOARD_KEYBOARD_DEFS_H_ */
