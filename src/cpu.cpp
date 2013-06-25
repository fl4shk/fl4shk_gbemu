#include "cpu.hpp"

void cpu::set_zflag () { set_bit ( af.lo, zflag_slot ); }
void cpu::clear_zflag () { clear_bit ( af.lo, zflag_slot ); }
bool cpu::get_zflag () const { return test_bit ( af.lo, zflag_slot ); }

void cpu::set_nflag () { set_bit ( af.lo, nflag_slot ); }
void cpu::clear_nflag () { clear_bit ( af.lo, nflag_slot ); }
bool cpu::get_nflag () const { return test_bit ( af.lo, nflag_slot ); }

void cpu::set_hflag () { set_bit ( af.lo, hflag_slot ); }
void cpu::clear_hflag () { clear_bit ( af.lo, hflag_slot ); }
bool cpu::get_hflag () const { return test_bit ( af.lo, hflag_slot ); }

void cpu::set_cflag () { set_bit ( af.lo, cflag_slot ); }
void cpu::clear_cflag () { clear_bit ( af.lo, cflag_slot ); }
bool cpu::get_cflag () const { return test_bit ( af.lo, cflag_slot ); }

// There are SO MANY dern load commands, geez (or at least load functions I made...)
void cpu::op_load_r_r ( u8 &r1, u8 r2 )
{
	pc++; r1 = r2; 
}

void cpu::op_load_r_n ( u8 &r )
{
	pc++;
	u8 n = op_read (pc); pc++; r = n;
}

void cpu::op_load_r_hl_mem ( u8 &r )
{
	pc++; r = op_read (hl.w);
}

void cpu::op_load_hl_mem_r ( u8 r )
{
	pc++; op_write ( hl.w, r ); 
}

void cpu::op_load_hl_mem_n ()
{
	pc++; u8 n = op_read (pc); pc++;
	op_write ( hl.w, n );
}

void cpu::op_load_a_rr_mem ( u16 rr )
{
	pc++; af.hi = op_read (rr); 
}

void cpu::op_load_rr_mem_a ( u16 rr )
{
	pc++; op_write ( rr, af.hi );
}

void cpu::op_load_a_mem_imm ()
{
	pc++; cpureg temp; 
	temp.lo = op_read (pc); pc++;
	temp.hi = op_read (pc); pc++;
	af.hi = op_read (temp.w); 
}

void cpu::op_load_mem_imm_a ()
{
	pc++; cpureg temp; 
	temp.lo = op_read (pc); pc++;
	temp.hi = op_read (pc); pc++;
	op_write ( temp.w, af.hi );
}

void cpu::op_load_a_io_c ()
{
	pc++; af.hi = op_read ( 0xFF00+(u16)bc.lo );
}

void cpu::op_load_io_c_a ()
{
	pc++; op_write ( 0xFF00+(u16)bc.lo, af.hi );
}

void cpu::op_load_a_io_n ()
{
	pc++; u8 n = op_read (pc); pc++;
	af.hi = op_read ( 0xFF00+(u16)n );
}

void cpu::op_load_io_n_a ()
{
	pc++; u8 n = op_read (pc); pc++;
	op_write ( 0xFF00+(u16)n, af.hi );
}

void cpu::op_load_a_hli ()
{
	pc++; af.hi = op_read (hl.w); hl.w++;
}

void cpu::op_load_hli_a ()
{
	pc++; op_write ( hl.w, af.hi ); hl.w++;
}

void cpu::op_load_a_hld ()
{
	pc++; af.hi = op_read (hl.w); hl.w--;
}

void cpu::op_load_hld_a ()
{
	pc++; op_write ( hl.w, af.hi ); hl.w--;
}

void cpu::op_load_rr_nn ( u16 &rr )
{
	pc++; cpureg nn;
	nn.lo = op_read (pc); pc++;
	nn.hi = op_read (pc); pc++;
	//rr = op_read_word (nn.w);
	rr = nn.w;
}

void cpu::op_load_sp_hl ()
{
	pc++; sp = hl.w;
}

