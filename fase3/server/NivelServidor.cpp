#include "NivelServidor.h"
#include "Temporizador.h"
#include "Server.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Enemigo1.h"
#include "Enemigo2.h"
#include "Enemigo3.h"
#include "Enemigo4.h"
#include "Nave.h"
#include <sys/socket.h>
#include "time_nanoseconds.h"

class Server;

NivelServidor::NivelServidor(){
    tiempo_transcurrido = 0;
    renderizados = 1;
}

void NivelServidor::cargarNivel(Server* server, int cant_jugadores){}

void NivelServidor::parallax(){}

void NivelServidor::iniciar_reconexion(int id, Server* server, int socket_id){}

void NivelServidor::iniciarNivel(Server* server, int t_niv){
    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
    Temporizador temporizador;
    temporizador.iniciar();

    float tiempo_por_enemigos = TIEMPO_NIVEL_SEGS/cant_enemigos;
    //int renderizados = 1;

    char fin_nivel[15];
    strcpy(fin_nivel, "fin");

    death_enemies = 0;

    int cant_jug = jugadores.size();
    for(int i=0; i<cant_jug; i++){
        score_nivel.push_back(0);
    }
    Temporizador finNiv;
    //while( tiempo_transcurrido < TIEMPO_NIVEL_SEGS ) {
    while( enemigosSiganVivos() || finNiv.transcurridoEnSegundos() <= 5) {

        if(enemigosSiganVivos()) finNiv.iniciar();

        if(jugadoresMuertos()){
            // Enviar GAME OVER
            strcpy(fin_nivel, "gameOver");
            break;
        }

        //if(server->cola_esta_vacia()) std::cout<<"ESTA VACIA LA COLA\n";
        while(! server->cola_esta_vacia()){
            void* dato = server->desencolar();
            procesar(server, (velocidades_t*)dato);
        }


        //printf("encola enemigo\n");
        tiempo_transcurrido = temporizador.transcurridoEnSegundos();
        if(tiempo_transcurrido/renderizados > tiempo_por_enemigos && renderizados<cant_enemigos) renderizados++;

        velocidades_t* v = create_velocidad(renderizados+4, "rend", 0, 0);
        server->encolar(v);
    }
    posiciones_t* pos = create_posicion(-1, fin_nivel, 0, 0);
    server->send_all(pos);
    free(pos);

    vector<NaveJugador*>::iterator posJ;
    for(posJ = jugadores.begin(); posJ != jugadores.end(); posJ++){
        vidas.push_back((*posJ)->getVidas());
        scores.push_back((*posJ)->getScore());
        mode_tests.push_back((*posJ)->inModeTest());
    }

    enviar_scores(server);
}

void NivelServidor::procesar(Server* server, velocidades_t* v){

    recibeNone(server, v);
    if(recibeModoTest(server,v)) return;
    if(! recibePrimerDisparo(server,v)){
        if(! recibeNaveEnemiga(server,v)){
            if(! recibeNaveJugador(server, v)) recibeJugadorDesconectado(server, v);
        }
    }
    free(v);
}

bool NivelServidor::esValidoReconectar(){
    return TIEMPO_NIVEL_SEGS - tiempo_transcurrido > 0;
}

