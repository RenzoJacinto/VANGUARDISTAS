#include "ManejoDeJson.h"
#include <stdio.h>


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

std::string ManejoDeJson::get_sprite_menu(){
    return j.at("menu");
}

int ManejoDeJson::get_nivel_de_log(){
    return j.at("log");
}

nlohmann::json& ManejoDeJson::searchValue(json& j_aux, const char* key){
    for(auto& el : j_aux.items()) {
        if(el.key() == key) return el.value();
    }
    return j_aux;
}

std::string ManejoDeJson::get_sprite_mapa(char const* key, char const* sp){

    json& j_aux = searchValue(j, "stages");
    json& j_nivel = searchValue(j_aux, key);
    json& j_sprites = searchValue(j_nivel, "sprites");

    for(auto& el : j_sprites.items()) {
        std::string sActualKey = el.key();
        if(strstr(sActualKey.c_str(),sp)){
            return el.value();
        }
    }
    return NULL;
}

int ManejoDeJson::get_cantidad_enemigo(char const* key, char const* sp){

    json& j_aux = searchValue(j, "stages");
    json& j_nivel = searchValue(j_aux, key);
    json& j_enemigos = searchValue(j_nivel, "enemigos");

    for(auto& el : j_enemigos.items()) {
        std::string sActualKey = el.key();
        if(strstr(sActualKey.c_str(),sp)){
            return el.value();
        }
    }
    return 0;
}

std::string ManejoDeJson::get_sprite_nave(char const* key, char const* sp){
    json& j_aux = searchValue(j, "naves");
    json& j_nivel = searchValue(j_aux, key);

    for(auto& el : j_nivel.items()) {
        std::string sActualKey = el.key();
        std::string sActualValue = el.value();
        if(strstr(sActualKey.c_str(),sp)){
            return sActualValue;
        }
    }
    return NULL;
}


