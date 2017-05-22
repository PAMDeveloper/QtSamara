#ifndef DELPHIRUNNER_H
#define DELPHIRUNNER_H
#include "delphi_defines.h"
#include "BhyTypeFAO.h"
#include "Bileau.h"
#include "MeteoCalc.h"
#include "MilBilanCarbone.h"
#include "Risocas2cpp.h"
#include "Riz.h"
#include "sorghum.h"
#include "ModelParameters.hpp"
#include "DateTime.hpp"

#include "parameters.h"
#include "meteo.h"
#include "variables.h"
#include "parcelle.h"

void run_samara_21(const samara::ModelParameters& parameters, int log_every) {
    //Corrections
    AttenMitch = 0.9955;
    CO2Slopetr = -0.0004;
    CO2Exp = 0.004;
    SlaMax = 0.005;
    SlaMin = 0.0018;
    PoidsSecGrain = 0.028;

    //Simu parameters
    std::string startDate = parameters.getString("datedebut");
    std::string endDate = parameters.getString("datefin");
    std::string semisDate = parameters.getString("datesemis");
    double DateDebutSimul = artis::utils::DateTime::toJulianDayNumber(startDate);
    double DateFinSimul = artis::utils::DateTime::toJulianDayNumber(endDate);
    double DateEnCours = DateDebutSimul;
    double DateSemis = artis::utils::DateTime::toJulianDayNumber(semisDate);
    double NbJAS = DateEnCours - DateSemis;
    double NbDaysSinceGermination = NilValue;

	//Initialization
    init_parcelle();

    Meteo::DegToRad(Latitude, LatRad);

    bool crop = false;
    bool cropDead = false;
    for (DateEnCours; DateEnCours < DateFinSimul; DateEnCours++) {
        TMax = parameters.get(DateEnCours).TMax;
        TMin = parameters.get(DateEnCours).TMin;
        TMoy = parameters.get(DateEnCours).TMoy;
        HMax = parameters.get(DateEnCours).HMax;
        HMin = parameters.get(DateEnCours).HMin;
        HMoy = parameters.get(DateEnCours).HMoy;
        Vt = parameters.get(DateEnCours).Vt;
        Ins = parameters.get(DateEnCours).Ins;
        Rg = parameters.get(DateEnCours).Rg;
        ETP = parameters.get(DateEnCours).ETP;
        Pluie = parameters.get(DateEnCours).Rain;
        NbJAS = DateEnCours - DateSemis;

        //crop entity
        if(NbJAS == 0) {
            risocas::RS_InitiationCulture(SDJLevee, SDJBVP, SDJRPR, SDJMatu1, SDJMatu2, SommeDegresJourMax, NumPhase, SumDegresDay, SeuilTemp, Lai, IcCum, FTSW, Cstr,
                                          DurPhase1, DurPhase2, DurPhase3, DurPhase4, DurPhase5, DurPhase6, TempLai, ApexHeightGain, ChangeNurseryStatus, ChangePhase,
                                          ChangeSsPhase, CstrPhase2, CstrPhase3, CstrPhase4, CstrPhase5, CstrPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4,
                                          CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3, CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2,
                                          CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, DAF, DemLeafAreaPlant, DemPanicleFillPop, DemStructInternodePlant,
                                          DemStructInternodePop, DemStructLeafPlant, DemStructLeafPop, DemStructPaniclePlant, DemStructPaniclePop, DemStructRootPlant,
                                          DemStructRootPop, DemStructSheathPop, DemStructTotPop, FloodwaterGain, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6,
                                          GainRootSystSoilSurfPop, GainRootSystVolPop, GrowthDryMatPop, GrowthResInternodePop, GrowthStructDeficit, GrowthStructInternodePop,
                                          GrowthStructLeafPop, GrowthStructPaniclePop, GrowthStructRootPop, GrowthStructSheathPop, GrowthStructTotPop, HaunGain, IcPhase2,
                                          IcPhase3, IcPhase4, IcPhase5, IcPhase6, IncreaseResInternodePop, Kcl, Kr, MobiliLeafDeath, NbDaysSinceGermination, NurseryStatus,
                                          PanicleFilDeficit, PanicleFilPop, PanicleSinkPop, PanStructMass, PlantLeafNumNew, ResInternodeMobiliDay, ResInternodeMobiliDayPot,
                                          RootFrontOld, RootSystSoilSurfPop, RootSystSoilSurfPopOld, RootSystVolPop, RootSystVolPopOld, SDJCorPhase4);
            crop = true;
        }

        if ( NumPhase == 7 && ChangePhase == 1 ) {
            crop = false;
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
            cropDead = true;
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

        if(crop)risocas::RS_Transplanting_V2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                                     CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                                     DryMatStructPaniclePop, DryMatResInternodePop);
        Meteo::AVGTempHum(TMin, TMax, HMin, HMax, TMoy, HMoy, TMoyCalc, HMoyCalc);
        Meteo::EvalDecli(DateEnCours, Decli);
        Meteo::EvalSunPosi(LatRad, Decli, SunPosi);
        Meteo::EvalDayLength(SunPosi, DayLength);
        Meteo::EvalSunDistance(DateEnCours, SunDistance);
        Meteo::EvalRayExtra(SunPosi, Decli, SunDistance, LatRad, RayExtra);
        Meteo::EvalRgMax(RayExtra, Altitude, RgMax);
        Meteo::InsToRg(DayLength, Ins, RayExtra, RgMax, Rg, RgCalc);
        Meteo::EvalPar(RgCalc, KPar, Par);

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
                                       DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop, DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
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
        risocas::RS_ResetVariablesToZero_V2_1(NumPhase, ChangePhase, CulmsPerPlant, CulmsPerHill, CulmsPop, GrainYieldPop, DryMatStructLeafPop, DryMatStructSheathPop,
                                              DryMatStructRootPop, DryMatStructInternodePop, DryMatResInternodePop, DryMatStructPaniclePop, DryMatStemPop, DryMatStructTotPop,
                                              DryMatVegeTotPop, DryMatPanicleTotPop, DryMatAboveGroundPop, DryMatTotPop, HarvestIndex, PanicleNumPop, PanicleNumPlant,
                                              GrainYieldPanicle, SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop, GrainFillingStatus, PhaseStemElongation, Sla, HaunIndex,
                                              ApexHeight, PlantHeight, PlantWidth, VitesseRacinaireDay, Kcl, KRolling, LIRkdfcl, LTRkdfcl, AssimPot, Assim, RespMaintTot,
                                              SupplyTot, AssimSurplus, AssimNotUsed, AssimNotUsedCum, TillerDeathPop, DeadLeafdrywtPop, ResCapacityInternodePop,
                                              InternodeResStatus, Cstr, FTSW, DryMatAboveGroundTotPop);//, LaiDead);
//                risocas::RS_EvalSimEndCycle_V2_1(NumPhase, ChangePhase, NbJAS, SimEndCycle);


        //QString Jour = QString::fromStdString(artis::utils::DateTime::toJulianDayFmt(DateEnCours, artis::utils::DATE_FORMAT_YMD));
        //stream << Jour << "\t" << A_AssimSurplus << "\t" << A_DemStructLeaf << "\t" << A_DemStructTot << "\t" << A_GrowthStructLeaf << "\t" << A_GrowthStructTot << "\t" << A_IncreaseResInternodePop << "\t" << A_ResInternodeMobiliDay << "\t" << ApexHeight << "\t" << ApexHeightGain << "\t" << Assim << "\t" << AssimNotUsed << "\t" << AssimNotUsedCum << "\t" << AssimPot << "\t" << AssimSurplus << "\t" << CapaRDE << "\t" << CapaREvap << "\t" << CapaRFE << "\t" << ChangeNurseryStatus << "\t" << ChangePhase << "\t" << ChangeSsPhase << "\t" << CoeffCO2Assim << "\t" << CoeffCO2Tr << "\t" << CoeffStressLogging << "\t" << Conversion << "\t" << ConversionEff << "\t" << CounterNursery << "\t" << Cstr << "\t" << CstrAssim << "\t" << CstrCum << "\t" << CstrMean << "\t" << CstrPhase2 << "\t" << CstrPhase3 << "\t" << CstrPhase4 << "\t" << CstrPhase5 << "\t" << CstrPhase6 << "\t" << CulmsPerHill << "\t" << CulmsPerHillFin << "\t" << CulmsPerHillMax << "\t" << CulmsPerPlant << "\t" << CulmsPerPlantFin << "\t" << CulmsPerPlantMax << "\t" << CulmsPop << "\t" << CumCarbonUsedPop << "\t" << CumCstrPhase2 << "\t" << CumCstrPhase3 << "\t" << CumCstrPhase4 << "\t" << CumCstrPhase5 << "\t" << CumCstrPhase6 << "\t" << CumLr << "\t" << CumEt << "\t" << CumFTSWPhase2 << "\t" << CumFTSWPhase3 << "\t" << CumFTSWPhase4 << "\t" << CumFTSWPhase5 << "\t" << CumFTSWPhase6 << "\t" << CumGrowthPop << "\t" << CumIcPhase2 << "\t" << CumIcPhase3 << "\t" << CumIcPhase4 << "\t" << CumIcPhase5 << "\t" << CumIcPhase6 << "\t" << CumIrrig << "\t" << CumIrrigFin << "\t" << CumDr << "\t" << CumPAR << "\t" << CumSupplyTot << "\t" << CumTr << "\t" << CumWReceived << "\t" << CumWUse << "\t" << CumWUseFin << "\t" << DAF << "\t" << DayLength << "\t" << DeadLeafdrywtPop << "\t" << Decli << "\t" << DegresDuJour << "\t" << DegresDuJourCor << "\t" << DemLeafAreaPlant << "\t" << DemPanicleFillPop << "\t" << DemResInternodePop << "\t" << DemStructInternodePlant << "\t" << DemStructInternodePop << "\t" << DemStructLeafPlant << "\t" << DemStructLeafPop << "\t" << DemStructPaniclePlant << "\t" << DemStructPaniclePop << "\t" << DemStructRootPlant << "\t" << DemStructRootPop << "\t" << DemStructSheathPop << "\t" << DemStructTotPop << "\t" << Density << "\t" << Dr << "\t" << DryMatAboveGroundPop << "\t" << DryMatAboveGroundPopFin << "\t" << DryMatAboveGroundTotPop << "\t" << DryMatPanicleTotPop << "\t" << DryMatResInternodePop << "\t" << DryMatResInternodePopOld << "\t" << DryMatStemPop << "\t" << DryMatStructInternodePop << "\t" << DryMatStructLeafPop << "\t" << DryMatStructPaniclePop << "\t" << DryMatStructRootPop << "\t" << DryMatStructSheathPop << "\t" << DryMatStructTotPop << "\t" << DryMatTotPop << "\t" << DryMatTotPopFin << "\t" << DryMatVegeTotPop << "\t" << DurGermFlow << "\t" << DurGermMat << "\t" << DurPhase1 << "\t" << DurPhase2 << "\t" << DurPhase3 << "\t" << DurPhase4 << "\t" << DurPhase5 << "\t" << DurPhase6 << "\t" << ETM << "\t" << ETR << "\t" << ETo << "\t" << EauDispo << "\t" << Evap << "\t" << EvapPot << "\t" << FTSW << "\t" << FertSpikeNumPop << "\t" << FloodwaterDepth << "\t" << FloodwaterGain << "\t" << FractionPlantHeightSubmer << "\t" << FractionRootsLogged << "\t" << FtswMoy << "\t" << FtswPhase2 << "\t" << FtswPhase3 << "\t" << FtswPhase4 << "\t" << FtswPhase5 << "\t" << FtswPhase6 << "\t" << GainRootSystSoilSurfPop << "\t" << GainRootSystVolPop << "\t" << GrainFillingStatus << "\t" << GrainFillingStatusFin << "\t" << GrainYieldPanicle << "\t" << GrainYieldPop << "\t" << GrainYieldPopFin << "\t" << GrowthDryMatPop << "\t" << GrowthPop << "\t" << GrowthResInternodePop << "\t" << GrowthStructDeficit << "\t" << GrowthStructInternodePop << "\t" << GrowthStructLeafPop << "\t" << GrowthStructPaniclePop << "\t" << GrowthStructRootPop << "\t" << GrowthStructSheathPop << "\t" << GrowthStructTotPop << "\t" << HMoyCalc << "\t" << HarvestIndex << "\t" << HaunGain << "\t" << HaunIndex << "\t" << Hum << "\t" << Ic << "\t" << IcCum << "\t" << IcMean << "\t" << IcPhase2 << "\t" << IcPhase3 << "\t" << IcPhase4 << "\t" << IcPhase5 << "\t" << IcPhase6 << "\t" << IncreaseResInternodePop << "\t" << InternodeResStatus << "\t" << IrrigAutoDay << "\t" << IrrigTotDay << "\t" << Irrigation << "\t" << KRolling << "\t" << KcTot << "\t" << Kce << "\t" << KceReal << "\t" << Kcl << "\t" << Kcp << "\t" << Kr << "\t" << LIRkdf << "\t" << LIRkdfcl << "\t" << LTRkdf << "\t" << LTRkdfcl << "\t" << Lai << "\t" << LaiDead << "\t" << LaiFin << "\t" << LastLeafLength << "\t" << LastLeafLengthPot << "\t" << LatRad << "\t" << LeafDeathPop << "\t" << Lr << "\t" << MaxLai << "\t" << MobiliLeafDeath << "\t" << NbDaysSinceGermination << "\t" << NbJAS << "\t" << NumPhase << "\t" << NumSsPhase << "\t" << NurseryStatus << "\t" << PARIntercepte << "\t" << PanStructMass << "\t" << PanicleFilDeficit << "\t" << PanicleFilPop << "\t" << PanicleNumPlant << "\t" << PanicleNumPop << "\t" << PanicleSinkPop << "\t" << Par << "\t" << PhaseStemElongation << "\t" << PlantHeight << "\t" << PlantLeafNumNew << "\t" << PlantLeafNumTot << "\t" << PlantWidth << "\t" << ProfRu << "\t" << RUE << "\t" << RayExtra << "\t" << RelPotLeafLength << "\t" << ResCapacityInternodePop << "\t" << ResInternodeMobiliDay << "\t" << ResInternodeMobiliDayPot << "\t" << ResUtil << "\t" << ReservePopFin << "\t" << RespMaintDebt << "\t" << RespMaintTot << "\t" << RgCalc << "\t" << RgMax << "\t" << RootFront << "\t" << RootFrontOld << "\t" << RootMassPerVol << "\t" << RootShootRatio << "\t" << RootSystSoilSurfPop << "\t" << RootSystSoilSurfPopOld << "\t" << RootSystVolPop << "\t" << RootSystVolPopOld << "\t" << RuRac << "\t" << RuSurf << "\t" << SDJCorPhase4 << "\t" << SeuilCstrMortality << "\t" << SeuilTemp << "\t" << SeuilTempSsPhase << "\t" << SimAnthesis50 << "\t" << SimEmergence << "\t" << SimEndCycle << "\t" << SimPanIni << "\t" << SimStartGermin << "\t" << SimStartMatu2 << "\t" << SimStartPSP << "\t" << Sla << "\t" << SlaMitch << "\t" << SlaNew << "\t" << SommeDegresJourMax << "\t" << SpikeNumPanicle << "\t" << SpikeNumPop << "\t" << StRuMax << "\t" << SterilityCold << "\t" << SterilityDrought << "\t" << SterilityHeat << "\t" << SterilityTot << "\t" << SterilityTotFin << "\t" << StockMacropores << "\t" << StockRac << "\t" << StockSurface << "\t" << StockTotal << "\t" << StressCold << "\t" << SumDDPhasePrec << "\t" << SumDegreDayCor << "\t" << SumDegresDay << "\t" << SumPP << "\t" << SunDistance << "\t" << SunPosi << "\t" << SupplyTot << "\t" << TMoyCalc << "\t" << TMoyPrec << "\t" << TempLai << "\t" << TillerDeathPop << "\t" << TmaxMoy << "\t" << TminMoy << "\t" << Tr << "\t" << TrEff << "\t" << TrEffInst << "\t" << TrPot << "\t" << VDPCalc << "\t" << ValRDE << "\t" << ValRFE << "\t" << ValRSurf << "\t" << VitesseRacinaire << "\t" << VitesseRacinaireDay << "\t" << VolMacropores << "\t" << VolRelMacropores << "\t" << WueEt << "\t" << WueTot << endl;

    }
    //stream.flush();
    //file.close();
}

