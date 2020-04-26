#include "LoggerError.h"

LoggerError::LoggerError(const char* filename){
    abrirArchivo(filename);
}

//void LoggerError::debug(const char* update){}
//void LoggerError::info(const char* update){}
void LoggerError::error(const char* update){
    escribir(update);
}
//void LoggerError::escribir(const char* update){}
//bool LoggerError::abrirArchivo(const char* filename){}
