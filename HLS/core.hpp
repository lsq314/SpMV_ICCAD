#ifndef CONFIG
#define CONFIG

//#include "/home/lsq/software/Xilinx_2018.3/Vivado/2018.3/include/gmp.h"
#include <stdint.h>
#include <hls_stream.h>
#include <ap_fixed.h>
#include <string.h>

#define N 55000
#define TERMINATE 1048576

typedef double val_t;

typedef uint32_t id_t;
template <typename T>
struct val_data {
    T data;
    bool partial_end;
    bool final_end;
};

template <typename T>
struct sub_result {
    T data;
    id_t rid;
    bool partial_end;
    bool final_end;
};

typedef hls::stream<uint32_t> id_stream;
typedef hls::stream<val_t> val_stream;
typedef hls::stream<val_data<val_t> > data_stream;
typedef hls::stream<sub_result<val_t> > subresult_stream;

template<typename To, typename From>
inline To Reinterpret(const From& val){
    return reinterpret_cast<const To&>(val);
}

void spmv(ap_uint<128>* matrix1, ap_uint<128>* matrix2, ap_uint<128>* rid, ap_uint<128>* cid,
          val_t* result, uint32_t* number_per_rbatch, ap_uint<128>* vector, ap_uint<128>* vector2,
		  ap_uint<128>* vector3, ap_uint<128>* vector4, uint32_t total_num,
		  uint32_t result_number, uint32_t vector_number);

void test_for_final_adder(val_t* result, uint32_t number);

#endif

