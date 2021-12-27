/*
 * rtc_manage.c
 *
 *  Created on: 6 lis 2016
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"
#include "../SW_TIMERS/sw_soft_timers.h"

#include "SW_RTC_STM32/sw_rtc_stm32.h"
#include "SW_RTC_DS3231/ds3231.h"

#include "../SW_GRAPHICS/text.h"
#include "sw_rtc_manage.h"


/********** Global structures for date and time ***************/
T_DATETIME	SetDateTime;
T_DATETIME	ShowDateTime;
/**************************************************************/

static void ( *datetime_event_callback )( void );
//static uint8_t 	rtcDatetimeFlag = 0;
//static bool 	i2cRtcExist 	= FALSE;


void copy_structures ( T_DATETIME *source, T_DATETIME *dest ) {
//	memcpy( dest->bytes, source->bytes, sizeof(source));
	dest->hh = source->hh;
	dest->mm = source->mm;
	dest->ss = source->ss;
}

void Rtc_Get_Time( uint8_t *hh, uint8_t *mm, uint8_t *ss ) {
	STM32F051_Rtc_Get_Time( (uint8_t *)&ShowDateTime );

	*hh = ShowDateTime.hh;
	*mm = ShowDateTime.mm;
	*ss = ShowDateTime.ss;
}

void Rtc_Get_Date( uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *dayOfWeek ) {
	STM32F051_Rtc_Get_Date( (uint8_t *)&ShowDateTime );

	*dayOfWeek 	= ShowDateTime.dayofweek;
	*day 		= ShowDateTime.day;
	*month 		= ShowDateTime.month;
	*year 		= ShowDateTime.year;
}

void Rtc_Set_Time( uint8_t hh, uint8_t mm, uint8_t ss ) {
	SetDateTime.hh = hh;
	SetDateTime.mm = mm;
	SetDateTime.ss = ss;
	STM32F051_Rtc_Set_Time( (uint8_t *)&SetDateTime );
}

void Rtc_Set_Date( uint8_t year, uint8_t month, uint8_t day, uint8_t dayOfWeek ) {
	SetDateTime.year		= year;
	SetDateTime.month		= month;
	SetDateTime.day			= day;
	SetDateTime.dayofweek	= dayOfWeek;

	STM32F051_Rtc_Set_Date( (uint8_t *)&SetDateTime );
}
/*************************************************************************/

/******** Zbiorcza funkcja do inicjalizacji zegara i peryferiów **********/
uint8_t clock_init( bool resetRtcDomain ) {
//	i2cRtcExist = ds3231_init();
//	if (i2cRtcExist == TRUE) return FALSE;

	STM32F051_Rtc_Clock_Config();
	if ( STM32F051_Rtc_Config( resetRtcDomain ) == RTC_CONFIG_RESET || resetRtcDomain == TRUE ) {
		Rtc_Set_Time( 23, 59, 50 );		// 23:59:50, default time
		Rtc_Set_Date( 10, 1, 1, 5 );	// 2010.07.01 Friday, default date
		return TRUE;
	}
	return FALSE;
	softTimer1 = 0;
}
/*************************************************************************/

/*************************************************************************/
void rtc_get_date_time( void ) {
	STM32F051_Rtc_Get_Time( (uint8_t *)&ShowDateTime );
	STM32F051_Rtc_Get_Date( (uint8_t *)&ShowDateTime );
}
/**************************************************************************/

/****************** Funkcje callback'owe i zdarzeniowe ********************/
void register_datetime_event_callback( void (*callback)( void ) ) {
	datetime_event_callback = callback;
}
void SW_DATETIME_EVENT( void ) {
	if ( softTimer1 == 0 ) {
		softTimer1 = 250;
		if( datetime_event_callback ) {
			datetime_event_callback();
		}
	}
}
/**************************************************************************/
/************* Liczy lata przestępne ***************************************
// jest podzielny przez 4, ale nie jest podzielny przez 100
// jest podzielny przez 400

uint8_t is_leap_year(uint16_t year) {
	if (   year%4==0		// Podzielny przez 4
		&& year%400==0 		// Podzielny przez 400
		&& year%100!=0 )	// Niepodzielny przez 100
		return 1;	// przestępny
	else
		return 0;	// zwykły
}
*************************************************************************/

