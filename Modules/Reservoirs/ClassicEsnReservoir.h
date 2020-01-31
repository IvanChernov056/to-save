#ifndef CLASSIC_ESN_RESERVOIR_H
#define CLASSIC_ESN_RESERVOIR_H

#include "utils.h"

class ClassicEsnReservoir {


    public:

        ClassicEsnReservoir (int i_neuronAmount, double i_timeConstant = 0.44, 
         double i_timeStep = 1.0, double i_leakingDecayRate = 0.9, double i_specRadius = 0.79, double i_density = 0.03, 
         double i_noiseVariance = 1e-3, double i_inputWeightValue = 0.14, double i_feedbackWeightValue = 0.56,
         double i_zeroProbInInputWeight = 0.5, double i_zeroProbInFeedbackWeight = 0.1, ActivationFunc i_f = acfn::genTanh());


        void init (int i_inpSize, int i_ofbSize);
        
        Column forward(const Column& i_inp, const Column i_feedback);
        Column learnForward(const Column& i_inp, const Column i_feedback);

    protected:

        Column totalIncomingSingnal(const Column& i_inp, const Column i_feedback);
        Column totalIncomingSingnalWhileLearn(const Column& i_inp, const Column i_feedback);

    private:

        static Matrix createOuterWeights(int rows, int cols, double nonZeroVal, double zeroProb);

    protected:

        Matrix d_Win;
        Matrix d_Wofb;
        SpMat  d_W;
        Column d_state;

        ActivationFunc d_func;

        const double d_timeConstant{0.44};
        const double d_timeStep{1.0};
        const double d_leakingDecayRate{0.9};
        
        double d_specRadius{0.79};
        double d_density{0.03};
        double d_noiseVariance{1e-5};

        double d_inputWeightValue;
        double d_feedbackWeightValue;

        double d_zeroProbInInputWeight;
        double d_zeroProbInFeedbackWeight;
};

#endif