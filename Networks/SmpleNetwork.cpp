#include "SimpleNetwork.h"


SimpleNetwork::SimpleNetwork(const ClassicEsnReservoir&  i_res, const LinerReaduot& i_rdout, int i_skipLen, int i_learnLen, int i_generateLen) :
    d_res(i_res), d_rdout(i_rdout), d_skipLen(i_skipLen), d_learnLen(i_learnLen), d_generateLen(i_generateLen)
{}

void SimpleNetwork::learn(const DataVector& i_inp, const DataList& i_etalon) {
    DataList skipData(i_etalon.begin(), i_etalon.begin()+d_skipLen-1);
    skipData = fn::glueTwoLists({ZerosColumn(i_etalon.front().n_elem)}, skipData);
    
    d_inputVec = CONCATINATE_PAIR_VERT(i_inp, OnesColumn(1));
    d_res.init(d_inputVec.n_elem, skipData.front().n_elem);
    skip(d_inputVec, skipData);

    DataList learnData(i_etalon.begin()+d_skipLen, i_etalon.begin()+d_skipLen + d_learnLen);
    DataList resOutput;
    for (const auto& v : learnData) {
        DataVector tmp = d_res.learnForward(d_inputVec, v);
        resOutput.push_back(tmp);
    }

    d_rdout.learn(resOutput, DataList(i_etalon.begin()+d_skipLen, i_etalon.begin()+d_skipLen + d_learnLen));
    d_genInitFeedback = *(i_etalon.begin()+d_skipLen + d_learnLen+1);
}

DataList SimpleNetwork::generate() {
    DataList result;
    DataVector feedback = d_genInitFeedback;
    for (int it = 0; it < d_generateLen; ++it) {
        DataVector h = d_res.forward(d_inputVec, feedback);
        feedback = d_rdout.forward(h);
        result.push_back(feedback);
    }

    return result;
}


void SimpleNetwork::skip(const DataVector& i_inp, const DataList& i_feedback) {
    auto fbIter = i_feedback.begin();
    for (; fbIter != i_feedback.end(); ++fbIter) {
        d_res.learnForward(i_inp, *fbIter);
    }
}


void SimpleNetwork::test(const DataVector& i_inp, const DataList& i_data) {
    learn(i_inp, i_data);
    DataList genData = std::move(generate());
    
    int etalonBias = d_skipLen + d_learnLen;
    DataList etalData(i_data.begin()+etalonBias, i_data.end());
    
    double nrmeError = fn::nrmse(genData, etalData);
    fn::plotFromData(etalData, "etalon");
    fn::plotFromData(genData, "gen_data");
    CONSOLE_LOG("nrmeError : " << nrmeError);
}
