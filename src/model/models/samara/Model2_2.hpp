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

#ifndef MODEL_MODELS_SAMARA_MODEL2_2_HPP
#define MODEL_MODELS_SAMARA_MODEL2_2_HPP

#include <model/models/ModelParameters.hpp>
#include <model/models/meteo/Meteo.hpp>
#include <model/models/samara/Model2.hpp>

#include <utils/DateTime.hpp>

namespace model { namespace models { namespace samara {

class Model2_2 : public Model2
{
public:
    Model2_2()
    { }

    virtual ~Model2_2()
    { }

    void compute(double time);

    void init(const model::models::ModelParameters& parameters);

protected:

    void transplanting();
    void evalRespMaint();
    void evalRelPotLeafLength();
    void evolPlantTilNumTot();
    void evolMobiliTillerDeath();
    void evalSlaMitch();
    void evalRUE();

    static double NullValue;

};

} } } // namespace model models samara

#endif
