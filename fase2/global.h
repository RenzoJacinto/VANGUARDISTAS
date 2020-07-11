#include "ManejoDeSDL.h"
#include "ManejoDeLog.h"
#include "ManejoDeJson.h"
#include "ManejoDeSonidos.h"
#include "Estado.h"
#include <pthread.h>

extern ManejoDeSDL sdl;
extern ManejoDeLog logger;
extern ManejoDeJson json;
extern pthread_mutex_t mutex;
extern Estado* estado;
extern ManejoDeSonidos sounds;
