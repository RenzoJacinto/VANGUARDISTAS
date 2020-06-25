#include "Estado.h"

Estado::Estado(){}

bool Estado::iniciar(){return true;}

bool Estado::sendData(void* dato){return true;}

void* Estado::receiveData(){return NULL;}

void* Estado::processData(void* dato){return NULL;}

void Estado::close(){}