void NivelServidor::setNaves(Server* server, int cant_jugadores){
    for(int i = 0; i<cant_jugadores; i++){
        NaveJugador* nave = new NaveJugador(200, 100*(i+1), i);
        jugadores.push_back(nave);

        char descrip[5];
        if(server->desconecto(i)) {
            printf("nave %d desconectada\n", i);
            strcpy(descrip, "off");
        }
        else strcpy(descrip, "on");
        posiciones_t* pos = create_posicion(i, descrip, 200, 100*(i+1));
        server->send_all(pos);
        //printf("SERVER: se crea nave jugador, id: %d\n", pos->id);
        free(pos);
    }

    for(int i = 0; i<cant_enemigos; i++){
        std::string sprite = "enemigo";
        int enemigo_random = 1 + randomNumber() % 4;
        sprite += std::to_string(enemigo_random);

        // Obtencion de la posicion pos = inf + rand()%(sup+1-inf)
        int y = 110 + rand() % (SCREEN_HEIGHT - 100 + 1);
        // SUPONGO EL BORDE DE RESPAWN COMO +/-100
        // CASO ENEMIGOS 1 y 2: sup = 800 + 100
        int x = SCREEN_WIDTH + 50 + rand() % 51;

        // CASO ENEMIGOS 3 y 4: inf = -100
        if(enemigo_random == 4 || enemigo_random == 3) x = -100 + rand() % 51;

        NaveEnemiga* enemigo;
        switch(enemigo_random){
            case 1: enemigo = new Enemigo1(x,y); break;
            case 2: enemigo = new Enemigo2(x,y); break;
            case 3: enemigo = new Enemigo3(x,y); break;
            case 4: enemigo = new Enemigo4(x,y); break;
        }

        //NaveEnemiga* enemigo = new NaveEnemiga(x, y, sprite.c_str(), jugadores);

        enemigos.push_back(enemigo);
        posiciones_t* pos = create_posicion(i+4, enemigo->getImagen(), x ,y);

        server->send_all(pos);
        free(pos);
        //printf("se crea nave enemiga\n");
    }
}

void NivelServidor::setScoresVidas(vector<int> vidas, vector<int> scores, vector<bool> modeTests, int cant_jugadores, Server* server){
    for(int i = 0; i<cant_jugadores; i++){
        posiciones_t* pos = create_posicion(i, "data", vidas[i] ,scores[i]);
        jugadores[i]->addScore(scores[i]);
        jugadores[i]->setVidas(vidas[i]);
        server->send_all(pos);

        if(modeTests[i]){
            pos->id = i;
            strcpy(pos->descrip, "test");
            pos->posX = 0;
            pos->posY = 0;
            jugadores[i]->set_modeTest();
            server->send_all(pos);
        }
        free(pos);
    }
    posiciones_t* pos = create_posicion(-1, "a", 0, 0);
    server->send_all(pos);
    free(pos);
}

velocidades_t* NivelServidor::create_velocidad(int id, const char* descrip, int x, int y){
    velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
    v->VelX = x;
    v->VelY = y;
    strcpy(v->descrip, descrip);
    v->id = id;

    return v;
}

posiciones_t* NivelServidor::create_posicion(int id,  const char* descrip, int x, int y){
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->posX = x;
    pos->posY = y;
    strcpy(pos->descrip, descrip);
    pos->id = id;

    return pos;
}

posicionesR_t* NivelServidor::create_posicionR(int id,  char const* descrip, int x, int y, int puntaje, int vida, int energia){
    posicionesR_t* pos = (posicionesR_t*)malloc(sizeof(posicionesR_t));
    pos->posX = x;
    pos->posY = y;
    strcpy(pos->descrip, descrip);
    pos->id = id;
    pos->score = puntaje;
    pos->vidas = vida;
    pos->energiaActual = energia;

    return pos;
}

// FUNCIONES PARA EL CORTE DE NIVEL
bool NivelServidor::jugadoresMuertos(){
    int cant_jug = jugadores.size();
    for(int i=0; i<cant_jug; i++){
        if(jugadores[i]->isAlive()) return false;
    }
    return true;
}

bool NivelServidor::enemigosSiganVivos(){
    return death_enemies != cant_enemigos;
}

// FUNCIONES PARA EL RECIBIMIENTO DE DATA
void NivelServidor::recibeNone(Server* server, velocidades_t* v){
    if(strcmp(v->descrip, "none") == 0){
        int x = jugadores[v->id]->getPosX();
        int y = jugadores[v->id]->getPosY();
        posiciones_t* pos = create_posicion(v->id, "none", x, y);
        server->send_all(pos);
        free(pos);
    }
}

