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

// Oh look, ANOTHER use for multiple inheritance
struct gbemu : public cpu, public mmu, public gpu
{
public:  // variables
	static const int divclks = 256, tmr00clks = 1024, tmr01clks = 16, tmr10clks = 64,
		tmr11clks = 256;
	
	int divclksleft, tmrclksleft;
	
	gbtype emumode;
	
public:  // functions
	gbemu ( const char *filename )
	{
		reset ();
		load_game (filename);
		run_game ();
	}
	
	void run_game ()
	{
		sf::RenderWindow app ( sf::VideoMode ( 160, 144 ), "FL4SHK GBemu" );
		app.setFramerateLimit (60); app.clear ( color::White );
		
		sf::Texture txtr; sf::Sprite spr;
		txtr.create ( 160, 144 ); txtr.setSmooth (false); 
		txtr.update ((u8 *)screen); spr.setTexture (txtr);
		
		for ( int i=0; i<num_pixels; ++i )
			screen [i] = color ( 255, 120, 0 );
		
		while ( app.isOpen () )
		{
			sf::Event event;
			
			while ( app.pollEvent (event) )
			{
				if ( event.type == sf::Event::Closed )
					app.close ();
			}
			
			if ( kinput::isKeyPressed (kb::Escape ) )
				app.close ();
			
			update ();
			
			txtr.update ((u8 *)screen);
			app.draw (spr);
			
			app.display ();
		}
	}
	
	u8 op_read ( u16 addr ) { return mmu::op_read (addr); }
	u16 op_read_word ( u16 addr ) { return mmu::op_read_word (addr); }
	void op_write ( u16 addr, u8 data ) { mmu::op_write ( addr, data ); }
	void op_free_write ( u16 addr, u8 data ) { mmu::op_free_write ( addr, data ); }
	
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

//#include "gbemu_funcs.hpp"

#endif // gbemu_hpp
