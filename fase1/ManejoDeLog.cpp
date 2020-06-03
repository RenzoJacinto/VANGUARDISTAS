#include "ManejoDeLog.h"
#include <string>
#include "LoggerError.h"
#include "LoggerInfo.h"
#include "LoggerDebug.h"
#include "Logger.h"

ManejoDeLog::ManejoDeLog(){
    iniciarLog();
    logger = new LoggerDebug(filename.c_str());
}

bool ManejoDeLog::crearLogger(int nivel){
    bool ok = true;
    logger->cerrarArchivo();
    free(logger);
    switch(nivel){
        case nivelInfo: logger = new LoggerInfo(filename.c_str()); break;
        case nivelError: logger = new LoggerError(filename.c_str()); break;
        case nivelDebug: logger = new LoggerDebug(filename.c_str()); break;
        default: logger = new LoggerDebug(filename.c_str()); ok = false; break;
    }
    return ok;
}

void ManejoDeLog::iniciarLog(){
    time_t ahora = time(0);

    // junto strings para el nombre del archivo
    std::string carpeta = "logs/";
    std::string extension = ".txt";
    char* hora_actual = ctime(&ahora);
    string horaActual(hora_actual);
    carpeta = carpeta + horaActual + extension;

    filename = carpeta;
}

void ManejoDeLog::info(const char* update){
    logger->info(update);
}

void ManejoDeLog::error(const char* update){
    logger->error(update);
}

void ManejoDeLog::debug(const char* update){
    logger->debug(update);
}

void ManejoDeLog::cerrar(){
    logger->cerrarArchivo();
}
