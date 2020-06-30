#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <math.h>
using namespace std;

Logger::Logger(){}

void Logger::debug(const char* update){}

void Logger::info(const char* update){}

void Logger::error(const char* update){}

void Logger::escribir(const char* update){
    char buffer[26];
    int millisec;
    struct tm* tm_info;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
    if (millisec>=1000) { // Allow for rounding up to nearest second
        millisec -=1000;
        tv.tv_sec++;
    }

    tm_info = localtime(&tv.tv_sec);

    strftime(buffer, 26, "%d/%m/%Y %H:%M:%S.", tm_info);
    std::string horaActual(buffer);
    std:string ms = std::to_string(millisec);

    const char* espacio = " - ";
    const char* new_line = "\n";
    string guion(espacio);
    string sUpdate(update);
    string newLine(new_line);
    string hora = horaActual + ms + guion + sUpdate + newLine;
    archivo_log << hora.c_str();
}
bool Logger::abrirArchivo(const char* filename){
    archivo_log.open(filename, ios::app);
    if(!archivo_log) return false;
    return true;
}

void Logger::cerrarArchivo(){
    archivo_log.close();
}


