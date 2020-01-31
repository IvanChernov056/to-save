#ifndef GA_FOR_SIMPLE_NETWORK_H
#define GA_FOR_SIMPLE_NETWORK_H


#include "GeneralGeneticAlgorithm.h"
#include "Networks.h"


class GAForSimpleNetworks : public GeneralGeneticAlgorithm<SimpleNetwork> {

public:
    GAForSimpleNetworks (int i_populationSize, int i_selectionSize, int i_generationsCount, float i_mutationChance, float i_crossChance) : 
    GeneralGeneticAlgorithm<SimpleNetwork> (i_populationSize, i_selectionSize, i_generationsCount, i_mutationChance, i_crossChance)
    {}
private:
    virtual std::unique_ptr<SimpleNetwork> makeNet(const Parameters<SimpleNetwork>& i_param) override {
        return std::make_unique<SimpleNetwork>((int)i_param["neuronAmount"], (double)i_param["inpVal"], (double)i_param["timeConstant"], 
        (double)i_param["timeStep"], (double)i_param["leakingDecayRate"], (double)i_param["specRadius"], (double)i_param["density"], 
        (double)i_param["noiseVariance"], (double)i_param["inputWeightValue"], (double)i_param["feedbackWeightValue"], 
        (double)i_param["zeroProbInInputWeight"], (double)i_param["zeroProbInFeedbackWeight"], (double)i_param["ridge"]);
    }

    virtual void setDiapasonsForNetParameters() const override{
        Parameters<SimpleNetwork>::setDiapasons(
            Parameters<SimpleNetwork>::DiapasonMap(
                {
                    {"neuronAmount", {50, 500}},
                    {"inpVal", {0.1, 0.3}},
                    {"timeConstant", {0.01, 1.0}},
                    {"timeStep", {0.01, 1.0}},
                    {"leakingDecayRate", {+0.0, 1.0}},
                    {"specRadius", {0.5, 2.0}},
                    {"density", {0.001, 0.99}},
                    {"noiseVariance", {+0.0, 2.0}},
                    {"ridge", {0.01, 1.5}},
                    {"inputWeightValue", {0.05, 0.5}},
                    {"feedbackWeightValue", {0.05, 1.0}},
                    {"zeroProbInInputWeight", {0.0, 1.0}},
                    {"zeroProbInFeedbackWeight", {0.0, 1.0}}
                }
            )
        );
    }
};

#endif