#include "NivelServer.h"

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

typedef struct data_send{
    Server* server;
    posiciones_t* pos;
    int id;
}data_send_t;

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

void* loop_enviar_server(void* p) {
    data_send_t* data = (data_send_t*)p;
    Server* server = data->server;
    server->sendData(data);
    return NULL;
}

bool NivelServer::procesar(){

	    bool quit = false;

	    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
	    Temporizador temporizador;
	    temporizador.iniciar();

        Server* server = (Server*) estado;

        std::map<int, NaveJugador*> jugadores;

        long int n = server->get_max_users();
        //for(int i = 0; i < n; ++i){
            //send(server->get_socket(i), (void*)n, sizeof(long int), MSG_NOSIGNAL);
        //}
        int x = 0;
        for(int i = 0; i < n; ++i){
            x += 100;
            NaveJugador* jugador = new NaveJugador( x, sdl.getScreenWidth() / 4, i);
            jugadores.insert({jugador->get_id(), jugador});
        }

        //NaveJugador* jugador1 = new NaveJugador( sdl.getScreenWidth() / 4, sdl.getScreenWidth() / 4, 0);
        //NaveJugador* jugador2 = new NaveJugador( sdl.getScreenWidth() / 2, sdl.getScreenWidth() / 4, 1);

        //std::map<int, NaveJugador*> jugadores = {
          //  { jugador1->get_id(), jugador1 },
            //{ jugador2->get_id(), jugador2 }
        //};

        vector<NaveEnemiga*> enemigos = crear_enemigos();
        //vector<NaveJugador*> jugadores(2);
        //jugadores[0] = jugador;
        //jugadores[1] = jugador1;

        float tiempo_por_enemigos;
        if(cantidad_enemigos != 0){
            tiempo_por_enemigos = TIEMPO_NIVEL_SEGS/cantidad_enemigos;
        }else{
            tiempo_por_enemigos = 0;
        }
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
                //int j = get_nave(jugadores, v->id);
                //printf("SERVER encontro j\n");
                //jugadores[j]->setVelX(v->VelX);
                NaveJugador* jugador = jugadores[v->id];
                jugador->setVelX(v->VelX);
                printf("SERVER seteo X %d\n", v->VelX);
                //jugadores[j]->setVelY(v->VelY);
                jugador->setVelY(v->VelY);
                printf("SERVER seteo Y %d\n", v->VelY);
                //jugadores[j]->mover(enemigos);
                jugador->mover(enemigos);
                printf("SERVER movio al jugador\n");
                posiciones_t* pos = (posiciones_t*) malloc(sizeof(posiciones_t));
                //pos->posX = jugadores[j]->getPosX();
                pos->posX = jugador->getPosX();
                //pos->posY = jugadores[j]->getPosY();
                pos->posY = jugador->getPosY();
                pos->id = v->id;
                printf("SERVER X:%d - Y:%d, ID:%d\n", pos->posX, pos->posY, pos->id);
                int total_bytes_writen = 0;
                int bytes_writen = 0;
                int sent_data_size = sizeof(posiciones_t);
                for(int i = 0; i < n; ++i){
                    while(sent_data_size > total_bytes_writen) {
                        bytes_writen = send(server->get_socket(i), pos+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
                        total_bytes_writen += bytes_writen;
                        if(bytes_writen<=0)break;
                    }
                    total_bytes_writen = 0;
                    bytes_writen = 0;
                }
                //data_send_t* data = (data_send_t*) malloc(sizeof(data_send_t));
                //data->server = server;
                //data->pos = pos;
                //for(int i = 0; i < n; ++i){
                    //data->id = i;
                    //pthread_t hilo;
                    //pthread_create(&hilo, NULL, loop_enviar_server, data);
                //}

                //send(server->get_socket(0), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
                //send(server->get_socket(1), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
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
            //for(int i = 0; i < renderizados && i < cantidad_enemigos; i++){
                //enemigos[i]->mover(jugador);
                //enemigos[i]->renderizar();
            //}

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
        for(int i = 0; i < n; ++i){
            jugadores[i]->cerrarNave();
        }
        //jugador1->cerrarNave();
        //jugador2->cerrarNave();
        for(posi = enemigos.begin(); posi != enemigos.end(); posi++){
            (*posi)->cerrarNave();
        }
        return quit;
}

void NivelServer::cerrar(){}

void NivelServer::cargarNivel(){}

void NivelServer::renderBackground(){}
