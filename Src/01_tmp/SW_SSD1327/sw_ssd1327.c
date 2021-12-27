#ifndef SSD1327_CPP
#define SSD1327_CPP

#include <stddef.h>
#include <stdint.h>


//#include "Arduino.h"
//#include "SPI.h"
#include "font8x8_basic.h"
#include "font16x16.h"
#include "font16x32.h"

#include "sw_SSD1327.h"

//---------------- PRIVATE ----------------------------------------------
static uint8_t frameBuffer[8192];   			// Should mirror the display's own frameBuffer.
static uint8_t SSD1327__changedPixels[1024];	// Each bit of this array represets whether a given byte of frameBuffer (e.g. a pair of pixels) is not up to date.
static int _cs;
static int _dc;
static int _rst;
//-----------------------------------------------------------------------

//------------------------- Modifications -------------------------------
#include <stdbool.h>
#include <string.h>

#include "sw_mcu_conf.h"
#include "../../SW_BOARD/gpio.h"
#include "../../SW_STM_PERIPHS/sw_spi.v2.h"
#include "../../SW_TIMERS/sw_soft_timers.h"

#define LOW 	0
#define HIGH 	1
#define CS		1
#define DC		2
#define RST		3

#define DC_PORT		PORTA
#define DC_PIN		PA0
#define RST_PORT	PORTA
#define RST_PIN		PA0

static const SPI_param_t spiParam = { MSBFIRST, CPOL_HIGH, CPHA_LEADING };

//------------------------ Base functions ----------------------------------------------
void SPI_transfer( uint8_t data ) {
	sw_spi_hw_send_buff( &data, 1, spi_8bit );
}

void digitalWrite( uint8_t reg, uint8_t state ) {
	switch (reg) {
		case CS:
			state ? gpio_pin_HI( SOFT_NSS_PORT, SOFT_NSS_PIN ):
					gpio_pin_LO( SOFT_NSS_PORT, SOFT_NSS_PIN );
			break;
		case DC:
			state ?	gpio_pin_HI( DC_PORT, DC_PIN ):
					gpio_pin_LO( DC_PORT, DC_PIN );
			break;
		case RST:
			state ?	gpio_pin_HI( RST_PORT, RST_PIN ):
					gpio_pin_LO( RST_PORT, RST_PIN );
			break;
			default:
				break;
	}
}

int	bitRead( uint8_t reg, uint8_t bit ) {
	if ( reg & (1 << bit) )
		return 1;
	return 0;
}
void bitWrite( uint8_t reg, uint8_t bit, uint8_t state ){
	if (state) {
		reg |= (1 << bit);
	} else {
		reg &= ~(1 << bit);
	}
}
void delay( int delay ){ delay_ms(delay); }

uint8_t _min( uint8_t a, uint8_t b ) {
	if (a>b) return b;
	else if (a<b) return a;
	return a;
}
uint8_t _max( uint8_t a, uint8_t b ) {
	if (a>b) 		return a;
	else if (a<b) 	return b;
	return a;
}
int abs( int i ) {
	if (i>0) 		return i;
	else if (i<0) 	return -i;
	return 0;
}

void SSD1327(int cs, int dc, int rst) {
	_cs = cs;
	_dc = dc;
	_rst = rst;
}
//TODO: Find a way to handle the write commands without toggling CS and DC every time
void SSD1327__writeCmd(uint8_t reg) {	//Writes a command byte to the driver
	digitalWrite(_dc, LOW);
	digitalWrite(_cs, LOW);
	SPI_transfer(reg);
	digitalWrite(_cs, HIGH);
}

void SSD1327__writeData(uint8_t data){	//Writes 1 byte to the display's memory
	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, LOW);
	SPI_transfer(data);
	digitalWrite(_cs, HIGH);
}
//-----------------------------------------------------------------------------------


