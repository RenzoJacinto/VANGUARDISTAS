#include "Logger.h"
#include <iostream>
#include <fstream>
using namespace std;

Logger::Logger(){}

void Logger::debug(const char* update){}

void Logger::info(const char* update){}

void Logger::error(const char* update){}

void Logger::escribir(const char* update){
    time_t ahora = time(0);
    char* hora_actual = ctime(&ahora);
    const char* espacio = " - ";
    string guion(espacio);
    string horaActual(hora_actual);
    string sUpdate(update);
    string hora = sUpdate + guion + horaActual;
    archivo_log << hora.c_str();
}
bool Logger::abrirArchivo(const char* filename){
    archivo_log.open(filename, ios::out);
    if(!archivo_log) return false;
    return true;
}

void Logger::cerrarArchivo(){
    archivo_log.close();
}


