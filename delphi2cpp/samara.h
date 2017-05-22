#ifndef SAMARA_H
#define SAMARA_H

class samara::ModelParameters;

void run_samara_2_1(double start, double end, const samara::ModelParameters& parameters);
void set_meteo_vars(const samara::ModelParameters& parameters);
void init_parameters(const samara::ModelParameters& parameters);
void init_parcelle();
void eval_Par();
void kill_crop();
void reset_variables();

#endif // SAMARA_H
