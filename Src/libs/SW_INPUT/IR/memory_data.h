/*
 * flash_data.h
 *
 *  Created on: 28 lip 2016
 *      Author: simon
 */

#ifndef LIB_MEMORY_DATA_H_
#define LIB_MEMORY_DATA_H_


#define DEF_SIRC_ADDR	0x0813							// Domyślny adres kodów pilota
#define TABLENGTH 		46								// Dlugośc tablic z kodami

// Deklarujemy strukturę do obsługi kodów IR i HID
typedef struct {
	uint16_t	irAddress;								// Zmienna adresu pilota
	uint16_t	*irCodesTAB;							// Tablica kodów IR
} T_CODES;

#if LOW_MEMORY
extern const char inviteString[19];
extern const char initString[17];
extern const char waitString[16];
extern const char gotIrString[11];
extern const char duplicateString[24];
extern const char endString[12];
extern const char byeString[22];
extern const char leaveString[14];
extern const char adresString[8];
extern const char logIrString[15];
#else
extern const char inviteString[53];
extern const char initString[22];
extern const char waitString[16];
extern const char gotIrString[21];
extern const char duplicateString[32];
extern const char endString[26];
extern const char byeString[52];
extern const char leaveString[16];
extern const char adresString[7];
extern const char logIrString[18];
#endif

extern T_CODES irManageFLASH;					// Struktura do obsługi FLASH
extern T_CODES irManageRAM;						// Struktura do obsługi RAM

extern const char * const  buttonLabels[];	// Tablica wskaźników we FLASH


#endif /* LIB_MEMORY_DATA_H_ */
