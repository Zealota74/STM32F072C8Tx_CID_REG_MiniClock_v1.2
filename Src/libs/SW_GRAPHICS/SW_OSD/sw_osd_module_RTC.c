/*
 * sw_osd_module_RTC.c
 *
 *  Created on: 01.11.2018
 *      Author: simon
 */

#include <string.h>

#include "sw_mcu_conf.h"

#include "../../SW_BOARD/gpio.h"
#include "../../SW_BOARD/sw_led_blink_debug.h"

#include "../../SW_GRAPHICS/SW_HT1632C/sw_ht3216c.h"
#include "../../SW_GRAPHICS/text.h"

#include "../../SW_RTC/sw_rtc_manage.h"

#include "sw_osd.h"

/********************************************************************************************/
//--------------------------------------------------
static T_STRING TextHH, TextMM, TextSS;
static T_STRING Delim1, Delim2, Delim3;
static T_STRING TextYEAR, TextMONTH, TextDAY;
static INLINE void osd_rtc_string_init(void) {
	TextHH.str = "00";
	TextMM.str = "00";
	TextSS.str = "00";

	Delim1.str = ":";
	Delim2.str = ":";
	Delim3.str = ";";
}
//--------------------------------------------------


static const T_COLOR colorClock = {1};
/********************************************************************************************/

/********************************* Draw the numbers on screen *******************************/
static void osd_rtc_puts_int_RAM( T_DISPLAY * buffer, T_STRING *string, int data,
					uint8_t textSize, T_COLOR color, T_COLOR bg, const T_GAMMA * gamma ) {
	char buffer1[16];
	char buffer2[16] = "0";

	itoa( data, buffer1, 10 );
	string->str = buffer1;

	if ( data > -1 && data < 10 ) {
		strcat( buffer2, buffer1 );
		string->str = buffer2;
	}
#ifdef UNICODE
	wchar_t buffer_wchar[16];
	(void)text_char_to_wide( buffer_wchar, str );
	string->str = buffer_wchar;
#endif
	string->stringLength = graphic_puts_RAM( buffer, string, textSize, color, bg, gamma );
}
/********************************************************************************************/

/********************************************************************************************/
static uint8_t osd_rtc_time_params( FONT_INFO * digitsFont, uint8_t align,
									uint8_t offset, uint8_t withSeconds ) {
	uint8_t W = SCREEN_WIDTH - 1, H = SCREEN_HEIGHT - 1;

	uint8_t hDigit = digitsFont->heightPixels;
	if ( hDigit > SCREEN_HEIGHT ) return 0;		// Font height larger screen height

	uint8_t wDigit = digitsFont->charInfo[ '0' - digitsFont->startChar ].widthBits;		// Width of "0" character
	uint8_t wDelim = digitsFont->charInfo[ ':' - digitsFont->startChar ].widthBits;		// Width of ";" character
	uint8_t wInter = digitsFont->interspacePixels;

	uint8_t allW = 2 * ( 2*wDigit + wInter ) + 1 *( 2*wInter + wDelim );

	if ( withSeconds == WITH_SEC ) allW = allW + ( 2*wDigit + wInter ) + ( 2*wInter + wDelim );

	if ( allW > SCREEN_WIDTH ) return SCREEN_TO_SMALL;

	uint8_t xPos = 0, yPos = 0;
	switch ((uint8_t)align % 3) {
		case 0:
			yPos = 0 + offset;
		break;
		case 1:
			yPos = (H + 1) / 2 - hDigit / 2;
		break;
		case 2:
			yPos = (H + 1) - hDigit - offset;
		break;
		default:
		break;
	}
	switch ((uint8_t)align / 3) {
		case 0:
			xPos = 0 + offset;
		break;
		case 1:
			xPos = (W + 1) / 2 - allW / 2;
		break;
		case 2:
			xPos = (W + 1) - allW - offset;
		break;
		default:
		break;
	}
	graphic_set_string( &TextHH, xPos, yPos, digitsFont );

	xPos += 2 * (wDigit + wInter);
	graphic_set_string( &Delim1, xPos, yPos, digitsFont );

	xPos += wDelim + wInter;
	graphic_set_string( &TextMM, xPos, yPos, digitsFont );

	xPos += 2 * (wDigit + wInter);
	graphic_set_string( &Delim2, xPos, yPos, digitsFont );

	xPos += wDelim + wInter;
	graphic_set_string( &TextSS, xPos, yPos, digitsFont );

	// Delimiters draw only once, during initialization
//	graphic_puts_RAM( ActualBuffer, &Delim1, FONTx1, &colorClock, (T_COLOR *)&BlackColor );
//	graphic_puts_RAM( ActualBuffer, &Delim2, FONTx1, &colorClock, (T_COLOR *)&BlackColor );
	return allW;
}
static uint8_t osd_rtc_date_params( FONT_INFO * digitsFont, uint8_t align, uint8_t offset ) {
	graphic_set_string( &TextYEAR, 	0, 0, (FONT_INFO *)&MagicMatrix_3x5_FontInfo );
	graphic_set_string( &TextMONTH, 16, 1, digitsFont );
	graphic_set_string( &TextDAY, 	25, 0, digitsFont );
	graphic_set_string( &Delim3, 	23, 1, digitsFont );

	return 0;
}
/********************************************************************************************/

