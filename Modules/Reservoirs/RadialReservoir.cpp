#include "RadialReservoir.h"
#include <set>

RadialReservoir::RadialReservoir(int i_neuronsAmount, double i_inverseDispersion, double i_leakingDecayRate) :
    d_neuronsAmount(i_neuronsAmount), d_inverseDispersion(i_inverseDispersion), d_leakingDecayRate(i_leakingDecayRate),
    d_func([](double x) {return exp(x);})
{
}


Column RadialReservoir::forward(const Column& i_inp) {
    Column h = ZerosColumn(d_neuronsAmount);

#pragma omp parallel for
    for (int i = 0; i < d_neuronsAmount; ++i) {
        Row inpT = i_inp.t();
        h[i] = d_func(d_inverseDispersion*fn::sqrEuqNorm(d_S.row(i) - inpT));
    }

    d_state = d_leakingDecayRate*h + (1.0 - d_leakingDecayRate)*d_state;
    return d_state;
}

void RadialReservoir::init (int i_inpSize, const DataList& i_data) {

    std::set<int> indexes;
    std::list<Row> rows;

    while (indexes.size() <  d_neuronsAmount) {
        int rndIdx = rand()%i_data.size();
        if (indexes.find(rndIdx) != indexes.end()) {
            indexes.insert(rndIdx);
            rows.push_back(i_data[rndIdx].t());
        }
    }
    
    auto it = rows.begin();
    for (int i = 0; it != rows.end(); ++it, ++i)
        d_S.row(i) = std::move(*it);
}