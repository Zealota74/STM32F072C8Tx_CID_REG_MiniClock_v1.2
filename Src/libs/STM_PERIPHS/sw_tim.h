/*
 * sw_tim.h
 *
 *  Created on: 28.02.2018
 *      Author: simon
 */

#ifndef LIBS_SW_STM_PERIPHS_SW_TIM_H_
#define LIBS_SW_STM_PERIPHS_SW_TIM_H_


#define PRIGROUP_0G_16S	((const uint32_t) 0x07)
#define PRIGROUP_2G_8S	((const uint32_t) 0x06)
#define PRIGROUP_4G_4S	((const uint32_t) 0x05)
#define PRIGROUP_8G_2S	((const uint32_t) 0x04)
#define PRIGROUP_16G_0S	((const uint32_t) 0x03)


static inline void __attribute__((always_inline)) sw_timer_start( TIM_TypeDef * tim ) {
	tim->CR1 |= TIM_CR1_CEN;			// Counter start
}
static inline void __attribute__((always_inline)) sw_timer_stop( TIM_TypeDef * tim ) {
	tim->CR1 &= ~TIM_CR1_CEN;		// Counter stop
}
static inline void __attribute__((always_inline)) sw_timer_opm_start( TIM_TypeDef * tim ) {
	tim->CR1 |= TIM_CR1_OPM;
}

void sw_tim7_init( void );
void nvic_priority(void);

#endif /* LIBS_SW_STM_PERIPHS_SW_TIM_H_ */