/********************************************************************************************/
static void osd_rtc_display_time( T_DISPLAY * buffer, uint8_t withSeconds ) {
	static uint8_t cnt;
	T_COLOR delimColor = colorClock;

	osd_rtc_puts_int_RAM( buffer, &TextHH, ShowDateTime.hh, FONTx1,
						  colorClock, BlackColor, &GammaRGB );
	if ( withSeconds == WITHOUT_SEC ) {
		if (cnt++%2) delimColor = colorClock;
		else 		 delimColor = BlackColor;
	}
	graphic_puts_RAM( buffer, &Delim1, FONTx1, delimColor, BlackColor, &GammaRGB );
	osd_rtc_puts_int_RAM( buffer, &TextMM, ShowDateTime.mm, FONTx1,
						  colorClock, BlackColor, &GammaRGB );

	if ( withSeconds == WITH_SEC ) {
		graphic_puts_RAM( buffer, &Delim2, FONTx1, colorClock, BlackColor, &GammaRGB );
		osd_rtc_puts_int_RAM( buffer, &TextSS, ShowDateTime.ss, FONTx1,
							  colorClock, BlackColor, &GammaRGB );
	}
}
static void osd_rtc_display_date( T_DISPLAY * buffer, T_DATETIME *dateTime ) {
	osd_rtc_puts_int_RAM( buffer, &TextYEAR, dateTime->year + 2000,
						  FONTx1, colorClock, BlackColor, &GammaRGB );
	osd_rtc_puts_int_RAM( buffer, &TextMONTH, dateTime->month, FONTx1,
						  colorClock, BlackColor, &GammaRGB );
	osd_rtc_puts_int_RAM( buffer, &TextDAY, dateTime->day, FONTx1,
						  colorClock, BlackColor, &GammaRGB );
}
/********************************************************************************************/

/************************************ Public functions **************************************/
void osd_display_time(void) {
	T_DISPLAY * nonActiveBuffer = fb_get_noActive_buffer(buffer1);

	fb_clear_screen( nonActiveBuffer );
	osd_rtc_display_time( nonActiveBuffer, WITHOUT_SEC );
	fb_buffer_is_ready(buffer1);
}
void osd_display_date(void) {
	T_DISPLAY * nonActiveBuffer = fb_get_noActive_buffer(buffer1);

	fb_clear_screen( nonActiveBuffer );
	osd_rtc_display_date( nonActiveBuffer, &ShowDateTime );
	fb_buffer_is_ready(buffer1);
}
/********************************************************************************************/

/********************************************************************************************/
void osd_rtc_fonts_param( FONT_INFO * timeFont, FONT_INFO * dateFont, uint8_t displaySeconds ) {
	osd_rtc_string_init();

	uint8_t check;
	check = osd_rtc_time_params( timeFont, ALIGN_CTOP, 0, displaySeconds );

	if ( check == SCREEN_TO_SMALL )	while(1) {}
	check = osd_rtc_date_params( dateFont, ALIGN_LTOP, 0 );
}
/********************************************************************************************/


//void osd_rtc_updateTime( T_DISPLAY * buffer, T_COLOR color ) {
//	osd_rtc_puts_int_RAM( buffer, &TextHH, ShowDateTime.hh, FONTx1, color, BlackColor, &GammaRGB );
//	graphic_puts_RAM( buffer, &Delim1, FONTx1, color, BlackColor, &GammaRGB );
//	osd_rtc_puts_int_RAM( buffer, &TextMM, ShowDateTime.mm, FONTx1, color, BlackColor, &GammaRGB );
//	graphic_puts_RAM( buffer, &Delim2, FONTx1, color, BlackColor, &GammaRGB );
//	osd_rtc_puts_int_RAM( buffer, &TextSS, ShowDateTime.ss, FONTx1, color, BlackColor, &GammaRGB );
//}
///********************************************************************************************/
//
///********************************************************************************************/
//void osd_setTime_text( T_DISPLAY * buffer, uint8_t withSeconds, FONT_INFO * font ) {
//
//}
///********************************************************************************************/

