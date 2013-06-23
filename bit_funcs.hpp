#ifndef bit_funcs_hpp
#define bit_funcs_hpp

template < typename t > bool test_bit ( t r, int bit_slot )
{
	return (bool)( ( r&(1<<bit_slot) )>>bit_slot );
}

template < typename t > bool get_bit ( t r, int bit_slot )
{
	return (bool)( ( r&(1<<bit_slot) )>>bit_slot );
}

template < typename t > void set_bit ( t &r, int bit_slot )
{
	r |= (1<<bit_slot);
}

template < typename t > void clear_bit ( t &r, int bit_slot )
{
	r &= ~(1<<bit_slot);
}

#endif // bit_funcs_hpp
