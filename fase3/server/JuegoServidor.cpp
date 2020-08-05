#include "JuegoServidor.h"
#include "Server.h"
#include "Nave.h"
#include "NivelServidor.h"
#include "Nivel1Servidor.h"
#include "Nivel2Servidor.h"
#include "Nivel3Servidor.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Temporizador.h"
#include <sys/socket.h>

JuegoServidor::JuegoServidor(int cant_jugadores, Server* server){

    Nivel1Servidor* nivel1 = new Nivel1Servidor();
    Nivel2Servidor* nivel2 = new Nivel2Servidor();
    Nivel3Servidor* nivel3 = new Nivel3Servidor();
    niveles.push_back(nivel1);
    niveles.push_back(nivel2);
    niveles.push_back(nivel3);

    nivel_actual = 0;
}

void JuegoServidor::iniciarJuego(Server* server, int t_niv){
    vector<NivelServidor*>::iterator nivel;
    vector<int> vidas;
    vector<int> scores;
    vector<bool> modeTests;
    for(nivel = niveles.begin(); nivel != niveles.end(); nivel++){

        (*nivel)->cargarNivel(server, server->getMaxUsers());
        if(nivel_actual!=0) (*nivel)->setScoresVidas(vidas, scores, modeTests, server->getMaxUsers(), server);
        server->crear_hilos_recibir();

        bool seguir = (*nivel)->iniciarNivel(server, t_niv);

        server->cerrar_hilos_recibir();

        server->vaciar_cola();
        (*nivel)->cerrar();
        printf("termino el nivel %d\n", nivel_actual);
        std::string msg = "Finalizo el nivel "+std::to_string(nivel_actual);
        logger.info(msg.c_str());
        if(!seguir) break;
        for(int i = time(NULL) + 10; time(NULL) != i; time(NULL));
        nivel_actual++;
        vidas = (*nivel)->getVidas();
        scores = (*nivel)->getScores();
        modeTests = (*nivel)->get_modeTests();
    }
}

void JuegoServidor::iniciar_reconexion(int id, Server* server, int socket_id){
    niveles[nivel_actual]->iniciar_reconexion(id, server, socket_id);
}

bool JuegoServidor::esValidoReconectar(){
    return niveles[nivel_actual]->esValidoReconectar();
}

int JuegoServidor::get_nivel_actual(){
    return nivel_actual;
}
