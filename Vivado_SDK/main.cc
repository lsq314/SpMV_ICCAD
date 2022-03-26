/*
 * Empty C++ Application
 */
#include <stdio.h>
#include <xspmv.h>
#include <xtmrctr.h>
#include <xil_cache.h>
#include <math.h>
#include <xil_exception.h>
#include <xscugic.h>
#include <arm_neon.h>
#include <stdlib.h>
#include <ff.h>

typedef double val_t;
typedef uint32_t id_t;

val_t eof = 1e-5;

static FATFS fatfs;
int rc;

#define val1B "val1.BIN"
#define val2B "val2.BIN"
#define ridB "rid.BIN"
#define cidB "cid.BIN"
#define numberB "rowl.BIN"

XSpmv x;
XSpmv_Config* xc;

XTmrCtr t;
XTmrCtr_Config* tc;

XScuGic s;
XScuGic_Config* sc;

int i, j, k;
u32 start_time, end_time;
bool isfinished=false;

u32 start_timer(XTmrCtr &t, uint8_t counter){
    XTmrCtr_Reset(&t, counter);
    u32 result = XTmrCtr_GetValue(&t, counter);
    XTmrCtr_Start(&t, counter);
    return result;
}

u32 stop_timer(XTmrCtr &t, uint8_t counter){
    u32 result = XTmrCtr_GetValue(&t, counter);
    XTmrCtr_Stop(&t, counter);
    return result;
}

void Handler(void *ptr){

    stop_timer(t, 0);
    end_time = Xil_In32((0xa0010000) + XTmrCtr_Offsets[(0)] + (XTC_TCR_OFFSET));
    printf("Totally run %u cycles\n", end_time - start_time);

    XSpmv *tx = (XSpmv *)ptr;
    XSpmv_InterruptClear(tx, 1);
//  XSpmv_InterruptDisable(tx, 0);
//  XSpmv_InterruptGlobalDisable(tx);

    isfinished = true;
    return;
}

int SD_Init(){
    FRESULT rc;
    rc = f_mount(&fatfs,"",0);
    if(rc){
        xil_printf("ERROR: f_mount returned %d\r\n",rc);
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}

int SD_Transfer_read(char *FileName,u64 DestinationAddress,u32 ByteLength){
    FIL fil;
    FRESULT rc;
    UINT br;

    rc=f_open(&fil,FileName,FA_READ);
    if(rc){
        xil_printf("ERROR:f_open returned %d\r\n",rc);
        return XST_FAILURE;
    }
    rc = f_lseek(&fil,0);
    if(rc){
        xil_printf("ERROR:f_open returned %d\r\n",rc);
        return XST_FAILURE;
    }
    rc = f_read(&fil,(void*)DestinationAddress,ByteLength,&br);
    if(rc){
        xil_printf("ERROR:f_open returned %d\r\n",rc);
        return XST_FAILURE;
    }
    rc = f_close(&fil);
    if(rc){
        xil_printf("ERROR:f_open returned %d\r\n",rc);
        return XST_FAILURE;
    }
    return XST_SUCCESS;
}

#define rows 115967
#define columns 115968

__attribute ((aligned (16))) val_t vector[columns/4];
__attribute ((aligned (16))) val_t vector1[columns/4];
__attribute ((aligned (16))) val_t vector2[columns/4];
__attribute ((aligned (16))) val_t vector3[columns/4];

val_t vectort[rows];

#define total 258370
#define rowlengt 3624

__attribute ((aligned (16))) id_t n_cid_f[total*4];
__attribute ((aligned (16))) val_t result_f[rows];
__attribute ((aligned (16))) val_t matrix1[total*2];
__attribute ((aligned (16))) val_t matrix2[total*2];
__attribute ((aligned (16))) id_t rid[total*4];
__attribute ((aligned (16))) id_t number_per_rbatch[rowlengt];
val_t resultr_f[rows] = {0.0};

int main(){

//    Xil_DCacheDisable();
    tc = XTmrCtr_LookupConfig(XPAR_AXI_TIMER_0_DEVICE_ID);
    XTmrCtr_CfgInitialize(&t, tc, tc->BaseAddress);

    rc = SD_Init();
    rc = SD_Transfer_read(val1B,(u64)matrix1,(total*2*8));
    rc = SD_Transfer_read(val2B,(u64)matrix2,(total*2*8));
    rc = SD_Transfer_read(ridB,(u64)rid,(total*4*4));
    rc = SD_Transfer_read(cidB,(u64)n_cid_f,(total*4*4));
    rc = SD_Transfer_read(numberB, (u64)number_per_rbatch, (rowlengt*4));

    for (int i=0; i<rows; i++){
        vectort[i] = (val_t)i;
		result_f[i] = (val_t)0;
		resultr_f[i] = (val_t)0;
    }

	for(int i=0; i<columns/8; i++){
		vector[i*2] = (val_t)(8*i);
		vector[i*2+1] = (val_t)(8*i+1);
		vector1[i*2] = (val_t)(8*i+2);
		vector2[i*2] = (val_t)(8*i+4);
		vector3[i*2] = (val_t)(8*i+6);
		vector1[i*2+1] = (val_t)(8*i+3);
		vector2[i*2+1] = (val_t)(8*i+5);
		vector3[i*2+1] = (val_t)(8*i+7);
	}

    Xil_DCacheFlush();

//    for(int i=0; i<10; i++)
//        printf("%u\n", number_per_rbatch[i]);

    xc = XSpmv_LookupConfig(XPAR_SPMV_0_DEVICE_ID);
    XSpmv_CfgInitialize(&x, xc);

    for(int i=0; i<total*4; i++){
        if (i%4<=1){
        	if (rid[i] != 1048576){
            	resultr_f[rid[i]] += vectort[n_cid_f[i]] * matrix1[i%2+i/4*2];
        	}
        }
        else{
        	if (rid[i] != 1048576){
        		resultr_f[rid[i]] += vectort[n_cid_f[i]] * matrix2[i%2+i/4*2];
        	}
        }
    }

    XSpmv_Set_cid_V(&x, (u64)n_cid_f);
    XSpmv_Set_result_number(&x, rows);
    XSpmv_Set_result_offset(&x, (u64)result_f);
    XSpmv_Set_total_num(&x, total);
    XSpmv_Set_vector_number(&x, columns);
    XSpmv_Set_vector_V(&x, (u64)vector);
    XSpmv_Set_vector2_V(&x, (u64)vector1);
    XSpmv_Set_vector3_V(&x, (u64)vector2);
    XSpmv_Set_vector4_V(&x, (u64)vector3);
    XSpmv_Set_number_per_rbatch(&x, (u64)number_per_rbatch);
    XSpmv_Set_matrix1_V(&x, (u64)matrix1);
    XSpmv_Set_matrix2_V(&x, (u64)matrix2);
    XSpmv_Set_rid_V(&x, (u64)rid);

    printf("begin\n");
    start_time = start_timer(t, 0);
    XSpmv_Start(&x);
    while(!XSpmv_IsDone(&x));
    end_time = stop_timer(t, 0);

    Xil_DCacheInvalidate();
    printf("Checking result...%u\n", end_time - start_time);
    for (i=0; i<rows; i++){
      if (fabs(resultr_f[i]-result_f[i]) > eof)
          printf("Error %f %f\n", resultr_f[i], result_f[i]);
//      else
//          printf("Success %f %f\n", resultr_f[i], result_f[i]);
    }
    printf("Success\n");

    return 0;
}
