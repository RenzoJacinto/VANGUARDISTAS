#include "Nivel.h"
#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Client.h"
#include "global.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

Nivel::Nivel(){
    dataFinNivel.h = 600;
    dataFinNivel.w = 800;
    dataFinNivel.x = 0;
    dataFinNivel.y = 0;
    renderizados = 1;
    renderGO = 0;
}

bool Nivel::iniciarNivel(Client* client){
    bool quit = false;
    //printf("intenta gettear nave %d\n", client->get_id());

    NaveJugador* jugador1 = jugadores[client->get_id()];

    shotFX = sounds.loadEffect(json.get_sound("FX","shot").c_str());
    explosion = sounds.loadEffect(json.get_sound("FX","boom").c_str());
    hitReceiveFX = sounds.loadEffect(json.get_sound("FX","receiveHit").c_str());
    lifeDownFX = sounds.loadEffect(json.get_sound("FX", "lifeDown").c_str());
    gameOverFX = sounds.loadEffect(json.get_sound("FX", "gameOver").c_str());
    levelUpFX = sounds.loadEffect(json.get_sound("FX", "levelUp").c_str());

    if(! deathUserTexture.loadFromFile(json.get_sprite_menu("deathUser"))){
        logger.error("No se pudo cargar el sprite de game over");
        deathUserTexture.loadFromFile(json.get_imagen_default("escenario"));
    } else logger.debug("Se cargo el sprite de game over");

    sounds.playMusic(gMusic);

    int id_nave = jugador1->get_id();
    if(jugador1->get_vidas() == 0) renderGO = 0;
    if(! lifeTexture.loadFromFile(json.get_sprite_puntajes("life")))
        logger.error("No se pudo cargar la textura de la vida");

    if(! puntajesBoxTexture.loadFromFile(json.get_sprite_puntajes("box")))
        logger.error("No se pudo cargar la textura de los puntajes");

    SDL_PumpEvents();
    SDL_FlushEvent(SDL_KEYDOWN);
    SDL_FlushEvent(SDL_KEYUP);

    while( usuarioNoRequieraSalir(quit) ){

        SDL_RenderClear( sdl.getRenderer() );

        //renders
        renderBackground();
        renderPuntajes();
        renderizar(id_nave);

        while( hayEventos() ) {
            if( eventoEsSalir() ) quit = true;
            int new_misil = NONE;
            jugador1->handleEvent( e, gMusic, &new_misil);
            enviarTecleo(new_misil, jugador1, client, id_nave);
        }


        velocidades_t* v = create_velocidad(id_nave, "on", jugador1->getVelX(), jugador1->getVelY());
        //printf("CLIENT ID: %d vel: %d - %d\n", v->id, v -> VelX, v->VelY);

        //if(!jugador1->isAlive() || !jugador1->renderizo()) {
        //    strcpy(v->descrip, "none");
        //}

        if(! client->sendData(v)){
            client->renderServerCaido();
            logger.error("El servidor esta caido, terminando la ejecucion...");
            free(v);
            return true;
        }
        free(v);

        while(!client->cola_esta_vacia()){
            void* dato = client->desencolar();
            posiciones_t* pos = (posiciones_t*) dato;
            if(pos->id == -1){
                if(strcmp(pos->descrip, "gameOver") == 0){
                    renderGameOver();
                    client->finalizar();
                    return true;
                } else{
                    recibeScoresDelNivel(client);
                    return false;
                }

            }
            procesar((posiciones_t*) dato);
            free(dato);
        }

        //renderizar();
    }
    //lifeTexture.free();
    //puntajesBoxTexture.free();
    //delete(jugador1);
    client->finalizar();
    return quit;
}
void Nivel::renderizar(int id_nave){

        list<Misil*>::iterator pos_m = misiles.begin();
        while(pos_m != misiles.end()){
            //(*pos_m)->mover();
            (*pos_m)->renderizar();
            Misil* misil = (*pos_m);
            pos_m = misiles.erase(pos_m);
            delete(misil);
        }
        //std::cout<<"SIZE: "<<misiles.size()<<"\n";

        //render jugador

        vector<NaveJugador*>::iterator pos;
        for(pos = jugadores.begin(); pos != jugadores.end(); pos++){
            if((*pos)->isAlive()) (*pos)->renderizar();
            if((*pos)->boomAvailable()){
                //
                if((*pos)->getFrame() == 0){
                    sounds.playEffect(explosion);
                    if((*pos)->get_id() == id_nave) sounds.playEffect(lifeDownFX);
                }
                (*pos)->renderBoom();
                //if((*pos)->getFrame() == 0)sounds.playEffect(explosion);
            }
        }

        for(int i = 0; i < renderizados; i++){
            if(enemigos[i]->isAlive()) enemigos[i]->renderizar();
        }

        for(int i = 0; i < renderizados; i++){
            if(enemigos[i]->boomAvailable()){
                if(enemigos[i]->getFrame() == 0) sounds.playEffect(explosion);
                enemigos[i]->renderBoom();
            }
        }
        //Todo este bloque deberiamos declararlo en otro lado

        if(jugadores[id_nave]->get_vidas() == 0){
            if(renderGO == 0) {
                sounds.playEffect(gameOverFX);
                renderGO += 1;
            }
            deathUserTexture.render(0,0);
        }

        SDL_RenderPresent( sdl.getRenderer() );
}

