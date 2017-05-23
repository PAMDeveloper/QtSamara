#include "samara.h"
#include "variables.h"

#include <QDebug>
#include <QFile>

#include <artis/utils/DateTime.hpp>

#include "BhyTypeFAO.h"
#include "Bileau.h"
#include "MeteoCalc.h"
#include "MilBilanCarbone.h"
#include "Risocas2cpp.h"
#include "Riz.h"
#include "sorghum.h"


void run_samara_2_1(const samara::ModelParameters& parameters) {
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
    if(nb_step > 0)
        DateFinSimul = DateDebutSimul + nb_step;
    double DateEnCours = DateDebutSimul;
    double DateSemis = artis::utils::DateTime::toJulianDayNumber(semisDate);
    double NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

    init_parcelle();
    LatRad = Latitude * M_PI * 1.0 / 180;

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
        std::string currentDate = artis::utils::DateTime::toJulianDayFmt(DateEnCours, artis::utils::DateFormat::DATE_FORMAT_YMD);
        TMax = parameters.get(DateEnCours).TMax;
        set_meteo_vars(parameters, DateEnCours);
        NbJAS = DateEnCours - DateSemis;
        if(NbJAS == 57) {
            qDebug() << NbJAS;
        }
        //crop entity
        if(NbJAS == 0) {
            init_culture();
            risocas::RS_Transplanting_V2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                                                 CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                                 DryMatStructPaniclePop, DryMatResInternodePop);
            crop = true;
        }
        eval_Par(DateEnCours);
        EToFao();

        if(crop) Riz::EvolPhenoPSPStress(SumPP, PPSens, SumDegreDayCor, SDJLevee, SDJBVP, SDJRPR, SDJMatu1, SDJMatu2, StockSurface, TxRuSurfGermi, RuSurf,
                                    DateEnCours, DateSemis, StockTotal, NumPhase, SumDDPhasePrec, SeuilTemp, ChangePhase, SeuilTempSsPhase, ChangeSsPhase, NumSsPhase);

        /** Phyllo **/
        risocas::RS_EvalSimAnthesis50(NumPhase, ChangePhase, NbJAS, SimAnthesis50);
        risocas::RS_EvalDateGermination(NumPhase, ChangePhase, NbDaysSinceGermination);
        risocas::RS_EvalColdStress(KCritStressCold1, KCritStressCold2, TMin, StressCold);
        risocas::RS_EvalSimEmergence(NumPhase, ChangePhase, NbJAS, SimEmergence);
        risocas::RS_EvalSimPanIni(NumPhase, ChangePhase, NbJAS, SimPanIni);
        risocas::RS_EvalSimStartGermin(NumPhase, ChangePhase, NbJAS, SimStartGermin);
        risocas::RS_EvalSimStartMatu2(NumPhase, ChangePhase, NbJAS, SimStartMatu2);
        risocas::RS_EvalSimStartPSP(NumPhase, ChangePhase, NbJAS, SimStartPSP);
        /*if (crop) */risocas::RS_EvalDegresJourVitMoy_V2(NumPhase, TMax, TMin, TBase, TOpt1, TOpt2, TLim, Cstr, DEVcstr, StressCold, DegresDuJour, DegresDuJourCor);
        risocas::RS_EvalSDJPhase4(NumPhase, DegresDuJourCor, SDJCorPhase4);
        risocas::RS_EvalDAF_V2(NumPhase, DAF);
        risocas::RS_Phyllochron(NumPhase, DegresDuJourCor, Phyllo, RelPhylloPhaseStemElong, PhaseStemElongation, HaunGain, HaunIndex);


        /** DD **/
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
        BhyTypeFAO::EvalETRETM(Evap, Tr, TrPot, ETM, ETR);
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
            kill_crop();
            reset_variables();
            crop = false;
        }
//                risocas::RS_EvalSimEndCycle_V2_1(NumPhase, ChangePhase, NbJAS, SimEndCycle);

