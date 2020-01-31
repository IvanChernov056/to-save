#ifndef LINER_READOUT_H
#define LINER_READOUT_H


#include "utils.h"

enum LearnMode {
    DIRECT_PINV,
    INDIRECT_PINV,
    RIDGE
};

class LinerReaduot {

    public:

        LinerReaduot(double i_ridge = 0.03, LearnMode i_mode = RIDGE);


        Column forward(const Column& i_inp);

        void learn(const DataList& i_inp, const DataList& i_etalon);


    protected:

        void directPinvLearn(const DataList& i_inp, const DataList& i_etalon);
        void indirectPinvLearn(const DataList& i_inp, const DataList& i_etalon);
        void ridgeLearn(const DataList& i_inp, const DataList& i_etalon);

    protected:

        Matrix d_Wout;
        double d_ridge;
        LearnMode d_mode;
};

#endif