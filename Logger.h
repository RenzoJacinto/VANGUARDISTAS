#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <fstream>
using namespace std;

class Logger{
    public:
        Logger();
        void debug(const char* update);
        void info(const char* update);
        void error(const char* update);
        void escribir(const char* update);

        bool abrirArchivo(const char* filename);
        void cerrarArchivo();
    private:
        ofstream archivo_log;
};

#endif
