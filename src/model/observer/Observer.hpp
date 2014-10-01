/**
 * @file model/observer/Observer.hpp
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

#ifndef MODEL_OBSERVER_OBSERVER_HPP
#define MODEL_OBSERVER_OBSERVER_HPP

#include <model/kernel/Model.hpp>

#include <model/observer/LAIView.hpp>
#include <model/observer/NUMPHASEView.hpp>
#include <model/observer/DEGRESDUJOURView.hpp>
#include <model/observer/DEGRESDUJOURCORView.hpp>
#include <model/observer/View.hpp>

#include <vector>

namespace model { namespace observer {

class Observer
{
    typedef std::vector < View* > Views;

public:
    Observer(const model::kernel::Model* model) :
        model(model)
    { }

    virtual ~Observer()
    {
        for (typename Views::iterator it = views.begin(); it != views.end();
             ++it) {
            delete (*it);
        }
    }

    void attachView(View* view)
    {
        views.push_back(view);
        view->attachModel(model);
    }

    void init()
    {
        // attachView(new LAIView);
        attachView(new NUMPHASEView);
        attachView(new DEGRESDUJOURView);
        attachView(new DEGRESDUJOURCORView);

    }

    void observe(double t)
    {
        for (typename Views::iterator it = views.begin(); it != views.end();
             ++it) {
            (*it)->observe(t);
        }
    }

private:
    const model::kernel::Model* model;
    Views views;
};

} } // namespace tnt model

#endif
