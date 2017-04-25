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

#ifndef MODEL_MODELS_SAMARA_MODEL2_1_HPP
#define MODEL_MODELS_SAMARA_MODEL2_1_HPP

#include <ModelParameters.hpp>
#include <models/Model2.hpp>

#include <artis/utils/DateTime.hpp>

class SamaraModel2_1 : public SamaraModel2
{
public:
    SamaraModel2_1() : SamaraModel2()
    { }

    virtual ~SamaraModel2_1()
    { }

    void compute(double t, bool /*update*/){
        //temporal params
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

        //compute
        SamaraModel::evalNbJas(t);
        SamaraModel2::transplanting();
        SamaraModel::degToRad();
        SamaraModel::avgTempHum();
        SamaraModel::evalDecli(t);
        SamaraModel::evalSunPosi();
        SamaraModel::evalDayLength();
        SamaraModel::evalSunDistance(t);
        SamaraModel::evalRayExtra();
        SamaraModel::evalRgMax();
        SamaraModel::insToRg();
        SamaraModel::evalPAR();
        SamaraModel::etoFAO();
        SamaraModel::evolPhenoStress(t);
        SamaraModel::evalSimAnthesis50();
        SamaraModel::evalDateGermination();
        SamaraModel::evalColdStress();
        SamaraModel::evalSimEmergence();
        SamaraModel::evalSimPanIni();
        SamaraModel::evalSimStartGermin();
        SamaraModel::evalSimStartMatu2();
        SamaraModel::evalSimStartPSP();
        SamaraModel2::evalDegreeDay();
        SamaraModel::evalSDJPhase4();
        SamaraModel2::evalDAF();
        SamaraModel::phyllochron();
        SamaraModel2_1::evolHauteur_SDJ_cstr();
        SamaraModel::evolKcpKceBilhy();
        SamaraModel::evalEvapPot();
        SamaraModel2_1::evolEvapSurfRFE_RDE();
        SamaraModel2::evalFTSW();
        SamaraModel2::evalCstrPFactorFAO();
        SamaraModel2_1::demandePlante();
        SamaraModel::evalTranspi();
        SamaraModel::evalETRETM();
        SamaraModel2::evolConsRes_Flood(); // bof bof
        SamaraModel::evalTMaxMoy();
        SamaraModel::evalTMinMoy();
        SamaraModel::evalFtswMoy();
        SamaraModel::evalSterility();
        SamaraModel::evalVitesseRacinaire();
        SamaraModel::evalConversion();
        SamaraModel2_1::evalParIntercepte();
        SamaraModel2_1::evalAssimPot();
        SamaraModel::evalCstrAssim();
        SamaraModel::evalAssim();
        SamaraModel2::transplantingShock();
        SamaraModel::evalRespMaint();
        SamaraModel::evalRelPotLeafLength();
        SamaraModel2::evolPlantTilNumTot();
        SamaraModel::evolPlantLeafNumTot();
        SamaraModel2_1::evolMobiliTillerDeath(); // bof bof
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
        SamaraModel::evalSlaMitch();
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
        SamaraModel2_1::evalRUE();
        SamaraModel::mortality(); //pour l'instant
        SamaraModel2_1::keyResults();
        SamaraModel2_1::evalSimEndCycle();
        //    for(auto it = parameters.getRawParameters()->begin(); it != parameters.getRawParameters()->end(); ++it) {
        //        std::cout << it->first << " " << it->second << std::endl;
        ////      v.push_back(it->first);
        ////      cout << it->first << "\n";
        //    }
    //    SamaraModel::writeResultInDB(parameters.get<std::string>("idsimulation"), t);
    //    SamaraModel::writeResultInFile(parameters.get<std::string>("idsimulation"), t);
    }

    void init(double t, const samara::ModelParameters& parameters){
        SamaraModel2::init(t, parameters);
        initCrop();
        initParcelle();
    }

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
};

#endif