void cpu::op_load_hl_sp_n ()
{
	pc++; s8 n = op_read (pc); pc++;
	hl.w = sp+(s16)n;
	
	clear_zflag (); clear_nflag ();
	
	u16 htest = ( sp&0xf );
	htest += ( n&0xf );
	
	if ( htest>0xf ) set_hflag ();
	
	if ( ( sp+n )>0xff ) set_cflag ();
}

void cpu::op_load_mem_imm_sp ()
{
	pc++; u16 nn = op_read_word (pc);
	pc += 2;
	cpureg temp;
	temp.w = sp;
	op_write ( nn, temp.lo ); nn++;
	op_write ( nn, temp.hi );
}

// stack commands
void cpu::op_push_rr ( cpureg rr )		// This is actually an instruction
{
	pc++; op_push_word (rr);
}

void cpu::op_pop_rr ( cpureg &rr )		// This is actually an instruction
{
	pc++; op_pop_word (rr);
}

void cpu::op_push_word ( cpureg rr )		// This is not an instruction
{
	--sp; op_write ( sp, rr.hi );
	--sp; op_write ( sp, rr.lo );
}

void cpu::op_pop_word ( cpureg &rr )		// This is not an instruction
{
	if ( sp<0xfffe )
	{
		rr.lo = op_read (sp); ++sp;
		rr.hi = op_read (sp); ++sp;
	}
	else
	{
		// error, not sure what to do...
	}
}

void cpu::op_push_word ( u16 rr )		// This is not an instruction
{
	cpureg temp; temp.w = rr;
	op_push_word (temp);
}

void cpu::op_pop_word ( u16 &rr )		// This is not an instruction
{
	cpureg temp; op_pop_word (temp);
	rr = temp.w;
}

// arithmetic commands

void cpu::op_add_r ( u8 r )
{
	pc++; 
	
	if ( ( ( af.hi&0x0f )+( r&0x0f ) )>0xf ) set_hflag ();
	else clear_hflag ();
	
	if ( ( af.hi+r )>0xff ) set_cflag ();
	else clear_cflag ();
	
	af.hi = ( ( af.hi+r )&0xff );	
	
	if ( af.hi == 0 ) set_zflag ();
	else clear_zflag ();
	
	clear_nflag ();
}

void cpu::op_add_n ()
{
	pc++; 
	u8 n = op_read (pc);
	
	op_add_r (n);
	
	//pc++;
	//
	//if ( ( ( af.hi&0x0f )+( n&0x0f ) )>0x0f ) set_hflag ();
	//else clear_hflag ();
	//
	//if ( ( af.hi+n )>0xff ) set_cflag ();
	//else clear_cflag ();
	//
	//af.hi = ( ( af.hi+n )&0xff );
	//
	//if ( af.hi == 0 ) set_zflag ();
	//else clear_zflag ();
	//
	//clear_nflag ();
}

void cpu::op_add_hl_mem ()
{
	//pc++; 
	u8 n = op_read (hl.w);
	
	op_add_r (n);
	
	//if ( ( ( af.hi&0x0f )+( n&0x0f ) )>0x0f ) set_hflag ();
	//else clear_hflag ();
	//
	//if ( ( af.hi+n )>0xff ) set_cflag ();
	//else clear_cflag ();
	//
	//af.hi = ( ( af.hi+n )&0xff );
	//
	//if ( af.hi == 0 ) set_zflag ();
	//else clear_zflag ();
	//
	//clear_nflag ();
}

void cpu::op_add_hl_rr ( u16 rr )
{
	clear_nflag ();
	
	static uint temp_hl_val = (uint)hl.w, val_to_add = (uint)rr;
	
	if ( ( temp_hl_val&0xfff )+( val_to_add&0xfff )>4095 ) set_hflag ();
	else clear_hflag ();
	if ( ( temp_hl_val+val_to_add ) > 65535 ) set_cflag (); else clear_cflag ();
	temp_hl_val = ( ( temp_hl_val+val_to_add )&0xffff );
	hl.w = (u16)temp_hl_val;
}