//-------------- Higher abstraction layer ----------------------------------------------
void SSD1327__setWriteZone(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { //defines a rectangular area of memory which the driver will itterate through. This function takes memory locations, meaning a 64x128 space
	SSD1327__writeCmd(0x15);	//Set Column Address
	SSD1327__writeCmd(x1);		//Beginning. Note that you must divide the column by 2, since 1 byte in memory is 2 pixels
	SSD1327__writeCmd(x2);		//End

	SSD1327__writeCmd(0x75);	//Set Row Address
	SSD1327__writeCmd(y1);		//Beginning
	SSD1327__writeCmd(y2);		//End
}

uint16_t SSD1327__coordsToAddress(uint8_t x, uint8_t y){ //Converts a pixel location to a linear memory address
	return (x/2)+(y*64);
}

void SSD1327__setPixelChanged(uint8_t x, uint8_t y, bool changed){
	uint16_t targetByte = SSD1327__coordsToAddress(x, y)/8;
	bitWrite(SSD1327__changedPixels[targetByte], SSD1327__coordsToAddress(x, y) % 8, changed);
}

void SSD1327__drawPixel(uint8_t x, uint8_t y, uint8_t color, bool display){//pixel xy coordinates 0-127, color 0-15, and whether to immediately output it to the display or buffer it
	int address = SSD1327__coordsToAddress(x,y);
	if((x%2) == 0){//If this is an even pixel, and therefore needs shifting to the more significant nibble
		frameBuffer[address] = (frameBuffer[address] & 0x0f) | (color<<4);
	} else {
		frameBuffer[address] = (frameBuffer[address] & 0xf0) | (color);
	}

	if(display){
		SSD1327__setWriteZone(x/2,y,x/2,y);
		SSD1327__writeData(frameBuffer[address]);
		SSD1327__setPixelChanged(x, y, false); // We've now synced the display with this byte of the buffer, no need to write it again
	} else {
		SSD1327__setPixelChanged(x, y, true); // This pixel is due for an update next refresh
	}
}

void SSD1327__drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color, bool display){//Draws a rectangle from x1,y1 to x2,y2.
	uint8_t xMin = _min(x1, x2); // TODO: double performance by writing whole bytes at a time
	uint8_t xMax = _max(x1, x2);
	uint8_t yMin = _min(y1, y2);
	uint8_t yMax = _max(y1, y2);
	for (uint8_t x = xMin; x <= xMax; x++) {
		for (uint8_t y = yMin; y <= yMax; y++) {
			SSD1327__drawPixel(x, y, color, display);
		}
	}
}

void SSD1327__drawHLine(int x, int y, int length, uint8_t color, bool display){
	for (uint8_t i = x; i < x+length; i++) {
		SSD1327__drawPixel(i, y, color, display);
	}
}

void SSD1327__drawVLine(int x, int y, int length, uint8_t color, bool display){
	for (uint8_t i = y; i < y+length; i++) {
		SSD1327__drawPixel(x, i, color, display);
	}
}

void SSD1327__drawLine(int x0, int y0, int x1, int y1, uint8_t color, bool display){	//Bresenham's line algorithm
	int deltaX = abs(x1-x0);
	int deltaY = abs(y1-y0);
	int signX = x0<x1 ? 1 : -1;
	int signY = y0<y1 ? 1 : -1;
	int error = (deltaX>deltaY ? deltaX : -deltaY)/2, error2;

	while (true) {
		SSD1327__drawPixel(x0, y0, color, display);
		if (x0==x1 && y0==y1) break;
		error2 = error;
		if (error2 >-deltaX) { error -= deltaY; x0 += signX; }
		if (error2 < deltaY) { error += deltaX; y0 += signY; }
	}
}

void SSD1327__drawByteAsRow(uint8_t x, uint8_t y, uint8_t byte, uint8_t color){			//Draws a byte as an 8 pixel row
	for (int i = 0; i < 8; i++) {
		if(bitRead(byte, i)){
			SSD1327__drawPixel(x+i, y, color, false);
		}
	}
}

void SSD1327__drawChar(uint8_t x, uint8_t y, char thisChar, uint8_t color){
	for (size_t i = 0; i < 8; i++) {
		SSD1327__drawByteAsRow(x, y+i, font8x8_basic[(unsigned char)thisChar][i], color);
	}
}
void SSD1327__drawChar16(uint8_t x, uint8_t y, char thisChar, uint8_t color){
	for (size_t row = 0; row < 16; row++) {
		SSD1327__drawByteAsRow(x, y+row, font16x16[(unsigned char)thisChar][row*2], color);
		SSD1327__drawByteAsRow(x+8, y+row, font16x16[(unsigned char)thisChar][(row*2)+1], color);
	}
}
void SSD1327__drawChar32(uint8_t x, uint8_t y, char thisChar, uint8_t color){
	for (size_t row = 0; row < 32; row++) {
		SSD1327__drawByteAsRow(x, y+row, font16x32[(unsigned char)thisChar][row*2], color);
		SSD1327__drawByteAsRow(x+8, y+row, font16x32[(unsigned char)thisChar][(row*2)+1], color);
	}
}

