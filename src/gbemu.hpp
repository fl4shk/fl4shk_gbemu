#ifndef gbemu_hpp
#define gbemu_hpp

#include "include.hpp"
#include "cpu.hpp"
#include "mmu.hpp"
#include "gpu.hpp"

// It should be fairly obvious what this stuff is for
// Though in all honesty, I won't be making use of it 
// until I implement non-DMG stuff
enum gbtype { dmg, sgb, cgb, agb };

// Oh look, a use for multiple inheritance
struct gbemu : public cpu, public mmu, public gpu
{
public:  // variables
	static const int divclks = 256; 
	static constexpr int tmrclks [] = { 1024, 16, 64, 256 };
	
	int divclksleft, tmrclksleft;
	//, clear_ime_clks, set_ime_clks	// going to do this at some point
	
	gbtype emumode;
	
	u8 joyp_state;
	
public:  // functions
	gbemu ( const char *filename )	// I probably ought to add command line arg stuff
	{
		load_game (filename);
		reset ();
		run_game ();
	}
	
	void run_game ();
	
	u8 op_read ( u16 addr ) { return mmu::op_read (addr); }
	u16 op_read_word ( u16 addr ) { return mmu::op_read_word (addr); }
	void op_write ( u16 addr, u8 data ) { mmu::op_write ( addr, data ); }
	void op_free_write ( u16 addr, u8 data ) { mmu::op_free_write ( addr, data ); }
	
	u16 get_pc () { return pc; }
	void pr () { cpu::pr (); }
	
	// The cpu::reset and mmu::reset functions are DMG-based,
	// so we need to do some extra stuff in the gbemu::reset function.
	// This should be easy because most stuff is the same across the board.
	void reset () 
	{
		cpu::reset (); mmu::reset (); gpu::reset ();
		
		divclksleft = divclks; tmrclksleft = tmrclks [0];
		
		switch (emumode)
		{
			default:
				break;
		}
	}
	
	u8 get_lcd_mode () { return gpu::get_lcd_mode (); }
	
	no_fcall void update ();
	no_fcall void update_timers ( int cycles );
	no_fcall void do_divreg ( int cycles );
	
	no_fcall void update_joyp ();
	no_fcall void keypressed ( int key );
	no_fcall void keyreleased ( int key );
	no_fcall u8 get_input_state ();
	
	void do_interrupts ();
	void service_int ( int which_int );
	void request_int ( int which_int );
	
};

//#include "gbemu_funcs.hpp"

#endif // gbemu_hpp
