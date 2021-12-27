/*
 * ht3216c.h
 *
 *  Created on: 29.06.2018
 *      Author: simon
 */

#ifndef HT3216C_H_
#define HT3216C_H_


/******************************** SW HT1632C ********************************/

#define HT_CS_PORT		PORTA
#define HT_CS_PIN		PA9
#define HT_CS_LO		gpio_pin_LO(HT_CS_PORT, HT_CS_PIN)
#define HT_CS_HI		gpio_pin_HI(HT_CS_PORT, HT_CS_PIN)

#define HT_WR_PORT		PORTB
#define HT_DATA_PORT	PORTB
#define HT_RD_PORT		PORTA

#define HT_WR_PIN		PB13
#define HT_DATA_PIN		PB15
#define HT_RD_PIN		PA8

#define HT_WR_LO		gpio_pin_LO(HT_WR_PORT, HT_WR_PIN)
#define HT_WR_HI		gpio_pin_HI(HT_WR_PORT, HT_WR_PIN)
#define HT_DATA_LO		gpio_pin_LO(HT_DATA_PORT, HT_DATA_PIN)
#define HT_DATA_HI		gpio_pin_HI(HT_DATA_PORT, HT_DATA_PIN)
#define HT_RD_LO		gpio_pin_LO(HT_RD_PORT, HT_RD_PIN)
#define HT_RD_HI		gpio_pin_HI(HT_RD_PORT, HT_RD_PIN)

#define SPI_HW_MODE			1
#define SPI_SW_MODE			2
#define SPI_TRANSFER_MODE	SPI_HW_MODE

#define	HORIZONTAL_BYTE		1
#define	VERTICAL_BYTE		2
#define	DISPLAY_MODE		HORIZONTAL_BYTE
#define HARDWARE_SYNC

#define HT1632C_BRIGHT_MIN	0
#define HT1632C_BRIGHT_MAX	15

#define DYNA_BRIGHT_OFF		0	// Dynamic brightness disable
#define DYNA_BRIGHT_ON		1	// Dynamic brightness enable
#define AUTO_BRIGHT			8
#define DYNAMIC_BRIGHT		9

#define HT1632C_START_SYS	(uint16_t)0b100000000010		// start system oscillator
#define HT1632C_STOP_SYS	(uint16_t)0b100000000000		// stop system oscillator and LED duty    <default
#define HT1632C_SET_CLOCK	(uint16_t)0b100000110000		// set clock to master with internal RC  <default
#define HT1632C_SET_LEYOUT	(uint16_t)0b100001000000		// NMOS 32*8 // 0b100-0010-ab00-0  a:0-NMOS,1-PMOS; b:0-32*8,1-24*16   default:ab=10
#define HT1632C_LED_ON		(uint16_t)0b100000000110		// start LEDs
#define HT1632C_LED_OFF		(uint16_t)0b100000000100		// stop LEDs    <default
#define HT1632C_SET_BRIGHT	(uint16_t)0b100101000000		// set brightness b = 0..15  add b<<1  //0b1001010xxxx0 xxxx:brightness 0..15=1/16..16/16 PWM
#define HT1632C_BLINK_ON	(uint16_t)0b100000010010		// Blinking on
#define HT1632C_BLINK_OFF	(uint16_t)0b100000010000		// Blinking off  <default
#define HT1632C_WRITE		(uint16_t)0b1010000000			// 101-aaaaaaa-dddd-dddd-dddd-dddd-dddd-... aaaaaaa:nibble address 0..3F   (5F for 24*16)

// ADRESS:	MSB first
// DATA:	LSB first     transferring a byte (MSB first) fills one row of one 8*8-matrix, MSB left, starting with the left matrix
// timing: pull strobe LOW, bits evaluated at rising clock edge, strobe high
// commands can be queued: 100-ccccccccc-ccccccccc-ccccccccc-... (ccccccccc: without 100 at front)
// setup: cast START_SYS, SET_CLOCK, SET_LAYOUT, LED_ON, brightness+(15<<1), BLINK_OFF


extern void HT1632C_init( void );
extern void HT1632C_brightness( uint16_t bright );
extern void TIM2_IRQHandler( void );
extern void EXTI4_15_IRQHandler(void);

extern void HT1632C_fill_screen( uint8_t pattern );
/****************************************************************************/


extern void register_scroll_event_callback( void (*callback)( uint8_t aaa ) );
extern void SW_SCROLL_EVENT( uint16_t * softTimer, uint8_t scrollOn );

extern void HT1632C_led_on( void );
extern void HT1632C_led_off( void );

#endif /* HT3216C_H_ */
