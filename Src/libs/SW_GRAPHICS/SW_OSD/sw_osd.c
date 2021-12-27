/*
 * sw_osd.c
 *
 *  Created on: 01.11.2018
 *      Author: simon
 */

#include "sw_hal_conf.h"
#include "sw_mcu_conf.h"

//------------------ Input ---------------------
#include "../../SW_INPUT/sw_keyboard.h"
#include "../../SW_TIMERS/sw_soft_timers.h"
#include "../../SW_INPUT/SW_IR/sw_irmanage.h"
//------------------ Graphic -------------------
#include "../fb.h"
#include "../gfx.h"
#include "../text.h"
//----------------------------------------------
#include "sw_osd.h"

//----------------------------------------------
static void ( *osd_event_callback )( void );

uint8_t osdDisplayContant = DISPLAY_TIME;

/**********************************************************************/
void osd_init( void ) {
	osd_rtc_fonts_param( (FONT_INFO *)&ArialBoldDigits8FontInfo,	// Default TIME font
						 (FONT_INFO *)&MagicMatrix_3x7_FontInfo,	// Default DATE font
						 WITHOUT_SEC );
	osd_bright_text_param();
	softTimer1 = 0;
}
/**********************************************************************/

/**********************************************************************/
void osd_keyboard_manage( void ) {
	KEY_EVENT_t presType  = keyboard_ptr()->keyEvent;
	uint8_t 	keyNumber = keyboard_ptr()->keyCoded;

	if ( presType == SHORT_PRESS ) {
		switch ( keyNumber ) {
			case KEY1:
				osd_bright_display_plus();
				break;
			case KEY3:
				osd_bright_display_minus();
				break;
			default:
				break;
		}
		osdDisplayContant = DISPLAY_BRIGHT;
		softTimer5 = 0;						// Break main loop
	}
}
/**********************************************************************/

/**********************************************************************/
void osd_manage_screens( void ) {
	volatile static uint8_t cnt = 1;
	switch ( osdDisplayContant ) {
		case DISPLAY_TIME:				// Switch to display time
			if ( cnt % ( 2*8 + 1 ) ) {
				osd_display_time();
				softTimer5 = 500;
				osdDisplayContant = DISPLAY_TIME;
			} else {
				osdDisplayContant = DISPLAY_DATE;
			}
			cnt++;
			break;

		case DISPLAY_DATE:				// Switch to display time
			osd_display_date();
			softTimer5 = 4000;
			osdDisplayContant = DISPLAY_TIME;
			break;

		case DISPLAY_BRIGHT:			// Switch to display time
			osd_keyboard_manage();
			softTimer5 = 2000;
			osdDisplayContant = DISPLAY_TIME;
			break;

//		case ENTER_SET_TIME:			// Switch to display time settings
//			osdDisplayContant = DISPLAY_SET_TIME;
//			break;
//		case EXIT_SET_TIME:
//			osdDisplayContant = DISPLAY_TIME;
//			break;
		default:
			break;
	}
}
/**********************************************************************/

/****************** Funkcje callback'owe i zdarzeniowe ****************/
void register_osd_event_callback( void (*callback)( void )) {
	osd_event_callback = callback;
}
void SW_OSD_EVENT( bool externalEvent ) {
	if ( softTimer5 == 0 ) {
		if( osd_event_callback ) {
			osd_event_callback();
		}
	}
}
/***********************************************************************/






/**********************************************************************/
void osd_ir_manage( void ) {
	uint16_t irCode = sw_ir_return_code();

	switch ( osdDisplayContant ) {

		case DISPLAY_IR_LEARN:
			osdDisplayContant = DISPLAY_IR_LEARN;
//			osd_display_ir_learn();
			break;

		default:
			osdDisplayContant = DISPLAY_BRIGHT;
			if ( irCode == IR_KEY_UP ) {
				osd_bright_display_plus();
			}
			else
			if ( irCode == IR_KEY_DOWN ) {
				osd_bright_display_minus();
			}
			softTimer5 = 2000;
			break;
	}
}
/**********************************************************************/

