/**
 * @file Meteo.cpp
 * @author The TNT Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2013-2014 ULCO http://www.univ-littoral.fr
 * Copyright (C) 2013-2014 INRA http://www.inra.fr
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

#include <model/models/meteo/Meteo.hpp>

using namespace BOOST_SPIRIT;

namespace model { namespace models { namespace meteo {

Meteo::Meteo(
    const model::models::ModelParameters& parameters) :
    TemporalFileReader(parameters)
{
    std::vector < std::string > fields;

    jumpLines(1);
    parseHeader(fields);
    parseColumnHeader(fields, "Tmin", "Tmin");
    parseColumnHeader(fields, "Tmax", "Tmax");
    parseColumnHeader(fields, "Pluie", "Rain");
    parseColumnHeader(fields, "ETP", "ETP");
    parseColumnHeader(fields, "Rayonnement Visible", "RG");
    createVariables();
    init(getBeginTime());
}

} } }

