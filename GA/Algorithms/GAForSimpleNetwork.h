#ifndef GA_FOR_SIMPLE_NETWORK_H
#define GA_FOR_SIMPLE_NETWORK_H


#include "GeneralGeneticAlgorithm.h"
#include <GA/Paramertrs/ParametrsForSimpleNetwork.h>

#include <Networks/Networks.h>


class GAForSimpleNetworks : public GeneralGeneticAlgorithm<SimpleNetwork, ParametersForSimpleNetwork> {

public:
    GAForSimpleNetworks (int i_populationSize, int i_selectionSize, int i_generationsCount, float i_mutationChance, float i_crossChance) : 
    GeneralGeneticAlgorithm<SimpleNetwork, ParametersForSimpleNetwork> (i_populationSize, i_selectionSize, i_generationsCount, i_mutationChance, i_crossChance)
    {}

    virtual std::unique_ptr<SimpleNetwork> makeNet(const ParametersForSimpleNetwork& i_param) override {
        return std::make_unique<SimpleNetwork>(i_param.neuronAmount, i_param.inpVal, i_param.timeConstant, i_param.timeStep, i_param.leakingDecayRate, 
        i_param.specRadius, i_param.density, i_param.noiseVariance, i_param.ridge);
    }
};

#endif