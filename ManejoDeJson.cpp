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
    cargar_config();
    return true;
}

bool ManejoDeJson::abrir_archivo_aux(const char* filename){
    archivo_json.open(filename, ios::in);
    if(!archivo_json) return false;
    return true;
}

void ManejoDeJson::cargar_config(){
    for (auto& el : j.items()) {
        //std::cout << el.key() << " : " << el.value() << "\n";
        if(el.key() == "enemigos") cargar_enemigos(el.value());
        if(el.key() == "log") config.log = el.value();
        if(el.key() == "stages") cargar_stages(el.value());
        if(el.key() == "jugador") config.jugador = el.value();
    }

}

void ManejoDeJson::cargar_stages(json& j_aux){
    for (auto& el : j_aux.items()) {
        if(el.key() == "menuBG") config.menuBG = el.value();
        if(el.key() == "mapaBG") config.mapaBG = el.value();
        if(el.key() == "ciudad") config.ciudad = el.value();
        if(el.key() == "planeta") config.planeta = el.value();
    }
}

void ManejoDeJson::cargar_enemigo1(json& j_aux){
    for (auto& el : j_aux.items()) {
        if(el.key() == "cantidad") config.cant_enemigo1 = el.value();
        if(el.key() == "sprite") config.sprite_enemigo1 = el.value();
    }
}

void ManejoDeJson::cargar_enemigo2(json& j_aux){
    for (auto& el : j_aux.items()) {
        if(el.key() == "cantidad") config.cant_enemigo2 = el.value();
        if(el.key() == "sprite") config.sprite_enemigo2 = el.value();
    }
}

void ManejoDeJson::cargar_enemigos(json& j_aux){
    for (auto& el : j_aux.items()) {
        if(el.key() == "enemigo1") cargar_enemigo1(el.value());
        if(el.key() == "enemigo2") cargar_enemigo2(el.value());
    }
}

int ManejoDeJson::get_nivel_de_log(){
    return config.log;
}

int ManejoDeJson::get_cantidad_enemigos_enemigo1(){
    return config.cant_enemigo1;
}

int ManejoDeJson::get_cantidad_enemigos_enemigo2(){
    return config.cant_enemigo2;
}

const char* ManejoDeJson::get_sprite_MenuBG(){
    return config.menuBG.c_str();
}

const char* ManejoDeJson::get_sprite_MapaBG(){
    return config.mapaBG.c_str();
}

const char* ManejoDeJson::get_sprite_Ciudad(){
    return config.ciudad.c_str();
}

const char* ManejoDeJson::get_sprite_Planeta(){
    return config.planeta.c_str();
}

const char* ManejoDeJson::get_sprite_Nave_Jugador(){
    return config.jugador.c_str();
}

const char* ManejoDeJson::get_sprite_Enemigo_Default(){
    return NULL;
}

const char* ManejoDeJson::get_sprite_Enemigo1(){
    return config.sprite_enemigo1.c_str();
}

const char* ManejoDeJson::get_sprite_Enemigo2(){
    return config.sprite_enemigo2.c_str();
}

const char* ManejoDeJson::get_sprite_Enemigo3(){
    return NULL;
}

const char* ManejoDeJson::get_sprite_Enemigo4(){
    return NULL;
}

