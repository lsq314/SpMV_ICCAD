#include "control.hpp"

void control_unit(id_stream* from1, id_stream* from2, id_stream* from3, id_stream* from4, uint32_t number,
		id_stream* to1, id_stream* to2, id_stream* c1, id_stream* c2, id_stream* c3, id_stream* c4){

	// This unit is used to generate the control instructions
	// Instructions: 32 -- REUSE1, 64 -- REUSE2, 2 -- LOAD1 or BUFFER1, 4 -- LOAD2 or BUFFER2
	//               8/16: used to control if no intra-/inter- reuse

	bool keepInput = false;
	uint32_t index1, index2, index3, index4;
	uint32_t last_index1, last_index2;

	for (uint32_t i=0; i<number; ){
#pragma HLS PIPELINE II=1
		if (!keepInput){
			index1 = from1->read();
			index2 = from2->read();
			index3 = from3->read();
			index4 = from4->read();

			bool e1 = index1 == index2;
			bool e2 = index2 == index3;
			bool e3 = index3 == index4;

			// if the four cids can be merged into two column indices
			if (e1 && e2 && e3) {
				to1->write(index1);
				to2->write(index1);
				c1->write(2);
				c2->write(2);
				c3->write(2);
				c4->write(2);

				last_index1 = index1;
				last_index2 = index1;

				i++;
			} else if (e1 && (!e2) && e3) {
				to1->write(index1);
				to2->write(index3);
				c1->write(2);
				c2->write(2);
				c3->write(4);
				c4->write(4);

				last_index1 = index1;
				last_index2 = index3;

				i++;
			} else if (!e1 && e2 && e3) {
				to1->write(index1);
				to2->write(index3);
				c1->write(2);
				c2->write(4);
				c3->write(4);
				c4->write(4);

				last_index1 = index1;
				last_index2 = index3;

				i++;
			} else if (e1 && e2 && !e3) {
				to1->write(index1);
				to2->write(index4);
				c1->write(2);
				c2->write(2);
				c3->write(2);
				c4->write(4);

				last_index1 = index1;
				last_index2 = index4;

				i++;
			} else {
				// if can't be merged into two column indices
				// there are two cases: case 1: reuse; case 2: can't reuse
				// general case:
				// cycle 0: index1 index1 index2 index2
				// cycle 1: index1 index2 newindex1 newindex2
				// the columns we selected have more than 3 elements
				if (i>0) {
					if (index1 == last_index1 && index2 == last_index2) {
						to1->write(index3);
						to2->write(index4);

						c1->write(32);
						c2->write(64);
						c3->write(2);
						c4->write(4);

						i++;
					} else if (e2 && index1 == last_index1) {
						to1->write(index2);
						to2->write(index4);

						c1->write(32);
						c2->write(2);
						c3->write(2);
						c4->write(4);

						last_index2 = index2;

						i++;
					} else if (e2 && index1 == last_index2) {
						to1->write(index2);
						to2->write(index4);

						c1->write(64);
						c2->write(2);
						c3->write(2);
						c4->write(4);

						last_index1 = index1;
						last_index2 = index2;

						i++;
					} else if (index1 == last_index1 && !e2) {
						to1->write(index2);
						to2->write(index3);

						c1->write(32);
						c2->write(2);
						c3->write(4);
						c4->write(4);

						last_index2 = index2;

						i++;
					} else if (index1 == last_index2 && !e2) {
						to1->write(index2);
						to2->write(index3);

						c1->write(64);
						c2->write(2);
						c3->write(4);
						c4->write(4);

						last_index1 = index1;

						i++;
					} else {
						to1->write(index1);
						to2->write(index2);
						c1->write(2);
						c2->write(4);
						c3->write(8);
						c4->write(16);
						keepInput = true;
					}

				} else {
					// need two more cycles
					to1->write(index1);
					to2->write(index2);
					c1->write(2);
					c2->write(4);
					c3->write(8);
					c4->write(16);
					keepInput = true;
				}
			}
		} else {
			to1->write(index3);
			to2->write(index4);

			i++;
			keepInput = false;
		}
	}

	to1->write(TERMINATE);
	to2->write(TERMINATE);
	c1->write(TERMINATE);
	c2->write(TERMINATE);
}

