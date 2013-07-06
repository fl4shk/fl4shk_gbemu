#ifndef include_hpp
#define include_hpp

#include <iostream>
#include <string>
#include <fstream>		// I don't know why this is here
#include <string.h>		// This is only needed for mmu::load_game ()
using namespace std;

#include <SFML/Graphics.hpp>

typedef unsigned char u8; typedef signed char s8;
typedef unsigned short u16; typedef signed short s16;
typedef unsigned long u32; typedef signed long s32;
typedef unsigned int uint;

// This is a thing to enable always_inline with less typing (at least for GCC and its variants)
#ifdef __GNUC__
	#define no_fcall inline __attribute__((always_inline))
#else
	#define no_fcall inline
#endif

typedef sf::Keyboard kinput; typedef sf::Keyboard::Key kb;

class color : public sf::Color
{
public:  // functions
	color () { r = 0; g = 0; b = 0; a = 0; }
	color ( u8 i_r, u8 i_g, u8 i_b, u8 i_a = 255 ) : sf::Color ( i_r, i_g, i_b, i_a ) {}
	void operator = ( const sf::Color tocopy ) 
		{ r = tocopy.r; g = tocopy.g; b = tocopy.b; a = tocopy.a; }
	void set_white () { operator = ( color ( 255, 255, 255 ) ); }
	void set_gray1 () { operator = ( color ( 170, 170, 170 ) ); }
	void set_gray2 () { operator = ( color ( 85, 85, 85 ) ); }
	void set_black () { operator = ( color ( 0, 0, 0 ) ); }
};

template < typename t > void print ( t val ) { cout << val; }
template < typename t > void printhex ( t val ) { cout << hex << val << dec; }
//void println () { cout << endl; }

// (disregard the following debugging stuff; it is being removed)
// Uncomment these to compile debugging stuff into the emulator
//#define cpu_debug
//#define mmu_debug
//#define gpu_debug
//#define apu_debug
//#define timer_debug
//#define int_debug			// interrupt debugging
//#define cgb_mmu_debug		// gbc-specific mmu debuging
//#define cgb_gfx_debug		// gbc-specific gfx debugging
//#define cgb_ir_debug		// gbc infrared communication debugging
//#define update_debug

namespace ioreg
{
	const int joy1 = 0xff00, serial = 0xff01, siocnt = 0xff02, divreg = 0xff04, 
		tmrcount = 0xff05, tmrmod = 0xff06, tmrctrl = 0xff07, intreq = 0xff0f, 
		nr10 = 0xff10, nr11 = 0xff11, nr12 = 0xff12, nr13 = 0xff13, nr14 = 0xff14, 
		nr21 = 0xff16, nr22 = 0xff17, nr23 = 0xff18, nr24 = 0xff19, nr30 = 0xff1a, 
		nr31 = 0xff1b, nr32 = 0xff1c, nr33 = 0xff1d, nr34 = 0xff1e, nr41 = 0xff20,
		nr42 = 0xff21, nr43 = 0xff22, nr44 = 0xff23, nr50 = 0xff24, nr51 = 0xff25, 
		nr52 = 0xff26, wavstart = 0xff30, wavend = 0xff3f, lcdctrl = 0xff40, 
		lcdstat = 0xff41, scrolly = 0xff42, scrollx = 0xff43, lcdcy = 0xff44, 
		lycomp = 0xff45, oamdma = 0xff46, dmgbgp = 0xff47, dmgobjp0 = 0xff48, 
		dmgobjp1 = 0xff49, wndy = 0xff4a, wndx = 0xff4b, intenab = 0xffff;
};

// Union for general-purpose registers (cpugpr looks strange to me, so I went with cpureg)
// The idea here is to reduce the amount of bitwise stuff at the C++ level.
// As a bonus, it also makes the code more readable. (I think...)
union cpureg { u16 w; struct { u8 lo, hi; }; };

enum jump_cndtn { z, nz, c, nc };	// jump condition

#include "bit_funcs.hpp"

#endif // include_hpp