#ifdef WITH_TRACE
        QString Jour = QString::fromStdString(artis::utils::DateTime::toJulianDayFmt(DateEnCours, artis::utils::DATE_FORMAT_YMD));
        stream << fixed << Jour << "\t" << A_AssimSurplus << "\t" << A_DemStructLeaf << "\t" << A_DemStructTot << "\t" << A_GrowthStructLeaf << "\t" << A_GrowthStructTot << "\t" << A_IncreaseResInternodePop << "\t" << A_ResInternodeMobiliDay << "\t" << ApexHeight << "\t" << ApexHeightGain << "\t" << Assim << "\t" << AssimNotUsed << "\t" << AssimNotUsedCum << "\t" << AssimPot << "\t" << AssimSurplus << "\t" << CapaRDE << "\t" << CapaREvap << "\t" << CapaRFE << "\t" << ChangeNurseryStatus << "\t" << ChangePhase << "\t" << ChangeSsPhase << "\t" << CoeffCO2Assim << "\t" << CoeffCO2Tr << "\t" << CoeffStressLogging << "\t" << Conversion << "\t" << ConversionEff << "\t" << CounterNursery << "\t" << Cstr << "\t" << CstrAssim << "\t" << CstrCum << "\t" << CstrMean << "\t" << CstrPhase2 << "\t" << CstrPhase3 << "\t" << CstrPhase4 << "\t" << CstrPhase5 << "\t" << CstrPhase6 << "\t" << CulmsPerHill << "\t" << CulmsPerHillFin << "\t" << CulmsPerHillMax << "\t" << CulmsPerPlant << "\t" << CulmsPerPlantFin << "\t" << CulmsPerPlantMax << "\t" << CulmsPop << "\t" << CumCarbonUsedPop << "\t" << CumCstrPhase2 << "\t" << CumCstrPhase3 << "\t" << CumCstrPhase4 << "\t" << CumCstrPhase5 << "\t" << CumCstrPhase6 << "\t" << CumLr << "\t" << CumEt << "\t" << CumFTSWPhase2 << "\t" << CumFTSWPhase3 << "\t" << CumFTSWPhase4 << "\t" << CumFTSWPhase5 << "\t" << CumFTSWPhase6 << "\t" << CumGrowthPop << "\t" << CumIcPhase2 << "\t" << CumIcPhase3 << "\t" << CumIcPhase4 << "\t" << CumIcPhase5 << "\t" << CumIcPhase6 << "\t" << CumIrrig << "\t" << CumIrrigFin << "\t" << CumDr << "\t" << CumPAR << "\t" << CumSupplyTot << "\t" << CumTr << "\t" << CumWReceived << "\t" << CumWUse << "\t" << CumWUseFin << "\t" << DAF << "\t" << DayLength << "\t" << DeadLeafdrywtPop << "\t" << Decli << "\t" << DegresDuJour << "\t" << DegresDuJourCor << "\t" << DemLeafAreaPlant << "\t" << DemPanicleFillPop << "\t" << DemResInternodePop << "\t" << DemStructInternodePlant << "\t" << DemStructInternodePop << "\t" << DemStructLeafPlant << "\t" << DemStructLeafPop << "\t" << DemStructPaniclePlant << "\t" << DemStructPaniclePop << "\t" << DemStructRootPlant << "\t" << DemStructRootPop << "\t" << DemStructSheathPop << "\t" << DemStructTotPop << "\t" << Density << "\t" << Dr << "\t" << DryMatAboveGroundPop << "\t" << DryMatAboveGroundPopFin << "\t" << DryMatAboveGroundTotPop << "\t" << DryMatPanicleTotPop << "\t" << DryMatResInternodePop << "\t" << DryMatResInternodePopOld << "\t" << DryMatStructStemPop << "\t" << DryMatStructInternodePop << "\t" << DryMatStructLeafPop << "\t" << DryMatStructPaniclePop << "\t" << DryMatStructRootPop << "\t" << DryMatStructSheathPop << "\t" << DryMatStructTotPop << "\t" << DryMatTotPop << "\t" << DryMatTotPopFin << "\t" << DryMatVegeTotPop << "\t" << DurGermFlow << "\t" << DurGermMat << "\t" << DurPhase1 << "\t" << DurPhase2 << "\t" << DurPhase3 << "\t" << DurPhase4 << "\t" << DurPhase5 << "\t" << DurPhase6 << "\t" << ETM << "\t" << ETR << "\t" << ETo << "\t" << EauDispo << "\t" << Evap << "\t" << EvapPot << "\t" << FTSW << "\t" << FertSpikeNumPop << "\t" << FloodwaterDepth << "\t" << FloodwaterGain << "\t" << FractionPlantHeightSubmer << "\t" << FractionRootsLogged << "\t" << FtswMoy << "\t" << FtswPhase2 << "\t" << FtswPhase3 << "\t" << FtswPhase4 << "\t" << FtswPhase5 << "\t" << FtswPhase6 << "\t" << GainRootSystSoilSurfPop << "\t" << GainRootSystVolPop << "\t" << GrainFillingStatus << "\t" << GrainFillingStatusFin << "\t" << GrainYieldPanicle << "\t" << GrainYieldPop << "\t" << GrainYieldPopFin << "\t" << GrowthDryMatPop << "\t" << GrowthPop << "\t" << GrowthResInternodePop << "\t" << GrowthStructDeficit << "\t" << GrowthStructInternodePop << "\t" << GrowthStructLeafPop << "\t" << GrowthStructPaniclePop << "\t" << GrowthStructRootPop << "\t" << GrowthStructSheathPop << "\t" << GrowthStructTotPop << "\t" << HMoyCalc << "\t" << HarvestIndex << "\t" << HaunGain << "\t" << HaunIndex << "\t" << Hum << "\t" << Ic << "\t" << IcCum << "\t" << IcMean << "\t" << IcPhase2 << "\t" << IcPhase3 << "\t" << IcPhase4 << "\t" << IcPhase5 << "\t" << IcPhase6 << "\t" << IncreaseResInternodePop << "\t" << InternodeResStatus << "\t" << IrrigAutoDay << "\t" << IrrigTotDay << "\t" << Irrigation << "\t" << KRolling << "\t" << KcTot << "\t" << Kce << "\t" << KceReal << "\t" << Kcl << "\t" << Kcp << "\t" << Kr << "\t" << LIRkdf << "\t" << LIRkdfcl << "\t" << LTRkdf << "\t" << LTRkdfcl << "\t" << Lai << "\t" << LaiDead << "\t" << LaiFin << "\t" << LastLeafLength << "\t" << LastLeafLengthPot << "\t" << LatRad << "\t" << LeafDeathPop << "\t" << Lr << "\t" << MaxLai << "\t" << MobiliLeafDeath << "\t" << NbDaysSinceGermination << "\t" << NbJAS << "\t" << NumPhase << "\t" << NumSsPhase << "\t" << NurseryStatus << "\t" << PARIntercepte << "\t" << PanStructMass << "\t" << PanicleFilDeficit << "\t" << PanicleFilPop << "\t" << PanicleNumPlant << "\t" << PanicleNumPop << "\t" << PanicleSinkPop << "\t" << Par << "\t" << PhaseStemElongation << "\t" << PlantHeight << "\t" << PlantLeafNumNew << "\t" << PlantLeafNumTot << "\t" << PlantWidth << "\t" << ProfRu << "\t" << RUE << "\t" << RayExtra << "\t" << RelPotLeafLength << "\t" << ResCapacityInternodePop << "\t" << ResInternodeMobiliDay << "\t" << ResInternodeMobiliDayPot << "\t" << ResUtil << "\t" << ReservePopFin << "\t" << RespMaintDebt << "\t" << RespMaintTot << "\t" << RgCalc << "\t" << RgMax << "\t" << RootFront << "\t" << RootFrontOld << "\t" << RootMassPerVol << "\t" << RootShootRatio << "\t" << RootSystSoilSurfPop << "\t" << RootSystSoilSurfPopOld << "\t" << RootSystVolPop << "\t" << RootSystVolPopOld << "\t" << RuRac << "\t" << RuSurf << "\t" << SDJCorPhase4 << "\t" << SeuilCstrMortality << "\t" << SeuilTemp << "\t" << SeuilTempSsPhase << "\t" << SimAnthesis50 << "\t" << SimEmergence << "\t" << SimEndCycle << "\t" << SimPanIni << "\t" << SimStartGermin << "\t" << SimStartMatu2 << "\t" << SimStartPSP << "\t" << Sla << "\t" << SlaMitch << "\t" << SlaNew << "\t" << SommeDegresJourMax << "\t" << SpikeNumPanicle << "\t" << SpikeNumPop << "\t" << StRuMax << "\t" << SterilityCold << "\t" << SterilityDrought << "\t" << SterilityHeat << "\t" << SterilityTot << "\t" << SterilityTotFin << "\t" << StockMacropores << "\t" << StockRac << "\t" << StockSurface << "\t" << StockTotal << "\t" << StressCold << "\t" << SumDDPhasePrec << "\t" << SumDegreDayCor << "\t" << SumDegresDay << "\t" << SumPP << "\t" << SunDistance << "\t" << SunPosi << "\t" << SupplyTot << "\t" << TMoyCalc << "\t" << TMoyPrec << "\t" << TempLai << "\t" << TillerDeathPop << "\t" << TmaxMoy << "\t" << TminMoy << "\t" << Tr << "\t" << TrEff << "\t" << TrEffInst << "\t" << TrPot << "\t" << VPDCalc << "\t" << ValRDE << "\t" << ValRFE << "\t" << ValRSurf << "\t" << VitesseRacinaire << "\t" << VitesseRacinaireDay << "\t" << VolMacropores << "\t" << VolRelMacropores << "\t" << WueEt << "\t" << WueTot << endl;
