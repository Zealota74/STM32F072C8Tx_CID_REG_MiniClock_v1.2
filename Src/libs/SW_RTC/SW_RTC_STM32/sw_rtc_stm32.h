/*
 * sw_stm32_rtc.h
 *
 *  Created on: 20.07.2018
 *      Author: simon
 */

#ifndef SW_RTC_SW_RTC_STM32_H_
#define SW_RTC_SW_RTC_STM32_H_

#define LSE_CLOCK	1
#define LSI_CLOCK	2
#define RTC_CLOCK	LSE_CLOCK

#define WPR_KEY1	0xCA
#define WPR_KEY2	0x53

#define NO_POWER_CHECK			(uint32_t)0xAAAA
#define RTC_CONFIG_RESET		1
#define RTC_CONFIG_NOT_RESET	0



void STM32F051_Rtc_Clock_Config( void );
uint8_t STM32F051_Rtc_Config( uint8_t resetRtcDomain );

void STM32F051_Rtc_Get_Time( uint8_t * bufferPTR );
void STM32F051_Rtc_Get_Date( uint8_t * bufferPTR );
void STM32F051_Rtc_Set_Time( uint8_t * bufferPTR );
void STM32F051_Rtc_Set_Date( uint8_t * bufferPTR );


#endif /* SW_RTC_SW_RTC_STM32_H_ */
