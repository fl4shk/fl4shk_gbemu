#ifndef cpu_cls_hpp
#define cpu_cls_hpp

// Union for general-purpose registers (cpugpr looks strange to me, so I went with cpureg)
// The idea here is to reduce the amount of bitwise stuff at the C++ level.
// As a bonus, it also makes the code more readable. (I think...)
union cpureg { u16 w; struct { u8 lo, hi; }; };

enum jump_cndtn { z, nz, c, nc };	// jump condition

struct cpu		// Gameboy CPU Core
{
public:  // variables
	cpureg af, bc, de, hl; u16 sp, pc;  //, m, t;
	
	static const int max_cycles = 69905,
		zflag_slot = 7, nflag_slot = 6, hflag_slot = 5, cflag_slot = 4;
	
	bool halted, stopped, ime, pending_set_ime, pending_clear_ime;
	
	
	
	// ram stuff for debugging the CPU core
	//static const int ram_size = 0x10000;
	//u8 ram [ram_size];
	
	
	
public:  // functions
	cpu () { reset (); }
	~cpu () {}
	
	// these functions are declared virtual to assist with multiple inheritance
	virtual u8 op_read ( u16 addr ) { return 0; };	
	virtual u16 op_read_word ( u16 addr ) { return 0; };
	virtual void op_write ( u16 addr, u8 data ) {};
	virtual bool stop () { return true; };
	
	// This function is declared virtual because of the multiple inheritance
	// TO DO:  Implement the differences between the different GB types
	virtual void reset () 
		{ af.w = 0x0013; bc.w = 0x00d8; de.w = 0x014d; 
		hl.w = 0x014d; sp = 0xFFFE; pc = 0x100; }
	
	// I don't think these two functions need to be virtual, but meh
	virtual int exec ();
	virtual no_fcall int exec_cb ();
	
	// the no_fcall thing is a #define that makes functions always_inline (gcc/g++ only AFAIK)
	no_fcall void set_zflag (); no_fcall void clear_zflag (); 
	no_fcall bool get_zflag () const;
	no_fcall void set_nflag (); no_fcall void clear_nflag (); 
	no_fcall bool get_nflag () const;
	no_fcall void set_hflag (); no_fcall void clear_hflag (); 
	no_fcall bool get_hflag () const;
	no_fcall void set_cflag (); no_fcall void clear_cflag (); 
	no_fcall bool get_cflag () const;
	
	no_fcall void op_xx ();
	no_fcall void op_cb ();
	
	// load commands
	no_fcall void op_load_r_r ( u8 &r1, u8 r2 ); 
	no_fcall void op_load_r_n ( u8 &r );
	
	no_fcall void op_load_r_hl_mem ( u8 &r ); 
	no_fcall void op_load_hl_mem_r ( u8 r );
	no_fcall void op_load_hl_mem_n ();
	
	no_fcall void op_load_a_rr_mem ( u16 rr ); 
	no_fcall void op_load_rr_mem_a ( u16 rr );
	no_fcall void op_load_a_mem_imm (); 
	no_fcall void op_load_mem_imm_a ();
	
	no_fcall void op_load_a_io_c (); 
	no_fcall void op_load_io_c_a ();
	no_fcall void op_load_a_io_n (); 
	no_fcall void op_load_io_n_a ();
	
	no_fcall void op_load_a_hli (); 
	no_fcall void op_load_hli_a ();
	no_fcall void op_load_a_hld (); 
	no_fcall void op_load_hld_a ();
	
	no_fcall void op_load_rr_nn ( u16 &rr ); 
	
	no_fcall void op_load_sp_hl (); 
	no_fcall void op_load_hl_sp_n (); 
	no_fcall void op_load_mem_imm_sp ();
	
