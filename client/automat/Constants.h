#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace automat
{
	// Arduino keyboard modifiers
	// http://arduino.cc/en/Reference/KeyboardModifiers
	
	const unsigned int L_CTRL   = 0x80;
	const unsigned int L_SHIFT  = 0x81;
	const unsigned int L_ALT    = 0x82;
	const unsigned int L_GUI    = 0x83;

	const unsigned int R_CTRL   = 0x84;
	const unsigned int R_SHIFT  = 0x85;
	const unsigned int R_ALT    = 0x86;
	const unsigned int R_GUI    = 0x87;

	const unsigned int UP       = 0xDA;
	const unsigned int DOWN     = 0xD9;
	const unsigned int LEFT     = 0xD8;
	const unsigned int RIGHT    = 0xD7;

	const unsigned int BACK     = 0xB2;
	const unsigned int TAB      = 0xB3;
	const unsigned int RET      = 0xB0;
	const unsigned int ESC      = 0xB1;
	const unsigned int INS      = 0xD1;
	const unsigned int DEL      = 0xD4;
	const unsigned int PG_UP    = 0xD3;
	const unsigned int PG_DN    = 0xD6;
	const unsigned int HOME     = 0xD2;
	const unsigned int END      = 0xD5;
	const unsigned int CAPS     = 0xC1;

	const unsigned int F1       = 0xC2;
	const unsigned int F2       = 0xC3;
	const unsigned int F3       = 0xC4;
	const unsigned int F4       = 0xC5;
	const unsigned int F5       = 0xC6;
	const unsigned int F6       = 0xC7;
	const unsigned int F7       = 0xC8;
	const unsigned int F8       = 0xC9;
	const unsigned int F9       = 0xCA;
	const unsigned int F10      = 0xCB;
	const unsigned int F11      = 0xCC;
	const unsigned int F12      = 0xCD;
}

#endif // CONSTANTS_H
