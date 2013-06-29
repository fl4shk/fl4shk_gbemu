#include "mmu.hpp"

u8 mmu::op_read ( u16 addr )
{
	return gbram [addr];	// temporary stuff until I feel like being accurate
}

u16 mmu::op_read_word ( u16 addr )
{
	cpureg ret; 
	ret.lo = op_read (addr); ret.hi = op_read (addr+1);
	return ret.w;
}

void mmu::op_write ( u16 addr, u8 data )
{
	
	// Not going to be handling banking just yet
	if ( addr<0x8000 )
	{
		//handle_banking ();
	}
	
	// TO --NOT-- DO Again:  Confuse addr<0xa000 with working with external RAM 
	// (AWFUL bug!)
	
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
	
	
	// TO DO:  implement tests to see if cartridge RAM is enabled
	// until I implement external memory handling, this area is restricted
	else if ( addr<0xc000 )
	{
		
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
	
	else if ( addr==0x0000 ) {}
	
	else if ( addr==ioreg::oamdma )
	{
		u16 dma_addr = data<<8;  // source addres is data*100
		for ( int i=0; i<0xa0; ++i )
		{
			op_write ( 0xfe00+i, op_read (dma_addr+i) );
		}
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
   gbram [0xFF05] = 0x00;	// TIMA
   gbram [0xFF06] = 0x00;	// TMA
   gbram [0xFF07] = 0x00;	// TAC
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
	in = fopen ( filename, "rb" );	// This only works for 32 KB ROMs such as Tetris
	fread ( gbram, 1, gbram_size, in );
	fclose (in);
}
