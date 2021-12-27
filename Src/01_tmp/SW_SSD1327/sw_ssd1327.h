/*
 * sw_ssd1327.h
 *
 *  Created on: 29 paź 2021
 *      Author: simon
 */

#ifndef LIBS_SW_GRAPHICS_SW_SSD1327_SW_SSD1327_H_
#define LIBS_SW_GRAPHICS_SW_SSD1327_SW_SSD1327_H_

//#include <Arduino.h>
//#include "stdint.h"
//#include "SPI.h"

// Scroll rate constants. See datasheet page 40.
#define SSD1327_SCROLL_2   0b111
#define SSD1327_SCROLL_3   0b100
#define SSD1327_SCROLL_4   0b101
#define SSD1327_SCROLL_5   0b110
#define SSD1327_SCROLL_6   0b000
#define SSD1327_SCROLL_32  0b001
#define SSD1327_SCROLL_64  0b010
#define SSD1327_SCROLL_256 0b011

//class SSD1327 {
//	public:
//		void SSD1327(int cs, int dc, int rst);
//		void SSD1327__writeCmd(uint8_t reg);
//		void SSD1327__writeData(uint8_t data);
//		void SSD1327__setWriteZone(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
//		uint16_t SSD1327__coordsToAddress(uint8_t x, uint8_t y);
//		void SSD1327__setPixelChanged(uint8_t x, uint8_t y, bool changed);
//		void SSD1327__drawPixel(uint8_t x, uint8_t y, uint8_t color, bool display);
//		void SSD1327__drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color, bool display = false);
//		void SSD1327__drawHLine(int x, int y, int length, uint8_t color, bool display = false);
//		void SSD1327__drawVLine(int x, int y, int length, uint8_t color, bool display = false);
//		void SSD1327__drawLine(int x1, int y1, int x2, int y2, uint8_t color, bool display = false);
//		void SSD1327__drawByteAsRow(uint8_t x, uint8_t y, uint8_t byte, uint8_t color);
//		void SSD1327__drawChar(uint8_t x, uint8_t y, char thisChar, uint8_t color);
//		void SSD1327__drawChar16(uint8_t x, uint8_t y, char thisChar, uint8_t color);
//		void SSD1327__drawChar32(uint8_t x, uint8_t y, char thisChar, uint8_t color);
//		void SSD1327__drawCharArray(uint8_t x, uint8_t y, char text[], uint8_t color, int size=8);
//		void SSD1327__drawString(uint8_t x, uint8_t y, String textString, uint8_t color, int size=8);
//		void SSD1327__setupScrolling(uint8_t startRow, uint8_t endRow, uint8_t startCol, uint8_t endCol, uint8_t scrollSpeed, bool right);
//		void SSD1327__startScrolling();
//		void SSD1327__stopScrolling();
//		void SSD1327__scrollStep(uint8_t startRow, uint8_t endRow, uint8_t startCol, uint8_t endCol, bool right);
//		void SSD1327__fillStripes(uint8_t offset);
//		void SSD1327__clearBuffer();
//		void SSD1327__writeFullBuffer();
//		void SSD1327__writeUpdates();
//		void SSD1327__setContrast(uint8_t contrast);
////		void initRegs();
//		void SSD1327__init();
//	private:
//		uint8_t frameBuffer[8192];   // Should mirror the display's own frameBuffer.
//		uint8_t changedPixels[1024]; // Each bit of this array represets whether a given byte of frameBuffer (e.g. a pair of pixels) is not up to date.
//		int _cs;
//		int _dc;
//		int _rst;
//};

#endif /* LIBS_SW_GRAPHICS_SW_SSD1327_SW_SSD1327_H_ */
