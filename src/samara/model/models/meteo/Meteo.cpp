/**
 * @file samara/model/models/meteo/Meteo.cpp
 * @author The Samara Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2013-2017 ULCO http://www.univ-littoral.fr
 * Copyright (C) 2013-2017 Cirad http://www.cirad.fr
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

#include <samara/model/models/meteo/Meteo.hpp>
#include <samara/utils/Connections.hpp>
#include <samara/utils/DateTime.hpp>

namespace meteo {

MeteoModel::MeteoModel (const samara::AbstractModel* parent)  :
    samara::AbstractAtomicModel < MeteoModel >(parent)
{ }


void MeteoModel::compute(double /* t */, bool /* update */)
{
    if (it == values.end()) {
        it = values.begin();
    } else {
        ++it;
    }
}
const model::models::Climate& MeteoModel::get() const
{ return *it; }

void MeteoModel::init(double /* t */,
                 const model::models::ModelParameters& parameters)
{
    this->values = parameters.getMeteoValues();
    it = values.end();
}

}
