/**
 * @file model/models/samara/Model.cpp
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

#include <model/models/samara/Model.hpp>

namespace model { namespace models { namespace samara {

double Model::NullValue = -999;

void Model::compute(double t)
{
    evalDegreeDay();
    phyllochron();
    EvalCstrAssim();
    EvalRespMaint();
    EvolPlantTilNumTot();
    EvolPlantLeafNumTot();
    EvalRelPotLeafLength();
    EvalDemandStructSheath();
    EvalDemandStructRoot();
    EvalDemandStructPanicle();
    EvolGrowthStructSheathPop();
    EvolGrowthStructRootPop();
    EvolGrowthStructPanPop();
    EvalSlaMitch();
    EvolKcpKceBilhy();
    EvolConsRes_Flood();
    EvalConversion();
    EvolPSPMVMD();
    EvolSomDegresJourCor();
    EvalMaximumLai();
    EvalVitesseRacinaire();
    InitiationCulture();
    EvalRootFront();
    EvalSDJPhase4();
    EvalDateGermination();
    EvalSterility();
    InitParcelle();
    EvalEvapPot();
    transplanting();
    TransplantingShock();
    EvalRuiss_FloodDyna();
    EvolRempliResRFE_RDE();
    PlantSubmergence();
    ExcessAssimilToRoot();
    EvolRempliMacropores();
    EvalFTSW();
    EvalDAF();
    EvalSimStartGermin();
    EvalSimEmergence();
    EvalSimStartPSP();
    EvalSimPanIni();
    EvalSimAnthesis50();
    EvalSimStartMatu2();
    EvalColdStress();
    EvalAssim();
    EvolWaterLoggingUpland();
    EvalStressWaterLogging();
    EvalCstrPFactorFAO();
    EvolHauteur_SDJ_cstr();
    EvalParIntercepte();
    EvolMobiliTillerDeath();
    EvolMobiliLeafDeath();
    EvalSupplyTot();
    EvalDemandStructLeaf();
    EvalDemandStructIN();
    EvalDemandTotAndIcPreFlow();
    EvolGrowthStructLeafPop();
    EvolGrowthStructINPop();
    AddResToGrowthStructPop();
    EvolDemPanFilPopAndIcPFlow();
    EvolPanicleFilPop();
    EvolGrowthReserveInternode();
    EvolGrowthTot();
    EvalLai();
    EvalClumpAndLightInter();
    EvalRUE();
    EvolEvapSurfRFE_RDE();;
    EvolDryMatTot();
    EvolGrowthStructTot();
    LeafRolling();
    EvalAssimPot();
    AutomaticIrrigation();
    EvolRurRFE_RDE();
    EvalSimEndCycle();
    Priority2GrowthPanStrctPop();
    KeyResults();
    DegToRad();
    AVGTempHum();
    EvalDecli(t);
    EvalSunPosi();
    EvalDayLength();
    EvalSunDistance(t);
    EvalRayExtra();
    EvalRgMax();
    InsToRg();
    EvalPAR();
    EToFAO();
    EvolPhenoStress();
    DemandePlante();
    EvalTranspi();
    EvalETRETM();
    EvolSomDegresJour();
    Mortality();
}

} } } // namespace model models samara