void cpu::op_adc_r ( u8 r )
{
	pc++; 
	
	int temp, flag_c;
	if ( get_cflag ()==0 ) flag_c = 0;
	else flag_c = 1;
	
	temp = af.hi+r+flag_c;
	
	clear_nflag ();
	
	if ( temp>0xff ) set_cflag ();
	else clear_cflag ();
	
	if ( ( ( af.hi&0x0f )+( r&0x0f )+flag_c )>0x0f ) set_hflag ();
	else clear_hflag ();
	
	af.hi = ( temp&0xff );
	
	if ( af.hi == 0 ) set_zflag ();
	else clear_zflag ();
	
}

void cpu::op_adc_n ()
{
	pc++; 
	u8 n = op_read (pc);
	
	op_adc_r (n);
	
	//pc++;
	//
	//int temp, flag_c;
	//if ( get_cflag ()==0 ) flag_c = 0;
	//else flag_c = 1;
	//
	//temp = af.hi+n+flag_c;
	//
	//clear_nflag ();
	//
	//if ( temp>0xff ) set_cflag ();
	//else clear_cflag ();
	//
	//if ( ( ( af.hi&0x0f )+( n&0x0f )+flag_c )>0x0f ) set_hflag ();
	//else clear_hflag ();
	//
	//af.hi = ( temp&0xff );
	//
	//if ( af.hi == 0 ) set_zflag ();
	//else clear_zflag ();
}

void cpu::op_adc_hl_mem ()
{
	//pc++; 
	u8 n = op_read (hl.w);
	
	op_adc_r (n);
	
	//int temp, flag_c;
	//if ( get_cflag ()==0 ) flag_c = 0;
	//else flag_c = 1;
	//
	//
	//
	//temp = af.hi+n+flag_c;
	//
	//clear_nflag ();
	//
	//if ( temp>0xff ) set_cflag ();
	//else clear_cflag ();
	//
	//if ( ( ( af.hi&0x0f )+( n&0x0f )+flag_c )>0x0f ) set_hflag ();
	//else clear_hflag ();
	//
	//af.hi = ( temp&0xff );
	//
	//if ( af.hi == 0 ) set_zflag ();
	//else clear_zflag ();
}

void cpu::op_sub_r ( u8 r )
{
	pc++; 
	set_nflag ();
	
	if ( af.hi==r ) set_zflag ();
	else clear_zflag ();
	
	if ( ( af.hi&0x0f )<( r&0x0f ) ) set_hflag ();
	else clear_hflag ();
	
	if ( af.hi<r ) { af.hi = ( 256 - ( r-af.hi ) ); set_cflag (); }
	else { af.hi -= r; clear_cflag (); }
}

void cpu::op_sub_n ()
{
	pc++; 
	u8 n = op_read (pc);
	
	op_sub_r (n);
	
	//pc++;
	//
	//set_nflag ();
	//
	//if ( af.hi==n ) set_zflag ();
	//else clear_zflag ();
	//
	//if ( ( af.hi&0x0f )<( n&0x0f ) ) set_hflag ();
	//else clear_hflag ();
	//
	//if ( af.hi<n ) { af.hi = ( 256 - ( n-af.hi ) ); set_cflag (); }
	//else { af.hi -= n; clear_cflag (); }
}

void cpu::op_sub_hl_mem ()
{
	//pc++; 
	u8 n = op_read (hl.w);
	
	op_sub_r (n);
}

void cpu::op_sbc_r ( u8 r )
{
	pc++; 
	
	static signed int int_buff, flag_c;
	if ( get_cflag ()!=0 ) flag_c = 1; else flag_c = 0;
	int_buff = af.hi - ( r+flag_c );
	
	if ( int_buff<0 ) { set_cflag (); int_buff += 256; }
	else clear_cflag ();
	
	if ( ( ( af.hi&0x0f ) - ( r&0x0f ) - flag_c )<0 ) set_hflag ();
	else clear_hflag ();
	
	af.hi = int_buff;
	set_nflag ();
	if ( af.hi == 0 ) set_zflag ();
	else clear_zflag ();
	
}

