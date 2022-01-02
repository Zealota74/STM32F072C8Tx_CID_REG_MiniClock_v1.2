/*
 * sw_dma.h
 *
 *  Created on: 24.04.2018
 *      Author: simon
 */

#ifndef LIBS_SW_STM_PERIPHS_SW_DMA_H_
#define LIBS_SW_STM_PERIPHS_SW_DMA_H_


void dma1_mem2mem( uint32_t * SrcBuff, uint32_t * DstBuff, uint32_t size, uint32_t bits );

enum { Bit_8, Bit_16, Bit_32 };

#endif /* LIBS_SW_STM_PERIPHS_SW_DMA_H_ */
