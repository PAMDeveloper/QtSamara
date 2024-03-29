#ifndef DELPHIRUNNER_H
#define DELPHIRUNNER_H
#include <ModelParameters.hpp>
#include <QDebug>
#include <QFile>
#include "delphi_defines.h"
#include <artis/utils/DateTime.hpp>
#include "BhyTypeFAO.h"
#include "Bileau.h"
#include "MeteoCalc.h"
#include "MilBilanCarbone.h"
#include "Risocas2cpp.h"
#include "Riz.h"
#include "sorghum.h"

#include "crop.h"
#include "variables.h"
#include "parcelle.h"
#include "meteo.h"
#include "parameters.h"

void run_samara_21(double start, double end, const samara::ModelParameters& parameters) {
    //Model Parameters
    init_parameters(parameters);

    //Corrections
    AttenMitch = 0.9955;
    CO2Slopetr = -0.0004;
    CO2Exp = 0.004;
    SlaMax = 0.005;
    SlaMin = 0.0018;
    PoidsSecGrain = 0.028;

    //Simu parameters
    std::string startDate = parameters.get<std::string>("datedebut");
    std::string endDate = parameters.get<std::string>("datefin");
    std::string semisDate = parameters.get<std::string>("datesemis");
    double DateDebutSimul = artis::utils::DateTime::toJulianDayNumber(startDate);
    double DateFinSimul = artis::utils::DateTime::toJulianDayNumber(endDate);
    double DateEnCours = DateDebutSimul;
    double DateSemis = artis::utils::DateTime::toJulianDayNumber(semisDate);
    double NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

    meteo::init_meteo(parameters);
    parcelle::init_parcelle();
    Meteo::DegToRad(Latitude, LatRad);

#ifdef WITH_TRACE
    QFile paramFile( "Params.csv" );
    paramFile.open(QIODevice::ReadWrite);
    QTextStream paramStream( &paramFile );
    paramStream << "DateDebutSimul" << "\t" << DateDebutSimul << " \n " << "DateFinSimul" << "\t" << DateFinSimul << " \n " << "DateEnCours" << "\t" << DateEnCours << " \n " << "DateSemis" << "\t" << DateSemis << " \n " << "Altitude" << "\t" << Altitude << " \n " << "ASScstr" << "\t" << ASScstr << " \n " << "AttenMitch" << "\t" << AttenMitch << " \n " << "BundHeight" << "\t" << BundHeight << " \n " << "Ca" << "\t" << Ca << " \n " << "CO2Cp" << "\t" << CO2Cp << " \n " << "CO2Exp" << "\t" << CO2Exp << " \n " << "CO2Slopetr" << "\t" << CO2Slopetr << " \n " << "CoeffAssimSla" << "\t" << CoeffAssimSla << " \n " << "CoefficientQ10" << "\t" << CoefficientQ10 << " \n " << "CoeffInternodeMass" << "\t" << CoeffInternodeMass << " \n " << "CoeffInternodeNum" << "\t" << CoeffInternodeNum << " \n " << "CoeffLeafDeath" << "\t" << CoeffLeafDeath << " \n " << "CoeffLeafWLRatio" << "\t" << CoeffLeafWLRatio << " \n " << "CoeffPanicleMass" << "\t" << CoeffPanicleMass << " \n " << "CoeffPanSinkPop" << "\t" << CoeffPanSinkPop << " \n " << "CoeffResCapacityInternode" << "\t" << CoeffResCapacityInternode << " \n " << "CoeffReserveSink" << "\t" << CoeffReserveSink << " \n " << "CoeffRootMassPerVolMax" << "\t" << CoeffRootMassPerVolMax << " \n " << "CoeffTillerDeath" << "\t" << CoeffTillerDeath << " \n " << "CoeffTransplantingShock" << "\t" << CoeffTransplantingShock << " \n " << "DensityField" << "\t" << DensityField << " \n " << "DensityNursery" << "\t" << DensityNursery << " \n " << "DEVcstr" << "\t" << DEVcstr << " \n " << "DurationNursery" << "\t" << DurationNursery << " \n " << "EpaisseurProf" << "\t" << EpaisseurProf << " \n " << "EpaisseurSurf" << "\t" << EpaisseurSurf << " \n " << "ExcessAssimToRoot" << "\t" << ExcessAssimToRoot << " \n " << "FTSWIrrig" << "\t" << FTSWIrrig << " \n " << "HaunCritTillering" << "\t" << HaunCritTillering << " \n " << "HumFC" << "\t" << HumFC << " \n " << "HumPF" << "\t" << HumPF << " \n " << "HumSat" << "\t" << HumSat << " \n " << "IcTillering" << "\t" << IcTillering << " \n " << "InternodeLengthMax" << "\t" << InternodeLengthMax << " \n " << "IrrigAuto" << "\t" << IrrigAuto << " \n " << "IrrigAutoResume" << "\t" << IrrigAutoResume << " \n " << "IrrigAutoStop" << "\t" << IrrigAutoStop << " \n " << "IrrigAutoTarget" << "\t" << IrrigAutoTarget << " \n " << "KcMax" << "\t" << KcMax << " \n " << "KCritSterCold1" << "\t" << KCritSterCold1 << " \n " << "KCritSterCold2" << "\t" << KCritSterCold2 << " \n " << "KCritSterFtsw1" << "\t" << KCritSterFtsw1 << " \n " << "KCritSterFtsw2" << "\t" << KCritSterFtsw2 << " \n " << "KCritSterHeat1" << "\t" << KCritSterHeat1 << " \n " << "KCritSterHeat2" << "\t" << KCritSterHeat2 << " \n " << "KCritStressCold1" << "\t" << KCritStressCold1 << " \n " << "KCritStressCold2" << "\t" << KCritStressCold2 << " \n " << "Kdf" << "\t" << Kdf << " \n " << "KPar" << "\t" << KPar << " \n " << "KRespInternode" << "\t" << KRespInternode << " \n " << "KRespMaintLeaf" << "\t" << KRespMaintLeaf << " \n " << "KRespMaintRoot" << "\t" << KRespMaintRoot << " \n " << "KRespMaintSheath" << "\t" << KRespMaintSheath << " \n " << "KRespPanicle" << "\t" << KRespPanicle << " \n " << "KTempMaint" << "\t" << KTempMaint << " \n " << "Latitude" << "\t" << Latitude << " \n " << "LeafLengthMax" << "\t" << LeafLengthMax << " \n " << "LifeSavingDrainage" << "\t" << LifeSavingDrainage << " \n " << "Mulch" << "\t" << Mulch << " \n " << "PanStructMassMax" << "\t" << PanStructMassMax << " \n " << "PercolationMax" << "\t" << PercolationMax << " \n " << "PEvap" << "\t" << PEvap << " \n " << "PFactor" << "\t" << PFactor << " \n " << "Phyllo" << "\t" << Phyllo << " \n " << "PlantsPerHill" << "\t" << PlantsPerHill << " \n " << "PlotDrainageDAF" << "\t" << PlotDrainageDAF << " \n " << "PoidsSecGrain" << "\t" << PoidsSecGrain << " \n " << "PourcRuiss" << "\t" << PourcRuiss << " \n " << "PPCrit" << "\t" << PPCrit << " \n " << "PPExp" << "\t" << PPExp << " \n " << "PPSens" << "\t" << PPSens << " \n " << "PriorityPan" << "\t" << PriorityPan << " \n " << "ProfRacIni" << "\t" << ProfRacIni << " \n " << "RankLongestLeaf" << "\t" << RankLongestLeaf << " \n " << "RelMobiliInternodeMax" << "\t" << RelMobiliInternodeMax << " \n " << "RelPhylloPhaseStemElong" << "\t" << RelPhylloPhaseStemElong << " \n " << "RollingBase" << "\t" << RollingBase << " \n " << "RollingSens" << "\t" << RollingSens << " \n " << "RootCstr" << "\t" << RootCstr << " \n " << "RootFrontMax" << "\t" << RootFrontMax << " \n " << "RootPartitMax" << "\t" << RootPartitMax << " \n " << "SDJBVP" << "\t" << SDJBVP << " \n " << "SDJLevee" << "\t" << SDJLevee << " \n " << "SDJMatu1" << "\t" << SDJMatu1 << " \n " << "SDJMatu2" << "\t" << SDJMatu2 << " \n " << "SDJRPR" << "\t" << SDJRPR << " \n " << "SeuilPP" << "\t" << SeuilPP << " \n " << "SeuilRuiss" << "\t" << SeuilRuiss << " \n " << "SlaMax" << "\t" << SlaMax << " \n " << "SlaMin" << "\t" << SlaMin << " \n " << "StockIniProf" << "\t" << StockIniProf << " \n " << "StockIniSurf" << "\t" << StockIniSurf << " \n " << "TBase" << "\t" << TBase << " \n " << "TempSLA" << "\t" << TempSLA << " \n " << "TilAbility" << "\t" << TilAbility << " \n " << "TLim" << "\t" << TLim << " \n " << "TOpt1" << "\t" << TOpt1 << " \n " << "TOpt2" << "\t" << TOpt2 << " \n " << "Transplanting" << "\t" << Transplanting << " \n " << "TransplantingDepth" << "\t" << TransplantingDepth << " \n " << "TxAssimBVP" << "\t" << TxAssimBVP << " \n " << "TxAssimMatu1" << "\t" << TxAssimMatu1 << " \n " << "TxAssimMatu2" << "\t" << TxAssimMatu2 << " \n " << "TxConversion" << "\t" << TxConversion << " \n " << "TxResGrain" << "\t" << TxResGrain << " \n " << "TxRuSurfGermi" << "\t" << TxRuSurfGermi << " \n " << "VRacBVP" << "\t" << VRacBVP << " \n " << "VRacLevee" << "\t" << VRacLevee << " \n " << "VRacMatu1" << "\t" << VRacMatu1 << " \n " << "VRacMatu2" << "\t" << VRacMatu2 << " \n " << "VRacPSP" << "\t" << VRacPSP << " \n " << "VRacRPR" << "\t" << VRacRPR << " \n " << "WaterLoggingSens" << "\t" << WaterLoggingSens << " \n " << "WtRatioLeafSheath" << "\t" << WtRatioLeafSheath << endl;
    paramStream.flush();
    paramFile.close();

    QFile file( "SimReport.csv" );
    file.open(QIODevice::ReadWrite);
    QTextStream stream( &file );
    stream << "Jour" << "\t" << "A_AssimSurplus" << "\t" << "A_DemStructLeaf" << "\t" << "A_DemStructTot" << "\t" << "A_GrowthStructLeaf" << "\t" << "A_GrowthStructTot" << "\t" << "A_IncreaseResInternodePop" << "\t" << "A_ResInternodeMobiliDay" << "\t" << "ApexHeight" << "\t" << "ApexHeightGain" << "\t" << "Assim" << "\t" << "AssimNotUsed" << "\t" << "AssimNotUsedCum" << "\t" << "AssimPot" << "\t" << "AssimSurplus" << "\t" << "CapaRDE" << "\t" << "CapaREvap" << "\t" << "CapaRFE" << "\t" << "ChangeNurseryStatus" << "\t" << "ChangePhase" << "\t" << "ChangeSsPhase" << "\t" << "CoeffCO2Assim" << "\t" << "CoeffCO2Tr" << "\t" << "CoeffStressLogging" << "\t" << "Conversion" << "\t" << "ConversionEff" << "\t" << "CounterNursery" << "\t" << "Cstr" << "\t" << "CstrAssim" << "\t" << "CstrCum" << "\t" << "CstrMean" << "\t" << "CstrPhase2" << "\t" << "CstrPhase3" << "\t" << "CstrPhase4" << "\t" << "CstrPhase5" << "\t" << "CstrPhase6" << "\t" << "CulmsPerHill" << "\t" << "CulmsPerHillFin" << "\t" << "CulmsPerHillMax" << "\t" << "CulmsPerPlant" << "\t" << "CulmsPerPlantFin" << "\t" << "CulmsPerPlantMax" << "\t" << "CulmsPop" << "\t" << "CumCarbonUsedPop" << "\t" << "CumCstrPhase2" << "\t" << "CumCstrPhase3" << "\t" << "CumCstrPhase4" << "\t" << "CumCstrPhase5" << "\t" << "CumCstrPhase6" << "\t" << "CumLr" << "\t" << "CumEt" << "\t" << "CumFTSWPhase2" << "\t" << "CumFTSWPhase3" << "\t" << "CumFTSWPhase4" << "\t" << "CumFTSWPhase5" << "\t" << "CumFTSWPhase6" << "\t" << "CumGrowthPop" << "\t" << "CumIcPhase2" << "\t" << "CumIcPhase3" << "\t" << "CumIcPhase4" << "\t" << "CumIcPhase5" << "\t" << "CumIcPhase6" << "\t" << "CumIrrig" << "\t" << "CumIrrigFin" << "\t" << "CumDr" << "\t" << "CumPAR" << "\t" << "CumSupplyTot" << "\t" << "CumTr" << "\t" << "CumWReceived" << "\t" << "CumWUse" << "\t" << "CumWUseFin" << "\t" << "DAF" << "\t" << "DayLength" << "\t" << "DeadLeafdrywtPop" << "\t" << "Decli" << "\t" << "DegresDuJour" << "\t" << "DegresDuJourCor" << "\t" << "DemLeafAreaPlant" << "\t" << "DemPanicleFillPop" << "\t" << "DemResInternodePop" << "\t" << "DemStructInternodePlant" << "\t" << "DemStructInternodePop" << "\t" << "DemStructLeafPlant" << "\t" << "DemStructLeafPop" << "\t" << "DemStructPaniclePlant" << "\t" << "DemStructPaniclePop" << "\t" << "DemStructRootPlant" << "\t" << "DemStructRootPop" << "\t" << "DemStructSheathPop" << "\t" << "DemStructTotPop" << "\t" << "Density" << "\t" << "Dr" << "\t" << "DryMatAboveGroundPop" << "\t" << "DryMatAboveGroundPopFin" << "\t" << "DryMatAboveGroundTotPop" << "\t" << "DryMatPanicleTotPop" << "\t" << "DryMatResInternodePop" << "\t" << "DryMatResInternodePopOld" << "\t" << "DryMatStemPop" << "\t" << "DryMatStructInternodePop" << "\t" << "DryMatStructLeafPop" << "\t" << "DryMatStructPaniclePop" << "\t" << "DryMatStructRootPop" << "\t" << "DryMatStructSheathPop" << "\t" << "DryMatStructTotPop" << "\t" << "DryMatTotPop" << "\t" << "DryMatTotPopFin" << "\t" << "DryMatVegeTotPop" << "\t" << "DurGermFlow" << "\t" << "DurGermMat" << "\t" << "DurPhase1" << "\t" << "DurPhase2" << "\t" << "DurPhase3" << "\t" << "DurPhase4" << "\t" << "DurPhase5" << "\t" << "DurPhase6" << "\t" << "ETM" << "\t" << "ETR" << "\t" << "Eto" << "\t" << "EauDispo" << "\t" << "Evap" << "\t" << "EvapPot" << "\t" << "FTSW" << "\t" << "FertSpikeNumPop" << "\t" << "FloodwaterDepth" << "\t" << "FloodwaterGain" << "\t" << "FractionPlantHeightSubmer" << "\t" << "FractionRootsLogged" << "\t" << "FtswMoy" << "\t" << "FtswPhase2" << "\t" << "FtswPhase3" << "\t" << "FtswPhase4" << "\t" << "FtswPhase5" << "\t" << "FtswPhase6" << "\t" << "GainRootSystSoilSurfPop" << "\t" << "GainRootSystVolPop" << "\t" << "GrainFillingStatus" << "\t" << "GrainFillingStatusFin" << "\t" << "GrainYieldPanicle" << "\t" << "GrainYieldPop" << "\t" << "GrainYieldPopFin" << "\t" << "GrowthDryMatPop_V2_1" << "\t" << "GrowthPop" << "\t" << "GrowthResInternodePop" << "\t" << "GrowthStructDeficit" << "\t" << "GrowthStructInternodePop" << "\t" << "GrowthStructLeafPop" << "\t" << "GrowthStructPaniclePop" << "\t" << "GrowthStructRootPop" << "\t" << "GrowthStructSheathPop" << "\t" << "GrowthStructTotPop" << "\t" << "HMoyCalc" << "\t" << "HarvestIndex" << "\t" << "Haungain" << "\t" << "HaunIndex" << "\t" << "Hum" << "\t" << "Ic" << "\t" << "IcCum" << "\t" << "IcMean" << "\t" << "IcPhase2" << "\t" << "IcPhase3" << "\t" << "IcPhase4" << "\t" << "IcPhase5" << "\t" << "IcPhase6" << "\t" << "IncreaseResInternodePop" << "\t" << "InternodeResStatus" << "\t" << "IrrigAutoDay" << "\t" << "IrrigTotDay" << "\t" << "Irrigation" << "\t" << "KRolling" << "\t" << "KcTot" << "\t" << "Kce" << "\t" << "KceReal" << "\t" << "Kcl" << "\t" << "Kcp" << "\t" << "Kr" << "\t" << "LIRkdf" << "\t" << "LIRkdfcl" << "\t" << "LTRkdf" << "\t" << "LTRkdfcl" << "\t" << "Lai" << "\t" << "LaiDead" << "\t" << "LaiFin" << "\t" << "LastLeafLength" << "\t" << "LastLeafLengthPot" << "\t" << "LatRad" << "\t" << "LeafDeathPop" << "\t" << "Lr" << "\t" << "MaxLai" << "\t" << "MobiliLeafDeath" << "\t" << "NbDaysSinceGermination" << "\t" << "NbJas" << "\t" << "NumPhase" << "\t" << "NumSsPhase" << "\t" << "NurseryStatus" << "\t" << "ParIntercepte" << "\t" << "PanStructMass" << "\t" << "PanicleFilDeficit" << "\t" << "PanicleFilPop" << "\t" << "PanicleNumPlant" << "\t" << "PanicleNumPop" << "\t" << "PanicleSinkPop" << "\t" << "Par" << "\t" << "PhaseStemElongation" << "\t" << "PlantHeight" << "\t" << "PlantLeafNumNew" << "\t" << "PlantLeafNumTot" << "\t" << "PlantWidth" << "\t" << "ProfRu" << "\t" << "RUE" << "\t" << "RayExtra" << "\t" << "RelPotLeafLength" << "\t" << "ResCapacityInternodePop" << "\t" << "ResInternodeMobiliDay" << "\t" << "ResInternodeMobiliDayPot" << "\t" << "ResUtil" << "\t" << "ReservePopFin" << "\t" << "RespMaintDebt" << "\t" << "RespMaintTot" << "\t" << "RgCalc" << "\t" << "RgMax" << "\t" << "RootFront" << "\t" << "RootFrontOld" << "\t" << "RootMassPerVol" << "\t" << "RootShootRatio" << "\t" << "RootSystSoilSurfPop" << "\t" << "RootSystSoilSurfPopOld" << "\t" << "RootSystVolPop" << "\t" << "RootSystVolPopOld" << "\t" << "RURac" << "\t" << "RuSurf" << "\t" << "SDJCorPhase4" << "\t" << "SeuilCstrMortality" << "\t" << "SeuilTemp" << "\t" << "SeuilTempSsPhase" << "\t" << "SimAnthesis50" << "\t" << "SimEmergence" << "\t" << "SimEndCycle" << "\t" << "SimPanIni" << "\t" << "SimStartGermin" << "\t" << "SimStartMatu2" << "\t" << "SimStartPSP" << "\t" << "Sla" << "\t" << "SlaMitch" << "\t" << "SlaNew" << "\t" << "SommeDegresJourMax" << "\t" << "SpikeNumPanicle" << "\t" << "SpikeNumPop" << "\t" << "StRuMax" << "\t" << "SterilityCold" << "\t" << "SterilityDrought" << "\t" << "SterilityHeat" << "\t" << "SterilityTot" << "\t" << "SterilityTotFin" << "\t" << "StockMacropores" << "\t" << "StockRac" << "\t" << "StockSurface" << "\t" << "StockTotal" << "\t" << "StressCold" << "\t" << "SumDDPhasePrec" << "\t" << "SumDegreDayCor" << "\t" << "SumDegresDay" << "\t" << "SumPP" << "\t" << "SunDistance" << "\t" << "SunPosi" << "\t" << "SupplyTot" << "\t" << "TMoyCalc" << "\t" << "TMoyPrec" << "\t" << "TempLai" << "\t" << "TillerDeathPop" << "\t" << "TMaxMoy" << "\t" << "TMinMoy" << "\t" << "Tr" << "\t" << "TrEff" << "\t" << "TrEffInst" << "\t" << "TrPot" << "\t" << "VDPCalc" << "\t" << "ValRDE" << "\t" << "ValRFE" << "\t" << "ValRSurf" << "\t" << "VitesseRacinaire" << "\t" << "VitesseRacinaireDay" << "\t" << "VolMacropores" << "\t" << "VolRelMacropores" << "\t" << "WueEt" << "\t" << "WueTot" << endl;
#endif

    bool crop = false;
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
        meteo::update_meteo(DateEnCours);

        NbJAS = DateEnCours - DateSemis;
        /****/

        //crop entity
        if(NbJAS == 0) {
            crop = true;
            if(crop)risocas::RS_Transplanting_V2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                                         CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                         DryMatStructPaniclePop, DryMatResInternodePop);
        }


        meteo::AVGTempHum();
        meteo::EvalPar(DateEnCours);
        Meteo::EToFao(ETP, Altitude, RgMax, RgCalc, TMin, TMax, HMin, HMax, HMoyCalc, TMoyCalc, Vt, ETo, TMoyPrec, VDPCalc);

        if(crop) Riz::EvolPhenoPSPStress(SumPP, PPSens, SumDegreDayCor, SDJLevee, SDJBVP, SDJRPR, SDJMatu1, SDJMatu2, StockSurface, TxRuSurfGermi, RuSurf,
                                    DateEnCours, DateSemis, StockTotal, NumPhase, SumDDPhasePrec, SeuilTemp, ChangePhase, SeuilTempSsPhase, ChangeSsPhase, NumSsPhase);

        /** PHASES **/
        risocas::RS_EvalSimAnthesis50(NumPhase, ChangePhase, NbJAS, SimAnthesis50);
        risocas::RS_EvalDateGermination(NumPhase, ChangePhase, NbDaysSinceGermination);
        risocas::RS_EvalColdStress(KCritStressCold1, KCritStressCold2, TMin, StressCold);
        risocas::RS_EvalSimEmergence(NumPhase, ChangePhase, NbJAS, SimEmergence);
        risocas::RS_EvalSimPanIni(NumPhase, ChangePhase, NbJAS, SimPanIni);
        risocas::RS_EvalSimStartGermin(NumPhase, ChangePhase, NbJAS, SimStartGermin);
        risocas::RS_EvalSimStartMatu2(NumPhase, ChangePhase, NbJAS, SimStartMatu2);
        risocas::RS_EvalSimStartPSP(NumPhase, ChangePhase, NbJAS, SimStartPSP);

        /** DD **/
        /*if (crop) */risocas::RS_EvalDegresJourVitMoy_V2(NumPhase, TMax, TMin, TBase, TOpt1, TOpt2, TLim, Cstr, DEVcstr, StressCold, DegresDuJour, DegresDuJourCor);
        risocas::RS_EvalSDJPhase4(NumPhase, DegresDuJourCor, SDJCorPhase4);
        risocas::RS_EvalDAF_V2(NumPhase, DAF);
        risocas::RS_Phyllochron(NumPhase, DegresDuJourCor, Phyllo, RelPhylloPhaseStemElong, PhaseStemElongation, HaunGain, HaunIndex);
        risocas::RS_EvolHauteur_SDJ_cstr_V2_1(PhaseStemElongation, CoeffInternodeNum, HaunGain, Cstr, InternodeLengthMax, RelPotLeafLength, LeafLengthMax,
                                              CulmsPerHill, IcMean, Kdf, Ic, WtRatioLeafSheath, StressCold, CstrMean, ApexHeightGain, ApexHeight, PlantHeight, PlantWidth);
        risocas::RS_EvolKcpKceBilhy(LTRkdfcl, KcMax, Mulch, Kcp, Kce, KcTot);
        risocas::RS_EvalEvapPot(ETo, Kce, EvapPot);
        risocas::RS_EvolEvapSurfRFE_RDE_V2_1(NumPhase, Kce, EvapPot, CapaREvap, CapaRDE, CapaRFE, RuRac, RuSurf, BundHeight, EpaisseurSurf, EpaisseurProf,
                                             RootFront, ResUtil, Evap, ValRSurf, ValRFE, ValRDE, StockRac, StockTotal, StockSurface, Kr, KceReal, FloodwaterDepth, StockMacropores);

        /** CROP **/
        if (crop) risocas::RS_EvalFTSW_V2(RuRac, StockTotal, StockMacropores, StRuMax, StockRac, FTSW);
        if (crop) risocas::RS_EvalCstrPFactorFAO_V2(PFactor, FTSW, ETo, KcTot, StockMacropores, CoeffStressLogging, Cstr);
        if (crop) Bileau::DemandePlante_V2_1(Kcp, ETo, Ca, CO2Slopetr, TrPot, CoeffCO2Tr);
        if (crop) Bileau::EvalTranspi(TrPot, Cstr, Tr);
        /*if (!cropDead)*/ BhyTypeFAO::EvalETRETM(Evap, Tr, TrPot, ETM, ETR);
        if (crop) risocas::RS_EvolConsRes_Flood_V2(NumPhase, RuRac, RuSurf, CapaREvap, Tr, Evap, CapaRDE, CapaRFE, EpaisseurSurf, EpaisseurProf, ResUtil, StockRac,
                                                   StockSurface, StockTotal, ValRFE, ValRDE, ValRSurf, FloodwaterDepth, StockMacropores);

        /*if (crop) */Riz::RS_EvalTMaxMoy(TMax, NumPhase, NumSsPhase, TmaxMoy);
        /*if (crop) */Riz::RS_EvalTMinMoy(TMin, NumPhase, NumSsPhase, TminMoy);

        Riz::RS_EvalFtswMoy(FTSW, NumPhase, NumSsPhase, FtswMoy);
        risocas::RS_EvalSterility(NumPhase, ChangePhase, KCritSterCold1, KCritSterCold2, KCritSterHeat1, KCritSterHeat2, KCritSterFtsw1, KCritSterFtsw2, TminMoy,
                                  TmaxMoy, FtswMoy, SterilityCold, SterilityHeat, SterilityDrought, SterilityTot);
        risocas::RS_EvalVitesseRacinaire(VRacLevee, VRacBVP, VRacRPR, VRacPSP, VRacMatu1, VRacMatu2, RootCstr, Cstr, NumPhase, DegresDuJourCor, VitesseRacinaire, VitesseRacinaireDay);

        if (crop) MilBilanCarbone::EvalConversion(NumPhase, TxConversion, TxAssimBVP, SumDegresDay, SumDDPhasePrec, TxAssimMatu1, TxAssimMatu2, SeuilTemp, Conversion);

        risocas::RS_EvalParIntercepte_V2_1(Par, Lai, Kdf, PARIntercepte, LIRkdfcl);
        if(crop)risocas::RS_EvalAssimPot_V2_1(PARIntercepte, Par, Conversion, TMax, TMin, TBase, TOpt1, DayLength, StressCold, CO2Exp, Ca, CO2Cp, SlaMin, Sla, CoeffAssimSla,
                                      AssimPot, CoeffCO2Assim);

        risocas::RS_EvalCstrAssim(Cstr, ASScstr, CstrAssim);
        if (crop) risocas::RS_EvalAssim(AssimPot, CstrAssim, Assim);

        risocas::RS_TransplantingShock_V2(CounterNursery, CoeffTransplantingShock, Assim);
        risocas::RS_EvalRespMaint(KRespMaintLeaf, KRespMaintSheath, KRespMaintRoot, KRespInternode, KRespPanicle, DryMatStructLeafPop, DryMatStructSheathPop,
                                  DryMatStructRootPop, DryMatStructInternodePop, DryMatStructPaniclePop, TMoyCalc, KTempMaint, CoefficientQ10, RespMaintTot);
        risocas::RS_EvalRelPotLeafLength(NumPhase, HaunIndex, RankLongestLeaf, RelPotLeafLength);
        risocas::RS_EvolPlantTilNumTot_V2(NumPhase, ChangePhase, PlantsPerHill, TilAbility, Density, Ic, IcTillering, Cstr, HaunIndex, HaunCritTillering, LTRkdfcl,
                                          CulmsPerHill, CulmsPerPlant, CulmsPop);
        risocas::RS_EvolPlantLeafNumTot(NumPhase, CulmsPerHill, HaunGain, PlantLeafNumNew, PlantLeafNumTot);
        risocas::RS_EvolMobiliTillerDeath_V2_1(NumPhase, SDJCorPhase4, SDJRPR, CoeffTillerDeath, Density, Ic, PlantsPerHill, TillerDeathPop, CulmsPop, CulmsPerPlant,
                                               CulmsPerHill, DryMatStructPaniclePop);
        risocas::RS_EvolMobiliLeafDeath_V2_1(NumPhase, Ic, CoeffLeafDeath, Sla, LeafDeathPop, DryMatStructLeafPop, MobiliLeafDeath, DeadLeafdrywtPop, LaiDead);
        risocas::RS_EvalSupplyTot_V2_1(NumPhase, PhaseStemElongation, Assim, MobiliLeafDeath, RespMaintTot, RespMaintDebt, AssimNotUsed, AssimNotUsedCum, AssimSurplus,
                                       SupplyTot, CumSupplyTot);
        risocas::RS_EvalDemandStructLeaf_V2_1(NumPhase, PlantLeafNumNew, SlaNew, SlaMax, RelPotLeafLength, Density, LeafLengthMax, CoeffLeafWLRatio, Cstr, StressCold,
                                              DemLeafAreaPlant, DemStructLeafPlant, DemStructLeafPop, A_DemStructLeaf);
        risocas::RS_EvalDemandStructSheath(NumPhase, DemStructLeafPop, WtRatioLeafSheath, SlaMin, SlaMax, Sla, StressCold, DemStructSheathPop);
        risocas::RS_EvalDemandStructRoot_V2(NumPhase, Density, CoeffRootMassPerVolMax, RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot, DemStructLeafPop,
                                            DemStructSheathPop, DryMatStructRootPop, RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop, GainRootSystSoilSurfPop,
                                            DemStructRootPop, RootSystSoilSurfPopOld, RootFrontOld, RootSystVolPopOld, DemStructRootPlant);
        risocas::RS_EvalDemandStructIN_V2_1(PhaseStemElongation, ApexHeightGain, CulmsPerHill, CoeffInternodeMass, Density, Ic, ResCapacityInternodePop, DryMatResInternodePop,
                                            CoeffReserveSink, NumPhase, DemStructInternodePlant, DemStructInternodePop, DemResInternodePop);
        risocas::RS_EvalDemandStructPanicle_V2(NumPhase, CoeffPanicleMass, CulmsPerHill, Ic, DryMatStructPaniclePop, Density, PanStructMassMax, StressCold, DemStructPaniclePlant,
                                               PanStructMass, DemStructPaniclePop);
        risocas::RS_EvalDemandTotAndIcPreFlow_V2_1(NumPhase, RespMaintTot, DemStructLeafPop, DemStructSheathPop, DemStructRootPop, DemStructInternodePop, DemStructPaniclePop,
                                                   SupplyTot, NbDaysSinceGermination, PlantHeight, Cstr, DemResInternodePop, DemStructTotPop, Ic, IcCum, IcMean, CstrCum, CstrMean,
                                                   A_DemStructTot);
        risocas::RS_EvolGrowthStructLeafPop_V2_1(NumPhase, Ic, SupplyTot, DemStructLeafPop, DemStructTotPop, GrowthStructLeafPop, A_GrowthStructLeaf);
        risocas::RS_EvolGrowthStructSheathPop(NumPhase, Ic, SupplyTot, DemStructSheathPop, DemStructTotPop, GrowthStructSheathPop);
        risocas::RS_EvolGrowthStructRootPop(NumPhase, Ic, SupplyTot, DemStructRootPop, DemStructTotPop, GrowthStructRootPop);
        risocas::RS_EvolGrowthStructINPop_V2_1(NumPhase, Ic, SupplyTot, DemStructInternodePop, DemStructTotPop, DemResInternodePop, GrowthStructInternodePop, GrowthResInternodePop);
        risocas::RS_EvolGrowthStructPanPop(NumPhase, Ic, SupplyTot, DemStructPaniclePop, DemStructTotPop, GrowthStructPaniclePop);
        risocas::RS_Priority2GrowthPanStrctPop_V2_1(PriorityPan, DemStructPaniclePop, NumPhase, GrowthStructTotPop, DemStructInternodePop, DemStructTotPop, DemStructLeafPop,
                                                    DemStructSheathPop, DemStructRootPop, DemResInternodePop, GrowthStructPaniclePop, GrowthStructInternodePop,
                                                    GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop, GrowthResInternodePop);
        if(crop) risocas::RS_EvolGrowthStructTot_V2_1(NumPhase, SupplyTot, GrowthResInternodePop, GrowthStructTotPop, AssimSurplus, GrowthStructLeafPop, GrowthStructSheathPop,
                                             GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop, A_GrowthStructLeaf, A_GrowthStructTot, A_AssimSurplus);
        if(crop) risocas::RS_AddResToGrowthStructPop_V2_1(NumPhase, Ic, PhaseStemElongation, DryMatResInternodePop, DemStructTotPop, DemStructLeafPop, DemStructSheathPop,
                                                 DemStructRootPop, DemStructInternodePop, DemStructPaniclePop, RelMobiliInternodeMax, GrowthResInternodePop,
                                                 ResInternodeMobiliDayPot, GrowthStructDeficit, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop,
                                                 GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop, ResInternodeMobiliDay, A_GrowthStructLeaf,
                                                 A_GrowthStructTot, A_ResInternodeMobiliDay);
        if(crop) risocas::RS_EvolDemPanFilPopAndIcPFlow_V2_1(NumPhase, DryMatStructPaniclePop, CoeffPanSinkPop, SterilityTot, DegresDuJourCor, SDJMatu1, SupplyTot, Assim,
                                                    RespMaintTot, StressCold, PanicleSinkPop, DemPanicleFillPop, AssimSurplus, Ic, A_AssimSurplus);
        if(crop)risocas::RS_EvolPanicleFilPop_V2_1(NumPhase, Ic, DryMatResInternodePop, DemPanicleFillPop, SupplyTot, RelMobiliInternodeMax, RespMaintTot, Assim,
                                           ResInternodeMobiliDayPot, AssimSurplus, PanicleFilDeficit, ResInternodeMobiliDay, PanicleFilPop, GrainYieldPop,
                                           A_AssimSurplus, A_ResInternodeMobiliDay);
        if(crop) risocas::RS_EvolGrowthReserveInternode_V2_1(NumPhase, PhaseStemElongation, DryMatStructInternodePop, DryMatStructSheathPop, CoeffResCapacityInternode,
                                                    AssimSurplus, ResInternodeMobiliDay, ResCapacityInternodePop, IncreaseResInternodePop, DryMatResInternodePop,
                                                    AssimNotUsed, AssimNotUsedCum, GrowthResInternodePop, DryMatResInternodePopOld, A_IncreaseResInternodePop);
        if(crop) risocas::RS_EvolGrowthTot_V2_1(NumPhase, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop,
                                       GrowthResInternodePop, PanicleFilPop, DryMatResInternodePop, DryMatResInternodePopOld, GrowthStructTotPop, GrowthDryMatPop,
                                       A_GrowthStructTot);
        if(crop)risocas::RS_ExcessAssimilToRoot_V2(NumPhase, ExcessAssimToRoot, DryMatStructRootPop, RootSystVolPop, CoeffRootMassPerVolMax, RootMassPerVol, GrowthStructRootPop,
                                           AssimNotUsed);
        if(crop) risocas::RS_EvolDryMatTot_V2_1(NumPhase, ChangePhase, PlantsPerHill, TxResGrain, PoidsSecGrain, Density, GrowthStructLeafPop, GrowthStructSheathPop,
                                       GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop, GrowthResInternodePop, GrainYieldPop,
                                       ResCapacityInternodePop, CulmsPerPlant, CoeffPanSinkPop, SterilityTot, DeadLeafdrywtPop, DryMatResInternodePopOld, PanicleFilPop,
                                       AssimNotUsedCum, MobiliLeafDeath, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                       DryMatStructPaniclePop, DryMatStructStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
                                       DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, InternodeResStatus, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
                                       SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, RootShootRatio, DryMatAboveGroundTotPop, CumGrowthPop,
                                       GrowthPop, CumCarbonUsedPop);

        risocas::RS_EvalLai_V2_1(NumPhase, ChangePhase, DryMatStructLeafPop, Sla, SlaMax, LeafLengthMax, RelPotLeafLength, GrowthStructTotPop, GrowthStructLeafPop,
                                 DemStructLeafPop, Lai, LastLeafLengthPot, LastLeafLength);
        risocas::RS_EvalMaximumLai(NumPhase, ChangePhase, Lai, TempLai, MaxLai);
        risocas::RS_LeafRolling_V2_1(NumPhase, RollingBase, RollingSens, FTSW, ETo, KRolling);

        if(crop)risocas::RS_EvalClumpAndLightInter_V2_1(NumPhase, KRolling, Density, PlantWidth, PlantHeight, Kdf, Lai, FractionPlantHeightSubmer, LIRkdf, LIRkdfcl, LTRkdf, LTRkdfcl);
        if(crop)risocas::RS_EvalSlaMitch(SlaMax, SlaMin, AttenMitch, SumDegresDay, SDJLevee, NumPhase, DegresDuJourCor, TOpt1, TBase, TempSLA, DryMatStructLeafPop,
                                 GrowthStructLeafPop, SlaMitch, SlaNew, Sla);

        risocas::RS_EvalRuiss_FloodDyna_V2(NumPhase, Pluie, SeuilRuiss, PourcRuiss, BundHeight, Irrigation, PlantHeight, LifeSavingDrainage, PlotDrainageDAF,
                                           VolMacropores, SeuilRuiss, PercolationMax, DAF, StockMacropores, FloodwaterDepth, EauDispo, Lr);

        if(crop)risocas::RS_AutomaticIrrigation_V2_1(NumPhase, IrrigAuto, IrrigAutoTarget, BundHeight, PlantHeight, Irrigation, PlotDrainageDAF, DAF, VolMacropores,
                                             VolRelMacropores, Pluie, FTSWIrrig, IrrigAutoStop, IrrigAutoResume, ChangeNurseryStatus, PercolationMax, NbJAS,
                                             RuSurf, ResUtil, RootFront, EpaisseurSurf, EpaisseurProf, ProfRacIni, FloodwaterDepth, IrrigAutoDay, IrrigTotDay,
                                             StockMacropores, EauDispo, RuRac, StockRac, FTSW, Lr);

        risocas::RS_EvolRempliResRFE_RDE_V2(NumPhase, RuSurf, EauDispo, RuRac, CapaRFE, CapaREvap, CapaRDE, StRuMax, PercolationMax, BundHeight, EpaisseurSurf,
                                            EpaisseurProf, VolMacropores, FloodwaterDepth, StockTotal, StockRac, Hum, StockSurface, Dr, ValRDE, ValRFE, ValRSurf,
                                            FloodwaterGain, StockMacropores);

        if (crop) risocas::RS_EvolWaterLoggingUpland_V2(PercolationMax, BundHeight, VolMacropores, Dr, Lr, StockMacropores);
        if (crop) risocas::RS_EvalStressWaterLogging_V2(StockMacropores, VolMacropores, RootFront, EpaisseurSurf, EpaisseurProf, WaterLoggingSens, FractionRootsLogged,
                                                        CoeffStressLogging);

        risocas::RS_EvolRempliMacropores_V2(NumPhase, EpaisseurSurf, EpaisseurProf, ResUtil, StockMacropores, RootFront, CapaRDE, CapaRFE, FloodwaterDepth,
                                            StockTotal, Hum, StockSurface, StockRac, ValRDE, ValRFE, ValRSurf);

        risocas::RS_EvolRurRFE_RDE_V2_1(VitesseRacinaire, Hum, ResUtil, StockSurface, RuSurf, ProfRacIni, EpaisseurSurf, EpaisseurProf, ValRDE, ValRFE, NumPhase,
                                        ChangePhase, FloodwaterDepth, StockMacropores, RootFrontMax, ChangeNurseryStatus, Transplanting, TransplantingDepth, RuRac,
                                        StockRac, StockTotal, FloodwaterGain, RootFront);
        risocas::RS_PlantSubmergence_V2(PlantHeight, FloodwaterDepth, FractionPlantHeightSubmer);

        /*if (crop) */risocas::RS_EvalRootFront(NumPhase, RuRac, ResUtil, RootFront);

        risocas::RS_EvolPSPMVMD(NumPhase, ChangePhase, SumDegreDayCor, DegresDuJourCor, SeuilPP, PPCrit, DayLength, PPExp, SumPP, SumDDPhasePrec, SeuilTemp);

        if (crop) MilBilanCarbone::EvolSomDegresJour(DegresDuJour, NumPhase, SumDegresDay);
        if (crop) risocas::RS_EvolSomDegresJourCor(DegresDuJourCor, NumPhase, SumDegreDayCor);

        risocas::RS_EvalRUE_V2_1(NumPhase, ChangePhase, PARIntercepte, DryMatTotPop, DeadLeafdrywtPop, DryMatStructRootPop, Tr, Evap, Dr, Lr, SupplyTot, AssimNotUsed,
                                 Irrigation, IrrigAutoDay, Pluie, Assim, AssimPot, Conversion, NbJAS, Transplanting, NurseryStatus, Density, DensityNursery,
                                 DryMatAboveGroundTotPop, /*DryMatAboveGroundPop,*/ RUE, CumPAR, CumTr, CumEt, CumWUse, CumWReceived, CumIrrig, CumDr, CumLr, TrEffInst,
                                 TrEff, WueEt, WueTot, ConversionEff);//, RUEgreen);

        Sorghum::SorghumMortality(Cstr, SeuilCstrMortality, NumPhase);
        risocas::RS_KeyResults_V2_1(NumPhase, CulmsPerPlant, CulmsPerHill, Cstr, FTSW, Ic, Lai, GrainYieldPop, DryMatAboveGroundPop, DryMatResInternodePop, DryMatTotPop,
                                    GrainFillingStatus, SterilityTot, CumIrrig, CumWUse, CulmsPerPlantMax, CulmsPerHillMax, DurPhase1, DurPhase2, DurPhase3, DurPhase4,
                                    DurPhase5, DurPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4, CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3,
                                    CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, IcPhase2, IcPhase3,
                                    IcPhase4, IcPhase5, IcPhase6, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6, CstrPhase2, CstrPhase3, CstrPhase4,
                                    CstrPhase5, CstrPhase6, DurGermFlow, DurGermMat, LaiFin, CulmsPerHillFin, CulmsPerPlantFin, GrainYieldPopFin, DryMatAboveGroundPopFin,
                                    ReservePopFin, DryMatTotPopFin, GrainFillingStatusFin, SterilityTotFin, CumIrrigFin, CumWUseFin);


        if ( NumPhase == 7 && ChangePhase == 1 ) {
            crop::kill_crop();
            variables::reset();
            crop = false;
        }
