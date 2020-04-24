#include "ManejoDeLog.h"
//#include <string>

ManejoDeLog::ManejoDeLog(){
    debug = true;
    info = true;
    error = true;
}

bool ManejoDeLog::iniciarLog(){
    time_t ahora = time(0);
    //const char* carpeta = "log/";
    char* hora_actual = ctime(&ahora);
    //std::string direccion = carpeta;
    //direccion.append(hora_actual);
    archivo_log.open (hora_actual, ios::out);
    if (!archivo_log){ return false;}
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