////Model Parameters
//double
//        Altitude, ASScstr, AttenMitch, BundHeight, Ca, CO2Cp, CO2Exp, CO2Slopetr, CoeffAssimSla, CoefficientQ10, CoeffInternodeMass, CoeffInternodeNum,
//        CoeffLeafDeath, CoeffLeafWLRatio, CoeffPanicleMass, CoeffPanSinkPop, CoeffResCapacityInternode, CoeffReserveSink, CoeffRootMassPerVolMax,
//        CoeffTillerDeath, CoeffTransplantingShock, DensityField, DensityNursery, DEVcstr, DurationNursery, EpaisseurProf, EpaisseurSurf,
//        ExcessAssimToRoot, FTSWIrrig, HaunCritTillering, HumFC, HumPF, HumSat, IcTillering, InternodeLengthMax, IrrigAuto, IrrigAutoResume, IrrigAutoStop,
//        IrrigAutoTarget, KcMax, KCritSterCold1, KCritSterCold2, KCritSterFtsw1, KCritSterFtsw2, KCritSterHeat1, KCritSterHeat2, KCritStressCold1,
//        KCritStressCold2, Kdf, KPar, KRespInternode, KRespMaintLeaf, KRespMaintRoot, KRespMaintSheath, KRespPanicle, KTempMaint, Latitude, LeafLengthMax,
//        LifeSavingDrainage, Mulch, PanStructMassMax, PercolationMax, PEvap, PFactor, Phyllo, PlantsPerHill, PlotDrainageDAF, PoidsSecGrain, PourcRuiss,
//        PPCrit, PPExp, PPSens, PriorityPan, ProfRacIni, RankLongestLeaf, RelMobiliInternodeMax, RelPhylloPhaseStemElong,  RollingBase, RollingSens, RootCstr, RootFrontMax,
//        RootPartitMax, SDJBVP, SDJLevee, SDJMatu1, SDJMatu2, SDJRPR, SeuilPP, SeuilRuiss, SlaMax, SlaMin, StockIniProf, StockIniSurf, TBase,
//        TempSLA, TilAbility, TLim, TOpt1, TOpt2, Transplanting, TransplantingDepth, TxAssimBVP, TxAssimMatu1, TxAssimMatu2, TxConversion, TxResGrain, TxRuSurfGermi,
//        VRacBVP, VRacLevee, VRacMatu1, VRacMatu2, VRacPSP, VRacRPR, WaterLoggingSens, WtRatioLeafSheath;

//Unused State
//    double ChangeSousPhase, CumWuse, DryMatStructStemPop, DureeDuJour, FertSpikePop, RUEgreen, SeuilTempPhasePrec, StRu, NbJourCompte, tabCstrIndiceCourant;

#endif // DELPHIRUNNER_H