void cpu::op_sbc_n ()
{
	pc++; 
	u8 n = op_read (pc);
	
	op_sbc_r (n);
	
	//pc++; ...
	
}

void cpu::op_sbc_hl_mem ()
{
	//pc++; 
	
	u8 n = op_read (hl.w);
	
	op_sbc_r (n);
	
}

void cpu::op_inc_r ( u8 &r )
{
	pc++; 
	
	clear_nflag ();
	if ( r==255 )
	{
		r = 0; set_zflag ();
	}
	else
	{
		r++; clear_zflag ();
	}
	
	if ( ( r&0x0f ) == 0 )
		set_hflag ();
	else
		clear_hflag ();
	
}

void cpu::op_inc_hl_mem ()
{
	u8 temp = op_read (hl.w);
	op_inc_r (temp);
	op_write ( hl.w, temp );
}

void cpu::op_inc_rr ( u16 &rr )
{
	pc++; rr++;
}

void cpu::op_dec_r ( u8 &r )
{
	pc++; 
	
	static u8 old_r = r;
	set_nflag ();
	
	if ( r==0 )
		r = 255;
	else
		r--;
	
	if ( ( old_r&0x0f )<( r&0x0f ) )
		set_hflag ();
	else
		clear_hflag ();
	
	if ( r==0 )
		set_zflag ();
	else
		clear_zflag ();
	
}

void cpu::op_dec_hl_mem ()  // next
{
	u8 temp = op_read (hl.w);
	op_dec_r (temp);
	op_write ( hl.w, temp );
	
}

void cpu::op_dec_rr ( u16 &rr )
{
	pc++; rr++;
}

void cpu::op_add_sp_dd ()
{
	pc++; u8 to_add = op_read (pc); pc++;
	static u16 result = sp + to_add; 
	
	clear_zflag (); clear_nflag ();
	if ( ( result&0xff )<( sp&0xff ) ) set_cflag ();
	else clear_cflag ();
	if ( ( result&0xf )<( sp&0xf ) ) set_hflag ();
	else clear_hflag ();
	sp = result;
}


// logic/other math commands

void cpu::op_and_r ( u8 r )
{
	pc++; af.hi &= r; 
	if ( af.hi == 0 ) set_zflag (); else clear_zflag ();
	clear_nflag (); set_hflag (); clear_cflag ();
}

void cpu::op_and_n ()
{
	pc++; af.hi &= op_read (pc); pc++;
	if ( af.hi == 0 ) set_zflag (); else clear_zflag ();
	clear_nflag (); set_hflag (); clear_cflag ();
}

void cpu::op_and_hl_mem ()
{
	pc++; af.hi &= op_read (hl.w);
	if ( af.hi == 0 ) set_zflag (); else clear_zflag ();
	clear_nflag (); set_hflag (); clear_cflag ();
}

void cpu::op_or_r ( u8 r )
{
	pc++; af.hi |= r;
	if ( af.hi == 0 ) set_zflag (); else clear_zflag ();
	clear_nflag (); clear_hflag (); clear_cflag ();
}

void cpu::op_or_n ()
{
	pc++; af.hi |= op_read (pc); pc++;
	if ( af.hi == 0 ) set_zflag (); else clear_zflag ();
	clear_nflag (); clear_hflag (); clear_cflag ();
}

void cpu::op_or_hl_mem ()
{
	pc++; af.hi |= op_read (hl.w);
	if ( af.hi == 0 ) set_zflag (); else clear_zflag ();
	clear_nflag (); clear_hflag (); clear_cflag ();
}

void cpu::op_xor_r ( u8 r )
{
	pc++; af.hi ^= r;
	if ( af.hi == 0 ) set_zflag (); else clear_zflag ();
	clear_nflag (); clear_hflag (); clear_cflag ();
}

void cpu::op_xor_n ()
{
	pc++; af.hi ^= op_read (pc); pc++;
	if ( af.hi == 0 ) set_zflag (); else clear_zflag ();
	clear_nflag (); clear_hflag (); clear_cflag ();
}

