#ifndef GENERAL_GENETIC_ALGORITHM_H
#define GENERAL_GENETIC_ALGORITHM_H

#include "utils.h"
#include "CommonParameters.h"

#include <map>
#include <list>
#include <memory>

template<class Net>
class GeneralGeneticAlgorithm {

    public:

        GeneralGeneticAlgorithm<Net> (int i_populationSize, int i_selectionSize, int i_generationsCount, 
            float i_mutationChance, float i_crossChance);

        Parameters<Net> start(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen);
    
    private:
        void initialization();

        void testGeneration(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum);
        
        void getAllResults(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum);
        void selection();
        void generateNewPopulation();

        virtual std::unique_ptr<Net> makeNet(const Parameters<Net>& i_param) = 0;
        virtual void setDiapasonsForNetParameters() const = 0;

    private:

        std::list<Parameters<Net>>    d_population;
        std::vector<Parameters<Net>>  d_selected;
        Parameters<Net>               d_worst;
        std::multimap<double, Parameters<Net>> d_allResultsPerGeneration;

        int d_populationSize;
        int d_selectionSize;
        int d_generationsCount;
};

template<class Net>
GeneralGeneticAlgorithm<Net>::GeneralGeneticAlgorithm (int i_populationSize, int i_selectionSize, int i_generationsCount, 
    float i_mutationChance, float i_crossChance) :
    d_populationSize(i_populationSize), d_selectionSize(i_selectionSize), d_generationsCount(i_generationsCount)
{
    Parameters<Net>::setChances(i_mutationChance, i_crossChance);
}

template<class Net>
Parameters<Net> GeneralGeneticAlgorithm<Net>::start(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen) {

    setDiapasonsForNetParameters();
    initialization();
    for (int generation = 1; generation <= d_generationsCount; ++generation) {
        testGeneration(i_data, i_skipLen, i_learnLen, i_generateLen, generation);
        CONSOLE_LOG("generation " << generation << '/' << d_generationsCount
                << " : less err = " << d_allResultsPerGeneration.cbegin()->first);
    }
    return d_allResultsPerGeneration.cbegin()->second;
}

template<class Net>
void GeneralGeneticAlgorithm<Net>::initialization() {
    for (int paramNum = 0; paramNum < d_populationSize; ++paramNum) {
        auto param = Parameters<Net>::random();
        param.setName("num_" + std::to_string(paramNum+1));
        d_population.push_back(std::move(param));
    }
}

template<class Net>
void GeneralGeneticAlgorithm<Net>::testGeneration(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum) {
    if (d_population.empty()) initialization();
    getAllResults(i_data, i_skipLen, i_learnLen, i_generateLen, i_genNum);
    selection();
    if (i_genNum < d_generationsCount)
        generateNewPopulation();
}

template<class Net>
void GeneralGeneticAlgorithm<Net>::getAllResults(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum) {
    if(!d_allResultsPerGeneration.empty())
        d_allResultsPerGeneration.clear();

    int paramNum = 1;
    std::string dirName = fn::mkdir(std::string("gen_") + std::to_string(i_genNum));
    for (const auto& param : d_population) {
        auto net = makeNet(param);
	
    std::string path = dirName + std::string("/"); 
    double result = net->test(i_data, i_skipLen, i_learnLen, i_generateLen, path + param.name(), paramNum == 1 && i_genNum == 1);
	FILE_LOG(path + std::string("report"), param << "\nresult : " << result << '\n');
        d_allResultsPerGeneration.emplace(std::make_pair(result, param));
        ++paramNum;
    }
}

template<class Net>
void GeneralGeneticAlgorithm<Net>::selection() {
    if(d_allResultsPerGeneration.empty()) {
        ERROR_LOG("GEN_ALG::results mas is empty");
        return;
    }

    if (!d_selected.empty())
        d_selected.clear();
    
    auto it = d_allResultsPerGeneration.cbegin();
    for (int paramNum = 0; paramNum < d_selectionSize; ++paramNum) 
        if (it != d_allResultsPerGeneration.cend()) {
            d_selected.push_back(it->second);
            it++;
        } else break;
    
    d_worst = d_allResultsPerGeneration.crbegin()->second;
}

template<class Net>
void GeneralGeneticAlgorithm<Net>::generateNewPopulation() {
    if(d_selected.empty()) {
        ERROR_LOG("GEN_ALG::selected list is empty");
        return;
    }

    int paramNum = 1;
    std::list<Parameters<Net>> newPopulation;
    for (int i = 0; i < d_selectionSize; ++i) {
        newPopulation.push_back(d_selected[i]);
        newPopulation.back().setName("num_" + std::to_string(paramNum++));
    }

    for (auto& param : d_selected) {
        auto newParam = param + d_worst;
        newParam.setName("num_" + std::to_string(paramNum++));
        newPopulation.push_back(std::move(newParam));
    }

    for (int i = 0; i < d_populationSize - 2*d_selectionSize; ++i) {
        int j = i % d_selectionSize;
        int k = (i + ::rand()) %d_selectionSize;

        auto newParam = d_selected[j] + d_selected[k];
        newParam.setName("num_" + std::to_string(paramNum++));
        newPopulation.push_back(std::move(newParam));
    }

    
    d_population = std::move(newPopulation);
}


#endif
