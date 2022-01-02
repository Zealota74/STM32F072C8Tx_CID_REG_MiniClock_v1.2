/*
 * sw_osd.c
 *
 *  Created on: 01.11.2018
 *      Author: simon
 */

#include "sw_hal_conf.h"
#include "sw_mcu_conf.h"

#include "../../SW_TIMERS/sw_soft_timers.h"

//------------------ Input ---------------------
#include "../../SW_INPUT/input.h"
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
void osd_manage_screens( void ) {
	volatile static uint8_t cnt = 1;
	switch ( osdDisplayContant ) {
		case DISPLAY_TIME:				// Switch to display time
			if ( cnt % ( 2*8 + 1 ) ) {
				osd_rtc_display_time();
				softTimer5 = 500;
				osdDisplayContant = DISPLAY_TIME;
			} else {
				osdDisplayContant = DISPLAY_DATE;
			}
			cnt++;
			break;

		case DISPLAY_DATE:				// Switch to display time
			osd_rtc_display_date();
			softTimer5 = 4000;
			osdDisplayContant = DISPLAY_TIME;
			break;

		case DISPLAY_BRIGHT:			// Switch to display time
			input_keyboard_proc();
			softTimer5 = 2000;
			osdDisplayContant = DISPLAY_TIME;
			break;

		case ENTER_SET_TIME:			// Switch to display time settings
			osd_rtc_display_setTime();
			break;
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







