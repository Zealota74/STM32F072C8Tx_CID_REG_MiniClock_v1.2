/*
 * sw_mcu_conf.h
 *
 *  Created on: 15.12.2017
 *      Author: simon
 */

#ifndef SW_HAL_CONF_H_
#define SW_HAL_CONF_H_

#include "stdint.h"


#ifndef bool
typedef uint8_t bool;
#endif

#define TRUE				(uint8_t)1
#define FALSE				(uint8_t)0

#define INLINE inline 	__attribute__((always_inline))
#define OPTIMISE(x)		__attribute__((optimize(x)))


#endif /* SW_HAL_CONF_H_ */