void cpu::op_xor_hl_mem ()
{
	pc++; af.hi ^= op_read (hl.w);
	if ( af.hi == 0 ) set_zflag (); else clear_zflag ();
	clear_nflag (); clear_hflag (); clear_cflag ();
}

void cpu::op_daa ()
{
	pc++; int tempa = af.hi;
	
	if ( get_nflag ()==0 )
	{
		if ( ( get_hflag ()!=0 )||( ( tempa&0xf )>0x9 ) ) tempa += 0x06;
		if ( ( get_cflag ()!=0 )||( tempa>0x9f ) ) tempa += 0x60;
	}
	else
	{
		if ( get_hflag ()!=0 ) tempa = ( ( tempa-6 )&0xff );
		if ( get_cflag ()!=0 ) tempa -= 0x60;
	}
	
	clear_hflag ();
	
	if ( ( tempa&0x100 )==0x100 ) set_cflag ();
	
	tempa &= 0xff;
	
	if ( tempa==0x00 ) set_zflag ();
	else clear_zflag ();
	
	af.hi = tempa;
}

void cpu::op_cpl ()
{
	pc++; af.hi ^= 0xff;
	set_nflag (); set_hflag ();
}

void cpu::op_cp_r ( u8 r )
{
	//pc++;
	u8 old_a = af.hi;
	op_sub_r (r);
	af.hi = old_a;
}

void cpu::op_cp_n ()
{
	pc++;
	u8 old_a = af.hi, n = op_read (pc);
	op_sub_r (n);
	af.hi = old_a;
	
	//pc++; ...
}

void cpu::op_cp_hl_mem ()
{
	//pc++;
	u8 old_a = af.hi, n = op_read (hl.w);
	op_sub_r (n);
	af.hi = old_a;
}



//void cpu::

// rotate and shift commands

void cpu::op_rlca ()
{
	pc++;
	
	clear_zflag (); clear_nflag (); clear_hflag ();
	
	if ( get_bit ( af.hi, 7 ) ) set_cflag ();
	else clear_cflag ();
	
	af.hi = ( af.hi<<1 )+get_cflag ();
}

void cpu::op_rla ()
{
	pc++;
	
	clear_zflag (); clear_nflag (); clear_hflag ();
	
	bool ocf = get_cflag ();
	
	if ( get_bit ( af.hi, 7 ) ) set_cflag ();
	else clear_cflag ();
	
	af.hi = ( af.hi<<1 )+ocf;
}

void cpu::op_rrca ()
{
	pc++;
	
	clear_zflag (); clear_nflag (); clear_hflag ();
	
	if ( get_bit ( af.hi, 0 ) ) set_cflag ();
	else clear_cflag ();
	
	af.hi = ( af.hi>>1 ) + ( get_cflag ()<<7 );
}

void cpu::op_rra ()
{
	pc++;
	
	clear_zflag (); clear_nflag (); clear_hflag ();
	
	bool ocf = get_cflag ();
	
	if ( get_bit ( af.hi, 0 ) ) set_cflag ();
	else clear_cflag ();
	
	af.hi = ( af.hi>>1 ) + ( ocf<<7 );	
}

void cpu::op_rlc_r ( u8 &r )
{
	pc++;
	
	clear_nflag (); clear_hflag ();
	
	if ( get_bit ( r, 7 ) ) set_cflag ();
	else clear_cflag ();
	
	r = ( r<<1 ) + get_cflag ();
	
	if ( r==0 ) set_zflag ();
	else clear_zflag ();
}

void cpu::op_rlc_hl_mem ()
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_rlc_r (temp);
	op_write ( hl.w, temp );
}

void cpu::op_rl_r ( u8 &r )
{
	pc++;
	
	clear_nflag (); clear_hflag ();
	
	bool ocf = get_cflag ();
	
	if ( get_bit ( r, 7 ) ) set_cflag ();
	else clear_cflag ();
	
	r = ( r<<1 ) + ocf;
	
	if ( r==0 ) set_zflag ();
	else clear_zflag ();
}

void cpu::op_rl_hl_mem ()
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_rl_r (temp);
	op_write ( hl.w, temp );
	
}

