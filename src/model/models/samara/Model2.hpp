/**
 * @file model/models/samara/Model2_1.hpp
 * @author The Samara Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2010-2014 Cirad http://www.cirad.fr
 * Copyright (C) 2014 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODEL_MODELS_SAMARA_MODEL2_HPP
#define MODEL_MODELS_SAMARA_MODEL2_HPP

#include <model/models/ModelParameters.hpp>
#include <model/models/meteo/Meteo.hpp>
#include <model/models/samara/Model.hpp>

#include <utils/DateTime.hpp>

namespace model { namespace models { namespace samara {

class Model2 : public Model
{
public:
    Model2()
    { }

    virtual ~Model2()
    { }

    virtual void compute(double time) = 0;

    void init(const model::models::ModelParameters& parameters);

protected:

    void initParcelle();
    void transplanting();
    void evalDegreeDay();
    void evalDAF();
    void evalFTSW();
    void evalCstrPFactorFAO();
    void evolConsRes_Flood();
    void transplantingShock();
    void evolPlantTilNumTot();
    void evalDemandStructRoot();
    void evalDemandStructPanicle();
    void excessAssimilToRoot();
    void evalRuiss_FloodDyna();
    void evolRempliResRFE_RDE();
    void evolWaterLoggingUpland();
    void evalStressWaterLogging();
    void evolRempliMacropores();
    void plantSubmergence();

    static double NullValue;

};

} } } // namespace model models samara

#endif
