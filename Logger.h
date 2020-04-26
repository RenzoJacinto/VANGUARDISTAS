#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <fstream>
using namespace std;

class Logger{

    public:

        Logger();
        virtual void debug(const char* update);
        virtual void info(const char* update);
        virtual void error(const char* update);
        void escribir(const char* update);

        bool abrirArchivo(const char* filename);
        void cerrarArchivo();

    private:

        ofstream archivo_log;
};

#endif
