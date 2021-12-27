/*
 * data_conv.c
 *
 *  Created on: 6 lis 2016
 *      Author: simon
 */


#include "sw_mcu_conf.h"


uint8_t sw_dec2bcd( uint8_t dec ) {
	return ( (dec/10)<<4 ) | (dec % 10);
}
uint8_t sw_bcd2dec( uint8_t bcd ) {
	return (( ((bcd) >> 4) & 0x0F) * 10) + +((bcd) & 0x0F);
}

/*
 * Funkcja zmienia kolejność bitów w bajcie.
 * Dla Atmega644 i 20MHz wykonuje się ok 50 us
 *
uint8_t sw_swap_byte2( uint8_t data ) {
	uint8_t tmp1 = 0x01, tmp2 = 0x80;
	uint8_t reversByte = 0;

	do  {
		if (tmp1 & data)
			reversByte = reversByte | tmp2;
		tmp2 >>= 1;
		tmp1 <<= 1;
	} while ( tmp1 );

	return reversByte;
}
*****************************************************/
/*
 * A tutaj druga wersja.
 * Dla tego samego zegara i procka czas to 3.5 us (??)
 */
uint8_t sw_swap_byte( uint8_t data ) {
	uint8_t reversByte = 0;
	if ( data & 0x01) reversByte |= 0x80;
	if ( data & 0x02) reversByte |= 0x40;
	if ( data & 0x04) reversByte |= 0x20;
	if ( data & 0x08) reversByte |= 0x10;
	if ( data & 0x10) reversByte |= 0x08;
	if ( data & 0x20) reversByte |= 0x04;
	if ( data & 0x40) reversByte |= 0x02;
	if ( data & 0x80) reversByte |= 0x01;
	return reversByte;
}
/*****************************************************/

