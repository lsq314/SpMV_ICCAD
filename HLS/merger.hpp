#ifndef MERGE
#define MERGE

#include "core.hpp"

void distributor(subresult_stream* from1, data_stream* to0,
        data_stream* to1,
        data_stream* to2, data_stream* to3,
        data_stream* to4, data_stream* to5,
        data_stream* to6, data_stream* to7,
        data_stream* to8, data_stream* to9,
        data_stream* to10, data_stream* to11,
        data_stream* to12, data_stream* to13,
        data_stream* to14, data_stream* to15,
        data_stream* to16, data_stream* to17,
        data_stream* to18, data_stream* to19,
        data_stream* to20, data_stream* to21,
        data_stream* to22, data_stream* to23,
        data_stream* to24, data_stream* to25,
        data_stream* to26, data_stream* to27,
        data_stream* to28, data_stream* to29,
        data_stream* to30, data_stream* to31){

	val_t _c_0 = (val_t)0;

	val_data<val_t> temp_result;
	temp_result.partial_end = false;
	temp_result.final_end = true;
	temp_result.data = _c_0;

	for(bool enable=true; enable; ){
#pragma HLS PIPELINE II=1
		if (!from1->empty()){
			sub_result<val_t> sub1 = from1->read();

			val_data<val_t> result0;
			result0.data = sub1.data;
			result0.final_end = true;
			result0.partial_end = sub1.partial_end;
			id_t index = sub1.rid;

			if (index%32 == 0) {
				to0->write(result0);
				// To avoid the whole engine stalling, we send each merger one sub_result to end the current row batch
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 1) {
				to1->write(result0);
				if (!sub1.partial_end) {
					to0->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 2) {
				to2->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to0->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 3) {
				to3->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to0->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 4) {
				to4->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to0->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 5) {
				to5->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to0->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 6) {
				to6->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to0->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 7) {
				to7->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to0->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 8) {
				to8->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to0->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 9) {
				to9->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to0->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 10) {
				to10->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to0->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 11) {
				to11->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to0->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 12) {
				to12->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to0->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 13) {
				to13->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to0->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 14) {
				to14->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to0->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 15) {
				to15->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to0->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 16) {
				to16->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to0->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 17) {
				to17->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to0->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 18) {
				to18->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to0->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 19) {
				to19->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to0->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 20) {
				to20->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to0->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 21) {
				to21->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to0->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 22) {
				to22->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to0->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 23) {
				to23->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to0->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 24) {
				to24->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to0->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 25) {
				to25->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to0->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 26) {
				to26->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to0->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 27) {
				to27->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to0->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 28) {
				to28->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to0->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 29) {
				to29->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to0->write(temp_result);
					to30->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 30) {
				to30->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to0->write(temp_result);
					to31->write(temp_result);
				}
			}
			else if (index%32 == 31) {
				to31->write(result0);
				if (!sub1.partial_end) {
					to1->write(temp_result);
					to2->write(temp_result);
					to3->write(temp_result);
					to4->write(temp_result);
					to5->write(temp_result);
					to6->write(temp_result);
					to7->write(temp_result);
					to8->write(temp_result);
					to9->write(temp_result);
					to10->write(temp_result);
					to11->write(temp_result);
					to12->write(temp_result);
					to13->write(temp_result);
					to14->write(temp_result);
					to15->write(temp_result);
					to16->write(temp_result);
					to17->write(temp_result);
					to18->write(temp_result);
					to19->write(temp_result);
					to20->write(temp_result);
					to21->write(temp_result);
					to22->write(temp_result);
					to23->write(temp_result);
					to24->write(temp_result);
					to25->write(temp_result);
					to26->write(temp_result);
					to27->write(temp_result);
					to28->write(temp_result);
					to29->write(temp_result);
					to30->write(temp_result);
					to0->write(temp_result);
				}
			}

			enable = sub1.final_end;
		}
	}

	val_data<val_t> result0;
	result0.final_end = false;

	to0->write(result0);
	to1->write(result0);
	to2->write(result0);
	to3->write(result0);
	to4->write(result0);
	to5->write(result0);
	to6->write(result0);
	to7->write(result0);
	to8->write(result0);
	to9->write(result0);
	to10->write(result0);
	to11->write(result0);
	to12->write(result0);
	to13->write(result0);
	to14->write(result0);
	to15->write(result0);
	to16->write(result0);
	to17->write(result0);
	to18->write(result0);
	to19->write(result0);
	to20->write(result0);
	to21->write(result0);
	to22->write(result0);
	to23->write(result0);
	to24->write(result0);
	to25->write(result0);
	to26->write(result0);
	to27->write(result0);
	to28->write(result0);
	to29->write(result0);
	to30->write(result0);
	to31->write(result0);
}

