/*
 * flash_data.c

 *
 *  Created on: 28 lip 2016
 *      Author: simon
 */

#include "sw_hal_conf.h"


#include "keys.h"
#include "memory_data.h"


#if LOW_MEMORY
const char initString[]			= "Learning enabled";
const char inviteString[]		= "Press key to learn";
const char waitString[]			= "Wait for code: ";
const char gotIrString[]		= "Got code: ";
const char duplicateString[]	= "Err: double code, other";
const char endString[]			= "Learn ended";
const char byeString[]			= "Press any key to stop";
const char leaveString[]		= "Code omited: ";
const char adresString[]		= "Address";
const char logIrString[]		= "IR log enabled";
#else
const char initString[]			= "Wlaczono tryb uczenia";
const char inviteString[]		= "Nacisnij dowolny klawisz pilota by rozpoczac uczenie";
const char waitString[]			= "Czekam na kod: ";
const char gotIrString[]		= "Zarejestrowano kod: ";
const char duplicateString[]	= "Blad: powtorny kod, podaj inny";
const char endString[]			= "Zakonczono tryb uczenia";
const char byeString[]			= "Nacisnij dowolny klawisz pilota by zakonczyc uczenie";
const char leaveString[]		= "Pominieto kod: ";
const char adresString[]		= "Adres?";
const char logIrString[]		= "Tryb logowania IR";
#endif

static const char label01[]	= "Power";
static const char label02[]	= "MenuUp";
static const char label03[]	= "MenuDown";
static const char label04[]	= "MenuLeft";
static const char label05[]	= "MenuRight";
static const char label06[]	= "MenuOK";
static const char label07[]	= "Exit";
static const char label08[]	= "Back";

static const char label09[]	= "Rewind";
static const char label10[]	= "FastForward";
static const char label11[]	= "PreviousTrack";
static const char label12[]	= "NextTrack";
static const char label13[]	= "Play";
static const char label14[]	= "Record";
static const char label15[]	= "Pause";
static const char label16[]	= "Stop";

static const char label17[]	= "ChannelUp";
static const char label18[]	= "ChannelDown";
static const char label19[]	= "Live";
static const char label20[]	= "OSDGuide";
static const char label21[]	= "Recordings";
static const char label22[]	= "Menu";
static const char label23[]	= "Info";

static const char label24[]	= "Fullscreen";
static const char label25[]	= "CodecInfo";
static const char label26[]	= "Radio";
static const char label27[]	= "SelectPlayer";
static const char label28[]	= "Teletext";
static const char label29[]	= "NextSubtitle";
static const char label30[]	= "AudioNextLanguage";

static const char label31[]	= "Red";
static const char label32[]	= "Green";
static const char label33[]	= "Yellow";
static const char label34[]	= "Blue";

static const char label35[]	= "Key1";
static const char label36[]	= "Key2";
static const char label37[]	= "Key3";
static const char label38[]	= "Key4";
static const char label39[]	= "Key5";
static const char label40[]	= "Key6";
static const char label41[]	= "Key7";
static const char label42[]	= "Key8";
static const char label43[]	= "Key9";
static const char label44[]	= "Key0";
static const char label45[]	= "Delete";
static const char label46[]	= "AspectRatio";

const char * const buttonLabels[] = {	// Tablica wskaźników do napisów
	label01,
	label02,
	label03,
	label04,
	label05,
	label06,
	label07,
	label08,
	label09,
	label10,
	label11,
	label12,
	label13,
	label14,
	label15,
	label16,
	label17,
	label18,
	label19,
	label20,
	label21,
	label22,
	label23,
	label24,
	label25,
	label26,
	label27,
	label28,
	label29,
	label30,
	label31,
	label32,
	label33,
	label34,
	label35,
	label36,
	label37,
	label38,
	label39,
	label40,
	label41,
	label42,
	label43,
	label44,
	label45,
	label46
};

