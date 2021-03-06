#include "mmu.hpp"

constexpr int mmu::rom_sizes [], mmu::ram_sizes [];

u8 mmu::op_read ( u16 addr )
{
	if ( addr>=0x4000 && addr<0x8000 )
	{
		switch (rom_mbc)
		{
			case none:
				return gbram [addr];
			case mbc1:
				static int temp_bank;
				switch ( mbc1stuff.rom_bank )
				{
					case 0x00:
						// temp_bank = 0x01;
						return cart_rom [addr];
					case 0x20:
						temp_bank = 0x21;
						return cart_rom [( addr-0x4000 )+( temp_bank*0x4000 )];
					case 0x40:
						temp_bank = 0x41;
						return cart_rom [( addr-0x4000 )+( temp_bank*0x4000 )];
					case 0x60:
						temp_bank = 0x61;
						return cart_rom [( addr-0x4000 )+( temp_bank*0x4000 )];
					default:
						return cart_rom [( addr-0x4000 )+( mbc1stuff.rom_bank*0x4000 )];
				}
			case mbc2:
				return cart_rom [addr];		// temporary stuff
			case mbc3:
				return cart_rom [addr];		// temporary stuff
			case mbc5:
				return cart_rom [addr];		// temporary stuff
		}
	}
	
	else if ( addr>=0xa000 && addr<0xc000 )
	{
		if ( hasram && ramsize_index!=0x00 )
		{
			switch (rom_mbc)
			{
				case none:
					return 0xff;
				case mbc1:
					if (mbc1stuff.enable_ram)
						return cart_ram [( addr-0xc000 )+( mbc1stuff.ram_bank*0x2000 )];
					else
						return 0xff;
				case mbc2:
					break;
				case mbc3:
					break;
				case mbc5:
					break;
			}
		}
		else
			return 0xff;
	}
	
	else if ( addr==ioreg::joyp )
	{
		//printf ( "R JOYP-PC A 0x%x\n", get_pc () );
		return get_input_state ();
	}
	
	else
		return gbram [addr];
	
	return 0x45;	// arbitrary value so compiler doesn't complain
}

u16 mmu::op_read_word ( u16 addr )
{
	cpureg ret; 
	ret.lo = op_read (addr); ret.hi = op_read (addr+1);
	return ret.w;
}

void mmu::op_write ( u16 addr, u8 data )
{
	if ( addr<0x8000 )
	{
		handle_banking ( addr, data );
	}
	
	else if ( addr<0xa000 )		// if we are doing stuff to VRAM
	{
		//cout << "Attempting to write to VRAM.... ";
		
		//if ( addr==0x9800 )
		//{
			//cout << "Writing 0x" << hex << (int)data << " from PC address 0x"
				//<< get_pc () << endl << dec;
		//}
		
		u8 lcdmode = get_lcd_mode ();
		if ( lcdmode==0 || lcdmode==1 || lcdmode==2 )
		{
			gbram [addr] = data;
			
			#ifdef gpu_debug
		
			cout << "Writing 0x" << hex << (int)data << " at address 0x"
				<< addr << dec << ".\n";
			
			#endif // gpu_debug
		}
		
		// if the LCD is disabled
		if ( !test_bit ( op_read (ioreg::lcdctrl), 7 ) )
		{
			gbram [addr] = data;
			
			#ifdef gpu_debug
		
			cout << "Writing 0x" << hex << (int)data << " at address 0x"
				<< addr << dec << ".\n";
			
			#endif // gpu_debug
			
		}
	}
	
	else if ( addr<0xc000 )
	{
		handle_cartram ( addr, data );
	}
	
	else if ( addr>=0xfe00 && addr<0xfea0 )		// if we are doing stuff to OAM
	{
		u8 lcdmode = get_lcd_mode ();
		if ( lcdmode==0 || lcdmode==1 )
			gbram [addr] = data;
		
		// if the LCD is disabled
		if ( !test_bit ( op_read (ioreg::lcdctrl), 7 ) )
			gbram [addr] = data;
	}
	
	else if ( addr>=0xe000 && addr<0xfe00 )
	{
		gbram [addr] = data; gbram [addr-0x2000] = data;
	}
	
	// This area is not usable
	else if ( addr>=0xfea0 && addr<=0xfeff ) {}
	
	else if ( addr==ioreg::divreg )
		gbram [ioreg::divreg] = 0x00;
	
	else if ( addr==ioreg::lcdcy )
		gbram [ioreg::lcdcy] = 0x00;
	
	//else if ( addr==0x0000 ) {}
	
	else if ( addr==ioreg::oamdma )
	{
		u16 dma_addr = data<<8;  // source addres is data*100
		for ( int i=0; i<0xa0; ++i )
		{
			op_write ( 0xfe00+i, op_read (dma_addr+i) );
		}
	}
	
	else if ( addr==ioreg::intreq )
	{
		//gbram [data] = addr|0xe0;		// I'll have none of this
		gbram [addr] = data|0xe0;
	}
	
	else if ( addr==ioreg::joyp )
	{
		gbram [ioreg::joyp] = 0xcf; 
		gbram [ioreg::joyp] |= ( data&0x30 );
	}
	
	// We don't need control over this area, so we write directly
	else
		gbram [addr] = data;
	
}