void Nivel::procesar(posiciones_t* pos){

    if(recibeColision(pos)) return;
    if(recibeModoTest(pos)) return;
    if(! recibeDisparo(pos)){
        if(! recibeHit(pos)){
            if(! recibeParallax(pos)) moverNaves(pos);
        }
    }
}

void Nivel::finalizar() {

    sounds.stopMusic();
    sounds.playEffect(levelUpFX);

    logger.info("Finalizó el nivel");
    gFinNivel.render(0,0, &dataFinNivel);


    /*Renderizado de puntajes */
    int cant_jug = jugadores.size();

    int score_nivel_ant[cant_jug];
    std::string names[cant_jug];
    for(int i=0; i<cant_jug; i++){
        names[i] = jugadores[i]->get_name();
        score_nivel_ant[i] = jugadores[i]->getScore() - score_nivel[i];
    }

    //Ordeno ambos arrays, para rankear los users
    for(int j=0; j<cant_jug; j++){
        for(int k=j+1; k<cant_jug; k++){
            if(score_nivel[j] < score_nivel[k]){
                //swap score
                int aux_sc = score_nivel[k];
                score_nivel[k] = score_nivel[j];
                score_nivel[j] = aux_sc;

                //swap score nivel ant
                int aux_sc_ant = score_nivel_ant[k];
                score_nivel_ant[k] = score_nivel_ant[j];
                score_nivel_ant[j] = aux_sc_ant;

                //swap name
                std::string aux_n = names[k];
                names[k] = names[j];
                names[j] = aux_n;
            }

        }

    }

    TextureW textureKing;
    if(! textureKing.loadFromFile(json.get_sprite_puntajes("corona"))){
        logger.error("No se pudo cargar el sprite de la corona");
        textureKing.loadFromFile(json.get_imagen_default("tiro"));
    } else logger.debug("Se cargo correctamente el sprite de la corona");
    textureKing.render(140, 315);
    textureKing.free();

    TextureW texture;
    for(int i=0; i<cant_jug; i++){
        texture.loadFromRenderedText(names[i], "fin");
        texture.render(186, i*48 + 300);

        texture.free();

        texture.loadFromRenderedText(std::to_string(score_nivel_ant[i]), "fin");
        texture.render(387, i*48 +300);

        int x_sc = texture.getWidth();
        texture.free();

        texture.loadFromRenderedText("+" + std::to_string(score_nivel[i]), "log");
        texture.render(387 + x_sc + 10, i*48 +300);

        texture.free();
    }
    SDL_RenderPresent( sdl.getRenderer() );
    for(int i = time(NULL) + 5; time(NULL) != i; time(NULL));

    freeSounds();

    logger.info("Se renderizo el final del nivel");
}

void Nivel::renderGameOver() {

    logger.info("Finalizó el nivel con GAME OVER");
    if(! gFinNivel.loadFromFile(json.get_sprite_menu("gameOver"))){
        logger.error("No se encontro el sprite de game over");
        gFinNivel.loadFromFile(json.get_imagen_default("escenario"));
    } else logger.debug("Se cargo el sprite de game over");
    sounds.stopMusic();
    sounds.playEffect(gameOverFX);
    gFinNivel.render(0,0, &dataFinNivel);
    SDL_RenderPresent( sdl.getRenderer() );
    for(int i = time(NULL) + 5; time(NULL) != i; time(NULL));

    freeSounds();

    logger.info("Se renderizo el final del nivel con GAME OVER");
}

