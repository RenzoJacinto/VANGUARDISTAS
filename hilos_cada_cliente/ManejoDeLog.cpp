#include "ManejoDeLog.h"
#include <string>
#include "LoggerError.h"
#include "LoggerInfo.h"
#include "LoggerDebug.h"
#include "Logger.h"
#include "global.h"

ManejoDeLog::ManejoDeLog(){
    iniciarLog();
    logg = new LoggerDebug(filename.c_str());
}

void ManejoDeLog::crearLogger(int nivel){
    logg->cerrarArchivo();
    free(logg);
    switch(nivel){
        case nivelInfo: logg = new LoggerInfo(filename.c_str()); break;
        case nivelError: logg = new LoggerError(filename.c_str()); break;
        case nivelDebug: logg = new LoggerDebug(filename.c_str()); break;
        default:
            logger.error("Nivel de log inexistente, se abre uno por default");
            crearLogger(json.get_nivel_de_log_default());
            break;
    }
}

void ManejoDeLog::iniciarLog(){
    time_t ahora = time(0);

    // junto strings para el nombre del archivo
    std::string carpeta = "logs/";
    std::string extension = ".txt";
    char* hora_actual = ctime(&ahora);
    string horaActual(hora_actual);
    carpeta = carpeta + horaActual + extension;

    filename = carpeta;
}

void ManejoDeLog::info(const char* update){
    logg->info(update);
}

void ManejoDeLog::error(const char* update){
    logg->error(update);
 }

void ManejoDeLog::debug(const char* update){
    logg->debug(update);
}

void ManejoDeLog::cerrar(){
    logg->cerrarArchivo();
}
