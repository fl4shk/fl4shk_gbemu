#include "gbemu.hpp"

void gbemu::update ()
{
	int cycles_this_update = 0;
	
	while ( cycles_this_update<max_cycles )
	{
		int cycles;
		
		if (!halted) cycles = exec ();
		else cycles = 4;
		
		if ( pending_clear_ime && op_read (pc-1) != 0xF3)
		{
			pending_clear_ime = false; ime = false;
		}
		if ( pending_set_ime && op_read (pc-1) != 0xFB )
		{
			pending_set_ime = false; ime = true;
		}
		
		cycles_this_update += cycles;
		update_timers (cycles);
		update_gfx (cycles);
		do_interrupts ();
	}
	render_screen ();
}

void gbemu::update_timers ( int cycles )
{
	
}

void gbemu::do_divreg ( int cycles )
{
	
}

void gbemu::do_interrupts ()
{
	if ( ime==true && gbram [ioreg::intreq]>0 )
	{
		for ( int i=0; i<5; ++i )
		{
			if ( test_bit ( gbram [ioreg::intreq], i ) 
				&& test_bit ( gbram [ioreg::intenab], i ) )
				service_int (i);
		}		
	}
}

void gbemu::service_int ( int which_int )
{
	ime = false;
	
	clear_bit ( gbram [ioreg::intreq], which_int );
	
	op_push_word (pc);
	
	switch (which_int)
	{
		case 0:  pc = 0x40; break;
		case 1:  pc = 0x48; break;
		case 2:  pc = 0x50; break;
		case 4:  pc = 0x60; break;
	}
	
}

void gbemu::request_int ( int which_int )
{
	u8 req = op_read (ioreg::intreq);
	set_bit ( req, which_int );
	op_write ( ioreg::intreq, req );
}