void Nivel::aumentarRenderizados(int i){
    pthread_mutex_lock(&mutex);
    if(i>renderizados)renderizados++;
    pthread_mutex_unlock(&mutex);
}

bool Nivel::setNaves(Client* client){
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    while(true){
        pos = (posiciones_t*)client->receiveData();
        if(!pos) return false;
        printf("recibe nave, ID: %d\n", pos->id);
        if(pos->id == -1) break;
        if(pos->id>3){
            NaveEnemiga* enemigo = new NaveEnemiga(pos->posX, pos->posY, pos->descrip);
            enemigos.push_back(enemigo);
        } else{
            printf("creo nave jugador\n");
            NaveJugador* nave = new NaveJugador(pos->posX, pos->posY, pos->id, client->get_id_user(pos->id));
            if(strcmp(pos->descrip, "off") == 0) {
                printf("nave off\n");
                nave->desconectar();
            }
            jugadores.push_back(nave);
        }
    }
    return true;
}

void Nivel::renderEnemigos(){
    for(int i = 0; i < renderizados; i++){
        if(enemigos[i]->isAlive()) enemigos[i]->renderizar();
    }
}
void Nivel::renderBooms(){
    for(int i = 0; i < renderizados; i++){
        if(enemigos[i]->boomAvailable()) enemigos[i]->renderBoom();
    }
}

void Nivel::renderPuntajes(){

    puntajesBoxTexture.render(0,0);

    TextureW nameTexture;
    TextureW scoreTexture;
    int size_box_name = 132;

    int size_life = lifeTexture.getWidth();

    int y_name = 16;
    int x_name = 0;

    int y_life = 38;
    int x_life = 0;

    int y_score = 35;
    int x_score = 0;

    vector<NaveJugador*>::iterator pos;
    for(pos = jugadores.begin(); pos != jugadores.end(); pos++){
        int id = (*pos)->get_id();
        if(id == 0 || id == 3){
            x_name = id * 200 + 36;
            x_life = id * 200 + 25;
            x_score = id * 200 + 75;
        } else{
            x_name = id*200 + 38;
            x_life = id*200 + 27;
            x_score = id*200 + 81;
        }

        std::string name = (*pos)->get_name();
        int score = (*pos)->getScore();
        std::string puntos = std::to_string(score);
        if(! nameTexture.loadFromRenderedText(name, "game"))
            logger.error("No se pudo cargar el id del jugador");
        if(! scoreTexture.loadFromRenderedText(puntos, "game"))
            logger.error("No se pudo cargar el score del jugador");
        else{
            int center = size_box_name - nameTexture.getWidth();
            center = center/2;
            nameTexture.render(x_name + center, y_name);
            scoreTexture.render(x_score + center, y_score);
        }
        nameTexture.free();
        scoreTexture.free();

        int cant_vidas = (*pos)->get_cant_vidas();
        for(int i=0; i<cant_vidas; i++){
            lifeTexture.render(x_life + i*size_life, y_life);
        }
    }

    //nameTexture.free();
    //scoreTexture.free();
    //lifeTexture.free();
}

velocidades_t* Nivel::create_velocidad(int id,  const char* descrip, int x, int y){
    velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
    v->VelX = x;
    v->VelY = y;
    strcpy(v->descrip, descrip);
    v->id = id;

    return v;
}

void Nivel::freeSounds(){
    //sounds.stopMusic();
    sounds.freeMusic(gMusic);

    sounds.freeEffect(shotFX);
    sounds.freeEffect(explosion);
    sounds.freeEffect(hitReceiveFX);
    sounds.freeEffect(lifeDownFX);
    sounds.freeEffect(gameOverFX);
    sounds.freeEffect(levelUpFX);
}

void Nivel::borrarNaves(){
    vector<NaveJugador*>::iterator posJ;
    for(posJ = jugadores.begin(); posJ != jugadores.end(); posJ++) delete((*posJ));
    vector<NaveEnemiga*>::iterator posE;
    for(posE = enemigos.begin(); posE != enemigos.end(); posE++) delete((*posE));
}

// FUNCIONES DE RECIBIMIENTO DE DATA (PROCESAR)
bool Nivel::recibeColision(posiciones_t* pos){
    bool ok = false;
    if(strcmp(pos->descrip, "colision") == 0){
        std::cout<<"en: "<<pos->posY<<"\n";
        std::cout<<"jug: "<<pos->id<<"\n";

        jugadores[pos->id]->die();
        if(pos->posY != -2){
            int score = enemigos[pos->posY]->getScore();
            jugadores[pos->id]->addScore(score);
            enemigos[pos->posY]->die();
        }

        // acumula el score;
        //score_nivel[pos->id] += score;
        ok = true;
    }
    return ok;
}

