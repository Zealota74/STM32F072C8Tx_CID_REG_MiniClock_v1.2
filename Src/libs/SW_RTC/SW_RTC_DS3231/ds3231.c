/*
 * ds3231.c
 *
 *  Created on: 6 lis 2016
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"

#include "gpio.h"

#include "../sw_i2c.h"
//#include "../../SW_GRAPHICS/SW_HT1632C/framebuffer.h"
//#include "../../SW_GRAPHICS/fonts_common.h"
//#include "../sw_rtc_manage.h"


#include "../data_conv_v2.h"
#include "ds3231.h"
#include "../../SW_BOARD/sw_led_blink_debug.h"


static T_I2CFRAME i2cRxBuff;
static T_I2CFRAME i2cTxBuff;


/*************************************************************************/
static INLINE void Configure_EXTI(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	gpio_pin_cfg( PORTB, PB3, gpio_mode_in_PU );

	SYSCFG->EXTICR[0] 	|= SYSCFG_EXTICR1_EXTI3_PB;
	EXTI->FTSR 			|= EXTI_FTSR_TR3;
	EXTI->RTSR 			|= EXTI_RTSR_TR3;
	EXTI->IMR 			|= EXTI_IMR_MR3;

	NVIC_EnableIRQ( EXTI2_3_IRQn );
}
void EXTI2_3_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR3) {
		EXTI->PR = EXTI_PR_PR3;
		ds3231_get_datetime();
	}
}
/*************************************************************************/

/*************************************************************************/
bool ds3231_init(void) {
	sw_i2c_init();

	uint8_t ds3231Ctrl = 0;
	bool test = sw_i2c_test_slave( DS3231_I2C_ADDR, ds3231Ctrl );
	if (test == I2C_ERROR || test == I2C_NACK)
		return I2C_ERROR;

	Configure_EXTI();
	return I2C_OK;
}
/*************************************************************************/

/************************* Read from DS3231 ******************************/
void ds3231_get_datetime( void ) {
	i2cRxBuff.memAddress = DS3231_TIME_ADDR; 	// memory address to read
	sw_i2c_dma_get_buff( DS3231_I2C_ADDR, (uint8_t *)&i2cRxBuff,
						 7, I2C_ADDRESS_BYTE );
}
/*************************************************************************/
/*************************************************************************/
void ds3231_get_temperature( T_TEMP * temp ) {
	i2cRxBuff.memAddress = DS3231_TEMP_CEL_ADDR; 	// memory address  to read
	sw_i2c_dma_get_buff( DS3231_I2C_ADDR, (uint8_t *)&i2cRxBuff,
						 3, I2C_ADDRESS_BYTE );
}
/*************************************************************************/


/************************** Write to DS3231 ******************************/
void ds3231_set_time( uint8_t hh,  uint8_t mm,  uint8_t ss ) {

	i2cTxBuff.memAddress = DS3231_TIME_ADDR; 	// memory address
	i2cTxBuff.data[0] = sw_dec2bcd(ss);
	i2cTxBuff.data[1] = sw_dec2bcd(mm);
	i2cTxBuff.data[2] = sw_dec2bcd(hh);
	sw_i2c_dma_send_buff( DS3231_I2C_ADDR, (uint8_t *)&i2cTxBuff, 1 + 3 );
}
/*************************************************************************/
/*************************************************************************/
void ds3231_set_date( uint8_t year,  uint8_t month,  uint8_t day, uint8_t dayofweek ) {

	i2cTxBuff.memAddress = DS3231_DATE_ADDR; 	// memory address
	i2cTxBuff.data[0] = sw_dec2bcd(dayofweek);
	i2cTxBuff.data[1] = sw_dec2bcd(day);
	i2cTxBuff.data[2] = sw_dec2bcd(month);
	i2cTxBuff.data[3] = sw_dec2bcd(year);
	sw_i2c_dma_send_buff( DS3231_I2C_ADDR, (uint8_t *)&i2cTxBuff, 1 + 4 );
}
/*************************************************************************/

/*************************************************************************/
void ds3231_rtc_get_time( uint8_t * buff ) {
	buff[0] = sw_bcd2dec( i2cRxBuff.data[0]);	// Seconds
	buff[1] = sw_bcd2dec( i2cRxBuff.data[1]);	// Seconds
	buff[2] = sw_bcd2dec( i2cRxBuff.data[2]);	// Seconds
}
/*************************************************************************/

