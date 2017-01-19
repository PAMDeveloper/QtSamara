/**
 * @file model/kernel/Model.hpp
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

#include <model/kernel/Model.hpp>

namespace model { namespace kernel {

void Model::compute(double t, bool /* update */)
{
    (*meteo_model)(t);
    samara_model->put < double >(t, samara::Model::TMAX,
                                 meteo_model->get().TMax);
    samara_model->put < double >(t, samara::Model::TMIN,
                                 meteo_model->get().TMin);
    samara_model->put < double >(t, samara::Model::TMOY,
                                 meteo_model->get().TMoy);
    samara_model->put < double >(t, samara::Model::HMAX,
                                 meteo_model->get().HMax);
    samara_model->put < double >(t, samara::Model::HMIN,
                                 meteo_model->get().HMin);
    samara_model->put < double >(t, samara::Model::HMOY,
                                 meteo_model->get().HMoy);
    samara_model->put < double >(t, samara::Model::WIND,
                                 meteo_model->get().Vt);
    samara_model->put < double >(t, samara::Model::INS,
                                 meteo_model->get().Ins);
    samara_model->put < double >(t, samara::Model::RG,
                                 meteo_model->get().Rg);
    samara_model->put < double >(t, samara::Model::Etp,
                                 meteo_model->get().ETP);
    samara_model->put < double >(t, samara::Model::RAIN,
                                 meteo_model->get().Rain);
    (*samara_model)(t);
}

} }
