#ifndef RADIAL_RESERVOIR_H
#define RADIAL_RESERVOIR_H


#include "utils.h"

class RadialReservoir {


    public:

        RadialReservoir (int i_neuronsAmount, double i_inverseDispersion, double i_leakingDecayRate);

        void init (int i_inpSize, const DataList& i_data);
        
        Column forward(const Column& i_inp);
        


    private:

        
    private:


        Matrix d_S;
        Column d_state;

        ActivationFunc d_func;

        int d_neuronsAmount;


        double d_inverseDispersion;
        const double d_leakingDecayRate{0.9};
};

#endif