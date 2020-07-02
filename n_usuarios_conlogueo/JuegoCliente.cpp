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
    renderizados = 1;
    menu = Menu();
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

void JuegoCliente::iniciarNivel(Client* client){
    printf("CLIENT inicia el nivel, ID: %d\n", client->get_id());
    bool quit = false;

    NaveJugador* jugador1 = jugadores[client->get_id()];
    printf("CLIENT obtiene su nave, ID: %d\n", jugador1->get_id());
    while( usuarioNoRequieraSalir(quit) ) {

        //printf("itera\n");
        while( hayEventos() ) {
            //printf("hay evento\n");
            if( eventoEsSalir() ) quit = true;
            jugador1->handleEvent( e );
        }

        //printf("obtuvo velocidad\n");
        velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
        v->id = client->get_id();
        v->VelX = jugador1->getVelX();
        v->VelY = jugador1->getVelY();

        printf("CLIENT ID: %d vel: %d - %d\n", v->id, v -> VelX, v->VelY);
        //printf("enviando_data\n")

        client->sendData(v);
        //printf("data enviada\n");

        while(!client->cola_esta_vacia()){
            void* dato = client->desencolar();
            procesar((posiciones_t*) dato);
        }

        //printf("CLIENT: pos: %d - %d\n", jugador1 -> getPosX(), jugador1->getPosY());

        free(v);


        SDL_RenderClear( sdl.getRenderer() );
        renderBackground();

        //render jugador
        vector<NaveJugador*>::iterator pos;
        for(pos = jugadores.begin(); pos != jugadores.end(); pos++)
        {
            (*pos)->renderizar();
        }

        for(int i = 0; i < renderizados; i++){
            //for(int j = 0; j<iteraciones;j++)enemigos[i]->mover(jugador1);
            enemigos[i]->renderizar();
        }

        //Todo este bloque deberiamos declararlo en otro lado

        SDL_RenderPresent( sdl.getRenderer() );
        //free(v);
        //for(int i = time(NULL) + 15; time(NULL) != i; time(NULL));

        }
    //free(pos);
    //free(v);
    //vector<NaveEnemiga*>::iterator pos;
    // CIERRA LAS NAVES
    //jugador1->cerrarNave();
    //jugador2->cerrarNave();
    client->finalizar();
    //for(pos = enemigos.begin(); pos != enemigos.end(); pos++){
    //    (*pos)->cerrarNave();
    //}
    //return quit;
}

void JuegoCliente::procesar(posiciones_t* pos){
    printf("CLIENT procesa data, ID: %d\n", pos->id);
    if(pos->id>3){
        aumentar_renderizados(pos->id-4);
        for(int i = 0; i < renderizados ; i++){
            enemigos[i]->mover(jugadores[0]);
        }
        return;
    }
    jugadores[pos->id]->setPosX(pos->posX);
    jugadores[pos->id]->setPosY(pos->posY);
    free(pos);
    //printf("CLIENT seteo x e y\n");
}

