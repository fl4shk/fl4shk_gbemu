#ifndef gbemu_cls_hpp
#define gbemu_cls_hpp

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
		dmgobjp1 = 0xff49, wnd_y = 0xff4a, wnd_x = 0xff4b, intenab = 0xffff;
};

#include "bit_funcs.hpp"
#include "cpu_cls.hpp"
#include "mmu_cls.hpp"
#include "gpu_cls.hpp"

// It should be fairly obvious what this stuff is for
// Though in all honesty, I won't be making use of it 
// until I implement non-DMG stuff
enum gbtype { dmg, sgb, cgb, agb };

// Oh look, ANOTHER use for multiple inheritance
struct gbemu : public cpu, public mmu, public gpu
{
public:  // variables
	static const int divclks = 256, tmr00clks = 1024, tmr01clks = 16, tmr10clks = 64,
		tmr11clks = 256;
	
	int divclksleft, tmrclksleft;
	
	gbtype emumode;
	
public:  // functions
	u8 op_read ( u16 addr ) { return mmu::op_read (addr); }
	u16 op_read_word ( u16 addr ) { return mmu::op_read_word (addr); }
	void op_write ( u16 addr, u8 data ) { mmu::op_write ( addr, data ); }
	
	// The cpu::reset and mmu::reset functions are DMG-based,
	// so we need to do some extra stuff in the gbemu::reset function.
	// This should be easy because most stuff is the same across the board.
	void reset () 
	{
		cpu::reset (); mmu::reset (); gpu::reset ();
		
		// still need to do timer and divider register stuffs
		//divclksleft = divclks; tmrclksleft = 
		
		switch (emumode)
		{
			default:
				break;
		}
	}
	
	u8 get_lcd_mode () { return gpu::get_lcd_mode (); }
	
	void update ();
	void update_timers ( int cycles );
	void do_divreg ( int cycles );
	
	void do_interrupts ();
	void service_int ( int which_int );
	void request_int ( int which_int );
	
};

#include "gbemu_funcs.hpp"

#endif // gbemu_cls_hpp
