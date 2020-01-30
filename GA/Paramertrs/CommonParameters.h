#ifndef COMMON_PARAMETERS_H
#define COMMON_PARAMETERS_H


#include <map>
#include <unordered_map>
#include <string>

#include <iostream>
#include <random>
#include <memory>
#define standart_rand_valuef (float)rand()/RAND_MAX
#define standart_rand_valued (double)rand()/RAND_MAX



//  we make this class template to get
//  different params for different networks
//  but actually Net template param is unnecessary
template<class Net>
class Parameters {

    private:
        struct Value;

    public:
        using ParamMap = std::map<const char*, Value>;
        using DiapasonMap = std::map<const char*, std::pair<Value, Value>>;
        Parameters(const std::string& i_name = "");

        Parameters(const ParamMap& map);


        static void setChances(float mutation, float cross);
        static void setDiapasons(const DiapasonMap& diapason);
        static Parameters<Net> random();
    
    public:

        Value operator[] (const std::string& i_idx) const;
        Value operator[] (const char* i_idx) const;
        
        const std::string& name() const;
        void setName(const std::string& i_name);

        template<class M>
        friend Parameters<M> operator+(const Parameters<M>& i_first, const Parameters<M>& i_second);
        template<class M>
        friend std::ostream& operator<<(std::ostream& io_os, const Parameters<M>& i_param);
    
    private:

        Value& operator[] (const std::string& i_idx);
        Value& operator[] (const char* i_idx);

        void mutate();

        template<class T>
        static T mutateOne(T currentValue, T lowerBound, T upperBound);
        template<class T>
        static T changeOne(T lowerBound, T upperBound);

    private:

        struct Value {

            enum ValueType {
                INT_VAL,
                DOUBLE_VAL
            } d_type;

            union {
                int d_intVal;
                double d_doubleVal;
            };

            Value(int val);
            Value(double val = 0.0);

            Value(const Value& val);

            operator double() const;
            operator int() const;

            Value& operator=(int val);
            Value& operator=(double val);

            Value& operator=(const Value& val);

            bool isInt() const;
            bool isDouble() const;

            ValueType getTypeID() const;
        };

        template<class M>
        friend std::ostream& operator<<(std::ostream& io_os, const typename Parameters<M>::Value& val);


        std::string d_name;
        ParamMap d_paramMap;

    private:

        static float s_mutateChance;
        static float s_crossChance;

        static std::unique_ptr<Parameters<Net>> s_upperBound;
        static std::unique_ptr<Parameters<Net>> s_lowerBound;
};
template<class Net>
float Parameters<Net>::s_mutateChance = 0.0;
template<class Net>
float Parameters<Net>::s_crossChance = 0.0;

template<class Net>
std::unique_ptr<Parameters<Net>> Parameters<Net>::s_upperBound = std::make_unique<Parameters<Net>>("lowerBound");
template<class Net>
std::unique_ptr<Parameters<Net>> Parameters<Net>::s_lowerBound = std::make_unique<Parameters<Net>>("upperBound");


template<class Net>
void Parameters<Net>::setChances(float mutateChance, float crossChance) {
    s_mutateChance = mutateChance;
    s_crossChance = crossChance;
}

template<class Net>
void Parameters<Net>::setDiapasons(const Parameters<Net>::DiapasonMap& diapason) {
    for(auto& [key, valPair] : diapason) {
        (*s_lowerBound)[key] = valPair.first;
        (*s_upperBound)[key] = valPair.second;
    }
}

template<class Net>
Parameters<Net> Parameters<Net>::random() {
    Parameters<Net> result;
    
    auto lowerIt = s_lowerBound->d_paramMap.cbegin();
    auto upperIt = s_upperBound->d_paramMap.cbegin();
    for (; lowerIt != s_lowerBound->d_paramMap.cend() && 
            upperIt != s_upperBound->d_paramMap.cend(); ++lowerIt, ++upperIt) {
        auto key = lowerIt->first;
        auto valTypeID = lowerIt->second.getTypeID();
        switch (valTypeID)
        {
        case Value::INT_VAL:
            result[key] = changeOne((int)lowerIt->second, (int)upperIt->second);
            break;
        case Value::DOUBLE_VAL:
            result[key] = changeOne((double)lowerIt->second, (double)upperIt->second);
            break;
        default:
            break;
        }
    }
    
    return result;
}



template<class Net>
template<class T>
T Parameters<Net>::mutateOne(T currentValue, T lowerBound, T upperBound) {
    float rnd = standart_rand_valuef;
    return rnd < s_mutateChance ? changeOne(lowerBound, upperBound) : currentValue;
}

template<class Net>
template<class T>
T Parameters<Net>::changeOne(T lowerBound, T upperBound) {
    double theta = standart_rand_valued;
    return static_cast<T>(theta*lowerBound + (1.0 - theta)*upperBound);
}






template<class Net>
Parameters<Net>::Value::Value(int val) {
    d_type = Parameters<Net>::Value::INT_VAL;
    d_intVal = val;
}

template<class Net>
Parameters<Net>::Value::Value(double val) {
    d_type = Parameters<Net>::Value::DOUBLE_VAL;
    d_doubleVal = val;
}

template<class Net>
Parameters<Net>::Value::Value(const Value& val) {
    d_type = val.d_type;
    switch (d_type)
    {
    case Parameters<Net>::Value::INT_VAL:
        d_intVal = val.d_intVal;
        break;
    case Parameters<Net>::Value::DOUBLE_VAL:
        d_doubleVal = val.d_doubleVal;
        break;
    default:
        break;
    }
}

