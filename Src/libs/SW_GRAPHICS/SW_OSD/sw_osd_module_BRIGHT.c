/*
 * sw_osd_module_KODI.c
 *
 *  Created on: 01.11.2018
 *      Author: simon
 */

#include "sw_mcu_conf.h"

#include "../../SW_GRAPHICS/SW_HT1632C/sw_ht3216c.h"

#include "../../SW_GRAPHICS/text.h"

#include "sw_osd.h"




static T_STRING Bright1, Bright2;

static const CHAR bright1[] 	= "Bright: ";
static const CHAR brightAU[] 	= "Au";
static const CHAR brightDYN[] 	= "Dy";


/********************************************************************************************/
void osd_bright_text_param( void ) {
	graphic_set_string( &Bright1,  0, 0, (FONT_INFO *)&Arial6pt8FontInfo_var );
	graphic_set_string( &Bright2, 23, 0, (FONT_INFO *)&Arial6pt8FontInfo_var );

	Bright1.str = (CHAR *)bright1;
}
/********************************************************************************************/

/********************************************************************************************/
static void osd_bright_display_number( T_DISPLAY * buffer, uint8_t number ) {
	graphic_puts_RAM( buffer, &Bright1, FONTx1, WhiteColor, BlackColor, &GammaRGB );

	if ( number < AUTO_BRIGHT) {
		graphic_puts_int_RAM( buffer, &Bright2, number, FONTx1, WhiteColor, BlackColor, &GammaRGB );
	} else
	if ( number == AUTO_BRIGHT) {
		Bright2.str = (CHAR *)brightAU;
		graphic_puts_RAM( buffer, &Bright2, FONTx1, WhiteColor, BlackColor, &GammaRGB );
	} else
	if ( number == DYNAMIC_BRIGHT) {
		Bright2.str = (CHAR *)brightDYN;
		graphic_puts_RAM( buffer, &Bright2, FONTx1, WhiteColor, BlackColor, &GammaRGB );
	}
}

static int8_t bright = 0;
/**********************************************************************/
void osd_bright_display_plus( void ) {
	bright++;
	if (bright > 9) bright = 9;

	HT1632C_brightness( bright );

	T_DISPLAY * NoActiveBuffer = fb_get_noActive_buffer(buffer1);

	fb_clear_screen	 ( NoActiveBuffer );
	osd_bright_display_number( NoActiveBuffer, bright );
	fb_buffer_is_ready(buffer1);
}
void osd_bright_display_minus( void ) {
	bright--;
	if (bright < 0) bright = 0;

	HT1632C_brightness( bright );

	T_DISPLAY * NoActiveBuffer = fb_get_noActive_buffer(buffer1);

	fb_clear_screen	 ( NoActiveBuffer );
	osd_bright_display_number( NoActiveBuffer, bright );
	fb_buffer_is_ready(buffer1);
}
/**********************************************************************/
