#ifndef MANEJODELOG_H_
#define MANEJODELOG_H_

#include <iostream>
#include <fstream>
using namespace std;

class ManejoDeLog{

    public:

        ManejoDeLog();
        bool iniciarLog();
        void informar(char* update);

    private:

        ofstream archivo_log;
        bool debug;
        bool info;
        bool error;

};

#endif
