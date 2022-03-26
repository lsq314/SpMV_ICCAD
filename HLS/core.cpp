#include "core.hpp"
#include "adder.hpp"
#include "util.hpp"
#include "control.hpp"
#include "merger.hpp"
#include "pe.hpp"

void dataflow_engine(val_t* result, ap_uint<128>* matrix1, ap_uint<128>* matrix2,
                     ap_uint<128>* rid, ap_uint<128>* cid, ap_uint<256>* vector_buffer, id_stream* row_batch_length,
					 uint32_t total_num, uint32_t result_number) {

    id_stream row_index("row_index");
#pragma HLS stream variable=row_index depth=16
    id_stream row_index1("row_index1");
#pragma HLS stream variable=row_index1 depth=16
    id_stream row_index2("row_index2");
#pragma HLS stream variable=row_index2 depth=16
    id_stream row_index3("row_index3");
#pragma HLS stream variable=row_index3 depth=16

    id_stream col_index("col_index");
#pragma HLS stream variable=col_index depth=4
    id_stream col_index1("col_index1");
#pragma HLS stream variable=col_index1 depth=4
    id_stream col_index2("col_index2");
#pragma HLS stream variable=col_index2 depth=4
    id_stream col_index3("col_index3");
#pragma HLS stream variable=col_index3 depth=4

    val_stream col_value("col_value");
#pragma HLS stream variable=col_value depth=4
    val_stream col_value1("col_value1");
#pragma HLS stream variable=col_value1 depth=4
    val_stream col_value2("col_value2");
#pragma HLS stream variable=col_value2 depth=4
    val_stream col_value3("col_value3");
#pragma HLS stream variable=col_value3 depth=4

    data_stream first("first");
#pragma HLS RESOURCE variable=first core=FIFO_LUTRAM
#pragma HLS stream variable=first depth=16
#pragma HLS DATA_PACK variable=first
    data_stream two("two");
#pragma HLS RESOURCE variable=two core=FIFO_LUTRAM
#pragma HLS stream variable=two depth=16
#pragma HLS DATA_PACK variable=two
    data_stream third("third");
#pragma HLS RESOURCE variable=third core=FIFO_LUTRAM
#pragma HLS stream variable=third depth=16
#pragma HLS DATA_PACK variable=third
    data_stream four("four");
#pragma HLS RESOURCE variable=four core=FIFO_LUTRAM
#pragma HLS stream variable=four depth=16
#pragma HLS DATA_PACK variable=four

    id_stream rowlength_sub1("rowlength_sub1");
#pragma HLS stream variable=rowlength_sub1 depth=2
    id_stream rowlength_sub2("rowlength_sub2");
#pragma HLS stream variable=rowlength_sub2 depth=2

    id_stream middle_index1("middle_index1");
#pragma HLS stream variable=middle_index1 depth=8
    id_stream middle_index2("middle_index2");
#pragma HLS stream variable=middle_index2 depth=8

    val_stream middle_data1("middle_data1");
#pragma HLS stream variable=middle_data1 depth=8
    val_stream middle_data2("middle_data2");
#pragma HLS stream variable=middle_data2 depth=8

    id_stream control_stream("control_stream");
#pragma HLS stream variable=control_stream depth=4
    id_stream control_stream1("control_stream1");
#pragma HLS stream variable=control_stream1 depth=4
    id_stream control_stream2("control_stream2");
#pragma HLS stream variable=control_stream2 depth=4
    id_stream control_stream3("control_stream3");
#pragma HLS stream variable=control_stream3 depth=4

    subresult_stream subresult_pe0("subresult_pe0");
#pragma HLS stream variable=subresult_pe0 depth=2
#pragma HLS DATA_PACK variable=subresult_pe0
    subresult_stream subresult_pe1("subresult_pe1");
#pragma HLS stream variable=subresult_pe1 depth=2
#pragma HLS DATA_PACK variable=subresult_pe1
    subresult_stream subresult_pe2("subresult_pe2");
#pragma HLS stream variable=subresult_pe2 depth=2
#pragma HLS DATA_PACK variable=subresult_pe2
    subresult_stream subresult_pe3("subresult_pe3");
#pragma HLS stream variable=subresult_pe3 depth=2
#pragma HLS DATA_PACK variable=subresult_pe3

	data_stream d0_merger0("d0_merger0");
#pragma HLS stream variable=d0_merger0 depth=64
#pragma HLS DATA_PACK variable=d0_merger0
#pragma HLS RESOURCE variable=d0_merger0 core=FIFO_LUTRAM
	data_stream d0_merger1("d0_merger1");
#pragma HLS stream variable=d0_merger1 depth=64
#pragma HLS DATA_PACK variable=d0_merger1
#pragma HLS RESOURCE variable=d0_merger1 core=FIFO_LUTRAM
	data_stream d0_merger2("d0_merger2");
#pragma HLS stream variable=d0_merger2 depth=64
#pragma HLS DATA_PACK variable=d0_merger2
#pragma HLS RESOURCE variable=d0_merger2 core=FIFO_LUTRAM
	data_stream d0_merger3("d0_merger3");
#pragma HLS stream variable=d0_merger3 depth=64
#pragma HLS DATA_PACK variable=d0_merger3
#pragma HLS RESOURCE variable=d0_merger3 core=FIFO_LUTRAM
	data_stream d0_merger4("d0_merger4");
#pragma HLS stream variable=d0_merger4 depth=64
#pragma HLS DATA_PACK variable=d0_merger4
#pragma HLS RESOURCE variable=d0_merger4 core=FIFO_LUTRAM
	data_stream d0_merger5("d0_merger5");
#pragma HLS stream variable=d0_merger5 depth=64
#pragma HLS DATA_PACK variable=d0_merger5
#pragma HLS RESOURCE variable=d0_merger5 core=FIFO_LUTRAM
	data_stream d0_merger6("d0_merger6");
#pragma HLS stream variable=d0_merger6 depth=64
#pragma HLS DATA_PACK variable=d0_merger6
#pragma HLS RESOURCE variable=d0_merger6 core=FIFO_LUTRAM
	data_stream d0_merger7("d0_merger7");
#pragma HLS stream variable=d0_merger7 depth=64
#pragma HLS DATA_PACK variable=d0_merger7
#pragma HLS RESOURCE variable=d0_merger7 core=FIFO_LUTRAM
	data_stream d0_merger8("d0_merger8");
#pragma HLS stream variable=d0_merger8 depth=64
#pragma HLS DATA_PACK variable=d0_merger8
#pragma HLS RESOURCE variable=d0_merger8 core=FIFO_LUTRAM
	data_stream d0_merger9("d0_merger9");
#pragma HLS stream variable=d0_merger9 depth=64
#pragma HLS DATA_PACK variable=d0_merger9
#pragma HLS RESOURCE variable=d0_merger9 core=FIFO_LUTRAM
	data_stream d0_merger10("d0_merger10");
#pragma HLS stream variable=d0_merger10 depth=64
#pragma HLS DATA_PACK variable=d0_merger10
#pragma HLS RESOURCE variable=d0_merger10 core=FIFO_LUTRAM
	data_stream d0_merger11("d0_merger11");
#pragma HLS stream variable=d0_merger11 depth=64
#pragma HLS DATA_PACK variable=d0_merger11
#pragma HLS RESOURCE variable=d0_merger11 core=FIFO_LUTRAM
	data_stream d0_merger12("d0_merger12");
#pragma HLS stream variable=d0_merger12 depth=64
#pragma HLS DATA_PACK variable=d0_merger12
#pragma HLS RESOURCE variable=d0_merger12 core=FIFO_LUTRAM
	data_stream d0_merger13("d0_merger13");
#pragma HLS stream variable=d0_merger13 depth=64
#pragma HLS DATA_PACK variable=d0_merger13
#pragma HLS RESOURCE variable=d0_merger13 core=FIFO_LUTRAM
	data_stream d0_merger14("d0_merger14");
#pragma HLS stream variable=d0_merger14 depth=64
#pragma HLS DATA_PACK variable=d0_merger14
#pragma HLS RESOURCE variable=d0_merger14 core=FIFO_LUTRAM
	data_stream d0_merger15("d0_merger15");
#pragma HLS stream variable=d0_merger15 depth=64
#pragma HLS DATA_PACK variable=d0_merger15
#pragma HLS RESOURCE variable=d0_merger15 core=FIFO_LUTRAM
	data_stream d0_merger16("d0_merger16");
#pragma HLS stream variable=d0_merger16 depth=64
#pragma HLS DATA_PACK variable=d0_merger16
#pragma HLS RESOURCE variable=d0_merger16 core=FIFO_LUTRAM
	data_stream d0_merger17("d0_merger17");
#pragma HLS stream variable=d0_merger17 depth=64
#pragma HLS DATA_PACK variable=d0_merger17
#pragma HLS RESOURCE variable=d0_merger17 core=FIFO_LUTRAM
	data_stream d0_merger18("d0_merger18");
#pragma HLS stream variable=d0_merger18 depth=64
#pragma HLS DATA_PACK variable=d0_merger18
#pragma HLS RESOURCE variable=d0_merger18 core=FIFO_LUTRAM
	data_stream d0_merger19("d0_merger19");
#pragma HLS stream variable=d0_merger19 depth=64
#pragma HLS DATA_PACK variable=d0_merger19
#pragma HLS RESOURCE variable=d0_merger19 core=FIFO_LUTRAM
	data_stream d0_merger20("d0_merger20");
#pragma HLS stream variable=d0_merger20 depth=64
#pragma HLS DATA_PACK variable=d0_merger20
#pragma HLS RESOURCE variable=d0_merger20 core=FIFO_LUTRAM
	data_stream d0_merger21("d0_merger21");
#pragma HLS stream variable=d0_merger21 depth=64
#pragma HLS DATA_PACK variable=d0_merger21
#pragma HLS RESOURCE variable=d0_merger21 core=FIFO_LUTRAM
	data_stream d0_merger22("d0_merger22");
#pragma HLS stream variable=d0_merger22 depth=64
#pragma HLS DATA_PACK variable=d0_merger22
#pragma HLS RESOURCE variable=d0_merger22 core=FIFO_LUTRAM
	data_stream d0_merger23("d0_merger23");
#pragma HLS stream variable=d0_merger23 depth=64
#pragma HLS DATA_PACK variable=d0_merger23
#pragma HLS RESOURCE variable=d0_merger23 core=FIFO_LUTRAM
	data_stream d0_merger24("d0_merger24");
#pragma HLS stream variable=d0_merger24 depth=64
#pragma HLS DATA_PACK variable=d0_merger24
#pragma HLS RESOURCE variable=d0_merger24 core=FIFO_LUTRAM
	data_stream d0_merger25("d0_merger25");
#pragma HLS stream variable=d0_merger25 depth=64
#pragma HLS DATA_PACK variable=d0_merger25
#pragma HLS RESOURCE variable=d0_merger25 core=FIFO_LUTRAM
	data_stream d0_merger26("d0_merger26");
#pragma HLS stream variable=d0_merger26 depth=64
#pragma HLS DATA_PACK variable=d0_merger26
#pragma HLS RESOURCE variable=d0_merger26 core=FIFO_LUTRAM
	data_stream d0_merger27("d0_merger27");
#pragma HLS stream variable=d0_merger27 depth=64
#pragma HLS DATA_PACK variable=d0_merger27
#pragma HLS RESOURCE variable=d0_merger27 core=FIFO_LUTRAM
	data_stream d0_merger28("d0_merger28");
#pragma HLS stream variable=d0_merger28 depth=64
#pragma HLS DATA_PACK variable=d0_merger28
#pragma HLS RESOURCE variable=d0_merger28 core=FIFO_LUTRAM
	data_stream d0_merger29("d0_merger29");
#pragma HLS stream variable=d0_merger29 depth=64
#pragma HLS DATA_PACK variable=d0_merger29
#pragma HLS RESOURCE variable=d0_merger29 core=FIFO_LUTRAM
	data_stream d0_merger30("d0_merger30");
#pragma HLS stream variable=d0_merger30 depth=64
#pragma HLS DATA_PACK variable=d0_merger30
#pragma HLS RESOURCE variable=d0_merger30 core=FIFO_LUTRAM
	data_stream d0_merger31("d0_merger31");
#pragma HLS stream variable=d0_merger31 depth=64
#pragma HLS DATA_PACK variable=d0_merger31
#pragma HLS RESOURCE variable=d0_merger31 core=FIFO_LUTRAM
	data_stream d1_merger0("d1_merger0");
#pragma HLS stream variable=d1_merger0 depth=64
#pragma HLS DATA_PACK variable=d1_merger0
#pragma HLS RESOURCE variable=d1_merger0 core=FIFO_LUTRAM
	data_stream d1_merger1("d1_merger1");
#pragma HLS stream variable=d1_merger1 depth=64
#pragma HLS DATA_PACK variable=d1_merger1
#pragma HLS RESOURCE variable=d1_merger1 core=FIFO_LUTRAM
	data_stream d1_merger2("d1_merger2");
#pragma HLS stream variable=d1_merger2 depth=64
#pragma HLS DATA_PACK variable=d1_merger2
#pragma HLS RESOURCE variable=d1_merger2 core=FIFO_LUTRAM
	data_stream d1_merger3("d1_merger3");
#pragma HLS stream variable=d1_merger3 depth=64
#pragma HLS DATA_PACK variable=d1_merger3
#pragma HLS RESOURCE variable=d1_merger3 core=FIFO_LUTRAM
	data_stream d1_merger4("d1_merger4");
#pragma HLS stream variable=d1_merger4 depth=64
#pragma HLS DATA_PACK variable=d1_merger4
#pragma HLS RESOURCE variable=d1_merger4 core=FIFO_LUTRAM
	data_stream d1_merger5("d1_merger5");
#pragma HLS stream variable=d1_merger5 depth=64
#pragma HLS DATA_PACK variable=d1_merger5
#pragma HLS RESOURCE variable=d1_merger5 core=FIFO_LUTRAM
	data_stream d1_merger6("d1_merger6");
#pragma HLS stream variable=d1_merger6 depth=64
#pragma HLS DATA_PACK variable=d1_merger6
#pragma HLS RESOURCE variable=d1_merger6 core=FIFO_LUTRAM
	data_stream d1_merger7("d1_merger7");
#pragma HLS stream variable=d1_merger7 depth=64
#pragma HLS DATA_PACK variable=d1_merger7
#pragma HLS RESOURCE variable=d1_merger7 core=FIFO_LUTRAM
	data_stream d1_merger8("d1_merger8");
#pragma HLS stream variable=d1_merger8 depth=64
#pragma HLS DATA_PACK variable=d1_merger8
#pragma HLS RESOURCE variable=d1_merger8 core=FIFO_LUTRAM
	data_stream d1_merger9("d1_merger9");
#pragma HLS stream variable=d1_merger9 depth=64
#pragma HLS DATA_PACK variable=d1_merger9
#pragma HLS RESOURCE variable=d1_merger9 core=FIFO_LUTRAM
	data_stream d1_merger10("d1_merger10");
#pragma HLS stream variable=d1_merger10 depth=64
#pragma HLS DATA_PACK variable=d1_merger10
#pragma HLS RESOURCE variable=d1_merger10 core=FIFO_LUTRAM
	data_stream d1_merger11("d1_merger11");
#pragma HLS stream variable=d1_merger11 depth=64
#pragma HLS DATA_PACK variable=d1_merger11
#pragma HLS RESOURCE variable=d1_merger11 core=FIFO_LUTRAM
	data_stream d1_merger12("d1_merger12");
#pragma HLS stream variable=d1_merger12 depth=64
#pragma HLS DATA_PACK variable=d1_merger12
#pragma HLS RESOURCE variable=d1_merger12 core=FIFO_LUTRAM
	data_stream d1_merger13("d1_merger13");
#pragma HLS stream variable=d1_merger13 depth=64
#pragma HLS DATA_PACK variable=d1_merger13
#pragma HLS RESOURCE variable=d1_merger13 core=FIFO_LUTRAM
	data_stream d1_merger14("d1_merger14");
#pragma HLS stream variable=d1_merger14 depth=64
#pragma HLS DATA_PACK variable=d1_merger14
#pragma HLS RESOURCE variable=d1_merger14 core=FIFO_LUTRAM
	data_stream d1_merger15("d1_merger15");
#pragma HLS stream variable=d1_merger15 depth=64
#pragma HLS DATA_PACK variable=d1_merger15
#pragma HLS RESOURCE variable=d1_merger15 core=FIFO_LUTRAM
	data_stream d1_merger16("d1_merger16");
#pragma HLS stream variable=d1_merger16 depth=64
#pragma HLS DATA_PACK variable=d1_merger16
#pragma HLS RESOURCE variable=d1_merger16 core=FIFO_LUTRAM
	data_stream d1_merger17("d1_merger17");
#pragma HLS stream variable=d1_merger17 depth=64
#pragma HLS DATA_PACK variable=d1_merger17
#pragma HLS RESOURCE variable=d1_merger17 core=FIFO_LUTRAM
	data_stream d1_merger18("d1_merger18");
#pragma HLS stream variable=d1_merger18 depth=64
#pragma HLS DATA_PACK variable=d1_merger18
#pragma HLS RESOURCE variable=d1_merger18 core=FIFO_LUTRAM
	data_stream d1_merger19("d1_merger19");
#pragma HLS stream variable=d1_merger19 depth=64
#pragma HLS DATA_PACK variable=d1_merger19
#pragma HLS RESOURCE variable=d1_merger19 core=FIFO_LUTRAM
	data_stream d1_merger20("d1_merger20");
#pragma HLS stream variable=d1_merger20 depth=64
#pragma HLS DATA_PACK variable=d1_merger20
#pragma HLS RESOURCE variable=d1_merger20 core=FIFO_LUTRAM
	data_stream d1_merger21("d1_merger21");
#pragma HLS stream variable=d1_merger21 depth=64
#pragma HLS DATA_PACK variable=d1_merger21
#pragma HLS RESOURCE variable=d1_merger21 core=FIFO_LUTRAM
	data_stream d1_merger22("d1_merger22");
#pragma HLS stream variable=d1_merger22 depth=64
#pragma HLS DATA_PACK variable=d1_merger22
#pragma HLS RESOURCE variable=d1_merger22 core=FIFO_LUTRAM
	data_stream d1_merger23("d1_merger23");
#pragma HLS stream variable=d1_merger23 depth=64
#pragma HLS DATA_PACK variable=d1_merger23
#pragma HLS RESOURCE variable=d1_merger23 core=FIFO_LUTRAM
	data_stream d1_merger24("d1_merger24");
#pragma HLS stream variable=d1_merger24 depth=64
#pragma HLS DATA_PACK variable=d1_merger24
#pragma HLS RESOURCE variable=d1_merger24 core=FIFO_LUTRAM
	data_stream d1_merger25("d1_merger25");
#pragma HLS stream variable=d1_merger25 depth=64
#pragma HLS DATA_PACK variable=d1_merger25
#pragma HLS RESOURCE variable=d1_merger25 core=FIFO_LUTRAM
	data_stream d1_merger26("d1_merger26");
#pragma HLS stream variable=d1_merger26 depth=64
#pragma HLS DATA_PACK variable=d1_merger26
#pragma HLS RESOURCE variable=d1_merger26 core=FIFO_LUTRAM
	data_stream d1_merger27("d1_merger27");
#pragma HLS stream variable=d1_merger27 depth=64
#pragma HLS DATA_PACK variable=d1_merger27
#pragma HLS RESOURCE variable=d1_merger27 core=FIFO_LUTRAM
	data_stream d1_merger28("d1_merger28");
#pragma HLS stream variable=d1_merger28 depth=64
#pragma HLS DATA_PACK variable=d1_merger28
#pragma HLS RESOURCE variable=d1_merger28 core=FIFO_LUTRAM
	data_stream d1_merger29("d1_merger29");
#pragma HLS stream variable=d1_merger29 depth=64
#pragma HLS DATA_PACK variable=d1_merger29
#pragma HLS RESOURCE variable=d1_merger29 core=FIFO_LUTRAM
	data_stream d1_merger30("d1_merger30");
#pragma HLS stream variable=d1_merger30 depth=64
#pragma HLS DATA_PACK variable=d1_merger30
#pragma HLS RESOURCE variable=d1_merger30 core=FIFO_LUTRAM
	data_stream d1_merger31("d1_merger31");
#pragma HLS stream variable=d1_merger31 depth=64
#pragma HLS DATA_PACK variable=d1_merger31
#pragma HLS RESOURCE variable=d1_merger31 core=FIFO_LUTRAM
	data_stream d2_merger0("d2_merger0");
#pragma HLS stream variable=d2_merger0 depth=64
#pragma HLS DATA_PACK variable=d2_merger0
#pragma HLS RESOURCE variable=d2_merger0 core=FIFO_LUTRAM
	data_stream d2_merger1("d2_merger1");
#pragma HLS stream variable=d2_merger1 depth=64
#pragma HLS DATA_PACK variable=d2_merger1
#pragma HLS RESOURCE variable=d2_merger1 core=FIFO_LUTRAM
	data_stream d2_merger2("d2_merger2");
#pragma HLS stream variable=d2_merger2 depth=64
#pragma HLS DATA_PACK variable=d2_merger2
#pragma HLS RESOURCE variable=d2_merger2 core=FIFO_LUTRAM
	data_stream d2_merger3("d2_merger3");
#pragma HLS stream variable=d2_merger3 depth=64
#pragma HLS DATA_PACK variable=d2_merger3
#pragma HLS RESOURCE variable=d2_merger3 core=FIFO_LUTRAM
	data_stream d2_merger4("d2_merger4");
#pragma HLS stream variable=d2_merger4 depth=64
#pragma HLS DATA_PACK variable=d2_merger4
#pragma HLS RESOURCE variable=d2_merger4 core=FIFO_LUTRAM
	data_stream d2_merger5("d2_merger5");
#pragma HLS stream variable=d2_merger5 depth=64
#pragma HLS DATA_PACK variable=d2_merger5
#pragma HLS RESOURCE variable=d2_merger5 core=FIFO_LUTRAM
	data_stream d2_merger6("d2_merger6");
#pragma HLS stream variable=d2_merger6 depth=64
#pragma HLS DATA_PACK variable=d2_merger6
#pragma HLS RESOURCE variable=d2_merger6 core=FIFO_LUTRAM
	data_stream d2_merger7("d2_merger7");
#pragma HLS stream variable=d2_merger7 depth=64
#pragma HLS DATA_PACK variable=d2_merger7
#pragma HLS RESOURCE variable=d2_merger7 core=FIFO_LUTRAM
	data_stream d2_merger8("d2_merger8");
#pragma HLS stream variable=d2_merger8 depth=64
#pragma HLS DATA_PACK variable=d2_merger8
#pragma HLS RESOURCE variable=d2_merger8 core=FIFO_LUTRAM
	data_stream d2_merger9("d2_merger9");
#pragma HLS stream variable=d2_merger9 depth=64
#pragma HLS DATA_PACK variable=d2_merger9
#pragma HLS RESOURCE variable=d2_merger9 core=FIFO_LUTRAM
	data_stream d2_merger10("d2_merger10");
#pragma HLS stream variable=d2_merger10 depth=64
#pragma HLS DATA_PACK variable=d2_merger10
#pragma HLS RESOURCE variable=d2_merger10 core=FIFO_LUTRAM
	data_stream d2_merger11("d2_merger11");
#pragma HLS stream variable=d2_merger11 depth=64
#pragma HLS DATA_PACK variable=d2_merger11
#pragma HLS RESOURCE variable=d2_merger11 core=FIFO_LUTRAM
	data_stream d2_merger12("d2_merger12");
#pragma HLS stream variable=d2_merger12 depth=64
#pragma HLS DATA_PACK variable=d2_merger12
#pragma HLS RESOURCE variable=d2_merger12 core=FIFO_LUTRAM
	data_stream d2_merger13("d2_merger13");
#pragma HLS stream variable=d2_merger13 depth=64
#pragma HLS DATA_PACK variable=d2_merger13
#pragma HLS RESOURCE variable=d2_merger13 core=FIFO_LUTRAM
	data_stream d2_merger14("d2_merger14");
#pragma HLS stream variable=d2_merger14 depth=64
#pragma HLS DATA_PACK variable=d2_merger14
#pragma HLS RESOURCE variable=d2_merger14 core=FIFO_LUTRAM
	data_stream d2_merger15("d2_merger15");
#pragma HLS stream variable=d2_merger15 depth=64
#pragma HLS DATA_PACK variable=d2_merger15
#pragma HLS RESOURCE variable=d2_merger15 core=FIFO_LUTRAM
	data_stream d2_merger16("d2_merger16");
#pragma HLS stream variable=d2_merger16 depth=64
#pragma HLS DATA_PACK variable=d2_merger16
#pragma HLS RESOURCE variable=d2_merger16 core=FIFO_LUTRAM
	data_stream d2_merger17("d2_merger17");
#pragma HLS stream variable=d2_merger17 depth=64
#pragma HLS DATA_PACK variable=d2_merger17
#pragma HLS RESOURCE variable=d2_merger17 core=FIFO_LUTRAM
	data_stream d2_merger18("d2_merger18");
#pragma HLS stream variable=d2_merger18 depth=64
#pragma HLS DATA_PACK variable=d2_merger18
#pragma HLS RESOURCE variable=d2_merger18 core=FIFO_LUTRAM
	data_stream d2_merger19("d2_merger19");
#pragma HLS stream variable=d2_merger19 depth=64
#pragma HLS DATA_PACK variable=d2_merger19
#pragma HLS RESOURCE variable=d2_merger19 core=FIFO_LUTRAM
	data_stream d2_merger20("d2_merger20");
#pragma HLS stream variable=d2_merger20 depth=64
#pragma HLS DATA_PACK variable=d2_merger20
#pragma HLS RESOURCE variable=d2_merger20 core=FIFO_LUTRAM
	data_stream d2_merger21("d2_merger21");
#pragma HLS stream variable=d2_merger21 depth=64
#pragma HLS DATA_PACK variable=d2_merger21
#pragma HLS RESOURCE variable=d2_merger21 core=FIFO_LUTRAM
	data_stream d2_merger22("d2_merger22");
#pragma HLS stream variable=d2_merger22 depth=64
#pragma HLS DATA_PACK variable=d2_merger22
#pragma HLS RESOURCE variable=d2_merger22 core=FIFO_LUTRAM
	data_stream d2_merger23("d2_merger23");
#pragma HLS stream variable=d2_merger23 depth=64
#pragma HLS DATA_PACK variable=d2_merger23
#pragma HLS RESOURCE variable=d2_merger23 core=FIFO_LUTRAM
	data_stream d2_merger24("d2_merger24");
#pragma HLS stream variable=d2_merger24 depth=64
#pragma HLS DATA_PACK variable=d2_merger24
#pragma HLS RESOURCE variable=d2_merger24 core=FIFO_LUTRAM
	data_stream d2_merger25("d2_merger25");
#pragma HLS stream variable=d2_merger25 depth=64
#pragma HLS DATA_PACK variable=d2_merger25
#pragma HLS RESOURCE variable=d2_merger25 core=FIFO_LUTRAM
	data_stream d2_merger26("d2_merger26");
#pragma HLS stream variable=d2_merger26 depth=64
#pragma HLS DATA_PACK variable=d2_merger26
#pragma HLS RESOURCE variable=d2_merger26 core=FIFO_LUTRAM
	data_stream d2_merger27("d2_merger27");
#pragma HLS stream variable=d2_merger27 depth=64
#pragma HLS DATA_PACK variable=d2_merger27
#pragma HLS RESOURCE variable=d2_merger27 core=FIFO_LUTRAM
	data_stream d2_merger28("d2_merger28");
#pragma HLS stream variable=d2_merger28 depth=64
#pragma HLS DATA_PACK variable=d2_merger28
#pragma HLS RESOURCE variable=d2_merger28 core=FIFO_LUTRAM
	data_stream d2_merger29("d2_merger29");
#pragma HLS stream variable=d2_merger29 depth=64
#pragma HLS DATA_PACK variable=d2_merger29
#pragma HLS RESOURCE variable=d2_merger29 core=FIFO_LUTRAM
	data_stream d2_merger30("d2_merger30");
#pragma HLS stream variable=d2_merger30 depth=64
#pragma HLS DATA_PACK variable=d2_merger30
#pragma HLS RESOURCE variable=d2_merger30 core=FIFO_LUTRAM
	data_stream d2_merger31("d2_merger31");
#pragma HLS stream variable=d2_merger31 depth=64
#pragma HLS DATA_PACK variable=d2_merger31
#pragma HLS RESOURCE variable=d2_merger31 core=FIFO_LUTRAM
	data_stream d3_merger0("d3_merger0");
#pragma HLS stream variable=d3_merger0 depth=64
#pragma HLS DATA_PACK variable=d3_merger0
#pragma HLS RESOURCE variable=d3_merger0 core=FIFO_LUTRAM
	data_stream d3_merger1("d3_merger1");
#pragma HLS stream variable=d3_merger1 depth=64
#pragma HLS DATA_PACK variable=d3_merger1
#pragma HLS RESOURCE variable=d3_merger1 core=FIFO_LUTRAM
	data_stream d3_merger2("d3_merger2");
#pragma HLS stream variable=d3_merger2 depth=64
#pragma HLS DATA_PACK variable=d3_merger2
#pragma HLS RESOURCE variable=d3_merger2 core=FIFO_LUTRAM
	data_stream d3_merger3("d3_merger3");
#pragma HLS stream variable=d3_merger3 depth=64
#pragma HLS DATA_PACK variable=d3_merger3
#pragma HLS RESOURCE variable=d3_merger3 core=FIFO_LUTRAM
	data_stream d3_merger4("d3_merger4");
#pragma HLS stream variable=d3_merger4 depth=64
#pragma HLS DATA_PACK variable=d3_merger4
#pragma HLS RESOURCE variable=d3_merger4 core=FIFO_LUTRAM
	data_stream d3_merger5("d3_merger5");
#pragma HLS stream variable=d3_merger5 depth=64
#pragma HLS DATA_PACK variable=d3_merger5
#pragma HLS RESOURCE variable=d3_merger5 core=FIFO_LUTRAM
	data_stream d3_merger6("d3_merger6");
#pragma HLS stream variable=d3_merger6 depth=64
#pragma HLS DATA_PACK variable=d3_merger6
#pragma HLS RESOURCE variable=d3_merger6 core=FIFO_LUTRAM
	data_stream d3_merger7("d3_merger7");
#pragma HLS stream variable=d3_merger7 depth=64
#pragma HLS DATA_PACK variable=d3_merger7
#pragma HLS RESOURCE variable=d3_merger7 core=FIFO_LUTRAM
	data_stream d3_merger8("d3_merger8");
#pragma HLS stream variable=d3_merger8 depth=64
#pragma HLS DATA_PACK variable=d3_merger8
#pragma HLS RESOURCE variable=d3_merger8 core=FIFO_LUTRAM
	data_stream d3_merger9("d3_merger9");
#pragma HLS stream variable=d3_merger9 depth=64
#pragma HLS DATA_PACK variable=d3_merger9
#pragma HLS RESOURCE variable=d3_merger9 core=FIFO_LUTRAM
	data_stream d3_merger10("d3_merger10");
#pragma HLS stream variable=d3_merger10 depth=64
#pragma HLS DATA_PACK variable=d3_merger10
#pragma HLS RESOURCE variable=d3_merger10 core=FIFO_LUTRAM
	data_stream d3_merger11("d3_merger11");
#pragma HLS stream variable=d3_merger11 depth=64
#pragma HLS DATA_PACK variable=d3_merger11
#pragma HLS RESOURCE variable=d3_merger11 core=FIFO_LUTRAM
	data_stream d3_merger12("d3_merger12");
#pragma HLS stream variable=d3_merger12 depth=64
#pragma HLS DATA_PACK variable=d3_merger12
#pragma HLS RESOURCE variable=d3_merger12 core=FIFO_LUTRAM
	data_stream d3_merger13("d3_merger13");
#pragma HLS stream variable=d3_merger13 depth=64
#pragma HLS DATA_PACK variable=d3_merger13
#pragma HLS RESOURCE variable=d3_merger13 core=FIFO_LUTRAM
	data_stream d3_merger14("d3_merger14");
#pragma HLS stream variable=d3_merger14 depth=64
#pragma HLS DATA_PACK variable=d3_merger14
#pragma HLS RESOURCE variable=d3_merger14 core=FIFO_LUTRAM
	data_stream d3_merger15("d3_merger15");
#pragma HLS stream variable=d3_merger15 depth=64
#pragma HLS DATA_PACK variable=d3_merger15
#pragma HLS RESOURCE variable=d3_merger15 core=FIFO_LUTRAM
	data_stream d3_merger16("d3_merger16");
#pragma HLS stream variable=d3_merger16 depth=64
#pragma HLS DATA_PACK variable=d3_merger16
#pragma HLS RESOURCE variable=d3_merger16 core=FIFO_LUTRAM
	data_stream d3_merger17("d3_merger17");
#pragma HLS stream variable=d3_merger17 depth=64
#pragma HLS DATA_PACK variable=d3_merger17
#pragma HLS RESOURCE variable=d3_merger17 core=FIFO_LUTRAM
	data_stream d3_merger18("d3_merger18");
#pragma HLS stream variable=d3_merger18 depth=64
#pragma HLS DATA_PACK variable=d3_merger18
#pragma HLS RESOURCE variable=d3_merger18 core=FIFO_LUTRAM
	data_stream d3_merger19("d3_merger19");
#pragma HLS stream variable=d3_merger19 depth=64
#pragma HLS DATA_PACK variable=d3_merger19
#pragma HLS RESOURCE variable=d3_merger19 core=FIFO_LUTRAM
	data_stream d3_merger20("d3_merger20");
#pragma HLS stream variable=d3_merger20 depth=64
#pragma HLS DATA_PACK variable=d3_merger20
#pragma HLS RESOURCE variable=d3_merger20 core=FIFO_LUTRAM
	data_stream d3_merger21("d3_merger21");
#pragma HLS stream variable=d3_merger21 depth=64
#pragma HLS DATA_PACK variable=d3_merger21
#pragma HLS RESOURCE variable=d3_merger21 core=FIFO_LUTRAM
	data_stream d3_merger22("d3_merger22");
#pragma HLS stream variable=d3_merger22 depth=64
#pragma HLS DATA_PACK variable=d3_merger22
#pragma HLS RESOURCE variable=d3_merger22 core=FIFO_LUTRAM
	data_stream d3_merger23("d3_merger23");
#pragma HLS stream variable=d3_merger23 depth=64
#pragma HLS DATA_PACK variable=d3_merger23
#pragma HLS RESOURCE variable=d3_merger23 core=FIFO_LUTRAM
	data_stream d3_merger24("d3_merger24");
#pragma HLS stream variable=d3_merger24 depth=64
#pragma HLS DATA_PACK variable=d3_merger24
#pragma HLS RESOURCE variable=d3_merger24 core=FIFO_LUTRAM
	data_stream d3_merger25("d3_merger25");
#pragma HLS stream variable=d3_merger25 depth=64
#pragma HLS DATA_PACK variable=d3_merger25
#pragma HLS RESOURCE variable=d3_merger25 core=FIFO_LUTRAM
	data_stream d3_merger26("d3_merger26");
#pragma HLS stream variable=d3_merger26 depth=64
#pragma HLS DATA_PACK variable=d3_merger26
#pragma HLS RESOURCE variable=d3_merger26 core=FIFO_LUTRAM
	data_stream d3_merger27("d3_merger27");
#pragma HLS stream variable=d3_merger27 depth=64
#pragma HLS DATA_PACK variable=d3_merger27
#pragma HLS RESOURCE variable=d3_merger27 core=FIFO_LUTRAM
	data_stream d3_merger28("d3_merger28");
#pragma HLS stream variable=d3_merger28 depth=64
#pragma HLS DATA_PACK variable=d3_merger28
#pragma HLS RESOURCE variable=d3_merger28 core=FIFO_LUTRAM
	data_stream d3_merger29("d3_merger29");
#pragma HLS stream variable=d3_merger29 depth=64
#pragma HLS DATA_PACK variable=d3_merger29
#pragma HLS RESOURCE variable=d3_merger29 core=FIFO_LUTRAM
	data_stream d3_merger30("d3_merger30");
#pragma HLS stream variable=d3_merger30 depth=64
#pragma HLS DATA_PACK variable=d3_merger30
#pragma HLS RESOURCE variable=d3_merger30 core=FIFO_LUTRAM
	data_stream d3_merger31("d3_merger31");
#pragma HLS stream variable=d3_merger31 depth=64
#pragma HLS DATA_PACK variable=d3_merger31
#pragma HLS RESOURCE variable=d3_merger31 core=FIFO_LUTRAM

	data_stream merger_to_final0("merger_to_final0");
#pragma HLS stream variable=merger_to_final0 depth=2
#pragma HLS DATA_PACK variable=merger_to_final0
	data_stream merger_to_final1("merger_to_final1");
#pragma HLS stream variable=merger_to_final1 depth=2
#pragma HLS DATA_PACK variable=merger_to_final1
	data_stream merger_to_final2("merger_to_final2");
#pragma HLS stream variable=merger_to_final2 depth=2
#pragma HLS DATA_PACK variable=merger_to_final2
	data_stream merger_to_final3("merger_to_final3");
#pragma HLS stream variable=merger_to_final3 depth=2
#pragma HLS DATA_PACK variable=merger_to_final3
	data_stream merger_to_final4("merger_to_final4");
#pragma HLS stream variable=merger_to_final4 depth=2
#pragma HLS DATA_PACK variable=merger_to_final4
	data_stream merger_to_final5("merger_to_final5");
#pragma HLS stream variable=merger_to_final5 depth=2
#pragma HLS DATA_PACK variable=merger_to_final5
	data_stream merger_to_final6("merger_to_final6");
#pragma HLS stream variable=merger_to_final6 depth=2
#pragma HLS DATA_PACK variable=merger_to_final6
	data_stream merger_to_final7("merger_to_final7");
#pragma HLS stream variable=merger_to_final7 depth=2
#pragma HLS DATA_PACK variable=merger_to_final7
	data_stream merger_to_final8("merger_to_final8");
#pragma HLS stream variable=merger_to_final8 depth=2
#pragma HLS DATA_PACK variable=merger_to_final8
	data_stream merger_to_final9("merger_to_final9");
#pragma HLS stream variable=merger_to_final9 depth=2
#pragma HLS DATA_PACK variable=merger_to_final9
	data_stream merger_to_final10("merger_to_final10");
#pragma HLS stream variable=merger_to_final10 depth=2
#pragma HLS DATA_PACK variable=merger_to_final10
	data_stream merger_to_final11("merger_to_final11");
#pragma HLS stream variable=merger_to_final11 depth=2
#pragma HLS DATA_PACK variable=merger_to_final11
	data_stream merger_to_final12("merger_to_final12");
#pragma HLS stream variable=merger_to_final12 depth=2
#pragma HLS DATA_PACK variable=merger_to_final12
	data_stream merger_to_final13("merger_to_final13");
#pragma HLS stream variable=merger_to_final13 depth=2
#pragma HLS DATA_PACK variable=merger_to_final13
	data_stream merger_to_final14("merger_to_final14");
#pragma HLS stream variable=merger_to_final14 depth=2
#pragma HLS DATA_PACK variable=merger_to_final14
	data_stream merger_to_final15("merger_to_final15");
#pragma HLS stream variable=merger_to_final15 depth=2
#pragma HLS DATA_PACK variable=merger_to_final15
	data_stream merger_to_final16("merger_to_final16");
#pragma HLS stream variable=merger_to_final16 depth=2
#pragma HLS DATA_PACK variable=merger_to_final16
	data_stream merger_to_final17("merger_to_final17");
#pragma HLS stream variable=merger_to_final17 depth=2
#pragma HLS DATA_PACK variable=merger_to_final17
	data_stream merger_to_final18("merger_to_final18");
#pragma HLS stream variable=merger_to_final18 depth=2
#pragma HLS DATA_PACK variable=merger_to_final18
	data_stream merger_to_final19("merger_to_final19");
#pragma HLS stream variable=merger_to_final19 depth=2
#pragma HLS DATA_PACK variable=merger_to_final19
	data_stream merger_to_final20("merger_to_final20");
#pragma HLS stream variable=merger_to_final20 depth=2
#pragma HLS DATA_PACK variable=merger_to_final20
	data_stream merger_to_final21("merger_to_final21");
#pragma HLS stream variable=merger_to_final21 depth=2
#pragma HLS DATA_PACK variable=merger_to_final21
	data_stream merger_to_final22("merger_to_final22");
#pragma HLS stream variable=merger_to_final22 depth=2
#pragma HLS DATA_PACK variable=merger_to_final22
	data_stream merger_to_final23("merger_to_final23");
#pragma HLS stream variable=merger_to_final23 depth=2
#pragma HLS DATA_PACK variable=merger_to_final23
	data_stream merger_to_final24("merger_to_final24");
#pragma HLS stream variable=merger_to_final24 depth=2
#pragma HLS DATA_PACK variable=merger_to_final24
	data_stream merger_to_final25("merger_to_final25");
#pragma HLS stream variable=merger_to_final25 depth=2
#pragma HLS DATA_PACK variable=merger_to_final25
	data_stream merger_to_final26("merger_to_final26");
#pragma HLS stream variable=merger_to_final26 depth=2
#pragma HLS DATA_PACK variable=merger_to_final26
	data_stream merger_to_final27("merger_to_final27");
#pragma HLS stream variable=merger_to_final27 depth=2
#pragma HLS DATA_PACK variable=merger_to_final27
	data_stream merger_to_final28("merger_to_final28");
#pragma HLS stream variable=merger_to_final28 depth=2
#pragma HLS DATA_PACK variable=merger_to_final28
	data_stream merger_to_final29("merger_to_final29");
#pragma HLS stream variable=merger_to_final29 depth=2
#pragma HLS DATA_PACK variable=merger_to_final29
	data_stream merger_to_final30("merger_to_final30");
#pragma HLS stream variable=merger_to_final30 depth=2
#pragma HLS DATA_PACK variable=merger_to_final30
	data_stream merger_to_final31("merger_to_final31");
#pragma HLS stream variable=merger_to_final31 depth=2
#pragma HLS DATA_PACK variable=merger_to_final31

	data_stream adder_inter0("adder_inter0");
#pragma HLS stream variable=adder_inter0 depth=6
#pragma HLS DATA_PACK variable=adder_inter0
	data_stream adder_inter1("adder_inter1");
#pragma HLS stream variable=adder_inter1 depth=6
#pragma HLS DATA_PACK variable=adder_inter1
	data_stream adder_inter2("adder_inter2");
#pragma HLS stream variable=adder_inter2 depth=6
#pragma HLS DATA_PACK variable=adder_inter2
	data_stream adder_inter3("adder_inter3");
#pragma HLS stream variable=adder_inter3 depth=6
#pragma HLS DATA_PACK variable=adder_inter3
	data_stream adder_inter4("adder_inter4");
#pragma HLS stream variable=adder_inter4 depth=6
#pragma HLS DATA_PACK variable=adder_inter4
	data_stream adder_inter5("adder_inter5");
#pragma HLS stream variable=adder_inter5 depth=6
#pragma HLS DATA_PACK variable=adder_inter5
	data_stream adder_inter6("adder_inter6");
#pragma HLS stream variable=adder_inter6 depth=6
#pragma HLS DATA_PACK variable=adder_inter6
	data_stream adder_inter7("adder_inter7");
#pragma HLS stream variable=adder_inter7 depth=6
#pragma HLS DATA_PACK variable=adder_inter7
	data_stream adder_inter8("adder_inter8");
#pragma HLS stream variable=adder_inter8 depth=6
#pragma HLS DATA_PACK variable=adder_inter8
	data_stream adder_inter9("adder_inter9");
#pragma HLS stream variable=adder_inter9 depth=6
#pragma HLS DATA_PACK variable=adder_inter9
	data_stream adder_inter10("adder_inter10");
#pragma HLS stream variable=adder_inter10 depth=6
#pragma HLS DATA_PACK variable=adder_inter10
	data_stream adder_inter11("adder_inter11");
#pragma HLS stream variable=adder_inter11 depth=6
#pragma HLS DATA_PACK variable=adder_inter11
	data_stream adder_inter12("adder_inter12");
#pragma HLS stream variable=adder_inter12 depth=6
#pragma HLS DATA_PACK variable=adder_inter12
	data_stream adder_inter13("adder_inter13");
#pragma HLS stream variable=adder_inter13 depth=6
#pragma HLS DATA_PACK variable=adder_inter13
	data_stream adder_inter14("adder_inter14");
#pragma HLS stream variable=adder_inter14 depth=6
#pragma HLS DATA_PACK variable=adder_inter14
	data_stream adder_inter15("adder_inter15");
#pragma HLS stream variable=adder_inter15 depth=6
#pragma HLS DATA_PACK variable=adder_inter15
	data_stream adder_inter16("adder_inter16");
#pragma HLS stream variable=adder_inter16 depth=6
#pragma HLS DATA_PACK variable=adder_inter16
	data_stream adder_inter17("adder_inter17");
#pragma HLS stream variable=adder_inter17 depth=6
#pragma HLS DATA_PACK variable=adder_inter17
	data_stream adder_inter18("adder_inter18");
#pragma HLS stream variable=adder_inter18 depth=6
#pragma HLS DATA_PACK variable=adder_inter18
	data_stream adder_inter19("adder_inter19");
#pragma HLS stream variable=adder_inter19 depth=6
#pragma HLS DATA_PACK variable=adder_inter19
	data_stream adder_inter20("adder_inter20");
#pragma HLS stream variable=adder_inter20 depth=6
#pragma HLS DATA_PACK variable=adder_inter20
	data_stream adder_inter21("adder_inter21");
#pragma HLS stream variable=adder_inter21 depth=6
#pragma HLS DATA_PACK variable=adder_inter21
	data_stream adder_inter22("adder_inter22");
#pragma HLS stream variable=adder_inter22 depth=6
#pragma HLS DATA_PACK variable=adder_inter22
	data_stream adder_inter23("adder_inter23");
#pragma HLS stream variable=adder_inter23 depth=6
#pragma HLS DATA_PACK variable=adder_inter23
	data_stream adder_inter24("adder_inter24");
#pragma HLS stream variable=adder_inter24 depth=6
#pragma HLS DATA_PACK variable=adder_inter24
	data_stream adder_inter25("adder_inter25");
#pragma HLS stream variable=adder_inter25 depth=6
#pragma HLS DATA_PACK variable=adder_inter25
	data_stream adder_inter26("adder_inter26");
#pragma HLS stream variable=adder_inter26 depth=6
#pragma HLS DATA_PACK variable=adder_inter26
	data_stream adder_inter27("adder_inter27");
#pragma HLS stream variable=adder_inter27 depth=6
#pragma HLS DATA_PACK variable=adder_inter27
	data_stream adder_inter28("adder_inter28");
#pragma HLS stream variable=adder_inter28 depth=6
#pragma HLS DATA_PACK variable=adder_inter28
	data_stream adder_inter29("adder_inter29");
#pragma HLS stream variable=adder_inter29 depth=6
#pragma HLS DATA_PACK variable=adder_inter29
	data_stream adder_inter30("adder_inter30");
#pragma HLS stream variable=adder_inter30 depth=6
#pragma HLS DATA_PACK variable=adder_inter30
	data_stream adder_inter31("adder_inter31");
#pragma HLS stream variable=adder_inter31 depth=6
#pragma HLS DATA_PACK variable=adder_inter31

    data_stream final_to_ddr0("final_to_ddr0");
#pragma HLS stream variable=final_to_ddrt0 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr0
    data_stream final_to_ddr1("final_to_ddr1");
#pragma HLS stream variable=final_to_ddrt1 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr1
    data_stream final_to_ddr2("final_to_ddr2");
#pragma HLS stream variable=final_to_ddrt2 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr2
    data_stream final_to_ddr3("final_to_ddr3");
#pragma HLS stream variable=final_to_ddrt3 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr3
    data_stream final_to_ddr4("final_to_ddr4");
#pragma HLS stream variable=final_to_ddrt4 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr4
    data_stream final_to_ddr5("final_to_ddr5");
#pragma HLS stream variable=final_to_ddrt5 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr5
    data_stream final_to_ddr6("final_to_ddr6");
#pragma HLS stream variable=final_to_ddrt6 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr6
    data_stream final_to_ddr7("final_to_ddr7");
#pragma HLS stream variable=final_to_ddrt7 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr7
    data_stream final_to_ddr8("final_to_ddr8");
#pragma HLS stream variable=final_to_ddrt8 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr8
    data_stream final_to_ddr9("final_to_ddr9");
#pragma HLS stream variable=final_to_ddrt9 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr9
    data_stream final_to_ddr10("final_to_ddr10");
#pragma HLS stream variable=final_to_ddrt10 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr10
    data_stream final_to_ddr11("final_to_ddr11");
#pragma HLS stream variable=final_to_ddrt11 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr11
    data_stream final_to_ddr12("final_to_ddr12");
#pragma HLS stream variable=final_to_ddrt12 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr12
    data_stream final_to_ddr13("final_to_ddr13");
#pragma HLS stream variable=final_to_ddrt13 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr13
    data_stream final_to_ddr14("final_to_ddr14");
#pragma HLS stream variable=final_to_ddrt14 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr14
    data_stream final_to_ddr15("final_to_ddr15");
#pragma HLS stream variable=final_to_ddrt15 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr15
    data_stream final_to_ddr16("final_to_ddr16");
#pragma HLS stream variable=final_to_ddrt16 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr16
    data_stream final_to_ddr17("final_to_ddr17");
#pragma HLS stream variable=final_to_ddrt17 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr17
    data_stream final_to_ddr18("final_to_ddr18");
#pragma HLS stream variable=final_to_ddrt18 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr18
    data_stream final_to_ddr19("final_to_ddr19");
#pragma HLS stream variable=final_to_ddrt19 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr19
    data_stream final_to_ddr20("final_to_ddr20");
#pragma HLS stream variable=final_to_ddrt20 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr20
    data_stream final_to_ddr21("final_to_ddr21");
#pragma HLS stream variable=final_to_ddrt21 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr21
    data_stream final_to_ddr22("final_to_ddr22");
#pragma HLS stream variable=final_to_ddrt22 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr22
    data_stream final_to_ddr23("final_to_ddr23");
#pragma HLS stream variable=final_to_ddrt23 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr23
    data_stream final_to_ddr24("final_to_ddr24");
#pragma HLS stream variable=final_to_ddrt24 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr24
    data_stream final_to_ddr25("final_to_ddr25");
#pragma HLS stream variable=final_to_ddrt25 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr25
    data_stream final_to_ddr26("final_to_ddr26");
#pragma HLS stream variable=final_to_ddrt26 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr26
    data_stream final_to_ddr27("final_to_ddr27");
#pragma HLS stream variable=final_to_ddrt27 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr27
    data_stream final_to_ddr28("final_to_ddr28");
#pragma HLS stream variable=final_to_ddrt28 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr28
    data_stream final_to_ddr29("final_to_ddr29");
#pragma HLS stream variable=final_to_ddrt29 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr29
    data_stream final_to_ddr30("final_to_ddr30");
#pragma HLS stream variable=final_to_ddrt30 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr30
    data_stream final_to_ddr31("final_to_ddr31");
#pragma HLS stream variable=final_to_ddrt31 depth=1
#pragma HLS DATA_PACK variable=final_to_ddr31

    val_stream result_b("result_b");
#pragma HLS RESOURCE variable=result_b core=FIFO_LUTRAM
#pragma HLS stream variable=result_b depth=32

#pragma HLS DATAFLOW

    rowlength_split(row_batch_length, &rowlength_sub1, &rowlength_sub2);

    load_matrix_value(&rowlength_sub1, matrix1, &first, &two, total_num);
    load_matrix_value(&rowlength_sub2, matrix2, &third, &four, total_num);

    load_row_index(rid, &row_index, &row_index1, &row_index2, &row_index3, total_num);
    load_col_index(cid, &col_index, &col_index1, &col_index2, &col_index3, total_num);

    control_unit(&col_index, &col_index1, &col_index2, &col_index3, total_num, &middle_index1, &middle_index2,
    		&control_stream, &control_stream1, &control_stream2, &control_stream3);

    get_data(&middle_index1, &middle_index2, &middle_data1, &middle_data2, vector_buffer);

    final_data(&control_stream, &control_stream1, &control_stream2, &control_stream3, &middle_data1,
    		&middle_data2, &col_value, &col_value1, &col_value2, &col_value3);

    processing_element(&first, &col_value,  &row_index,  &subresult_pe0);
    processing_element(&two,   &col_value1, &row_index1, &subresult_pe1);
	processing_element(&third, &col_value2, &row_index2, &subresult_pe2);
	processing_element(&four,  &col_value3, &row_index3, &subresult_pe3);

	distributor(&subresult_pe0, &d0_merger0, &d0_merger1, &d0_merger2, &d0_merger3, &d0_merger4, &d0_merger5, &d0_merger6, &d0_merger7, &d0_merger8, &d0_merger9, &d0_merger10, &d0_merger11, &d0_merger12, &d0_merger13, &d0_merger14, &d0_merger15, &d0_merger16, &d0_merger17, &d0_merger18, &d0_merger19, &d0_merger20, &d0_merger21, &d0_merger22, &d0_merger23, &d0_merger24, &d0_merger25, &d0_merger26, &d0_merger27, &d0_merger28, &d0_merger29, &d0_merger30, &d0_merger31);
	distributor(&subresult_pe1, &d1_merger0, &d1_merger1, &d1_merger2, &d1_merger3, &d1_merger4, &d1_merger5, &d1_merger6, &d1_merger7, &d1_merger8, &d1_merger9, &d1_merger10, &d1_merger11, &d1_merger12, &d1_merger13, &d1_merger14, &d1_merger15, &d1_merger16, &d1_merger17, &d1_merger18, &d1_merger19, &d1_merger20, &d1_merger21, &d1_merger22, &d1_merger23, &d1_merger24, &d1_merger25, &d1_merger26, &d1_merger27, &d1_merger28, &d1_merger29, &d1_merger30, &d1_merger31);
	distributor(&subresult_pe2, &d2_merger0, &d2_merger1, &d2_merger2, &d2_merger3, &d2_merger4, &d2_merger5, &d2_merger6, &d2_merger7, &d2_merger8, &d2_merger9, &d2_merger10, &d2_merger11, &d2_merger12, &d2_merger13, &d2_merger14, &d2_merger15, &d2_merger16, &d2_merger17, &d2_merger18, &d2_merger19, &d2_merger20, &d2_merger21, &d2_merger22, &d2_merger23, &d2_merger24, &d2_merger25, &d2_merger26, &d2_merger27, &d2_merger28, &d2_merger29, &d2_merger30, &d2_merger31);
	distributor(&subresult_pe3, &d3_merger0, &d3_merger1, &d3_merger2, &d3_merger3, &d3_merger4, &d3_merger5, &d3_merger6, &d3_merger7, &d3_merger8, &d3_merger9, &d3_merger10, &d3_merger11, &d3_merger12, &d3_merger13, &d3_merger14, &d3_merger15, &d3_merger16, &d3_merger17, &d3_merger18, &d3_merger19, &d3_merger20, &d3_merger21, &d3_merger22, &d3_merger23, &d3_merger24, &d3_merger25, &d3_merger26, &d3_merger27, &d3_merger28, &d3_merger29, &d3_merger30, &d3_merger31);

	merger(&d0_merger0, &d1_merger0, &d2_merger0, &d3_merger0, &merger_to_final0);
	merger(&d0_merger1, &d1_merger1, &d2_merger1, &d3_merger1, &merger_to_final1);
	merger(&d0_merger2, &d1_merger2, &d2_merger2, &d3_merger2, &merger_to_final2);
	merger(&d0_merger3, &d1_merger3, &d2_merger3, &d3_merger3, &merger_to_final3);
	merger(&d0_merger4, &d1_merger4, &d2_merger4, &d3_merger4, &merger_to_final4);
	merger(&d0_merger5, &d1_merger5, &d2_merger5, &d3_merger5, &merger_to_final5);
	merger(&d0_merger6, &d1_merger6, &d2_merger6, &d3_merger6, &merger_to_final6);
	merger(&d0_merger7, &d1_merger7, &d2_merger7, &d3_merger7, &merger_to_final7);
	merger(&d0_merger8, &d1_merger8, &d2_merger8, &d3_merger8, &merger_to_final8);
	merger(&d0_merger9, &d1_merger9, &d2_merger9, &d3_merger9, &merger_to_final9);
	merger(&d0_merger10, &d1_merger10, &d2_merger10, &d3_merger10, &merger_to_final10);
	merger(&d0_merger11, &d1_merger11, &d2_merger11, &d3_merger11, &merger_to_final11);
	merger(&d0_merger12, &d1_merger12, &d2_merger12, &d3_merger12, &merger_to_final12);
	merger(&d0_merger13, &d1_merger13, &d2_merger13, &d3_merger13, &merger_to_final13);
	merger(&d0_merger14, &d1_merger14, &d2_merger14, &d3_merger14, &merger_to_final14);
	merger(&d0_merger15, &d1_merger15, &d2_merger15, &d3_merger15, &merger_to_final15);
	merger(&d0_merger16, &d1_merger16, &d2_merger16, &d3_merger16, &merger_to_final16);
	merger(&d0_merger17, &d1_merger17, &d2_merger17, &d3_merger17, &merger_to_final17);
	merger(&d0_merger18, &d1_merger18, &d2_merger18, &d3_merger18, &merger_to_final18);
	merger(&d0_merger19, &d1_merger19, &d2_merger19, &d3_merger19, &merger_to_final19);
	merger(&d0_merger20, &d1_merger20, &d2_merger20, &d3_merger20, &merger_to_final20);
	merger(&d0_merger21, &d1_merger21, &d2_merger21, &d3_merger21, &merger_to_final21);
	merger(&d0_merger22, &d1_merger22, &d2_merger22, &d3_merger22, &merger_to_final22);
	merger(&d0_merger23, &d1_merger23, &d2_merger23, &d3_merger23, &merger_to_final23);
	merger(&d0_merger24, &d1_merger24, &d2_merger24, &d3_merger24, &merger_to_final24);
	merger(&d0_merger25, &d1_merger25, &d2_merger25, &d3_merger25, &merger_to_final25);
	merger(&d0_merger26, &d1_merger26, &d2_merger26, &d3_merger26, &merger_to_final26);
	merger(&d0_merger27, &d1_merger27, &d2_merger27, &d3_merger27, &merger_to_final27);
	merger(&d0_merger28, &d1_merger28, &d2_merger28, &d3_merger28, &merger_to_final28);
	merger(&d0_merger29, &d1_merger29, &d2_merger29, &d3_merger29, &merger_to_final29);
	merger(&d0_merger30, &d1_merger30, &d2_merger30, &d3_merger30, &merger_to_final30);
	merger(&d0_merger31, &d1_merger31, &d2_merger31, &d3_merger31, &merger_to_final31);

	final_adder(&merger_to_final0, &adder_inter0);
	final_adder(&merger_to_final1, &adder_inter1);
	final_adder(&merger_to_final2, &adder_inter2);
	final_adder(&merger_to_final3, &adder_inter3);
	final_adder(&merger_to_final4, &adder_inter4);
	final_adder(&merger_to_final5, &adder_inter5);
	final_adder(&merger_to_final6, &adder_inter6);
	final_adder(&merger_to_final7, &adder_inter7);
	final_adder(&merger_to_final8, &adder_inter8);
	final_adder(&merger_to_final9, &adder_inter9);
	final_adder(&merger_to_final10, &adder_inter10);
	final_adder(&merger_to_final11, &adder_inter11);
	final_adder(&merger_to_final12, &adder_inter12);
	final_adder(&merger_to_final13, &adder_inter13);
	final_adder(&merger_to_final14, &adder_inter14);
	final_adder(&merger_to_final15, &adder_inter15);
	final_adder(&merger_to_final16, &adder_inter16);
	final_adder(&merger_to_final17, &adder_inter17);
	final_adder(&merger_to_final18, &adder_inter18);
	final_adder(&merger_to_final19, &adder_inter19);
	final_adder(&merger_to_final20, &adder_inter20);
	final_adder(&merger_to_final21, &adder_inter21);
	final_adder(&merger_to_final22, &adder_inter22);
	final_adder(&merger_to_final23, &adder_inter23);
	final_adder(&merger_to_final24, &adder_inter24);
	final_adder(&merger_to_final25, &adder_inter25);
	final_adder(&merger_to_final26, &adder_inter26);
	final_adder(&merger_to_final27, &adder_inter27);
	final_adder(&merger_to_final28, &adder_inter28);
	final_adder(&merger_to_final29, &adder_inter29);
	final_adder(&merger_to_final30, &adder_inter30);
	final_adder(&merger_to_final31, &adder_inter31);

	accumulator(&adder_inter0, &final_to_ddr0);
	accumulator(&adder_inter1, &final_to_ddr1);
	accumulator(&adder_inter2, &final_to_ddr2);
	accumulator(&adder_inter3, &final_to_ddr3);
	accumulator(&adder_inter4, &final_to_ddr4);
	accumulator(&adder_inter5, &final_to_ddr5);
	accumulator(&adder_inter6, &final_to_ddr6);
	accumulator(&adder_inter7, &final_to_ddr7);
	accumulator(&adder_inter8, &final_to_ddr8);
	accumulator(&adder_inter9, &final_to_ddr9);
	accumulator(&adder_inter10, &final_to_ddr10);
	accumulator(&adder_inter11, &final_to_ddr11);
	accumulator(&adder_inter12, &final_to_ddr12);
	accumulator(&adder_inter13, &final_to_ddr13);
	accumulator(&adder_inter14, &final_to_ddr14);
	accumulator(&adder_inter15, &final_to_ddr15);
	accumulator(&adder_inter16, &final_to_ddr16);
	accumulator(&adder_inter17, &final_to_ddr17);
	accumulator(&adder_inter18, &final_to_ddr18);
	accumulator(&adder_inter19, &final_to_ddr19);
	accumulator(&adder_inter20, &final_to_ddr20);
	accumulator(&adder_inter21, &final_to_ddr21);
	accumulator(&adder_inter22, &final_to_ddr22);
	accumulator(&adder_inter23, &final_to_ddr23);
	accumulator(&adder_inter24, &final_to_ddr24);
	accumulator(&adder_inter25, &final_to_ddr25);
	accumulator(&adder_inter26, &final_to_ddr26);
	accumulator(&adder_inter27, &final_to_ddr27);
	accumulator(&adder_inter28, &final_to_ddr28);
	accumulator(&adder_inter29, &final_to_ddr29);
	accumulator(&adder_inter30, &final_to_ddr30);
	accumulator(&adder_inter31, &final_to_ddr31);

	coalesce(&final_to_ddr0, &final_to_ddr1, &final_to_ddr2, &final_to_ddr3,
			&final_to_ddr4, &final_to_ddr5, &final_to_ddr6, &final_to_ddr7,
			&final_to_ddr8, &final_to_ddr9, &final_to_ddr10, &final_to_ddr11, &final_to_ddr12,
			&final_to_ddr13, &final_to_ddr14, &final_to_ddr15, &final_to_ddr16, &final_to_ddr17,
			&final_to_ddr18, &final_to_ddr19, &final_to_ddr20, &final_to_ddr21, &final_to_ddr22,
			&final_to_ddr23, &final_to_ddr24, &final_to_ddr25, &final_to_ddr26, &final_to_ddr27,
			&final_to_ddr28, &final_to_ddr29, &final_to_ddr30, &final_to_ddr31, &result_b);

	store_result_to_DDR(&result_b, result, result_number);

}

