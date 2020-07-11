#ifndef MANEJODEJSON_H_
#define MANEJODEJSON_H_

#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"

using namespace std;

class ManejoDeJson{
    public:

        ManejoDeJson();
        bool abrir_archivo();
        bool abrir_archivo_aux(const char* filename);

        //para todos
        nlohmann::json& searchValue(nlohmann::json& j_aux, const char* key);

        int get_nivel_de_log();
        int get_cantidad_enemigo(const char* key);
        int get_max_users();

        //errores de clave
        int get_nivel_de_log_default();
        int get_cantidad_enemigo_default(const char* key);
        int get_max_users_default();


    private:

        ifstream archivo_json;
        using json = nlohmann::json;
        json j;
        json def;
};

#endif