#endif
    }
    #ifdef WITH_TRACE
    stream.flush();
    file.close();
#endif
}


void init_parameters(const samara::ModelParameters& parameters) {
    Altitude = parameters.get<double>("altitude");
    ASScstr = parameters.get<double>("asscstr");
    AttenMitch = parameters.get<double>("attenmitch");
    BundHeight = parameters.get<double>("bundheight");
    Ca = parameters.get<double>("ca");
    CO2Cp = parameters.get<double>("co2cp");
    CO2Exp = parameters.get<double>("co2exp");
    CO2Slopetr = parameters.get<double>("co2slopetr");
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
    TempSLA = parameters.get<double>("tempsla");
    TilAbility = parameters.get<double>("tilability");
    TLim = parameters.get<double>("tlim");
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

void reset_variables() {
    CulmsPerPlant = 0;
    CulmsPerHill = 0;
    CulmsPop = 0;
    GrainYieldPop = 0;
    DryMatStructLeafPop = 0;
    DryMatStructSheathPop = 0;
    DryMatStructRootPop = 0;
    DryMatStructInternodePop = 0;
    DryMatResInternodePop = 0;
    DryMatStructPaniclePop = 0;
    DryMatStructStemPop = 0;
    DryMatStructTotPop = 0;
    DryMatVegeTotPop = 0;
    DryMatPanicleTotPop = 0;
    DryMatAboveGroundPop = 0;
    DryMatTotPop = 0;
    HarvestIndex = 0;
    PanicleNumPop = 0;
    PanicleNumPlant = 0;
    GrainYieldPanicle = 0;
    SpikeNumPop = 0;
    SpikeNumPanicle = 0;
    FertSpikeNumPop = 0;
    GrainFillingStatus = 0;
    PhaseStemElongation = 0;
    Sla = 0;
    HaunIndex = 0;
    ApexHeight = 0;
    PlantHeight = 0;
    PlantWidth = 0;
    VitesseRacinaireDay = 0;
    Kcl = 0;
    KRolling = 0;
    LIRkdfcl = 0;
    LTRkdfcl = 1;
    AssimPot = 0;
    Assim = 0;
    RespMaintTot = 0;
    SupplyTot = 0;
    AssimSurplus = 0;
    AssimNotUsed = 0;
    AssimNotUsedCum = 0;
    TillerDeathPop = 0;
    DeadLeafdrywtPop = 0;
    ResCapacityInternodePop = 0;
    InternodeResStatus = 0;
    Cstr = 0;
    FTSW = 0;
    DryMatAboveGroundTotPop = 0;
}

void set_meteo_vars(const samara::ModelParameters& parameters, double t) {
    TMax = parameters.get(t).TMax;
    TMin = parameters.get(t).TMin;
    TMoy = parameters.get(t).TMoy; //local
    HMax = parameters.get(t).HMax;
    HMin = parameters.get(t).HMin;
    HMoy = parameters.get(t).HMoy;//local
    Vt = parameters.get(t).Vt;
    Ins = parameters.get(t).Ins;
    Rg = parameters.get(t).Rg;
    ETP = parameters.get(t).ETP;
    Pluie = parameters.get(t).Rain;
    if (((TMin != NilValue) && (TMax != NilValue))) {
      TMoyCalc = (TMax + TMin) * 1.0 / 2;
    } else {
      TMoyCalc = TMoy;
    }
    if (((HMin != NilValue) && (HMax != NilValue))) {
      HMoyCalc = (HMax + HMin) * 1.0 / 2;
    } else {
      HMoyCalc = HMoy;
    }
}


void init_parcelle() {
    try {
		double Stockini2;
		double Stockini1;
        VolRelMacropores = 100 * (HumSat - HumFC);
        ResUtil = (HumFC - HumPF) * 1000;
        ProfRu = EpaisseurSurf + EpaisseurProf;
        // à supprimer ultérieurement dans les différents modules
        RuSurf = ResUtil * EpaisseurSurf * 1.0 / 1000;
        CapaREvap = 0.5 * EpaisseurSurf * HumPF;
        CapaRFE = PEvap * (CapaREvap + RuSurf);
        CapaRDE = RuSurf - CapaRFE;
        StRuMax = ResUtil * ProfRu * 1.0 / 1000;
        Stockini1 = min(StockIniSurf, CapaREvap + RuSurf);
        Stockini2 = min(StockIniProf, ResUtil * EpaisseurProf * 1.0 / 1000);
        ValRSurf = min(Stockini1, CapaREvap + CapaRDE);
        ValRDE = max(0., ValRSurf - CapaREvap);
        ValRFE = max(0., Stockini1 - (CapaREvap + CapaRDE));
        StockSurface = ValRDE + ValRFE;
        StockTotal = StockSurface + Stockini2; //transpirable
        Hum = StockTotal;
        LTRkdfcl = 1;
        LIRkdf = 0;
        LTRkdf = 0;
        CounterNursery = 0;
        VolMacropores = VolRelMacropores * (EpaisseurSurf + EpaisseurProf) * 1.0 / 100;

    } catch (...) {
        error_message("RS_InitParcelle_V2", URisocas);
    }
}

void eval_Par(double t) {
  try {
    double tDay = artis::utils::DateTime::dayOfYear(t);
    Decli = 0.409 * sin(0.0172 * tDay - 1.39);
    SunPosi = acos(-tan(LatRad) * tan(Decli));
    DayLength = 7.64 * SunPosi;
    SunDistance = 1 + 0.033 * cos(2 * M_PI * 1.0 / 365 * tDay);
    RayExtra = 24 * 60 * 0.0820 * 1.0 / M_PI * SunDistance *
               (SunPosi * sin(Decli) * sin(LatRad) +
                cos(Decli) * cos(LatRad) * sin(SunPosi));
    RgMax = (0.75 + 0.00002 * Altitude) * RayExtra;

    if ((Rg == NilValue)) {
      RgCalc = (0.25 + 0.50 * std::min(Ins * 1.0 / DayLength, 1.)) * RayExtra;
    } else {
      RgCalc = Rg;
    }

     Par = KPar * RgCalc;
  } catch (...) {
    error_message("EvalRg", UMeteo);
  }
}


void EToFao() {
    try {	 
      if ((ETP == NilValue)) {
		  double eActual; double eSat; double RgRgMax; double TLat; double delta; double KPsy; double Eaero; double Erad; double Rn; double G;
        eSat = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) +
                         exp(17.27 * TMin * 1.0 / (TMin + 237.3)));
        if ((HMax == NilValue))
          eActual = eSat * HMoyCalc * 1.0 / 100;
        else
          eActual = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) *
                              HMin * 1.0 / 100 + exp(17.27 * TMin * 1.0 / (TMin + 237.3)) *
                              HMax * 1.0 / 100);
        VPDCalc = eSat - eActual;
        RgRgMax = RgCalc * 1.0 / RgMax;
        if ((RgRgMax > 1))
          RgRgMax = 1;
        Rn = 0.77 * RgCalc - (1.35 * RgRgMax - 0.35) *
             (0.34 - 0.14 * std::pow(eActual, 0.5)) *
             (pow(TMax + 273.16, 4) + std::pow(TMin + 273.16, 4)) * 2.45015 * std::pow(10
                                                                                       , -9);
        // chaleur latente de vaporisation de l'eau
        TLat = 2.501 - 2.361 * std::pow(10, -3) * TMoyCalc;
        //  pente de la courbe de pression de vapeur saturante en kPa/°C
        delta = 4098 * (0.6108 * exp(17.27 * TMoyCalc * 1.0 / (TMoyCalc + 237.3))) * 1.0 / std::pow(TMoyCalc
                + 237.3, 2);
        // constante psychrométrique en kPa/°C
        KPsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Altitude * 1.0 / 293), 5.26) * 1.0 / TLat;
        // Radiative
        G = 0.38 * (TMoyCalc - TMoyPrec);
        Erad = 0.408 * (Rn - G) * delta * 1.0 / (delta + KPsy * (1 + 0.34 * Vt));
        // Partie évaporative de ET0 = Eaéro
        Eaero = (900 * 1.0 / (TMoyCalc + 273.16)) * ((eSat - eActual) * Vt) * KPsy * 1.0 /
                (delta + KPsy * (1 + 0.34 * Vt));
        ETo = Erad + Eaero;
      } else {
        ETo = ETP;
      }
      TMoyPrec = TMoyCalc;

    } catch (...) {
      error_message("EToFAO", UMeteo);
    }
  }

