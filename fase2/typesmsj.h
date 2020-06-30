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
    TextureW gNaveTexture;
    int x;
    int y;
} client_vw_t;

#endif
