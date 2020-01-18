#include "LinerReadout.h"

LinerReaduot::LinerReaduot(LearnMode i_mode, double i_ridge) :
    d_mode(i_mode), d_ridge(i_ridge)
{
}


Column LinerReaduot::forward(const Column& i_inp) {
    return d_Wout*i_inp;
}

void LinerReaduot::learn(const DataList& i_inp, const DataList& i_etalon) {
    switch (d_mode)
    {
    case RIDGE:
        ridgeLearn(i_inp, i_etalon);
        break;

    case DIRECT_PINV:
        directPinvLearn(i_inp, i_etalon);
        break;

    case INDIRECT_PINV:
        indirectPinvLearn(i_inp, i_etalon);
        break;
    default:
        break;
    }
}



void LinerReaduot::directPinvLearn(const DataList& i_inp, const DataList& i_etalon) {
    Matrix X = fn::listToMatrix(i_inp);
    Matrix Y = fn::listToMatrix(i_etalon);

    d_Wout = Y*PINV(X);
}
void LinerReaduot::indirectPinvLearn(const DataList& i_inp, const DataList& i_etalon) {
    Matrix X = fn::listToMatrix(i_inp);
    Matrix Y = fn::listToMatrix(i_etalon)*X.t();

    X = X*X.t();

    d_Wout = Y*PINV(X);
}
void LinerReaduot::ridgeLearn(const DataList& i_inp, const DataList& i_etalon) {
    Matrix X = fn::listToMatrix(i_inp);
    Matrix Y = fn::listToMatrix(i_etalon)*X.t();

    d_Wout = Y*INV_SYMPD(X*X.t() + d_ridge*EyeMatrix(X.n_rows));
}