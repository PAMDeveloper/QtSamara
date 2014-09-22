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

#include <model/models/ModelParameters.hpp>
#include <model/models/meteo/Meteo.hpp>

#include <utils/DateTime.hpp>

namespace model { namespace models { namespace samara {

class Model
{
public:
    Model()
    { }

    virtual ~Model()
    { }

    void assignClimate(const model::models::meteo::Climate& climate);

    void compute(double time);

    void init(const model::models::ModelParameters& parameters);

    double lai() const
    { return Lai; }

private:
    void evalDegreeDay(double t);
    void phyllochron();
    void evalCstrAssim();
    void evalRespMaint();
    void evolPlantTilNumTot();
    void evolPlantLeafNumTot();
    void evalRelPotLeafLength();
    void evalDemandStructSheath();
    void evalDemandStructRoot();
    void evalDemandStructPanicle();
    void evolGrowthStructSheathPop();
    void evolGrowthStructRootPop();
    void evolGrowthStructPanPop();
    void evalSlaMitch();
    void evolKcpKceBilhy();
    void evolConsRes_Flood();
    void evalConversion();
    void evolPSPMVMD(double t);
    void evolSomDegresJourCor(double t);
    void evalMaximumLai();
    void evalVitesseRacinaire();
    void initCrop();
    void evalRootFront();
    void evalSDJPhase4();
    void evalDateGermination();
    void evalSterility();
    void initParcelle();
    void evalEvapPot();
    void transplanting();
    void transplantingShock();
    void evalRuiss_FloodDyna();
    void evolRempliResRFE_RDE();
    void plantSubmergence();
    void excessAssimilToRoot();
    void evolRempliMacropores();
    void evalFTSW();
    void evalDAF();
    void evalSimStartGermin();
    void evalSimEmergence();
    void evalSimStartPSP();
    void evalSimPanIni();
    void evalSimAnthesis50();
    void evalSimStartMatu2();
    void evalColdStress();
    void evalAssim();
    void evolWaterLoggingUpland();
    void evalStressWaterLogging();
    void evalCstrPFactorFAO();
    void evolHauteur_SDJ_cstr();
    void evalParIntercepte();
    void evolMobiliTillerDeath();
    void evolMobiliLeafDeath();
    void evalSupplyTot();
    void evalDemandStructLeaf();
    void evalDemandStructIN();
    void evalDemandTotAndIcPreFlow();
    void evolGrowthStructLeafPop();
    void evolGrowthStructINPop();
    void addResToGrowthStructPop();
    void evolDemPanFilPopAndIcPFlow();
    void evolPanicleFilPop();
    void evolGrowthReserveInternode();
    void evolGrowthTot();
    void evalLai();
    void evalClumpAndLightInter();
    void evalRUE();
    void evolEvapSurfRFE_RDE();
    void evolDryMatTot();
    void evolGrowthStructTot();
    void leafRolling();
    void evalAssimPot();
    void automaticIrrigation();
    void evolRurRFE_RDE();
    void evalSimEndCycle();
    void priority2GrowthPanStrctPop();
    void keyResults();
    void degToRad();
    void avgTempHum();
    void evalDecli(double t);
    void evalSunPosi();
    void evalDayLength();
    void evalSunDistance(double t);
    void evalRayExtra();
    void evalRgMax();
    void insToRg();
    void evalPAR();
    void etoFAO();
    void evolPhenoStress(double t);
    void demandePlante();
    void evalTranspi();
    void evalETRETM();
    void evolSomDegresJour();
    void mortality();

    void evalTMaxMoy();
    void evalTMinMoy();
    void evalFtswMoy();
    double calculeLaMoyenne(double laValeur, double leCompteur, double laMoyenne);

    // constantes
   static  double NullValue;