//                risocas::RS_EvalSimEndCycle_V2_1(NumPhase, ChangePhase, NbJAS, SimEndCycle);

#ifdef WITH_TRACE
        QString Jour = QString::fromStdString(artis::utils::DateTime::toJulianDayFmt(DateEnCours, artis::utils::DATE_FORMAT_YMD));
        stream << Jour << "\t" << A_AssimSurplus << "\t" << A_DemStructLeaf << "\t" << A_DemStructTot << "\t" << A_GrowthStructLeaf << "\t" << A_GrowthStructTot << "\t" << A_IncreaseResInternodePop << "\t" << A_ResInternodeMobiliDay << "\t" << ApexHeight << "\t" << ApexHeightGain << "\t" << Assim << "\t" << AssimNotUsed << "\t" << AssimNotUsedCum << "\t" << AssimPot << "\t" << AssimSurplus << "\t" << CapaRDE << "\t" << CapaREvap << "\t" << CapaRFE << "\t" << ChangeNurseryStatus << "\t" << ChangePhase << "\t" << ChangeSsPhase << "\t" << CoeffCO2Assim << "\t" << CoeffCO2Tr << "\t" << CoeffStressLogging << "\t" << Conversion << "\t" << ConversionEff << "\t" << CounterNursery << "\t" << Cstr << "\t" << CstrAssim << "\t" << CstrCum << "\t" << CstrMean << "\t" << CstrPhase2 << "\t" << CstrPhase3 << "\t" << CstrPhase4 << "\t" << CstrPhase5 << "\t" << CstrPhase6 << "\t" << CulmsPerHill << "\t" << CulmsPerHillFin << "\t" << CulmsPerHillMax << "\t" << CulmsPerPlant << "\t" << CulmsPerPlantFin << "\t" << CulmsPerPlantMax << "\t" << CulmsPop << "\t" << CumCarbonUsedPop << "\t" << CumCstrPhase2 << "\t" << CumCstrPhase3 << "\t" << CumCstrPhase4 << "\t" << CumCstrPhase5 << "\t" << CumCstrPhase6 << "\t" << CumLr << "\t" << CumEt << "\t" << CumFTSWPhase2 << "\t" << CumFTSWPhase3 << "\t" << CumFTSWPhase4 << "\t" << CumFTSWPhase5 << "\t" << CumFTSWPhase6 << "\t" << CumGrowthPop << "\t" << CumIcPhase2 << "\t" << CumIcPhase3 << "\t" << CumIcPhase4 << "\t" << CumIcPhase5 << "\t" << CumIcPhase6 << "\t" << CumIrrig << "\t" << CumIrrigFin << "\t" << CumDr << "\t" << CumPAR << "\t" << CumSupplyTot << "\t" << CumTr << "\t" << CumWReceived << "\t" << CumWUse << "\t" << CumWUseFin << "\t" << DAF << "\t" << DayLength << "\t" << DeadLeafdrywtPop << "\t" << Decli << "\t" << DegresDuJour << "\t" << DegresDuJourCor << "\t" << DemLeafAreaPlant << "\t" << DemPanicleFillPop << "\t" << DemResInternodePop << "\t" << DemStructInternodePlant << "\t" << DemStructInternodePop << "\t" << DemStructLeafPlant << "\t" << DemStructLeafPop << "\t" << DemStructPaniclePlant << "\t" << DemStructPaniclePop << "\t" << DemStructRootPlant << "\t" << DemStructRootPop << "\t" << DemStructSheathPop << "\t" << DemStructTotPop << "\t" << Density << "\t" << Dr << "\t" << DryMatAboveGroundPop << "\t" << DryMatAboveGroundPopFin << "\t" << DryMatAboveGroundTotPop << "\t" << DryMatPanicleTotPop << "\t" << DryMatResInternodePop << "\t" << DryMatResInternodePopOld << "\t" << DryMatStructStemPop << "\t" << DryMatStructInternodePop << "\t" << DryMatStructLeafPop << "\t" << DryMatStructPaniclePop << "\t" << DryMatStructRootPop << "\t" << DryMatStructSheathPop << "\t" << DryMatStructTotPop << "\t" << DryMatTotPop << "\t" << DryMatTotPopFin << "\t" << DryMatVegeTotPop << "\t" << DurGermFlow << "\t" << DurGermMat << "\t" << DurPhase1 << "\t" << DurPhase2 << "\t" << DurPhase3 << "\t" << DurPhase4 << "\t" << DurPhase5 << "\t" << DurPhase6 << "\t" << ETM << "\t" << ETR << "\t" << ETo << "\t" << EauDispo << "\t" << Evap << "\t" << EvapPot << "\t" << FTSW << "\t" << FertSpikeNumPop << "\t" << FloodwaterDepth << "\t" << FloodwaterGain << "\t" << FractionPlantHeightSubmer << "\t" << FractionRootsLogged << "\t" << FtswMoy << "\t" << FtswPhase2 << "\t" << FtswPhase3 << "\t" << FtswPhase4 << "\t" << FtswPhase5 << "\t" << FtswPhase6 << "\t" << GainRootSystSoilSurfPop << "\t" << GainRootSystVolPop << "\t" << GrainFillingStatus << "\t" << GrainFillingStatusFin << "\t" << GrainYieldPanicle << "\t" << GrainYieldPop << "\t" << GrainYieldPopFin << "\t" << GrowthDryMatPop << "\t" << GrowthPop << "\t" << GrowthResInternodePop << "\t" << GrowthStructDeficit << "\t" << GrowthStructInternodePop << "\t" << GrowthStructLeafPop << "\t" << GrowthStructPaniclePop << "\t" << GrowthStructRootPop << "\t" << GrowthStructSheathPop << "\t" << GrowthStructTotPop << "\t" << HMoyCalc << "\t" << HarvestIndex << "\t" << HaunGain << "\t" << HaunIndex << "\t" << Hum << "\t" << Ic << "\t" << IcCum << "\t" << IcMean << "\t" << IcPhase2 << "\t" << IcPhase3 << "\t" << IcPhase4 << "\t" << IcPhase5 << "\t" << IcPhase6 << "\t" << IncreaseResInternodePop << "\t" << InternodeResStatus << "\t" << IrrigAutoDay << "\t" << IrrigTotDay << "\t" << Irrigation << "\t" << KRolling << "\t" << KcTot << "\t" << Kce << "\t" << KceReal << "\t" << Kcl << "\t" << Kcp << "\t" << Kr << "\t" << LIRkdf << "\t" << LIRkdfcl << "\t" << LTRkdf << "\t" << LTRkdfcl << "\t" << Lai << "\t" << LaiDead << "\t" << LaiFin << "\t" << LastLeafLength << "\t" << LastLeafLengthPot << "\t" << LatRad << "\t" << LeafDeathPop << "\t" << Lr << "\t" << MaxLai << "\t" << MobiliLeafDeath << "\t" << NbDaysSinceGermination << "\t" << NbJAS << "\t" << NumPhase << "\t" << NumSsPhase << "\t" << NurseryStatus << "\t" << PARIntercepte << "\t" << PanStructMass << "\t" << PanicleFilDeficit << "\t" << PanicleFilPop << "\t" << PanicleNumPlant << "\t" << PanicleNumPop << "\t" << PanicleSinkPop << "\t" << Par << "\t" << PhaseStemElongation << "\t" << PlantHeight << "\t" << PlantLeafNumNew << "\t" << PlantLeafNumTot << "\t" << PlantWidth << "\t" << ProfRu << "\t" << RUE << "\t" << RayExtra << "\t" << RelPotLeafLength << "\t" << ResCapacityInternodePop << "\t" << ResInternodeMobiliDay << "\t" << ResInternodeMobiliDayPot << "\t" << ResUtil << "\t" << ReservePopFin << "\t" << RespMaintDebt << "\t" << RespMaintTot << "\t" << RgCalc << "\t" << RgMax << "\t" << RootFront << "\t" << RootFrontOld << "\t" << RootMassPerVol << "\t" << RootShootRatio << "\t" << RootSystSoilSurfPop << "\t" << RootSystSoilSurfPopOld << "\t" << RootSystVolPop << "\t" << RootSystVolPopOld << "\t" << RuRac << "\t" << RuSurf << "\t" << SDJCorPhase4 << "\t" << SeuilCstrMortality << "\t" << SeuilTemp << "\t" << SeuilTempSsPhase << "\t" << SimAnthesis50 << "\t" << SimEmergence << "\t" << SimEndCycle << "\t" << SimPanIni << "\t" << SimStartGermin << "\t" << SimStartMatu2 << "\t" << SimStartPSP << "\t" << Sla << "\t" << SlaMitch << "\t" << SlaNew << "\t" << SommeDegresJourMax << "\t" << SpikeNumPanicle << "\t" << SpikeNumPop << "\t" << StRuMax << "\t" << SterilityCold << "\t" << SterilityDrought << "\t" << SterilityHeat << "\t" << SterilityTot << "\t" << SterilityTotFin << "\t" << StockMacropores << "\t" << StockRac << "\t" << StockSurface << "\t" << StockTotal << "\t" << StressCold << "\t" << SumDDPhasePrec << "\t" << SumDegreDayCor << "\t" << SumDegresDay << "\t" << SumPP << "\t" << SunDistance << "\t" << SunPosi << "\t" << SupplyTot << "\t" << TMoyCalc << "\t" << TMoyPrec << "\t" << TempLai << "\t" << TillerDeathPop << "\t" << TmaxMoy << "\t" << TminMoy << "\t" << Tr << "\t" << TrEff << "\t" << TrEffInst << "\t" << TrPot << "\t" << VDPCalc << "\t" << ValRDE << "\t" << ValRFE << "\t" << ValRSurf << "\t" << VitesseRacinaire << "\t" << VitesseRacinaireDay << "\t" << VolMacropores << "\t" << VolRelMacropores << "\t" << WueEt << "\t" << WueTot << endl;