void kill_crop(){
    NumPhase = 0;
    RelPotLeafLength = 0;
    RootFront = 0;
    SumDegreDayCor = 0;
    RootMassPerVol = 0;
    TrPot = 0;
    Lai = 0;
    LIRkdfcl = 0;
    RespMaintDebt = 0;
    WueEt = 0;
    WueTot = 0;
    A_DemStructLeaf = 0;
    A_DemStructTot = 0;
    CoeffCO2Tr = 0;
    CoeffCO2Assim = 0;
    CstrMean = 0;
    DemResInternodePop = 0;
    GrowthDryMatPop = 0;
    Tr = 0;
    TrEff = 0;
    ConversionEff = 0;
    GrowthPop = 0;
    IcMean = 0;
    DryMatResInternodePopOld = 0;
    CumGrowthPop = 0;
    CumCarbonUsedPop = 0;
    SterilityTot = 0;
    SterilityDrought = 0;
    A_AssimSurplus = 0;
    ETM = 0;
}


void init_culture() {
    try {
        NumPhase = 0;
        SommeDegresJourMax = SDJLevee + SDJBVP + SDJRPR + SDJMatu1 + SDJMatu2;
        SumDegresDay = 0;
        SeuilTemp = 0;
        Lai = 0;
        IcCum = 0;
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
        CumFTSWPhase2 = 0;
        CumFTSWPhase3 = 0;
        CumFTSWPhase4 = 0;
        CumFTSWPhase5 = 0;
        CumFTSWPhase6 = 0;
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
        FloodwaterGain = 0;
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
    } catch (...) {
        error_message("RS_InitiationCulture", URisocas);
    }
}

