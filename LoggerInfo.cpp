#include "LoggerInfo.h"

LoggerInfo::LoggerInfo(const char* filename){
    abrirArchivo(filename);
}

void LoggerInfo::info(const char* update){
    escribir(update);
}
//void LoggerInfo::error(const char* update){}
//void LoggerInfo::escribir(const char* update){}
//bool LoggerDebug::abrirArchivo(const char* filename){}
