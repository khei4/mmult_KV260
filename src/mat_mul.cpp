#include "mat_mul.h"

void mat_mul(AXI_STREAM &in_strm, AXI_STREAM &out_strm) {
// TODO:なんぞこの行
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

#pragma HLS INTERFACE axis port = in_strm
#pragma HLS INTERFACE axis port = out_strm
  unsigned int mat_a[SIZE][SIZE];
  unsigned int mat_b[SIZE][SIZE];

// TODO: いろいろな分割の仕方をためしてみる
#pragma HLS ARRAY_PARTITION variable=mat_a complete dim=0
#pragma HLS ARRAY_PARTITION variable=mat_b complete dim=0

  AP_AXIS elem;

  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
#pragma HLS PIPELINE II=1
      elem = in_strm.read();
      mat_a[i][j] = elem.data;
    }
  }

  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
#pragma HLS PIPELINE II=1
      elem = in_strm.read();
      mat_b[i][j] = elem.data;
    }
  }

  AP_AXIS out_val;
  out_val.keep = elem.keep;
  // ストローブ信号
  out_val.strb = elem.strb;
  out_val.last = 0;
  for (int i = 0; i < SIZE; ++i)
    for (int j = 0; j < SIZE; ++j) {
      // TODO: ここにPIPELINEを入れた場合どんな回路ができるか全く読めていない
#pragma HLS PIPELINE II=1
      out_val.data = 0;
      for (int k = 0; k < SIZE; ++k)
        out_val.data += mat_a[i][k] * mat_b[k][j];
      if (i + 1 == SIZE && j + 1 == SIZE) 
        out_val.last = 1;
      out_strm << out_val;
      
    }
}
