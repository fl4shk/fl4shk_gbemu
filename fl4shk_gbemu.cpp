#include <iostream>
#include <string>
#include <fstream>
using namespace std;

typedef unsigned char u8; typedef signed char s8;
typedef unsigned short u16; typedef signed short s16;
typedef unsigned long u32; typedef signed long s32;
typedef unsigned int uint;

template < typename t > void print ( t val ) { cout << val; }
template < typename t > void printhex ( t val ) { cout << hex << val << dec; }
void println () { cout << endl; }

// (To be implemented)
// Uncomment these to compile debugging stuff into the emulator
#define cpu_debug
//#define mmu_debug
//#define gfx_debug
//#define apu_debug
//#define timer_debug
//#define int_debug			// interrupt debugging
//#define cgb_mmu_debug		// gbc-specific mmu debuging
//#define cgb_gfx_debug		// gbc-specific gfx debugging
//#define cgb_ir_debug		// gbc infrared communication debugging


// This is a thing to enable always_inline with less typing (at least for GCC and its variants)
#ifdef __GNUC__
	#define no_fcall inline __attribute__((always_inline))
#else
	#define no_fcall inline
#endif

#include <SFML/Graphics.hpp>

// Oh look, ANOTHER use for multiple inheritance!
class rt_input : public sf::Keyboard, public sf::Joystick, public sf::Mouse {};

#include "gbemu_cls.hpp"

int main ( int argc, char **args )
{
	
	
	return 0;
}
