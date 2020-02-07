#include "PcaReducer.h"


PcaReducer::PcaReducer(int i_outDim) : d_outDim(i_outDim) {

}

void PcaReducer::learn (const DataList& i_data) {
    Matrix dataMat = fn::listToMatrix(i_data);
    d_W = fn::getPcaMatrix(dataMat, d_outDim);
}

Column PcaReducer::forward(const Column& i_inp) {
    return d_W*i_inp;
}
