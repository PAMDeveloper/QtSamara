/**
 * @file models/Model2_1.hpp
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

#ifndef MODEL_MODELS_SAMARA_MODEL2_2_HPP
#define MODEL_MODELS_SAMARA_MODEL2_2_HPP

#include <ModelParameters.hpp>
#include <models/Model2_1.hpp>

#include <artis/utils/DateTime.hpp>

class SamaraModel2_2 : public SamaraModel2_1
{
public:
    SamaraModel2_2() : SamaraModel2_1()
    { }

    virtual ~SamaraModel2_2()
    { }

    void compute(double t, bool update){
        TMax = _parameters.get(t).TMax;
        TMin = _parameters.get(t).TMin;
        TMoy = _parameters.get(t).TMoy;
        HMax = _parameters.get(t).HMax;
        HMin = _parameters.get(t).HMin;
        HMoy = _parameters.get(t).HMoy;
        Vent = _parameters.get(t).Vt;
        Ins = _parameters.get(t).Ins;
        Rg = _parameters.get(t).Rg;
        ETP = _parameters.get(t).ETP;
        Rain = _parameters.get(t).Rain;
        SamaraModel::evalNbJas(t);
        SamaraModel2_2::transplanting(); // ok
        SamaraModel::degToRad(); // ok
        SamaraModel::avgTempHum(); // ok
        SamaraModel::evalDecli(t); // ok
        SamaraModel::evalSunPosi(); // ok
        SamaraModel::evalDayLength(); // ok
        SamaraModel::evalSunDistance(t); // ok
        SamaraModel::evalRayExtra(); // ok
        SamaraModel::evalRgMax(); // ok
        SamaraModel::insToRg(); // ok
        SamaraModel::evalPAR(); // ok
        SamaraModel::etoFAO(); // ok
        SamaraModel::evolPhenoStress(t); // ok
        // Model::evalSimAnthesis50(); // ok
        SamaraModel::evalDateGermination(); // ok
        SamaraModel::evalColdStress(); // ok
        // Model::evalSimEmergence();
        // Model::evalSimPanIni();
        // Model::evalSimStartGermin();
        // Model::evalSimStartMatu2();
        // Model::evalSimStartPSP();
        SamaraModel2::evalDegreeDay(); // ok
        SamaraModel::evalSDJPhase4();
        SamaraModel2::evalDAF();
        SamaraModel::phyllochron(); // ok
        SamaraModel2_1::evolHauteur_SDJ_cstr(); // ok
        SamaraModel::evolKcpKceBilhy(); // ok
        SamaraModel::evalEvapPot(); // ok
        SamaraModel2_1::evolEvapSurfRFE_RDE(); // ok
        SamaraModel2::evalFTSW(); // ok
        SamaraModel2::evalCstrPFactorFAO(); // ok
        SamaraModel2_1::demandePlante(); // ok
        SamaraModel::evalTranspi(); // ok
        SamaraModel::evalETRETM(); // ok
        SamaraModel2::evolConsRes_Flood(); // bof bof
        SamaraModel::evalTMaxMoy(); // ok
        SamaraModel::evalTMinMoy(); // ok
        SamaraModel::evalFtswMoy(); // ok
        SamaraModel::evalSterility(); // ok
        SamaraModel::evalVitesseRacinaire(); // ok
        SamaraModel::evalConversion(); // ok
        SamaraModel2_1::evalParIntercepte(); // ok
        SamaraModel2_1::evalAssimPot(); // ok
        SamaraModel::evalCstrAssim(); // ok
        SamaraModel::evalAssim(); // ok
        SamaraModel2::transplantingShock(); // ok
        SamaraModel2_2::evalRespMaint(); // ok
        SamaraModel2_2::evalRelPotLeafLength(); // ok
        SamaraModel2_2::evolPlantTilNumTot(); // ok
        SamaraModel::evolPlantLeafNumTot(); // ok
        SamaraModel2_2::evolMobiliTillerDeath(); // bof bof
        SamaraModel2_1::evolMobiliLeafDeath(); // bof bof
        SamaraModel2_1::evalSupplyTot(); // bof bof
        SamaraModel2_1::evalDemandStructLeaf(); // bof bof
        SamaraModel::evalDemandStructSheath(); // bof bof
        SamaraModel2::evalDemandStructRoot();
        SamaraModel2_1::evalDemandStructIN();
        SamaraModel2::evalDemandStructPanicle();
        SamaraModel2_1::evalDemandTotAndIcPreFlow();
        SamaraModel2_1::evolGrowthStructLeafPop();
        SamaraModel::evolGrowthStructSheathPop();
        SamaraModel::evolGrowthStructRootPop();
        SamaraModel2_1::evolGrowthStructINPop();
        SamaraModel::evolGrowthStructPanPop();
        SamaraModel2_1::priority2GrowthPanStrctPop();
        SamaraModel2_1::evolGrowthStructTot();
        SamaraModel2_1::addResToGrowthStructPop();
        SamaraModel2_1::evolDemPanFilPopAndIcPFlow();
        SamaraModel2_1::evolPanicleFilPop();
        SamaraModel2_1::evolGrowthReserveInternode();
        SamaraModel2_1::evolGrowthTot();
        SamaraModel2::excessAssimilToRoot();
        SamaraModel2_1::evolDryMatTot();
        SamaraModel2_1::evalLai();
        SamaraModel::evalMaximumLai();
        SamaraModel2_1::leafRolling();
        SamaraModel2_1::evalClumpAndLightInter();
        SamaraModel2_2::evalSlaMitch();
        SamaraModel2::evalRuiss_FloodDyna();
        SamaraModel2_1::automaticIrrigation();
        SamaraModel2::evolRempliResRFE_RDE();
        SamaraModel2::evolWaterLoggingUpland();
        SamaraModel2::evalStressWaterLogging();
        SamaraModel2::evolRempliMacropores();
        SamaraModel2_1::evolRurRFE_RDE();
        SamaraModel2::plantSubmergence();
        SamaraModel::evalRootFront();
        SamaraModel::evolPSPMVMD(t);
        SamaraModel::evolSomDegresJour();
        SamaraModel::evolSomDegresJourCor();
        SamaraModel2_2::evalRUE();
        // Model::mortality(); //pour l'instant
        // Model2_1::keyResults();
        // Model2_1::evalSimEndCycle();
    }

    void init(double t, const samara::ModelParameters& parameters){
        SamaraModel2::init(t, parameters);
        initCrop();
        initParcelle();
    }

protected:
    void transplanting();
    void evalRespMaint();
    void evalRelPotLeafLength();
    void evolPlantTilNumTot();
    void evolMobiliTillerDeath();
    void evalSlaMitch();
    void evalRUE();
    void initParcelle();
};

#endif
