#include "ManejoDeJson.h"
#include <stdio.h>
#include "global.h"


ManejoDeJson::ManejoDeJson(){}

bool ManejoDeJson::abrir_archivo(){
    bool abrio_config = true;
    if(!abrir_archivo_aux("config/configuracion.json")){
        abrio_config = false;
        logger.error("No se encuentra el archivo en la direccion config/configuracion.json");
    } else {
        try{archivo_json >> j;}
        catch(nlohmann::detail::parse_error ){
            logger.error("El archivo de configuracion.json esta creado incorrectamente");
            abrio_config = false;
        }
        archivo_json.close();
    }
    if(!abrir_archivo_aux("config/default.json")){
        logger.error("No se encuentra el archivo en la direccion config/default.json");
        return false;
    } else {
        try{archivo_json >> def;}
        catch(nlohmann::detail::parse_error){
            logger.error("El archivo default.json esta creado incorrectamente");
        }

        if(!abrio_config) j = def;
        archivo_json.close();
    }

    return true;
}

bool ManejoDeJson::abrir_archivo_aux(const char* filename){
    archivo_json.open(filename, ios::in);
    if(!archivo_json) return false;
    return true;
}

nlohmann::json& ManejoDeJson::searchValue(json& j_aux, const char* key){
    for(auto& el : j_aux.items()) {
        if(el.key() == key) return el.value();
    }
    string clave(key);
    std::string mensaje = "No se encontro la clave: " + clave;
    logger.error(mensaje.c_str());
    return def.at("error");
}

int ManejoDeJson::get_nivel_de_log(){
    try { return j.at("log"); }
    catch (nlohmann::detail::out_of_range) {
        logger.error("No se encuentra el log. Se abre el nivel de log de default.json");
        return get_nivel_de_log_default();
    }
    catch(nlohmann::detail::type_error){
        logger.error("El nivel de log debe ser un numero y es un string. Se abre el nivel de log de default.json");
        return get_nivel_de_log_default();
    }
}

int ManejoDeJson::get_cantidad_enemigo(const char* key){

    json& j_aux = searchValue(j, "stages");
    if(j_aux == "errorKey") return get_cantidad_enemigo_default(key);

    try{return j_aux.at(key);}
    catch (nlohmann::detail::out_of_range) {
        std::string mensaje1 = "No se encontro la clave enemigos. Se obtiene de default.json";
        logger.error(mensaje1.c_str());
        return get_cantidad_enemigo_default(key);
    }
    catch(nlohmann::detail::type_error){
        std::string mensaje1 = "La cantidad de enemigos, debe ser un numero y es un string. Se obtiene de default.json";
        logger.error(mensaje1.c_str());
        return get_cantidad_enemigo_default(key);
    }
}

// PARA LOS DE DEFAULT (SOLO LLAMADAS EN CASOS DE ERROR DE CLAVE)

int ManejoDeJson::get_nivel_de_log_default(){
    logger.debug("Se obtuvo el nivel de log de default.json");
    return def.at("log");
}

int ManejoDeJson::get_cantidad_enemigo_default(const char* key){

    json& j_aux = searchValue(def, "stages");

    logger.debug("Se obtuvo de default.json la cantidad de enemigos");

    return j_aux.at(key);
}

int ManejoDeJson::get_max_users_default(){
    logger.debug("Se obtuvo la cantidad de users del archivo de default.json");
    return j.at("users");
}


// PARA EL SERVER
int ManejoDeJson::get_max_users(){

    try { return j.at("users"); }
    catch (nlohmann::detail::out_of_range) {
        logger.error("No se encuentra la cantidad de clientes");
        return get_max_users_default();
    }
    catch(nlohmann::detail::type_error){
        logger.error("La cantidad maxima de users debe ser un numero y es un string");
        return get_max_users_default();
    }
}

