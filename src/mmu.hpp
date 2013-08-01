#ifndef mmu_hpp
#define mmu_hpp

#include "include.hpp"

enum mbc_type 
	{ none, mbc1, mbc2, mbc3, mbc5 };	// Don't really want to emulate other types
	// I also don't want to emulate MBC2, it's strange IMO

struct mmu
{
public:  // variables
	static const int gbram_size = 0x10000;
	u8 gbram [gbram_size]; 
	
	mbc_type rom_mbc; 
	int romsize_index, ramsize_index; 
	bool hasram, hasbatt, hasrtc;	// has ram, has battery, has mbc3 rtc
	static constexpr int rom_sizes [] = { 0x8000, 0x10000, 0x20000, 0x40000, 0x80000, 
		0x100000, 0x200000, 0x400000 }, ram_sizes [] = { 0x00, 0x800, 0x2000, 0x8000 };
	u8 cart_rom [ rom_sizes [0x07] ], cart_ram [ ram_sizes [0x03] ];
	
	struct { u8 rom_bank, ram_bank; bool enable_ram, romram_mode; } mbc1stuff;
	struct { } mbc2stuff;
	struct { } mbc3stuff;
	struct { } mbc5stuff;
	
public:  // functions
	virtual u8 op_read ( u16 addr );
	virtual u16 op_read_word ( u16 addr );
	virtual void op_write ( u16 addr, u8 data );
	virtual void op_free_write ( u16 addr, u8 data );
	
	virtual u8 get_lcd_mode () = 0;
	virtual u16 get_pc () = 0;
	
	virtual void pr () = 0;
	
	virtual u8 get_input_state () = 0;
	
	virtual void reset ();
	
	void handle_banking ( u16 addr, u8 data );
	void handle_cartram ( u16 addr, u8 data );
	
	void load_game ( const char *filename );
};

//#include "mmu_funcs.hpp"

#endif // mmu_hpp
