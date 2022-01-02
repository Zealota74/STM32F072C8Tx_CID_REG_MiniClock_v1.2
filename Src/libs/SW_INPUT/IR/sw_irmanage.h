/*
 * sw_irmanage.h
 *
 *  Created on: 24.03.2018
 *      Author: simon
 */

#ifndef LIBS_SW_IR_SW_IRMANAGE_H_
#define LIBS_SW_IR_SW_IRMANAGE_H_

#define IR_OMIT				9									// Ilość impulsów, które pomijamy
#define NO_IR_CODE			0xFFFF								//
#define AUTOREPEAT_ON		1
#define AUTOREPEAT_OFF		0
#define AUTOREPEAT_MS		1000


#define ENTER_NO		0
#define ENTER_1			1
#define ENTER_2			2


#define IR_SIRC_ADDR	(uint16_t)0x813
#define	IR_KEY_UP		(uint16_t)0x2d5f
#define IR_KEY_DOWN		(uint16_t)0x2d60
#define IR_KEY_LEFT		(uint16_t)0x2d62
#define IR_KEY_RIGHT	(uint16_t)0x2d61
#define IR_KEY_OK		(uint16_t)0x2d5e

#define	IR_KEY_CHUP		(uint16_t)0x2d10
#define	IR_KEY_CHDOWN	(uint16_t)0x2d11
#define	IR_KEY_RED		(uint16_t)0x2d78	// KEY_F1			26
#define	IR_KEY_GREEN	(uint16_t)0x2d64	// KEY_F2			27
#define	IR_KEY_YELLOW	(uint16_t)0x2d7a	// KEY_F3			28
#define	IR_KEY_BLUE		(uint16_t)0x2d7b	// KEY_F4			29

void sw_ir_init( void );
uint16_t sw_ir_return_code( void );

void TIM14_IRQHandler(void);

void register_irDecoded_event_callback( void (*callback)( void ) );
void SW_IR_DECODED_EVENT( void );


#endif /* LIBS_SW_IR_SW_IRMANAGE_H_ */
