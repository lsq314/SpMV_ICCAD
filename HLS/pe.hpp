#ifndef PE
#define PE

#include "core.hpp"

void processing_element(data_stream* from1, val_stream* from2, id_stream* from3, subresult_stream* to0){

	val_t _c_0 = (val_t)0;

	for(bool enable = true; enable; ){
#pragma HLS PIPELINE II=1
		if (!from1->empty() && !from3->empty() && !from2->empty()){

			val_data<val_t> val = from1->read();
			id_t index = from3->read();
			val_t col_value = from2->read();

			val_t result_t = val.data*col_value;

			sub_result<val_t> result0;
			result0.partial_end = val.partial_end;
            result0.data = (index == TERMINATE) ? _c_0 : result_t;
			result0.final_end = val.final_end;
			result0.rid = index;

			// if current row index is not padding rid
			// or the row index is padding one but it's in the last group
			if (result0.rid != TERMINATE || !result0.partial_end)
				to0->write(result0);

			enable = val.final_end;
		}
	}

}

#endif