#endif
    }
    #ifdef WITH_TRACE
    stream.flush();
    file.close();
#endif
}


//void TSimule::Aller(int const &numSimule)
//// Gère le déroullement de la simulation :
//// On récupère d'abord le plus petit pas de temps de tous les modules
//// Ensuite tant qu'on a pas atteint la date de fin de simulation :
////  - on execute les modules
////  - on change la date de la prochaine execution
////  - on incremente la date en cours avec le plus petit pas de temps
////  - on fait passer les bookmarks sur l'enregistrement suivant

//{
//    try {
//        // recuperation de la date de fin de la simulation
//        datefin = GetVal("FinSimul");
//        //VN déplacé dans la boucle
//        //semis   := GetVal('DateSemisCalc');
//        anPrec = 0;

//        // boucle tant que la date courante thisDate n'a pas atteind la date de fin
//        while (thisDate <= datefin) {
//            //VN contraintes
//            Pars.SetVariable("Dateencours", thisDate);
//            Pars.SetVariable("DebutSimul", thisDate);
//            Pars.SetVariable("finsimul", datefin);
//            semis = GetVal("DateSemisCalc");

//            //VN saveSemis
//            //if  (thisDate = semis) then
//            if ((contextObjet.GetCurrentInstance("Crop") == 00) && (thisDate == semis)) {
//                // Création de crop si on est arrivé à la date de semis on place l'instance de Crop dans le context
//                //ShowMessage(DateToStr(GetVal('DateSemisCalc')));
//                contextObjet.SetCurrentInstance(TEntityInstance.create("Crop", thisDate, DbModule1.Variete.findfield("id").AsVariant));
//            }

