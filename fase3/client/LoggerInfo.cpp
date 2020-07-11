#include "LoggerInfo.h"

LoggerInfo::LoggerInfo(const char* filename){
    abrirArchivo(filename);
}

void LoggerInfo::debug(const char* update){}

void LoggerInfo::info(const char* update){
    std::string info = "INFO: ";
    std::string update_aux = string(update);
    info = info + update_aux;
    const char* mensaje = info.c_str();
    escribir(mensaje);
}

void LoggerInfo::error(const char* update){
    std::string info = "ERROR: ";
    std::string update_aux = string(update);
    info = info + update_aux;
    const char* mensaje = info.c_str();
    escribir(mensaje);
}
