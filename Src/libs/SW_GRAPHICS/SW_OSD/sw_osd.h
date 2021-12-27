/*
 * sw_osd.h
 *
 *  Created on: 01.11.2018
 *      Author: simon
 */

#ifndef LIBS_SW_GRAPHICS_SW_OSD_H_
#define LIBS_SW_GRAPHICS_SW_OSD_H_



#define DISPLAY_TIME		(uint8_t)(1 << 0)
#define DISPLAY_DATE		(uint8_t)(1 << 1)
#define DISPLAY_BRIGHT		(uint8_t)(1 << 2)
#define DISPLAY_SET_TIME	(uint8_t)(1 << 3)
#define DISPLAY_IR_LEARN	(uint8_t)(1 << 4)

// Soft OSD triggers
#define ENTER_SET_TIME		(uint8_t)11
#define EXIT_SET_TIME		(uint8_t)12
#define ENTER_IR_LEARN		(uint8_t)13
#define EXIT_IR_LEARN		(uint8_t)14


#define PLUS				(int8_t)1
#define MINUS				(int8_t)-1

#define WITHOUT_SEC			1
#define WITH_SEC			2

#define SCREEN_TO_SMALL		0


extern uint8_t osdDisplayContant;

void osd_manage_screens( void );
void osd_bright_display_minus( void );
void osd_bright_display_plus( void );

void osd_init( void );
void osd_keyboard_manage( void );
void osd_ir_manage( void );

//void osd_rtc_updateTime( T_DISPLAY * buffer, T_COLOR color );

void osd_rtc_fonts_param( FONT_INFO * timeFont, FONT_INFO * dateFont, uint8_t displaySeconds );
void osd_bright_text_param( void );

void osd_display_time( void );
void osd_display_date( void );

void osd_display_ir_learn( void );


void register_osd_event_callback( void (*callback)( void ) );
void SW_OSD_EVENT( bool externalEvent );

#endif /* LIBS_SW_GRAPHICS_SW_OSD_H_ */
