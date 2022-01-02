#ifndef _LIB_KEYS_H
#define _LIB_KEYS_H 1

// Definiujemy nazwy dla kodów HID
#define	HID_KEY_S			0x16	// Power			1
#define	HID_KEY_UP			0x52	// Menu_Up			2
#define	HID_KEY_DOWN		0x51	// Menu_Down		3
#define	HID_KEY_LEFT		0x50	// Menu_Left		4
#define	HID_KEY_RIGHT		0x4F	// Menu_Right		5
#define	HID_KEY_ENTER		0x28	// Menu_OK			6

#define	HID_KEY_ESC			0x29	// Exit				7
#define	HID_KEY_BACKSPACE	0x2A	// Back				8

#define	HID_KEY_R			0x15	// Rewind			9
#define	HID_KEY_F			0x09	// Fast_Forward		10
#define	HID_KEY_COMMA		0x36	// Previous_Track	11
#define	HID_KEY_DOT			0x37	// Next_Track		12
#define	HID_KEY_P			0x13	// Play				13
#define	HID_KEY_W			0x1A	// Record			14
#define	HID_KEY_SPACE		0x2C	// Pause			15
#define	HID_KEY_X			0x1B	// Stop				16

#define	HID_KEY_PAGEUP		0x4B	// Channel_Up		17
#define	HID_KEY_PAGEDOWN	0x4E	// Channel_Down		18

#define	HID_KEY_H			0x0B	// Live				19
#define	HID_KEY_G			0x0A	// OSD_Guide		20
#define	HID_KEY_K			0x0E	// Recordings		21
#define	HID_KEY_M			0x10	// Menu				22
#define	HID_KEY_I			0x0C	// Info				23
#define	HID_KEY_TAB			0x2B	// Fullscreen		24
#define	HID_KEY_O			0x12	// Codec_info		25

#define	HID_KEY_F1			0x3A	// Red				26
#define	HID_KEY_F2			0x3B	// Green			27
#define	HID_KEY_F3			0x3C	// Yellow			28
#define	HID_KEY_F4			0x3D	// Blue				29

#define	HID_KEY_1			0x1E	// Key1				30
#define	HID_KEY_2			0x1F	// Key2				31
#define	HID_KEY_3			0x20	// Key3				32
#define	HID_KEY_4			0x21	// Key4				33
#define	HID_KEY_5			0x22	// Key5				34
#define	HID_KEY_6			0x23	// Key6				35
#define	HID_KEY_7			0x24	// Key7				36
#define	HID_KEY_8			0x25	// Key8				37
#define	HID_KEY_9			0x26	// Key9				38
#define	HID_KEY_0			0x27	// Key0				39
#define	HID_KEY_V			0x19	// Teletext			40
#define	HID_KEY_T			0x17	// Next_subtitle	41
#define	HID_KEY_A			0x04	// Audio_next		42
#define	HID_KEY_DELETE		0x4C	// Delete			43
#define	HID_KEY_Z			0x1D	// Aspect_ratio		44
#define	HID_KEY_J			0x0D	// Radio			45
#define	HID_KEY_Y			0x1C	// Select_player	46

// Definiujemy nazwy dla kodów IR (SIRC)
#define	Power				0x2d15	// KEY_S			1
#define	Menu_Up				0x2d5f	// KEY_UP			2
#define	Menu_Down			0x2d60	// KEY_DOWN			3
#define	Menu_Left			0x2d62	// KEY_LEFT			4
#define	Menu_Right			0x2d61	// KEY_RIGHT		5
#define	Menu_OK				0x2d5e	// KEY_ENTER		6
#define	Previous_Menu		0x2d43	// KEY_ESC			7
#define	Back				0x2d55	// KEY_BACKSPACE	8
#define	Rewind				0x2d1b	// KEY_R			9
#define	Fast_Forward		0x2d1c	// KEY_F			10
#define	Previous_Track		0x2d57	// KEY_COMMA		11
#define	Next_Track			0x2d56	// KEY_DOT			13
#define	Play				0x2d1a	// KEY_P			12
#define	Record				0x2d1d	// KEY_W			14
#define	Pause				0x2d19	// KEY_SPACE		15
#define	Stop				0x2d18	// KEY_X			16
#define	Channel_Up			0x2d10	// KEY_PAGEUP		17
#define	Channel_Down		0x2d11	// KEY_PAGEDOWN		18
#define	Live				0x2d50	// KEY_H			19
#define	OSD_Guide			0x2d2a	// KEY_G			20
#define	Recordings			0x2d2b	// KEY_K			21
#define	Menu				0x2d42	// KEY_M			22
#define	Info				0x2d44	// KEY_I			23
#define	Fullscreen			0x2d6b	// KEY_TAB			24
#define	Codec_info			0x2d13	// KEY_O			25
#define	Red					0x2d78	// KEY_F1			26
#define	Green				0x2d64	// KEY_F2			27
#define	Yellow				0x2d7a	// KEY_F3			28
#define	Blue				0x2d7b	// KEY_F4			29
#define	Key1				0x2d00	// KEY_1			30
#define	Key2				0x2d01	// KEY_2			31
#define	Key3				0x2d02	// KEY_3			32
#define	Key4				0x2d03	// KEY_4			33
#define	Key5				0x2d04	// KEY_5			34
#define	Key6				0x2d05	// KEY_6			35
#define	Key7				0x2d06	// KEY_7			36
#define	Key8				0x2d07	// KEY_8			37
#define	Key9				0x2d08	// KEY_9			38
#define	Key0				0x2d09	// KEY_0			39
#define	Teletext			0x2d7c	// KEY_V			40
#define	Next_subtitle		0x2d68	// KEY_T			41
#define	Audio_next_language 0x2d17	// KEY_A			42
#define	Delete				0x2d79	// KEY_DELETE		43
#define	Aspect_ratio		0x2d76	// KEY_Z			44
#define	Radio				0x2d3f	// KEY_J			45
#define	Select_player		0x2d1f	// KEY_Y			46

#endif //_LIB_KEYS_H

