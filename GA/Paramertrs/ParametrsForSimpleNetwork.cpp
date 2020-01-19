#include "ParametrsForSimpleNetwork.h"


namespace {
    struct bound {
        int neuronAmount;
        double timeConstant;
        double timeStep;
        double leakingDecayRate;
        double specRadius;
        double density;
        double noiseVariance;
        double ridge;
    };


    static struct bound low = {
        50,
        0.01,
        0.01,
        +0.0,
        0.5,
        0.001,
        +0.0,
        0.01
    };

    static struct bound up = {
        500,
        1.0,
        1.0,
        1.0,
        2.0,
        0.99,
        2.0,
        1.5
    };
}

ParametersForSimpleNetwork::ParametersForSimpleNetwork(float i_mutation, float i_cross) :
    d_mutation(i_mutation), d_cross(i_cross)
{}

ParametersForSimpleNetwork ParametersForSimpleNetwork::rand(float i_mutation, float i_cross) {
    ParametersForSimpleNetwork result(i_mutation, i_cross);

    result.neuronAmount = modifyOne(low.neuronAmount, up.neuronAmount);
    result.timeConstant = modifyOne(low.timeConstant, up.timeConstant);
    result.timeStep = modifyOne(low.timeStep, up.timeStep);
    result.leakingDecayRate = modifyOne(low.leakingDecayRate, up.leakingDecayRate);
    result.specRadius = modifyOne(low.specRadius, up.specRadius);
    result.density = modifyOne(low.density, up.density);
    result.noiseVariance = modifyOne(low.noiseVariance, up.noiseVariance);
    result.ridge = modifyOne(low.ridge, up.ridge);

    return result;
}

void ParametersForSimpleNetwork::mutateAll () {
    neuronAmount = mutateOne(neuronAmount, low.neuronAmount, up.neuronAmount);
    timeConstant = mutateOne(timeConstant, low.timeConstant, up.timeConstant);
    timeStep = mutateOne(timeStep, low.timeStep, up.timeStep);
    leakingDecayRate = mutateOne(leakingDecayRate, low.leakingDecayRate, up.leakingDecayRate);
    specRadius = mutateOne(specRadius, low.specRadius, up.specRadius);
    density = mutateOne(density, low.density, up.density);
    noiseVariance = mutateOne(noiseVariance, low.noiseVariance, up.noiseVariance);
    ridge = mutateOne(ridge, low.ridge, up.ridge);
}

ParametersForSimpleNetwork operator+ (const ParametersForSimpleNetwork& i_first, const ParametersForSimpleNetwork& i_second) {
    ParametersForSimpleNetwork result (i_first.d_mutation, i_second.d_cross);

    result.neuronAmount = result.crossOne(i_first.neuronAmount, i_second.neuronAmount);
    result.timeConstant = result.crossOne(i_first.timeConstant, i_second.timeConstant);
    result.timeStep = result.crossOne(i_first.timeStep, i_second.timeStep);
    result.leakingDecayRate = result.crossOne(i_first.leakingDecayRate, i_second.leakingDecayRate);
    result.specRadius = result.crossOne(i_first.specRadius, i_second.specRadius);
    result.density = result.crossOne(i_first.density, i_second.density);
    result.noiseVariance = result.crossOne(i_first.noiseVariance, i_second.noiseVariance);
    result.ridge = result.crossOne(i_first.ridge, i_second.ridge);

    return result;
}

