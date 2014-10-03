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
#include <model/models/meteo/Meteo.hpp>

#include <map>
#include <vector>

namespace model { namespace kernel {

class Model
{
public:
    Model() : samara_model(0), meteo_model(0)
    { }

    virtual ~Model()
    {
        delete samara_model;
        delete meteo_model;
    }

    void build();

    void compute(double t);

    void init(const model::models::ModelParameters& parameters)
    {
        samara_model->init(parameters);
        meteo_model->init(parameters);
    }

    double lai() const
    { return samara_model->lai(); }

    double numphase() const
    { return samara_model->numphase(); }

    double degresdujour() const
    { return samara_model->degresdujour(); }

    double degresdujourcor() const
    { return samara_model->degresdujourcor(); }

    double ftsw() const
    { return samara_model->ftsw(); }

    double cstr() const
    { return samara_model->cstr(); }

    double rootfront() const
    { return samara_model->rootfront(); }

    double eto() const
    { return samara_model->eto(); }

    double etm() const
    { return samara_model->etm(); }

    double etr() const
    { return samara_model->etr(); }

    double rue() const
    { return samara_model->rue(); }

    double conversioneff() const
    { return samara_model->conversioneff(); }

    double ic() const
    { return samara_model->ic(); }

    double culmsperplant() const
    { return samara_model->culmsperplant(); }

    double culmspop() const
    { return samara_model->culmspop(); }

    double culmsperhill() const
    { return samara_model->culmsperhill(); }

    double grainyieldpop() const
    { return samara_model->grainyieldpop(); }

    double drymatstructleafpop() const
    { return samara_model->drymatstructleafpop(); }

    double drymatstructsheathpop() const
    { return samara_model->drymatstructsheathpop(); }

    double drymatstructrootpop() const
    { return samara_model->drymatstructrootpop(); }

    double drymatstructinternodepop() const
    { return samara_model->drymatstructinternodepop(); }

    double drymatresinternodepop() const
    { return samara_model->drymatresinternodepop(); }

    double drymatstructpaniclepop() const
    { return samara_model->drymatstructpaniclepop(); }

    double drymatstructtotpop() const
    { return samara_model->drymatstructtotpop(); }

    double drymatvegetotpop() const
    { return samara_model->drymatvegetotpop(); }

    double drymatpanicletotpop() const
    { return samara_model->drymatpanicletotpop(); }

    double drymatstempop() const
    { return samara_model->drymatstempop(); }

    double drymatabovegroundpop() const
    { return samara_model->drymatabovegroundpop(); }

    double drymattotpop() const
    { return samara_model->drymattotpop(); }

    double sterilitycold() const
    { return samara_model->sterilitycold(); }

    double sterilityheat() const
    { return samara_model->sterilityheat(); }

    double sterilitydrought() const
    { return samara_model->sterilitydrought(); }

    double sterilitytot() const
    { return samara_model->sterilitytot(); }

    double harvestindex() const
    { return samara_model->harvestindex(); }

    double paniclenumpop() const
    { return samara_model->paniclenumpop(); }

    double paniclenumplant() const
    { return samara_model->paniclenumplant(); }

    double grainyieldpanicle() const
    { return samara_model->grainyieldpanicle(); }

    double spikenumpop() const
    { return samara_model->spikenumpop(); }

    double spikenumpanicle() const
    { return samara_model->spikenumpanicle(); }

    double fertspikenumpop() const
    { return samara_model->fertspikenumpop(); }

    double grainfillingstatus() const
    { return samara_model->grainfillingstatus(); }

    double phasestemelongation() const
    { return samara_model->phasestemelongation(); }

    double sla() const
    { return samara_model->sla(); }

    double haunindex() const
    { return samara_model->haunindex(); }

    double apexheight() const
    { return samara_model->apexheight(); }

    double plantheight() const
    { return samara_model->plantheight(); }

    double plantwidth() const
    { return samara_model->plantwidth(); }

    double krolling() const
    { return samara_model->krolling(); }

    double lirkdfcl() const
    { return samara_model->lirkdfcl(); }

    double ltrkdfcl() const
    { return samara_model->ltrkdfcl(); }

    double assimpot() const
    { return samara_model->assimpot(); }

    double assim() const
    { return samara_model->assim(); }

    double respmainttot() const
    { return samara_model->respmainttot(); }

    double supplytot() const
    { return samara_model->supplytot(); }

    double assimsurplus() const
    { return samara_model->assimsurplus(); }

    double assimnotused() const
    { return samara_model->assimnotused(); }

    double tillerdeathpop() const
    { return samara_model->tillerdeathpop(); }

    double plantleafnumtot() const
    { return samara_model->plantleafnumtot(); }

    double deadleafdrywtpop() const
    { return samara_model->deadleafdrywtpop(); }

    double laidead() const
    { return samara_model->laidead(); }

    double rescapacityinternodepop() const
    { return samara_model->rescapacityinternodepop(); }

    double internoderesstatus() const
    { return samara_model->internoderesstatus(); }

    double daylength() const
    { return samara_model->daylength(); }

    double par() const
    { return samara_model->par(); }

    double rgcalc() const
    { return samara_model->rgcalc(); }

    double vpdcalc() const
    { return samara_model->vpdcalc(); }

    double tmoycalc() const
    { return samara_model->tmoycalc(); }



private:
// models
    model::models::samara::Model* samara_model;
    model::models::meteo::Meteo* meteo_model;
};

} }

#endif