void SSD1327__drawCharArray(uint8_t x, uint8_t y, char text[], uint8_t color, int size){
	const char* thisChar;
	uint8_t xOffset = 0;
	if(size==16){
		for (thisChar = text; *thisChar != '\0'; thisChar++) {
			SSD1327__drawChar16(x+xOffset, y, *thisChar, color);
			xOffset += 8;
		}
	} else if(size==32){
		for (thisChar = text; *thisChar != '\0'; thisChar++) {
			SSD1327__drawChar32(x+xOffset, y, *thisChar, color);
			xOffset += 16;
		}
	}
	 else {
		for (thisChar = text; *thisChar != '\0'; thisChar++) {
			SSD1327__drawChar(x+xOffset, y, *thisChar, color);
			xOffset += 8;
		}
	}
}

void SSD1327__drawString( uint8_t x, uint8_t y, const char * textString, uint8_t color, int size ) {
	char text[64];
	memcpy( (char *)text, (const char *) textString, 64 );
	SSD1327__drawCharArray( x,y, text, color, size );
}


void SSD1327__fillStripes(uint8_t offset){ //gradient test pattern
	for(int i = 0; i < 8192; i++){
		uint8_t color = ((i+offset) & 0xF) | (((i+offset) & 0xF)<<4);
		frameBuffer[i] = color;
	}
	for (uint16_t i = 0; i < 1024; i++) {
		SSD1327__changedPixels[i] = 0xFF; // Set all pixels to be updated next frame. fillStripes should not be used without a full write anyways, but just in case
	}
}

void SSD1327__setupScrolling(uint8_t startRow, uint8_t endRow, uint8_t startCol, uint8_t endCol, uint8_t scrollSpeed, bool right){
	uint8_t swap;
	if (startRow > endRow) { // Ensure start row is before end
		swap = startRow;
		startRow = endRow;
		endRow = swap;
	}
	if (startCol > endCol) { // Ditto for columns
		swap = startCol;
		startCol = endCol;
		endCol = swap;
	}
	SSD1327__writeCmd(0x2E);   // Deactivate scrolling before changing anything
	if (right) {
		SSD1327__writeCmd(0x26); // Scroll right
	} else {
		SSD1327__writeCmd(0x27); // Scroll left
	}
	SSD1327__writeCmd(0); // Dummy byte
	SSD1327__writeCmd(startRow);
	SSD1327__writeCmd(scrollSpeed);
	SSD1327__writeCmd(endRow);
	SSD1327__writeCmd(startCol);
	SSD1327__writeCmd(endCol);
	SSD1327__writeCmd(0); // Dummy byte
};

void SSD1327__startScrolling() {
	SSD1327__writeCmd(0x2F);
}

void SSD1327__stopScrolling() {
	SSD1327__writeCmd(0x2E);
}

void SSD1327__scrollStep(uint8_t startRow, uint8_t endRow, uint8_t startCol, uint8_t endCol, bool right){
	SSD1327__setupScrolling(startRow, endRow, startCol, endCol, SSD1327_SCROLL_2, right);
	SSD1327__startScrolling();
	delay(15);
	SSD1327__stopScrolling();
}

void SSD1327__clearBuffer() {
	for(int i = 0; i < 8192; i++){
		if (frameBuffer[i]) { // If there is a non-zero (non-black) byte here, make sure it gets updated
			frameBuffer[i] = 0;
			bitWrite(SSD1327__changedPixels[i/8], i%8, 1); // Mark this pixel as needing an update
		}
	}
}

void SSD1327__writeFullBuffer() {		// Outputs the full framebuffer to the display
	SSD1327__setWriteZone(0,0,63,127);	//Full display
	for(int i = 0; i < 8192; i++){
		SSD1327__writeData(frameBuffer[i]);
	}
	for (uint16_t i = 0; i < 1024; i++) {
		SSD1327__changedPixels[i] = 0;	// Set all pixels as up to date.
	}
}

