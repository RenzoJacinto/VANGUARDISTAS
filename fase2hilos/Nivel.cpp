#include "Nivel.h"
#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Server.h"
#include "Client.h"
#include "Estado.h"
#include "Temporizador.h"
#include "global.h"
#include "Server.h"
#include "Estado.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

Nivel::Nivel(){
    dataFinNivel.h = 600;
    dataFinNivel.w = 800;
    dataFinNivel.x = 0;
    dataFinNivel.y = 0;
}

//manejoSDL
//Menu
//TextureW
//BotonIniciar


typedef struct velocidades {
    int id;
    int VelX;
    int VelY;
} velocidades_t;

typedef struct posiciones {
    int id;
    int posX;
    int posY;
} posiciones_t;

void* loop_recibir_cliente(void* p) {
    Client* client = (Client*)p;
    client->recibir_encolar();
    return NULL;
}

void* loop_recibir_server_0(void* p) {
    Server* sv = (Server*)p;
    sv->recibir_encolar(0);
    return NULL;
}

void* loop_recibir_server_1(void* p) {
    Server* sv = (Server*)p;
    sv->recibir_encolar(1);
    return NULL;
}

int get_nave(vector<NaveJugador*> jugadores, int id) {
    int k = 0;
    vector<NaveJugador*>::iterator pos;
    for(pos = jugadores.begin(); pos != jugadores.end(); pos++){
//        printf("id buscada: %d, id de iteracion: %d\n", id, (*pos)->get_id());
        if(id == (*pos)->get_id()) break;
        k++;
    }
    return k;
}

vector<NaveEnemiga*> Nivel::crear_enemigos(){
    vector<NaveEnemiga*> enemigos(cantidad_enemigos);
    for(int i=0; i<cantidad_enemigos; i++){

        // Elige un enemigo random
        std::string sprite = "enemigo";
        int enemigo_random = 1 + rand() % 4;
        sprite += std::to_string(enemigo_random);

        // Obtencion de la posicion pos = inf + rand()%(sup+1-inf)
        int y = 50 + rand() % (sdl.getScreenHeight() + 1);
        // SUPONGO EL BORDE DE RESPAWN COMO +/-100
        // CASO ENEMIGOS 1 y 2: sup = 800 + 100
        int x = sdl.getScreenWidth() + rand() % 51;

        // CASO ENEMIGOS 3 y 4: inf = -100
        if(enemigo_random == 4 || enemigo_random == 3) x = -50 + rand() % 51;

        NaveEnemiga* enemigo = new NaveEnemiga(x, y, sprite.c_str());

        enemigos[i]=enemigo;
    }
    std::string mensaje = "Se crearon los " + std::to_string(cantidad_enemigos) + " enemigos";
    logger.debug(mensaje.c_str());
    return enemigos;
}

