#ifndef SIMPLE_NETWORK_H
#define SIMPLE_NETWORK_H

#include <Modules/readouts.h>
#include <Modules/reservoirs.h>


class SimpleNetwork {

    public:

        SimpleNetwork(int neuronAmount, double timeConstant, double timeStep, double leakingDecayRate, 
        double specRadius, double density, double noiseVariance, double ridge) {}
        SimpleNetwork(const ClassicEsnReservoir&  i_res, const LinerReaduot& i_rdout, int i_skipLen, int i_learnLen, int i_generateLen);

        void test (const DataVector& i_inp, const DataList& i_etalon);
        
    protected:

        void learn(const DataVector& i_inp, const DataList& i_etalon);
        DataList generate();
        void skip(const DataVector& i_inp, const DataList& i_feedback);

    protected:

        ClassicEsnReservoir d_res;
        LinerReaduot        d_rdout;

        Column  d_genInitFeedback;
        Column  d_inputVec;

        int d_skipLen; 
        int d_learnLen; 
        int d_generateLen;
};

#endif