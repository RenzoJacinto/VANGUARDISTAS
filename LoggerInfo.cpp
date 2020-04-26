#include "LoggerInfo.h"

LoggerInfo::LoggerInfo(const char* filename){
    abrirArchivo(filename);
}

void LoggerInfo::debug(const char* update){}

void LoggerInfo::info(const char* update){
    escribir(update);
}

void LoggerInfo::error(const char* update){
    escribir(update);
}
