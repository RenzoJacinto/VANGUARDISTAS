#include "ManejoDeSDL.h"
#include "ManejoDeLog.h"
#include "ManejoDeJson.h"
#include "ManejoDeSonidos.h"
#include <pthread.h>
#include "typesmsj.h"

extern ManejoDeSDL sdl;
extern ManejoDeLog logger;
extern ManejoDeJson json;
extern pthread_mutex_t mutex;
extern ManejoDeSonidos sounds;
