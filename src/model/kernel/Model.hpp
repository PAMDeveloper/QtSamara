/**
 * @file model/kernel/Model.hpp
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

#ifndef MODEL_KERNEL_MODEL_HPP
#define MODEL_KERNEL_MODEL_HPP

#include <model/models/ModelParameters.hpp>

#include <model/models/samara/Model.hpp>

#include <map>
#include <vector>

namespace model { namespace kernel {

class Model
{
public:
    Model() : model(0)
    { }

    virtual ~Model()
    { delete model; }

    void build();

    void compute(double t);

    void init(const model::models::ModelParameters& parameters)
    { model->init(parameters); }

private:
// models
    model::models::samara::Model* model;
};

} }

#endif
