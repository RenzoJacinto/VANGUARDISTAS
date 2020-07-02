#ifndef MANEJODELOG_H_
#define MANEJODELOG_H_

#include  "Logger.h"

class ManejoDeLog{

    public:
        static const int nivelDebug = 3;
        static const int nivelInfo = 2;
        static const int nivelError = 1;

        ManejoDeLog();
        void crearLogger(int nivel);
        void iniciarLog();
        void cerrar();

        void info(const char* update);
        void error(const char* update);
        void debug(const char* update);

    private:
        Logger* logg = new Logger();
        std::string filename;
};

#endif

