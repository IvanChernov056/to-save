#ifndef PARAMETRS_FOR_SIMPLE_NETWORK_H
#define PARAMETRS_FOR_SIMPLE_NETWORK_H

#include <Utils/utils.h>

class ParametersForSimpleNetwork {

    public:
    
        ParametersForSimpleNetwork(float i_mutation=0.05, float i_cross=0.5);
        static ParametersForSimpleNetwork rand(float i_mutation, float i_cross);

        friend ParametersForSimpleNetwork operator+ (const ParametersForSimpleNetwork& i_first, const ParametersForSimpleNetwork& i_second);

    private:

        void mutateAll ();

        template<class T>
        T mutateOne(T i_currentValue, T i_lowBound, T i_upBound) {
            if (standart_rand_valuef < d_mutation) 
                return modifyOne(i_lowBound, i_upBound);
            return i_currentValue;
        }

        template <class T>
        static T modifyOne(T i_lowBound, T i_upBound) {
            double t = standart_rand_valuef;
            return static_cast<T>((1.0 - t)*static_cast<double>(i_lowBound) + t*static_cast<double>(i_upBound));
        }

        template<class T>
        T crossOne (T i_first, T i_second) {
            return standart_rand_valuef < d_cross ? i_first : i_second;
        } 

    public:
        
        int neuronAmount = 400;
        double inpVal = 0.2;
        double timeConstant = 0.44;
        double timeStep = 1.0;
        double leakingDecayRate = 0.9;
        double specRadius = 0.79;
        double density = 0.03;
        double noiseVariance = 1e-3;
        double ridge = 0.3;

    private:

        float d_mutation = 0.05;
        float d_cross = 0.5;
};


std::ostream& operator<< (std::ostream& io_os, const ParametersForSimpleNetwork& i_param);

#endif
