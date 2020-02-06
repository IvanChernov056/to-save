#ifndef UTIL_FUNCTIONS_H
#define UTIL_FUNCTIONS_H

#include "types.h"
#include "macro.h"

namespace fn {
    Matrix listToMatrix (const DataList& i_list);
    DataList zipTwoLists (const DataList& i_first, const DataList& i_second);
    DataList glueTwoLists (const DataList& i_first, const DataList& i_second);

    Matrix getPcaMatrix(const Matrix& i_data, int i_outDim);
    double nrmse (const DataList& i_predict, const DataList& i_etalon);
    double epsilonNorm (const DataList& i_predict, const DataList& i_etalon, double i_epsilon);
    double sqrEuqNorm (const Tensor& i_tensor);

    template<class T>
    T average (const std::vector<T>& i_list) {
        if(i_list.empty()) 
            throw std::runtime_error("avrg::empty list");

        T avrg = i_list.front();
        auto it = i_list.begin()+1;
        for (; it!=i_list.end(); ++it) { 
            avrg += *it;
        }

        return avrg / i_list.size();
    }

    template<class T>
    T average(T* i_data, int i_len) {
        if (i_len <= 0)
            throw std::runtime_error("average : len must be greater then zero");

        T avrg = *i_data;
        for (int i = 1; i < i_len; i++)
            avrg += *(i_data+i);
        
        return avrg/i_len;
    }

    void saveDataList (const DataList& i_data, const std::string& i_file);
    void plotFromFile(const std::string& i_dataName, const char* i_imgType = "png", const char* i_lineType = "lines");
    void plotFromData(const DataList& i_data, const std::string& i_dataName = "tmp_data.dat", const char* i_imgType = "png", const char* i_lineType = "lines");
    DataList readDataFromFile (int i_len, const std::string& i_file, int i_startPoint = 0);
    std::string mkdir(const std::string& dirName);
    void rm(const std::string& toRemove, const std::string& keys = "");

    void logMessageToFile(const std::string& i_fileName = "", const std::string& i_message = "");
}


#endif
