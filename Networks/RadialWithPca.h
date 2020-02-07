#ifndef RADIAL_WITH_PCA_H
#define RADIAL_WITH_PCA_H

#include "RadialReservoir.h"
#include "PcaReducer.h"
#include "LinerReadout.h"


class RadialWithPca {

    public:

        RadialWithPca (int i_neuronsAmount, double i_inverseDispersion, double i_leakingDecayRate, double i_reduceRatio, double ridge);

        double test (const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name);
        double test2 (const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, const std::string& i_name);

    private:

        void learn(const DataList& i_data, int i_skipLen, int i_learnLen);
        DataList generate(int i_generateLen);
        DataList predict (const DataList& i_inp);
        void skip(const DataList& i_feedback);

    private:


        std::unique_ptr<RadialReservoir> d_resPtr;
        std::unique_ptr<PcaReducer>          d_reducer;
        std::unique_ptr<LinerReaduot>        d_rdoutPtr;

        Column  d_genInitFeedback;

};


#endif