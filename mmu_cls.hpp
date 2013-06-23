#ifndef mmu_cls_hpp
#define mmu_cls_hpp

struct mmu
{
public:  // variables
	static const int gbram_size = 0x10000;
	u8 gbram [gbram_size];
	
public:  // functions
	virtual no_fcall u8 op_read ( u16 addr );
	virtual no_fcall u16 op_read_word ( u16 addr );
	virtual void op_write ( u16 addr, u8 data );
	virtual no_fcall void op_free_write ( u16 addr, u8 data );
	
	virtual u8 get_lcd_mode () = 0;
	
	virtual void reset ();
	
	
};

#include "mmu_funcs.hpp"

#endif // mmu_cls_hpp
