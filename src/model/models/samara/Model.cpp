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

void Model::assignClimate(const model::models::meteo::Climate& climate)
{
    TMax = climate.TMax;
    TMin = climate.TMin;
    TMoy = climate.TMoy;
    HMax = climate.HMax;
    HMin = climate.HMin;
    HMoy = climate.HMoy;
    Vent = climate.Vt;
    Ins = climate.Ins;
    Rg = climate.Rg;
    ETP = climate.ETP;
    Rain = climate.Rain;
}

void Model::compute(double t)
{
    transplanting(); // ok
    degToRad(); // ok
    avgTempHum(); // ok
    evalDecli(t); // ok
    evalSunPosi(); // ok
    evalDayLength(); // ok
    evalSunDistance(t); // ok
    evalRayExtra(); // ok
    evalRgMax(); // ok
    insToRg(); // ok
    evalPAR(); // ok
    etoFAO(); // ok
    evolPhenoStress(t); // ok
    // evalSimAnthesis50(); // ok
    // evalDateGermination(); // ok
    evalColdStress(); // ok
    // evalSimEmergence();
    // evalSimPanIni();
    // evalSimStartGermin();
    // evalSimStartMatu2();
    // evalSimStartPSP();
    evalDegreeDay(); // ok
    evalSDJPhase4();
    // evalDAF();
    phyllochron(); // ok
    evolHauteur_SDJ_cstr(); // ok
    evolKcpKceBilhy(); // ok
    evalEvapPot(); // ok
    evolEvapSurfRFE_RDE(); // ok
    evalFTSW(); // ok
    evalCstrPFactorFAO(); // ok
    demandePlante(); // ok
    evalTranspi(); // ok
    evalETRETM(); // ok
    evolConsRes_Flood(); // bof bof
    evalTMaxMoy(); // ok
    evalTMinMoy(); // ok
    evalFtswMoy(); // ok
    evalSterility(); // ok
    evalVitesseRacinaire(); // ok
    evalConversion(); // ok
    evalParIntercepte(); // ok
    evalAssimPot(); // ok
    evalCstrAssim(); // ok
    evalAssim(); // ok
    transplantingShock(); // ok
    evalRespMaint(); // ok
    evalRelPotLeafLength(); // ok
    evolPlantTilNumTot(); // ok
    evolPlantLeafNumTot(); // ok
    evolMobiliTillerDeath(); // bof bof
    evolMobiliLeafDeath(); // bof bof
    evalSupplyTot(); // bof bof
    evalDemandStructLeaf(); // bof bof
    evalDemandStructSheath(); // bof bof
    evalDemandStructRoot();
    evalDemandStructIN();
    evalDemandStructPanicle();
    evalDemandTotAndIcPreFlow();
    evolGrowthStructLeafPop();
    evolGrowthStructSheathPop();
    evolGrowthStructRootPop();
    evolGrowthStructINPop();
    evolGrowthStructPanPop();
    evolGrowthStructTot();
    priority2GrowthPanStrctPop();
    addResToGrowthStructPop();
    evolDemPanFilPopAndIcPFlow();
    evolPanicleFilPop();
    evolGrowthReserveInternode();
    evolGrowthTot();
    excessAssimilToRoot();
    evolDryMatTot();
    evalLai();
    evalMaximumLai();
    leafRolling();
    evalClumpAndLightInter();
    evalSlaMitch();
    evalRuiss_FloodDyna();
    automaticIrrigation();
    evolRempliResRFE_RDE();
    evolWaterLoggingUpland();
    evalStressWaterLogging();
    evolRempliMacropores();
    evolRurRFE_RDE();
    plantSubmergence();
    evalRootFront();
    evolPSPMVMD(t);
    evolSomDegresJour();
    evolSomDegresJourCor();
    evalRUE();
    // //mortality(); //pour l'instant
    // keyResults();
    // evalSimEndCycle();
}