void cpu::op_rrc_r ( u8 &r )
{
	pc++;
	
	clear_nflag (); clear_hflag ();
	
	if ( get_bit ( r, 0 ) ) set_cflag ();
	else clear_cflag ();
	
	r = ( r>>1 ) + ( get_cflag ()<<7 );
	
	if ( r==0 ) set_zflag ();
	else clear_zflag ();
}

void cpu::op_rrc_hl_mem ()
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_rrc_r (temp);
	op_write ( hl.w, temp );
}

void cpu::op_rr_r ( u8 &r )
{
	pc++;
	
	clear_nflag (); clear_hflag ();
	
	bool ocf = get_cflag ();
	
	if ( get_bit ( r, 0 ) ) set_cflag ();
	else clear_cflag ();
	
	r = ( r>>1 ) + ( ocf<<7 );
	
	if ( r==0 ) set_zflag ();
	else clear_zflag ();
}

void cpu::op_rr_hl_mem ()
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_rr_r (temp);
	op_write ( hl.w, temp );
}

void cpu::op_sla_r ( u8 &r )
{
	pc++;
	
	clear_nflag (); clear_hflag ();
	
	if ( get_bit ( r, 7 ) ) set_cflag ();
	else clear_cflag ();
	
	r <<= 1;
	
	if ( r==0 ) set_zflag ();
	else clear_zflag ();
}

void cpu::op_sla_hl_mem ()
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_sla_r (temp);
	op_write ( hl.w, temp );
}

void cpu::op_sra_r ( u8 &r )
{
	pc++;
	
	clear_nflag (); clear_hflag ();
	
	if ( get_bit ( r, 0 ) ) set_cflag ();
	else clear_cflag ();
	
	r >>= 1;
	
	if ( get_bit ( r, 6 ) ) set_bit ( r, 7 );
	else clear_bit ( r, 7 );
	
	if ( r==0 ) set_zflag ();
	else clear_zflag ();
}

void cpu::op_sra_hl_mem ()
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_sra_r (temp);
	op_write ( hl.w, temp );
}

void cpu::op_srl_r ( u8 &r )
{
	pc++;
	
	clear_nflag (); clear_hflag ();
	
	if ( get_bit ( r, 0 ) ) set_cflag ();
	else clear_cflag ();
	
	r >>= 1;
	
	if ( r==0 ) set_zflag ();
	else clear_zflag ();
}

void cpu::op_srl_hl_mem ()
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_srl_r (temp);
	op_write ( hl.w, temp );
}

void cpu::op_swap_r ( u8 &r )
{
	pc++;
	
	clear_nflag (); clear_hflag (); clear_cflag ();	
	
	u8 hi = ( r&0xf0 ) >> 4, lo = ( r&0x0f );
	
	r = ( lo<<4 ) + hi;
	
	if ( r==0 ) set_zflag ();
	else clear_zflag ();	
}

void cpu::op_swap_hl_mem ()
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_swap_r (temp);
	op_write ( hl.w, temp );
}

// single bit operation commands

void cpu::op_bit_n_r ( u8 n, u8 r )
{
	pc++;
	clear_nflag (); set_hflag ();
	
	if ( !get_bit ( r, n ) ) set_zflag ();
	else clear_zflag ();
}

void cpu::op_bit_n_hl_mem ( u8 n )
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_bit_n_r ( n, temp );
	//op_write ( hl.w, temp );
}

void cpu::op_set_n_r ( u8 n, u8 &r )
{
	pc++;
	set_bit ( r, n );
}

void cpu::op_set_n_hl_mem ( u8 n )
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_set_n_r ( n, temp );
	op_write ( hl.w, temp );
}

void cpu::op_res_n_r ( u8 n, u8 &r )
{
	pc++;
	clear_bit ( r, n );
}

void cpu::op_res_n_hl_mem ( u8 n )
{
	//pc++;
	u8 temp = op_read (hl.w);
	op_res_n_r ( n, temp );
	op_write ( hl.w, temp );
}

// cpu control commands

void cpu::op_ccf ()
{
	pc++;
	clear_nflag (); clear_hflag ();
	
	if ( get_cflag () ) clear_cflag ();
	else set_cflag ();
}