    // parameters
    double Altitude;
    double ASScstr;
    double AttenMitch;
    double BundHeight;
    double Ca;
    double CO2Cp;
    double CO2Exp;
    double CoeffAssimSla;
    double CoefficientQ10;
    double CoeffInternodeMass;
    double CoeffInternodeNum;
    double CoeffLeafDeath;
    double CoeffLeafWLRatio;
    double CoeffPanicleMass;
    double CoeffPanSinkPop;
    double CoeffResCapacityInternode;
    double CoeffReserveSink;
    double CoeffRootMassPerVolMax;
    double CoeffTillerDeath;
    double CoeffTransplantingShock;
    double DensityField;
    double DensityNursery;
    double DEVcstr;
    double DurationNursery;
    double EpaisseurProf;
    double EpaisseurSurf;
    double ExcessAssimToRoot;
    double FTSWIrrig;
    double HaunCritTillering;
    double HumFC;
    double HumPF;
    double HumSat;
    double IcTillering;
    double InternodeLengthMax;
    double IrrigAutoResume;
    double IrrigAutoStop;
    double IrrigAutoTarget;
    double KcMax;
    double KCritSterCold1;
    double KCritSterCold2;
    double KCritSterFtsw1;
    double KCritSterFtsw2;
    double KCritSterHeat1;
    double KCritSterHeat2;
    double KCritStressCold1;
    double KCritStressCold2;
    double Kdf;
    double KRespInternode;
    double KRespMaintLeaf;
    double KRespMaintRoot;
    double KRespMaintSheath;
    double KRespPanicle;
    double KTempMaint;
    double Latitude;
    double LeafLengthMax;
    double LifeSavingDrainage;
    double Mulch;
    double PanStructMassMax;
    double PercolationMax;
    double PFactor;
    double Phyllo;
    double PlantsPerHill;
    double PlotDrainageDAF;
    double PoidsSecGrain;
    double PourcRuiss;
    double PPCrit;
    double PPExp;
    double PriorityPan;
    double ProfRacIni;
    double RankLongestLeaf;
    double RelMobiliInternodeMax;
    double RollingBase;
    double RollingSens;
    double RootCstr;
    double RootFrontMax;
    double RootPartitMax;
    double SDJBVP;
    double SDJLevee;
    double SDJMatu1;
    double SDJMatu2;
    double SDJRPR;
    double SeuilPP;
    double SeuilRuiss;
    double SlaMax;
    double SlaMin;
    double StockIniProf;
    double StockIniSurf;
    double TBase;
    double TempSla;
    double TilAbility;
    double TLet;
    double TOpt1;
    double TOpt2;
    double TransplantingDepth;
    double TxAssimBVP;
    double TxAssimMatu1;
    double TxAssimMatu2;
    double TxConversion;
    double TxResGrain;
    double VRacBVP;
    double VRacLevee;
    double VRacMatu1;
    double VRacMatu2;
    double VRacPSP;
    double VRacRPR;
    double WaterLoggingSens;
    double WtRatioLeafSheath;
    int IrrigAuto;
    int Transplanting;