void JuegoCliente::cargarNivel(Client* client){

    logger.info(">>>> CARGANDO EL NIVEL 1 ....");

    std::cout<<"CARGAR NIVEL\n";

    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    while(true){
        pos = (posiciones_t*)client->receiveData();
        if(pos == NULL){
            std::cout<<"recibio nulo\n";
            continue;
        }
        printf("recibe nave, ID: %d\n", pos->id);
        if(pos->id == -1) break;
        if(pos->id>3){
            NaveEnemiga* enemigo = new NaveEnemiga(pos->posX, pos->posY, pos->descrip);
            enemigos.push_back(enemigo);
        } else{
            printf("creo nave jugador\n");
            NaveJugador* nave = new NaveJugador(0, 0, pos->id);
            jugadores.push_back(nave);
        }
    }
    /*cantidad_enemigos = json.get_cantidad_enemigo("nivel1");
    if(cantidad_enemigos == 0){
        logger.error("Cantidad de enemigos del nivel1 inexistente, se cargo una por defecto");
        cantidad_enemigos = json.get_cantidad_enemigo_default("nivel1");
    }
    std::string mensaje = "Se cargo la cantidad de enemigos: " + std::to_string(cantidad_enemigos);
    logger.debug(mensaje.c_str());*/

    cargarImagen("nivel1", "mapaBG", &gBGTexture);
    cargarImagen("nivel1", "ciudad", &gCiudadTexture);
    cargarImagen("nivel1", "planeta", &gPlanetaTexture);
    cargarImagen("nivel1", "nube1", &gNube1);
    cargarImagen("nivel1", "nube2", &gNube2);

    cargarImagen("nivel1", "finNivel", &gFinNivel);

    scrollingOffsetBG = 0;
    scrollingOffsetCity = 0;
    tierraInicial = 700;
    scrollingOffsetNube1 = 120;
    scrollingOffsetNube2 = 0;

    dataBG.h = 600;
    dataBG.w = 2048;
    dataBG.x = 0;
    dataBG.y = 0;

    dataCiudad.h = 450;
    dataCiudad.w = 2048;
    dataCiudad.x = 0;
    dataCiudad.y = 0;

    dataNube1.h = 600;
    dataNube1.w = 800;
    dataNube1.x = 0;
    dataNube1.y = 0;

    dataNube2.h = 600;
    dataNube2.w = 800;
    dataNube2.x = 0;
    dataNube2.y = 0;

    logger.info("<<<< SE CARGO EL NIVEL 1");
}

void JuegoCliente::cerrar(){
	gBGTexture.free();
	gCiudadTexture.free();
	gPlanetaTexture.free();
	gNube1.free();
	gNube2.free();

	gFinNivel.free();
}

void JuegoCliente::renderBackground(){

    parallax();

	gBGTexture.render( scrollingOffsetBG, 0, &dataBG );
	gBGTexture.render( scrollingOffsetBG + dataBG.w, 0, &dataBG );

	gPlanetaTexture.render(tierraInicial, -50);
    tierraInicial -= 0.2;

	gCiudadTexture.render(scrollingOffsetCity, 150, &dataCiudad);
	gCiudadTexture.render(scrollingOffsetCity + dataCiudad.w, 150, &dataCiudad);

	gNube1.render( scrollingOffsetNube1, 0, &dataNube1 );
	gNube1.render( scrollingOffsetNube1 + dataNube1.w, 0, &dataNube1 );
	gNube2.render( scrollingOffsetNube2, 0, &dataNube2 );
	gNube2.render( scrollingOffsetNube2 + dataNube2.w, 0, &dataNube2 );
}


void JuegoCliente::parallax(){

    scrollingOffsetBG -= 0.5;
    if( scrollingOffsetBG < -gBGTexture.getWidth() ) scrollingOffsetBG = 0;

	scrollingOffsetCity -= 5;
    if( scrollingOffsetCity < -dataCiudad.w ) scrollingOffsetCity = 0;

    scrollingOffsetNube1 -= 8;
    if( scrollingOffsetNube1 < -dataNube1.w ) scrollingOffsetNube1 = 0;

    scrollingOffsetNube2 -= 10;
    if( scrollingOffsetNube2 < -dataNube2.w ) scrollingOffsetNube2 = 0;

}

void JuegoCliente::aumentar_renderizados(int i){
    pthread_mutex_lock(&mutex);
    if(i>renderizados)renderizados++;
    pthread_mutex_unlock(&mutex);
}

std::string JuegoCliente::get_id(){
    return menu.get_id();
}

std::string JuegoCliente::get_password(){
    return menu.get_pass();
}

void JuegoCliente::render_errorLoguin(int intentos){

    std::string intentos_rest = std::to_string(intentos);
    menu.renderErrorLoguin(intentos_rest.c_str());
}

void JuegoCliente::cerrarMenu(){
    menu.cerrar();
}
