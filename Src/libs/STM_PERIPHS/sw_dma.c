/*
 * sw_dma.c
 *
 *  Created on: 24.04.2018
 *      Author: simon
 */

#include "sw_mcu_conf.h"

static DMA_Channel_TypeDef *DMA_MEM2MEM_Channel = DMA1_Channel7;

/***** UWAGA niegotowe ************************************/

void dma1_mem2mem( uint32_t * SrcBuff, uint32_t * DstBuff, uint32_t size, uint32_t bits ) {
	// Source buffer size MUST be <= destination buffer

	RCC->AHBENR |= RCC_AHBENR_DMA1EN;					// DMA1 clock enabled

	// CNDTR, CMAR and CPAR allow modified when channel is OFF
	DMA_MEM2MEM_Channel->CCR &= ~DMA_CCR_EN;			// Clear CCR_EN bit - ready to CMAR,CPAR,CNDTR modification

	DMA_MEM2MEM_Channel->CMAR = (uint32_t)( (uint8_t *)SrcBuff );		// Memory start address, 32-bit pointer
	DMA_MEM2MEM_Channel->CPAR = (uint32_t)( (uint8_t *)DstBuff );		// Peripheral start address, 32-bit pointer
	DMA_MEM2MEM_Channel->CNDTR = size;

	DMA_MEM2MEM_Channel->CCR = DMA_CCR_MEM2MEM |		// Copy memory to memory
//						 DMA_CCR_MSIZE_0 |		// 0b00 - 8 bit
//						 DMA_CCR_MSIZE_1 |		// 0b01 - 16 bit
//						 DMA_CCR_PSIZE_0 |		// 0b10 - 32 bit
//						 DMA_CCR_PSIZE_1 |		//
//						 DMA_CCR_MINC | 		// Memory increment mode enabled
						 DMA_CCR_PINC | 		// Peripheral increment mode enabled
						 DMA_CCR_DIR;			// Data transfer direction. CMAR -> CPAR (Memory to Peripheral)

	DMA_MEM2MEM_Channel->CCR &= ~(DMA_CCR_MSIZE | DMA_CCR_PSIZE);
	bits = (bits << DMA_CCR_PSIZE_Pos);
	DMA_MEM2MEM_Channel->CCR |= bits;

	DMA_MEM2MEM_Channel->CCR |= DMA_CCR_EN;		// DMA channel1 ON, start of copy

	while( (DMA1->ISR & DMA_ISR_TCIF7) == 0 );	// Wait for Channel1 Transfer Complete
	DMA1->IFCR = DMA_IFCR_CTCIF7;
}
