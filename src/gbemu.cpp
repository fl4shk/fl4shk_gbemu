#include "gbemu.hpp"

constexpr int gbemu::tmrclks [];

void gbemu::update ()
{
	int cycles_this_update = 0;
	
	while ( cycles_this_update<max_cycles )
	{
		int cycles;
		
		cycles = exec ();
		
		//cout << "pending_clear_ime:  " << pending_clear_ime 
			//<< " pending_set_ime:  " << pending_set_ime << "\n";
		
		if ( pending_clear_ime /*&& op_read (pc-1) != 0xf3*/ )
		{	
			pending_clear_ime = false; ime = false;
		}
		if ( pending_set_ime /*&& op_read (pc-1) != 0xfb*/ )
		{
			pending_set_ime = false; ime = true;
		}
		
		cycles_this_update += cycles;
		update_timers (cycles);
		update_gfx (cycles);
		do_interrupts ();
		
		//#ifdef update_debug
		//cout << "cycles_this_update:  " << cycles_this_update << "\n";
		//#endif // update_debug
		
	}
	//render_screen ();  // it was a dummy function anyway
}

void gbemu::run_game ()
{
	// We don't need app to exist outside of run_game ()
	sf::RenderWindow app ( sf::VideoMode ( 160, 144 ), "FL4SHK GBemu" );
	app.setFramerateLimit (60); app.clear ( color::White );
	
	sf::Texture txtr; sf::Sprite spr;
	txtr.create ( hres, vres ); txtr.setSmooth (false); 
	spr.setTexture (txtr);
	
	for ( int y=0; y<vres; ++y )
	{
		for ( int x=0; x<hres; ++x )
			screen [y*hres+x] = color ( (u8)y*hres+x, 0, 0 );
	}
	
	while ( app.isOpen () )
	{
		sf::Event event;
		
		while ( app.pollEvent (event) )
		{
			if ( event.type == sf::Event::Closed )
				app.close ();
			
			else if ( event.type == sf::Event::KeyPressed )
			{
				if ( event.key.code == kb::A )
				{
					for ( u16 i=0x8000; i<0xa000; i+=2 )
					{
						cout << hex << "addr 0x" << i << ":  0x"
							<< (int)op_read (i) << "  ";
						cout << hex << "addr 0x" << i+1 << ":  0x"
							<< (int)op_read (i+1) << endl;
					}
					
					sf::Clock tempclk;
					while ( tempclk.getElapsedTime ().asSeconds ()<1 ) {}
				}
				else if ( event.key.code == kb::Escape )
					app.close ();
			}
		}
		
		update ();
		
		txtr.update ((u8 *)screen);
		
		app.draw (spr);
		
		app.display ();
	}
}

void gbemu::update_timers ( int cycles )
{
	divclksleft -= cycles;
	
	if ( divclksleft<=0 ) { divclksleft = divclks; ++gbram [ioreg::divreg]; }
	
	if ( test_bit ( gbram [ioreg::tmrctrl], 2 ) )
	{
		tmrclksleft -= cycles;
		
		if ( tmrclksleft<=0 )
		{
			if ( gbram [ioreg::tmrcount]==0xff )
			{
				gbram [ioreg::tmrcount] = gbram [ioreg::tmrmod];
				request_int (2);
			}
			else
				++gbram [ioreg::tmrcount];
			
			tmrclksleft = tmrclks [ gbram [ioreg::tmrctrl]&0x03 ];
		}
	}
}

void gbemu::do_interrupts ()
{
	//#ifdef int_debug //disabling this
	//cout << "We are in the do_interrupts () function.\n";
	//cout << "Interrupt Master Enable:  " << ime << ".\n";
	//cout << "gbram [ioreg::intreq]:  " 
		//<< hex << (int)gbram [ioreg::intreq] << dec << ".\n";
	//#endif //int_debug
	
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
	#ifdef int_debug
	cout << "We are servicing interrupt " << which_int << ".\n";
	#endif //int_debug
	
	if (halted) { halted = false; ++pc; }
	
	ime = false;
	
	clear_bit ( gbram [ioreg::intreq], which_int );
	
	op_push_word (pc);
	
	switch (which_int)
	{
		case 0:  pc = 0x40; break;
		case 1:  pc = 0x48; break;
		case 2:  pc = 0x50; break;
		//case 3:  pc = 0x58; break;  // not doing that stuff yet
		case 4:  pc = 0x60; break;
	}
	
}

void gbemu::request_int ( int which_int )
{
	//cout << "Requesting interrupt " << which_int << endl;
	
	u8 req = op_read (ioreg::intreq);
	set_bit ( req, which_int );
	op_write ( ioreg::intreq, req );
}
