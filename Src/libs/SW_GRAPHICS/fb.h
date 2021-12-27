/*
 * framebuffer.h
 *
 *  Created on: 12.08.2018
 *      Author: simon
 */

#ifndef LIBS_SW_GRAPHICS_FRAMEBUFFER_H_
#define LIBS_SW_GRAPHICS_FRAMEBUFFER_H_

#include <arm_math.h>

#define ONLY_RETURN_FLAG ((int)-1)
#define SYNC_FLAG_SET	1
#define SYNC_FLAG_CLR	0

//--------------- Display type ---------------
#define COLOR_MONO
//#define PANEL_RGB
//--------------------------------------------

#ifdef PANEL_RGB
#define FRAMEBUFFER_MAXBITDEPTH		1

// Single panel parameters
#	define PANEL_WIDTH				64
#	define PANEL_HEIGHT				32
// Number of panels, per display
#	define PANELS					2
#	define PANEL_ROWS_CNT			16
#	define PANEL_ROW_SCAN			16
#	define FRAMEBUFFER_BUFFERS		1
#	define SCREEN_HEIGHT			PANEL_HEIGHT
#	define SCREEN_WIDTH				( (uint32_t)(PANELS * PANEL_WIDTH) )
#	define FRAMEBUFFER_ALL			(uint32_t)FRAMEBUFFER_BITSPERLINE * FRAMEBUFFER_MAXBITDEPTH * PANEL_HEIGHT/2
//Number of bits to clock out each time
#	define FRAMEBUFFER_BITSPERLINE	(uint32_t)( SCREEN_WIDTH )
typedef union {
	uint8_t rows_0_31;
	struct {
		uint8_t rows_0_15: 3;
		uint8_t rows_16_31: 3;
//		uint8_t tmp: 2;
	};
}T_RGB_2ROWS;

typedef struct {
	T_RGB_2ROWS Buffer0;
	T_RGB_2ROWS Buffer1;
}T_RGB_Buffers;

typedef union {
	uint32_t color24bit;
	struct {		// bits
		uint8_t B;	// 7 - 0
		uint8_t G;	// 15 - 8
		uint8_t R;	// 16 - 24
		uint8_t tmp;
	};
} T_RGB;
typedef T_RGB			T_COLOR;
#elif defined COLOR_MONO
#	define SCREEN_WIDTH				32
#	define SCREEN_HEIGHT			8
#	define FRAMEBUFFER_BITSPERLINE	SCREEN_WIDTH
#	define FRAMEBUFFER_ALL			(uint32_t)SCREEN_WIDTH * SCREEN_HEIGHT/8
	typedef struct {
		uint8_t mono;
	} T_MONO;
	typedef T_MONO	T_DISPLAY;
	typedef T_MONO	T_COLOR;
#endif


typedef struct {
	float32_t  gammaR, gammaG, gammaB;
} T_GAMMA;

enum {
	buffer1, buffer2
};

extern T_DISPLAY * fb_get_active_buffer( uint8_t buffNum );
extern T_DISPLAY * fb_get_noActive_buffer( uint8_t buffNum );
extern void fb_switch_buff( uint8_t buffNum );
extern void fb_buffer_is_ready( uint8_t buffNum );


extern void fb_clear_screen( T_DISPLAY * buffer );
extern void fb_set_pixel_RAM( volatile T_DISPLAY * buffer, int16_t x, int16_t y, const T_COLOR color );

#endif /* LIBS_SW_GRAPHICS_FRAMEBUFFER_H_ */
