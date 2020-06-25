#include "ManejoDeSDL.h"
#include "ManejoDeLog.h"
#include "ManejoDeJson.h"
#include <pthread.h>
#include "ColaMultihilo.h"

extern ManejoDeSDL sdl;
extern ManejoDeLog logger;
extern ManejoDeJson json;
extern pthread_mutex_t mutex;
extern ColaMultihilo cola;
