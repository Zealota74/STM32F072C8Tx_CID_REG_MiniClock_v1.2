/*
 * ds3231.h
 *
 *  Created on: 6 lis 2016
 *      Author: simon
 */

#ifndef LIBS_DS3231_H_
#define LIBS_DS3231_H_


//#define DS3231_I2C_ADDR			0xD0	// 8-bit address
#define DS3231_I2C_ADDR			0x68	// 7-bit address

#define DS3231_CONTROL			0x0E

#define DS3231_TIME_ADDR		0x00
#define DS3231_DATE_ADDR		0x03
#define DS3231_ALARM1_ADDR		0x07
#define DS3231_ALARM2_ADDR		0x0B
#define DS3231_TEMP_CEL_ADDR	0x11

#define DS3231_INTCN	2
#define DS3231_RS1		3
#define DS3231_RS2		4

typedef union {
	uint8_t buffer[8];
	struct {
		uint8_t memAddress;
		uint8_t data[7];
	};
} T_I2CFRAME;


typedef struct {
	int8_t  cel;
	uint8_t fract;
} T_TEMP;
/******************************************************************************************/

void ds3231_get_datetime( void );

extern bool ds3231_init( void );
extern void ds3231_get_datetime( void );
extern void ds3231_get_temperature( T_TEMP *temp );

extern void ds3231_rtc_get_time( uint8_t * buff );

extern void ds3231_set_time( uint8_t hh,  uint8_t mm,  uint8_t ss);
extern void ds3231_set_date( uint8_t year,  uint8_t month,  uint8_t day, uint8_t dayofweek);

void EXTI2_3_IRQHandler( void );


bool at24cxx_init(void);

#endif /* LIBS_DS3231_H_ */
