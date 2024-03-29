#ifndef SAMARA_H
#define SAMARA_H

//#include <ModelParameters.hpp>
#include "delphi_defines.h"
#include "parameters.h"

void run_samara_2_1(/*const samara::ModelParameters& parameters*/);
void set_meteo_vars_2(SamaraParameters * parameters, double t);
void init_parameters_2(SamaraParameters * params);
void init_parcelle();
void eval_Par(double t);
void EToFao();
void kill_crop();
void reset_variables();
void init_culture();

#endif // SAMARA_H
