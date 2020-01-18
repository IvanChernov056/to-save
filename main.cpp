#include <Networks/Networks.h>


int main (int argc, char* argv[]) {

    if (argc != 2)
        return 0;
    
    
    int     skipLen = 1000, learnLen = 4000, genLen = 1000;
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

    ClassicEsnReservoir reservoir (400);
    LinerReaduot readout;
    SimpleNetwork   network(reservoir, readout, skipLen, learnLen, genLen);

    DataVector inpVec(1);
    inpVec[0] = 0.2;
    network.test(inpVec, data);
    
    CONSOLE_LOG("end");

    return 0;
}