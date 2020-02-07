#include "ClassicWithPca.h"


ClassicWithPca::ClassicWithPca (int neuronAmount, double inpVal, double timeConstant, double timeStep, double leakingDecayRate, 
        double specRadius, double density, double noiseVariance, double i_inputWeightValue, double i_feedbackWeightValue, 
        double i_zeroProbInInputWeight, double i_zeroProbInFeedbackWeight, double i_reduceRatio, double ridge) 
{
    d_resPtr = std::make_unique<ClassicEsnReservoir>(neuronAmount, timeConstant, timeStep, leakingDecayRate, 
                specRadius, density, noiseVariance, i_inputWeightValue, i_feedbackWeightValue, 
                i_zeroProbInInputWeight, i_zeroProbInFeedbackWeight);
    d_rdoutPtr = std::make_unique<LinerReaduot>(ridge);
    d_reducer = std::make_unique<PcaReducer>((int)(i_reduceRatio*neuronAmount));

    d_inputVec = OnesColumn(2);
    d_inputVec[0] = inpVal;
}

double ClassicWithPca::test (const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name) {
    learn(i_data, i_skipLen, i_learnLen);
    DataList genData = generate(i_generateLen);
    int etalonBias = i_skipLen + i_learnLen;
    DataList etalData(i_data.begin()+etalonBias, i_data.end());
    
    double nrmeError = fn::nrmse(genData, etalData);
    double epsilonError = fn::epsilonNorm(genData, etalData, 1e-3);
   
    fn::plotFromData(genData, i_name.c_str());

    double testResult = epsilonError;

    return epsilonError;
}

double ClassicWithPca::test2 (const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name) {
    learn(i_data, i_skipLen, i_learnLen);
    int etalonBias = i_skipLen + i_learnLen;
    DataList etalData(i_data.begin()+etalonBias, i_data.end());
    DataList inpData(i_data.begin()+etalonBias-1, i_data.end()-1);

    DataList predictedData = predict(inpData);
    
    double nrmeError = fn::nrmse(predictedData, etalData);
    double epsilonError = fn::epsilonNorm(predictedData, etalData, 1e-3);
   
    fn::plotFromData(predictedData, i_name.c_str());

    double testResult = epsilonError;

    return epsilonError;
}



void ClassicWithPca::learn(const DataList& i_data, int i_skipLen, int i_learnLen) {
    DataList skipData(i_data.begin(), i_data.begin()+i_skipLen-1);
    skipData = fn::glueTwoLists({ZerosColumn(i_data.front().n_elem)}, skipData);
    
    d_resPtr->init(d_inputVec.n_elem, skipData.front().n_elem);
    skip(d_inputVec, skipData);

    DataList learnData(i_data.begin()+i_skipLen, i_data.begin()+i_skipLen + i_learnLen);

    d_reducer->learn(learnData);

    DataList resOutput;
    for (const auto& v : learnData) {
        DataVector tmp = d_resPtr->learnForward(d_inputVec, v);
        DataVector tmp2 = d_reducer->forward(tmp);
        resOutput.push_back(tmp2);
    }

    d_rdoutPtr->learn(resOutput, DataList(i_data.begin()+i_skipLen, i_data.begin()+i_skipLen + i_learnLen));
    d_genInitFeedback = *(i_data.begin()+i_skipLen + i_learnLen+1);
}

DataList ClassicWithPca::generate(int i_generateLen) {
    DataList result;
    DataVector feedback = d_genInitFeedback;
    for (int it = 0; it < i_generateLen; ++it) {
        DataVector h = d_resPtr->forward(d_inputVec, feedback);
        DataVector tmp = d_reducer->forward(h);
        feedback = d_rdoutPtr->forward(h);
        result.push_back(feedback);
    }

    return result;
}

DataList ClassicWithPca::predict (const DataList& i_inp) {
    DataList result;
    for (const auto& v : i_inp) {
        Column h = d_resPtr->forward(d_inputVec, v);
        Column p = d_reducer->forward(h);
        Column y = d_rdoutPtr->forward(v);
        result.push_back(y);
    }
    return result;
}

void ClassicWithPca::skip(const DataVector& i_inp, const DataList& i_feedback) {
    auto fbIter = i_feedback.begin();
    for (; fbIter != i_feedback.end(); ++fbIter) {
        d_resPtr->learnForward(i_inp, *fbIter);
    }
}