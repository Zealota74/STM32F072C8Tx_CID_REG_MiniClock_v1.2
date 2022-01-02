/*
 * framebuffer.c
 *
 *  Created on: 20.08.2018
 *      Author: simon
 */

#include "stdint.h"
#include "string.h"

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"

#include "sw_dma.h"
#include "gpio.h"
#include "../SW_BOARD/sw_led_blink_debug.h"
#include "colors.h"
#include "fb.h"

// Definiujemy główny bufor wyświetlacza --------------------------------------

/********************************* Double buffer technique *******************************/
static T_DISPLAY      Buffer1[ FRAMEBUFFER_ALL ];
static T_DISPLAY Buffer1_Back[ FRAMEBUFFER_ALL ];
static T_DISPLAY	  Buffer2[ FRAMEBUFFER_ALL ];
static T_DISPLAY Buffer2_Back[ FRAMEBUFFER_ALL ];


static T_FB FB[2] = {
	{ Buffer1, Buffer1_Back, Buffer1, SYNC_FLAG_CLR },
	{ Buffer2, Buffer2_Back, Buffer2, SYNC_FLAG_CLR }
};


T_DISPLAY * fb_get_active_buffer( uint8_t buffNum ) {
	return FB[buffNum].ActualBuffer;
}
T_DISPLAY * fb_get_noActive_buffer( uint8_t buffNum ) {
	if ( FB[buffNum].ActualBuffer == FB[buffNum].Buff ) {
		return (T_DISPLAY *)FB[buffNum].Back_buff;
	} else {
		return (T_DISPLAY *)FB[buffNum].Buff;
	}
}
void fb_switch_buff( uint8_t buffNum ) {
	if ( FB[buffNum].syncFlag ) {
		FB[buffNum].syncFlag = SYNC_FLAG_CLR;
	} else {
		return;
	}
	if ( FB[buffNum].ActualBuffer == FB[buffNum].Buff ) {
		FB[buffNum].ActualBuffer = (T_DISPLAY *)FB[buffNum].Back_buff;
	} else {
		FB[buffNum].ActualBuffer = (T_DISPLAY *)FB[buffNum].Buff;
	}
}
void fb_buffer_is_ready(uint8_t buffNum) {
	FB[buffNum].syncFlag = SYNC_FLAG_SET;
}


void fb_clear_screen( T_DISPLAY * buffer ) {
	memset( (uint8_t *)buffer, 0x00, ( FRAMEBUFFER_ALL ) );
}

#define REVERSE		1
void fb_set_pixel_RAM( volatile T_DISPLAY * buffer, int16_t x, int16_t y, const T_COLOR color ) {
/***************************** Base routine, set - clear pixel ***************************/
	if ( x < 0 || x > (SCREEN_WIDTH  - 1) ||
		 y < 0 || y > (SCREEN_HEIGHT - 1)	) return;

	if( REVERSE ) {
		if ( color.mono ) {
			buffer[ 31 - ( (7-y) + 8*(x/8) ) ].mono |=  (1 << (x%8) );
		} else {
			buffer[ 31 - ( (7-y) + 8*(x/8) ) ].mono &= ~(1 << (x%8) );
		}
	} else {	// REVERSE = 0
		if ( color.mono ) {
			buffer[ (7-y) + 8*(x/8) ].mono |=  (1 << (7 - x%8) );
		} else {
			buffer[ (7-y) + 8*(x/8) ].mono &= ~(1 << (7 - x%8) );
		}
	}
}
/*****************************************************************************************/


