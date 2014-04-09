/**
 * @file model/models/meteo/Meteo.hpp
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

#ifndef MODEL_MODELS_METEO_METEO_HPP
#define MODEL_MODELS_METEO_METEO_HPP

#include <model/models/ModelParameters.hpp>
#include <model/models/common/TemporalFileReader.hpp>

namespace model { namespace models { namespace meteo {

class Meteo : public model::models::common::TemporalFileReader
{
public:
    Meteo(const model::models::ModelParameters& parameters);

    virtual ~Meteo()
    { }

protected:
    virtual std::string columnSeparator() const
    { return " \n\t"; }

    virtual std::string dateFormat() const
    { return ""; }

    virtual int yearColumn() const
    { return 0; }

    virtual int monthColumn() const
    { return 1; }

    virtual int weekColumn() const
    { return -1; }

    virtual int dayColumn() const
    { return 2; }
};

} } }

#endif