void cpu::op_scf ()
{
	pc++;
	clear_nflag (); clear_hflag (); set_cflag ();
}

void cpu::op_nop ()	// seeing as NOP is short for No OPeration, this does NOTHING
{
	pc++;
}

void cpu::op_halt ()
{
	pc++;
	halted = true;
}

void cpu::op_stop ()
{
	pc++;
	// I am unsure of what to do to emulate this opcode
	// New item on emulator programming to-do list is look up what this opcode does
}

void cpu::op_di ()
{
	pc++;
	pending_clear_ime = true;
}

void cpu::op_ei ()
{
	pc++;
	pending_set_ime = true;
}

// jump commands

void cpu::op_jp_nn ()
{
	pc++;	// We need the pc++ so that we don't use the current pc for op_read_word
	u16 nn = op_read_word (pc);
	// We *DON'T* need a pc+=2 because we are not using a jr/call instruction
	pc = nn;
}

void cpu::op_jp_hl ()
{
	pc++;
	pc = hl.w;
}

void cpu::op_jp_f_nn ( jump_cndtn f )
{
	pc++; 
	switch (f)
	{
		case z:  
			if ( get_zflag () )
				pc = hl.w;
			break;
		case nz:  
			if ( !get_zflag () )
				pc = hl.w;
			break;
		case c:  
			if ( get_cflag () )
				pc = hl.w;
			break;
		case nc:  
			if ( !get_cflag () )
				pc = hl.w;
			break;
		default:  
			break;
	}
}

void cpu::op_jr_n ()
{
	pc++; 
	s8 n = (s8)op_read (pc);
	pc++;
	pc += n;
} 

void cpu::op_jr_f_n ( jump_cndtn f )
{
	pc++; 
	s8 n = (s8)op_read (pc);
	pc++;
	
	switch (f)
	{
		case z:  
			if ( get_zflag () )
				pc += n;
			break;
		case nz:  
			if ( !get_zflag () )
				pc += n;
			break;
		case c:  
			if ( get_cflag () )
				pc += n;
			break;
		case nc:  
			if ( !get_cflag () )
				pc += n;
			break;
		default:  
			break;
	}
}

void cpu::op_call_nn ()
{
	pc++; 
	u16 nn = op_read_word (pc);
	pc += 2;
	
	op_push_word (pc);
	pc = nn;
}

void cpu::op_call_f_nn ( jump_cndtn f )
{
	pc++; 
	u16 nn = op_read_word (pc);
	pc += 2;
	
	switch (f)
	{
		case z:  
			if ( get_zflag () )
			{
				op_push_word (pc);
				pc = nn;
			}
			break;
		case nz:  
			if ( !get_zflag () )
			{
				op_push_word (pc);
				pc = nn;
			}
			break;
		case c:  
			if ( get_cflag () )
			{
				op_push_word (pc);
				pc = nn;
			}
			break;
		case nc:  
			if ( !get_cflag () )
			{
				op_push_word (pc);
				pc = nn;
			}
			break;
		default:  
			break;
	}
}

void cpu::op_ret ()
{
	// We don't need the pc++ here
	op_pop_word (pc);
}

void cpu::op_ret_f ( jump_cndtn f )
{
	// We don't need the pc++ here
	switch (f)
	{
		case z:  
			if ( get_zflag () )
				op_pop_word (pc);
			break;
		case nz:  
			if ( !get_zflag () )
				op_pop_word (pc);
			break;
		case c:  
			if ( get_cflag () )
				op_pop_word (pc);
			break;
		case nc:  
			if ( !get_cflag () )
				op_pop_word (pc);
			break;
		default:  
			break;
	}
}

void cpu::op_reti ()
{
	// We don't need the pc++ here
	op_pop_word (pc);
	ime = true;
}

void cpu::op_rst ( u8 nn )
{
	pc++; 
	op_push_word (pc);
	pc = (u16)nn;
}

// this is in a separate file to make stuff more readable... or something
#include "cpu_jumptable.hpp"
