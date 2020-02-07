#ifndef CLASSIC_WITH_SOM_H
#define CLASSIC_WITH_SOM_H

#include "ClassicEsnReservoir.h"
#include "SomReducer.h"
#include "LinerReadout.h"


class ClassicWithSom {

    public:

        ClassicWithSom (int neuronAmount, double inpVal, double timeConstant, double timeStep, double leakingDecayRate, 
        double specRadius, double density, double noiseVariance, double i_inputWeightValue, double i_feedbackWeightValue, 
        double i_zeroProbInInputWeight, double i_zeroProbInFeedbackWeight, double i_reduceRatio, double ridge);

        double test (const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name);
        double test2 (const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name);

    private:

        void learn(const DataList& i_data, int i_skipLen, int i_learnLen);
        DataList generate(int i_generateLen);
        DataList predict (const DataList& i_inp);
        void skip(const DataVector& i_inp, const DataList& i_feedback);

    private:


        std::unique_ptr<ClassicEsnReservoir> d_resPtr;
        std::unique_ptr<SomReducer>          d_reducer;
        std::unique_ptr<LinerReaduot>        d_rdoutPtr;

        Column  d_genInitFeedback;
        Column  d_inputVec;
};


#endif