void get_data(id_stream* from1, id_stream* from2, val_stream* to1, val_stream* to2, ap_uint<256>* buffer) {

	bool enable = false;
	val_t _c_0 = (val_t)0;

	for (; !enable; ){
#pragma HLS PIPELINE II=1
		id_t index1 = from1->read();
		id_t index2 = from2->read();

		enable = (index1 == TERMINATE) && (index2 == TERMINATE);

		if (!enable){
			ap_uint<64> data1 = buffer[index1/4](((index1%4)+1)*64-1, (index1%4)*64);
			ap_uint<64> data2 = buffer[index2/4](((index2%4)+1)*64-1, (index2%4)*64);
			to1->write(Reinterpret<val_t>(static_cast<ap_uint<64> >(data1)));
			to2->write(Reinterpret<val_t>(static_cast<ap_uint<64> >(data2)));
		} else {
			// enable the pipeline
			to1->write(_c_0);
			to2->write(_c_0);
		}
	}
}

void final_data(id_stream* c1, id_stream* c2, id_stream* c3, id_stream* c4, val_stream* from1, val_stream* from2,
		val_stream* to1, val_stream* to2, val_stream* to3, val_stream* to4){

	bool keepInput = false;
	uint32_t cdata1, cdata2, cdata3, cdata4;
	val_t final_data1, final_data2, final_data3, final_data4;
	val_t last_data1, last_data2;

	bool enable = false;
	for (; !enable; ){
#pragma HLS PIPELINE II=1
		val_t data1 = from1->read();
		val_t data2 = from2->read();

		if (!keepInput) {
			cdata1 = c1->read();
			cdata2 = c2->read();

			enable = (cdata1 == TERMINATE) && (cdata2 == TERMINATE);

			if (!enable) {
				cdata3 = c3->read();
				cdata4 = c4->read();

				// if cdata3 is 8, it means that no intra-/inter- reuse and hence two cycles are required to access the four x elements
				if (cdata3 != 8){
					if (cdata1 < 32){
						final_data1 = (cdata1 == 2) ? data1 : data2;
						final_data2 = (cdata2 == 2) ? data1 : data2;
						final_data3 = (cdata3 == 2) ? data1 : data2;
						final_data4 = (cdata4 == 2) ? data1 : data2;

						last_data1 = data1;
						last_data2 = data2;

						to1->write(final_data1);
						to2->write(final_data2);
						to3->write(final_data3);
						to4->write(final_data4);
					} else {
						if (cdata1 == 32 && cdata2 == 64){
							// last_data1, last_data2, data1, data2 and don't update
							to1->write(last_data1);
							to2->write(last_data2);
							to3->write(data1);
							to4->write(data2);
						} else if (cdata1 == 32 && cdata2 == 2 && cdata3 == 2) {
							// last_data1, data1, data1, data2 and update last_data2
							to1->write(last_data1);
							final_data2 = (cdata2 == 2) ? data1 : data2;
							final_data3 = (cdata3 == 2) ? data1 : data2;
							final_data4 = (cdata4 == 2) ? data1 : data2;
							last_data2 = final_data2;

							to2->write(final_data2);
							to3->write(final_data3);
							to4->write(final_data4);
						} else if (cdata1 == 64 && cdata2 == 2 && cdata3 == 2) {
							// last_data2, data1, data1, data2 and update last_data1 and last_data2
							to1->write(last_data2);
							final_data2 = (cdata2 == 2) ? data1 : data2;
							final_data3 = (cdata3 == 2) ? data1 : data2;
							final_data4 = (cdata4 == 2) ? data1 : data2;
							last_data1 = last_data2;
							last_data2 = final_data2;

							to2->write(final_data2);
							to3->write(final_data3);
							to4->write(final_data4);
						} else if (cdata1 == 32 && cdata2 == 2 && cdata3 == 4) {
							// last_data1, data1, data2 and don't update
							to1->write(last_data1);
							final_data2 = (cdata2 == 2) ? data1 : data2;
							final_data3 = (cdata3 == 2) ? data1 : data2;

							to2->write(final_data2);
							to3->write(final_data3);
							to4->write(0.0);
						} else if (cdata1 == 64 && cdata2 == 2 && cdata3 == 4) {
							// last_data2, data1, data2 and update last_data1
							to1->write(last_data2);
							final_data2 = (cdata2 == 2) ? data1 : data2;
							final_data3 = (cdata3 == 2) ? data1 : data2;

							last_data1 = last_data2;

							to2->write(final_data2);
							to3->write(final_data3);
							to4->write(0.0);
						}
					}
				} else {
					final_data1 = (cdata1 == 2) ? data1 : data2;
					final_data2 = (cdata2 == 2) ? data1 : data2;

					to1->write(final_data1);
					to2->write(final_data2);
					keepInput = true;
				}
			}

		} else {
			keepInput = false;
			final_data3 = (cdata3 == 8) ? data1 : data2;
			final_data4 = (cdata4 == 8) ? data1 : data2;

			to3->write(final_data3);
			to4->write(final_data4);
		}
	}
}