bool NivelServidor::recibeModoTest(Server* server, velocidades_t* v){
    bool ok = false;
    if(strcmp(v->descrip, "test") == 0){
        posiciones_t* pos = create_posicion(v->id, "test", 0, 0);
        jugadores[v->id]->set_modeTest();
        server->send_all(pos);
        free(pos);
        ok = true;
    }
    return ok;
}

bool NivelServidor::recibePrimerDisparo(Server* server, velocidades_t* v){

    bool ok = false;
    if(strcmp(v->descrip, "shot0") == 0){
        int id = v->id;
        int vx = v->VelX;
        int vy = v->VelY;
        int danio = 0;
        if(id > 3){
            danio = enemigos[id-4]->get_damage();
        } else{
            danio = jugadores[id]->get_damage();
        }
        Misil* misil = new Misil(vx, vy, id, danio);
        misiles.push_back(misil);
        posiciones_t* pos = create_posicion(v->id, "shot0", vx, vy);
        server->send_all(pos);
        free(pos);
        ok = true;
    }
    return ok;
}

bool NivelServidor::recibeNaveEnemiga(Server* server, velocidades_t* v){
    bool ok = false;
    if(v->id > 3){
        // mueve enemigos
        moverEnemigos(server, v);

        //ACTUALIZA EL BG
        parallax();
        posiciones_t* pos = create_posicion(v->id, "bg", 0, 0);
        server->send_all(pos);
        free(pos);

        //mueve misiles
        moverMisiles(server, v);
        ok = true;
    }
    return ok;
}

bool NivelServidor::recibeNaveJugador(Server* server, velocidades_t* v){
    bool ok = false;
    if(strcmp(v->descrip, "off") != 0){
        int id = v->id;
        if(jugadores[id]->isAlive()){
            if(v->VelX == 0 && v->VelY == 0) return true;
            jugadores[id]->setVelX(v->VelX);
            jugadores[id]->setVelY(v->VelY);
            int colision_id = jugadores[id]->mover(enemigos);

            if(colision_id == -2){
                posiciones_t* pos = create_posicion(id, "colision", 0, colision_id);
                server->send_all(pos);
                free(pos);
            } else{

                if(colision_id != -1){
                    int score = enemigos[colision_id]->getScore();
                    jugadores[id]->addScore(score);
                    death_enemies++;
                    posiciones_t* pos = create_posicion(id, "colision", 0, colision_id);
                    server->send_all(pos);
                    free(pos);

                    score_nivel[id] += score;
                } else {
                    int posX = jugadores[id]->getPosX();
                    int posY = jugadores[id]->getPosY();
                    posiciones_t* pos = create_posicion(id, v->descrip, posX, posY);
                    server->send_all(pos);
                    free(pos);
                }
            }
        }
        ok = true;
    }
    return ok;
}

void NivelServidor::recibeJugadorDesconectado(Server* server, velocidades_t* v){
    if(strcmp(v->descrip, "off") == 0){
        posiciones_t* pos = create_posicion(v->id, "off", 0, 0);
        server->send_all(pos);
        free(pos);
    }
}