bool Nivel::recibeModoTest(posiciones_t* pos){
    bool ok = false;
    if(strcmp(pos->descrip, "test") == 0){
        jugadores[pos->id]->set_modeTest();
        ok = true;
    }
    return ok;
}

bool Nivel::recibeDisparo(posiciones_t* pos){
    bool ok = false;
    if(strcmp(pos->descrip, "shot0") == 0 || strcmp(pos->descrip, "shot1") == 0){
        if(strcmp(pos->descrip, "shot0") == 0) sounds.playEffect(shotFX);
        Misil* misil = new Misil(pos->posX, pos->posY, pos->id);
        misiles.push_back(misil);
        ok = true;
    }
    return ok;
}

bool Nivel::recibeHit(posiciones_t* pos){
    bool ok = false;
    if(strcmp(pos->descrip, "hit") == 0){
        if(pos->id > 3){
            enemigos[pos->id - 4]->setEnergias(pos->posX, pos->posY);
            int score = enemigos[pos->id -4]->getScore();
            if(! enemigos[pos->id - 4]->isAlive()){
                jugadores[pos->posY]->addScore(score);
                // acumula el score;
                //score_nivel[pos->id] += score;
            } else sounds.playEffect(hitReceiveFX);
        } else{
            jugadores[pos->id]->setEnergias(pos->posX, pos->posY);
            if(jugadores[pos->id]->isAlive())sounds.playEffect(hitReceiveFX);
        }
        ok = true;
    }
    return ok;
}

bool Nivel::recibeParallax(posiciones_t* pos){
    bool ok = false;
    if(strcmp(pos->descrip, "bg") == 0){
        parallax();
        ok = true;
    }
    return ok;
}

void Nivel::moverNaves(posiciones_t* pos){
    // es enemigo
    if(pos->id>3){
        aumentarRenderizados(pos->id-3);
        enemigos[pos->id - 4]->setPosX(pos->posX);
        enemigos[pos->id - 4]->setPosY(pos->posY);
        if(strcmp(pos->descrip, "turret") == 0) enemigos[pos->id - 4]->setTurret();
        else enemigos[pos->id - 4]->quitTurret();
        return;
    }
    if(strcmp(pos->descrip, "off") != 0){
        //jugadores[pos->id]->endBoom();
        jugadores[pos->id]->setPosX(pos->posX);
        jugadores[pos->id]->setPosY(pos->posY);
        if(!jugadores[pos->id]->isOn()) jugadores[pos->id]->conectar();
    } else{
        printf("desconectado\n");
        jugadores[pos->id]->desconectar();
    }
}

void Nivel::recibeScoresDelNivel(Client* client){
    int cant_jug = jugadores.size();
    for(int i=0; i<cant_jug; i++){
        score_nivel.push_back(0);
    }
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    while(true){
        pos = (posiciones_t*)client->receiveData();
        //printf("recibe nave, ID: %d\n", pos->id);
        if(pos->id == -1) break;
        score_nivel[pos->id] = pos->posX;
    }
    free(pos);
}

// FUNCIONES AUX PARA ENVIO ANTE TECLEO
void Nivel::enviarTecleo(int new_misil, NaveJugador* jugador, Client* client, int id_nave){
    if(new_misil == MISIL){
        velocidades_t* v_shot = create_velocidad(id_nave, "shot0", jugador->getPosX(), jugador->getPosY());
        //if(!jugador->isAlive() || !jugador->renderizo()) {
        //    strcpy(v_shot->descrip, "none");
        //}
        client->sendData(v_shot);
        free(v_shot);
    } else if(new_misil == MODO_TEST){
        velocidades_t* v_shot = create_velocidad(id_nave, "test", 0, 0);
        //if(!jugador->isAlive() || !jugador->renderizo()) {
        //    strcpy(v_shot->descrip, "none");
        //}
        client->sendData(v_shot);
        free(v_shot);
    }
}

void Nivel::parallax(){}

void Nivel::cerrar(){}

bool Nivel::cargarNivel(Client* client){}

void Nivel::renderBackground(){}

void Nivel::reconectar(Client* client){}
