#ifndef cpu_jumptable_hpp
#define cpu_jumptable_hpp

void cpu::pr ()		// print regs
{
	//printf ( "  PC=0x%x SP=0x%x A=0x%x F=0x%x B=0x%x C=0x%x D=0x%x E=0x%x H=0x%x L=0x%x", 
		//pc, sp, af.hi, af.lo, bc.hi, bc.lo, de.hi, de.lo, hl.hi, hl.lo );
	//printf ("\n");
	
	printf ("  PC="); 
	if ( pc<0x10 ) printf ("000");
	else if ( pc<0x100 ) printf ("00"); 
	else if ( pc<0x1000 ) printf ("0");
	printf ( "%X", pc ); 
	
	printf (" AF=");
	if ( af.hi<0x10 ) { printf ("0"); } printf ( "%X", af.hi );
	if ( af.lo<0x10 ) { printf ("0"); } printf ( "%X", af.lo );
	
	printf (" BC=");
	if ( bc.hi<0x10 ) { printf ("0"); } printf ( "%X", bc.hi );
	if ( bc.lo<0x10 ) { printf ("0"); } printf ( "%X", bc.lo );
	
	printf (" DE=");
	if ( de.hi<0x10 ) { printf ("0"); } printf ( "%X", de.hi );
	if ( de.lo<0x10 ) { printf ("0"); } printf ( "%X", de.lo );
	
	printf (" HL=");
	if ( hl.hi<0x10 ) { printf ("0"); } printf ( "%X", hl.hi );
	if ( hl.lo<0x10 ) { printf ("0"); } printf ( "%X", hl.lo );
	
	printf (" SP=");
	if ( sp<0x10 ) printf ("000");
	else if ( sp<0x100 ) printf ("00"); 
	else if ( sp<0x1000 ) printf ("0");
	printf ( "%X\n", sp ); 
}

void cpu::print_undefined ( u8 opcode )
{
	cout << "Warning:  At PC address 0x" << (int)pc << " Undefined Opcode (0x" 
		<< hex << (int)opcode << dec << ").  Executing NOP Instead.\n";
	op_nop ();
	cout << "Executing infinite loop for logging purposes....\n";
	for (;;);
}

