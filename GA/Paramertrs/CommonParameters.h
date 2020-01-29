#ifndef COMMON_PARAMETERS_H
#define COMMON_PARAMETERS_H

#include "utils.h"

#include <map>
#include <unordered_map>
#include <string>


class CommonParameters {

    public:
        CommonParameters(const std::string& i_name = "");

        double operator[] (const std::string& i_idx) const;
        double operator[] (const char* i_idx) const;

        double& operator[] (const std::string& i_idx);
        double& operator[] (const char* i_idx);
        
        const std::string& name() const;
        void setName(const std::string& i_name);

        template<class Param>
        friend Param operator+(const Param& i_first, const Param& i_second);
        template<class Param>
        friend std::ostream& operator<<(std::ostream& io_os, const Param& i_param);
    
    protected:

        virtual void mutate() = 0;

    protected:

        std::string d_name;
        std::unordered_map<const char*, double> d_paramMap;

};

template<class Param>
Param operator+(const Param& i_first, const Param& i_second){
    Param result;

    auto it1 = i_first.d_paramMap.cbegin();
    auto it2 = i_second.d_paramMap.cbegin();

    for (; it1 != i_first.d_paramMap.cend() && it2 != i_second.d_paramMap.cend(); ++it1, ++it2) {
        float rnd = standart_rand_valuef;
        result[it1->first] = rnd < Param::s_crossChance ? it1->second : it2->second;
    }

    result.mutate();
    return result;
}

template<class Param>
std::ostream& operator<<(std::ostream& io_os, const Param& i_param) {
    io_os << "name : " << i_param.d_name << '\n';

    for(auto& [key, val] : i_param.d_paramMap)  
        io_os << key << " : " << val << '\n';

    return io_os;
}
#endif