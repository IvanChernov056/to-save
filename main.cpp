#include <Networks/Networks.h>
#include <GA/Algorithms/GAForSimpleNetwork.h>


int main (int argc, char* argv[]) {

    if (argc != 2)
        return 0;
    
    
    int     skipLen = 1000, learnLen = 8000, genLen = 1500;
    int     totalLen = skipLen+learnLen+genLen;
    DataList data = fn::readDataFromFile(totalLen, argv[1], 1000);


    CONSOLE_LOG("start : " << data.front().n_elem);
    if (data.front().n_elem == 0)
        return 1;
    
    for (auto& v : data) {
        if (v.n_elem == 0)
        return -1;
    }

    // DataList etalon(data.begin()+skipLen + learnLen, data.end());
    // fn::plotFromData(etalon, "tmp_etalon");
    // fn::plotFromData(data, "tmp_data");

    // return 0;


    GAForSimpleNetworks alg(200, 20, 100, 0.05, 0.5);
    auto param = alg.start(data, skipLen, learnLen, genLen);

    std::ofstream outParamFile("out_param_file.param");
    outParamFile << param;
    outParamFile.close();


    return 0;
}
