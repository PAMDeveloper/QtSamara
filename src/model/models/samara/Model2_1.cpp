/**
 * @file model/models/samara/Model2_1.cpp
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

#include <model/models/samara/Model2_1.hpp>

namespace model { namespace models { namespace samara {

double Model2_1::NullValue = -999;

void Model2_1::compute(double t)
{
    Model::evalNbJas(t);
    Model2::transplanting(); // ok
    Model::degToRad(); // ok
    Model::avgTempHum(); // ok
    Model::evalDecli(t); // ok
    Model::evalSunPosi(); // ok
    Model::evalDayLength(); // ok
    Model::evalSunDistance(t); // ok
    Model::evalRayExtra(); // ok
    Model::evalRgMax(); // ok
    Model::insToRg(); // ok
    Model::evalPAR(); // ok
    Model::etoFAO(); // ok
    Model::evolPhenoStress(t); // ok
    // Model::evalSimAnthesis50(); // ok
    Model::evalDateGermination(); // ok
    Model::evalColdStress(); // ok
    // Model::evalSimEmergence();
    // Model::evalSimPanIni();
    // Model::evalSimStartGermin();
    // Model::evalSimStartMatu2();
    // Model::evalSimStartPSP();
    Model2::evalDegreeDay(); // ok
    Model::evalSDJPhase4();
    Model2::evalDAF();
    Model::phyllochron(); // ok
    Model2_1::evolHauteur_SDJ_cstr(); // ok
    Model::evolKcpKceBilhy(); // ok
    Model::evalEvapPot(); // ok
    Model2_1::evolEvapSurfRFE_RDE(); // ok
    Model2::evalFTSW(); // ok
    Model2::evalCstrPFactorFAO(); // ok
    Model2_1::demandePlante(); // ok
    Model::evalTranspi(); // ok
    Model::evalETRETM(); // ok
    Model2::evolConsRes_Flood(); // bof bof
    Model::evalTMaxMoy(); // ok
    Model::evalTMinMoy(); // ok
    Model::evalFtswMoy(); // ok
    Model::evalSterility(); // ok
    Model::evalVitesseRacinaire(); // ok
    Model::evalConversion(); // ok
    Model2_1::evalParIntercepte(); // ok
    Model2_1::evalAssimPot(); // ok
    Model::evalCstrAssim(); // ok
    Model::evalAssim(); // ok
    Model2::transplantingShock(); // ok
    Model::evalRespMaint(); // ok
    Model::evalRelPotLeafLength(); // ok
    Model2::evolPlantTilNumTot(); // ok
    Model::evolPlantLeafNumTot(); // ok
    Model2_1::evolMobiliTillerDeath(); // bof bof
    Model2_1::evolMobiliLeafDeath(); // bof bof
    Model2_1::evalSupplyTot(); // bof bof
    Model2_1::evalDemandStructLeaf(); // bof bof
    Model::evalDemandStructSheath(); // bof bof
    Model2::evalDemandStructRoot();
    Model2_1::evalDemandStructIN();
    Model2::evalDemandStructPanicle();
    Model2_1::evalDemandTotAndIcPreFlow();
    Model2_1::evolGrowthStructLeafPop();
    Model::evolGrowthStructSheathPop();
    Model::evolGrowthStructRootPop();
    Model2_1::evolGrowthStructINPop();
    Model::evolGrowthStructPanPop();
    Model2_1::priority2GrowthPanStrctPop();
    Model2_1::evolGrowthStructTot();
    Model2_1::addResToGrowthStructPop();
    Model2_1::evolDemPanFilPopAndIcPFlow();
    Model2_1::evolPanicleFilPop();
    Model2_1::evolGrowthReserveInternode();
    Model2_1::evolGrowthTot();
    Model2::excessAssimilToRoot();
    Model2_1::evolDryMatTot();
    Model2_1::evalLai();
    Model::evalMaximumLai();
    Model2_1::leafRolling();
    Model2_1::evalClumpAndLightInter();
    Model::evalSlaMitch();
    Model2::evalRuiss_FloodDyna();
    Model2_1::automaticIrrigation();
    Model2::evolRempliResRFE_RDE();
    Model2::evolWaterLoggingUpland();
    Model2::evalStressWaterLogging();
    Model2::evolRempliMacropores();
    Model2_1::evolRurRFE_RDE();
    Model2::plantSubmergence();
    Model::evalRootFront();
    Model::evolPSPMVMD(t);
    Model::evolSomDegresJour();
    Model::evolSomDegresJourCor();
    Model2_1::evalRUE();
    // Model::mortality(); //pour l'instant
    // Model2_1::keyResults();
    // Model2_1::evalSimEndCycle();
}

void Model2_1::init(const model::models::ModelParameters& parameters)
{

    Model2::init(parameters);

    initCrop();
    initParcelle();
}

} } } // namespace model models samara
