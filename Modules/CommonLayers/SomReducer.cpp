#include "SomReducer.h"

SomReducer::SomReducer() {}

void SomReducer::init() {}

Column SomReducer::forward(const Column& i_inp) {
    return d_W*i_inp;
}


void SomReducer::calcMap(int maxIdx) {

#pragma omp parallel for
    for (int i = 0; i < d_outDimension; ++i) {
        double d = (i - maxIdx + d_outDimension) % d_outDimension;
        d_map[i] = exp(-d*d/ (2*d_sigma));
    }
}

void SomReducer::learn(const DataList& i_data) {
    init();

    double sigmaReducer = exp(-1.0 / 100);
    double thetaReducer = exp(-1.0 / 100);
    for (int iter = 0; iter < d_iterations; ++iter) {
        for (const auto& v : i_data) {
            Column y = forward(v);
            uint maxIndex = y.index_max();
            
            calcMap(maxIndex);

#pragma omp parallel for
            for (int i = 0; i < d_W.n_cols; ++i) {
                d_W.row(i) += d_map[i]*d_theta*y[i]*(v.t() - d_W.row(i));
            }
        }

        d_sigma *= sigmaReducer;
        d_theta *= thetaReducer;
    }
}