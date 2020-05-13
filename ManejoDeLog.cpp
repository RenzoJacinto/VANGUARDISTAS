#include "ManejoDeLog.h"
#include <string>
#include "LoggerError.h"
#include "LoggerInfo.h"
#include "LoggerDebug.h"
#include "Logger.h"

ManejoDeLog::ManejoDeLog(){}

bool ManejoDeLog::crearLogger(int nivel, const char* filename){
    switch(nivel){
        case nivelInfo: logger = new LoggerInfo(filename); break;
        case nivelError: logger = new LoggerError(filename); break;
        case nivelDebug: logger = new LoggerDebug(filename); break;
        default: logger = NULL;
    }
    return logger != NULL;
}

bool ManejoDeLog::iniciarLog(int nivel){
    time_t ahora = time(0);

    // junto strings para el nombre del archivo
    std::string carpeta = "logs/";
    std::string extension = ".txt";
    char* hora_actual = ctime(&ahora);
    string horaActual(hora_actual);
    carpeta = carpeta + horaActual + extension;

    const char* filename = carpeta.c_str();

    return crearLogger(nivel, filename);
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
