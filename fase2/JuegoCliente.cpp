#include "Nivel.h"
#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Client.h"
#include "JuegoCliente.h"
#include "Nivel1.h"
#include "Nivel2.h"
#include "Nivel3.h"

JuegoCliente::JuegoCliente(){
    //renderizados = 1;
    menu = Menu();
    Nivel1* nivel1 = new Nivel1();
    Nivel2* nivel2 = new Nivel2();
    Nivel3* nivel3 = new Nivel3();
    niveles.push_back(nivel1);
    niveles.push_back(nivel2);
    niveles.push_back(nivel3);
}

bool JuegoCliente::iniciarSDL(){
    bool ok = true;
    if(! sdl.iniciarSDL()){
        logger.error("Fallo la inicializacion de SDL");
        ok = false;
    }
    return ok;
}

void JuegoCliente::init_menu(){

    if (menu.cargarImagen()) menu.procesar();
}

void JuegoCliente::iniciarJuego(Client* client, int nivel){
    for(int i = nivel; i<3;i++)
    {
        niveles[i]->cargarNivel(client);
        client->crear_hilo_recibir();
        if(niveles[i]->iniciarNivel(client)) break;
        niveles[i]->finalizar();
        niveles[i]->cerrar();
        //client->vaciar_cola();
        client->cerrar_hilo_recibir();
        //nivel_actual++;
    }
}

std::string JuegoCliente::get_id(){
    return menu.get_id();
}

std::string JuegoCliente::get_password(){
    return menu.get_pass();
}

void JuegoCliente::render_errorLogin(int intentos, int accion_recibida){
    std::string intentos_rest = std::to_string(intentos);
    menu.renderErrorLogin(intentos_rest.c_str(), accion_recibida);
}

void JuegoCliente::cerrarMenu(){
    menu.cerrar();
}

void JuegoCliente::renderWaitUsers(){
    menu.renderWaitUsers();

}

void JuegoCliente::cerrar_ventana()
{
    sdl.cerrar();
}

void JuegoCliente::reconectar(Client* client, int nivel)
{
    if(nivel>2) return;
    printf("entra a juego cliente\n");
    niveles[nivel]->reconectar(client);
    printf("cargo los datos de reconex\n");
    client->crear_hilo_recibir();
    if(niveles[nivel]->iniciarNivel(client)) return;
    niveles[nivel]->finalizar();
    niveles[nivel]->cerrar();
    //client->vaciar_cola();
    client->cerrar_hilo_recibir();
    iniciarJuego(client, nivel+1);
}

void JuegoCliente::reconectarSiguiente(Client* client, int nivel)
{
    if(nivel>2) return;
    client->recibir_encolar();
    client->vaciar_cola();
    iniciarJuego(client, nivel);
}
