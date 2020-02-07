#ifndef PCA_REDUCER_H
#define PCA_REDUCER_H


#include "utils.h"

class PcaReducer {


    public: 
        PcaReducer(int i_outDim);

        void learn (const DataList& i_data);
        Column forward(const Column& i_inp);

    private:

        Matrix d_W;

        int d_outDim;

};

#endif