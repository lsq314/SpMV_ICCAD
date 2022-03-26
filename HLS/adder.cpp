#include "core.hpp"
#include "adder.hpp"

void final_adder(data_stream* from1, data_stream* to1){

	uint32_t i = 0;
	// this index points to the available element in the 6-element buffer
	uint32_t index = 0;
	// the ping-pong 6-element buffers
	val_t subresult_ping[6];
	val_t subresult_pong[6];

	// control the exchange of ping-pong buffers
	bool exchange = true;
	uint32_t change_time = 0;
	uint32_t transfer_counter = 0;
	uint32_t expected_threshold = 0;

	// initialize the ping-pong buffer
	for (; i<6; i++){
		subresult_ping[i] = (val_t)0;
		subresult_pong[i] = (val_t)0;
	}

	for (bool enable=true; enable; ){
#pragma HLS DEPENDENCE variable=subresult_ping array inter RAW false
#pragma HLS DEPENDENCE variable=subresult_pong array inter RAW false
#pragma HLS PIPELINE II=1
		if (!from1->empty()) {
			// if the current cycle has input from the corresponding merger
			val_data<val_t> temp = from1->read();
			// whether all the elements has been processed, the last one whose final_end is true is used for control
			if (temp.final_end) {
				// judge which buffer is used
				if (exchange){
					subresult_ping[index] += temp.data;
					// when exchange happens and all the elements are transferred
					// transfer_count: the number of elements which have been transferred
					// expected_threshold: we expect 6 elements that shall be transferred in each row batch
					if (change_time > 0 && transfer_counter < expected_threshold) {
						val_data<val_t> subresult;
						subresult.data = subresult_pong[index];
						subresult.final_end = temp.final_end;
						to1->write(subresult);
						// when transfer one element reset the element and update the counter
						subresult_pong[index] = (val_t)0;
						transfer_counter += 1;
					}
				} else {
					// if the other buffer is used
					subresult_pong[index] += temp.data;
					// the change_time must be bigger than 0
					if (transfer_counter < expected_threshold) {
						val_data<val_t> subresult;
						subresult.data = subresult_ping[index];
						subresult.final_end = temp.final_end;
						to1->write(subresult);
						subresult_ping[index] = (val_t)0;
						transfer_counter += 1;
					}
				}

				index = (index==5) ? 0 : index+1;

				// when one row batch ends, we update the parameters to stream the subresults (6 elements) to the final accumulator
				expected_threshold = (temp.partial_end) ? expected_threshold : expected_threshold + 6;
				exchange = (temp.partial_end) ? exchange : !exchange;
				change_time = (temp.partial_end) ? change_time : change_time + 1;
			}
			enable = temp.final_end;
		} else if (transfer_counter < expected_threshold) {
			// when there is no input and the sub-results are not transferred
			if (exchange){
				if (change_time > 0) {
					val_data<val_t> subresult;
					subresult.data = subresult_pong[index];
					subresult.final_end = true;
					to1->write(subresult);
					subresult_pong[index] = (val_t)0;
					transfer_counter += 1;
				}
			} else {
				val_data<val_t> subresult;
				subresult.data = subresult_ping[index];
				subresult.final_end = true;
				to1->write(subresult);
				subresult_ping[index] = (val_t)0;
				transfer_counter += 1;
			}

			index = (index==5) ? 0 : index+1;
		}
	}

	// deal with the remaining elements in the FIFO
	// we observe that 2 elements are remaining in the FIFO
	if (!exchange) {
		for (i=4; i<6; i++){
#pragma HLS PIPELINE II=1
			val_data<val_t> subresult;
			subresult.data = subresult_ping[index];
			subresult.final_end = (i < 5);
			to1->write(subresult);
			index = (index==5) ? 0 : index+1;
		}
	} else {
		for (i=4; i<6; i++){
#pragma HLS PIPELINE II=1
			val_data<val_t> subresult;
			subresult.data = subresult_pong[index];
			subresult.final_end = (i < 5);
			to1->write(subresult);
			index = (index==5) ? 0 : index+1;
		}
	}

}

void accumulator(data_stream* from1, data_stream* to1){

	uint32_t i = 0;

	for (bool enable=true; enable; ){
		val_data<val_t> final;
		final.data = (val_t)0;
		// the computation of the final results can be covered by the next batch, thus the II can be greater than 1
		for (i=0; i<6; i++){
#pragma HLS PIPELINE
			val_data<val_t> temp = from1->read();
			final.data += temp.data;
			final.final_end = temp.final_end;
			enable = temp.final_end;
		}
		to1->write(final);
	}
}
