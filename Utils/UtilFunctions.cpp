#include "UtilFunctions.h"
#include "macro.h"


const int LINE_LENGTH = 1024;

namespace fn {
    Matrix listToMatrix (const DataList& i_list) {
        Matrix result = i_list[0];
        auto iter = i_list.begin()+1;
        for (; iter != i_list.end(); ++iter)
            result = CONCATINATE_PAIR_HORIZ(result, *iter);
        
        return result;
    }

    DataList zipTwoLists (const DataList& i_first, const DataList& i_second) {
        DataList result;
        auto fIter = i_first.begin();
        auto sIter = i_second.begin();
        for (; fIter!=i_first.end() && sIter!=i_second.end(); ++fIter, ++sIter) {
            DataVector tmp = CONCATINATE_PAIR_VERT(*fIter, *sIter);
            result.push_back(tmp);
        }
        return result;
    }

    DataList glueTwoLists (const DataList& i_first, const DataList& i_second) {
        DataList result;
        for (const auto& v : i_first) result.push_back(v);
        for (const auto& v : i_second) result.push_back(v);
        return result;
    }

    double nrmse (const DataList& i_predicted, const DataList& i_etalon) {
        auto etalonAvrg = average(i_etalon);
        
        std::vector<double> numeratorList;
        std::vector<double> denumeratorList;

        auto predIt = i_predicted.begin();
        auto etalIt = i_etalon.begin();
        for (;predIt!=i_predicted.end()&&etalIt!=i_etalon.end(); ++predIt, ++etalIt) {
            numeratorList.push_back(sqrEuqNorm(*predIt - *etalIt));
            denumeratorList.push_back(sqrEuqNorm(*etalIt - etalonAvrg));
        }

        double numerator = average(numeratorList);
        double denumerator = average(denumeratorList);
        double result;

        if (denumerator > 0)
            result = numerator / denumerator;
        
        if (result < 0)
            throw std::runtime_error("nrmse:: negative result");
        
        return sqrt(result);
    }

    double sqrEuqNorm (const Tensor& i_tensor) {
        double nrm = 0;
        i_tensor.for_each([&nrm](double x) {
            nrm += x*x;
        });
        return nrm;
    }


    double epsilonNorm(const DataList& i_predicted, const DataList& i_etalon, double i_epsilon) {
	DataVector avrg = average(i_etalon);
	double max = +0.0;

	auto pIter = i_predicted.cbegin();
	auto eIter = i_etalon.cbegin();

	for (; pIter != i_predicted.cend() && eIter != i_etalon.cend();
					++pIter, ++eIter) {
	    double tmp = NORM2(*pIter - *eIter);
	    if (tmp > max && tmp > i_epsilon) max = tmp;
	}

	return max;
    }


    void printDataVector(const DataVector& i_vec, std::ostream& i_os) {
        for (int i = 0; i < i_vec.n_elem-1; ++i)
            i_os << i_vec[i] << '\t';
        i_os << i_vec[i_vec.n_elem-1] << '\n';
    }

    void saveDataList (const DataList& i_data, const std::string& i_file) {
        std::ofstream file(i_file);
        for(const auto& v : i_data)
            printDataVector(v, file);
        file.close();
    }

    void plotFromFile(const std::string& i_dataName, const char* i_imgType, const char* i_lineType) {
        std::ofstream commandFile("command_plot.plt");
        commandFile << "set terminal " << i_imgType << '\n'
                    << "set output \'" << i_dataName << '.' << i_imgType << "\'\n"
                    << "plot \'" << i_dataName << "\' "
                    << "with " << i_lineType
                    << "\n"
                    ;
        commandFile.close();
        system("gnuplot command_plot.plt");
    }

    void plotFromData(const DataList& i_data, const std::string& i_dataName, const char* i_imgType, const char* i_lineType) {
        saveDataList(i_data, i_dataName);
        plotFromFile(i_dataName, i_imgType, i_lineType);
    }

    int getDataVectorSize (std::istream& i_is) {
        int size=0;
        while (!i_is.eof()) {
            double tmp;
            i_is >> tmp;
	    if (!i_is.fail() && !i_is.bad())
                ++size;
        }
        i_is.seekg(0);

        return size;
    }

    void buffLine(std::istream& i_is, std::ostream& i_buff) {
        auto pos = i_is.tellg();
        char c_line[LINE_LENGTH];
        i_is.getline(c_line, LINE_LENGTH);
        i_is.seekg(pos);
        i_buff << c_line;
    }

    void skipLine(std::istream& i_is) {
        char c_line[LINE_LENGTH];
        i_is.getline(c_line, LINE_LENGTH);
    }

    DataVector readLineToVector(std::istream& i_is, int i_vecSize) {
        DataVector result(i_vecSize);
        for (int i = 0; i < i_vecSize; ++i)
            i_is >> result[i];
        return result;
    }

    void skipToStartPoint (std::istream& i_is, int i_startPoint) {
        if (i_startPoint <= 0) return;
        for (int lineNum = 0; lineNum < i_startPoint; ++lineNum)
            skipLine(i_is);
    }

    DataList readDataFromFile (int i_len, const std::string& i_file, int i_startPoint) {
        std::ifstream file (i_file);
        DataList result;

        std::stringstream lineBuffer;
        buffLine(file, lineBuffer);
        int vecSize = getDataVectorSize(lineBuffer);

        skipToStartPoint(file, i_startPoint);

        for (int lineNum = 0; lineNum < i_len && !file.eof(); ++lineNum) {
            DataVector vec = readLineToVector(file, vecSize);
            result.push_back(vec);
        }

        return result;
    }


    std::string mkdir(const std::string& dirName) {
	std::string command = std::string("~/my_local_command/mkdir.sh ") + dirName;
	system(command.c_str());
	return dirName;
    }
}