void mmu::op_free_write ( u16 addr, u8 data )
{
	gbram [addr] = data;
}

void mmu::reset ()
{
   gbram [0xFF00] = 0xCF;	// JOYP
   gbram [0xFF05] = 0x00;	// TIMA
   gbram [0xFF06] = 0x00;	// TMA
   gbram [0xFF07] = 0x00;	// TAC
   gbram [0xFF0F] = 0xE1;	// IF
   gbram [0xFF10] = 0x80;	// NR10
   gbram [0xFF11] = 0xBF;	// NR11
   gbram [0xFF12] = 0xF3;	// NR12
   gbram [0xFF14] = 0xBF;	// NR14
   gbram [0xFF16] = 0x3F;	// NR21
   gbram [0xFF17] = 0x00;	// NR22
   gbram [0xFF19] = 0xBF;	// NR24
   gbram [0xFF1A] = 0x7F;	// NR30
   gbram [0xFF1B] = 0xFF;	// NR31
   gbram [0xFF1C] = 0x9F;	// NR32
   gbram [0xFF1E] = 0xBF;	// NR33
   gbram [0xFF20] = 0xFF;	// NR41
   gbram [0xFF21] = 0x00;	// NR42
   gbram [0xFF22] = 0x00;	// NR43
   gbram [0xFF23] = 0xBF;	// NR30
   gbram [0xFF24] = 0x77;	// NR50
   gbram [0xFF25] = 0xF3;	// NR51
   gbram [0xFF26] = 0xF1;	// NR52
   gbram [0xFF40] = 0x91;	// LCDC
   gbram [0xFF41] = 0x02;	// LCD_STAT
   gbram [0xFF42] = 0x00;	// SCY
   gbram [0xFF43] = 0x00;	// SCX
   gbram [0xFF45] = 0x00;	// LYC
   gbram [0xFF47] = 0xFC;	// BGP
   gbram [0xFF48] = 0xFF;	// OBP0
   gbram [0xFF49] = 0xFF;	// OBP1
   gbram [0xFF4A] = 0x00;	// WY
   gbram [0xFF4B] = 0x00;	// WX
   gbram [0xFFFF] = 0x00;	// IE
}

