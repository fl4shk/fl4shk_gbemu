#ifndef gpu_hpp
#define gpu_hpp

#include "include.hpp"

struct gpu
{
public:  // variables
	static const int scanlclks = 456;
	
	int scanlcounter;
	
	//The following are typical when the display is enabled:
	//Mode 2  2_____2_____2_____2_____2_____2___________________2____
	//Mode 3  _33____33____33____33____33____33__________________3___
	//Mode 0  ___000___000___000___000___000___000________________000
	//Mode 1  ____________________________________11111111111111_____
	
	
public:  // functions
	virtual u8 op_read ( u16 addr ) = 0;
	virtual u16 op_read_word ( u16 addr ) = 0;
	virtual void op_write ( u16 addr, u8 data ) = 0;
	virtual void op_free_write ( u16 addr, u8 data ) = 0;
	virtual void request_int ( int which_int ) = 0;
	
	virtual void reset ();
	
	void update_gfx ( int cycles );
	virtual u8 get_lcd_mode (); 
	void render_screen ();
	void draw_scanline ();
	
};

#endif // gpu_hpp
