#include "LoggerError.h"

LoggerError::LoggerError(const char* filename){
    abrirArchivo(filename);
}

void LoggerError::debug(const char* update){}

void LoggerError::info(const char* update){}

void LoggerError::error(const char* update){
    std::string info = "ERROR: ";
    std::string update_aux = string(update);
    info = info + update_aux;
    const char* mensaje = info.c_str();
    escribir(mensaje);
}