int cpu::exec ()
{
	//branched = 0;
	
	#ifdef cpu_debug
	//if (false)
	//if (halted)
	{
		printf ("Opcode ");
		if ( op_read (pc)<0x10 ) 
			printf ("0");
		printf ( "%x", op_read (pc) );
		
		if ( op_read (pc)!=0xcb )
			pr ();
		else
			printf ("\n");
		
		
		//if ( pc==0xcbb0 )
		//{
			//printf ("Executing infinite loop...\n");
			//for (;;);
		//}
	}
	#endif
	
	opcode = op_read (pc);
	
	// the return value is the number of cycles
	
	switch (opcode)
	{
		case 0x00:		// success
			op_nop ();
			
			//if ( op_read (pc-1)==0x00 && op_read (pc-2)==0x00 
				//&& pc!=0x101 && pc!=0x102 && false )
			//{
				//cout << "Error:  Too many NOPs.  Executing infinite loop.\n";
				//for (;;);
			//}
			
			return 4;
		case 0x01:		// success
			op_load_rr_nn (bc.w);
			return 12;
		case 0x02:		// success
			op_load_rr_mem_a (bc.w);
			return 8;
		case 0x03:		// success
			op_inc_rr (bc.w);
			return 8;
		case 0x04:
			op_inc_r (bc.hi);
			return 4;
		case 0x05:
			op_dec_r (bc.hi);
			return 4;
		case 0x06:
			op_load_r_n (bc.hi);
			return 8;
		case 0x07:
			op_rlca ();
			return 4;
		case 0x08:
			op_load_mem_imm_sp ();
			return 20;
		case 0x09:
			op_add_hl_rr (bc.w);
			return 8;
		case 0x0a:
			op_load_a_rr_mem (bc.w);
			return 8;
		case 0x0b:
			op_dec_rr (bc.w);
			return 8;
		case 0x0c:
			op_inc_r (bc.lo);
			return 4;
		case 0x0d:
			op_dec_r (bc.lo);
			return 4;
		case 0x0e:
			op_load_r_n (bc.lo);
			return 8;
		case 0x0f:
			op_rrca ();
			return 4;
		case 0x10:
			op_stop ();
			return 4;
		case 0x11:
			op_load_rr_nn (de.w);
			return 12;
		case 0x12:
			op_load_rr_mem_a (de.w);
			return 8;
		case 0x13:
			op_inc_rr (de.w);
			return 8;
		case 0x14:
			op_inc_r (de.hi);
			return 4;
		case 0x15:
			op_dec_r (de.hi);
			return 4;
		case 0x16:
			op_load_r_n (de.hi);
			return 8;
		case 0x17:
			op_rla ();
			return 4;
		case 0x18:
			op_jr_n ();
			return 8;
		case 0x19:
			op_add_hl_rr (de.w);
			return 8;
		case 0x1a:
			op_load_a_rr_mem (de.w);
			return 8;
		case 0x1b:
			op_dec_rr (de.w);
			return 8;
		case 0x1c:
			op_inc_r (de.lo);
			return 4;
		case 0x1d:
			op_dec_r (de.lo);
			return 4;
		case 0x1e:
			op_load_r_n (de.lo);
			return 8;
		case 0x1f:
			op_rra ();
			return 4;
		case 0x20:
			op_jr_f_n (nz);
			return 8;
		case 0x21:
			op_load_rr_nn (hl.w);
			return 12;
		case 0x22:
			op_load_hli_a ();
			return 8;
		case 0x23:
			op_inc_rr (hl.w);
			return 8;
		case 0x24:
			op_inc_r (hl.hi);
			return 4;
		case 0x25:
			op_dec_r (hl.hi);
			return 4;
		case 0x26:
			op_load_r_n (hl.hi);
			return 8;
		case 0x27:
			op_daa ();
			return 4;
		case 0x28:
			op_jr_f_n (z);
			return 8;
		case 0x29:
			op_add_hl_rr (hl.w);
			return 8;
		case 0x2a:
			op_load_a_hli ();
			return 8;
		case 0x2b:
			op_dec_rr (hl.w);
			return 8;
		case 0x2c:
			op_inc_r (hl.lo);
			return 4;
		case 0x2d:
			op_dec_r (hl.lo);
			return 4;
		case 0x2e:
			op_load_r_n (hl.lo);
			return 8;
		case 0x2f:
			op_cpl ();
			return 4;
		case 0x30:
			op_jr_f_n (nc);
			return 8;
		case 0x31:
			op_load_rr_nn (sp);
			return 12;
		case 0x32:
			op_load_hld_a ();
			return 8;
		case 0x33:
			op_inc_rr (sp);
			return 8;
		case 0x34:
			op_inc_hl_mem ();
			return 12;
		case 0x35:
			op_dec_hl_mem ();
			return 12;
		case 0x36:
			op_load_hl_mem_n ();
			return 12;
		case 0x37:
			op_scf ();
			return 4;
		case 0x38:
			op_jr_f_n (c);
			return 8;
		case 0x39:
			op_add_hl_rr (sp);
			return 8;
		case 0x3a:
			op_load_a_hld ();
			return 8;
		case 0x3b:
			op_dec_rr (sp);
			return 8;
		case 0x3c:
			op_inc_r (af.hi);
			return 4;
		case 0x3d:
			op_dec_r (af.hi);
			return 4;
		case 0x3e:
			op_load_r_n (af.hi);
			return 8;
		case 0x3f:
			op_ccf ();
			return 4;
		case 0x40:
			op_load_r_r ( bc.hi, bc.hi );
			return 4;
		case 0x41:
			op_load_r_r ( bc.hi, bc.lo );
			return 4;
		case 0x42:
			op_load_r_r ( bc.hi, de.hi );
			return 4;
		case 0x43:
			op_load_r_r ( bc.hi, de.lo );
			return 4;
		case 0x44:
			op_load_r_r ( bc.hi, hl.hi );
			return 4;
		case 0x45:
			op_load_r_r ( bc.hi, hl.lo );
			return 4;
		case 0x46:
			op_load_r_hl_mem (bc.hi);
			return 8;
		case 0x47:
			op_load_r_r ( bc.hi, af.hi );
			return 4;
		case 0x48:
			op_load_r_r ( bc.lo, bc.hi );
			return 4;
		case 0x49:
			op_load_r_r ( bc.lo, bc.lo );
			return 4;
		case 0x4a:
			op_load_r_r ( bc.lo, de.hi );
			return 4;
		case 0x4b:
			op_load_r_r ( bc.lo, de.lo );
			return 4;
		case 0x4c:
			op_load_r_r ( bc.lo, hl.hi );
			return 4;
		case 0x4d:
			op_load_r_r ( bc.lo, hl.lo );
			return 4;
		case 0x4e:
			op_load_r_hl_mem (bc.lo);
			return 8;
		case 0x4f:
			op_load_r_r ( bc.lo, af.hi );
			return 4;
		case 0x50:
			op_load_r_r ( de.hi, bc.hi );
			return 4;
		case 0x51:
			op_load_r_r ( de.hi, bc.lo );
			return 4;
		case 0x52:
			op_load_r_r ( de.hi, de.hi );
			return 4;
		case 0x53:
			op_load_r_r ( de.hi, de.lo );
			return 4;
		case 0x54:
			op_load_r_r ( de.hi, hl.hi );
			return 4;
		case 0x55:
			op_load_r_r ( de.hi, hl.lo );
			return 4;
		case 0x56:
			op_load_r_hl_mem (de.hi);
			return 8;
		case 0x57:
			op_load_r_r ( de.hi, af.hi );
			return 4;
		case 0x58:
			op_load_r_r ( de.lo, bc.hi );
			return 4;
		case 0x59:
			op_load_r_r ( de.lo, bc.lo );
			return 4;
		case 0x5a:
			op_load_r_r ( de.lo, de.hi );
			return 4;
		case 0x5b:
			op_load_r_r ( de.lo, de.lo );
			return 4;
		case 0x5c:
			op_load_r_r ( de.lo, hl.hi );
			return 4;
		case 0x5d:
			op_load_r_r ( de.lo, hl.lo );
			return 4;
		case 0x5e:
			op_load_r_hl_mem (de.lo);
			return 8;
		case 0x5f:
			op_load_r_r ( de.lo, af.hi );
			return 4;
		case 0x60:
			op_load_r_r ( hl.hi, bc.hi );
			return 4;
		case 0x61:
			op_load_r_r ( hl.hi, bc.lo );
			return 4;
		case 0x62:
			op_load_r_r ( hl.hi, de.hi );
			return 4;
		case 0x63:
			op_load_r_r ( hl.hi, de.lo );
			return 4;
		case 0x64:
			op_load_r_r ( hl.hi, hl.hi );
			return 4;
		case 0x65:
			op_load_r_r ( hl.hi, hl.lo );
			return 4;
		case 0x66:
			op_load_r_hl_mem (hl.hi);
			return 8;
		case 0x67:
			op_load_r_r ( hl.hi, af.hi );
			return 4;
		case 0x68:
			op_load_r_r ( hl.lo, bc.hi );
			return 4;
		case 0x69:
			op_load_r_r ( hl.lo, bc.lo );
			return 4;
		case 0x6a:
			op_load_r_r ( hl.lo, de.hi );
			return 4;
		case 0x6b:
			op_load_r_r ( hl.lo, de.lo );
			return 4;
		case 0x6c:
			op_load_r_r ( hl.lo, hl.hi );
			return 4;
		case 0x6d:
			op_load_r_r ( hl.lo, hl.lo );
			return 4;
		case 0x6e:
			op_load_r_hl_mem (hl.lo);
			return 8;
		case 0x6f:
			op_load_r_r ( hl.lo, af.hi );
			return 4;
		case 0x70:
			op_load_hl_mem_r (bc.hi);
			return 8;
		case 0x71:
			op_load_hl_mem_r (bc.lo);
			return 8;
		case 0x72:
			op_load_hl_mem_r (de.hi);
			return 8;
		case 0x73:
			op_load_hl_mem_r (de.lo);
			return 8;
		case 0x74:
			op_load_hl_mem_r (hl.hi);
			return 8;
		case 0x75:
			op_load_hl_mem_r (hl.lo);
			return 8;
		case 0x76:
			op_halt ();
			return 4;
		case 0x77:
			op_load_hl_mem_r (af.hi);
			return 8;
		case 0x78:
			op_load_r_r ( af.hi, bc.hi );
			return 4;
		case 0x79:
			op_load_r_r ( af.hi, bc.lo );
			return 4;
		case 0x7a:
			op_load_r_r ( af.hi, de.hi );
			return 4;
		case 0x7b:
			op_load_r_r ( af.hi, de.lo );
			return 4;
		case 0x7c:
			op_load_r_r ( af.hi, hl.hi );
			return 4;
		case 0x7d:
			op_load_r_r ( af.hi, hl.lo );
			return 4;
		case 0x7e:
			op_load_r_hl_mem (af.hi);
			return 8;
		case 0x7f:
			op_load_r_r ( af.hi, af.hi );
			return 4;
		case 0x80:
			op_add_r (bc.hi);
			return 4;
		case 0x81:
			op_add_r (bc.lo);
			return 4;
		case 0x82:
			op_add_r (de.hi);
			return 4;
		case 0x83:
			op_add_r (de.lo);
			return 4;
		case 0x84:
			op_add_r (hl.hi);
			return 4;
		case 0x85:
			op_add_r (hl.lo);
			return 4;
		case 0x86:
			op_add_hl_mem ();
			return 8;
		case 0x87:
			op_add_r (af.hi);
			return 4;
		case 0x88:
			op_adc_r (bc.hi);
			return 4;
		case 0x89:
			op_adc_r (bc.lo);
			return 4;
		case 0x8a:
			op_adc_r (de.hi);
			return 4;
		case 0x8b:
			op_adc_r (de.lo);
			return 4;
		case 0x8c:
			op_adc_r (hl.hi);
			return 4;
		case 0x8d:
			op_adc_r (hl.lo);
			return 4;
		case 0x8e:
			op_adc_hl_mem ();
			return 8;
		case 0x8f:
			op_adc_r (af.hi);
			return 4;
		case 0x90:
			op_sub_r (bc.hi);
			return 4;
		case 0x91:
			op_sub_r (bc.lo);
			return 4;
		case 0x92:
			op_sub_r (de.hi);
			return 4;
		case 0x93:
			op_sub_r (de.lo);
			return 4;
		case 0x94:
			op_sub_r (hl.hi);
			return 4;
		case 0x95:
			op_sub_r (hl.lo);
			return 4;
		case 0x96:
			op_sub_hl_mem ();
			return 8;
		case 0x97:
			op_sub_r (af.hi);
			return 4;
		case 0x98:
			op_sbc_r (bc.hi);
			return 4;
		case 0x99:
			op_sbc_r (bc.lo);
			return 4;
		case 0x9a:
			op_sbc_r (de.hi);
			return 4;
		case 0x9b:
			op_sbc_r (de.lo);
			return 4;
		case 0x9c:
			op_sbc_r (hl.hi);
			return 4;
		case 0x9d:
			op_sbc_r (hl.lo);
			return 4;
		case 0x9e:
			op_sbc_hl_mem ();
			return 8;
		case 0x9f:
			op_sbc_r (af.hi);
			return 4;
		case 0xa0:
			op_and_r (bc.hi);
			return 4;
		case 0xa1:
			op_and_r (bc.lo);
			return 4;
		case 0xa2:
			op_and_r (de.hi);
			return 4;
		case 0xa3:
			op_and_r (de.lo);
			return 4;
		case 0xa4:
			op_and_r (hl.hi);
			return 4;
		case 0xa5:
			op_and_r (hl.lo);
			return 4;
		case 0xa6:
			op_and_hl_mem ();
			return 8;
		case 0xa7:
			op_and_r (af.hi);
			return 4;
		case 0xa8:
			op_xor_r (bc.hi);
			return 4;
		case 0xa9:
			op_xor_r (bc.lo);
			return 4;
		case 0xaa:
			op_xor_r (de.hi);
			return 4;
		case 0xab:
			op_xor_r (de.lo);
			return 4;
		case 0xac:
			op_xor_r (hl.hi);
			return 4;
		case 0xad:
			op_xor_r (hl.lo);
			return 4;
		case 0xae:
			op_xor_hl_mem ();
			return 8;
		case 0xaf:
			op_xor_r (af.hi);
			return 4;
		case 0xb0:
			op_or_r (bc.hi);
			return 4;
		case 0xb1:
			op_or_r (bc.lo);
			return 4;
		case 0xb2:
			op_or_r (de.hi);
			return 4;
		case 0xb3:
			op_or_r (de.lo);
			return 4;
		case 0xb4:
			op_or_r (hl.hi);
			return 4;
		case 0xb5:
			op_or_r (hl.lo);
			return 4;
		case 0xb6:
			op_or_hl_mem ();
			return 8;
		case 0xb7:
			op_or_r (af.hi);
			return 4;
		case 0xb8:
			op_cp_r (bc.hi);
			return 4;
		case 0xb9:
			op_cp_r (bc.lo);
			return 4;
		case 0xba:
			op_cp_r (de.hi);
			return 4;
		case 0xbb:
			op_cp_r (de.lo);
			return 4;
		case 0xbc:
			op_cp_r (hl.hi);
			return 4;
		case 0xbd:
			op_cp_r (hl.lo);
			return 4;
		case 0xbe:
			op_cp_hl_mem ();
			return 8;
		case 0xbf:
			op_cp_r (af.hi);
			return 4;
		case 0xc0:
			op_ret_f (nz);
			return 8;
		case 0xc1:
			op_pop_rr (bc);
			return 12;
		case 0xc2:
			op_jp_f_nn (nz);
			return 12;
		case 0xc3:
			op_jp_nn ();
			return 12;
		case 0xc4:
			op_call_f_nn (nz);
			return 12;
		case 0xc5:
			op_push_rr (bc);
			return 16;
		case 0xc6:
			op_add_n ();
			return 8;
		case 0xc7:
			op_rst (0x00);
			return 32;
		case 0xc8:
			op_ret_f (z);
			return 8;
		case 0xc9:
			op_ret ();
			return 8;
		case 0xca:
			op_jp_f_nn (z);
			return 12;
		case 0xcb:
			return exec_cb ();
		case 0xcc:
			op_call_f_nn (z);
			return 12;
		case 0xcd:
			op_call_nn ();
			return 12;
		case 0xce:
			op_adc_n ();
			return 8;
		case 0xcf:
			op_rst (0x08);
			return 32;
		case 0xd0:
			op_ret_f (nc);
			return 8;
		case 0xd1:
			op_pop_rr (de);
			return 12;
		case 0xd2:
			op_jp_f_nn (nc);
			return 12;
		case 0xd3:
			print_undefined (opcode);
			return 0;
		case 0xd4:
			op_call_f_nn (nc);
			return 12;
		case 0xd5:
			op_push_rr (de);
			return 16;
		case 0xd6:
			op_sub_n ();
			return 8;
		case 0xd7:
			op_rst (0x10);
			return 32;
		case 0xd8:
			op_ret_f (c);
			return 8;
		case 0xd9:
			op_reti ();
			return 8;
		case 0xda:
			op_jp_f_nn (c);
			return 12;
		case 0xdb:
			print_undefined (opcode);
			return 0;
		case 0xdc:
			op_call_f_nn (c);
			return 12;
		case 0xdd:
			print_undefined (opcode);
			return 0;
		case 0xde:
			op_sbc_n ();
			return 8;
		case 0xdf:
			op_rst (0x18);
			return 32;
		case 0xe0:
			op_load_io_n_a ();
			return 12;
		case 0xe1:
			op_pop_rr (hl);
			return 12;
		case 0xe2:
			op_load_io_c_a ();
			return 8;
		case 0xe3:
			print_undefined (opcode);
			return 0;
		case 0xe4:
			print_undefined (opcode);
			return 0;
		case 0xe5:
			op_push_rr (hl);
			return 16;
		case 0xe6:
			op_and_n ();
			return 8;
		case 0xe7:
			op_rst (0x20);
			return 32;
		case 0xe8:
			op_add_sp_dd ();
			return 16;
		case 0xe9:
			op_jp_hl ();
			return 4;
		case 0xea:
			op_load_mem_imm_a ();
			return 16;
		case 0xeb:
			print_undefined (opcode);
			return 0;
		case 0xec:
			print_undefined (opcode);
			return 0;
		case 0xed:
			print_undefined (opcode);
			return 0;
		case 0xee:
			op_xor_n ();
			return 8;
		case 0xef:
			op_rst (0x28);
			return 32;
		case 0xf0:
			op_load_a_io_n ();
			return 12;
		case 0xf1:
			op_pop_rr (af);
			af.lo &= 0xf0;
			return 12;
		case 0xf2:
			op_load_a_io_c ();
			return 8;
		case 0xf3:
			op_di ();
			return 4;
		case 0xf4:
			print_undefined (opcode);
			return 0;
		case 0xf5:
			op_push_rr (af);
			return 16;
		case 0xf6:
			op_or_n ();
			return 8;
		case 0xf7:
			op_rst (0x30);
			return 32;
		case 0xf8:
			op_load_hl_sp_n ();
			return 12;
		case 0xf9:
			op_load_sp_hl ();
			return 8;
		case 0xfa:
			op_load_a_mem_imm ();
			return 16;
		case 0xfb:
			op_ei ();
			return 4;
		case 0xfc:
			print_undefined (opcode);
			return 0;
		case 0xfd:
			print_undefined (opcode);
			return 0;
		case 0xfe:
			op_cp_n ();
			return 8;
		case 0xff:
			op_rst (0x38);
			return 32;
		default:
			cout << "Woah, this shouldn't occur.  Is opcode 0x" 
				<< hex << opcode << dec << " unimplemented?\n";
			pc++;
			return 0;
			
	}
	
	return 87;	// arbitrary value so the compiler won't complain
}

