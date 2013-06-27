#ifndef mmu_hpp
#define mmu_hpp

#include "include.hpp"

struct mmu
{
public:  // variables
	static const int gbram_size = 0x10000;
	u8 gbram [gbram_size];
	
public:  // functions
	virtual u8 op_read ( u16 addr );
	virtual u16 op_read_word ( u16 addr );
	virtual void op_write ( u16 addr, u8 data );
	virtual void op_free_write ( u16 addr, u8 data );
	
	virtual u8 get_lcd_mode () = 0;
	virtual u16 get_pc () = 0;
	
	virtual void reset ();
	
	void handle_banking ();
	
	void load_game ( const char *filename );
};

//#include "mmu_funcs.hpp"

#endif // mmu_hpp
