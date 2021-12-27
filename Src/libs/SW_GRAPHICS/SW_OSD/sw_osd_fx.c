/*
 * animations.c
 *
 *  Created on: 12.11.2018
 *      Author: simon
 */

#include "sw_mcu_conf.h"

#include "../../SW_BOARD/gpio.h"
#include "../../SW_BOARD/sw_led_blink_debug.h"

#include "../fb.h"
#include "../text.h"
#include "../../SW_RTC/sw_rtc_manage.h"
#include "sw_osd_fx.h"


#define ENG

enum {
	sun, mon, tue, wen,
	thu, fri, sut
};
enum {
	jan, feb, mar, apr, may, jun,
	jul, aug, sep, oct, nov, dec
};

/********************** Days and months tables ****************/
#ifdef ENG
const CHAR * const days[] = {
	"Sunday", "Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday"
};
const CHAR * const months[] = {
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
};
#else
const CHAR * const days[] = {
	"Niedziela", "Poniedziałek", "Wtorek", "Środa",
	"Czwartek", "Piątek", "Sobota"
};
const CHAR * const months[] = {
	"Styczeń", "Luty", "Marzec", "Kwiecień", "Maj", "Czerwiec", "Lipiec",
	"Sierpień", "Wrzesień", "Październik", "Listopad", "Grudzień"
};

#endif
/*************************************************************************/

/*************************************************************************/
static CHAR * *sw_rtc_get_tables( uint8_t table ) {
	if (table == 0) return 	(CHAR * *)&months[0];
	else return 			(CHAR * *)&days[0];
}
/*************************************************************************/

/*************************************************************************/
static CHAR	Date[20];

//Format: "20 październik 2018"
CHAR * sw_date_to_string( void ) {
	Date[0] = '\0';

	char Buffer[13];
	// Add day to string
	itoa( ShowDateTime.day, Buffer, 10 );
	strcat( Buffer, " " );

#ifdef UNICODE
	wchar_t Buffer_w[13];
	text_char_to_wide( Buffer_w, Buffer );

	// Add month to string
	wcscat( Date, Buffer_w );

	// I'm testing pointer to pointer :)
	wchar_t * * ptr = (wchar_t * *)sw_rtc_get_tables( RETURN_MONTHS ) + ShowDateTime.month - 1;
	wcscat( Date, (wchar_t *) *ptr );
	wcscat( Date, L" " );
#else
	strcat( Date, Buffer );
	CHAR * * ptr = (CHAR * *)sw_rtc_get_tables( RETURN_MONTHS ) + ShowDateTime.month - 1;
	strcat( Date, (CHAR *) *ptr );
	strcat( Date, " " );
#endif
	itoa( 2000 + ShowDateTime.year, Buffer, 10 );

#ifdef UNICODE
	text_char_to_wide( Buffer_w, Buffer );
	wcscat( Date, Buffer_w );						// Add day to string
#else
	strcat( Date, Buffer );
#endif
	return Date;
}
/*************************************************************************/

/********************************************************************************************/
static T_STRING Text2;

void scroll_text_param( FONT_INFO * font ) {
	graphic_set_string( &Text2, 0, 0, font );
}

void scroll_text_display( uint8_t Length ) {
	static int16_t k = 0;
	uint8_t textLength;

	Text2.str = sw_date_to_string();
	Text2.x   = k++;

	T_DISPLAY * NoActiveBuffer = fb_get_noActive_buffer(buffer2);
	fb_clear_screen( NoActiveBuffer );
	textLength = graphic_puts_RAM( NoActiveBuffer, &Text2, FONTx1, WhiteColor, BlackColor, &GammaRGB );
	fb_buffer_is_ready(buffer2);

	if ( k == SCREEN_WIDTH) {
		k = -textLength;
	}
}