void spmv(ap_uint<128>* matrix1, ap_uint<128>* matrix2, ap_uint<128>* rid, ap_uint<128>* cid,
          val_t* result, uint32_t* number_per_rbatch, ap_uint<128>* vector, ap_uint<128>* vector2,
		  ap_uint<128>* vector3, ap_uint<128>* vector4, uint32_t total_num,
		  uint32_t result_number, uint32_t vector_number){
	/*
	 *  matrix1, matrix2, rid, cid are the pointers to matrixs, cids, rids for multiple memory ports
	 *	result refers to the pointer of result vector
	 *	result_number refers to the result vector's length
	 *	total_num refers to the total mini-batches (4-element batch)
	 *	vector is the pointer to vector and vector_number refers to the number of vector
	 *  number_per_rbatch refers to the numbers in each row-batch, used for control
	 */
#pragma HLS INTERFACE s_axilite port=total_num bundle=ctr
#pragma HLS INTERFACE s_axilite port=result_number bundle=ctr
#pragma HLS INTERFACE s_axilite bundle=ctr port=vector_number
#pragma HLS INTERFACE s_axilite port=return bundle=ctr

#pragma HLS INTERFACE m_axi depth=73578 port=matrix1 offset=slave bundle=mat1
#pragma HLS INTERFACE m_axi depth=73578 port=matrix2 offset=slave bundle=mat2
#pragma HLS INTERFACE m_axi depth=73578 port=rid offset=slave bundle=rid
#pragma HLS INTERFACE m_axi depth=73578 port=cid offset=slave bundle=cid
#pragma HLS INTERFACE m_axi depth=3242 port=result offset=slave bundle=result
#pragma HLS INTERFACE m_axi depth=406 port=vector offset=slave bundle=vector
#pragma HLS INTERFACE m_axi depth=406 port=vector2 offset=slave bundle=vector2
#pragma HLS INTERFACE m_axi depth=406 port=vector3 offset=slave bundle=vector3
#pragma HLS INTERFACE m_axi depth=406 port=vector4 offset=slave bundle=vector4
#pragma HLS INTERFACE m_axi depth=102 port=number_per_rbatch offset=slave bundle=rbatchnum

    ap_uint<256> vector_buffer[N];
#pragma HLS RESOURCE variable=vector_buffer core=RAM_T2P_URAM

    // control the switch of different row batches
    id_stream row_batch_length("row_batch_element");
#pragma HLS stream variable=row_batch_element depth=6900

    uint32_t number_row_batch = (result_number % 32 == 0) ? result_number / 32 : (result_number / 32 + 1);
    uint32_t number_vector_length = (vector_number % 8 == 0) ? vector_number / 8 : (vector_number / 8 + 1);

    // load vectors to Ultra-RAM and length of each row_batch
    load_configuration(vector_buffer, &row_batch_length, vector, vector2, vector3, vector4,
    		number_vector_length, number_per_rbatch, number_row_batch);

    // main algorithm
	dataflow_engine(result, matrix1, matrix2, rid, cid, vector_buffer, &row_batch_length, total_num, result_number);
}
