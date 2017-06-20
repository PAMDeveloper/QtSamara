#ifndef SAMARA_H
#define SAMARA_H

#include "samara_defines.h"
#include "parameters.h"

pair <vector <string>, vector < vector <double> > > run_samara_2_1(SamaraParameters * parameters);
void set_meteo_vars(SamaraParameters * parameters, double t);
void init_parameters(SamaraParameters * params);
void init_parcelle();
void eval_Par(double t);
void EToFao();
void kill_crop();
void reset_variables();
void init_culture();

#endif // SAMARA_H
