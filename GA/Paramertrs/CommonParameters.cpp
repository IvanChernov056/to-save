#include "CommonParameters.h"


CommonParameters::CommonParameters(const std::string& i_name ) :
    d_name(i_name)
{
}

double CommonParameters::operator[] (const std::string& i_idx) const {
    return d_paramMap.at(i_idx.c_str());
}
double CommonParameters::operator[] (const char* i_idx) const {
    return d_paramMap.at(i_idx);
}

double& CommonParameters::operator[] (const std::string& i_idx) {
    return d_paramMap[i_idx.c_str()];
}
double& CommonParameters::operator[] (const char* i_idx) {
    return d_paramMap[i_idx];
}

const std::string& CommonParameters::name() const {
    return d_name;
}

void CommonParameters::setName(const std::string& i_name) {
    d_name = i_name;
}