#if DEF_SIRC_ADDR
const uint16_t irTabFLASH[TABLENGTH] = {		// Tablica domyślnych kodów IR
/* 1 */		Power,
/* 2 */		Menu_Up,
/* 3 */		Menu_Down,
/* 4 */		Menu_Left,
/* 5 */		Menu_Right,
/* 6 */		Menu_OK,

/* 7 */		Previous_Menu,
/* 8 */		Back,

/* 9 */		Rewind,
/* 10 */	Fast_Forward,
/* 11 */	Previous_Track,
/* 12 */	Next_Track,
/* 13 */	Play,
/* 14 */	Record,
/* 15 */	Pause,
/* 16 */	Stop,

/* 17 */	Channel_Up,
/* 18 */	Channel_Down,
/* 19 */	Live,
/* 20 */	OSD_Guide,
/* 21 */	Recordings,
/* 22 */	Menu,
/* 23 */	Info,

/* 24 */	Fullscreen,
/* 25 */	Codec_info,
/* 26 */	Radio,
/* 27 */	Select_player,
/* 28 */	Teletext,

/* 29 */	Next_subtitle,
/* 30 */	Audio_next_language,

/* 31 */	Red,
/* 32 */	Green,
/* 33 */	Yellow,
/* 34 */	Blue,

/* 35 */	Key1,
/* 36 */	Key2,
/* 37 */	Key3,
/* 38 */	Key4,
/* 39 */	Key5,
/* 40 */	Key6,
/* 41 */	Key7,
/* 42 */	Key8,
/* 43 */	Key9,
/* 44 */	Key0,
/* 45 */	Delete,
/* 46 */	Aspect_ratio
};
#endif

const uint8_t hidTabFLASH[TABLENGTH] = {				// Tablica kodów HID
/* 1 */		HID_KEY_S,
/* 2 */		HID_KEY_UP,
/* 3 */		HID_KEY_DOWN,
/* 4 */		HID_KEY_LEFT,
/* 5 */		HID_KEY_RIGHT,
/* 6 */		HID_KEY_ENTER,

/* 7 */		HID_KEY_ESC,
/* 8 */		HID_KEY_BACKSPACE,

/* 9 */		HID_KEY_R,
/* 10 */	HID_KEY_F,
/* 11 */	HID_KEY_COMMA,
/* 12 */	HID_KEY_DOT,
/* 13 */	HID_KEY_P,
/* 14 */	HID_KEY_W,
/* 15 */	HID_KEY_SPACE,
/* 16 */	HID_KEY_X,

/* 17 */	HID_KEY_PAGEUP,
/* 18 */	HID_KEY_PAGEDOWN,
/* 19 */	HID_KEY_H,
/* 20 */	HID_KEY_G,
/* 21 */	HID_KEY_K,
/* 22 */	HID_KEY_M,
/* 23 */	HID_KEY_I,

/* 24 */	HID_KEY_TAB,
/* 25 */	HID_KEY_O,
/* 26 */	HID_KEY_J,
/* 27 */	HID_KEY_Y,
/* 28 */	HID_KEY_V,

/* 29 */	HID_KEY_T,
/* 30 */	HID_KEY_A,

/* 31 */	HID_KEY_F1,
/* 32 */	HID_KEY_F2,
/* 33 */	HID_KEY_F3,
/* 34 */	HID_KEY_F4,

/* 35 */	HID_KEY_1,
/* 36 */	HID_KEY_2,
/* 37 */	HID_KEY_3,
/* 38 */	HID_KEY_4,
/* 39 */	HID_KEY_5,
/* 40 */	HID_KEY_6,
/* 41 */	HID_KEY_7,
/* 42 */	HID_KEY_8,
/* 43 */	HID_KEY_9,
/* 44 */	HID_KEY_0,
/* 45 */	HID_KEY_DELETE,
/* 46 */	HID_KEY_Z
};

/*********************** Rezerwujemy pamięć w RAM ********************/
static uint16_t irTabRAM[TABLENGTH];
/*********************************************************************/

/**** Struktura w RAM odwołująca się do danych (tablic) we FLASH *****/
T_CODES irManageFLASH = {
	DEF_SIRC_ADDR,
	(uint16_t *)irTabFLASH,	// Wskaźnik do tablicy domyślnych kodów IR
};
/***** Struktura w RAM odwołująca się do danych (tablic) w RAM *******/
T_CODES irManageRAM = {
	0,
	irTabRAM,
};
/*********************************************************************/

