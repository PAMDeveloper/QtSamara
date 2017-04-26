#ifndef DELPHIRUNNER_H
#define DELPHIRUNNER_H
#include "delphi_defines.h"
#include "BhyTypeFAO.h"
#include "Bileau.h"
#include "Meteo.h"
#include "MilBilanCarbone.h"
#include "Risocas2cpp.h"
#include "Riz.h"
#include "sorghum.h"
#include <ModelParameters.hpp>

void InitSimulation(double &anDebutSimul, double &anFinSimul, double &nbAnSim, double &dateDebutSimul, double &dateFinSimul, double &annee, double &maxNbjSimule, double &dateEnCours, double &nbjSemis, double &dateSemisCalc,
                    double const &NbJAS, double const &codeParcelle)
// Initialisation des divers paramètres d'entrée. Cette procédure correspond au module 0.
{
  try {
    // récupération des dates et écriture dans rescrop(ou tabValues) à la sortie de la procedure
    //    anDebutSimul = DbModule1.Simule.findfield( "AnDebutSimul" ).Value;
    //    anFinSimul = DbModule1.Simule.findfield( "AnFinSimul" ).Value;
    //    nbAnSim = DbModule1.Simule.findfield( "NbAnSim" ).Value;
    //    dateFinSimul = DbModule1.Simule.findfield( "FinSimul" ).Value;
    //    dateDebutSimul = DbModule1.Simule.findfield( "DebutSimul" ).Value;
    //    annee = DbModule1.Simule.findfield( "AnDebutSimul" ).Value;
//    dateEnCours = DbModule1.Simule.findfield("DebutSimul").Value;
    maxNbjSimule = std::trunc(dateFinSimul - dateDebutSimul) + 1;
    nbjSemis = NilValue;
    dateSemisCalc = NilValue;

  } catch (...) {
    AfficheMessageErreur("TSimule.InitSimulation", UClasseSimule);
  }
}
void run_21(samara::ModelParameters parameters) {
  //Simu parameters
    double AnDebutSimul, AnFinSimul, NbAnSim, DateFinSimul, DateDebutSimul, Annee, DateEnCours, MaxNbjSimule, NbjSemis, DateSemisCalc, CodeParcelle;

  //Model Parameters
  double
  Altitude, ASScstr, AttenMitch, BundHeight, Ca, CO2Cp, CO2Exp, CoeffAssimSla, CoefficientQ10, CoeffInternodeMass, CoeffInternodeNum,
            CoeffLeafDeath, CoeffLeafWLRatio, CoeffPanicleMass, CoeffPanSinkPop, CoeffResCapacityInternode, CoeffReserveSink, CoeffRootMassPerVolMax,
            CoeffTillerDeath, CoeffTransplantingShock, DensityField, DensityNursery, DEVcstr, DurationNursery, EpaisseurProf, EpaisseurSurf,
            ExcessAssimToRoot, FTSWIrrig, HaunCritTillering, HumFC, HumPF, HumSat, IcTillering, InternodeLengthMax, IrrigAutoResume, IrrigAutoStop,
            IrrigAutoTarget, KcMax, KCritSterCold1, KCritSterCold2, KCritSterFtsw1, KCritSterFtsw2, KCritSterHeat1, KCritSterHeat2, KCritStressCold1,
            KCritStressCold2, Kdf, KRespInternode, KRespMaintLeaf, KRespMaintRoot, KRespMaintSheath, KRespPanicle, KTempMaint, Latitude, LeafLengthMax,
            LifeSavingDrainage, Mulch, PanStructMassMax, PercolationMax, PFactor, Phyllo, PlantsPerHill, PlotDrainageDAF, PoidsSecGrain, PourcRuiss,
            PPCrit, PPExp, PriorityPan, ProfRacIni, RankLongestLeaf, RelMobiliInternodeMax, RollingBase, RollingSens, RootCstr, RootFrontMax,
            RootPartitMax, SDJBVP, SDJLevee, SDJMatu1, SDJMatu2, SDJRPR, SeuilPP, SeuilRuiss, SlaMax, SlaMin, StockIniProf, StockIniSurf, TBase,
            TempSLA, TilAbility, TLim, TOpt1, TOpt2, TransplantingDepth, TxAssimBVP, TxAssimMatu1, TxAssimMatu2, TxConversion, TxResGrain, TxRuSurfGermi,
            VRacBVP, VRacLevee, VRacMatu1, VRacMatu2, VRacPSP, VRacRPR, WaterLoggingSens, WtRatioLeafSheath, IrrigAuto, Transplanting;

  //States
  double A_AssimSurplus, A_DemStructLeaf, A_DemStructTot, A_GrowthStructLeaf, A_GrowthStructTot, A_IncreaseResInternodePop, ApexHeight, ApexHeightGain,
         A_ResInternodeMobiliDay, Assim, AssimNotUsed, AssimNotUsedCum, AssimPot, AssimSurplus, CapaRDE, CapaREvap, CapaRFE, ChangePhase, ChangeSousPhase,
         CO2Slopetr, CoeffCO2Assim, CoeffCO2Tr, CoeffStressLogging, Conversion, ConversionEff, CounterNursery, Cstr, CstrAssim, CstrCum,
         CstrMean, CstrPhase2, CstrPhase3, CstrPhase4, CstrPhase5, CstrPhase6, CulmsPerHill, CulmsPerHillFin, CulmsPerHillMax, CulmsPerPlant,
         CulmsPerPlantFin, CulmsPerPlantMax, CulmsPop, CumCarbonUsedPop, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4, CumCstrPhase5, CumCstrPhase6,
         CumDr, CumEt, CumFTSWPhase2, CumFTSWPhase3, CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumGrowthPop, CumIcPhase2, CumIcPhase3, CumIcPhase4,
         CumIcPhase5, CumIcPhase6, CumIrrig, CumIrrigFin, CumLr, CumPAR, CumSupplyTot, CumTr, CumWReceived, CumWuse, CumWUse, CumWUseFin, DAF, DateSemis,
         DayLength, DeadLeafdrywtPop, Decli, DegresDuJour, DegresDuJourCor, DemLeafAreaPlant, DemPanicleFillPop, DemResInternodePop, DemStructInternodePlant,
         DemStructInternodePop, DemStructLeafPlant, DemStructLeafPop, DemStructPaniclePlant, DemStructPaniclePop, DemStructRootPlant, DemStructRootPop,
         DemStructSheathPop, DemStructTotPop, Density, Dr, DryMatAboveGroundPop, DryMatAboveGroundPopFin, DryMatAboveGroundTotPop, DryMatPanicleTotPop,
         DryMatResInternodePop, DryMatResInternodePopOld, DryMatStemPop, DryMatStructInternodePop, DryMatStructLeafPop, DryMatStructPaniclePop, DryMatStructRootPop,
         DryMatStructSheathPop, DryMatStructStemPop, DryMatStructTotPop, DryMatTotPop, DryMatTotPopFin, DryMatVegeTotPop, DureeDuJour, DurGermFlow, DurGermMat,
         DurPhase1, DurPhase2, DurPhase3, DurPhase4, DurPhase5, DurPhase6, EauDispo, ETM, ETo, ETP, ETR, Evap, EvapPot, FertSpikeNumPop, FertSpikePop,
         FloodwaterDepth, FloodwaterGain, FractionPlantHeightSubmer, FractionRootsLogged, FTSW, FtswMoy, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase5,
         FtswPhase6, GainRootSystSoilSurfPop, GainRootSystVolPop, GrainFillingStatus, GrainFillingStatusFin, GrainYieldPanicle, GrainYieldPop, GrainYieldPopFin,
         GrowthDryMatPop, GrowthPop, GrowthResInternodePop, GrowthStructDeficit, GrowthStructInternodePop, GrowthStructLeafPop, GrowthStructPaniclePop,
         GrowthStructRootPop, GrowthStructSheathPop, GrowthStructTotPop, HarvestIndex, HaunGain, HaunIndex, HMax, HMin, HMoy, HMoyCalc, Hum, Ic, IcCum, IcMean,
         IcPhase2, IcPhase3, IcPhase4, IcPhase5, IcPhase6, IncreaseResInternodePop, Ins, InternodeResStatus, Irrigation, IrrigAutoDay, IrrigTotDay, Kce,
         KceReal, Kcl, Kcp, KcTot, KPar, Kr, KRolling, Lai, LaiDead, LaiFin, LastLeafLength, LastLeafLengthPot, LatRad, LeafDeathPop, LIRkdf, LIRkdfcl, Lr,
         LTRkdf, LTRkdfcl, MaxLai, MobiliLeafDeath, MonCompteur, NbDaysSinceGermination, NbJAS, NumPhase, NumSsPhase, PanicleFilDeficit, PanicleFilPop,
         PanicleNumPlant, PanicleNumPop, PanicleSinkPop, PanStructMass, Par, PARIntercepte, PEvap, PhaseStemElongation, PlantHeight, PlantLeafNumNew, PlantLeafNumTot,
         PlantWidth, PPSens, ProfRu, Pluie, RayExtra, RelPhylloPhaseStemElong, RelPotLeafLength, ResCapacityInternodePop, ReservePopFin, ResInternodeMobiliDay,
         ResInternodeMobiliDayPot, RespMaintDebt, RespMaintTot, ResUtil, Rg, RgCalc, RgMax, RootFront, RootFrontOld, RootMassPerVol, RootShootRatio,
         RootSystSoilSurfPop, RootSystSoilSurfPopOld, RootSystVolPop, RootSystVolPopOld, RUE, RUEgreen, RuRac, RuSurf, SDJCorPhase4, SeuilCstrMortality,
         SeuilTempPhasePrec, SeuilTemp, SeuilTempSsPhase, SimAnthesis50, SimEmergence, SimEndCycle, SimPanIni, SimStartGermin,
         SimStartMatu2, SimStartPSP, Sla, SlaMitch, SlaNew, SumDegresDay, SumDegreDayCor, SommeDegresJourMax, SumDDPhasePrec,
         SpikeNumPanicle, SpikeNumPop, SterilityCold, SterilityDrought, SterilityHeat, SterilityTot, SterilityTotFin, StockMacropores, StockRac, StockSurface,
         StockTotal, StressCold, StRu, StRuMax, SumPP, SunDistance, SunPosi, SupplyTot, TempLai, TillerDeathPop, TMax, TmaxMoy, TMin, TminMoy, TMoy, TMoyCalc,
         TMoyPrec, Tr, TrEff, TrEffInst, TrPot, ValRDE, ValRFE, ValRSurf, Vt, VitesseRacinaire, VitesseRacinaireDay, VolMacropores, VolRelMacropores, VDPCalc,
         WueEt, WueTot, ChangeNurseryStatus, ChangeSsPhase, NbJourCompte, NurseryStatus, tabCstrIndiceCourant;

  //removed, added

InitSimulation(AnDebutSimul, AnFinSimul, NbAnSim, DateDebutSimul, DateFinSimul, Annee, MaxNbjSimule, DateEnCours, NbjSemis, DateSemisCalc, NbJAS, CodeParcelle);

risocas::RS_InitParcelle_V2(StockIniSurf, StockIniProf, EpaisseurSurf, EpaisseurProf, HumPF, HumFC, HumSat, PEvap, DateSemis, ResUtil, StockTotal, LTRkdfcl,
                              Hum, RuSurf, ProfRu, StRuMax, CapaREvap, CapaRFE, CapaRDE, ValRSurf, ValRDE, ValRFE, StockSurface, CounterNursery, VolRelMacropores,
                              VolMacropores, LIRkdf, LTRkdf);
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

risocas::RS_Transplanting_V2(NumPhase, DensityNursery, DensityField, DurationNursery, PlantsPerHill, Transplanting, NurseryStatus, ChangeNurseryStatus,
                               CounterNursery, Density, DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
                               DryMatStructPaniclePop, DryMatResInternodePop);

Meteo::DegToRad(Latitude, LatRad);
Meteo::AVGTempHum(TMin, TMax, HMin, HMax, TMoy, HMoy, TMoyCalc, HMoyCalc);
Meteo::EvalDecli(DateEnCours, Decli);
Meteo::EvalSunPosi(LatRad, Decli, SunPosi);
Meteo::EvalDayLength(SunPosi, DayLength);
Meteo::EvalSunDistance(DateEnCours, SunDistance);
Meteo::EvalRayExtra(SunPosi, Decli, SunDistance, LatRad, RayExtra);
Meteo::EvalRgMax(RayExtra, Altitude, RgMax);
Meteo::InsToRg(DayLength, Ins, RayExtra, RgMax, Rg, RgCalc);
Meteo::EvalPar(RgCalc, KPar, Par);
Meteo::EToFao(ETP, Altitude, RgMax, RgCalc, TMin, TMax, HMin, HMax, HMoyCalc, TMoyCalc, Vt, ETo, TMoyPrec, VDPCalc);
Riz::EvolPhenoPSPStress(SumPP, PPSens, SumDegreDayCor, SDJLevee, SDJBVP, SDJRPR, SDJMatu1, SDJMatu2, StockSurface, TxRuSurfGermi, RuSurf,
                          DateEnCours, DateSemis, StockTotal, NumPhase, SumDDPhasePrec, SeuilTemp, ChangePhase, SeuilTempSsPhase, ChangeSsPhase, NumSsPhase);
risocas::RS_EvalSimAnthesis50(NumPhase, ChangePhase, NbJAS, SimAnthesis50);
risocas::RS_EvalDateGermination(NumPhase, ChangePhase, NbDaysSinceGermination);
risocas::RS_EvalColdStress(KCritStressCold1, KCritStressCold2, TMin, StressCold);
risocas::RS_EvalSimEmergence(NumPhase, ChangePhase, NbJAS, SimEmergence);
risocas::RS_EvalSimPanIni(NumPhase, ChangePhase, NbJAS, SimPanIni);
risocas::RS_EvalSimStartGermin(NumPhase, ChangePhase, NbJAS, SimStartGermin);
risocas::RS_EvalSimStartMatu2(NumPhase, ChangePhase, NbJAS, SimStartMatu2);
risocas::RS_EvalSimStartPSP(NumPhase, ChangePhase, NbJAS, SimStartPSP);
risocas::RS_EvalDegresJourVitMoy_V2(NumPhase, TMax, TMin, TBase, TOpt1, TOpt2, TLim, Cstr, DEVcstr, StressCold, DegresDuJour, DegresDuJourCor);
risocas::RS_EvalSDJPhase4(NumPhase, DegresDuJourCor, SDJCorPhase4);
risocas::RS_EvalDAF_V2(NumPhase, DAF);
risocas::RS_Phyllochron(NumPhase, DegresDuJourCor, Phyllo, RelPhylloPhaseStemElong, PhaseStemElongation, HaunGain, HaunIndex);
risocas::RS_EvolHauteur_SDJ_cstr_V2_1(PhaseStemElongation, CoeffInternodeNum, HaunGain, Cstr, InternodeLengthMax, RelPotLeafLength, LeafLengthMax,
                                        CulmsPerHill, IcMean, Kdf, Ic, WtRatioLeafSheath, StressCold, CstrMean, ApexHeightGain, ApexHeight, PlantHeight, PlantWidth);
risocas::RS_EvolKcpKceBilhy(LTRkdfcl, KcMax, Mulch, Kcp, Kce, KcTot);
risocas::RS_EvalEvapPot(ETo, Kce, EvapPot);
risocas::RS_EvolEvapSurfRFE_RDE_V2_1(NumPhase, Kce, EvapPot, CapaREvap, CapaRDE, CapaRFE, RuRac, RuSurf, BundHeight, EpaisseurSurf, EpaisseurProf,
                                       RootFront, ResUtil, Evap, ValRSurf, ValRFE, ValRDE, StockRac, StockTotal, StockSurface, Kr, KceReal, FloodwaterDepth, StockMacropores);
risocas::RS_EvalFTSW_V2(RuRac, StockTotal, StockMacropores, StRuMax, StockRac, FTSW);
risocas::RS_EvalCstrPFactorFAO_V2(PFactor, FTSW, ETo, KcTot, StockMacropores, CoeffStressLogging, Cstr);
Bileau::DemandePlante_V2_1(Kcp, ETo, Ca, CO2Slopetr, TrPot, CoeffCO2Tr);
Bileau::EvalTranspi(TrPot, Cstr, Tr);
BhyTypeFAO::EvalETRETM(Evap, Tr, TrPot, ETM, ETR);
risocas::RS_EvolConsRes_Flood_V2(NumPhase, RuRac, RuSurf, CapaREvap, Tr, Evap, CapaRDE, CapaRFE, EpaisseurSurf, EpaisseurProf, ResUtil, StockRac,
                                   StockSurface, StockTotal, ValRFE, ValRDE, ValRSurf, FloodwaterDepth, StockMacropores);
Riz::RS_EvalTMaxMoy(TMax, NumPhase, NumSsPhase, TmaxMoy);
Riz::RS_EvalTMinMoy(TMin, NumPhase, NumSsPhase, TminMoy);
Riz::RS_EvalFtswMoy(FTSW, NumPhase, NumSsPhase, FtswMoy);
risocas::RS_EvalSterility(NumPhase, ChangePhase, KCritSterCold1, KCritSterCold2, KCritSterHeat1, KCritSterHeat2, KCritSterFtsw1, KCritSterFtsw2, TminMoy,
                            TmaxMoy, FtswMoy, SterilityCold, SterilityHeat, SterilityDrought, SterilityTot);
risocas::RS_EvalVitesseRacinaire(VRacLevee, VRacBVP, VRacRPR, VRacPSP, VRacMatu1, VRacMatu2, RootCstr, Cstr, NumPhase, DegresDuJourCor, VitesseRacinaire, VitesseRacinaireDay);
MilBilanCarbone::EvalConversion(NumPhase, TxConversion, TxAssimBVP, SumDegresDay, SumDDPhasePrec, TxAssimMatu1, TxAssimMatu2, SeuilTemp, Conversion);
risocas::RS_EvalParIntercepte_V2_1(Par, Lai, Kdf, PARIntercepte, LIRkdfcl);
risocas::RS_EvalAssimPot_V2_1(PARIntercepte, Par, Conversion, TMax, TMin, TBase, TOpt1, DayLength, StressCold, CO2Exp, Ca, CO2Cp, SlaMin, Sla, CoeffAssimSla,
                                AssimPot, CoeffCO2Assim);
risocas::RS_EvalCstrAssim(Cstr, ASScstr, CstrAssim);
risocas::RS_EvalAssim(AssimPot, CstrAssim, Assim);
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
risocas::RS_EvolGrowthStructTot_V2_1(NumPhase, SupplyTot, GrowthResInternodePop, GrowthStructTotPop, AssimSurplus, GrowthStructLeafPop, GrowthStructSheathPop,
                                       GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop, A_GrowthStructLeaf, A_GrowthStructTot, A_AssimSurplus);
risocas::RS_AddResToGrowthStructPop_V2_1(NumPhase, Ic, PhaseStemElongation, DryMatResInternodePop, DemStructTotPop, DemStructLeafPop, DemStructSheathPop,
                                           DemStructRootPop, DemStructInternodePop, DemStructPaniclePop, RelMobiliInternodeMax, GrowthResInternodePop,
                                           ResInternodeMobiliDayPot, GrowthStructDeficit, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop,
                                           GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop, ResInternodeMobiliDay, A_GrowthStructLeaf,
                                           A_GrowthStructTot, A_ResInternodeMobiliDay);
risocas::RS_EvolDemPanFilPopAndIcPFlow_V2_1(NumPhase, DryMatStructPaniclePop, CoeffPanSinkPop, SterilityTot, DegresDuJourCor, SDJMatu1, SupplyTot, Assim,
                                              RespMaintTot, StressCold, PanicleSinkPop, DemPanicleFillPop, AssimSurplus, Ic, A_AssimSurplus);
risocas::RS_EvolPanicleFilPop_V2_1(NumPhase, Ic, DryMatResInternodePop, DemPanicleFillPop, SupplyTot, RelMobiliInternodeMax, RespMaintTot, Assim,
                                     ResInternodeMobiliDayPot, AssimSurplus, PanicleFilDeficit, ResInternodeMobiliDay, PanicleFilPop, GrainYieldPop,
                                     A_AssimSurplus, A_ResInternodeMobiliDay);
risocas::RS_EvolGrowthReserveInternode_V2_1(NumPhase, PhaseStemElongation, DryMatStructInternodePop, DryMatStructSheathPop, CoeffResCapacityInternode,
                                              AssimSurplus, ResInternodeMobiliDay, ResCapacityInternodePop, IncreaseResInternodePop, DryMatResInternodePop,
                                              AssimNotUsed, AssimNotUsedCum, GrowthResInternodePop, DryMatResInternodePopOld, A_IncreaseResInternodePop);
risocas::RS_EvolGrowthTot_V2_1(NumPhase, GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop,
                                 GrowthResInternodePop, PanicleFilPop, DryMatResInternodePop, DryMatResInternodePopOld, GrowthStructTotPop, GrowthDryMatPop,
                                 A_GrowthStructTot);
risocas::RS_ExcessAssimilToRoot_V2(NumPhase, ExcessAssimToRoot, DryMatStructRootPop, RootSystVolPop, CoeffRootMassPerVolMax, RootMassPerVol, GrowthStructRootPop,
                                     AssimNotUsed);
risocas::RS_EvolDryMatTot_V2_1(NumPhase, ChangePhase, PlantsPerHill, TxResGrain, PoidsSecGrain, Density, GrowthStructLeafPop, GrowthStructSheathPop,
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
risocas::RS_EvalClumpAndLightInter_V2_1(NumPhase, KRolling, Density, PlantWidth, PlantHeight, Kdf, Lai, FractionPlantHeightSubmer, LIRkdf, LIRkdfcl, LTRkdf, LTRkdfcl);
risocas::RS_EvalSlaMitch(SlaMax, SlaMin, AttenMitch, SumDegresDay, SDJLevee, NumPhase, DegresDuJourCor, TOpt1, TBase, TempSLA, DryMatStructLeafPop,
                           GrowthStructLeafPop, SlaMitch, SlaNew, Sla);
risocas::RS_EvalRuiss_FloodDyna_V2(NumPhase, Pluie, SeuilRuiss, PourcRuiss, BundHeight, Irrigation, PlantHeight, LifeSavingDrainage, PlotDrainageDAF,
                                     VolMacropores, SeuilRuiss, PercolationMax, DAF, StockMacropores, FloodwaterDepth, EauDispo, Lr);
risocas::RS_AutomaticIrrigation_V2_1(NumPhase, IrrigAuto, IrrigAutoTarget, BundHeight, PlantHeight, Irrigation, PlotDrainageDAF, DAF, VolMacropores,
                                       VolRelMacropores, Pluie, FTSWIrrig, IrrigAutoStop, IrrigAutoResume, ChangeNurseryStatus, PercolationMax, NbJAS,
                                       RuSurf, ResUtil, RootFront, EpaisseurSurf, EpaisseurProf, ProfRacIni, FloodwaterDepth, IrrigAutoDay, IrrigTotDay,
                                       StockMacropores, EauDispo, RuRac, StockRac, FTSW, Lr);
risocas::RS_EvolRempliResRFE_RDE_V2(NumPhase, RuSurf, EauDispo, RuRac, CapaRFE, CapaREvap, CapaRDE, StRuMax, PercolationMax, BundHeight, EpaisseurSurf,
                                      EpaisseurProf, VolMacropores, FloodwaterDepth, StockTotal, StockRac, Hum, StockSurface, Dr, ValRDE, ValRFE, ValRSurf,
                                      FloodwaterGain, StockMacropores);
risocas::RS_EvolWaterLoggingUpland_V2(PercolationMax, BundHeight, VolMacropores, Dr, Lr, StockMacropores);
risocas::RS_EvalStressWaterLogging_V2(StockMacropores, VolMacropores, RootFront, EpaisseurSurf, EpaisseurProf, WaterLoggingSens, FractionRootsLogged,
                                        CoeffStressLogging);
risocas::RS_EvolRempliMacropores_V2(NumPhase, EpaisseurSurf, EpaisseurProf, ResUtil, StockMacropores, RootFront, CapaRDE, CapaRFE, FloodwaterDepth,
                                      StockTotal, Hum, StockSurface, StockRac, ValRDE, ValRFE, ValRSurf);
risocas::RS_EvolRurRFE_RDE_V2_1(VitesseRacinaire, Hum, ResUtil, StockSurface, RuSurf, ProfRacIni, EpaisseurSurf, EpaisseurProf, ValRDE, ValRFE, NumPhase,
                                  ChangePhase, FloodwaterDepth, StockMacropores, RootFrontMax, ChangeNurseryStatus, Transplanting, TransplantingDepth, RuRac,
                                  StockRac, StockTotal, FloodwaterGain, RootFront);
risocas::RS_PlantSubmergence_V2(PlantHeight, FloodwaterDepth, FractionPlantHeightSubmer);
risocas::RS_EvalRootFront(NumPhase, RuRac, ResUtil, RootFront);
risocas::RS_EvolPSPMVMD(NumPhase, ChangePhase, SumDegreDayCor, DegresDuJourCor, SeuilPP, PPCrit, DayLength, PPExp, SumPP, SumDDPhasePrec, SeuilTemp);
MilBilanCarbone::EvolSomDegresJour(DegresDuJour, NumPhase, SumDegresDay);
risocas::RS_EvolSomDegresJourCor(DegresDuJourCor, NumPhase, SumDegreDayCor);
risocas::RS_EvalRUE_V2_1(NumPhase, ChangePhase, PARIntercepte, DryMatTotPop, DeadLeafdrywtPop, DryMatStructRootPop, Tr, Evap, Dr, Lr, SupplyTot, AssimNotUsed,
                           Irrigation, IrrigAutoDay, Pluie, Assim, AssimPot, Conversion, NbJAS, Transplanting, NurseryStatus, Density, DensityNursery,
                           DryMatAboveGroundTotPop, DryMatAboveGroundPop, RUE, CumPAR, CumTr, CumEt, CumWUse, CumWReceived, CumIrrig, CumDr, CumLr, TrEffInst,
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
risocas::RS_EvalSimEndCycle_V2_1(NumPhase, ChangePhase, NbJAS, SimEndCycle);
}

void run_2() {}
void run_22() {}
void run_23() {}

#endif // DELPHIRUNNER_H
