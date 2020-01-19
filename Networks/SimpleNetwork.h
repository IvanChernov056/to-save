#ifndef SIMPLE_NETWORK_H
#define SIMPLE_NETWORK_H

#include <Modules/readouts.h>
#include <Modules/reservoirs.h>

#include <memory>

class SimpleNetwork {

    public:

        SimpleNetwork(int neuronAmount, double inpVal, double timeConstant, double timeStep, double leakingDecayRate, 
        double specRadius, double density, double noiseVariance, double ridge);
        ~SimpleNetwork()= default;

        double test (const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name, bool drawEtalon);
        
    protected:

        void learn(const DataList& i_data, int i_skipLen, int i_learnLen);
        DataList generate(int i_generateLen);
        void skip(const DataVector& i_inp, const DataList& i_feedback);

    protected:

        std::unique_ptr<ClassicEsnReservoir> d_resPtr;
        std::unique_ptr<LinerReaduot>        d_rdoutPtr;

        Column  d_genInitFeedback;
        Column  d_inputVec;

        int d_skipLen = 0; 
        int d_learnLen = 0; 
        int d_generateLen = 0;
};

#endif