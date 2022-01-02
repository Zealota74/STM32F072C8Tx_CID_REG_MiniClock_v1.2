/*
 * rtc_manage.h
 *
 *  Created on: 6 lis 2016
 *      Author: simon
 */

#ifndef LIBS_RTC_MANAGE_H_
#define LIBS_RTC_MANAGE_H_

#define ALIGN_LTOP			(uint8_t)0
#define ALIGN_CTOP			(uint8_t)3
#define ALIGN_RTOP			(uint8_t)6

#define ALIGN_LMEDIUM		(uint8_t)1
#define ALIGN_CMEDIUM		(uint8_t)4
#define ALIGN_RMEDIUM		(uint8_t)7

#define ALIGN_LBOTTOM		(uint8_t)2
#define ALIGN_CBOTTOM		(uint8_t)5
#define ALIGN_RBOTTOM		(uint8_t)8

#define RETURN_DAYS			(uint8_t)0
#define RETURN_MONTHS		(uint8_t)0

/************************** Unia przechowująca dane dot. daty i czasu *********************/
typedef union {
	uint8_t bytes[7];
	struct {
		uint8_t ss;
		uint8_t mm;
		uint8_t hh;
		uint8_t dayofweek;
		uint8_t day;
		uint8_t month;
		uint8_t year;
	};
} T_DATETIME;

typedef struct {
	uint8_t timeYpos;
	uint8_t hhPos;
	uint8_t mmPos;
	uint8_t ssPos;
	uint8_t delim1Pos;
	uint8_t delim2Pos;
} T_TIME_POS;

//typedef struct {
//	T_TIME_POS time_pos;
//	FONT_INFO * font;
//	uint32_t fontColor;
//} T_CLOCK;

#define SET_TO_SHOW		0
#define SHOW_TO_SET		1

extern T_DATETIME	SetDateTime;
extern T_DATETIME	ShowDateTime;


uint8_t clock_init( bool resetRtcDomain );

void Rtc_Get_Time( uint8_t *hh, uint8_t *mm, uint8_t *ss );
void Rtc_Get_Date( uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *dayOfWeek );
void Rtc_Set_Time( uint8_t hh, uint8_t mm, uint8_t ss );
void Rtc_Set_Date( uint8_t year, uint8_t month, uint8_t day, uint8_t dayOfWeek );

void rtc_get_date_time(void);

void SW_DATETIME_EVENT(void);
void register_datetime_event_callback( void (*callback)(void) );

extern void Rtc_copy_structures( uint8_t direction );

#endif /* LIBS_RTC_MANAGE_H_ */