// FUNCIONES AUXILIARES PARA LAS MISMAS
void NivelServidor::moverEnemigos(Server* server, velocidades_t* v){
    for(int i = 0; i < v->id - 4; i++){
        if(enemigos[i]->esBoss() && cant_enemigos - death_enemies > 1) continue;
        if(enemigos[i]->isAlive()){
            int colision_id = enemigos[i]->procesarAccion(jugadores);
            bool disparo = enemigos[i]->seDisparo();
            if (disparo){
                /*int xMisil = enemigos[i]->getPosX()-enemigos[i]->getRadio();
                int yMisil = enemigos[i]->getPosY();*/
                int xMisil = enemigos[i]->get_posX_misil();
                int yMisil = enemigos[i]->get_posY_misil();
                velocidades_t* vMisil = create_velocidad(i+4, "shot0", xMisil, yMisil);
                server->encolar(vMisil);
                bool disparoTriple = enemigos[i]->getDisparoTriple();
                if (disparoTriple){
                    velocidades_t* vMisil2 = create_velocidad(i+4, "shot0", xMisil, yMisil + 100);
                    server->encolar(vMisil2);
                    velocidades_t* vMisil3 = create_velocidad(i+4, "shot0", xMisil, yMisil - 100);
                    server->encolar(vMisil3);
                }
                //enemigos[i]->reiniciarDisparo();
            }
            if(colision_id != -1){
                printf("colision enemigo\n");
                int enemie = -2;
                if(strcmp(enemigos[i]->getClave(), "boss") != 0){
                    int score = enemigos[i]->getScore();
                    death_enemies++;
                    jugadores[colision_id]->addScore(score);
                    score_nivel[colision_id] += score;
                    enemie = i;
                }
                posiciones_t* pos = create_posicion(colision_id, "colision", 0, enemie);
                server->send_all(pos);
                free(pos);
            } else{
                int posX = enemigos[i]->getPosX();
                int posY = enemigos[i]->getPosY();
                posiciones_t* pos;
                if(enemigos[i]->isTurret()) pos = create_posicion(i+4, "turret", posX, posY);
                else pos = create_posicion(i+4, "ASD", posX, posY);
                server->send_all(pos);
                free(pos);
            }
        }
    }

}

void NivelServidor::moverMisiles(Server* server, velocidades_t* v){
    list<Misil*>::iterator pos_m = misiles.begin();
    while(pos_m != misiles.end()){
        int ok = -1;
        if((*pos_m)->get_id() <= 3){
            ok = (*pos_m)->mover(enemigos, renderizados);
        } else{
            ok = (*pos_m)->mover(jugadores);
        }

        if(ok == -1){
            Misil* misil = (*pos_m);
            pos_m = misiles.erase(pos_m);
            delete(misil);
            //printf("borra misil\n");
        } else if(ok == -2){
            //printf("no impacto\n");
            int posX = (*pos_m)->getPosX();
            int posY = (*pos_m)->getPosY();
            posiciones_t* pos = create_posicion((*pos_m)->get_id(), "shot1", posX, posY);
            server->send_all(pos);
            free(pos);
            pos_m++;
        } else{
            //printf("impacto\n");
            int posX=0;
            if(ok > 3) posX = enemigos[ok-4]->getVidaActual();
            else posX = jugadores[ok]->getVidaActual();

            posiciones_t* pos = create_posicion(ok, "hit", posX, (*pos_m)->get_id());
            server->send_all(pos);
            if(pos->posX <= 0 && ok > 3) {
                int score = enemigos[ok-4]->getScore();
                jugadores[pos->posY]->addScore(score);
                death_enemies++;

                score_nivel[pos->posY] += score;
            }
            pos_m = misiles.erase(pos_m);
            free(pos);
        }
    }
}

void NivelServidor::cerrar(){
    vector<NaveJugador*>::iterator posJ;
    for(posJ = jugadores.begin(); posJ != jugadores.end(); posJ++) free((*posJ));
    vector<NaveEnemiga*>::iterator posE;
    for(posE = enemigos.begin(); posE != enemigos.end(); posE++) free((*posE));
}

vector<int> NivelServidor::getVidas(){
    return vidas;
}

vector<int> NivelServidor::getScores(){
    return scores;
}

vector<bool> NivelServidor::get_modeTests(){
    return mode_tests;
}

void NivelServidor::enviar_scores(Server* server){
    int cant_jugadores = jugadores.size();
    for(int i = 0; i<cant_jugadores; i++){
        posiciones_t* pos = create_posicion(i, "score", score_nivel[i], 0);
        server->send_all(pos);
        free(pos);
    }
    posiciones_t* pos = create_posicion(-1, "a", 0, 0);
    server->send_all(pos);
    free(pos);
}
