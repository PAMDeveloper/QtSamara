/**
 * @file model/models/samara/Model.hpp
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

#ifndef MODEL_MODELS_SAMARA_MODEL_HPP
#define MODEL_MODELS_SAMARA_MODEL_HPP

namespace model { namespace models { namespace samara {

class Model
{
public:
    Model()
    { }

    virtual ~Model()
    { }

    void compute(double time);

private:
    void evalDegreeDay();
    void phyllochron();
    void EvalCstrAssim();
    void EvalRespMaint();
    void EvolPlantTilNumTot();
    void EvolPlantLeafNumTot();
    void EvalRelPotLeafLength();
    void EvalDemandStructSheath();
    void EvalDemandStructRoot();
    void EvalDemandStructPanicle();
    void EvolGrowthStructSheathPop();
    void EvolGrowthStructRootPop();
    void EvolGrowthStructPanPop();
    void EvalSlaMitch();
    void EvolKcpKceBilhy();
    void EvolConsRes_Flood();
    void EvalConversion();
    void EvolPSPMVMD();
    void EvolSomDegresJourCor();
    void EvalMaximumLai();
    void EvalVitesseRacinaire();
    void InitiationCulture();
    void EvalRootFront();
    void EvalSDJPhase4();
    void EvalDateGermination();
    void EvalSterility();
    void InitParcelle();
    void EvalEvapPot();
    void transplanting();
    void TransplantingShock();
    void EvalRuiss_FloodDyna();
    void EvolRempliResRFE_RDE();
    void PlantSubmergence();
    void ExcessAssimilToRoot();
    void EvolRempliMacropores();
    void EvalFTSW();
    void EvalDAF();
    void EvalSimStartGermin();
    void EvalSimEmergence();
    void EvalSimStartPSP();
    void EvalSimPanIni();
    void EvalSimAnthesis50();
    void EvalSimStartMatu2();
    void EvalColdStress();
    void EvalAssim();
    void EvolWaterLoggingUpland();
    void EvalStressWaterLogging();
    void EvalCstrPFactorFAO();
    void EvolHauteur_SDJ_cstr();
    void EvalParIntercepte();
    void EvolMobiliTillerDeath();
    void EvolMobiliLeafDeath();
    void EvalSupplyTot();
    void EvalDemandStructLeaf();
    void EvalDemandStructIN();
    void EvalDemandTotAndIcPreFlow();
    void EvolGrowthStructLeafPop();
    void EvolGrowthStructINPop();
    void AddResToGrowthStructPop();
    void EvolDemPanFilPopAndIcPFlow();
    void EvolPanicleFilPop();
    void EvolGrowthReserveInternode();
    void EvolGrowthTot();
    void EvalLai();
    void EvalClumpAndLightInter();
    void EvalRUE();
    void EvolEvapSurfRFE_RDE();
    void EvolDryMatTot();
    void EvolGrowthStructTot();
    void LeafRolling();
    void EvalAssimPot();
    void AutomaticIrrigation();
    void EvolRurRFE_RDE();
    void EvalSimEndCycle();
    void Priority2GrowthPanStrctPop();
    void KeyResults();
    void DegToRad();
    void AVGTempHum();
    void EvalDecli(double t);
    void EvalSunPosi();
    void EvalDayLength();
    void EvalSunDistance(double t);
    void EvalRayExtra();
    void EvalRgMax();
    void InsToRg();
    void EvalPAR();
    void EToFAO();
    void EvolPhenoStress();
    void DemandePlante();
    void EvalTranspi();
    void EvalETRETM();
    void EvolSomDegresJour();
    void Mortality();

    // constantes
   static  double NullValue;

    // parameters
    double Phyllo;
    double TMax;
    double TMin;
    double TBase;
    double TOpt1;
    double TOpt2;
    double TLet;
    double DEVcstr;
    double CoeffInternodeNum;
    double InternodeLengthMax;
    double RelPotLeafLength;
    double LeafLengthMax;
    double Kdf;
    double WtRatioLeafSheath;
    double PriorityPan;
    double EpaisseurProf;
    double EpaisseurSurf;
    double TransplantingDepth;
    int Transplanting;
    double RootFrontMax;
    double ProfRacIni;
    double PercolationMax;
    double IrrigAutoTarget;
    int IrrigAuto;
    double PlotDrainageDAF;
    double IrrigAutoResume;
    double IrrigAutoStop;
    double FTSWIrrig;
    double BundHeight;
    double CoeffAssimSla;
    double SlaMax;
    double SlaMin;
    double CoeffCO2Assim;
    double Ca;
    double CO2Cp;
    double CO2Exp;
    double RollingSens;
    double RollingBase;
    double PoidsSecGrain;
    double CoeffPanSinkPop;
    double PlantsPerHill;
    double TxResGrain;
    double DensityNursery;
    double DensityField;
    double CoeffResCapacityInternode;
    double RelMobiliInternodeMax;
    double SDJMatu1;
    double SDJMatu2;
    double SDJBVP;
    double SDJLevee;
    double CoeffReserveSink;
    double CoeffInternodeMass;
    double CoeffLeafWLRatio;
    double CoeffLeafDeath;
    double CoeffTillerDeath;
    double SDJRPR;
    double PFactor;
    double WaterLoggingSens;
    double KCritStressCold1;
    double KCritStressCold2;
    double CoeffRootMassPerVolMax;
    double ExcessAssimToRoot;
    double PourcRuiss;
    double SeuilRuiss;
    double LifeSavingDrainage;
    double CoeffTransplantingShock;
    double DurationNursery;
    double StockIniProf;
    double StockIniSurf;
    double HumPF;
    double HumFC;
    double HumSat;
    double KCritSterFtsw1;
    double KCritSterFtsw2;
    double KCritSterHeat1;
    double KCritSterHeat2;
    double KCritSterCold1;
    double KCritSterCold2;
    double RootCstr;

    double VRacMatu1;
    double VRacMatu2;
    double VRacRPR;
    double VRacPSP;
    double VRacBVP;
    double VRacLevee;
    double SeuilPP;
    double PPCrit;
    double PPExp;
    double TxConversion;
    double TxAssimMatu1;
    double TxAssimMatu2;
    double TxAssimBVP;
    double Mulch;
    double KcMax;
    double TempSla;
    double AttenMitch;
    double PanStructMassMax;
    double CoeffPanicleMass;
    double RootPartitMax;
    double RankLongestLeaf;

    double TilAbility;
    double IcTillering;
    double HaunCritTillering;
    double kRespPanicle;
    double kRespInternode;
    double kRespMaintRoot;
    double kRespMaintSheath;
    double kRespMaintLeaf;
    double kTempMaint;

    double CoefficientQ10;
    double ASScstr;

    // state variables
    double NumPhase;
    double ChangePhase;
    double RelPhylloPhaseStemElong;
    double PhaseStemElongation;
    double HaunGain;
    double HaunIndex;
    double cstr;
    double StressCold;
    double DegresDuJour;
    double DegresDuJourCor;
    double IcMean;
    double Ic;
    double CstrMean;
    double ApexHeightGain;
    double ApexHeight;
    double PlantHeight;
    double PlantWidth;
    double CulmsPerPlant;
    double CulmsPerPlantMax;
    double CulmsPerHill;
    double CulmsPerHillMax;
    double CulmsPop;
    double GrainYieldPop;
    double DryMatStructLeafPop;
    double DryMatStructSheathPop;
    double DryMatStructRootPop;
    double DryMatStructInternodePop;
    double DryMatResInternodePop;
    double DryMatStructPaniclePop;
    double DryMatStructStemPop;
    double DryMatStructTotPop;
    double DryMatVegeTotPop;
    double DryMatPanicleTotPop;
    double DryMatAboveGroundPop;
    double DryMatTotPop;
    double HarvestIndex;
    double PanicleNumPop;
    double PanicleNumPlant;
    double GrainYieldPanicle;
    double SpikeNumPop;
    double SpikeNumPanicle;
    double FertSpikePop;
    double GrainFillingStatus;
    double Sla;
    double VitesseRacinaireDay;
    double Kcl;
    double KRolling;
    double LIRKdfcl;
    double LtrKdfcl;
    double AssimPot;
    double Assim;
    double RespMaintTot;
    double SupplyTot;
    double AssimSurplus;
    double AssimNotUsed;
    double AssimNotUsedCum;
    double TillerDeathPop;
    double DeadLeafDryWtPop;
    double ResCapacityInternodePop;
    double InternodeResStatus;
    double FTSW;
    double DryMatAboveGroundTotPop;
    double CumWUse;
    double CumWUseFin;
    double CumIrrig;
    double CumIrrigFin;
    double SterilityTot;
    double SterilityTotFin;
    double GrainFillingStatusFin;
    double DryMatTotPopFin;
    double ReservePopFin;
    double DryMatAboveGroundPopFin;
    double GrainYieldPopFin;
    double CulmsPerPlantFin;
    double CulmsPerHillFin;
    double Lai;
    double LaiFin;
    double DurGermMat;
    double DurGermFlow;
    double CstrPhase6;
    double CstrPhase5;
    double CstrPhase4;
    double CstrPhase3;
    double CstrPhase2;
    double FtswPhase6;
    double FtswPhase5;
    double FtswPhase4;
    double FtswPhase3;
    double FtswPhase2;
    double IcPhase6;
    double IcPhase5;
    double IcPhase4;
    double IcPhase3;
    double IcPhase2;
    double DurPhase6;
    double DurPhase5;
    double DurPhase4;
    double DurPhase3;
    double DurPhase2;
    double DurPhase1;
    double CumCstrPhase6;
    double CumCstrPhase5;
    double CumCstrPhase4;
    double CumCstrPhase3;
    double CumCstrPhase2;
    double CumFtswPhase6;
    double CumFtswPhase5;
    double CumFtswPhase4;
    double CumFtswPhase3;
    double CumFtswPhase2;
    double CumIcPhase6;
    double CumIcPhase5;
    double CumIcPhase4;
    double CumIcPhase3;
    double CumIcPhase2;

    double DemResInternodePop;
    double GrowthResInternodePop;
    double DemStructRootPop;
    double GrowthStructRootPop;
    double DemStructSheathPop;
    double GrowthStructSheathPop;
    double DemStructLeafPop;
    double GrowthStructLeafPop;
    double DemStructTotPop;
    double GrowthStructTotPop;
    double DemStructInternodePop;
    double GrowthStructInternodePop;
    double DemStructPaniclePop;
    double GrowthStructPaniclePop;
    double SimEndCycle;
    double NbJas;
    double StockTotal;
    double ResUtil;
    double RootFront;
    double StockRac;
    double FloodwaterDepth;
    double StockMacropores;

    int ChangeNurseryStatus;
    int NurseryStatus;

    double RuRac;
    double RuSurf;
    double ValRFE;
    double ValRDE;
    double VitesseRacinaire;
    double Hum;
    double IrrigAutoDay;
    double IrrigTotDay;
    double EauDispo;
    double Rain;
    double VolMacropores;
    double DAF;
    double Lr;
    double Irrigation;
    double PAR;
    double DayLength;
    double Conversion;
    double PARIntercepte;
    double Eto;

    double A_GrowthStructTot;
    double A_GrowthStructLeaf;
    double A_AssimSurplus;
    double FertSpikeNumPop;

    double Density;
    double RootShootRatio;
    double CumCarbonUsedPop;
    double DeadLeafDrywtPop;

    double GrowthPop;
    double MobiliLeafDeath;
    double PanicleFilPop;
    double DryMatResInternodePopOld;
    double CumGrowthPop;
    double Kce;
    double KceReal; // parameter ????
    double DryMatStemPop;
    double Kr;
    double StockSurface;
    double CapaRDE;
    double CapaREvap;
    double CapaRFE;
    double ValRSurf;
    double EvapPot;
    double Evap;
    double CumPar;
    double RUE;
    double CumWuse;
    double WueTot;
    double CumEt;
    double WueEt;
    double CumTr;
    double TrEff;
    double TrEffInst;
    double Tr;
    double ConversionEff;
    double CumLr;
    double Dr;
    double CumDr;
    double CumWReceived;
    double LTRkdfcl;
    double LTRkdf;
    double LIRkdfcl;
    double LIRkdf;
    double FractionPlantHeightSubmer;
    double LastLeafLength; // parameter ???
    double LastLeafLengthPot; // parameter ???
    double GrowthDryMatPop;
    double ResInternodeMobiliDay;
    double A_IncreaseResInternodePop;
    double A_ResInternodeMobiliDay;
    double DemPanicleFillPop;
    double PanicleFilDeficit;
    double IncreaseResInternodePop;
    double ResInternodeMobiliDayPot;
    double PanicleSinkPop;
    double GrowthStructDeficit;
    double NbDaysSinceGermination;
    double IcCumul;
    double CstrCumul;
    double A_DemStructTot;
    double DemStructInternodePlant;
    double A_DemStructLeaf;
    double DemStructLeafPlant;
    double SlaNew;
    double PlantLeafNumNew;
    double DemLeafAreaPlant;
    double CumSupplyTot;
    double RespMaintDebt;
    double LaiDead;
    double LeafDeathPop;
    double SDJCorPhase4;
    double CoeffStressLogging;
    double KcTot;
    double FractionRootsLogged;
    double CstrAssim;

    double SimStartMatu2;
    double SimAnthesis50;
    double SimPanIni;
    double SimStartPSP;
    double SimEmergence;
    double SimStartGermin;

    double StRuMax;
    double RootMassPerVol;
    double RootSystVolPop;
    double FloodWaterDepth;

    double CounterNursery;
    double ETP;
    double ProfRu;
    double PEvap;
    double VolRelMacropores;

    double SterilityDrought;
    double SterilityHeat;
    double SterilityCold;

    double FtswMoy;
    double TMaxMoy;
    double TMinMoy;

    double DegreDuJourCor;
    double SDJPhase4;

    double RootSystVolPopOld;
    double RootSystSoilSurfPopOld;
    double RootSystSoilSurfPop;
    double RootFrontOld;
    double PanStructMass;
    double GainRootSystVolPop;
    double GainRootSystSoilSurfPop;
    double FloodWaterGain;
    double DemStructRootPlant;
    double DemStructPaniclePlant;

    int ChangeSsPhase;
    double TempLai;
    double SeuilTempPhaseSuivante;
    double SommeDegresJour;
    double SommeDegresJourMaximale;
    double MaximumLai;
    double SommeDegresJourCor;
    double SommeDegresJourPhasePrecedente;
    double SeuilTempPhasePrec;
    double DureeDuJour;

    double SumPP;
    double Kcp;
    double SlaMitch;

    double PlantLeafNumTot;

    double TMoyCalc;

    double LatRad;
    double Lat;
    double TMoy;
    double HMin;
    double HMax;
    double HMoyCalc;
    double HMoy;
    double SunPosi;
    double Decli;
    double SunDistance;
    double RayExtra;
    double RgMax;
    double Alt;
    double RGLue;
    double RGCalc;
    double Ins;
    double KPar;
    double Rg;
    double HrMax;
    double HrMin;
    double HrMoy;
    double VPD;
    double RayGlobal;
    double Tlat;
    double Vent;
    double TMoyPrec;
    double ChangeSousPhase;
    double PourcRuSurfGermi;
    double StRu;
    double Kpsy;
    double SeuilTempSousPhaseSuivante;
    double NumSousPhase;
    double MonCompteur;
    double TrPot;
    double CoeffCO2Tr;
    double CO2SlopeTr;
    double ETM;
    double ETR;
    int NbJourCompte;
    int tabCstrIndiceCourant;
    double tabCstr[5];
    double SeuilCstrMortality;
};

} } } // namespace model models samara

#endif
