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

void Model::init(const model::models::ModelParameters& parameters)
{
    ASScstr = parameters.get < double >("ASScstr");
    AttenMitch = parameters.get < double >("AttenMitch");
    BundHeight = parameters.get < double >("BundHeight");
    Ca = parameters.get < double >("Ca");
    CO2Cp = parameters.get < double >("CO2Cp");
    CO2Exp = parameters.get < double >("CO2Exp");
    CoeffAssimSla = parameters.get < double >("CoeffAssimSla");
    CoeffCO2Assim = parameters.get < double >("CoeffCO2Assim");
    CoefficientQ10 = parameters.get < double >("CoefficientQ10");
    CoeffInternodeMass = parameters.get < double >("CoeffInternodeMass");
    CoeffInternodeNum = parameters.get < double >("CoeffInternodeNum");
    CoeffLeafDeath = parameters.get < double >("CoeffLeafDeath");
    CoeffLeafWLRatio = parameters.get < double >("CoeffLeafWLRatio");
    CoeffPanicleMass = parameters.get < double >("CoeffPanicleMass");
    CoeffPanSinkPop = parameters.get < double >("CoeffPanSinkPop");
    CoeffResCapacityInternode =
        parameters.get < double >("CoeffResCapacityInternode");
    CoeffReserveSink = parameters.get < double >("CoeffReserveSink");
    CoeffRootMassPerVolMax =
        parameters.get < double >("CoeffRootMassPerVolMax");
    CoeffTillerDeath = parameters.get < double >("CoeffTillerDeath");
    CoeffTransplantingShock =
        parameters.get < double >("CoeffTransplantingShock");
    DensityField = parameters.get < double >("DensityField");
    DensityNursery = parameters.get < double >("DensityNursery");
    DEVcstr = parameters.get < double >("DEVcstr");
    DurationNursery = parameters.get < double >("DurationNursery");
    EpaisseurProf = parameters.get < double >("EpaisseurProf");
    EpaisseurSurf = parameters.get < double >("EpaisseurSurf");
    ExcessAssimToRoot = parameters.get < double >("ExcessAssimToRoot");
    FTSWIrrig = parameters.get < double >("FTSWIrrig");
    HaunCritTillering = parameters.get < double >("HaunCritTillering");
    HumFC = parameters.get < double >("HumFC");
    HumPF = parameters.get < double >("HumPF");
    HumSat = parameters.get < double >("HumSat");
    IcTillering = parameters.get < double >("IcTillering");
    InternodeLengthMax = parameters.get < double >("InternodeLengthMax");
    IrrigAutoResume = parameters.get < double >("IrrigAutoResume");
    IrrigAutoStop = parameters.get < double >("IrrigAutoStop");
    IrrigAutoTarget = parameters.get < double >("IrrigAutoTarget");
    KcMax = parameters.get < double >("KcMax");
    KCritSterCold1 = parameters.get < double >("KCritSterCold1");
    KCritSterCold2 = parameters.get < double >("KCritSterCold2");
    KCritSterFtsw1 = parameters.get < double >("KCritSterFtsw1");
    KCritSterFtsw2 = parameters.get < double >("KCritSterFtsw2");
    KCritSterHeat1 = parameters.get < double >("KCritSterHeat1");
    KCritSterHeat2 = parameters.get < double >("KCritSterHeat2");
    KCritStressCold1 = parameters.get < double >("KCritStressCold1");
    KCritStressCold2 = parameters.get < double >("KCritStressCold2");
    Kdf = parameters.get < double >("Kdf");
    kRespInternode = parameters.get < double >("kRespInternode");
    kRespMaintLeaf = parameters.get < double >("kRespMaintLeaf");
    kRespMaintRoot = parameters.get < double >("kRespMaintRoot");
    kRespMaintSheath = parameters.get < double >("kRespMaintSheath");
    kRespPanicle = parameters.get < double >("kRespPanicle");
    kTempMaint = parameters.get < double >("kTempMaint");
    LeafLengthMax = parameters.get < double >("LeafLengthMax");
    LifeSavingDrainage = parameters.get < double >("LifeSavingDrainage");
    Mulch = parameters.get < double >("Mulch");
    PanStructMassMax = parameters.get < double >("PanStructMassMax");
    PercolationMax = parameters.get < double >("PercolationMax");
    PFactor = parameters.get < double >("PFactor");
    Phyllo = parameters.get < double >("Phyllo");
    PlantsPerHill = parameters.get < double >("PlantsPerHill");
    PlotDrainageDAF = parameters.get < double >("PlotDrainageDAF");
    PoidsSecGrain = parameters.get < double >("PoidsSecGrain");
    PourcRuiss = parameters.get < double >("PourcRuiss");
    PPCrit = parameters.get < double >("PPCrit");
    PPExp = parameters.get < double >("PPExp");
    PriorityPan = parameters.get < double >("PriorityPan");
    ProfRacIni = parameters.get < double >("ProfRacIni");
    RankLongestLeaf = parameters.get < double >("RankLongestLeaf");
    RelMobiliInternodeMax = parameters.get < double >("RelMobiliInternodeMax");
    RelPotLeafLength = parameters.get < double >("RelPotLeafLength");
    RollingBase = parameters.get < double >("RollingBase");
    RollingSens = parameters.get < double >("RollingSens");
    RootCstr = parameters.get < double >("RootCstr");
    RootFrontMax = parameters.get < double >("RootFrontMax");
    RootPartitMax = parameters.get < double >("RootPartitMax");
    SDJBVP = parameters.get < double >("SDJBVP");
    SDJLevee = parameters.get < double >("SDJLevee");
    SDJMatu1 = parameters.get < double >("SDJMatu1");
    SDJMatu2 = parameters.get < double >("SDJMatu2");
    SDJRPR = parameters.get < double >("SDJRPR");
    SeuilPP = parameters.get < double >("SeuilPP");
    SeuilRuiss = parameters.get < double >("SeuilRuiss");
    SlaMax = parameters.get < double >("SlaMax");
    SlaMin = parameters.get < double >("SlaMin");
    StockIniProf = parameters.get < double >("StockIniProf");
    StockIniSurf = parameters.get < double >("StockIniSurf");
    TBase = parameters.get < double >("TBase");
    TempSla = parameters.get < double >("TempSla");
    TilAbility = parameters.get < double >("TilAbility");
    TLet = parameters.get < double >("TLet");
    TMax = parameters.get < double >("TMax");
    TMin = parameters.get < double >("TMin");
    TOpt1 = parameters.get < double >("TOpt1");
    TOpt2 = parameters.get < double >("TOpt2");
    TransplantingDepth = parameters.get < double >("TransplantingDepth");
    TxAssimBVP = parameters.get < double >("TxAssimBVP");
    TxAssimMatu1 = parameters.get < double >("TxAssimMatu1");
    TxAssimMatu2 = parameters.get < double >("TxAssimMatu2");
    TxConversion = parameters.get < double >("TxConversion");
    TxResGrain = parameters.get < double >("TxResGrain");
    VRacBVP = parameters.get < double >("VRacBVP");
    VRacLevee = parameters.get < double >("VRacLevee");
    VRacMatu1 = parameters.get < double >("VRacMatu1");
    VRacMatu2 = parameters.get < double >("VRacMatu2");
    VRacPSP = parameters.get < double >("VRacPSP");
    VRacRPR = parameters.get < double >("VRacRPR");
    WaterLoggingSens = parameters.get < double >("WaterLoggingSens");
    WtRatioLeafSheath = parameters.get < double >("WtRatioLeafSheath");
    IrrigAuto = parameters.get < int >("IrrigAuto");
    Transplanting = parameters.get < int >("Transplanting");
}

} } } // namespace model models samara
