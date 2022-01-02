/*
 * sw_iwdg.c
 *
 *  Created on: 17.09.2018
 *      Author: simon
 */

#ifndef LIBS_SW_STM_PERIPHS_SW_IWDG_C_
#define LIBS_SW_STM_PERIPHS_SW_IWDG_C_


#define IWDG_KR_REFRESH			(uint32_t)0xAAAA
#define IWDG_KR_WRITE_ACCESS	(uint32_t)0x5555
#define IWDG_KR_START			(uint32_t)0xCCCC

#define IWDG_RELOAD				(uint32_t)0x1400	// 40Khz / 8 = 5120->0x1400 -> 1s watch



#endif /* LIBS_SW_STM_PERIPHS_SW_IWDG_C_ */
