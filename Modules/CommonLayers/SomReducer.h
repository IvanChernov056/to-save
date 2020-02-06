#ifndef SOM_REDUCER_H
#define SOM_REDUCER_H


#include "utils.h"

class SomReducer {

    public:

        SomReducer();

        void init();

        void learn(const DataList& i_data);
        Column forward(const Column& i_inp);

    private:

        void calcMap(int maxIdx) ;

    private:

        Matrix d_W;
        Column d_map;
        
        int d_outDimension;
        int d_iterations{500};

        double d_sigma;
        double d_theta;
};

#endif