	// stack commands
	no_fcall void op_push_rr ( cpureg rr ); 
	no_fcall void op_pop_rr ( cpureg &rr );
	no_fcall void op_push_word ( cpureg rr ); 
	no_fcall void op_push_word ( u16 rr );
	no_fcall void op_pop_word ( cpureg &rr ); 
	no_fcall void op_pop_word ( u16 &rr );
	
	
	// math/logic commands
	no_fcall void op_add_r ( u8 r ); 
	no_fcall void op_add_n (); 
	no_fcall void op_add_hl_mem ();
	no_fcall void op_adc_r ( u8 r ); 
	no_fcall void op_adc_n (); 
	no_fcall void op_adc_hl_mem ();
	no_fcall void op_sub_r ( u8 r ); 
	no_fcall void op_sub_n (); 
	no_fcall void op_sub_hl_mem ();
	no_fcall void op_sbc_r ( u8 r ); 
	no_fcall void op_sbc_n (); 
	no_fcall void op_sbc_hl_mem ();
	
	no_fcall void op_and_r ( u8 r ); 
	no_fcall void op_and_n (); 
	no_fcall void op_and_hl_mem ();
	no_fcall void op_or_r ( u8 r ); 
	no_fcall void op_or_n (); 
	no_fcall void op_or_hl_mem ();
	no_fcall void op_xor_r ( u8 r ); 
	no_fcall void op_xor_n (); 
	no_fcall void op_xor_hl_mem ();
	
	no_fcall void op_cp_r ( u8 r ); 
	no_fcall void op_cp_n (); 
	no_fcall void op_cp_hl_mem ();
	no_fcall void op_inc_r ( u8 &r ); 
	no_fcall void op_inc_hl_mem ();
	no_fcall void op_dec_r ( u8 &r ); 
	no_fcall void op_dec_hl_mem ();
	
	no_fcall void op_daa (); 
	no_fcall void op_cpl ();  
	
	no_fcall void op_add_hl_rr ( u16 rr ); 
	no_fcall void op_inc_rr ( u16 &rr ); 
	no_fcall void op_dec_rr ( u16 &rr );
	no_fcall void op_add_sp_dd (); 
	
	// rotate and shift commands
	no_fcall void op_rlca (); 
	no_fcall void op_rla (); 
	no_fcall void op_rrca (); 
	no_fcall void op_rra ();
	no_fcall void op_rlc_r ( u8 &r ); 
	no_fcall void op_rlc_hl_mem ();
	no_fcall void op_rl_r ( u8 &r ); 
	no_fcall void op_rl_hl_mem ();
	no_fcall void op_rrc_r ( u8 &r); 
	no_fcall void op_rrc_hl_mem ();
	no_fcall void op_rr_r ( u8 &r ); 
	no_fcall void op_rr_hl_mem ();
	
	no_fcall void op_sla_r ( u8 &r ); 
	no_fcall void op_sla_hl_mem ();
	no_fcall void op_sra_r ( u8 &r ); 
	no_fcall void op_sra_hl_mem ();
	no_fcall void op_srl_r ( u8 &r ); 
	no_fcall void op_srl_hl_mem ();
	
	no_fcall void op_swap_r ( u8 &r ); 
	no_fcall void op_swap_hl_mem ();
	
	// single bit operation commands
	no_fcall void op_bit_n_r ( u8 n, u8 r ); 
	no_fcall void op_bit_n_hl_mem ( u8 n );
	no_fcall void op_set_n_r ( u8 n, u8 &r ); 
	no_fcall void op_set_n_hl_mem ( u8 n );
	no_fcall void op_res_n_r ( u8 n, u8 &r ); 
	no_fcall void op_res_n_hl_mem ( u8 n );
	
	// cpu control commands
	no_fcall void op_ccf (); 
	no_fcall void op_scf ();
	no_fcall void op_nop ();
	no_fcall void op_halt (); 
	no_fcall void op_stop ();
	
	no_fcall void op_di (); 
	no_fcall void op_ei ();
	
	// jump commands
	no_fcall void op_jp_nn (); 
	no_fcall void op_jp_hl (); 
	no_fcall void op_jp_f_nn ( jump_cndtn f );
	no_fcall void op_jr_n (); 
	no_fcall void op_jr_f_n ( jump_cndtn f );
	
	no_fcall void op_call_nn (); 
	no_fcall void op_call_f_nn ( jump_cndtn f );
	no_fcall void op_ret (); 
	no_fcall void op_ret_f ( jump_cndtn f ); 
	no_fcall void op_reti ();
	no_fcall void op_rst ( u8 nn );
};


#include "cpu_funcs.hpp"
#include "cpu_jumptable.hpp"

#endif // cpu_cls_hpp
