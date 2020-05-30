#include "LoggerDebug.h"

LoggerDebug::LoggerDebug(const char* filename){
    abrirArchivo(filename);
}

void LoggerDebug::debug(const char* update){
    std::string info = "DEBUG: ";
    std::string update_aux = string(update);
    info = info + update_aux;
    const char* mensaje = info.c_str();
    escribir(mensaje);
}

void LoggerDebug::info(const char* update){
    std::string info = "INFO: ";
    std::string update_aux = string(update);
    info = info + update_aux;
    const char* mensaje = info.c_str();
    escribir(mensaje);
}

void LoggerDebug::error(const char* update){
    std::string info = "ERROR: ";
    std::string update_aux = string(update);
    info = info + update_aux;
    const char* mensaje = info.c_str();
    escribir(mensaje);
}
