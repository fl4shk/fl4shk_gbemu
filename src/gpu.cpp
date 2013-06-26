#include "gpu.hpp"

void gpu::reset ()
{
	scanlcounter = scanlclks;
}

void gpu::update_gfx ( int cycles )
{
	if ( test_bit ( op_read (ioreg::lcdctrl), 7 ) )  // if the LCD is enabled
	{
		scanlcounter -= cycles;
		u8 stat = op_read (ioreg::lcdstat), lcdcy = op_read (ioreg::lcdcy);
		
		if ( op_read (ioreg::lycomp) == lcdcy )
			{ set_bit ( stat, 2 ); op_free_write ( ioreg::lcdstat, stat ); }
		else 
			{ clear_bit ( stat, 2 ); op_free_write ( ioreg::lcdstat, stat ); }
		
		if ( test_bit ( stat, 6 ) && test_bit ( stat, 2 ) ) 
			request_int (1);
		
		if ( scanlcounter>375 && scanlcounter<=456 )	// Mode 2
		{
			if ( lcdcy<144 )	// If we are not in VBlank Period
			{
				stat &= 0xfc; stat |= 0x02; op_free_write ( ioreg::lcdstat, stat );
				
				if ( test_bit ( stat, 5 ) )		// If the Mode 2 Interrupt is enabled
					request_int (1);			// Request an LCD Status Interrupt
			}
		}
		else if ( scanlcounter>205 && scanlcounter<=375 )	// Mode 3
		{
			if ( lcdcy<144 )	// If we are not in VBlank Period
			{
				stat |= 0x03; op_free_write ( ioreg::lcdstat, stat );
			}
		}
		else if ( scanlcounter>0 && scanlcounter<=205 )	// Mode 0
		{
			if ( lcdcy<144 )	// If we are not in VBlank Period
			{
				stat &= 0xfc; op_free_write ( ioreg::lcdstat, stat );
				
				if ( test_bit ( stat, 3 ) )		// If the Mode 0 Interrupt is enabled
					request_int (1);			// Request an LCD Status Interrupt
			}
		}
		else if ( scanlcounter<=0 )
		{
			gpu::reset ();
			
			if ( lcdcy<144 )	// If we are not in VBlank Period
			{
				draw_scanline ();
				
				++lcdcy; op_free_write ( ioreg::lcdcy, lcdcy );
				
				if ( lcdcy==144 )		// If we have just made it to VBlank Period
					request_int (0);	// Request a VBlank Interrupt
			}
			else if ( lcdcy<153 )	// If we are in VBlank Period
			{
				++lcdcy; op_free_write ( ioreg::lcdcy, lcdcy );
				
				stat &= 0xfc; stat |= 0x01; op_free_write ( ioreg::lcdstat, stat );
				
				
			}
			else	// We have finished with VBlank Period
			{
				op_free_write ( ioreg::lcdcy, 0 );
			}
		}
		
		// old code (doesn't compile currently)
		// (it also isn't very easy to read IMO)
		/*u8 lcdmode = get_lcd_mode ();
		
		switch (lcdmode)
		{
			case 0x00:  // H-Blank Period
				if ( test_bit ( stat, 3 ) ) request_int (1);
				m0clksleft -= cycles;
				if ( m0clksleft<=0 )
				{
					// op_free_write is needed so that we don't reset lcdcy
					lcdcy++; op_free_write ( ioreg::lcdcy, lcdcy );
					if ( lcdcy==144 )
					{
						// We are now in VBlank period, so we set LCD mode to 1
						stat &= 0xfc; stat |= 0x01; 
						op_write ( ioreg::lcdstat, stat ); 
						request_int (0);  // We should only request VBint once
					}
					else
					{
						draw_scanline ();
						gpu::reset ();  // shortcut thing
					}
				}
				break;
			case 0x01:  // V-Blank Period
				if ( test_bit ( stat, 4 ) ) request_int (1);
				m1clksleft -= cycles;
				if ( m1clksleft<=0 )
				{
					gpu::reset ();
				}
				if ( scanlcounter<=0 )
				{
					scanlcounter = scanlclks; 
					u8 lcdcy = op_read (ioreg::lcdcy); lcdcy++;
					op_free_write ( ioreg::lcdcy, lcdcy );
				}
				break;
			case 0x02:  // Reading from OAM
				if ( test_bit ( stat, 5 ) ) request_int (1);
				m2clksleft -= cycles;
				if ( m2clksleft<=0 )
				{
					stat |= 0x03; op_write ( ioreg::lcdstat, stat );
				}
				break;
			case 0x03:  // Reading from OAM and VRAM
				m3clksleft -= cycles;
				if ( m3clksleft<=0 )
				{
					stat &= 0xfc; op_write ( ioreg::lcdstat, stat );
				}
				break;
			default:  // I REALLY doubt this will show up
				cout << "Woah, I don't think THIS should have happened.\n";
				break;
		}*/
	}
	else  // if the LCD is disabled
	{
		u8 stat = op_read (ioreg::lcdstat);
		clear_bit ( stat, 1 ); set_bit ( stat, 0 );
		op_free_write ( ioreg::lcdstat, stat );
		if ( test_bit ( stat, 4 ) )
			request_int (1);
		gpu::reset ();
		op_free_write ( ioreg::lcdcy, 0 );
	}
}

u8 gpu::get_lcd_mode ()
{
	return op_read (ioreg::lcdstat)&0x03;
}

void gpu::render_screen ()
{
	
}

void gpu::draw_scanline ()
{
	
}
