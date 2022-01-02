/*
 * input.h
 *
 *  Created on: 1 sty 2022
 *      Author: simon
 */

#ifndef LIBS_SW_INPUT_INPUT_H_
#define LIBS_SW_INPUT_INPUT_H_

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"




extern void input_init(void);
extern void input_keyboard_proc( void );

extern void input_event(void);


#endif /* LIBS_SW_INPUT_INPUT_H_ */
