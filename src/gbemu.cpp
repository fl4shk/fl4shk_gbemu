#include "gbemu.hpp"

constexpr int gbemu::tmrclks [];

void gbemu::update ()
{
	int cycles_this_update = 0;
	
	while ( cycles_this_update<max_cycles )
	{
		//printf ( "if:  %x  ie:  %x\n", gbram [ioreg::intreq], gbram [ioreg::intenab] );
		//pr ();
		
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
		
		//if ( ime && test_bit ( gbram [ioreg::intreq], 4 ) 
			//&& test_bit ( gbram [ioreg::intenab], 4 ) )
			//update_joyp ();
		//else if (halted)
			//update_joyp ();
		
	}
	//render_screen ();  // it was a dummy function anyway
}

void gbemu::run_game ()
{
	// We don't need app to exist outside of run_game ()
	sf::RenderWindow app ( sf::VideoMode ( 160, 144 ), "FL4SHK GBemu" );
	app.setFramerateLimit (60); 
	app.clear ( color::White );
	
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
				if ( event.key.code == kb::Escape )
					app.close ();
				else if ( event.key.code == kb::Z )
				{
					#ifdef gpu_debug
					for ( u16 i=0x8000; i<0xa000; i+=2 )
					{
						cout << hex << "addr 0x" << i << ":  0x"
							<< (int)op_read (i) << "  ";
						cout << hex << "addr 0x" << i+1 << ":  0x"
							<< (int)op_read (i+1) << endl;
					}
					
					//sf::Clock tempclk;
					//while ( tempclk.getElapsedTime ().asSeconds ()<1.0f );
					
					
					
					#endif // gpu_debug
				}
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
	
	//printf ( "divclksleft:  %i  tmrclksleft:  %i  cycles:  %i\n", 
		//divclksleft, tmrclksleft, cycles );
	
	if ( divclksleft<=0 ) { divclksleft = divclks; ++gbram [ioreg::divreg]; }
	
	if ( test_bit ( gbram [ioreg::tmrctrl], 2 ) )
	{
		tmrclksleft -= cycles;
		
		if ( tmrclksleft<=0 )
		{
			if ( gbram [ioreg::tmrcount]==0xff )
			{
				#ifdef timer_debug
				//printf ("tmrcount==0xff\n");
				
				printf ( "intreq:  %x\n", gbram [ioreg::intreq] );
				#endif // timer_debug
				
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
	
	//if ( which_int==4 )
		//printf ("We are servicing the Joypad Interrupt");
	
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
	
	//if ( which_int == 4 )
		//printf ( "Requesting interrupt %i\n", which_int );
	
	u8 req = op_read (ioreg::intreq);
	set_bit ( req, which_int );
	op_write ( ioreg::intreq, req );
}

void gbemu::update_joyp ()
{
	//printf ("ASDF\n");
	
	joyp_state = 0xff;
	
	if ( keydown (kb::Return) ) keypressed (7);
	else keyreleased (7);
	if ( keydown (kb::A) ) keypressed (6);
	else keyreleased (6);
	if ( keydown (kb::J) ) keypressed (5);
	else keyreleased (5);
	if ( keydown (kb::K) ) keypressed (4);
	else keyreleased (4);
	if ( keydown (kb::D) ) keypressed (3);
	else keyreleased (3);
	if ( keydown (kb::E) ) keypressed (2);
	else keyreleased (2);
	if ( keydown (kb::S) ) keypressed (1);
	else keyreleased (1);
	if ( keydown (kb::F) ) keypressed (0);
	else keyreleased (0);
}

void gbemu::keypressed ( int key )
{
	//printf ( "Key %i has been pressed.\n", key );
	
	bool previously_unset = false, button = true, req_int = false;
	
	if ( test_bit ( joyp_state, key )==0 )
		previously_unset = true;
	
	clear_bit ( joyp_state, key );
	
	if ( key<=3 )
		button = false;
	
	u8 key_req = gbram [ioreg::joyp];
	
	if ( button && !test_bit ( key_req, 5 ) )
		req_int = true;
	
	else if ( !button && test_bit ( key_req, 4 ) )
		req_int = true;
	
	if ( req_int && !previously_unset )
		request_int (4);
}

void gbemu::keyreleased ( int key )
{
	set_bit ( joyp_state, key );	// remember, 1 means the key is unpressed
}

u8 gbemu::get_input_state ()
{
	update_joyp ();
	
	u8 ret = gbram [ioreg::joyp];
	ret ^= 0xff;
	
	// buttons
	if ( !test_bit ( ret, 4 ) )
	{
		u8 top_joyp = joyp_state >> 4;
		top_joyp |= 0xf0;
		ret &= top_joyp;
	}
	// dpad
	else if ( !test_bit ( ret, 5 ) )
	{
		u8 bottom_joyp = joyp_state & 0xf;
		bottom_joyp |= 0xf0;
		ret &= bottom_joyp;
	}
	
	return ret;
}
