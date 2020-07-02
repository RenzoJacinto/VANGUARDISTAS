#ifndef TYPESMSJ_H_
#define TYPESMSJ_H_

typedef struct client{
    char id[28];
    char pass[28];
} client_t;

typedef struct position{
    int client_socket;
    int x;
    int y;
} position_t;

typedef struct client_vw{
    int tipo_nave;
    int serial;
    int x;
    int y;
} client_vw_t;

static const int TIPO_JUGADOR = 0;
static const int TIPO_ENEMIGO = 1;

static const int SERIAL_ENEMIGO_1 = 1;
static const int SERIAL_ENEMIGO_2 = 2;
static const int SERIAL_ENEMIGO_3 = 3;
static const int SERIAL_ENEMIGO_4 = 4;

static const int SERIAL_JUGADOR_1 = 1;
static const int SERIAL_JUGADOR_2 = 2;
static const int SERIAL_JUGADOR_3 = 3;
static const int SERIAL_JUGADOR_4 = 4;


#endif