void mmu::load_game ( const char *filename )
{
	memset ( gbram, 0, gbram_size );
	
	FILE *in;
	in = fopen ( filename, "rb" );
	fread ( gbram, 1, rom_sizes [0x00], in );
	fclose (in);
	
	romsize_index = gbram [0x0148]; ramsize_index = gbram [0x0149];
	hasrtc = false;
	
	in = fopen ( filename, "rb" );
	fread ( cart_rom, 1, rom_sizes [romsize_index], in );
	fclose (in);
	
	// not handling save data battery stuff just yet
	
	// set the MBC stuff
	switch (gbram [0x0147])
	{
		case 0x00:
			rom_mbc = none; hasram = false; hasbatt = false;
			break;
		case 0x01:
			rom_mbc = mbc1; hasram = false; hasbatt = false;
			mbc1stuff = { 0x01, 0x00, 0, 0 };
			break;
		case 0x02:
			rom_mbc = mbc1; hasram = true; hasbatt = false;
			mbc1stuff = { 0x01, 0x00, 0, 0 };
			break;
		case 0x03:
			rom_mbc = mbc1; hasram = true; hasbatt = true;
			mbc1stuff = { 0x01, 0x00, 0, 0 };
			break;
		case 0x05:		// MBC2 has built-in RAM
			rom_mbc = mbc2; hasram = false; hasbatt = false;
			break;
		case 0x06:		// MBC2 has built-in RAM
			rom_mbc = mbc2; hasram = false; hasbatt = true;
			break;
		case 0x0f:
			rom_mbc = mbc3; hasram = false; hasbatt = true; hasrtc = true;
			break;
		case 0x10:
			rom_mbc = mbc3; hasram = true; hasbatt = true; hasrtc = true;
			break;
		case 0x11:
			rom_mbc = mbc3; hasram = false; hasbatt = false;
			break;
		case 0x12:
			rom_mbc = mbc3; hasram = true; hasbatt = false;
			break;
		case 0x13:
			rom_mbc = mbc3; hasram = true; hasbatt = true;
			break;
		case 0x19:
			rom_mbc = mbc5; hasram = false; hasbatt = false;
			break;
		case 0x1a:
			rom_mbc = mbc5; hasram = true; hasbatt = false;
			break;
		case 0x1b:
			rom_mbc = mbc5; hasram = true; hasbatt = true;
			break;
		case 0x1c:
			rom_mbc = mbc5; hasram = false; hasbatt = false;
			break;
		case 0x1d:
			rom_mbc = mbc5; hasram = true; hasbatt = false;
			break;
		case 0x1e:
			rom_mbc = mbc5; hasram = true; hasbatt = true;
			break;
	}
	
}

void mmu::handle_banking ( u16 addr, u8 data )
{
	switch (rom_mbc)
	{
		case none:
			break;
		case mbc1:
			if ( addr<0x2000 )	// RAM Enable Stuff
			{
				if ( ( data&0x0f )==0x0a ) mbc1stuff.enable_ram = 1;
				else mbc1stuff.enable_ram = 0;
			}
			else if ( addr<0x4000 )	// ROM Bank Number (Lower 5 Bits)
			{
				mbc1stuff.rom_bank &= 0x60; mbc1stuff.rom_bank |= ( data&0x1f );
			}
			else if ( addr<0x6000 )	// RAM Bank Number or Upper 2 Bits of ROM Bank Number
			{
				if (!mbc1stuff.romram_mode)
				{
					mbc1stuff.rom_bank &= 0x1f; 
					mbc1stuff.rom_bank |= ( ( data&0x03 )<<0x05 );
				}
				else // if (mbc1stuff.romram_mode)
					mbc1stuff.ram_bank = data&0x03;
			}
			else if ( addr<0x8000 )	// ROM/RAM Mode Select
			{
				if ( data==0x00 ) mbc1stuff.romram_mode = 0;
				else if ( data==0x01 ) mbc1stuff.romram_mode = 1;
			}
			break;
		case mbc2:
			break;
		case mbc3:
			break;
		case mbc5:
			break;
	}
}

void mmu::handle_cartram ( u16 addr, u8 data )
{
	switch (rom_mbc)
	{
		case none:
			break;
		case mbc1:
			if (mbc1stuff.enable_ram)
				cart_ram [( addr-0xc000 )+( mbc1stuff.ram_bank*0x2000 )] = data;
			break;
		case mbc2:
			break;
		case mbc3:
			break;
		case mbc5:
			break;
	}
}