    // state variables
    double A_AssimSurplus;
    double A_DemStructLeaf;
    double A_DemStructTot;
    double A_GrowthStructLeaf;
    double A_GrowthStructTot;
    double A_IncreaseResInternodePop;
    double ApexHeight;
    double ApexHeightGain;
    double A_ResInternodeMobiliDay;
    double Assim;
    double AssimNotUsed;
    double AssimNotUsedCum;
    double AssimPot;
    double AssimSurplus;
    double CapaRDE;
    double CapaREvap;
    double CapaRFE;
    double ChangePhase;
    double ChangeSousPhase;
    double CO2SlopeTr;
    double CoeffCO2Assim;
    double CoeffCO2Tr;
    double CoeffStressLogging;
    double Conversion;
    double ConversionEff;
    double CounterNursery;
    double cstr;
    double CstrAssim;
    double CstrCumul;
    double CstrMean;
    double CstrPhase2;
    double CstrPhase3;
    double CstrPhase4;
    double CstrPhase5;
    double CstrPhase6;
    double CulmsPerHill;
    double CulmsPerHillFin;
    double CulmsPerHillMax;
    double CulmsPerPlant;
    double CulmsPerPlantFin;
    double CulmsPerPlantMax;
    double CulmsPop;
    double CumCarbonUsedPop;
    double CumCstrPhase2;
    double CumCstrPhase3;
    double CumCstrPhase4;
    double CumCstrPhase5;
    double CumCstrPhase6;
    double CumDr;
    double CumEt;
    double CumFtswPhase2;
    double CumFtswPhase3;
    double CumFtswPhase4;
    double CumFtswPhase5;
    double CumFtswPhase6;
    double CumGrowthPop;
    double CumIcPhase2;
    double CumIcPhase3;
    double CumIcPhase4;
    double CumIcPhase5;
    double CumIcPhase6;
    double CumIrrig;
    double CumIrrigFin;
    double CumLr;
    double CumPar;
    double CumSupplyTot;
    double CumTr;
    double CumWReceived;
    double CumWuse;
    double CumWUse;
    double CumWUseFin;
    double DAF;
    double DateSemis;
    double DayLength;
    double DeadLeafDrywtPop;
    double DeadLeafDryWtPop;
    double Decli;
    double DegreDuJourCor;
    double DegresDuJour;
    double DegresDuJourCor;
    double DemLeafAreaPlant;
    double DemPanicleFillPop;
    double DemResInternodePop;
    double DemStructInternodePlant;
    double DemStructInternodePop;
    double DemStructLeafPlant;
    double DemStructLeafPop;
    double DemStructPaniclePlant;
    double DemStructPaniclePop;
    double DemStructRootPlant;
    double DemStructRootPop;
    double DemStructSheathPop;
    double DemStructTotPop;
    double Density;
    double Dr;
    double DryMatAboveGroundPop;
    double DryMatAboveGroundPopFin;
    double DryMatAboveGroundTotPop;
    double DryMatPanicleTotPop;
    double DryMatResInternodePop;
    double DryMatResInternodePopOld;
    double DryMatStemPop;
    double DryMatStructInternodePop;
    double DryMatStructLeafPop;
    double DryMatStructPaniclePop;
    double DryMatStructRootPop;
    double DryMatStructSheathPop;
    double DryMatStructStemPop;
    double DryMatStructTotPop;
    double DryMatTotPop;
    double DryMatTotPopFin;
    double DryMatVegeTotPop;
    double DureeDuJour;
    double DurGermFlow;
    double DurGermMat;
    double DurPhase1;
    double DurPhase2;
    double DurPhase3;
    double DurPhase4;
    double DurPhase5;
    double DurPhase6;
    double EauDispo;
    double ETM;
    double Eto;
    double ETP;
    double ETR;
    double Evap;
    double EvapPot;
    double FertSpikeNumPop;
    double FertSpikePop;
    double FloodwaterDepth;
    double FloodWaterDepth;
    double FloodWaterGain;
    double FractionPlantHeightSubmer;
    double FractionRootsLogged;
    double FTSW;
    double FtswMoy;
    double FtswPhase2;
    double FtswPhase3;
    double FtswPhase4;
    double FtswPhase5;
    double FtswPhase6;
    double GainRootSystSoilSurfPop;
    double GainRootSystVolPop;
    double GrainFillingStatus;
    double GrainFillingStatusFin;
    double GrainYieldPanicle;
    double GrainYieldPop;
    double GrainYieldPopFin;
    double GrowthDryMatPop;
    double GrowthPop;
    double GrowthResInternodePop;
    double GrowthStructDeficit;
    double GrowthStructInternodePop;
    double GrowthStructLeafPop;
    double GrowthStructPaniclePop;
    double GrowthStructRootPop;
    double GrowthStructSheathPop;
    double GrowthStructTotPop;
    double HarvestIndex;
    double HaunGain;
    double HaunIndex;
    double HMax;
    double HMin;
    double HMoy;
    double HMoyCalc;
    double HrMax;
    double HrMin;
    double HrMoy;
    double Hum;
    double Ic;
    double IcCumul;
    double IcMean;
    double IcPhase2;
    double IcPhase3;
    double IcPhase4;
    double IcPhase5;
    double IcPhase6;
    double IncreaseResInternodePop;
    double Ins;
    double InternodeResStatus;
    double Irrigation;
    double IrrigAutoDay;
    double IrrigTotDay;
    double Kce;
    double KceReal; // parameter ????
    double Kcl;
    double Kcp;
    double KcTot;
    double KPar;
    double Kpsy;
    double Kr;
    double KRolling;
    double Lai;
    double LaiDead;
    double LaiFin;
    double LastLeafLength; // parameter ???
    double LastLeafLengthPot; // parameter ???
    double LatRad;
    double LeafDeathPop;
    double LIRkdf;
    double LIRkdfcl;
    double LIRKdfcl;
    double Lr;
    double LTRkdf;
    double LtrKdfcl;
    double LTRkdfcl;
    double MaximumLai;
    double MobiliLeafDeath;
    double MonCompteur;
    double NbDaysSinceGermination;
    double NbJas;
    double NumPhase;
    double NumSousPhase;
    double PanicleFilDeficit;
    double PanicleFilPop;
    double PanicleNumPlant;
    double PanicleNumPop;
    double PanicleSinkPop;
    double PanStructMass;
    double PAR;
    double PARIntercepte;
    double PEvap;
    double PhaseStemElongation;
    double PlantHeight;
    double PlantLeafNumNew;
    double PlantLeafNumTot;
    double PlantWidth;
    double PourcRuSurfGermi;
    double ProfRu;
    double Rain;
    double RayExtra;
    double RayGlobal;
    double RelPhylloPhaseStemElong;
    double RelPotLeafLength;
    double ResCapacityInternodePop;
    double ReservePopFin;
    double ResInternodeMobiliDay;
    double ResInternodeMobiliDayPot;
    double RespMaintDebt;
    double RespMaintTot;
    double ResUtil;
    double Rg;
    double RGCalc;
    double RGLue;
    double RgMax;
    double RootFront;
    double RootFrontOld;
    double RootMassPerVol;
    double RootShootRatio;
    double RootSystSoilSurfPop;
    double RootSystSoilSurfPopOld;
    double RootSystVolPop;
    double RootSystVolPopOld;
    double RUE;
    double RuRac;
    double RuSurf;
    double SDJCorPhase4;
    double SDJPhase4;
    double SeuilCstrMortality;
    double SeuilTempPhasePrec;
    double SeuilTempPhaseSuivante;
    double SeuilTempSousPhaseSuivante;
    double SimAnthesis50;
    double SimEmergence;
    double SimEndCycle;
    double SimPanIni;
    double SimStartGermin;
    double SimStartMatu2;
    double SimStartPSP;
    double Sla;
    double SlaMitch;
    double SlaNew;
    double SommeDegresJour;
    double SommeDegresJourCor;
    double SommeDegresJourMaximale;
    double SommeDegresJourPhasePrecedente;
    double SpikeNumPanicle;
    double SpikeNumPop;
    double SterilityCold;
    double SterilityDrought;
    double SterilityHeat;
    double SterilityTot;
    double SterilityTotFin;
    double StockMacropores;
    double StockRac;
    double StockSurface;
    double StockTotal;
    double StressCold;
    double StRu;
    double StRuMax;
    double SumPP;
    double SunDistance;
    double SunPosi;
    double SupplyTot;
    double tabCstr[5];
    double TempLai;
    double TillerDeathPop;
    double Tlat;
    double TMax;
    double TMaxMoy;
    double TMin;
    double TMinMoy;
    double TMoy;
    double TMoyCalc;
    double TMoyPrec;
    double Tr;
    double TrEff;
    double TrEffInst;
    double TrPot;
    double ValRDE;
    double ValRFE;
    double ValRSurf;
    double Vent;
    double VitesseRacinaire;
    double VitesseRacinaireDay;
    double VolMacropores;
    double VolRelMacropores;
    double VPD;
    double WueEt;
    double WueTot;
    int ChangeNurseryStatus;
    int ChangeSsPhase;
    int NbJourCompte;
    int NurseryStatus;
    int tabCstrIndiceCourant;
};

} } } // namespace model models samara

#endif