int cpu::exec_cb ()
{	
	pc++;
			
	#ifdef cpu_debug
	//cout << "CB opcode 0x" << hex << (int)opcode << dec << ".\n";
	//if (false)
	{
		printf ("CB opcode ");
		if ( op_read (pc)<0x10 ) 
			printf ("0");
		printf ( "%X", op_read (pc) );
		
		pr ();
	}
	#endif
	
	opcode = op_read (pc);
	
	switch (opcode)
	{
		case 0x00:
			op_rlc_r (bc.hi);
			return 8;
		case 0x01:
			op_rlc_r (bc.lo);
			return 8;
		case 0x02:
			op_rlc_r (de.hi);
			return 8;
		case 0x03:
			op_rlc_r (de.lo);
			return 8;
		case 0x04:
			op_rlc_r (hl.hi);
			return 8;
		case 0x05:
			op_rlc_r (hl.lo);
			return 8;
		case 0x06:
			op_rlc_hl_mem ();
			return 16;
		case 0x07:
			op_rlc_r (af.hi);
			return 8;
		case 0x08:
			op_rrc_r (bc.hi);
			return 8;
		case 0x09:
			op_rrc_r (bc.lo);
			return 8;
		case 0x0a:
			op_rrc_r (de.hi);
			return 8;
		case 0x0b:
			op_rrc_r (de.lo);
			return 8;
		case 0x0c:
			op_rrc_r (hl.hi);
			return 8;
		case 0x0d:
			op_rrc_r (hl.lo);
			return 8;
		case 0x0e:
			op_rrc_hl_mem ();
			return 16;
		case 0x0f:
			op_rrc_r (af.hi);
			return 8;
		case 0x10:
			op_rl_r (bc.hi);
			return 8;
		case 0x11:
			op_rl_r (bc.lo);
			return 8;
		case 0x12:
			op_rl_r (de.hi);
			return 8;
		case 0x13:
			op_rl_r (de.lo);
			return 8;
		case 0x14:
			op_rl_r (hl.hi);
			return 8;
		case 0x15:
			op_rl_r (hl.lo);
			return 8;
		case 0x16:
			op_rl_hl_mem ();
			return 16;
		case 0x17:
			op_rl_r (af.hi);
			return 8;
		case 0x18:
			op_rr_r (bc.hi);
			return 8;
		case 0x19:
			op_rr_r (bc.lo);
			return 8;
		case 0x1a:
			op_rr_r (de.hi);
			return 8;
		case 0x1b:
			op_rr_r (de.lo);
			return 8;
		case 0x1c:
			op_rr_r (hl.hi);
			return 8;
		case 0x1d:
			op_rr_r (hl.lo);
			return 8;
		case 0x1e:
			op_rr_hl_mem ();
			return 16;
		case 0x1f:
			op_rr_r (af.hi);
			return 8;
		case 0x20:
			op_sla_r (bc.hi);
			return 8;
		case 0x21:
			op_sla_r (bc.lo);
			return 8;
		case 0x22:
			op_sla_r (de.hi);
			return 8;
		case 0x23:
			op_sla_r (de.lo);
			return 8;
		case 0x24:
			op_sla_r (hl.hi);
			return 8;
		case 0x25:
			op_sla_r (hl.lo);
			return 8;
		case 0x26:
			op_sla_hl_mem ();
			return 16;
		case 0x27:
			op_sla_r (af.hi);
			return 8;
		case 0x28:
			op_sra_r (bc.hi);
			return 8;
		case 0x29:
			op_sra_r (bc.lo);
			return 8;
		case 0x2a:
			op_sra_r (de.hi);
			return 8;
		case 0x2b:
			op_sra_r (de.lo);
			return 8;
		case 0x2c:
			op_sra_r (hl.hi);
			return 8;
		case 0x2d:
			op_sra_r (hl.lo);
			return 8;
		case 0x2e:
			op_sra_hl_mem ();
			return 16;
		case 0x2f:
			op_sra_r (af.hi);
			return 8;
		case 0x30:
			op_swap_r (bc.hi);
			return 8;
		case 0x31:
			op_swap_r (bc.lo);
			return 8;
		case 0x32:
			op_swap_r (de.hi);
			return 8;
		case 0x33:
			op_swap_r (de.lo);
			return 8;
		case 0x34:
			op_swap_r (hl.hi);
			return 8;
		case 0x35:
			op_swap_r (hl.lo);
			return 8;
		case 0x36:
			op_swap_hl_mem ();
			return 16;
		case 0x37:
			op_swap_r (af.hi);
			return 8;
		case 0x38:
			op_srl_r (bc.hi);
			return 8;
		case 0x39:
			op_srl_r (bc.lo);
			return 8;
		case 0x3a:
			op_srl_r (de.hi);
			return 8;
		case 0x3b:
			op_srl_r (de.lo);
			return 8;
		case 0x3c:
			op_srl_r (hl.hi);
			return 8;
		case 0x3d:
			op_srl_r (hl.lo);
			return 8;
		case 0x3e:
			op_srl_hl_mem ();
			return 16;
		case 0x3f:
			op_srl_r (af.hi);
			return 8;
		case 0x40:
			op_bit_n_r ( 0, bc.hi );
			return 8;
		case 0x41:
			op_bit_n_r ( 0, bc.lo );
			return 8;
		case 0x42:
			op_bit_n_r ( 0, de.hi );
			return 8;
		case 0x43:
			op_bit_n_r ( 0, de.lo );
			return 8;
		case 0x44:
			op_bit_n_r ( 0, hl.hi );
			return 8;
		case 0x45:
			op_bit_n_r ( 0, hl.lo );
			return 8;
		case 0x46:
			op_bit_n_hl_mem (0); 
			return 16;
		case 0x47:
			op_bit_n_r ( 0, af.hi );
			return 8;
		case 0x48:
			op_bit_n_r ( 1, bc.hi );
			return 8;
		case 0x49:
			op_bit_n_r ( 1, bc.lo );
			return 8;
		case 0x4a:
			op_bit_n_r ( 1, de.hi );
			return 8;
		case 0x4b:
			op_bit_n_r ( 1, de.lo );
			return 8;
		case 0x4c:
			op_bit_n_r ( 1, hl.hi );
			return 8;
		case 0x4d:
			op_bit_n_r ( 1, hl.lo );
			return 8;
		case 0x4e:
			op_bit_n_hl_mem (1); 
			return 16;
		case 0x4f:
			op_bit_n_r ( 1, af.hi );
			return 8;
		case 0x50:
			op_bit_n_r ( 2, bc.hi );
			return 8;
		case 0x51:
			op_bit_n_r ( 2, bc.lo );
			return 8;
		case 0x52:
			op_bit_n_r ( 2, de.hi );
			return 8;
		case 0x53:
			op_bit_n_r ( 2, de.lo );
			return 8;
		case 0x54:
			op_bit_n_r ( 2, hl.hi );
			return 8;
		case 0x55:
			op_bit_n_r ( 2, hl.lo );
			return 8;
		case 0x56:
			op_bit_n_hl_mem (2); 
			return 16;
		case 0x57:
			op_bit_n_r ( 2, af.hi );
			return 8;
		case 0x58:
			op_bit_n_r ( 3, bc.hi );
			return 8;
		case 0x59:
			op_bit_n_r ( 3, bc.lo );
			return 8;
		case 0x5a:
			op_bit_n_r ( 3, de.hi );
			return 8;
		case 0x5b:
			op_bit_n_r ( 3, de.lo );
			return 8;
		case 0x5c:
			op_bit_n_r ( 3, hl.hi );
			return 8;
		case 0x5d:
			op_bit_n_r ( 3, hl.lo );
			return 8;
		case 0x5e:
			op_bit_n_hl_mem (3); 
			return 16;
		case 0x5f:
			op_bit_n_r ( 3, af.hi );
			return 8;
		case 0x60:
			op_bit_n_r ( 4, bc.hi );
			return 8;
		case 0x61:
			op_bit_n_r ( 4, bc.lo );
			return 8;
		case 0x62:
			op_bit_n_r ( 4, de.hi );
			return 8;
		case 0x63:
			op_bit_n_r ( 4, de.lo );
			return 8;
		case 0x64:
			op_bit_n_r ( 4, hl.hi );
			return 8;
		case 0x65:
			op_bit_n_r ( 4, hl.lo );
			return 8;
		case 0x66:
			op_bit_n_hl_mem (4); 
			return 16;
		case 0x67:
			op_bit_n_r ( 4, af.hi );
			return 8;
		case 0x68:
			op_bit_n_r ( 5, bc.hi );
			return 8;
		case 0x69:
			op_bit_n_r ( 5, bc.lo );
			return 8;
		case 0x6a:
			op_bit_n_r ( 5, de.hi );
			return 8;
		case 0x6b:
			op_bit_n_r ( 5, de.lo );
			return 8;
		case 0x6c:
			op_bit_n_r ( 5, hl.hi );
			return 8;
		case 0x6d:
			op_bit_n_r ( 5, hl.lo );
			return 8;
		case 0x6e:
			op_bit_n_hl_mem (5); 
			return 16;
		case 0x6f:
			op_bit_n_r ( 5, af.hi );
			return 8;
		case 0x70:
			op_bit_n_r ( 6, bc.hi );
			return 8;
		case 0x71:
			op_bit_n_r ( 6, bc.lo );
			return 8;
		case 0x72:
			op_bit_n_r ( 6, de.hi );
			return 8;
		case 0x73:
			op_bit_n_r ( 6, de.lo );
			return 8;
		case 0x74:
			op_bit_n_r ( 6, hl.hi );
			return 8;
		case 0x75:
			op_bit_n_r ( 6, hl.lo );
			return 8;
		case 0x76:
			op_bit_n_hl_mem (6); 
			return 16;
		case 0x77:
			op_bit_n_r ( 6, af.hi );
			return 8;
		case 0x78:
			op_bit_n_r ( 7, bc.hi );
			return 8;
		case 0x79:
			op_bit_n_r ( 7, bc.lo );
			return 8;
		case 0x7a:
			op_bit_n_r ( 7, de.hi );
			return 8;
		case 0x7b:
			op_bit_n_r ( 7, de.lo );
			return 8;
		case 0x7c:
			op_bit_n_r ( 7, hl.hi );
			return 8;
		case 0x7d:
			op_bit_n_r ( 7, hl.lo );
			return 8;
		case 0x7e:
			op_bit_n_hl_mem (7); 
			return 16;
		case 0x7f:
			op_bit_n_r ( 7, af.hi );
			return 8;
		case 0x80:
			op_res_n_r ( 0, bc.hi );
			return 8;
		case 0x81:
			op_res_n_r ( 0, bc.lo );
			return 8;
		case 0x82:
			op_res_n_r ( 0, de.hi );
			return 8;
		case 0x83:
			op_res_n_r ( 0, de.lo );
			return 8;
		case 0x84:
			op_res_n_r ( 0, hl.hi );
			return 8;
		case 0x85:
			op_res_n_r ( 0, hl.lo );
			return 8;
		case 0x86:
			op_res_n_hl_mem (0); 
			return 16;
		case 0x87:
			op_res_n_r ( 0, af.hi );
			return 8;
		case 0x88:
			op_res_n_r ( 1, bc.hi );
			return 8;
		case 0x89:
			op_res_n_r ( 1, bc.lo );
			return 8;
		case 0x8a:
			op_res_n_r ( 1, de.hi );
			return 8;
		case 0x8b:
			op_res_n_r ( 1, de.lo );
			return 8;
		case 0x8c:
			op_res_n_r ( 1, hl.hi );
			return 8;
		case 0x8d:
			op_res_n_r ( 1, hl.lo );
			return 8;
		case 0x8e:
			op_res_n_hl_mem (1); 
			return 16;
		case 0x8f:
			op_res_n_r ( 1, af.hi );
			return 8;
		case 0x90:
			op_res_n_r ( 2, bc.hi );
			return 8;
		case 0x91:
			op_res_n_r ( 2, bc.lo );
			return 8;
		case 0x92:
			op_res_n_r ( 2, de.hi );
			return 8;
		case 0x93:
			op_res_n_r ( 2, de.lo );
			return 8;
		case 0x94:
			op_res_n_r ( 2, hl.hi );
			return 8;
		case 0x95:
			op_res_n_r ( 2, hl.lo );
			return 8;
		case 0x96:
			op_res_n_hl_mem (2); 
			return 16;
		case 0x97:
			op_res_n_r ( 2, af.hi );
			return 8;
		case 0x98:
			op_res_n_r ( 3, bc.hi );
			return 8;
		case 0x99:
			op_res_n_r ( 3, bc.lo );
			return 8;
		case 0x9a:
			op_res_n_r ( 3, de.hi );
			return 8;
		case 0x9b:
			op_res_n_r ( 3, de.lo );
			return 8;
		case 0x9c:
			op_res_n_r ( 3, hl.hi );
			return 8;
		case 0x9d:
			op_res_n_r ( 3, hl.lo );
			return 8;
		case 0x9e:
			op_res_n_hl_mem (3); 
			return 16;
		case 0x9f:
			op_res_n_r ( 3, af.hi );
			return 8;
		case 0xa0:
			op_res_n_r ( 4, bc.hi );
			return 8;
		case 0xa1:
			op_res_n_r ( 4, bc.lo );
			return 8;
		case 0xa2:
			op_res_n_r ( 4, de.hi );
			return 8;
		case 0xa3:
			op_res_n_r ( 4, de.lo );
			return 8;
		case 0xa4:
			op_res_n_r ( 4, hl.hi );
			return 8;
		case 0xa5:
			op_res_n_r ( 4, hl.lo );
			return 8;
		case 0xa6:
			op_res_n_hl_mem (4); 
			return 16;
		case 0xa7:
			op_res_n_r ( 4, af.hi );
			return 8;
		case 0xa8:
			op_res_n_r ( 5, bc.hi );
			return 8;
		case 0xa9:
			op_res_n_r ( 5, bc.lo );
			return 8;
		case 0xaa:
			op_res_n_r ( 5, de.hi );
			return 8;
		case 0xab:
			op_res_n_r ( 5, de.lo );
			return 8;
		case 0xac:
			op_res_n_r ( 5, hl.hi );
			return 8;
		case 0xad:
			op_res_n_r ( 5, hl.lo );
			return 8;
		case 0xae:
			op_res_n_hl_mem (5); 
			return 16;
		case 0xaf:
			op_res_n_r ( 5, af.hi );
			return 8;
		case 0xb0:
			op_res_n_r ( 6, bc.hi );
			return 8;
		case 0xb1:
			op_res_n_r ( 6, bc.lo );
			return 8;
		case 0xb2:
			op_res_n_r ( 6, de.hi );
			return 8;
		case 0xb3:
			op_res_n_r ( 6, de.lo );
			return 8;
		case 0xb4:
			op_res_n_r ( 6, hl.hi );
			return 8;
		case 0xb5:
			op_res_n_r ( 6, hl.lo );
			return 8;
		case 0xb6:
			op_res_n_hl_mem (6); 
			return 16;
		case 0xb7:
			op_res_n_r ( 6, af.hi );
			return 8;
		case 0xb8:
			op_res_n_r ( 7, bc.hi );
			return 8;
		case 0xb9:
			op_res_n_r ( 7, bc.lo );
			return 8;
		case 0xba:
			op_res_n_r ( 7, de.hi );
			return 8;
		case 0xbb:
			op_res_n_r ( 7, de.lo );
			return 8;
		case 0xbc:
			op_res_n_r ( 7, hl.hi );
			return 8;
		case 0xbd:
			op_res_n_r ( 7, hl.lo );
			return 8;
		case 0xbe:
			op_res_n_hl_mem (7); 
			return 16;
		case 0xbf:
			op_res_n_r ( 7, af.hi );
			return 8;
		case 0xc0:
			op_set_n_r ( 0, bc.hi );
			return 8;
		case 0xc1:
			op_set_n_r ( 0, bc.lo );
			return 8;
		case 0xc2:
			op_set_n_r ( 0, de.hi );
			return 8;
		case 0xc3:
			op_set_n_r ( 0, de.lo );
			return 8;
		case 0xc4:
			op_set_n_r ( 0, hl.hi );
			return 8;
		case 0xc5:
			op_set_n_r ( 0, hl.lo );
			return 8;
		case 0xc6:
			op_set_n_hl_mem (0); 
			return 16;
		case 0xc7:
			op_set_n_r ( 0, af.hi );
			return 8;
		case 0xc8:
			op_set_n_r ( 1, bc.hi );
			return 8;
		case 0xc9:
			op_set_n_r ( 1, bc.lo );
			return 8;
		case 0xca:
			op_set_n_r ( 1, de.hi );
			return 8;
		case 0xcb:
			op_set_n_r ( 1, de.lo );
			return 8;
		case 0xcc:
			op_set_n_r ( 1, hl.hi );
			return 8;
		case 0xcd:
			op_set_n_r ( 1, hl.lo );
			return 8;
		case 0xce:
			op_set_n_hl_mem (1); 
			return 16;
		case 0xcf:
			op_set_n_r ( 1, af.hi );
			return 8;
		case 0xd0:
			op_set_n_r ( 2, bc.hi );
			return 8;
		case 0xd1:
			op_set_n_r ( 2, bc.lo );
			return 8;
		case 0xd2:
			op_set_n_r ( 2, de.hi );
			return 8;
		case 0xd3:
			op_set_n_r ( 2, de.lo );
			return 8;
		case 0xd4:
			op_set_n_r ( 2, hl.hi );
			return 8;
		case 0xd5:
			op_set_n_r ( 2, hl.lo );
			return 8;
		case 0xd6:
			op_set_n_hl_mem (2); 
			return 16;
		case 0xd7:
			op_set_n_r ( 2, af.hi );
			return 8;
		case 0xd8:
			op_set_n_r ( 3, bc.hi );
			return 8;
		case 0xd9:
			op_set_n_r ( 3, bc.lo );
			return 8;
		case 0xda:
			op_set_n_r ( 3, de.hi );
			return 8;
		case 0xdb:
			op_set_n_r ( 3, de.lo );
			return 8;
		case 0xdc:
			op_set_n_r ( 3, hl.hi );
			return 8;
		case 0xdd:
			op_set_n_r ( 3, hl.lo );
			return 8;
		case 0xde:
			op_set_n_hl_mem (3); 
			return 16;
		case 0xdf:
			op_set_n_r ( 3, af.hi );
			return 8;
		case 0xe0:
			op_set_n_r ( 4, bc.hi );
			return 8;
		case 0xe1:
			op_set_n_r ( 4, bc.lo );
			return 8;
		case 0xe2:
			op_set_n_r ( 4, de.hi );
			return 8;
		case 0xe3:
			op_set_n_r ( 4, de.lo );
			return 8;
		case 0xe4:
			op_set_n_r ( 4, hl.hi );
			return 8;
		case 0xe5:
			op_set_n_r ( 4, hl.lo );
			return 8;
		case 0xe6:
			op_set_n_hl_mem (4); 
			return 16;
		case 0xe7:
			op_set_n_r ( 4, af.hi );
			return 8;
		case 0xe8:
			op_set_n_r ( 5, bc.hi );
			return 8;
		case 0xe9:
			op_set_n_r ( 5, bc.lo );
			return 8;
		case 0xea:
			op_set_n_r ( 5, de.hi );
			return 8;
		case 0xeb:
			op_set_n_r ( 5, de.lo );
			return 8;
		case 0xec:
			op_set_n_r ( 5, hl.hi );
			return 8;
		case 0xed:
			op_set_n_r ( 5, hl.lo );
			return 8;
		case 0xee:
			op_set_n_hl_mem (5); 
			return 16;
		case 0xef:
			op_set_n_r ( 5, af.hi );
			return 8;
		case 0xf0:
			op_set_n_r ( 6, bc.hi );
			return 8;
		case 0xf1:
			op_set_n_r ( 6, bc.lo );
			return 8;
		case 0xf2:
			op_set_n_r ( 6, de.hi );
			return 8;
		case 0xf3:
			op_set_n_r ( 6, de.lo );
			return 8;
		case 0xf4:
			op_set_n_r ( 6, hl.hi );
			return 8;
		case 0xf5:
			op_set_n_r ( 6, hl.lo );
			return 8;
		case 0xf6:
			op_set_n_hl_mem (6); 
			return 16;
		case 0xf7:
			op_set_n_r ( 6, af.hi );
			return 8;
		case 0xf8:
			op_set_n_r ( 7, bc.hi );
			return 8;
		case 0xf9:
			op_set_n_r ( 7, bc.lo );
			return 8;
		case 0xfa:
			op_set_n_r ( 7, de.hi );
			return 8;
		case 0xfb:
			op_set_n_r ( 7, de.lo );
			return 8;
		case 0xfc:
			op_set_n_r ( 7, hl.hi );
			return 8;
		case 0xfd:
			op_set_n_r ( 7, hl.lo );
			return 8;
		case 0xfe:
			op_set_n_hl_mem (7); 
			return 16;
		case 0xff:
			op_set_n_r ( 7, af.hi );
			return 8;
	}
	
	
	return 87;	// arbitrary value so the compiler won't complain
}

#endif // cpu_jumptable_hpp
