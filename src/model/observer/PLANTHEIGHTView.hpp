/**
 * @file model/observer/LAIView.hpp
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

#ifndef MODEL_OBSERVER_PLANTHEIGHTVIEW_HPP
#define MODEL_OBSERVER_PLANTHEIGHTVIEW_HPP

#include <model/observer/View.hpp>

#include <utils/DateTime.hpp>

namespace model { namespace observer {

class PLANTHEIGHTView : public View
{
public:
    PLANTHEIGHTView()
    { }

    virtual ~PLANTHEIGHTView()
    { }

    virtual void observe(double /* t */)
    {
        std::cout << "\t" << model->plantheight();
    }
};

} }

#endif
