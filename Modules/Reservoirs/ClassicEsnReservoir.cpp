#include "ClassicEsnReservoir.h"

ClassicEsnReservoir::ClassicEsnReservoir (int i_neuronAmount, ActivationFunc i_f, double i_timeConstant, double i_timeStep,
 double i_leakingDecayRate, double i_specRadius, double i_density, double i_noiseVariance) : 
    d_W((Matrix)RandnSpMatrix(i_neuronAmount, i_neuronAmount, i_density)), d_func(i_f), d_timeConstant(i_timeConstant),
    d_timeStep(i_timeStep), d_leakingDecayRate(i_leakingDecayRate), d_specRadius(i_specRadius), d_density(i_density),
    d_noiseVariance(i_noiseVariance), d_state(ZerosColumn(i_neuronAmount))
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


void ClassicEsnReservoir::init (int i_inpSize, int i_ofbSize) {
    int nr = d_W.n_rows;
    d_Win = RanduMatrix(nr, i_inpSize).transform([](double x) {return 0.015*(x - 0.5);});
    d_Wofb = RanduMatrix(nr, i_inpSize).transform([](double x) {return 0.56*(x - 0.5);});
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
    return d_Win*i_inp + d_W*d_state + d_Wofb*i_feedback;
}

Column ClassicEsnReservoir::totalIncomingSingnalWhileLearn(const Column& i_inp, const Column i_feedback) {
    return d_Win*i_inp + d_W*d_state + d_Wofb*i_feedback + d_noiseVariance*NoiseColumn(d_W.n_rows);
}
