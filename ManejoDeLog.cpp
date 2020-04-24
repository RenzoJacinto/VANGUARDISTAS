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

void ManejoDeLog::informar(char* update){
    archivo_log << update;
}


