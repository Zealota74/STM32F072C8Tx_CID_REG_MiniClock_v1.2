/*
 * sw_mcu_conf.h
 *
 *  Created on: 15.12.2017
 *      Author: simon
 */

#ifndef LIBS_SW_MCU_CONF_H_
#define LIBS_SW_MCU_CONF_H_

/********* Board type ********/
// defined in global project settings
/*****************************/

/******* A tutaj wybieramy procesor *****************/
#ifdef KA_NUCLEO
#include "stm32f4xx.h"
#endif

#ifdef NUCLEO_STM32F303RE
#include "stm32f3xx.h"
#endif

#ifdef STM32F303CBTx_mini
#define BSRR_REG
#include "stm32f3xx.h"
#endif

#ifdef STM32F072C8Tx_MINICLOCK
#define BSRR_REG
#include "stm32f0xx.h"
#endif
/****************************************************/

#include "sw_hal_conf.h"

#endif /* LIBS_SW_MCU_CONF_H_ */