void merger(data_stream* from2, data_stream* from3,
        data_stream* from4, data_stream* from5, data_stream* to1){

	// Note that there are at most four inputs to one merger, we read them in round-robin fashion

	val_t _c_0 = (val_t)0;

	val_data<val_t> tempsub;
	tempsub.partial_end = false;
	tempsub.final_end = true;
	tempsub.data = _c_0;

	bool final_end3 = true;
	bool final_end4 = true;
	bool final_end5 = true;
	bool final_end6 = true;

	bool whether_read3 = false;
	bool whether_read4 = false;
	bool whether_read5 = false;
	bool whether_read6 = false;

	bool have_read3 = false;
	bool have_read4 = false;
	bool have_read5 = false;
	bool have_read6 = false;

	bool partial_end3 = true;
	bool partial_end4 = true;
	bool partial_end5 = true;
	bool partial_end6 = true;

	for (bool enable=true; enable; ){
#pragma HLS PIPELINE II=1

		// if all the partial_ends are received, then begin the new row-batch
		if (!partial_end3 && !partial_end4 && !partial_end5 && !partial_end6){
			partial_end3 = true;
			partial_end4 = true;
			partial_end5 = true;
			partial_end6 = true;
		}

		if ((have_read3 || from2->empty() || !partial_end3) && (have_read4 || from3->empty() || !partial_end4) &&
				(have_read5 || from4->empty() || !partial_end5) && (have_read6 || from5->empty() || !partial_end6)){
			have_read3 = false;
			have_read4 = false;
			have_read5 = false;
			have_read6 = false;
		}

		whether_read3 = !have_read3 && !from2->empty() && partial_end3;
		whether_read4 = !have_read4 && !from3->empty() && partial_end4;
		whether_read5 = !have_read5 && !from4->empty() && partial_end5;
		whether_read6 = !have_read6 && !from5->empty() && partial_end6;

		if (whether_read3) {
			val_data<val_t> temp = from2->read();
			final_end3 = temp.final_end;
			partial_end3 = temp.partial_end;
			if (temp.final_end){
				if (temp.partial_end){
					to1->write(temp);
				} else {
					if (tempsub.data == _c_0) {
						tempsub.data = temp.data;
					}
					if (!partial_end3 && !partial_end4 && !partial_end5 && !partial_end6) {
						to1->write(tempsub);
						tempsub.data = _c_0;
					}
				}
			}
			whether_read3 = false;
			have_read3 = true;
		}
		else if (whether_read4) {
			val_data<val_t> temp = from3->read();
			final_end4 = temp.final_end;
			partial_end4 = temp.partial_end;
			if (temp.final_end){
				if (temp.partial_end){
					to1->write(temp);
				} else {
					if (tempsub.data == _c_0) {
						tempsub.data = temp.data;
					}
					if (!partial_end3 && !partial_end4 && !partial_end5 && !partial_end6) {
						to1->write(tempsub);
						tempsub.data = _c_0;
					}
				}
			}
			whether_read4 = false;
			have_read4 = true;
		}
		else if (whether_read5) {
			val_data<val_t> temp = from4->read();
			final_end5=temp.final_end;
			partial_end5 = temp.partial_end;
			if (temp.final_end){
				if (temp.partial_end){
					to1->write(temp);
				} else {
					if (tempsub.data == _c_0) {
						tempsub.data = temp.data;
					}
					if (!partial_end3 && !partial_end4 && !partial_end5 && !partial_end6) {
						to1->write(tempsub);
						tempsub.data = _c_0;
					}
				}
			}
			whether_read5 = false;
			have_read5 = true;
		}
		else if (whether_read6)  {
			val_data<val_t> temp = from5->read();
			final_end6=temp.final_end;
			partial_end6 = temp.partial_end;
			if (temp.final_end){
				if (temp.partial_end){
					to1->write(temp);
				} else {
					if (tempsub.data == _c_0) {
						tempsub.data = temp.data;
					}
					if (!partial_end3 && !partial_end4 && !partial_end5 && !partial_end6) {
						to1->write(tempsub);
						tempsub.data = _c_0;
					}
				}
			}
			whether_read6 = false;
			have_read6 = true;
		}

		enable = final_end3 || final_end4 || final_end5 || final_end6;
	}

	val_data<val_t> temp;
	temp.final_end = false;
	temp.partial_end = true;

	to1->write(temp);
}

#endif
