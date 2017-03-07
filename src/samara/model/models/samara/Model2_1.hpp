/**
 * @file samara/model/models/samara/Model2_1.hpp
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

#ifndef MODEL_MODELS_SAMARA_MODEL2_1_HPP
#define MODEL_MODELS_SAMARA_MODEL2_1_HPP

#include <samara/model/models/ModelParameters.hpp>
#include <samara/model/models/samara/Model2.hpp>

#include <samara/utils/DateTime.hpp>

namespace samara {

class SamaraModel2_1 : public SamaraModel2
{
public:
    SamaraModel2_1(const samara::AbstractModel* parent) : SamaraModel2(parent)
    { }

    virtual ~SamaraModel2_1()
    { }

    void compute(double t, bool update);
    void init(double t, const model::models::ModelParameters& parameters);

protected:
    void evolHauteur_SDJ_cstr();
    void evolEvapSurfRFE_RDE();
    void demandePlante();
    void evalParIntercepte();
    void evalAssimPot();
    void evolMobiliTillerDeath();
    void evolMobiliLeafDeath();
    void evalSupplyTot();
    void evalDemandStructLeaf();
    void evalDemandStructIN();
    void evalDemandTotAndIcPreFlow();
    void evolGrowthStructLeafPop();
    void evolGrowthStructINPop();
    void priority2GrowthPanStrctPop();
    void evolGrowthStructTot();
    void addResToGrowthStructPop();
    void evolDemPanFilPopAndIcPFlow();
    void evolPanicleFilPop();
    void evolGrowthReserveInternode();
    void evolGrowthTot();
    void evolDryMatTot();
    void evalLai();
    void leafRolling();
    void evalClumpAndLightInter();
    void automaticIrrigation();
    void evolRurRFE_RDE();
    void evalRUE();
    void evalSimEndCycle();
    void keyResults();

    static double NullValue;
};

} // namespace samara

#endif
