#ifndef CONTROL
#define CONTROL

#include "core.hpp"

void control_unit(id_stream* from1, id_stream* from2, id_stream* from3, id_stream* from4, uint32_t number,
		id_stream* to1, id_stream* to2, id_stream* c1, id_stream* c2, id_stream* c3, id_stream* c4);
void get_data(id_stream* from1, id_stream* from2, val_stream* to1, val_stream* to2, ap_uint<256>* buffer);
void final_data(id_stream* c1, id_stream* c2, id_stream* c3, id_stream* c4, val_stream* from1, val_stream* from2,
		val_stream* to1, val_stream* to2, val_stream* to3, val_stream* to4);

#endif
