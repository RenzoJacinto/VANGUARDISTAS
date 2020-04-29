#include "ManejoDeJson.h"
#include <stdio.h>

ManejoDeJson::ManejoDeJson(){}

bool ManejoDeJson::abrir_archivo(){
    archivo_json.open("config/configuracion.json", ios::in);
    if(!archivo_json){
        archivo_json.open("config/default.json", ios::in);
        if(!archivo_json) return false;
    }return true;
}

int ManejoDeJson::get_nivel_de_log(){
    return 0;
}

int ManejoDeJson::get_cantidad_enemigos_enemigo1(){
    return 0;
}

int ManejoDeJson::get_cantidad_enemigos_enemigo2(){
    return 0;
}

char* ManejoDeJson::get_sprite_MenuBG(){
    return NULL;
}

char* ManejoDeJson::get_sprite_MapaBG(){
    return NULL;
}

char* ManejoDeJson::get_sprite_Ciudad(){
    return NULL;
}

char* ManejoDeJson::get_sprite_Planeta(){
    return NULL;
}

char* ManejoDeJson::get_sprite_Nave_Jugador(){
    return NULL;
}

char* ManejoDeJson::get_sprite_Nave_Enemiga(){
    return NULL;
}
