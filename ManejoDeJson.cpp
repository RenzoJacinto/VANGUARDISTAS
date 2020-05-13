#include "ManejoDeJson.h"
#include <stdio.h>
#include "Mapa.h"
#include <list>

ManejoDeJson::ManejoDeJson(){}

bool ManejoDeJson::abrir_archivo(){
    if(!abrir_archivo_aux("config/configuracion.json")){
        printf("No se encuentra el archivo en la direccion config/configuracion.json\n");
        if(!abrir_archivo_aux("config/default.json")){
            printf("No se encuentra el archivo en la direccion config/default.json\n");
            return false;
        }
    }
    archivo_json >> j;
    archivo_json.close();
    return true;
}

bool ManejoDeJson::abrir_archivo_aux(const char* filename){
    archivo_json.open(filename, ios::in);
    if(!archivo_json) return false;
    return true;
}

const char* ManejoDeJson::get_sprite_menu(){
    std::string menu = j.at("menu");
    return menu.c_str();
}

int ManejoDeJson::get_nivel_de_log(){
    return j.at("log");
}

int ManejoDeJson::get_cantidad_enemigos(){
    return j.at("enemigos");
}

nlohmann::json& ManejoDeJson::searchValue(json& j_aux, const char* key){
    for(auto& el : j_aux.items()) {
        if(el.key() == key) return el.value();
    }
    return j_aux;
}

const char* ManejoDeJson::get_sprite_mapa(char const* key, char const* sp){

    json& j_aux = searchValue(j, "stages");
    json& j_nivel = searchValue(j_aux, key);

    for(auto& el : j_nivel.items()) {
        std::string sActual = el.value();
        if(strstr(sActual.c_str(),sp)){
            return sActual.c_str();
        }
    }
    return NULL;
}

