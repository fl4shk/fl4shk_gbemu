#ifndef gpu_hpp
#define gpu_hpp

#include "include.hpp"

enum col_slot { white, gray1, gray2, black };
struct gpu
{
public:  // variables
	static const int scanlclks = 456;
	
	int scanlcounter;
	
	static const int hres = 160, vres = 144;
	static const int num_pixels = hres*vres; color screen [num_pixels];
	static const int tiles_per_row = 20;
	
	
public:  // functions
	virtual u8 op_read ( u16 addr ) = 0;
	virtual u16 op_read_word ( u16 addr ) = 0;
	virtual void op_write ( u16 addr, u8 data ) = 0;
	virtual void op_free_write ( u16 addr, u8 data ) = 0;
	virtual void request_int ( int which_int ) = 0;
	
	virtual void pr () = 0;
	
	virtual void reset ();
	
	void update_gfx ( int cycles );
	virtual u8 get_lcd_mode (); 
	no_fcall void draw_scanline ();
	no_fcall void render_bg (); no_fcall void render_obj ();
	
	no_fcall col_slot get_color ( u8 color_num, u16 addr );
};

#endif // gpu_hpp
