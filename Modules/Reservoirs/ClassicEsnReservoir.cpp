#include "ClassicEsnReservoir.h"

ClassicEsnReservoir::ClassicEsnReservoir (int i_neuronAmount, double i_timeConstant, double i_timeStep,
 double i_leakingDecayRate, double i_specRadius, double i_density, double i_noiseVariance, double i_inputWeightValue,
 double i_feedbackWeightValue, double i_zeroProbInInputWeight, double i_zeroProbInFeedbackWeight, ActivationFunc i_f) : 
    d_W(RandnSpMatrix(i_neuronAmount, i_neuronAmount, i_density)), d_func(i_f), d_timeConstant(i_timeConstant),
    d_timeStep(i_timeStep), d_leakingDecayRate(i_leakingDecayRate), d_specRadius(i_specRadius), d_density(i_density),
    d_noiseVariance(i_noiseVariance), d_state(ZerosColumn(i_neuronAmount)), d_inputWeightValue(i_inputWeightValue),
    d_feedbackWeightValue(i_feedbackWeightValue), d_zeroProbInInputWeight(i_zeroProbInInputWeight), 
    d_zeroProbInFeedbackWeight(i_zeroProbInFeedbackWeight)
{
    try {
        ComplexColumn eigVal = EIGEN_SPARSE_VAL(d_W, i_neuronAmount);
        Column absVal = ABS(eigVal);
        double radius = absVal.max();
        if (radius > 0)
            d_W *= d_specRadius/radius;
    } catch (...) {
        ERROR_LOG("eig fail");
    }
}

Matrix ClassicEsnReservoir::createOuterWeights(int rows, int cols, double nonZeroVal, double zeroProb) {
    Matrix result = ZerosMatrix(rows, cols);
    result.transform(
        [nonZeroVal, zeroProb](double x)->double{
            auto rnd1 = standart_rand_valued;
            if (rnd1 < zeroProb)
                return  0.0;

            auto rnd2 = standart_rand_valuef;
            return rnd2 < 0.5 ? nonZeroVal : -nonZeroVal;
        }
    );
    return result;
}

void ClassicEsnReservoir::init (int i_inpSize, int i_ofbSize) {
    int nr = d_W.n_rows;
    d_Win = createOuterWeights(nr, i_inpSize, d_inputWeightValue, d_zeroProbInInputWeight);
    d_Wofb = createOuterWeights(nr, i_ofbSize, d_feedbackWeightValue, d_zeroProbInFeedbackWeight);
}


Column ClassicEsnReservoir::forward(const Column& i_inp, const Column i_feedback) {
    Column h = totalIncomingSingnal(i_inp, i_feedback);
    d_state = (1.0 - d_timeConstant*d_timeStep*d_leakingDecayRate)*d_state + d_timeConstant*d_timeStep*h.transform(d_func);
    return d_state;
}

Column ClassicEsnReservoir::learnForward(const Column& i_inp, const Column i_feedback) {
    Column h = totalIncomingSingnalWhileLearn(i_inp, i_feedback);
    d_state = (1.0 - d_timeConstant*d_timeStep*d_leakingDecayRate)*d_state + d_timeConstant*d_timeStep*h.transform(d_func);
    return d_state;
}


Column ClassicEsnReservoir::totalIncomingSingnal(const Column& i_inp, const Column i_feedback) {
    try{
        Column result = d_Win*i_inp + d_W*d_state + d_Wofb*i_feedback;
        return result;
    } catch(...) {
        throw;
    }
}

Column ClassicEsnReservoir::totalIncomingSingnalWhileLearn(const Column& i_inp, const Column i_feedback) {
    try{
    Column result = d_Win*i_inp + d_W*d_state + d_Wofb*i_feedback + d_noiseVariance*NoiseColumn(d_W.n_rows);
    return result;
    }catch(...) {
        throw;
    }
}
