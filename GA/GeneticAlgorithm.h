#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <Utils/utils.h>
#include <map>
#include <list>
#include <memory>

template<class Net, class Param>
class GeneticAlgoritnm {

    public:

        GeneticAlgoritnm<Net, Param> (int i_populationSize, int i_selectionSize, int i_generationsCount);

        Param start(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen);
    
    private:
        void initialization();

        void testGeneration(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum);
        
        void getAllResults(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen);
        void selection();
        void generateNewPopulation();

        virtual std::unique_ptr<Net> makeNet(const Param& i_param) = 0;

    private:

        std::list<Param>    d_population;
        std::vector<Param>  d_selected;
        Param               d_worst;
        std::multimap<double, list<Param>::iterator> d_allResultsPerGeneration;

        int d_populationSize;
        int d_selectionSize;
        int d_generationsCount;
};

template<class Net, class Param>
GeneticAlgoritnm<Net, Param>::GeneticAlgoritnm (int i_populationSize, int i_selectionSize, int i_generationsCount) :
    d_populationSize(i_populationSize), d_selectionSize(i_selectionSize), d_generationsCount(i_generationsCount)
{
}

template<class Net, class Param>
Param GeneticAlgoritnm<Net, Param>::start(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen) {

    initialization();
    for (int generation = 1; generation <= d_generationsCount; ++generation) {
        testGeneration(i_data, i_skipLen, i_learnLen, i_generateLen, generation);
        CONSOLE_LOG("generation " << generation << '/' << d_generationsCount 
                << " : less err = " << d_allResultsPerGeneration.cbegin()->first);
    }
    return d_allResultsPerGeneration.cbegin()->second;
}

template<class Net, class Param>
void GeneticAlgoritnm<Net, Param>::initialization() {
    for (int paramNum = 0; paramNum < d_populationSize; ++paramNum)
        d_populationSize.push_back(Param::rand());
}

template<class Net, class Param>
void GeneticAlgoritnm<Net, Param>::testGeneration(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum) {
    if (d_population.empty()) initialization();
    getAllResults(i_data, i_skipLen, i_learnLen, i_generateLen);
    selection();
    if (i_genNum < d_generationCount)
        generateNewPopulation();
}

template<class Net, class Param>
void GeneticAlgoritnm<Net, Param>::getAllResults(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen) {
    if(!d_allResultsPerGeneration.empty())
        d_allResultsPerGeneration.clear();
    
    for (const auto& param : d_population) {
        auto net = makeNet(param);
        double result = net->test(i_data, i_skipLen, i_learnLen, i_generateLen);
        d_allResultsPerGeneration.emplace(std::make_pair(result, param));
    }
}

template<class Net, class Param>
void GeneticAlgoritnm<Net, Param>::selection() {
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

template<class Net, class Param>
void GeneticAlgoritnm<Net, Param>::generateNewPopulation() {
    if(d_selected.empty()) {
        ERROR_LOG("GEN_ALG::selected list is empty");
        return;
    }

    std::list<Param> newPopulation;
    for (int i = 0; i < d_populationSize - d_selectionSize; ++i) {
        int j = i % d_selectionSize;
        int k = (i + rand()) %d_selectionSize;

        newPopulation.push_back(d_selected[j] + d_selected[k]);
    }

    for (auto& param : d_selected)
        newPopulation.push_back(param + d_worst);
    
    d_population.swap(newPopulation);
}


#endif