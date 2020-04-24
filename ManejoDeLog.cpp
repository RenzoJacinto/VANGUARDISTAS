#include "ManejoDeLog.h"

ManejoDeLog::ManejoDeLog(){
    debug = true;
    info = true;
    error = true;
}

bool ManejoDeLog::iniciarLog(){
    archivo_log.open ("log.txt", ios::out);
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

