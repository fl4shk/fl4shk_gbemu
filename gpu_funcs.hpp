#ifndef gpu_funcs_hpp
#define gpu_funcs_hpp

void gpu::reset ()
{
	m2clksleft = mode2clks; m3clksleft = mode3clks; 
	m0clksleft = mode0clks; m1clksleft = mode1clks;
	srclksleft = screenrclks;
	scanlcounter = scanlclks;
}

void gpu::update_gfx ( int cycles )
{
	if ( test_bit ( op_read (ioreg::lcdctrl), 7 ) )  // if the LCD is enabled
	{
		scanlcounter -= cycles;
		u8 stat = op_read (ioreg::lcdstat), lcdcy = op_read (ioreg::lcdcy);
		
		if ( op_read (ioreg::lycomp) == lcdcy )
			{ set_bit ( stat, 2 ); op_write ( ioreg::lcdstat, stat ); }
		else 
			{ clear_bit ( stat, 2 ); op_write ( ioreg::lcdstat, stat ); }
		
		if ( test_bit ( stat, 6 ) && test_bit ( stat, 2 ) ) 
			request_int (1);
		
		u8 lcdmode = get_lcd_mode ();
		
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
		}	
	}
	else  // if the LCD is disabled
	{
		u8 stat = op_read (ioreg::lcdstat);
		clear_bit ( stat, 1 ); set_bit ( stat, 0 );
		op_write ( ioreg::lcdstat, stat );
		if ( test_bit ( stat, 4 ) )
			request_int (1);
		gpu::reset ();
		op_free_write ( ioreg::lcdcy, 0 );
	}
	//gpu::reset ();
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

#endif // gpu_funcs_hpp
