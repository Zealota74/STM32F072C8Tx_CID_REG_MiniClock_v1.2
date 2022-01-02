/*
 * sw_sw_iwdg.c
 *
 *  Created on: 17.09.2018
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"



#include "sw_iwdg.h"
INLINE void Configure_IWDG(void) {
	/* Enable the peripheral clock RTC */
	/* (1) Enable the LSI */
	/* (2) Wait while it is not ready */
	RCC->CSR |= RCC_CSR_LSION; /* (1) */
	while ((RCC->CSR & RCC_CSR_LSIRDY) != RCC_CSR_LSIRDY) /* (2) */
	{
		/* add time out here for a robust application */
	}

//  Enable the peripheral clock of DBG register ( uncomment for debug purpose ) //
//  RCC->APB2ENR |= RCC_APB2ENR_DBGMCUEN;
//  DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP; // To be able to debug //

	/* Configure IWDG */
	/* (1) Activate IWDG (not needed if done in option bytes) */
	/* (2) Enable write access to IWDG registers */
	/* (3) Set prescaler by 8 */
	/* (4) Set reload value to have a roll-over each 1s */
	/* (5) Check if flags are reset */
	/* (6) Refresh counter */
	IWDG->KR = IWDG_KR_START; /* (1) */
	IWDG->KR = IWDG_KR_WRITE_ACCESS; /* (2) */

	IWDG->PR = IWDG_PR_PR_0; /* (3) */
	IWDG->RLR = IWDG_RELOAD; /* (4) */
	while (IWDG->SR) { /* (5) */
		/* add time out here for a robust application */
	}
	IWDG->KR = IWDG_KR_REFRESH; /* (6) */
}
