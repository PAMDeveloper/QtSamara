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

#include <model/models/samara/Model2_2.hpp>

namespace model { namespace models { namespace samara {

double Model2_2::NullValue = -999;

void Model2_2::compute(double t)
{
    // evalNbJas(t);
    // transplanting(); // ok
    // degToRad(); // ok
    // avgTempHum(); // ok
    // evalDecli(t); // ok
    // evalSunPosi(); // ok
    // evalDayLength(); // ok
    // evalSunDistance(t); // ok
    // evalRayExtra(); // ok
    // evalRgMax(); // ok
    // insToRg(); // ok
    // evalPAR(); // ok
    // etoFAO(); // ok
    // evolPhenoStress(t); // ok
    // // evalSimAnthesis50(); // ok
    // evalDateGermination(); // ok
    // evalColdStress(); // ok
    // // evalSimEmergence();
    // // evalSimPanIni();
    // // evalSimStartGermin();
    // // evalSimStartMatu2();
    // // evalSimStartPSP();
    // evalDegreeDay(); // ok
    // evalSDJPhase4();
    // evalDAF();
    // phyllochron(); // ok
    // evolHauteur_SDJ_cstr(); // ok
    // evolKcpKceBilhy(); // ok
    // evalEvapPot(); // ok
    // evolEvapSurfRFE_RDE(); // ok
    // evalFTSW(); // ok
    // evalCstrPFactorFAO(); // ok
    // demandePlante(); // ok
    // evalTranspi(); // ok
    // evalETRETM(); // ok
    // evolConsRes_Flood(); // bof bof
    // evalTMaxMoy(); // ok
    // evalTMinMoy(); // ok
    // evalFtswMoy(); // ok
    // evalSterility(); // ok
    // evalVitesseRacinaire(); // ok
    // evalConversion(); // ok
    // evalParIntercepte(); // ok
    // evalAssimPot(); // ok
    // evalCstrAssim(); // ok
    // evalAssim(); // ok
    // transplantingShock(); // ok
    // evalRespMaint(); // ok
    // evalRelPotLeafLength(); // ok
    // evolPlantTilNumTot(); // ok
    // evolPlantLeafNumTot(); // ok
    // evolMobiliTillerDeath(); // bof bof
    // evolMobiliLeafDeath(); // bof bof
    // evalSupplyTot(); // bof bof
    // evalDemandStructLeaf(); // bof bof
    // evalDemandStructSheath(); // bof bof
    // evalDemandStructRoot();
    // evalDemandStructIN();
    // evalDemandStructPanicle();
    // evalDemandTotAndIcPreFlow();
    // evolGrowthStructLeafPop();
    // evolGrowthStructSheathPop();
    // evolGrowthStructRootPop();
    // evolGrowthStructINPop();
    // evolGrowthStructPanPop();
    // evolGrowthStructTot();
    // priority2GrowthPanStrctPop();
    // addResToGrowthStructPop();
    // evolDemPanFilPopAndIcPFlow();
    // evolPanicleFilPop();
    // evolGrowthReserveInternode();
    // evolGrowthTot();
    // excessAssimilToRoot();
    // evolDryMatTot();
    // evalLai();
    // evalMaximumLai();
    // leafRolling();
    // evalClumpAndLightInter();
    // evalSlaMitch();
    // evalRuiss_FloodDyna();
    // automaticIrrigation();
    // evolRempliResRFE_RDE();
    // evolWaterLoggingUpland();
    // evalStressWaterLogging();
    // evolRempliMacropores();
    // evolRurRFE_RDE();
    // plantSubmergence();
    // evalRootFront();
    // evolPSPMVMD(t);
    // evolSomDegresJour();
    // evolSomDegresJourCor();
    // evalRUE();
    // // //mortality(); //pour l'instant
    // // keyResults();
    // // evalSimEndCycle();
}

void Model2_2::init(const model::models::ModelParameters& parameters)
{
    Model2::init(parameters);

    initCrop();
    initParcelle();
}

} } } // namespace model models samara
