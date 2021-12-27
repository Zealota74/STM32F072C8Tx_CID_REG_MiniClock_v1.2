/*
 * sw_stm32_rtc.c
 *
 *  Created on: 20.07.2018
 *      Author: simon
 */


#include "sw_mcu_conf.h"

#include "sw_rtc_stm32.h"
#include "../data_conv_v2.h"

//#include "../../SW_STM_PERIPHS/sw_iwdg.h"

void STM32F051_rtc_calib( void ) {
	/* (1) Write access for RTC registers */
	/* (2) Enable init phase */
	/* (3) Wait until it is allow to modify RTC register values */
	/* (4) set prescaler, 40kHz/125 => 320 Hz, 320Hz/320 => 1Hz */
	/* (5) New time in TR */
	/* (6) Disable init phase */
	/* (7) Wait until it's allow to modify calibartion register */
	/* (8) Set calibration to around +20ppm, which is a standard value @25°C */
	/* Note: the calibration is relevant when LSE is selected for RTC clock */
	/* (9) Disable write access for RTC registers */
	RTC->WPR = 0xCA; 									/* (1) */
	RTC->WPR = 0x53; 									/* (1) */
	RTC->ISR |= RTC_ISR_INIT; 							/* (2) */
	while ((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF) /* (3) */
	{
	 /* add time out here for a robust application */
	}
	RTC->PRER 	= (124<<16) | 319; 							/* (4) */
	RTC->TR 	= RTC_TR_PM;//| Time; 						/* (5) */
	RTC->ISR &=~ RTC_ISR_INIT; 								/* (6) */
	while((RTC->ISR & RTC_ISR_RECALPF) == RTC_ISR_RECALPF) 	/* (7) */
	{
	 // add time out here for a robust application //
	}
	RTC->CALR 	= RTC_CALR_CALP | 482; 						/* (8) */
	RTC->WPR 	= 0xFE; 									/* (9) */
	RTC->WPR 	= 0x64; 									/* (9) */
}

/*********************** Base functions *******************************/
static void STM32F051_Rtc_init_on(void) {
	RTC->ISR |= RTC_ISR_INIT;		// enter initialization mode.
	while ( (RTC->ISR & RTC_ISR_INITF ) != RTC_ISR_INITF) {}
}
static void STM32F051_Rtc_init_off(void) {
	RTC->ISR &= ~RTC_ISR_INIT;		// Exit the initialization mode
}

static void STM32F051_Rtc_protect_off(void) {
	PWR->CR	|= PWR_CR_DBP;			// Write protection OFF
	RTC->WPR = WPR_KEY1;			// Unlock the write protection
	RTC->WPR = WPR_KEY2;			// on all the RTC registers
}
static void STM32F051_Rtc_protect_on(void) {
	PWR->CR	 &= ~PWR_CR_DBP;		// Write protection ON
}
/**********************************************************************/

/**********************************************************************/
void STM32F051_Rtc_Clock_Config( void ) {
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	STM32F051_Rtc_protect_off();

	RCC->BDCR &= ~RCC_BDCR_RTCEN;	// RTC clock enabled

#if RTC_CLOCK == LSI_CLOCK
	RCC->CSR |= RCC_CSR_LSION;
	while ( (RCC->CSR & RCC_CSR_LSIRDY) == 0 ) {}
#elif RTC_CLOCK == LSE_CLOCK
	RCC->BDCR |= RCC_BDCR_LSEON;	// LSE 32kHz clock enabled
	while ( (RCC->BDCR & RCC_BDCR_LSERDY) == 0 ) {}
#endif

	RCC->BDCR &= ~RCC_BDCR_RTCSEL;		// Reset oscillator selection

#if RTC_CLOCK == LSI_CLOCK
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSI;	// Internal 40KHz oscillator clock
#elif RTC_CLOCK == LSE_CLOCK
	RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;	// External 32KHz oscillator clock
#endif
	RCC->BDCR |= RCC_BDCR_RTCEN;		// RTC clock enabled

	STM32F051_Rtc_protect_on();

	// Activate the watchdog
	// Be sure reset watchdog unless 1s
//	configure_IWDG();
}
/*************************************************************************/

/*************************************************************************/
uint8_t  STM32F051_Rtc_Config( uint8_t resetRtcDomain ) {

	if ( resetRtcDomain == 1) {

	}
	else
	if ( NO_POWER_CHECK == RTC->BKP4R ) {
		return RTC_CONFIG_NOT_RESET;
	}
	STM32F051_Rtc_protect_off();
	STM32F051_Rtc_init_on();

#if RTC_CLOCK == LSI_CLOCK
	// 40KHz/320/256 = 1Hz
	RTC->PRER = (128-1) << RTC_PRER_PREDIV_A_Pos |	// The asynchronous prescaler division factor is set to 128
				(320-1) << RTC_PRER_PREDIV_S_Pos;	// and the synchronous division factor to 256
#else
	// 32KHz/128/256 = 1Hz
	RTC->PRER = (128-1) << RTC_PRER_PREDIV_A_Pos |	// The asynchronous prescaler division factor is set to 128
				(256-1) << RTC_PRER_PREDIV_S_Pos;	// and the synchronous division factor to 256
#endif

//	RTC->CR |= RTC_CR_FMT;		// 12 hours format: AM/PM
	RTC->CR	|= RTC_CR_ADD1H;	// Add 1 hour (summer time change)


	RTC->BKP4R = NO_POWER_CHECK;// Set magic number to backup register

	STM32F051_Rtc_init_off();
	STM32F051_Rtc_protect_on();

//	RTC->CR |= RTC_CR_TSIE;
//	NVIC_EnableIRQ( RTC_IRQn );

	return RTC_CONFIG_RESET;
}
/*************************************************************************/
//void RTC_IRQHandler( void ) {
//
//}
/**************************************** Get time from RTC *************************************/
void STM32F051_Rtc_Get_Time( uint8_t * bufferPTR ) {
	uint32_t tmp = RTC->TR & (RTC_TR_ST_Msk | RTC_TR_SU_Msk);
	*(bufferPTR + 0) = sw_bcd2dec( (uint8_t) tmp );							// Seconds

	tmp = RTC->TR & (RTC_TR_MNT_Msk | RTC_TR_MNU_Msk);
	*(bufferPTR + 1) = sw_bcd2dec( (uint8_t) (tmp >> RTC_TR_MNU_Pos ) );	// Minutes

	tmp = RTC->TR & (RTC_TR_HT_Msk | RTC_TR_HU_Msk);
	*(bufferPTR + 2) = sw_bcd2dec( (uint8_t) (tmp >> RTC_TR_HU_Pos) );		// Hours
}
/*************************************************************************************************/
/**************************************** Get date from RTC *************************************/
void STM32F051_Rtc_Get_Date( uint8_t * bufferPTR ) {
	uint32_t tmp = RTC->DR & (RTC_DR_WDU_Msk);
	*(bufferPTR + 3) = sw_bcd2dec( (uint8_t) (tmp >> RTC_DR_WDU_Pos) );		// Days of week

	tmp = RTC->DR & (RTC_DR_DT_Msk | RTC_DR_DU_Msk);
	*(bufferPTR + 4) = sw_bcd2dec( (uint8_t) (tmp >> RTC_DR_DU_Pos) );		// Days

	tmp = RTC->DR & (RTC_DR_MT_Msk | RTC_DR_MU_Msk);
	*(bufferPTR + 5) = sw_bcd2dec( (uint8_t) (tmp >> RTC_DR_MU_Pos) );		// Months

	tmp = RTC->DR & (RTC_DR_YT_Msk | RTC_DR_YU_Msk);
	*(bufferPTR + 6) = sw_bcd2dec( (uint8_t) (tmp >> RTC_DR_YU_Pos) );		// Years
}
/*************************************************************************************************/

/**************************************** Upload time to RTC *************************************/
void STM32F051_Rtc_Set_Time( uint8_t * bufferPTR ) {
	uint32_t tmp = 0;
	tmp =  (uint32_t)sw_dec2bcd( *(bufferPTR + 0) ) << RTC_TR_SU_Pos;		// Seconds
	tmp |= (uint32_t)sw_dec2bcd( *(bufferPTR + 1) ) << RTC_TR_MNU_Pos;		// Minutes
	tmp |= (uint32_t)sw_dec2bcd( *(bufferPTR + 2) ) << RTC_TR_HU_Pos;		// Hours

	STM32F051_Rtc_protect_off();
	STM32F051_Rtc_init_on();

	RTC->TR = tmp;															// Write time to RTC
	STM32F051_Rtc_init_off();
	STM32F051_Rtc_protect_on();
}
/*************************************************************************************************/
/**************************************** Upload date to RTC *************************************/
void STM32F051_Rtc_Set_Date( uint8_t * bufferPTR ) {
	uint32_t tmp = 0;
	tmp =  (uint32_t)sw_dec2bcd( *(bufferPTR + 3) ) << RTC_DR_WDU_Pos;		// Days of week
	tmp |= (uint32_t)sw_dec2bcd( *(bufferPTR + 4) ) << RTC_DR_DU_Pos;		// Days
	tmp |= (uint32_t)sw_dec2bcd( *(bufferPTR + 5) ) << RTC_DR_MU_Pos;		// Months
	tmp |= (uint32_t)sw_dec2bcd( *(bufferPTR + 6) ) << RTC_DR_YU_Pos;		// Years

	STM32F051_Rtc_protect_off();
	STM32F051_Rtc_init_on();
	RTC->DR = tmp;															// Write date to RTC
	STM32F051_Rtc_init_off();
	STM32F051_Rtc_protect_on();
}
/*************************************************************************************************/

void RTC_IRQHandler( void ) {
	static uint8_t simon;
	simon++;
}
