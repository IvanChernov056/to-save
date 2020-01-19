#ifndef GA_FOR_SIMPLE_NETWORK_H
#define GA_FOR_SIMPLE_NETWORK_H


#include "Algorithms/GeneralGeneticAlgorithm.h"
#include "ParametrsForSimpleNetwork.h"

#include <Networks/Networks.h>


class GAForSimpleNetworks : public GeneralGeneticAlgoritnm<SimpleNetwork, ParametersForSimpleNetwork> {

public:
    GAForSimpleNetworks (int i_populationSize, int i_selectionSize, int i_generationsCount, float i_mutationChance, float i_crossChance) : 
    GeneralGeneticAlgoritnm<SimpleNetwork, ParametersForSimpleNetwork> (i_populationSize, i_selectionSize, i_generationsCount, i_mutationChance, i_crossChance)
    {}

    virtual std::unique_ptr<SimpleNetwork> makeNet(const ParametersForSimpleNetwork& i_param) override {
        
    }
};

#endif