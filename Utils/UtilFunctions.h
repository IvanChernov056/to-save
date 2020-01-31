#ifndef UTIL_FUNCTIONS_H
#define UTIL_FUNCTIONS_H

#include "types.h"
#include "macro.h"

namespace fn {
    Matrix listToMatrix (const DataList& i_list);
    DataList zipTwoLists (const DataList& i_first, const DataList& i_second);
    DataList glueTwoLists (const DataList& i_first, const DataList& i_second);

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

    void saveDataList (const DataList& i_data, const std::string& i_file);
    void plotFromFile(const std::string& i_dataName, const char* i_imgType = "png", const char* i_lineType = "lines");
    void plotFromData(const DataList& i_data, const std::string& i_dataName = "tmp_data.dat", const char* i_imgType = "png", const char* i_lineType = "lines");
    DataList readDataFromFile (int i_len, const std::string& i_file, int i_startPoint = 0);
    std::string mkdir(const std::string& dirName);

    void logMessageToFile(const std::string& i_fileName = "", const std::string& i_message = "");
}


#endif
