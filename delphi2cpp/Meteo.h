#ifndef METEO_H
#define METEO_H
#include "parameters.h"


namespace meteo {
const samara::ModelParameters * parameters;

void init_meteo(const samara::ModelParameters& p) {
    parameters = &p;
}





}


#endif // METEO_H
