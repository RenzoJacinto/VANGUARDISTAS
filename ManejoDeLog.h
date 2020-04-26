#ifndef MANEJODELOG_H_
#define MANEJODELOG_H_

#include  "Logger.h"

class ManejoDeLog{

    public:
        static const int nivelDebug = 3;
        static const int nivelInfo = 2;
        static const int nivelError = 1;

        ManejoDeLog();
        bool crearLogger(int nivel, const char* filename);
        bool iniciarLog(int nivel);
        void cerrar();

        void info(const char* update);
        void error(const char* update);
        void debug(const char* update);

    private:
        Logger* logger = new Logger();
};

#endif

