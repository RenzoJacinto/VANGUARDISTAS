#include "NivelCliente.h"

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

bool NivelCliente::procesar(){

	    bool quit = false;

	    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
	    Temporizador temporizador;
	    temporizador.iniciar();

        Client* client = (Client*) estado;

        int id = client->get_id();
        std::map<int, NaveJugador*> jugadores;
        NaveJugador* jugador1 = new NaveJugador( 100, sdl.getScreenWidth() / 4, id);
        jugadores.insert({id, jugador1});
        const int n = 2;
        for(int i = 0; i < (n-1); ++i){
            ++id;
            NaveJugador* jugador = new NaveJugador( 200, sdl.getScreenWidth() / 4, id%n);
            jugadores.insert({jugador->get_id(), jugador});
        }
        //id++;
        //NaveJugador* jugador2 = new NaveJugador( sdl.getScreenWidth() / 2, sdl.getScreenWidth() / 4, id%2);

       // std::map<int, NaveJugador*> jugadores = {
         //   { jugador1->get_id(), jugador1 },
           // { jugador2->get_id(), jugador2 }
        //};

        //vector<NaveJugador*> jugadores(2);
        //jugadores[0] = jugador;
        //jugadores[1] = jugador1;
        //vector<NaveEnemiga*> enemigos = crear_enemigos();

        float tiempo_por_enemigos;
        if(cantidad_enemigos != 0){
            tiempo_por_enemigos = TIEMPO_NIVEL_SEGS/cantidad_enemigos;
        }else{
            tiempo_por_enemigos = 0;
        }
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
                jugador1->handleEvent( e );
            }

            velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
            v->id = client->get_id();
            v->VelX = jugador1->getVelX();
            v->VelY = jugador1->getVelY();

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


            printf("CLIENT: pos: %d - %d\n", jugador1 -> getPosX(), jugador1->getPosY());

            while(!client->cola_esta_vacia()) {
                printf("recibio cosas\n");
                posiciones_t* pos = (posiciones_t*)client->desencolar_procesar();
                printf("desencolo\n");
                //int j = get_nave(jugadores, pos->id);
                //printf("encontro j %d\n", j);
                NaveJugador* jugador = jugadores[pos->id];
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
            for(int i = 0; i < n; ++i){
                jugadores[i]->renderizar();
            }
            //jugador1->renderizar();
            //jugador2->renderizar();

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
        for(int i = 0; i < n; ++i){
            jugadores[i]->cerrarNave();
        }
        //jugador1->cerrarNave();
        //jugador2->cerrarNave();
        client->finalizar();
        //for(pos = enemigos.begin(); pos != enemigos.end(); pos++){
        //    (*pos)->cerrarNave();
        //}
        return quit;
}

void NivelCliente::cerrar(){}

void NivelCliente::cargarNivel(){}

void NivelCliente::renderBackground(){}