void Model::init(const model::models::ModelParameters& parameters)
{
    Altitude = parameters.get < double >("Altitude");
    ASScstr = parameters.get < double >("ASScstr");
    AttenMitch = parameters.get < double >("AttenMitch");
    BundHeight = parameters.get < double >("BundHeight");
    Ca = parameters.get < double >("Ca");
    CO2Cp = parameters.get < double >("CO2Cp");
    CO2Exp = parameters.get < double >("CO2Exp");
    CoeffAssimSla = parameters.get < double >("CoeffAssimSla");
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
    std::string dSemis;
    utils::DateTime::format_date(parameters.get < std::string >("DateSemis"), dSemis);
    DateSemis = utils::DateTime::toJulianDayNumber(dSemis);
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
    KPar = parameters.get < double > ("Kpar");
    KRespInternode = parameters.get < double >("KRespInternode");
    KRespMaintLeaf = parameters.get < double >("KRespMaintLeaf");
    KRespMaintRoot = parameters.get < double >("KRespMaintRoot");
    KRespMaintSheath = parameters.get < double >("KRespMaintSheath");
    KRespPanicle = parameters.get < double >("KRespPanicle");
    KTempMaint = parameters.get < double >("KTempMaint");
    Latitude = parameters.get < double >("Latitude");
    LeafLengthMax = parameters.get < double >("LeafLengthMax");
    LifeSavingDrainage = parameters.get < double >("LifeSavingDrainage");
    Mulch = parameters.get < double >("Mulch");
    PanStructMassMax = parameters.get < double >("PanStructMassMax");
    PercolationMax = parameters.get < double >("PercolationMax");
    PEvap = parameters.get < double >("PEvap");
    PFactor = parameters.get < double >("PFactor");
    Phyllo = parameters.get < double >("Phyllo");
    PlantsPerHill = parameters.get < double >("PlantsPerHill");
    PlotDrainageDAF = parameters.get < double >("PlotDrainageDAF");
    PoidsSecGrain = parameters.get < double >("PoidsSecGrain");
    PourcRuiss = parameters.get < double >("PourcRuiss");
    PPCrit = parameters.get < double >("PPCrit");
    PPExp = parameters.get < double >("PPExp");
    PPSens = parameters.get < double >("PPSens");
    PriorityPan = parameters.get < double >("PriorityPan");
    ProfRacIni = parameters.get < double >("ProfRacIni");
    RankLongestLeaf = parameters.get < double >("RankLongestLeaf");
    RelMobiliInternodeMax = parameters.get < double >("RelMobiliInternodeMax");
    RelPhylloPhaseStemElong = parameters.get < double >("RelPhylloPhaseStemElong");
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
    TOpt1 = parameters.get < double >("TOpt1");
    TOpt2 = parameters.get < double >("TOpt2");
    TransplantingDepth = parameters.get < double >("TransplantingDepth");
    TxAssimBVP = parameters.get < double >("TxAssimBVP");
    TxAssimMatu1 = parameters.get < double >("TxAssimMatu1");
    TxAssimMatu2 = parameters.get < double >("TxAssimMatu2");
    TxConversion = parameters.get < double >("TxConversion");
    TxResGrain = parameters.get < double >("TxResGrain");
    TxRuSurfGermi = parameters.get < double >("TxRuSurfGermi");
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

    initCrop();
    initParcelle();
}

void Model::initCrop()
{
    NumPhase = 0;
    SommeDegresJourMaximale = SDJLevee + SDJBVP + SDJRPR +
        SDJMatu1 + SDJMatu2;
    SommeDegresJour = 0;
    SeuilTempPhaseSuivante = 0;
    Lai = 0;
    IcCumul = 0;
    FTSW = 1;
    cstr = 1;
    DurPhase1 = 0;
    DurPhase2 = 0;
    DurPhase3 = 0;
    DurPhase4 = 0;
    DurPhase5 = 0;
    DurPhase6 = 0;
    TempLai = 0;
    ApexHeightGain = 0;
    ChangeNurseryStatus = 0;
    ChangePhase = 0;
    ChangeSsPhase = 0;
    CstrPhase2 = 0;
    CstrPhase3 = 0;
    CstrPhase4 = 0;
    CstrPhase5 = 0;
    CstrPhase6 = 0;
    CumCstrPhase2 = 0;
    CumCstrPhase3 = 0;
    CumCstrPhase4 = 0;
    CumCstrPhase5 = 0;
    CumCstrPhase6 = 0;
    CumFtswPhase2 = 0;
    CumFtswPhase3 = 0;
    CumFtswPhase4 = 0;
    CumFtswPhase5 = 0;
    CumFtswPhase6 = 0;
    CumIcPhase2 = 0;
    CumIcPhase3 = 0;
    CumIcPhase4 = 0;
    CumIcPhase5 = 0;
    CumIcPhase6 = 0;
    DAF = 0;
    DemLeafAreaPlant = 0;
    DemPanicleFillPop = 0;
    DemStructInternodePlant = 0;
    DemStructInternodePop = 0;
    DemStructLeafPlant = 0;
    DemStructLeafPop = 0;
    DemStructPaniclePlant = 0;
    DemStructPaniclePop = 0;
    DemStructRootPlant = 0;
    DemStructRootPop = 0;
    DemStructSheathPop = 0;
    DemStructTotPop = 0;
    FloodWaterGain = 0;
    FtswPhase2 = 0;
    FtswPhase3 = 0;
    FtswPhase4 = 0;
    FtswPhase5 = 0;
    FtswPhase6 = 0;
    GainRootSystSoilSurfPop = 0;
    GainRootSystVolPop = 0;
    GrowthDryMatPop = 0;
    GrowthResInternodePop = 0;
    GrowthStructDeficit = 0;
    GrowthStructInternodePop = 0;
    GrowthStructLeafPop = 0;
    GrowthStructPaniclePop = 0;
    GrowthStructRootPop = 0;
    GrowthStructSheathPop = 0;
    GrowthStructTotPop = 0;
    HaunGain = 0;
    IcPhase2 = 0;
    IcPhase3 = 0;
    IcPhase4 = 0;
    IcPhase5 = 0;
    IcPhase6 = 0;
    IncreaseResInternodePop = 0;
    Kcl = 0;
    Kr = 0;
    MobiliLeafDeath = 0;
    NbDaysSinceGermination = 0;
    NurseryStatus = 0;
    PanicleFilDeficit = 0;
    PanicleFilPop = 0;
    PanicleSinkPop = 0;
    PanStructMass = 0;
    PlantLeafNumNew = 0;
    ResInternodeMobiliDay = 0;
    ResInternodeMobiliDayPot = 0;
    RootFrontOld = 0;
    RootSystSoilSurfPop = 0;
    RootSystSoilSurfPopOld = 0;
    RootSystVolPop = 0;
    RootSystVolPopOld = 0;
    SDJCorPhase4 = 0;
}

} } } // namespace model models samara
