/**
 * @file samara/model/models/samara/SamaraModel.cpp
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

#include <samara/model/models/samara/SamaraModel.hpp>
#include <samara/utils/DateTime.hpp>

namespace samara {

void SamaraModel::init(double /* t */,
                 const model::models::ModelParameters& parameters)
{
    Altitude = parameters.get<double>("altitude");
    ASScstr = parameters.get<double>("asscstr");
    AttenMitch = parameters.get<double>("attenmitch");
    BundHeight = parameters.get<double>("bundheight");
    Ca = parameters.get<double>("ca");
    CO2Cp = parameters.get<double>("co2cp");
    CO2Exp = parameters.get<double>("co2exp");
    CO2SlopeTr = parameters.get<double>("co2slopetr");
    CoeffAssimSla = parameters.get<double>("coeffassimsla");
    CoefficientQ10 = parameters.get<double>("coefficientq10");
    CoeffInternodeMass = parameters.get<double>("coeffinternodemass");
    CoeffInternodeNum = parameters.get<double>("coeffinternodenum");
    CoeffLeafDeath = parameters.get<double>("coeffleafdeath");
    CoeffLeafWLRatio = parameters.get<double>("coeffleafwlratio");
    CoeffPanicleMass = parameters.get<double>("coeffpaniclemass");
    CoeffPanSinkPop = parameters.get<double>("coeffpansinkpop");
    CoeffResCapacityInternode = parameters.get<double>("coeffrescapacityinternode");
    CoeffReserveSink = parameters.get<double>("coeffreservesink");
    CoeffRootMassPerVolMax = parameters.get<double>("coeffrootmasspervolmax");
    CoeffTillerDeath = parameters.get<double>("coefftillerdeath");
    CoeffTransplantingShock = parameters.get<double>("coefftransplantingshock");
    DateSemis = utils::DateTime::toJulianDayNumber(parameters.get<std::string>("datesemis"));
    DensityField = parameters.get<double>("densityfield");
    DensityNursery = parameters.get<double>("densitynursery");
    DEVcstr = parameters.get<double>("devcstr");
    DurationNursery = parameters.get<double>("durationnursery");
    EpaisseurProf = parameters.get<double>("epaisseurprof");
    EpaisseurSurf = parameters.get<double>("epaisseursurf");
    ExcessAssimToRoot = parameters.get<double>("excessassimtoroot");
    FTSWIrrig = parameters.get<double>("ftswirrig");
    HaunCritTillering = parameters.get<double>("hauncrittillering");
    HumFC = parameters.get<double>("humfc");
    HumPF = parameters.get<double>("humpf");
    HumSat = parameters.get<double>("humsat");
    IcTillering = parameters.get<double>("ictillering");
    InternodeLengthMax = parameters.get<double>("internodelengthmax");
    IrrigAuto = parameters.get<int>("irrigauto");
    IrrigAutoResume = parameters.get<double>("irrigautoresume");
    IrrigAutoStop = parameters.get<double>("irrigautostop");
    IrrigAutoTarget = parameters.get<double>("irrigautotarget");
    KcMax = parameters.get<double>("kcmax");
    KCritSterCold1 = parameters.get<double>("kcritstercold1");
    KCritSterCold2 = parameters.get<double>("kcritstercold2");
    KCritSterFtsw1 = parameters.get<double>("kcritsterftsw1");
    KCritSterFtsw2 = parameters.get<double>("kcritsterftsw2");
    KCritSterHeat1 = parameters.get<double>("kcritsterheat1");
    KCritSterHeat2 = parameters.get<double>("kcritsterheat2");
    KCritStressCold1 = parameters.get<double>("kcritstresscold1");
    KCritStressCold2 = parameters.get<double>("kcritstresscold2");
    Kdf = parameters.get<double>("kdf");
    KPar = parameters.get<double> ("kpar");
    KRespInternode = parameters.get<double>("krespinternode");
    KRespMaintLeaf = parameters.get<double>("krespmaintleaf");
    KRespMaintRoot = parameters.get<double>("krespmaintroot");
    KRespMaintSheath = parameters.get<double>("krespmaintsheath");
    KRespPanicle = parameters.get<double>("kresppanicle");
    KTempMaint = parameters.get<double>("ktempmaint");
    Latitude = parameters.get<double>("latitude");
    LeafLengthMax = parameters.get<double>("leaflengthmax");
    LifeSavingDrainage = parameters.get<double>("lifesavingdrainage");
    Mulch = parameters.get<double>("mulch");
    PanStructMassMax = parameters.get<double>("panstructmassmax");
    PercolationMax = parameters.get<double>("percolationmax");
    PEvap = parameters.get<double>("pevap");
    PFactor = parameters.get<double>("pfactor");
    Phyllo = parameters.get<double>("phyllo");
    PlantsPerHill = parameters.get<double>("plantsperhill");
    PlotDrainageDAF = parameters.get<double>("plotdrainagedaf");
    PoidsSecGrain = parameters.get<double>("poidssecgrain");
    PourcRuiss = parameters.get<double>("pourcruiss");
    PPCrit = parameters.get<double>("ppcrit");
    PPExp = parameters.get<double>("ppexp");
    PPSens = parameters.get<double>("ppsens");
    PriorityPan = parameters.get<double>("prioritypan");
    ProfRacIni = parameters.get<double>("profracini");
    RankLongestLeaf = parameters.get<double>("ranklongestleaf");
    RelMobiliInternodeMax = parameters.get<double>("relmobiliinternodemax");
    RelPhylloPhaseStemElong = parameters.get<double>("relphyllophasestemelong");
    RollingBase = parameters.get<double>("rollingbase");
    RollingSens = parameters.get<double>("rollingsens");
    RootCstr = parameters.get<double>("rootcstr");
    RootFrontMax = parameters.get<double>("rootfrontmax");
    RootPartitMax = parameters.get<double>("rootpartitmax");
    SDJBVP = parameters.get<double>("sdjbvp");
    SDJLevee = parameters.get<double>("sdjlevee");
    SDJMatu1 = parameters.get<double>("sdjmatu1");
    SDJMatu2 = parameters.get<double>("sdjmatu2");
    SDJRPR = parameters.get<double>("sdjrpr");
    SeuilPP = parameters.get<double>("seuilpp");
    SeuilRuiss = parameters.get<double>("seuilruiss");
    SlaMax = parameters.get<double>("slamax");
    SlaMin = parameters.get<double>("slamin");
    StockIniProf = parameters.get<double>("stockiniprof");
    StockIniSurf = parameters.get<double>("stockinisurf");
    TBase = parameters.get<double>("tbase");
    TempSla = parameters.get<double>("tempsla");
    TilAbility = parameters.get<double>("tilability");
    TLet = parameters.get<double>("tlim");
    TOpt1 = parameters.get<double>("topt1");
    TOpt2 = parameters.get<double>("topt2");
    Transplanting = parameters.get<double>("transplanting");
    TransplantingDepth = parameters.get<double>("transplantingdepth");
    TxAssimBVP = parameters.get<double>("txassimbvp");
    TxAssimMatu1 = parameters.get<double>("txassimmatu1");
    TxAssimMatu2 = parameters.get<double>("txassimmatu2");
    TxConversion = parameters.get<double>("txconversion");
    TxResGrain = parameters.get<double>("txresgrain");
    TxRuSurfGermi = parameters.get<double>("txrusurfgermi");
    VRacBVP = parameters.get<double>("vracbvp");
    VRacLevee = parameters.get<double>("vraclevee");
    VRacMatu1 = parameters.get<double>("vracmatu1");
    VRacMatu2 = parameters.get<double>("vracmatu2");
    VRacPSP = parameters.get<double>("vracpsp");
    VRacRPR = parameters.get<double>("vracrpr");
    WaterLoggingSens = parameters.get<double>("waterloggingsens");
    WtRatioLeafSheath = parameters.get<double>("wtratioleafsheath");
}

void SamaraModel::initCrop()
{
    NumPhase = 0;
    SommeDegresJourMaximale = SDJLevee + SDJBVP + SDJRPR + SDJMatu1 + SDJMatu2;
    SommeDegresJour = 0;
    SeuilTempPhaseSuivante = 0;
    Lai = 0;
    IcCumul = 0;
    FTSW = 1;
    Cstr = 1;
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

} // namespace samara
