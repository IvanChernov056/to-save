#include "RadialWithPca.h"


RadialWithPca::RadialWithPca (int i_neuronsAmount, double i_inverseDispersion, double i_leakingDecayRate, 
                                double i_reduceRatio, double ridge) 
{
    d_resPtr = std::make_unique<RadialReservoir>(i_neuronsAmount, i_inverseDispersion, i_leakingDecayRate);
    d_rdoutPtr = std::make_unique<LinerReaduot>(ridge);
    d_reducer = std::make_unique<PcaReducer>((int)(i_reduceRatio*i_neuronsAmount));

}

double RadialWithPca::test (const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name) {
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

double RadialWithPca::test2 (const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name) {
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



void RadialWithPca::learn(const DataList& i_data, int i_skipLen, int i_learnLen) {
    DataList skipData(i_data.begin(), i_data.begin()+i_skipLen-1);
    skipData = fn::glueTwoLists({ZerosColumn(i_data.front().n_elem)}, skipData);
    
    d_resPtr->init(skipData.front().n_elem, i_data);
    skip(skipData);

    DataList learnData(i_data.begin()+i_skipLen, i_data.begin()+i_skipLen + i_learnLen);

    d_reducer->learn(learnData);

    DataList resOutput;
    for (const auto& v : learnData) {
        DataVector tmp = d_resPtr->forward(v);
        DataVector tmp2 = d_reducer->forward(tmp);
        resOutput.push_back(tmp2);
    }

    d_rdoutPtr->learn(resOutput, DataList(i_data.begin()+i_skipLen, i_data.begin()+i_skipLen + i_learnLen));
    d_genInitFeedback = *(i_data.begin()+i_skipLen + i_learnLen+1);
}

DataList RadialWithPca::generate(int i_generateLen) {
    DataList result;
    DataVector feedback = d_genInitFeedback;
    for (int it = 0; it < i_generateLen; ++it) {
        DataVector h = d_resPtr->forward(feedback);
        DataVector tmp = d_reducer->forward(h);
        feedback = d_rdoutPtr->forward(h);
        result.push_back(feedback);
    }

    return result;
}

DataList RadialWithPca::predict (const DataList& i_inp) {
    DataList result;
    for (const auto& v : i_inp) {
        Column h = d_resPtr->forward(v);
        Column p = d_reducer->forward(h);
        Column y = d_rdoutPtr->forward(v);
        result.push_back(y);
    }
    return result;
}

void RadialWithPca::skip(const DataList& i_feedback) {
    auto fbIter = i_feedback.begin();
    for (; fbIter != i_feedback.end(); ++fbIter) {
        d_resPtr->forward(*fbIter);
    }
}