template<class Net>
Parameters<Net>::Value::operator int() const {
    switch (d_type)
    {
    case Parameters<Net>::Value::INT_VAL:
        return d_intVal;
        break;
    case Parameters<Net>::Value::DOUBLE_VAL:
        return static_cast<int>(d_doubleVal);
        break;
    default:
        break;
    }
    return int();
}

template<class Net>
Parameters<Net>::Value::operator double() const {
    switch (d_type)
    {
    case Parameters<Net>::Value::INT_VAL:
        return static_cast<double>(d_intVal);
        break;
    case Parameters<Net>::Value::DOUBLE_VAL:
        return d_doubleVal;
        break;
    default:
        break;
    }
    return double();
}

template<class Net>
typename Parameters<Net>::Value& Parameters<Net>::Value::operator=(int val) {
    d_type = INT_VAL;
    d_intVal = val;
    return *this;
}

template<class Net>
typename Parameters<Net>::Value& Parameters<Net>::Value::operator=(double val) {
    d_type = DOUBLE_VAL;
    d_doubleVal = val;
    return *this;
}

template<class Net>
typename Parameters<Net>::Value& Parameters<Net>::Value::operator=(const Parameters<Net>::Value& val) {
    d_type = val.d_type;
    switch (d_type)
    {
    case Parameters<Net>::Value::INT_VAL:
        d_intVal = val.d_intVal;
        break;
    case Parameters<Net>::Value::DOUBLE_VAL:
        d_doubleVal = val.d_doubleVal;
        break;
    default:
        break;
    }
    return *this;
}

template<class Net>
typename Parameters<Net>::Value::ValueType Parameters<Net>::Value::getTypeID() const {
    return d_type;
}

template<class Net>
std::ostream& operator<<(std::ostream& io_os, const typename  Parameters<Net>::Value& val) {
    switch (val.d_type)
    {
    case Parameters<Net>::Value::INT_VAL:
        io_os << val.d_intVal;
        break;
    case Parameters<Net>::Value::DOUBLE_VAL:
        io_os << val.d_doubleVal;
        break;
    default:
        break;
    }
    return io_os;
}




template<class Net>
Parameters<Net>::Parameters (const std::string& i_name ) :
    d_name(i_name)
{}

template<class Net>
Parameters<Net>::Parameters(const std::map<const char*, Parameters<Net>::Value>& map) :
    d_name("none"), d_paramMap(map)
{}

template<class Net>
typename Parameters<Net>::Value Parameters<Net>::operator[] (const std::string& i_idx) const {
    return d_paramMap.at(i_idx.c_str());
}

template<class Net>
typename Parameters<Net>::Value Parameters<Net>::operator[] (const char* i_idx) const {
    return d_paramMap.at(i_idx);
}


template<class Net>
typename Parameters<Net>::Value& Parameters<Net>::operator[] (const std::string& i_idx) {
    return d_paramMap[i_idx.c_str()];
}

template<class Net>
typename Parameters<Net>::Value& Parameters<Net>::operator[] (const char* i_idx) {
    return d_paramMap[i_idx];
}


template<class Net>
const std::string& Parameters<Net>::name() const {
    return d_name;
}

template<class Net>
void Parameters<Net>::setName(const std::string& i_name) {
    d_name = i_name;
}

template<class Net>
void Parameters<Net>::mutate() {
    
    auto it = d_paramMap.begin();
    auto lowerIt = s_lowerBound->d_paramMap.cbegin();
    auto upperIt = s_upperBound->d_paramMap.cbegin();

    for(;it != d_paramMap.end() && lowerIt != s_lowerBound->d_paramMap.cend() && 
        upperIt != s_upperBound->d_paramMap.cend(); ++lowerIt, ++upperIt, ++it) {
        auto valTypeID = it->second.getTypeID();
        switch (valTypeID)
        {
        case Value::INT_VAL:
            it->second = mutateOne((int)it->second, (int)lowerIt->second, (int)upperIt->second);
            break;
        case Value::DOUBLE_VAL:
            it->second = mutateOne((double)it->second, (double)lowerIt->second, (double)upperIt->second);
            break;
        default:
            break;
        }
        
    }
}

template<class Net>
Parameters<Net> operator+(const Parameters<Net>& i_first, const Parameters<Net>& i_second){
    Parameters<Net> result;

    auto it1 = i_first.d_paramMap.cbegin();
    auto it2 = i_second.d_paramMap.cbegin();

    for (; it1 != i_first.d_paramMap.cend() && it2 != i_second.d_paramMap.cend(); ++it1, ++it2) {
        float rnd = standart_rand_valuef;
        result[it1->first] = rnd < Parameters<Net>::s_crossChance ? it1->second : it2->second;
    }

    result.mutate();
    return result;
}

template<class Net>
std::ostream& operator<<(std::ostream& io_os, const Parameters<Net>& i_param) {
    io_os << "name : " << i_param.d_name << '\n';

    for(auto& [key, val] : i_param.d_paramMap)   {
        auto valTypeID = val.getTypeID();
        switch (valTypeID)
        {
        case Parameters<Net>::Value::INT_VAL:
            io_os << key << " : " << (int)val << '\n';
            break;
        case Parameters<Net>::Value::DOUBLE_VAL:
            io_os << key << " : " << (double)val << '\n';
            break;
        default:
            break;
        }
    }

    return io_os;
}
#endif

