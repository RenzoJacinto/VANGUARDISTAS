#include "ManejoDeLog.h"
#include <string>

ManejoDeLog::ManejoDeLog(){
    debug = true;
    info = true;
    error = true;
}

bool ManejoDeLog::iniciarLog(){
    time_t ahora = time(0);

    // junto strings para el nombre del archivo
    std::string carpeta = "logs/";
    char* hora_actual = ctime(&ahora);
    string horaActual(hora_actual);
    carpeta += horaActual;

    const char* filename = carpeta.c_str();

    archivo_log.open(filename, ios::out);

    if (!archivo_log) return false;
    return true;
}

void ManejoDeLog::informar(const char* update){
    archivo_log << update;
    archivo_log << "\n";
}

void ManejoDeLog::cerrar(){
    archivo_log.close();
}

bool ManejoDeLog::seDebeInformarDebug(){
    return debug;
}

bool ManejoDeLog::seDebeInformarInfo(){
    return info;
}

bool ManejoDeLog::seDebeInformarError(){
    return error;
}
