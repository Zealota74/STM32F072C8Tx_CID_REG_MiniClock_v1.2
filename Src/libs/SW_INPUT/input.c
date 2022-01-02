/*
 * input.c
 *
 *  Created on: 1 sty 2022
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "gpio.h"
#include "../SW_TIMERS/sw_soft_timers.h"

#include "KEYBOARD/keyboard.h"
#include "IR/sw_irmanage.h"

#include "input.h"

void input_init(void) {
	sw_keyboard_init();
	sw_ir_init();
}

INLINE void input_event(void) {
	SW_KEYBOARD_EVENT();
	SW_IR_DECODED_EVENT();
}

/**********************************************************************/
void input_keyboard_proc( void ) {
	KEY_EVENT_t presType  = keyboard_ptr()->keyEvent;
	uint8_t 	keyNumber = keyboard_ptr()->keyCoded;

	if ( presType == SHORT_PRESS ) {
		switch ( keyNumber ) {
			case KEY1:
//				osd_bright_display_plus();
				break;
			case KEY2:
//				osd_bright_display_minus();
				break;
			case KEY3:
//				osd_bright_display_minus();
				break;
			default:
				break;
		}
//		osdDisplayContant = DISPLAY_BRIGHT;
		softTimer5 = 0;						// Break main loop
	}
	if ( presType == VLONG_PRESS ) {
		switch ( keyNumber ) {
			case KEY2:
//				osd_rtc_display_setTime();
				break;
			default:
				break;
		}
//		osdDisplayContant = DISPLAY_SET_TIME;
		softTimer5 = 0;						// Break main loop
	}
}
/**********************************************************************/

/**********************************************************************/
void osd_ir_manage( void ) {
//	uint16_t irCode = sw_ir_return_code();

//	switch ( osdDisplayContant ) {
//
//		case DISPLAY_IR_LEARN:
//			osdDisplayContant = DISPLAY_IR_LEARN;
////			osd_display_ir_learn();
//			break;
//
//		default:
//			osdDisplayContant = DISPLAY_BRIGHT;
//			if ( irCode == IR_KEY_UP ) {
////				osd_bright_display_plus();
//			}
//			else
//			if ( irCode == IR_KEY_DOWN ) {
////				osd_bright_display_minus();
//			}
//			softTimer5 = 2000;
//			break;
//	}
}
/**********************************************************************/
