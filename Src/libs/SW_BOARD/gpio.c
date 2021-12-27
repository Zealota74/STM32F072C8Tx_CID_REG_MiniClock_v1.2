/*
 * gpio.c
 *
 *  Created on: 06.07.2017
 *      Author: simon
 */

#include "sw_mcu_conf.h"
#include "sw_hal_conf.h"

#include "gpio.h"

/******************* Funkcja ustawiająca trybyb pracy pinów *********************/
void gpio_pin_cfg( GPIO_TypeDef * const port, T_GPIO_PIN pin, T_GPIO_MODE mode ) {
	if (mode & 0x100u)
		port->OTYPER |= pin;
	else
		port->OTYPER &= (uint32_t) ~pin;
	pin = __builtin_ctz(pin) * 2;

	uint32_t reset_mask = ~(0x03u << pin);
	uint32_t reg_val;

	reg_val = port->MODER;
	reg_val &= reset_mask;
	reg_val |= (((mode & 0x600u) >> 9u) << pin);
	port->MODER = reg_val;
	reg_val = port->PUPDR;
	reg_val &= reset_mask;
	reg_val |= (((mode & 0x30u) >> 4u) << pin);
	port->PUPDR = reg_val;
	reg_val = port->OSPEEDR;
	reg_val &= reset_mask;
	reg_val |= (((mode & 0xC0u) >> 6u) << pin);
	port->OSPEEDR = reg_val;

	volatile uint32_t * reg_adr;
	reg_adr = &port->AFR[0];
	pin *= 2;

	if (pin > 28) {
		pin -= 32;
		reg_adr = &port->AFR[1];
	}
	reg_val = *reg_adr;
	reg_val &= ~(0x0fu << pin);
	reg_val |= (uint32_t) (mode & 0x0ful) << pin;
	*reg_adr = reg_val;
}
/*****************************************************************************/

/********************************* Moja propozycja ***************************/
void sw_pin_setup( uint32_t pin, uint32_t mode ) {

				// Czyścimy ustawienia dla pinów 0 i 5
GPIOB->MODER &= ~( GPIO_MODER_MODER0 | GPIO_MODER_MODER5);

				// Definicja pinów 0, 2 i 3 jako funkcja alternatywna
GPIOB->MODER |= (ALT_PIN << 2 * pin) | (ALT_PIN << 2 * pin) | (ALT_PIN << 2 * pin)

				// Definicja pinów 7, 9 i 11 jako funkcja alternatywna
			  | (ALT_PIN << 2 * pin) | (ALT_PIN << 2 * pin) | (ALT_PIN << 2 * pin)

			  // Definicja pinów 13 i 14 jako analogowe
			  | (ANA_PIN << 2 * pin) | (ANA_PIN << 2 * pin)
			  ;
}
/********************************************************************************************/



#ifdef NUCLEO_STM32F303RE
void RCC_gpio_init(void) {

#ifdef RTC_PORTA_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
#endif
#ifdef RTC_PORTB_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
#endif
#ifdef RTC_PORTC_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
#endif
#ifdef RTC_PORTD_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIODEN;
#endif
#ifdef RTC_PORTE_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
#endif
#ifdef RTC_PORTF_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
#endif
#ifdef RTC_PORTG_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOGEN;
#endif
#ifdef RTC_PORTH_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOHEN;
#endif
}
#endif
#ifdef KA_NUCLEO
void RCC_gpio_init( void ) {

#ifdef RTC_PORTA_ENABLED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
#endif
#ifdef RTC_PORTB_ENABLED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
#endif
#ifdef RTC_PORTC_ENABLED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
#endif
#ifdef RTC_PORTD_ENABLED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
#endif
#ifdef RTC_PORTE_ENABLED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
#endif
#ifdef RTC_PORTF_ENABLED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
#endif
#ifdef RTC_PORTG_ENABLED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
#endif
#ifdef RTC_PORTH_ENABLED
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
#endif
}
#endif

#ifdef STM32F072C8Tx_MINICLOCK
void RCC_gpio_init( void ) {

#ifdef RTC_PORTA_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
#endif
#ifdef RTC_PORTB_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
#endif
#ifdef RTC_PORTC_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
#endif
#ifdef RTC_PORTD_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIODEN;
#endif
#ifdef RTC_PORTE_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
#endif
#ifdef RTC_PORTF_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
#endif
#ifdef RTC_PORTG_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOGEN;
#endif
#ifdef RTC_PORTH_ENABLED
	RCC->AHBENR |= RCC_AHBENR_GPIOHEN;
#endif
}
#endif

#ifdef BSRR_REG
	OPTIMISE("O0") inline void gpio_pin_LO(GPIO_TypeDef * const port, T_GPIO_PIN pin) {
		port->BRR = pin;
	}
	OPTIMISE("O0") inline void gpio_pin_HI(GPIO_TypeDef * const port, T_GPIO_PIN pin) {
		port->BSRR = pin;
	}
#else
	inline void gpio_pin_LO( GPIO_TypeDef * const port, T_GPIO_PIN pin ) {
		port->BSRRH = pin;
	}
	inline void gpio_pin_HI(  GPIO_TypeDef * const port, T_GPIO_PIN pin ) {
		port->BSRRL = pin;
	}
#endif

	OPTIMISE("O0") inline void gpio_pin_XOR( GPIO_TypeDef * const port, T_GPIO_PIN pin ) {
	port->ODR ^= pin;
}

void gpio_pin_IMPULSES( GPIO_TypeDef * const port, T_GPIO_PIN pin, uint8_t num ) {
	num = 2*num + 2;
	while ( --num ) {
		if ( num%2 ) {
			port->BRR = pin;
		} else {
			port->BSRR = pin;
		}
	}
}
