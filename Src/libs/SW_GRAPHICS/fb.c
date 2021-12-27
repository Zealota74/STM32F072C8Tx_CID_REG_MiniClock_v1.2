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

#include "../SW_STM_PERIPHS/sw_dma.h"
#include "../SW_BOARD/gpio.h"
#include "../SW_BOARD/sw_led_blink_debug.h"
#include "colors.h"
#include "fb.h"

// Definiujemy główny bufor wyświetlacza --------------------------------------

volatile uint8_t SyncFlags[] = { SYNC_FLAG_CLR, SYNC_FLAG_CLR };
/*****************************************************************************************/

/********************************* Double buffer technique *******************************/
static T_DISPLAY      Buffer1[ FRAMEBUFFER_ALL ];
static T_DISPLAY Buffer1_Back[ FRAMEBUFFER_ALL ];
static T_DISPLAY	  Buffer2[ FRAMEBUFFER_ALL ];
static T_DISPLAY Buffer2_Back[ FRAMEBUFFER_ALL ];

typedef struct {
	T_DISPLAY * buff;
	T_DISPLAY * back_buff;
	uint8_t 	syncFlag;
} T_FB;


static const T_FB FrameBuffers[2] = {
	{Buffer1, Buffer1_Back, SYNC_FLAG_CLR},
	{Buffer2, Buffer2_Back, SYNC_FLAG_CLR},
};


const T_DISPLAY * Buffer[] 		= { Buffer1, 		Buffer2 };
const T_DISPLAY * Buffer_Back[]	= { Buffer1_Back,	Buffer2_Back };
	  T_DISPLAY * ActualBuffer[]= { Buffer1,		Buffer2 };

T_DISPLAY* fb_get_active_buffer( uint8_t buffNum ) {
	return ActualBuffer[buffNum];
}
T_DISPLAY* fb_get_active_buffer1( uint8_t buffNum ) {
	return FrameBuffers[buffNum].buff;
}
T_DISPLAY * fb_get_noActive_buffer( uint8_t buffNum ) {
	if ( ActualBuffer[ buffNum ] == Buffer[ buffNum ] ) {
		return (T_DISPLAY *)Buffer_Back[ buffNum ];
	} else {
		return (T_DISPLAY *)Buffer[ buffNum ];
	}
}

void fb_switch_buff( uint8_t buffNum ) {
	if ( SyncFlags[ buffNum ] ) {
		SyncFlags[ buffNum ] = SYNC_FLAG_CLR;
	} else {
		return;
	}
	if (ActualBuffer[ buffNum ] == Buffer[ buffNum ]) {
		ActualBuffer[ buffNum ] = (T_DISPLAY *)Buffer_Back[ buffNum ];
	} else {
		ActualBuffer[ buffNum ] = (T_DISPLAY *)Buffer[ buffNum ];
	}
}

void fb_buffer_is_ready(uint8_t buffNum) {
	SyncFlags[ buffNum ] = SYNC_FLAG_SET;
}


//uint8_t fb_sync_buffer1( int8_t buff ) {
//	uint8_t syncOld = syncFlag1;
//	if ( buff != ONLY_RETURN_FLAG ) {
//		syncFlag1 = buff; //while(syncFlag) {}
//	}
//	return syncOld;
//}
//uint8_t fb_sync_buffer2( int8_t buff ) {
//	uint8_t syncOld = syncFlag2;
//	if ( buff != ONLY_RETURN_FLAG ) {
//		syncFlag2 = buff; //while(syncFlag) {}
//	}
//	return syncOld;
//}

void fb_clear_screen( T_DISPLAY * buffer ) {
	memset( (void *)buffer, 0x00, ( FRAMEBUFFER_ALL ) );
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


