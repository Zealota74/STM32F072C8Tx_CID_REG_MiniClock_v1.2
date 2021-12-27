/*
 * sw_i2c.h
 *
 *  Created on: 02.09.2018
 *      Author: simon
 */

#ifndef LIBS_SW_STM_PERIPHS_SW_I2C_H_
#define LIBS_SW_STM_PERIPHS_SW_I2C_H_


#define I2C_SPEED_100kHz	1
#define I2C_SPEED_400kHz	2
#define I2C_SPEED			I2C_SPEED_400kHz

#define I2C_8BIT_ADDRESS	0
#define I2C_7BIT_ADDRESS	1

#define I2C_8BIT_ADDR	1

#define I2C_ERROR	(bool)0
#define I2C_OK		(bool)1
#define I2C_NACK	(bool)2

#define I2C_ADDRESS_BYTE	1
#define I2C_ADDRESS_HWORD	2		// Half word - 2 bytes
#define I2C_ADDRESS_FWORD	3		// Word		 - 4 bytes



void sw_i2c_init( void );
bool sw_i2c_test_slave( uint8_t slaveAddr, uint8_t testData );


void DMA1_Channel6_IRQHandler( void );
void DMA1_Channel7_IRQHandler( void );
void I2C1_ER_IRQHandler( void );

void sw_i2c_dma_send_buff( uint8_t slaveAddr, uint8_t *buff, uint16_t sizeBuff );
void sw_i2c_dma_get_buff ( uint8_t slaveAddr, uint8_t *buff, uint16_t sizeBuff, uint8_t wordLength );


#endif /* LIBS_SW_STM_PERIPHS_SW_I2C_H_ */
