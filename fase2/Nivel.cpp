#include "Nivel.h"
#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Temporizador.h"
#include "global.h"

Nivel::Nivel(){
    dataFinNivel.h = 600;
    dataFinNivel.w = 800;
    dataFinNivel.x = 0;
    dataFinNivel.y = 0;
}

vector<NaveEnemiga*> Nivel::crear_enemigos(client_vw_t** clients_vw){
    vector<NaveEnemiga*> enemigos(cantidad_enemigos);
    size_t size_view = sizeof(client_vw_t);
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

        client_vw_t* client_view = (client_vw_t*)malloc(size_view);
        client_view->serial = enemigo_random;
        client_view->tipo_nave = TIPO_ENEMIGO;
        client_view->x = enemigos[i]->getPosX();
        client_view->y = enemigos[i]->getPosY();

        clients_vw[i] = client_view;
    }
    std::string mensaje = "Se crearon los " + std::to_string(cantidad_enemigos) + " enemigos";
    logger.debug(mensaje.c_str());
    return enemigos;
}

bool Nivel::procesarServer(ColaMultihilo* cola, std::string nivel){

	    bool quit = false;

	    cantidad_enemigos = json.get_cantidad_enemigo(nivel.c_str());
        if(cantidad_enemigos == 0){
            std::string msj = "Cantidad de enemigos del " + nivel + " inexistente, se cargo una por defecto";
            logger.error(msj.c_str());
            cantidad_enemigos = json.get_cantidad_enemigo_default(nivel.c_str());
        }
        std::string mensaje = "Se cargo la cantidad de enemigos: " + std::to_string(cantidad_enemigos);
        logger.debug(mensaje.c_str());
        std::cout<<mensaje<<"\n";

	    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
	    Temporizador temporizador;
	    temporizador.iniciar();

        size_t size_view = sizeof(client_vw_t);
        clients_vw = (client_vw_t**)malloc(size_view * cantidad_enemigos);
        vector<NaveEnemiga*> enemigos = crear_enemigos(clients_vw);

        float tiempo_por_enemigos = TIEMPO_NIVEL_SEGS/cantidad_enemigos;
        double tiempo_nivel = 0;
        int renderizados = 1;


	    // Mientras que siga corriendo la app
	    while( tiempo_nivel < TIEMPO_NIVEL_SEGS ) {

            for(int i = 0; i < renderizados && i < cantidad_enemigos; i++){
                enemigos[i]->mover();
                client_vw_t* client_view = (client_vw_t*)malloc(size_view);
                client_view->tipo_nave = clients_vw[i]->tipo_nave;
                client_view->serial = clients_vw[i]->serial;
                client_view->x = enemigos[i]->getPosX();
                client_view->y = enemigos[i]->getPosY();

                cola->push(client_view);
            }

            tiempo_nivel = temporizador.transcurridoEnSegundos();
            if(tiempo_nivel/renderizados > tiempo_por_enemigos) renderizados++;

            //Hasta aca
        }
        vector<NaveEnemiga*>::iterator pos;
        // CIERRA LAS NAVES
        for(pos = enemigos.begin(); pos != enemigos.end(); pos++){
            (*pos)->cerrarNave();
        }
        return quit;
}

bool Nivel::procesarClient(position_t* pos){

	    bool quit = false;
        NaveJugador* jugador = new NaveJugador(10,10);
	    // Mientras que siga corriendo la app
	    while( usuarioNoRequieraSalir(quit)) {
            while( hayEventos() ){
		         if( eventoEsSalir() ) quit = true;
		         jugador->handleEvent(e);
            }
            jugador->mover();
            pos->x = jugador->getPosX();
            pos->y = jugador->getPosY();

            SDL_RenderClear(sdl.getRenderer());
			renderBackground();
			SDL_RenderPresent(sdl.getRenderer());
        }
        //jugador->cerrarNave();
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
