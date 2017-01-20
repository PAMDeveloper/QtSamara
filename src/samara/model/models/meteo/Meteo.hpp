/**
 * @file samara/model/models/meteo/Meteo.hpp
 * @author The Samara Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2013-2017 Cirad http://www.cirad.fr
 * Copyright (C) 2013-2017 ULCO http://www.univ-littoral.fr
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

#include <samara/model/kernel/AbstractAtomicModel.hpp>
#include <samara/model/models/ModelParameters.hpp>
#include <samara/model/models/common/TemporalFileReader.hpp>

namespace meteo {

struct Climate
{
    double TMax;
    double TMin;
    double TMoy;
    double HMax;
    double HMin;
    double HMoy;
    double Vt;
    double Ins;
    double Rg;
    double ETP;
    double Rain;

    Climate(double TMax, double TMin, double TMoy, double HMax, double HMin,
            double HMoy, double Vt, double Ins, double Rg, double ETP,
            double Rain) :
        TMax(TMax), TMin(TMin), TMoy(TMoy), HMax(HMax), HMin(HMin),
        HMoy(HMoy), Vt(Vt), Ins(Ins), Rg(Rg), ETP(ETP), Rain(Rain)
    { }
};

class Model : public samara::AbstractAtomicModel < Model >
{
public:
    Model(const samara::AbstractModel* parent)  :
        samara::AbstractAtomicModel < Model >(parent)
    { }

    virtual ~Model()
    { }

    void compute(double t, bool update);

    const Climate& get() const
    { return *it; }

    void init(double t, const model::models::ModelParameters& parameters);

private:
    double begin;
    double end;

    std::vector < Climate > values;
    std::vector < Climate >::const_iterator it;
};

}

#endif
