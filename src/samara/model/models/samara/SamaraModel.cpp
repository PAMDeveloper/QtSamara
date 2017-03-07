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
    
    Ca = parameters.get<double>("ca");

    //std::cout << "Ca = " << Ca << std::endl;

    CO2Cp = parameters.get<double>("co2cp");

    //std::cout << "CO2Cp = " << CO2Cp << std::endl;

    CO2Exp = parameters.get<double>("co2exp");

    //std::cout << "CO2Exp = " << CO2Exp << std::endl;

    CO2SlopeTr = parameters.get<double>("co2slopetr");

    //std::cout << "CO2SlopeTr = " << CO2SlopeTr << std::endl;

   CoeffAssimSla = parameters.get<double>("coeffassimsla");

    //std::cout << "CoeffAssimSla = " << CoeffAssimSla << std::endl;

    CoefficientQ10 = parameters.get<double>("coefficientq10");

    //std::cout << "CoefficientQ10 = " << CoefficientQ10 << std::endl;

    CoeffInternodeMass = parameters.get<double>("coeffinternodemass");

    //std::cout << "CoeffInternodeMass = " << CoeffInternodeMass << std::endl;

    CoeffInternodeNum = parameters.get<double>("coeffinternodenum");

    //std::cout << "CoeffInternodeNum = " << CoeffInternodeNum << std::endl;

    CoeffLeafDeath = parameters.get<double>("coeffleafdeath");

    //std::cout << "CoeffLeafDeath = " << CoeffLeafDeath << std::endl;

    CoeffLeafWLRatio = parameters.get<double>("coeffleafwlratio");

    //std::cout << "CoeffLeafWLRatio = " << CoeffLeafWLRatio << std::endl;

    CoeffPanicleMass = parameters.get<double>("coeffpaniclemass");

    //std::cout << "CoeffPanicleMass = " << CoeffPanicleMass << std::endl;

    CoeffPanSinkPop = parameters.get<double>("coeffpansinkpop");

    //std::cout << "CoeffPanSinkPop = " << CoeffPanSinkPop << std::endl;

    CoeffResCapacityInternode = parameters.get<double>("coeffrescapacityinternode");

    //std::cout << "CoeffResCapacityInternode = " << CoeffResCapacityInternode << std::endl;

    CoeffReserveSink = parameters.get<double>("coeffreservesink");

    //std::cout << "CoeffReserveSink = " << CoeffReserveSink << std::endl;

    CoeffRootMassPerVolMax = parameters.get<double>("coeffrootmasspervolmax");

    //std::cout << "CoeffRootMassPerVolMax = " << CoeffRootMassPerVolMax << std::endl;

    CoeffTillerDeath = parameters.get<double>("coefftillerdeath");

    //std::cout << "CoeffTillerDeath = " << CoeffTillerDeath << std::endl;

    CoeffTransplantingShock = parameters.get<double>("coefftransplantingshock");

    //std::cout << "CoeffTransplantingShock = " << CoeffTransplantingShock << std::endl;
    //std::string dateuu = parameters.getStringVal("datesemis");//
    DateSemis = utils::DateTime::toJulianDayNumber(parameters.get<std::string>("datesemis"));

    //std::cout << "DateSemis = " << DateSemis << std::endl;

    DensityField = parameters.get<double>("densityfield");

    //std::cout << "DensityField = " << DensityField << std::endl;

    DensityNursery = parameters.get<double>("densitynursery");

    //std::cout << "DensityNursery = " << DensityNursery << std::endl;

    DEVcstr = parameters.get<double>("devcstr");

    //std::cout << "DEVcstr = " << DEVcstr << std::endl;

    DurationNursery = parameters.get<double>("durationnursery");

    //std::cout << "DurationNursery = " << DurationNursery << std::endl;

    EpaisseurProf = parameters.get<double>("epaisseurprof");

    //std::cout << "EpaisseurProf = " << EpaisseurProf << std::endl;

    EpaisseurSurf = parameters.get<double>("epaisseursurf");

    //std::cout << "EpaisseurSurf = " << EpaisseurSurf << std::endl;

    ExcessAssimToRoot = parameters.get<double>("excessassimtoroot");

    //std::cout << "ExcessAssimToRoot = " << ExcessAssimToRoot << std::endl;

    FTSWIrrig = parameters.get<double>("ftswirrig");

    //std::cout << "FTSWIrrig = " << FTSWIrrig << std::endl;

    HaunCritTillering = parameters.get<double>("hauncrittillering");

    //std::cout << "HaunCritTillering = " << HaunCritTillering << std::endl;

    HumFC = parameters.get<double>("humfc");

    //std::cout << "HumFC = " << HumFC << std::endl;

    HumPF = parameters.get<double>("humpf");

    //std::cout << "HumPF = " << HumPF << std::endl;

    HumSat = parameters.get<double>("humsat");

    //std::cout << "HumSat = " << HumSat << std::endl;

    IcTillering = parameters.get<double>("ictillering");

    //std::cout << "IcTillering = " << IcTillering << std::endl;

    InternodeLengthMax = parameters.get<double>("internodelengthmax");

    //std::cout << "InternodeLengthMax = " << InternodeLengthMax << std::endl;

    IrrigAuto = parameters.get<int>("irrigauto");

    //std::cout << "IrrigAuto = " << IrrigAuto << std::endl;

    IrrigAutoResume = parameters.get<double>("irrigautoresume");

    //std::cout << "IrrigAutoResume = " << IrrigAutoResume << std::endl;

    IrrigAutoStop = parameters.get<double>("irrigautostop");

    //std::cout << "IrrigAutoStop = " << IrrigAutoStop << std::endl;

    IrrigAutoTarget = parameters.get<double>("irrigautotarget");

    //std::cout << "IrrigAutoTarget = " << IrrigAutoTarget << std::endl;

    KcMax = parameters.get<double>("kcmax");

    //std::cout << "KcMax = " << KcMax << std::endl;

    KCritSterCold1 = parameters.get<double>("kcritstercold1");

    //std::cout << "KCritSterCold1 = " << KCritSterCold1 << std::endl;

    KCritSterCold2 = parameters.get<double>("kcritstercold2");

    //std::cout << "KCritSterCold2 = " << KCritSterCold2 << std::endl;

    KCritSterFtsw1 = parameters.get<double>("kcritsterftsw1");

    //std::cout << "KCritSterFtsw1 = " << KCritSterFtsw1 << std::endl;

    KCritSterFtsw2 = parameters.get<double>("kcritsterftsw2");

    //std::cout << "KCritSterFtsw2 = " << KCritSterFtsw2 << std::endl;

    KCritSterHeat1 = parameters.get<double>("kcritsterheat1"
                                               );

    //std::cout << "KCritSterHeat1 = " << KCritSterHeat1 << std::endl;

    KCritSterHeat2 = parameters.get<double>("kcritsterheat2");

    //std::cout << "KCritSterHeat2 = " << KCritSterHeat2 << std::endl;

    KCritStressCold1 = parameters.get<double>("kcritstresscold1");

    //std::cout << "KCritStressCold1 = " << KCritStressCold1 << std::endl;

    KCritStressCold2 = parameters.get<double>("kcritstresscold2");

    //std::cout << "KCritStressCold2 = " << KCritStressCold2 << std::endl;

    Kdf = parameters.get<double>("kdf");

    //std::cout << "Kdf = " << Kdf << std::endl;

    KPar = parameters.get<double> ("kpar");

    //std::cout << "KPar = " << KPar << std::endl;

    KRespInternode = parameters.get<double>("krespinternode");

    //std::cout << "KRespInternode = " << KRespInternode << std::endl;

    KRespMaintLeaf = parameters.get<double>("krespmaintleaf");

    //std::cout << "KRespMaintLeaf = " << KRespMaintLeaf << std::endl;

    KRespMaintRoot = parameters.get<double>("krespmaintroot");

    //std::cout << "KRespMaintRoot = " << KRespMaintRoot << std::endl;

    KRespMaintSheath = parameters.get<double>("krespmaintsheath");

    //std::cout << "KRespMaintSheath = " << KRespMaintSheath << std::endl;

    KRespPanicle = parameters.get<double>("kresppanicle");

    //std::cout << "KRespPanicle = " << KRespPanicle << std::endl;

    KTempMaint = parameters.get<double>("ktempmaint");

    //std::cout << "KTempMaint = " << KTempMaint << std::endl;

    Latitude = parameters.get<double>("latitude");

    //std::cout << "Latitude = " << Latitude << std::endl;

    LeafLengthMax = parameters.get<double>("leaflengthmax");

    //std::cout << "LeafLengthMax = " << LeafLengthMax << std::endl;

    LifeSavingDrainage = parameters.get<double>("lifesavingdrainage");

    //std::cout << "LifeSavingDrainage = " << LifeSavingDrainage << std::endl;

    Mulch = parameters.get<double>("mulch");

    //std::cout << "Mulch = " << Mulch << std::endl;

    PanStructMassMax = parameters.get<double>("panstructmassmax");

    //std::cout << "PanStructMassMax = " << PanStructMassMax << std::endl;

    PercolationMax = parameters.get<double>("percolationmax");

    //std::cout << "PercolationMax = " << PercolationMax << std::endl;

    PEvap = parameters.get<double>("pevap");

    //std::cout << "PEvap = " << PEvap << std::endl;

    PFactor = parameters.get<double>("pfactor");

    //std::cout << "PFactor = " << PFactor << std::endl;

    Phyllo = parameters.get<double>("phyllo");

    //std::cout << "Phyllo = " << Phyllo << std::endl;

    PlantsPerHill = parameters.get<double>("plantsperhill");

    //std::cout << "PlantsPerHill = " << PlantsPerHill << std::endl;

    PlotDrainageDAF = parameters.get<double>("plotdrainagedaf");

    //std::cout << "PlotDrainageDAF = " << PlotDrainageDAF << std::endl;

    PoidsSecGrain = parameters.get<double>("poidssecgrain");

    //std::cout << "PoidsSecGrain = " << PoidsSecGrain << std::endl;

    PourcRuiss = parameters.get<double>("pourcruiss");

    //std::cout << "PourcRuiss = " << PourcRuiss << std::endl;

    PPCrit = parameters.get<double>("ppcrit");

    //std::cout << "PPCrit = " << PPCrit << std::endl;

    PPExp = parameters.get<double>("ppexp");

    //std::cout << "PPExp = " << PPExp << std::endl;

    PPSens = parameters.get<double>("ppsens");

    //std::cout << "PPSens = " << PPSens << std::endl;

    PriorityPan = parameters.get<double>("prioritypan");

    //std::cout << "PriorityPan = " << PriorityPan << std::endl;

    ProfRacIni = parameters.get<double>("profracini");

    //std::cout << "ProfRacIni = " << ProfRacIni << std::endl;

    RankLongestLeaf = parameters.get<double>("ranklongestleaf");

    //std::cout << "RankLongestLeaf = " << RankLongestLeaf << std::endl;

    RelMobiliInternodeMax = parameters.get<double>("relmobiliinternodemax");

    //std::cout << "RemMobiliInternodeMax = " << RelMobiliInternodeMax << std::endl;

    RelPhylloPhaseStemElong = parameters.get<double>("relphyllophasestemelong");

    //std::cout << "RelPhylloPhaseStemElong = " << RelPhylloPhaseStemElong << std::endl;

    RollingBase = parameters.get<double>("rollingbase");

    //std::cout << "RollingBase = " << RollingBase << std::endl;

    RollingSens = parameters.get<double>("rollingsens");

    //std::cout << "RollingSens = " << RollingSens << std::endl;

    RootCstr = parameters.get<double>("rootcstr");

    //std::cout << "RootCstr = " << RootCstr << std::endl;

    RootFrontMax = parameters.get<double>("rootfrontmax");

    //std::cout << "RootFrontMax = " << RootFrontMax << std::endl;

    RootPartitMax = parameters.get<double>("rootpartitmax");

    //std::cout << "RootPartitMax = " << RootPartitMax << std::endl;

    SDJBVP = parameters.get<double>("sdjbvp");

    //std::cout << "SDJBVP = " << SDJBVP << std::endl;

    SDJLevee = parameters.get<double>("sdjlevee");

    //std::cout << "SDJLevee = " << SDJLevee << std::endl;

    SDJMatu1 = parameters.get<double>("sdjmatu1");

    //std::cout << "SDJMatu1 = " << SDJMatu1 << std::endl;

    SDJMatu2 = parameters.get<double>("sdjmatu2");

    //std::cout << "SDJMatu2 = " << SDJMatu2 << std::endl;

    SDJRPR = parameters.get<double>("sdjrpr");

    //std::cout << "SDJRPR = " << SDJRPR << std::endl;

    SeuilPP = parameters.get<double>("seuilpp");

    //std::cout << "SeuilPP = " << SeuilPP << std::endl;

    SeuilRuiss = parameters.get<double>("seuilruiss");

    //std::cout << "SeuilRuiss = " << SeuilRuiss << std::endl;

    SlaMax = parameters.get<double>("slamax");

    //std::cout << "SlaMax = " << SlaMax << std::endl;

    SlaMin = parameters.get<double>("slamin");

    //std::cout << "SlaMin = " << SlaMin << std::endl;

    StockIniProf = parameters.get<double>("stockiniprof");

    //std::cout << "StockIniProf = " << StockIniProf << std::endl;

    StockIniSurf = parameters.get<double>("stockinisurf");

    //std::cout << "StockIniSurf = " << StockIniSurf << std::endl;

    TBase = parameters.get<double>("tbase");

    //std::cout << "TBase = " << TBase << std::endl;

    TempSla = parameters.get<double>("tempsla");

    //std::cout << "TempSla = " << TempSla << std::endl;

    TilAbility = parameters.get<double>("tilability");

    //std::cout << "TilAbility = " << TilAbility << std::endl;

    TLet = parameters.get<double>("tlim");

    //std::cout << "TLet = " << TLet << std::endl;

    TOpt1 = parameters.get<double>("topt1");

    //std::cout << "TOpt1 = " << TOpt1 << std::endl;

    TOpt2 = parameters.get<double>("topt2");

    //std::cout << "TOpt2 = " << TOpt2 << std::endl;

    Transplanting = parameters.get<double>("transplanting");

    //std::cout << "Transplanting = " << Transplanting << std::endl;

    TransplantingDepth = parameters.get<double>("transplantingdepth");

    //std::cout << "TransplantingDepth = " << TransplantingDepth << std::endl;

    TxAssimBVP = parameters.get<double>("txassimbvp");

    //std::cout << "TxAssimBVP = " << TxAssimBVP << std::endl;

    TxAssimMatu1 = parameters.get<double>("txassimmatu1");

    //std::cout << "TxAssimMatu1 = " << TxAssimMatu1 << std::endl;

    TxAssimMatu2 = parameters.get<double>("txassimmatu2");

    //std::cout << "TxAssimMatu2 = " << TxAssimMatu2 << std::endl;

    TxConversion = parameters.get<double>("txconversion");

    //std::cout << "TxConversion = " << TxConversion << std::endl;

    TxResGrain = parameters.get<double>("txresgrain");

    //std::cout << "TxResGrain = " << TxResGrain << std::endl;

    TxRuSurfGermi = parameters.get<double>("txrusurfgermi");

    //std::cout << "TxRuSurfGermi = " << TxRuSurfGermi << std::endl;

    VRacBVP = parameters.get<double>("vracbvp");

    //std::cout << "VRacBVP = " << VRacBVP << std::endl;

    VRacLevee = parameters.get<double>("vraclevee");

    //std::cout << "VRacLevee = " << VRacLevee << std::endl;

    VRacMatu1 = parameters.get<double>("vracmatu1");

    //std::cout << "VRacMatu1 = " << VRacMatu1 << std::endl;

    VRacMatu2 = parameters.get<double>("vracmatu2");

    //std::cout << "VRacMatu2 = " << VRacMatu2 << std::endl;

    VRacPSP = parameters.get<double>("vracpsp");

    //std::cout << "VRacPSP = " << VRacPSP << std::endl;

    VRacRPR = parameters.get<double>("vracrpr");

    //std::cout << "VRacRPR = " << VRacRPR << std::endl;

    WaterLoggingSens = parameters.get<double>("waterloggingsens");

    //std::cout << "WaterLoggingSens = " << WaterLoggingSens << std::endl;

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