void SSD1327__writeUpdates() {			// Writes only the pixels that have changed to the display
	for (size_t y = 0; y < 128; y++) {
		bool continued = false; // If we can continue with the write zone we're using
		for (size_t x = 0; x < 128; x++) {
			uint16_t address = SSD1327__coordsToAddress(x, y);
			if ( bitRead(SSD1327__changedPixels[address/8], address % 8) ) { // If we need an update here
				if (!continued) { // Just write the byte, no new write zone needed
					continued = true;
					SSD1327__setWriteZone(x/2, y, 63, 127); // Set the write zone for this new byte and any subsequent ones
				}
				SSD1327__writeData(frameBuffer[address]);
				bitWrite(SSD1327__changedPixels[address/8], address % 8, 0);
			} else {
				continued = false; // The chain of pixels is broken
			}
		}
	}
}

void SSD1327__setContrast(uint8_t contrast){
	SSD1327__writeCmd(0x81);			//set contrast control
	SSD1327__writeCmd(contrast);		//Contrast byte
}

static void SSD1327__initRegs(){	//Sends all the boilerplate startup and config commands to the driver
	SSD1327__writeCmd(0xae);		//--turn off oled panel

	SSD1327__writeCmd(0x15);		//set column addresses
	SSD1327__writeCmd(0x00);		//start column  0
	SSD1327__writeCmd(0x7f);		//end column  127

	SSD1327__writeCmd(0x75);		//set row addresses
	SSD1327__writeCmd(0x00);		//start row  0
	SSD1327__writeCmd(0x7f);		//end row  127

	SSD1327__writeCmd(0x81);		//set contrast control
	SSD1327__writeCmd(0x80);		//50% (128/255)

	SSD1327__writeCmd(0xa0);		//gment remap
	SSD1327__writeCmd(0x51);		//51 (To my understanding, this is orientation

	SSD1327__writeCmd(0xa1); 		//start line
	SSD1327__writeCmd(0x00);

	SSD1327__writeCmd(0xa2);		//display offset
	SSD1327__writeCmd(0x00);

	SSD1327__writeCmd(0xa4);		//rmal display
	SSD1327__writeCmd(0xa8);		//set multiplex ratio
	SSD1327__writeCmd(0x7f);

	SSD1327__writeCmd(0xb1); 		//set phase leghth
	SSD1327__writeCmd(0xf1);

	SSD1327__writeCmd(0xb3);		//set dclk
	SSD1327__writeCmd(0x00);		//80Hz:0xc1 90Hz:0xe1  100Hz:0x00  110Hz:0x30 120Hz:0x50  130Hz:0x70   01

	SSD1327__writeCmd(0xab);		//Enable vReg
	SSD1327__writeCmd(0x01);

	SSD1327__writeCmd(0xb6);		//set phase leghth
	SSD1327__writeCmd(0x0f);

	SSD1327__writeCmd(0xbe);		//Set vcomh voltage
	SSD1327__writeCmd(0x0f);

	SSD1327__writeCmd(0xbc);		//set pre-charge voltage
	SSD1327__writeCmd(0x08);

	SSD1327__writeCmd(0xd5);		//second precharge period
	SSD1327__writeCmd(0x62);

	SSD1327__writeCmd(0xfd);		//Unlock commands
	SSD1327__writeCmd(0x12);

	SSD1327__writeCmd(0xAF);
	delay(300);
}

void SSD1327__init(){
	gpio_pin_cfg( SOFT_NSS_PORT, SOFT_NSS_PIN, 	gpio_mode_output_PP_FS );
	gpio_pin_cfg( DC_PORT, 		 DC_PIN, 		gpio_mode_output_PP_FS );
	gpio_pin_cfg( RST_PORT,		 RST_PIN, 		gpio_mode_output_PP_FS );

	sw_spi_hw_init( &spiParam );

	SSD1327( CS, DC, RST );

	digitalWrite( _rst, HIGH );		//Reset display
	delay(100);
	digitalWrite( _rst, LOW );
	delay(100);
	digitalWrite( _rst, HIGH );
	delay(100);

	SSD1327__initRegs();
}
#endif
