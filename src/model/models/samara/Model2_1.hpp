/**
 * @file model/models/samara/Model2_1.hpp
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

#ifndef MODEL_MODELS_SAMARA_MODEL2_1_HPP
#define MODEL_MODELS_SAMARA_MODEL2_1_HPP

#include <model/models/ModelParameters.hpp>
#include <model/models/meteo/Meteo.hpp>
#include <model/models/samara/Model.hpp>

#include <utils/DateTime.hpp>

namespace model { namespace models { namespace samara {

class Model2_1 : public Model
{
public:
    Model2_1()
    { }

    virtual ~Model2_1()
    { }

    void compute(double time);

    void init(const model::models::ModelParameters& parameters);

private:
    void evalDegreeDay();
    void phyllochron();
    void evalCstrAssim();
    void evalRespMaint();
    void evolPlantTilNumTot();
    void evolPlantLeafNumTot();
    void evalRelPotLeafLength();
    void evalDemandStructSheath();
    void evalDemandStructRoot();
    void evalDemandStructPanicle();
    void evolGrowthStructSheathPop();
    void evolGrowthStructRootPop();
    void evolGrowthStructPanPop();
    void evalSlaMitch();
    void evolKcpKceBilhy();
    void evolConsRes_Flood();
    void evalConversion();
    void evolPSPMVMD(double t);
    void evolSomDegresJourCor();
    void evalMaximumLai();
    void evalVitesseRacinaire();
    void initCrop();
    void evalRootFront();
    void evalSDJPhase4();
    void evalDateGermination();
    void evalSterility();
    void initParcelle();
    void evalEvapPot();
    void transplanting();
    void transplantingShock();
    void evalRuiss_FloodDyna();
    void evolRempliResRFE_RDE();
    void plantSubmergence();
    void excessAssimilToRoot();
    void evolRempliMacropores();
    void evalFTSW();
    void evalDAF();
    void evalSimStartGermin();
    void evalSimEmergence();
    void evalSimStartPSP();
    void evalSimPanIni();
    void evalSimAnthesis50();
    void evalSimStartMatu2();
    void evalColdStress();
    void evalAssim();
    void evolWaterLoggingUpland();
    void evalStressWaterLogging();
    void evalCstrPFactorFAO();
    void evolHauteur_SDJ_cstr();
    void evalParIntercepte();
    void evolMobiliTillerDeath();
    void evolMobiliLeafDeath();
    void evalSupplyTot();
    void evalDemandStructLeaf();
    void evalDemandStructIN();
    void evalDemandTotAndIcPreFlow();
    void evolGrowthStructLeafPop();
    void evolGrowthStructINPop();
    void addResToGrowthStructPop();
    void evolDemPanFilPopAndIcPFlow();
    void evolPanicleFilPop();
    void evolGrowthReserveInternode();
    void evolGrowthTot();
    void evalLai();
    void evalClumpAndLightInter();
    void evalRUE();
    void evolEvapSurfRFE_RDE();
    void evolDryMatTot();
    void evolGrowthStructTot();
    void leafRolling();
    void evalAssimPot();
    void automaticIrrigation();
    void evolRurRFE_RDE();
    void evalSimEndCycle();
    void priority2GrowthPanStrctPop();
    void keyResults();
    void degToRad();
    void avgTempHum();
    void evalDecli(double t);
    void evalSunPosi();
    void evalDayLength();
    void evalSunDistance(double t);
    void evalRayExtra();
    void evalRgMax();
    void insToRg();
    void evalPAR();
    void etoFAO();
    void evolPhenoStress(double t);
    void demandePlante();
    void evalTranspi();
    void evalETRETM();
    void evolSomDegresJour();
    void mortality();
    void evalNbJas(double t);
    void evalTMaxMoy();
    void evalTMinMoy();
    void evalFtswMoy();
    double calculeLaMoyenne(double laValeur, double leCompteur, double laMoyenne);

    static double NullValue;

};

} } } // namespace model models samara

#endif