//            // S'il existe une instance de type Crop <=> a partir du moment ou il y a eu le semis
//            if (!(contextObjet.GetCurrentInstance("Crop") == 00)) {
//                test = contextObjet.GetCurrentInstance("Crop").GetVal("NumPhase");
//                //dateEnStr := FormatDateTime('dd:mm',contextObjet.GetCurrentInstance('Crop').GetVal('DateMaturite'));

//                // Si le numero de la phase de l'instance de Crop est 7
//                // Destruction de l'entité Crop car elle est a Maturité on place l'instance de Crop dans le context
//                if (contextObjet.GetCurrentInstance("Crop").GetVal("NumPhase") == 7)
//                    contextObjet.FreeEntityInstance("Crop");
//            } // fin : test sur existence d'une instance de Crop

//            //Ajout VN simcons
//            contextObjet.RunInitInstances;
//            //Lance les modules d'initialisation si il faut le faire.

//            StepExe; // parcours du tableau et execution des modules selon leur pas de temps

//            thisDate = thisDate + minorStep; // passage au pas de temps suivant

//            //VN simcons (permet la manipulation de la fin de simul dans les modules)
//            //      datefin = GetVal("FinSimul");

//            if (thisDate != datefin) {
//                //if (thisDate - Semis>0) and (thisDate - Semis<10) then ShowMessage(DateToStr(thisDate)+' '+DateToStr(semis));
//                SetVal("DateEnCours", thisDate);
//                //ShowMessage(DateToStr(thisDate));
//                SetVal("NbJAS", thisDate - semis);
//            }
//            if (mainForm.butStopSimulation.Caption == "Arrêt en cours")
//                thisDate = datefin + 1;
//        } // fin du while thisDate <= dateFin do

//        //VN save: Dans le cas de semis calculé on remet à jour la variable nbjas dans resjour
//        // Finaliser le choix forcer ou calculer une date de semis....
//        // Modif CB 02/06 ne fiare MajNbjAs que si date semis calculee
//        if (contextObjet.GetCurrentInstance("Plot").GetVal("DateSemis") == NullValue)
//            // Modif CB 02/06
//            majNbJas(GetVal("DateSemisCalc"), GetVal("DebutSimul"), datefin - 1);

//    } catch (Exception const &e)
//    {error_message(e.message + "TSimule.Aller", UClasseSimule);}

//}



void run_2() {}
void run_22() {}
void run_23() {}

#endif // DELPHIRUNNER_H
