/**
 * @file model/models/samara/Model.cpp
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

#include <model/models/samara/Model.hpp>
#include <utils/DateTime.hpp>

namespace samara {

void Model::init(double /* t */,
                 const model::models::ModelParameters& parameters)
{
    Altitude = parameters.get < double >("Altitude");

    // std::cout << "Altitude = " << Altitude << std::endl;

    ASScstr = parameters.get < double >("ASScstr");

    // std::cout << "ASScstr = " << ASScstr << std::endl;

    AttenMitch = parameters.get < double >("AttenMitch");

    // std::cout << "AttendMitch = " << AttenMitch << std::endl;

    BundHeight = parameters.get < double >("BundHeight");

    // std::cout << "BundHeight = " << BundHeight << std::endl;

    Ca = parameters.get < double >("Ca");

    // std::cout << "Ca = " << Ca << std::endl;

    CO2Cp = parameters.get < double >("CO2Cp");

    // std::cout << "CO2Cp = " << CO2Cp << std::endl;

    CO2Exp = parameters.get < double >("CO2Exp");

    // std::cout << "CO2Exp = " << CO2Exp << std::endl;

    CO2SlopeTr = parameters.get < double >("CO2SlopeTr");

    // std::cout << "CO2SlopeTr = " << CO2SlopeTr << std::endl;

    CoeffAssimSla = parameters.get < double >("CoeffAssimSla");

    // std::cout << "CoeffAssimSla = " << CoeffAssimSla << std::endl;

    CoefficientQ10 = parameters.get < double >("CoefficientQ10");

    // std::cout << "CoefficientQ10 = " << CoefficientQ10 << std::endl;

    CoeffInternodeMass = parameters.get < double >("CoeffInternodeMass");

    // std::cout << "CoeffInternodeMass = " << CoeffInternodeMass << std::endl;

    CoeffInternodeNum = parameters.get < double >("CoeffInternodeNum");

    // std::cout << "CoeffInternodeNum = " << CoeffInternodeNum << std::endl;

    CoeffLeafDeath = parameters.get < double >("CoeffLeafDeath");

    // std::cout << "CoeffLeafDeath = " << CoeffLeafDeath << std::endl;

    CoeffLeafWLRatio = parameters.get < double >("CoeffLeafWLRatio");

    // std::cout << "CoeffLeafWLRatio = " << CoeffLeafWLRatio << std::endl;

    CoeffPanicleMass = parameters.get < double >("CoeffPanicleMass");

    // std::cout << "CoeffPanicleMass = " << CoeffPanicleMass << std::endl;

    CoeffPanSinkPop = parameters.get < double >("CoeffPanSinkPop");

    // std::cout << "CoeffPanSinkPop = " << CoeffPanSinkPop << std::endl;

    CoeffResCapacityInternode =
        parameters.get < double >("CoeffResCapacityInternode");

    // std::cout << "CoeffResCapacityInternode = " << CoeffResCapacityInternode
              // << std::endl;

    CoeffReserveSink = parameters.get < double >("CoeffReserveSink");

    // std::cout << "CoeffReserveSink = " << CoeffReserveSink << std::endl;

    CoeffRootMassPerVolMax =
        parameters.get < double >("CoeffRootMassPerVolMax");

    // std::cout << "CoeffRootMassPerVolMax = " << CoeffRootMassPerVolMax
              // << std::endl;

    CoeffTillerDeath = parameters.get < double >("CoeffTillerDeath");

    // std::cout << "CoeffTillerDeath = " << CoeffTillerDeath << std::endl;

    CoeffTransplantingShock =
        parameters.get < double >("CoeffTransplantingShock");

    // std::cout << "CoeffTransplantingShock = " << CoeffTransplantingShock
              // << std::endl;

    std::string dSemis;
    utils::DateTime::format_date(parameters.get < std::string >("DateSemis"),
                                 dSemis);
    DateSemis = utils::DateTime::toJulianDayNumber(dSemis);

    // std::cout << "DateSemis = " << DateSemis << std::endl;

    DensityField = parameters.get < double >("DensityField");

    // std::cout << "DensityField = " << DensityField << std::endl;

    DensityNursery = parameters.get < double >("DensityNursery");

    // std::cout << "DensityNursery = " << DensityNursery << std::endl;

    DEVcstr = parameters.get < double >("DEVcstr");

    // std::cout << "DEVcstr = " << DEVcstr << std::endl;

    DurationNursery = parameters.get < double >("DurationNursery");

    // std::cout << "DurationNursery = " << DurationNursery << std::endl;

    EpaisseurProf = parameters.get < double >("EpaisseurProf");

    // std::cout << "EpaisseurProf = " << EpaisseurProf << std::endl;

    EpaisseurSurf = parameters.get < double >("EpaisseurSurf");

    // std::cout << "EpaisseurSurf = " << EpaisseurSurf << std::endl;

    ExcessAssimToRoot = parameters.get < double >("ExcessAssimToRoot");

    // std::cout << "ExcessAssimToRoot = " << ExcessAssimToRoot << std::endl;

    FTSWIrrig = parameters.get < double >("FTSWIrrig");

    // std::cout << "FTSWIrrig = " << FTSWIrrig << std::endl;

    HaunCritTillering = parameters.get < double >("HaunCritTillering");

    // std::cout << "HaunCritTillering = " << HaunCritTillering << std::endl;

    HumFC = parameters.get < double >("HumFC");

    // std::cout << "HumFC = " << HumFC << std::endl;

    HumPF = parameters.get < double >("HumPF");

    // std::cout << "HumPF = " << HumPF << std::endl;

    HumSat = parameters.get < double >("HumSat");

    // std::cout << "HumSat = " << HumSat << std::endl;

    IcTillering = parameters.get < double >("IcTillering");

    // std::cout << "IcTillering = " << IcTillering << std::endl;

    InternodeLengthMax = parameters.get < double >("InternodeLengthMax");

    // std::cout << "InternodeLengthMax = " << InternodeLengthMax << std::endl;

    IrrigAuto = parameters.get < int >("IrrigAuto");

    // std::cout << "IrrigAuto = " << IrrigAuto << std::endl;

    IrrigAutoResume = parameters.get < double >("IrrigAutoResume");

    // std::cout << "IrrigAutoResume = " << IrrigAutoResume << std::endl;

    IrrigAutoStop = parameters.get < double >("IrrigAutoStop");

    // std::cout << "IrrigAutoStop = " << IrrigAutoStop << std::endl;

    IrrigAutoTarget = parameters.get < double >("IrrigAutoTarget");

    // std::cout << "IrrigAutoTarget = " << IrrigAutoTarget << std::endl;

    KcMax = parameters.get < double >("KcMax");

    // std::cout << "KcMax = " << KcMax << std::endl;

    KCritSterCold1 = parameters.get < double >("KCritSterCold1");

    // std::cout << "KCritSterCold1 = " << KCritSterCold1 << std::endl;

    KCritSterCold2 = parameters.get < double >("KCritSterCold2");

    // std::cout << "KCritSterCold2 = " << KCritSterCold2 << std::endl;

    KCritSterFtsw1 = parameters.get < double >("KCritSterFtsw1");

    // std::cout << "KCritSterFtsw1 = " << KCritSterFtsw1 << std::endl;

    KCritSterFtsw2 = parameters.get < double >("KCritSterFtsw2");

    // std::cout << "KCritSterFtsw2 = " << KCritSterFtsw2 << std::endl;

    KCritSterHeat1 = parameters.get < double >("KCritSterHeat1");

    // std::cout << "KCritSterHeat1 = " << KCritSterHeat1 << std::endl;

    KCritSterHeat2 = parameters.get < double >("KCritSterHeat2");

    // std::cout << "KCritSterHeat2 = " << KCritSterHeat2 << std::endl;

    KCritStressCold1 = parameters.get < double >("KCritStressCold1");

    // std::cout << "KCritStressCold1 = " << KCritStressCold1 << std::endl;

    KCritStressCold2 = parameters.get < double >("KCritStressCold2");

    // std::cout << "KCritStressCold2 = " << KCritStressCold2 << std::endl;

    Kdf = parameters.get < double >("Kdf");

    // std::cout << "Kdf = " << Kdf << std::endl;

    KPar = parameters.get < double > ("Kpar");

    // std::cout << "KPar = " << KPar << std::endl;

    KRespInternode = parameters.get < double >("KRespInternode");

    // std::cout << "KRespInternode = " << KRespInternode << std::endl;

    KRespMaintLeaf = parameters.get < double >("KRespMaintLeaf");

    // std::cout << "KRespMaintLeaf = " << KRespMaintLeaf << std::endl;

    KRespMaintRoot = parameters.get < double >("KRespMaintRoot");

    // std::cout << "KRespMaintRoot = " << KRespMaintRoot << std::endl;

    KRespMaintSheath = parameters.get < double >("KRespMaintSheath");

    // std::cout << "KRespMaintSheath = " << KRespMaintSheath << std::endl;

    KRespPanicle = parameters.get < double >("KRespPanicle");

    // std::cout << "KRespPanicle = " << KRespPanicle << std::endl;

    KTempMaint = parameters.get < double >("KTempMaint");

    // std::cout << "KTempMaint = " << KTempMaint << std::endl;

    Latitude = parameters.get < double >("Latitude");

    // std::cout << "Latitude = " << Latitude << std::endl;

    LeafLengthMax = parameters.get < double >("LeafLengthMax");

    // std::cout << "LeafLengthMax = " << LeafLengthMax << std::endl;

    LifeSavingDrainage = parameters.get < double >("LifeSavingDrainage");

    // std::cout << "LifeSavingDrainage = " << LifeSavingDrainage << std::endl;

    Mulch = parameters.get < double >("Mulch");

    // std::cout << "Mulch = " << Mulch << std::endl;

    PanStructMassMax = parameters.get < double >("PanStructMassMax");

    // std::cout << "PanStructMassMax = " << PanStructMassMax << std::endl;

    PercolationMax = parameters.get < double >("PercolationMax");

    // std::cout << "PercolationMax = " << PercolationMax << std::endl;

    PEvap = parameters.get < double >("PEvap");

    // std::cout << "PEvap = " << PEvap << std::endl;

    PFactor = parameters.get < double >("PFactor");

    // std::cout << "PFactor = " << PFactor << std::endl;

    Phyllo = parameters.get < double >("Phyllo");

    // std::cout << "Phyllo = " << Phyllo << std::endl;

    PlantsPerHill = parameters.get < double >("PlantsPerHill");

    // std::cout << "PlantsPerHill = " << PlantsPerHill << std::endl;

    PlotDrainageDAF = parameters.get < double >("PlotDrainageDAF");

    // std::cout << "PlotDrainageDAF = " << PlotDrainageDAF << std::endl;

    PoidsSecGrain = parameters.get < double >("PoidsSecGrain");

    // std::cout << "PoidsSecGrain = " << PoidsSecGrain << std::endl;

    PourcRuiss = parameters.get < double >("PourcRuiss");

    // std::cout << "PourcRuiss = " << PourcRuiss << std::endl;

    PPCrit = parameters.get < double >("PPCrit");

    // std::cout << "PPCrit = " << PPCrit << std::endl;

    PPExp = parameters.get < double >("PPExp");

    // std::cout << "PPExp = " << PPExp << std::endl;

    PPSens = parameters.get < double >("PPSens");

    // std::cout << "PPSens = " << PPSens << std::endl;

    PriorityPan = parameters.get < double >("PriorityPan");

    // std::cout << "PriorityPan = " << PriorityPan << std::endl;

    ProfRacIni = parameters.get < double >("ProfRacIni");

    // std::cout << "ProfRacIni = " << ProfRacIni << std::endl;

    RankLongestLeaf = parameters.get < double >("RankLongestLeaf");

    // std::cout << "RankLongestLeaf = " << RankLongestLeaf << std::endl;

    RelMobiliInternodeMax = parameters.get < double >("RelMobiliInternodeMax");

    // std::cout << "RemMobiliInternodeMax = " << RelMobiliInternodeMax
              // << std::endl;

    RelPhylloPhaseStemElong = parameters.get < double >(
        "RelPhylloPhaseStemElong");

    // std::cout << "RelPhylloPhaseStemElong = " << RelPhylloPhaseStemElong
              // << std::endl;

    RollingBase = parameters.get < double >("RollingBase");

    // std::cout << "RollingBase = " << RollingBase << std::endl;

    RollingSens = parameters.get < double >("RollingSens");

    // std::cout << "RollingSens = " << RollingSens << std::endl;

    RootCstr = parameters.get < double >("RootCstr");

    // std::cout << "RootCstr = " << RootCstr << std::endl;

    RootFrontMax = parameters.get < double >("RootFrontMax");

    // std::cout << "RootFrontMax = " << RootFrontMax << std::endl;

    RootPartitMax = parameters.get < double >("RootPartitMax");

    // std::cout << "RootPartitMax = " << RootPartitMax << std::endl;

    SDJBVP = parameters.get < double >("SDJBVP");

    // std::cout << "SDJBVP = " << SDJBVP << std::endl;

    SDJLevee = parameters.get < double >("SDJLevee");

    // std::cout << "SDJLevee = " << SDJLevee << std::endl;

    SDJMatu1 = parameters.get < double >("SDJMatu1");

    // std::cout << "SDJMatu1 = " << SDJMatu1 << std::endl;

    SDJMatu2 = parameters.get < double >("SDJMatu2");

    // std::cout << "SDJMatu2 = " << SDJMatu2 << std::endl;

    SDJRPR = parameters.get < double >("SDJRPR");

    // std::cout << "SDJRPR = " << SDJRPR << std::endl;

    SeuilPP = parameters.get < double >("SeuilPP");

    // std::cout << "SeuilPP = " << SeuilPP << std::endl;

    SeuilRuiss = parameters.get < double >("SeuilRuiss");

    // std::cout << "SeuilRuiss = " << SeuilRuiss << std::endl;

    SlaMax = parameters.get < double >("SlaMax");

    // std::cout << "SlaMax = " << SlaMax << std::endl;

    SlaMin = parameters.get < double >("SlaMin");

    // std::cout << "SlaMin = " << SlaMin << std::endl;

    StockIniProf = parameters.get < double >("StockIniProf");

    // std::cout << "StockIniProf = " << StockIniProf << std::endl;

    StockIniSurf = parameters.get < double >("StockIniSurf");

    // std::cout << "StockIniSurf = " << StockIniSurf << std::endl;

    TBase = parameters.get < double >("TBase");

    // std::cout << "TBase = " << TBase << std::endl;

    TempSla = parameters.get < double >("TempSla");

    // std::cout << "TempSla = " << TempSla << std::endl;

    TilAbility = parameters.get < double >("TilAbility");

    // std::cout << "TilAbility = " << TilAbility << std::endl;

    TLet = parameters.get < double >("TLet");

    // std::cout << "TLet = " << TLet << std::endl;

    TOpt1 = parameters.get < double >("TOpt1");

    // std::cout << "TOpt1 = " << TOpt1 << std::endl;

    TOpt2 = parameters.get < double >("TOpt2");

    // std::cout << "TOpt2 = " << TOpt2 << std::endl;

    Transplanting = parameters.get < double >("Transplanting");

    // std::cout << "Transplanting = " << Transplanting << std::endl;

    TransplantingDepth = parameters.get < double >("TransplantingDepth");

    // std::cout << "TransplantingDepth = " << TransplantingDepth << std::endl;

    TxAssimBVP = parameters.get < double >("TxAssimBVP");

    // std::cout << "TxAssimBVP = " << TxAssimBVP << std::endl;

    TxAssimMatu1 = parameters.get < double >("TxAssimMatu1");

    // std::cout << "TxAssimMatu1 = " << TxAssimMatu1 << std::endl;

    TxAssimMatu2 = parameters.get < double >("TxAssimMatu2");

    // std::cout << "TxAssimMatu2 = " << TxAssimMatu2 << std::endl;

    TxConversion = parameters.get < double >("TxConversion");

    // std::cout << "TxConversion = " << TxConversion << std::endl;

    TxResGrain = parameters.get < double >("TxResGrain");

    // std::cout << "TxResGrain = " << TxResGrain << std::endl;

    TxRuSurfGermi = parameters.get < double >("TxRuSurfGermi");

    // std::cout << "TxRuSurfGermi = " << TxRuSurfGermi << std::endl;

    VRacBVP = parameters.get < double >("VRacBVP");

    // std::cout << "VRacBVP = " << VRacBVP << std::endl;

    VRacLevee = parameters.get < double >("VRacLevee");

    // std::cout << "VRacLevee = " << VRacLevee << std::endl;

    VRacMatu1 = parameters.get < double >("VRacMatu1");

    // std::cout << "VRacMatu1 = " << VRacMatu1 << std::endl;

    VRacMatu2 = parameters.get < double >("VRacMatu2");

    // std::cout << "VRacMatu2 = " << VRacMatu2 << std::endl;

    VRacPSP = parameters.get < double >("VRacPSP");

    // std::cout << "VRacPSP = " << VRacPSP << std::endl;

    VRacRPR = parameters.get < double >("VRacRPR");

    // std::cout << "VRacRPR = " << VRacRPR << std::endl;

    WaterLoggingSens = parameters.get < double >("WaterLoggingSens");

    // std::cout << "WaterLoggingSens = " << WaterLoggingSens << std::endl;

    WtRatioLeafSheath = parameters.get < double >("WtRatioLeafSheath");

    // std::cout << "WtRatioLeafSheath = " << WtRatioLeafSheath << std::endl;
}

void Model::initCrop()
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
