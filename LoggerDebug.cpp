#include "LoggerDebug.h"

LoggerDebug::LoggerDebug(const char* filename){
    abrirArchivo(filename);
}

void LoggerDebug::debug(const char* update){
    escribir(update);
}

void LoggerDebug::info(const char* update){
    escribir(update);
}

void LoggerDebug::error(const char* update){
    escribir(update);
}
