#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>

using namespace hls;
typedef ap_axis<32, 0, 0, 0> AP_AXIS;
typedef stream<AP_AXIS> AXI_STREAM;

const int SIZE = 4;