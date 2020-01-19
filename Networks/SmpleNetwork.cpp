#include "SimpleNetwork.h"

SimpleNetwork::SimpleNetwork(int neuronAmount, double inpVal, double timeConstant, double timeStep, double leakingDecayRate, 
        double specRadius, double density, double noiseVariance, double ridge) 
{
    d_resPtr = std::make_unique<ClassicEsnReservoir>(neuronAmount, timeConstant, timeStep, leakingDecayRate, 
                specRadius, density, noiseVariance);
    d_rdoutPtr = std::make_unique<LinerReaduot>(ridge);

    d_inputVec = OnesColumn(2);
    d_inputVec[0] = inpVal;
}
void SimpleNetwork::learn(const DataList& i_etalon, int i_skipLen, int i_learnLen) {
    DataList skipData(i_etalon.begin(), i_etalon.begin()+i_skipLen-1);
    skipData = fn::glueTwoLists({ZerosColumn(i_etalon.front().n_elem)}, skipData);
    
    d_resPtr->init(d_inputVec.n_elem, skipData.front().n_elem);
    skip(d_inputVec, skipData);

    DataList learnData(i_etalon.begin()+i_skipLen, i_etalon.begin()+i_skipLen + i_learnLen);
    DataList resOutput;
    for (const auto& v : learnData) {
        DataVector tmp = d_resPtr->learnForward(d_inputVec, v);
        resOutput.push_back(tmp);
    }

    d_rdoutPtr->learn(resOutput, DataList(i_etalon.begin()+i_skipLen, i_etalon.begin()+i_skipLen + i_learnLen));
    d_genInitFeedback = *(i_etalon.begin()+i_skipLen + i_learnLen+1);
}

DataList SimpleNetwork::generate(int i_generateLen) {
    DataList result;
    DataVector feedback = d_genInitFeedback;
    for (int it = 0; it < i_generateLen; ++it) {
        DataVector h = d_resPtr->forward(d_inputVec, feedback);
        feedback = d_rdoutPtr->forward(h);
        result.push_back(feedback);
    }

    return result;
}


void SimpleNetwork::skip(const DataVector& i_inp, const DataList& i_feedback) {
    auto fbIter = i_feedback.begin();
    for (; fbIter != i_feedback.end(); ++fbIter) {
        d_resPtr->learnForward(i_inp, *fbIter);
    }
}


double SimpleNetwork::test(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name, bool drawEtalon) {
    
    CONSOLE_LOG("name : " << i_name);
    CONSOLE_LOG("for skip : " << i_skipLen);
    CONSOLE_LOG("for learn : " << i_learnLen);
    CONSOLE_LOG("for generate : " << i_generateLen);

    learn(i_data, i_skipLen, i_learnLen);
    DataList genData = generate(i_generateLen);
    int etalonBias = i_skipLen + i_learnLen;
    DataList etalData(i_data.begin()+etalonBias, i_data.end());
    
    double nrmeError = fn::nrmse(genData, etalData);
    if(drawEtalon) fn::plotFromData(etalData, "etalon");
    fn::plotFromData(genData, i_name.c_str());
    CONSOLE_LOG("nrmeError : " << nrmeError);

    return nrmeError;
}