bool Nivel::procesar_servidor(){

	    bool quit = false;

	    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
	    Temporizador temporizador;
	    temporizador.iniciar();

        Server* server = (Server*) estado;

        NaveJugador* jugador = new NaveJugador( sdl.getScreenWidth() / 4, sdl.getScreenWidth() / 4, 0);
        NaveJugador* jugador1 = new NaveJugador( sdl.getScreenWidth() / 2, sdl.getScreenWidth() / 4, 1);

        vector<NaveEnemiga*> enemigos = crear_enemigos();
        vector<NaveJugador*> jugadores(2);
        jugadores[0] = jugador;
        jugadores[1] = jugador1;

        float tiempo_por_enemigos = TIEMPO_NIVEL_SEGS/cantidad_enemigos;
        double tiempo_nivel = 0;
        int renderizados = 1;

        //velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
        //v->id = 1;
        //v->VelX = 0;
        //v->VelY = 0;

        //posiciones_t* pos = (posiciones_t*) malloc(sizeof(posiciones_t));
        //pos->posX = 200;
        //pos->posY = 200;

        //velocidades_t* v1 = (velocidades_t*) malloc(sizeof(velocidades_t));
        //v1->VelX = 0;
        //v1->VelY = 0;
        pthread_t hilo1;
        pthread_t hilo2;
        printf("creando hilos\n");
        pthread_create(&hilo1, NULL, loop_recibir_server_0, estado);
        //i++;
        pthread_create(&hilo2, NULL, loop_recibir_server_1, estado);
        printf("hilos creados\n");
        //e.key.keysym.sym = SDLK_CLEAR;

	    // Mientras que siga corriendo la app
	    while( usuarioNoRequieraSalir(quit) && tiempo_nivel < TIEMPO_NIVEL_SEGS ) {
		    //while( hayEventos() ){
            //    if( eventoEsSalir() ) quit = true;
            //    jugador->handleEvent( e );
            //}


            //int total_bytes_writen = 0;
			//int bytes_writen = 0;
			//int sent_data_size = sizeof(velocidades_t);

			//printf("recibiendo_data\n");
			//while(sent_data_size > total_bytes_writen) {

                //bytes_writen = recv(server->get_socket(0), v+total_bytes_writen, sizeof(velocidades_t)-total_bytes_writen, MSG_NOSIGNAL);
                //total_bytes_writen+=bytes_writen;
                //bytes_writen = recv(server->get_socket(1), v1+total_bytes_writen, sizeof(velocidades_t)-total_bytes_writen, MSG_NOSIGNAL);
            //}
            //printf("data recibida\n");

            //printf("SERVER vel: %d - %d\n", v -> VelX, v->VelY);
            while(!server->cola_esta_vacia()) {
                printf("SERVER iteracion\n");
                velocidades_t* v = (velocidades_t*)server->desencolar_procesar_enviar();
                int j = get_nave(jugadores, v->id);
                printf("SERVER encontro j\n");
                jugadores[j]->setVelX(v->VelX);
                printf("SERVER seteo X %d\n", v->VelX);
                jugadores[j]->setVelY(v->VelY);
                printf("SERVER seteo Y %d\n", v->VelY);
                jugadores[j]->mover(enemigos);
                printf("SERVER movio al jugador\n");
                posiciones_t* pos = (posiciones_t*) malloc(sizeof(posiciones_t));
                pos->posX = jugador->getPosX();
                pos->posY = jugador->getPosY();
                pos->id = v->id;
                printf("SERVER X:%d - Y:%d, ID:%d\n", pos->posX, pos->posY, pos->id);
                int total_bytes_writen = 0;
                int bytes_writen = 0;
                int sent_data_size = sizeof(posiciones_t);
                while(sent_data_size > total_bytes_writen) {
                    bytes_writen = send(server->get_socket(0), pos+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
                    total_bytes_writen += bytes_writen;
                    if(bytes_writen<=0)break;
                }
                //send(server->get_socket(0), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
                //send(server->get_socket(1), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
                total_bytes_writen = 0;
                bytes_writen = 0;
                while(sent_data_size > total_bytes_writen) {
                    bytes_writen = send(server->get_socket(1), pos+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
                    total_bytes_writen += bytes_writen;
                    if(bytes_writen<=0) break;
                }
                free(v);
                free(pos);
            }
            //jugador->setVelX(v->VelX);
            //jugador->setVelY(v->VelY);
			//jugador->mover(enemigos);

            //jugador1->setVelX(v1->VelX);
            //jugador1->setVelY(v1->VelY);
			//jugador1->mover(enemigos);

			//pos -> posX = jugador->getPosX();
			//pos -> posY = jugador->getPosY();

			//pos1 -> posX = jugador1->getPosX();
			//pos1 -> posY = jugador1->getPosY();

			//printf("SERVER pos: %d - %d\n", pos -> posX, pos->posY);

			//total_bytes_writen = 0;
			//bytes_writen = 0;
			//sent_data_size = sizeof(posiciones_t);

			//printf("enviando_data\n");
			//while(sent_data_size > total_bytes_writen) {
                //bytes_writen = send(server->get_socket(0), pos+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
                //bytes_writen = send(server->get_socket(1), pos1+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
                //bytes_writen = send(server->get_socket(0), pos1+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
                //bytes_writen = send(server->get_socket(1), pos+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
                //total_bytes_writen += bytes_writen;
            //}
            //printf("data enviada\n");
            //SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
            //SDL_RenderClear( sdl.getRenderer() );
			//renderBackground();

            //render jugador
            //jugador->renderizar();

            //Todo este bloque deberiamos declararlo en otro lado
            for(int i = 0; i < renderizados && i < cantidad_enemigos; i++){
                enemigos[i]->mover(jugador);
                //enemigos[i]->renderizar();
            }

            tiempo_nivel = temporizador.transcurridoEnSegundos();
            if(tiempo_nivel/renderizados > tiempo_por_enemigos) renderizados++;

            //Hasta aca

			//SDL_RenderPresent( sdl.getRenderer() );
        }
        //free(pos);
        //free(pos1);
        //free(v);
        //free(v1);
        //client->finalizar();
        vector<NaveEnemiga*>::iterator posi;
        // CIERRA LAS NAVES
        jugador->cerrarNave();
        for(posi = enemigos.begin(); posi != enemigos.end(); posi++){
            (*posi)->cerrarNave();
        }
        return quit;
}

bool Nivel::procesar_cliente(){

	    bool quit = false;

	    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
	    Temporizador temporizador;
	    temporizador.iniciar();

        Client* client = (Client*) estado;

        int id = client->get_id();
        NaveJugador* jugador = new NaveJugador( sdl.getScreenWidth() / 4, sdl.getScreenWidth() / 4, id);
        id++;
        NaveJugador* jugador1 = new NaveJugador( sdl.getScreenWidth() / 2, sdl.getScreenWidth() / 4, id%2);

        vector<NaveJugador*> jugadores(2);
        jugadores[0] = jugador;
        jugadores[1] = jugador1;
        //vector<NaveEnemiga*> enemigos = crear_enemigos();

        float tiempo_por_enemigos = TIEMPO_NIVEL_SEGS/cantidad_enemigos;
        double tiempo_nivel = 0;
        int renderizados = 1;

        //e.key.keysym.sym = SDLK_CLEAR;
        pthread_t hilo;
        pthread_create(&hilo, NULL, loop_recibir_cliente, estado);

        //velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
        //v->id = client->get_id();
        //posiciones_t* pos = (posiciones_t*) malloc(sizeof(posiciones_t));
        //pos->posX = 200;
        //pos->posY = 200;

        //posiciones_t* pos1 = (posiciones_t*) malloc(sizeof(posiciones_t));
        //pos1->posX = 400;
        //pos1->posY = 400;

	    // Mientras que siga corriendo la app
	    while( usuarioNoRequieraSalir(quit) && tiempo_nivel < TIEMPO_NIVEL_SEGS ) {

		    while( hayEventos() ){
                if( eventoEsSalir() ) quit = true;
                jugador->handleEvent( e );
            }

            velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
            v->id = client->id;
            v->VelX = jugador->getVelX();
            v->VelY = jugador->getVelY();

            printf("CLIENT vel: %d - %d\n", v -> VelX, v->VelY);
            int total_bytes_writen = 0;
            int bytes_writen = 0;
            int sent_data_size = sizeof(velocidades_t);
			//printf("enviando_data\n")
            while(sent_data_size > total_bytes_writen) {
                bytes_writen = send(client->get_socket(), v + total_bytes_writen, sizeof(velocidades_t) - total_bytes_writen, MSG_NOSIGNAL);
                total_bytes_writen += bytes_writen;
                if(bytes_writen<=0) {
                    printf("error en send CLIENT\n");
                    break;
                }
            }
            printf("data enviada\n");


            printf("CLIENT: pos: %d - %d\n", jugador -> getPosX(), jugador->getPosY());

            while(!client->cola_esta_vacia()) {
                printf("recibio cosas\n");
                posiciones_t* pos = (posiciones_t*)client->desencolar_procesar();
                printf("desencolo\n");
                int j = get_nave(jugadores, pos->id);
                printf("encontro j %d\n", j);
                jugador->setPosX(pos -> posX);
                printf("seteo X %d\n", pos->posX);
                jugador->setPosY(pos -> posY);
                printf("seteo Y %d\n", pos->posX);
                free(pos);
            }

            free(v);

            //send(client->get_socket(), v, sizeof(velocidades_t), MSG_NOSIGNAL);

            //jugador->setVelX(v->VelX);
            //jugador->setVelY(v->VelY);
			//jugador->mover(enemigos);

			//posiciones_t* pos = (posiciones_t*) malloc(sizeof(posiciones_t));

			//total_bytes_writen = 0;
			//bytes_writen = 0;
			//sent_data_size = sizeof(posiciones_t);

			//printf("recibiendo_data\n");
			//while(sent_data_size > total_bytes_writen) {
                //bytes_writen = recv(client->get_socket(), pos + total_bytes_writen, sizeof(posiciones_t) - total_bytes_writen, MSG_NOSIGNAL);
                //bytes_writen = recv(client->get_socket(), pos1 + total_bytes_writen, sizeof(posiciones_t) - total_bytes_writen, MSG_NOSIGNAL);
                //total_bytes_writen += bytes_writen;
            //}
            //printf("data recibida\n");



            //jugador1->setPosX(pos1 -> posX);
			//jugador1->setPosY(pos1 -> posY);

            SDL_RenderClear( sdl.getRenderer() );
			renderBackground();

            //render jugador
            jugador->renderizar();
            jugador1->renderizar();

            //SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );

            //Todo este bloque deberiamos declararlo en otro lado
            //for(int i = 0; i < renderizados && i < cantidad_enemigos; i++){
                //enemigos[i]->mover(jugador);
                //enemigos[i]->renderizar();
            //}

            tiempo_nivel = temporizador.transcurridoEnSegundos();
            if(tiempo_nivel/renderizados > tiempo_por_enemigos) renderizados++;

            //Hasta aca
            SDL_RenderPresent( sdl.getRenderer() );
            //free(v);
            //for(int i = time(NULL) + 15; time(NULL) != i; time(NULL));

        }
        //free(pos);
        //free(v);
        //vector<NaveEnemiga*>::iterator pos;
        // CIERRA LAS NAVES
        jugador->cerrarNave();
        client->finalizar();
        //for(pos = enemigos.begin(); pos != enemigos.end(); pos++){
        //    (*pos)->cerrarNave();
        //}
        return quit;
}

void Nivel::finalizar() {
    logger.info("Finalizó el nivel");
    gFinNivel.render(0,0, &dataFinNivel);
    SDL_RenderPresent( sdl.getRenderer() );
    logger.info("Se renderizo el final del nivel");
}


void Nivel::cerrar(){}

void Nivel::cargarNivel(){}

void Nivel::renderBackground(){}
