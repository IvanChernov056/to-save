#ifndef GENERAL_GENETIC_ALGORITHM_H
#define GENERAL_GENETIC_ALGORITHM_H

#include <Utils/utils.h>
#include <map>
#include <list>
#include <memory>

template<class Net, class Param>
class GeneralGeneticAlgoritnm {

    public:

        GeneralGeneticAlgoritnm<Net, Param> (int i_populationSize, int i_selectionSize, int i_generationsCount, 
            float i_mutationChance, float i_crossChance);

        Param start(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen);
    
    private:
        void initialization();

        void testGeneration(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum);
        
        void getAllResults(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum);
        void selection();
        void generateNewPopulation();

        virtual std::unique_ptr<Net> makeNet(const Param& i_param) = 0;

    private:

        std::list<Param>    d_population;
        std::vector<Param>  d_selected;
        Param               d_worst;
        std::multimap<double, Param> d_allResultsPerGeneration;

        int d_populationSize;
        int d_selectionSize;
        int d_generationsCount;

        float   d_mutationChance;
        float   d_crossChance;
};

template<class Net, class Param>
GeneralGeneticAlgoritnm<Net, Param>::GeneralGeneticAlgoritnm (int i_populationSize, int i_selectionSize, int i_generationsCount, 
    float i_mutationChance, float i_crossChance) :
    d_populationSize(i_populationSize), d_selectionSize(i_selectionSize), d_generationsCount(i_generationsCount),
    d_mutationChance(i_mutationChance), d_crossChance(i_crossChance)
{
}

template<class Net, class Param>
Param GeneralGeneticAlgoritnm<Net, Param>::start(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen) {

    initialization();
    for (int generation = 1; generation <= d_generationsCount; ++generation) {
        testGeneration(i_data, i_skipLen, i_learnLen, i_generateLen, generation);
        CONSOLE_LOG("generation " << generation << '/' << d_generationsCount 
                << " : less err = " << d_allResultsPerGeneration.cbegin()->first);
    }
    return d_allResultsPerGeneration.cbegin()->second;
}

template<class Net, class Param>
void GeneralGeneticAlgoritnm<Net, Param>::initialization() {
    for (int paramNum = 0; paramNum < d_populationSize; ++paramNum)
        d_population.push_back(Param::rand(d_mutationChance, d_crossChance));
}

template<class Net, class Param>
void GeneralGeneticAlgoritnm<Net, Param>::testGeneration(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum) {
    if (d_population.empty()) initialization();
    getAllResults(i_data, i_skipLen, i_learnLen, i_generateLen, i_genNum);
    selection();
    if (i_genNum < d_generationsCount)
        generateNewPopulation();
}

template<class Net, class Param>
void GeneralGeneticAlgoritnm<Net, Param>::getAllResults(const DataList& i_data, int i_skipLen, int i_learnLen, int i_generateLen, int i_genNum) {
    if(!d_allResultsPerGeneration.empty())
        d_allResultsPerGeneration.clear();
    int paramNum = 1;
    for (const auto& param : d_population) {
        auto net = makeNet(param);
        std::string name = "gen_" + std::to_string(i_genNum) + "_num_" + std::to_string(paramNum);
        double result = net->test(i_data, i_skipLen, i_learnLen, i_generateLen, name, paramNum == 1 && i_genNum == 1);
        d_allResultsPerGeneration.emplace(std::make_pair(result, param));
        ++paramNum;
    }
}

template<class Net, class Param>
void GeneralGeneticAlgoritnm<Net, Param>::selection() {
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
void GeneralGeneticAlgoritnm<Net, Param>::generateNewPopulation() {
    if(d_selected.empty()) {
        ERROR_LOG("GEN_ALG::selected list is empty");
        return;
    }

    std::list<Param> newPopulation;
    for (int i = 0; i < d_selectionSize; ++i)
	newPopulation.push_back(d_selected[i]);

    for (auto& param : d_selected)
        newPopulation.push_back(param + d_worst);

    for (int i = 0; i < d_populationSize - 2*d_selectionSize; ++i) {
        int j = i % d_selectionSize;
        int k = (i + ::rand()) %d_selectionSize;

        newPopulation.push_back(d_selected[j] + d_selected[k]);
    }

    
    d_population.swap(newPopulation);
}


#endif
