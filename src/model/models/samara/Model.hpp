/**
 * @file model/models/samara/Model.hpp
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

#ifndef MODEL_MODELS_SAMARA_MODEL_HPP
#define MODEL_MODELS_SAMARA_MODEL_HPP

#include <model/kernel/AbstractAtomicModel.hpp>

namespace samara {

class Model : public samara::AbstractAtomicModel < Model >
{
public:
    enum internals { LAI, NUMPHASE, DEGRESDUJOUR, DEGRESDUJOURCOR, Ftsw,
                     CSTR, ROOTFRONT, ETO, Etm, Etr, Rue, CONVERSIONEFF, IC,
                     CULMSPERPLANT, CULMSPOP, CULMSPERHILL, GRAINYIELDPOP,
                     DRYMATSTRUCTLEAFPOP, DRYMATSTRUCTSHEATHPOP,
                     DRYMATSTRUCTROOTPOP, DRYMATSTRUCTINTERNODEPOP,
                     DRYMATRESINTERNODEPOP, DRYMATSTRUCTPANICLEPOP,
                     DRYMATSTRUCTTOTPOP, DRYMATVEGETOTPOP, DRYMATPANICLETOTPOP,
                     DRYMATSTEMPOP, DRYMATABOVEGROUNDPOP,
                     DRYMATTOTPOP, STERILITYCOLD, STERILITYHEAT,
                     STERILITYDROUGHT, STERILITYTOT, HARVESTINDEX,
                     PANICLENUMPOP, PANICLENUMPLANT, GRAINYIELDPANICLE,
                     SPIKENUMPOP, SPIKENUMPANICLE, FERTSPIKENUMPOP,
                     GRAINFILLINGSTATUS, PHASESTEMELONGATION, SLA,
                     HAUNINDEX, APEXHEIGHT, PLANTHEIGHT, PLANTWIDTH, KROLLING,
                     LIRKDFCL, LTRKDFCL, ASSIMPOT, ASSIM, RESPMAINTTOT,
                     SUPPLYTOT, ASSIMSURPLUS, ASSIMNOTUSED, TILLERDEATHPOP,
                     PLANTLEAFNUMTOT, DEADLEAFDRYWTPOP, LAIDEAD,
                     RESCAPACITYINTERNODEPOP, INTERNODERESSTATUS,
                     DAYLENGTH, Par, RGCALC, VPDCALC, TMOYCALC, HMOYCALC,
                     KCEREAL, STOCKTOTAL, EAUDISPO, STOCKSURFACE, STOCKRAC,
                     RURAC, KCP, KCE, EVAPPOT, EVAP, TRPOT, TR, LR, DR,
                     PARINTERCEPTE, SOMMEDEGRESJOUR, VITESSERACINAIRE,
                     CSTRASSIM, RAYEXTRA, CUMPAR, SOMMEDEGRESJOURCOR,
                     SUMPP, SOMMEDEGRESJOURPHASEPRECEDENTE, RESPMAINTDEBT,
                     TMINMOY, TMAXMOY, FTSWMOY, ROOTSHOOTRATIO, TREFFINST,
                     TREFF, WUEET, WUETOT, FLOODWATERDEPTH, IRRIGAUTODAY,
                     IRRIGTOTDAY, FRACTIONPLANTHEIGHTSUBMER, DENSITY,
                     ROOTMASSPERVOL, VOLMACROPORES, STOCKMACROPORES,
                     RELPOTLEAFLENGTH, APEXHEIGHTGAIN, HAUNGAIN, CONVERSION,
                     STRESSCOLD, FRACTIONROOTSLOGGED, RESUTIL, KCTOT, NBJAS };
    enum externals { TMAX, TMIN, TMOY, HMAX, HMIN, HMOY, WIND, INS, RG, Etp,
                     RAIN };

    Model(const samara::AbstractModel* parent) :
        AbstractAtomicModel < Model >(parent)
    {
        internal(LAI, &Model::Lai);
        internal(NUMPHASE, &Model::NumPhase);
        internal(DEGRESDUJOUR, &Model::DegresDuJour);
        internal(DEGRESDUJOURCOR, &Model::DegresDuJourCor);
        internal(Ftsw, &Model::FTSW);
        internal(CSTR, &Model::Cstr);
        internal(ROOTFRONT, &Model::RootFront);
        internal(ETO, &Model::Eto);
        internal(Etm, &Model::ETM);
        internal(Etr, &Model::ETR);
        internal(Rue, &Model::RUE);
        internal(CONVERSIONEFF, &Model::ConversionEff);
        internal(IC, &Model::Ic);
        internal(CULMSPERPLANT, &Model::CulmsPerPlant);
        internal(CULMSPOP, &Model::CulmsPop);
        internal(CULMSPERHILL, &Model::CulmsPerHill);
        internal(GRAINYIELDPOP, &Model::GrainYieldPop);
        internal(DRYMATSTRUCTLEAFPOP, &Model::DryMatStructLeafPop);
        internal(DRYMATSTRUCTSHEATHPOP, &Model::DryMatStructSheathPop);
        internal(DRYMATSTRUCTROOTPOP, &Model::DryMatStructRootPop);
        internal(DRYMATSTRUCTINTERNODEPOP, &Model::DryMatStructInternodePop);
        internal(DRYMATRESINTERNODEPOP, &Model::DryMatResInternodePop);
        internal(DRYMATSTRUCTPANICLEPOP, &Model::DryMatStructPaniclePop);
        internal(DRYMATSTRUCTTOTPOP, &Model::DryMatStructTotPop);
        internal(DRYMATVEGETOTPOP, &Model::DryMatVegeTotPop);
        internal(DRYMATPANICLETOTPOP, &Model::DryMatPanicleTotPop);
        internal(DRYMATSTEMPOP, &Model::DryMatStemPop);
        internal(DRYMATABOVEGROUNDPOP, &Model::DryMatAboveGroundPop);
        internal(DRYMATTOTPOP, &Model::DryMatTotPop);
        internal(STERILITYCOLD, &Model::SterilityCold);
        internal(STERILITYHEAT, &Model::SterilityHeat);
        internal(STERILITYDROUGHT, &Model::SterilityDrought);
        internal(STERILITYTOT, &Model::SterilityTot);
        internal(HARVESTINDEX, &Model::HarvestIndex);
        internal(PANICLENUMPOP, &Model::PanicleNumPop);
        internal(PANICLENUMPLANT, &Model::PanicleNumPlant);
        internal(GRAINYIELDPANICLE, &Model::GrainYieldPanicle);
        internal(SPIKENUMPOP, &Model::SpikeNumPop);
        internal(SPIKENUMPANICLE, &Model::SpikeNumPanicle);
        internal(FERTSPIKENUMPOP, &Model::FertSpikeNumPop);
        internal(GRAINFILLINGSTATUS, &Model::GrainFillingStatus);
        internal(PHASESTEMELONGATION, &Model::PhaseStemElongation);
        internal(SLA, &Model::Sla);
        internal(HAUNINDEX, &Model::HaunIndex);
        internal(APEXHEIGHT, &Model::ApexHeight);
        internal(PLANTHEIGHT, &Model::PlantHeight);
        internal(PLANTWIDTH, &Model::PlantWidth);
        internal(KROLLING, &Model::KRolling);
        internal(LIRKDFCL, &Model::LIRkdfcl);
        internal(LTRKDFCL, &Model::LTRkdfcl);
        internal(ASSIMPOT, &Model::AssimPot);
        internal(ASSIM, &Model::Assim);
        internal(RESPMAINTTOT, &Model::RespMaintTot);
        internal(SUPPLYTOT, &Model::SupplyTot);
        internal(ASSIMSURPLUS, &Model::AssimSurplus);
        internal(ASSIMNOTUSED, &Model::AssimNotUsed);
        internal(TILLERDEATHPOP, &Model::TillerDeathPop);
        internal(PLANTLEAFNUMTOT, &Model::PlantLeafNumTot);
        internal(DEADLEAFDRYWTPOP, &Model::DeadLeafDrywtPop);
        internal(LAIDEAD, &Model::LaiDead);
        internal(RESCAPACITYINTERNODEPOP, &Model::ResCapacityInternodePop);
        internal(INTERNODERESSTATUS, &Model::InternodeResStatus);
        internal(DAYLENGTH, &Model::DayLength);
        internal(Par, &Model::PAR);
        internal(RGCALC, &Model::RgCalc);
        internal(VPDCALC, &Model::VPD);
        internal(TMOYCALC, &Model::TMoyCalc);
        internal(HMOYCALC, &Model::HMoyCalc);
        internal(KCEREAL, &Model::KceReal);
        internal(STOCKTOTAL, &Model::StockTotal);
        internal(EAUDISPO, &Model::EauDispo);
        internal(STOCKSURFACE, &Model::StockSurface);
        internal(STOCKRAC, &Model::StockRac);
        internal(RURAC, &Model::RuRac);
        internal(KCP, &Model::Kcp);
        internal(KCE, &Model::Kce);
        internal(EVAPPOT, &Model::EvapPot);
        internal(EVAP, &Model::Evap);
        internal(TRPOT, &Model::TrPot);
        internal(TR, &Model::Tr);
        internal(LR, &Model::Lr);
        internal(DR, &Model::Dr);
        internal(PARINTERCEPTE, &Model::PARIntercepte);
        internal(SOMMEDEGRESJOUR, &Model::SommeDegresJour);
        internal(VITESSERACINAIRE, &Model::VitesseRacinaire);
        internal(CSTRASSIM, &Model::CstrAssim);
        internal(RAYEXTRA, &Model::RayExtra);
        internal(CUMPAR, &Model::CumPar);
        internal(SOMMEDEGRESJOURCOR, &Model::SommeDegresJourCor);
        internal(SUMPP, &Model::SumPP);
        internal(SOMMEDEGRESJOURPHASEPRECEDENTE,
                 &Model::SommeDegresJourPhasePrecedente);
        internal(RESPMAINTDEBT, &Model::RespMaintDebt);
        internal(TMINMOY, &Model::TMinMoy);
        internal(TMAXMOY, &Model::TMaxMoy);
        internal(FTSWMOY, &Model::FtswMoy);
        internal(ROOTSHOOTRATIO, &Model::RootShootRatio);
        internal(TREFFINST, &Model::TrEffInst);
        internal(TREFF, &Model::TrEff);
        internal(WUEET, &Model::WueEt);
        internal(WUETOT, &Model::WueTot);
        internal(FLOODWATERDEPTH, &Model::FloodwaterDepth);
        internal(IRRIGAUTODAY, &Model::IrrigAutoDay);
        internal(IRRIGTOTDAY, &Model::IrrigTotDay);
        internal(FRACTIONPLANTHEIGHTSUBMER, &Model::FractionPlantHeightSubmer);
        internal(DENSITY, &Model::Density);
        internal(ROOTMASSPERVOL, &Model::RootMassPerVol);
        internal(VOLMACROPORES, &Model::VolMacropores);
        internal(STOCKMACROPORES, &Model::StockMacropores);
        internal(RELPOTLEAFLENGTH, &Model::RelPotLeafLength);
        internal(APEXHEIGHTGAIN, &Model::ApexHeightGain);
        internal(HAUNGAIN, &Model::HaunGain);
        internal(CONVERSION, &Model::Conversion);
        internal(STRESSCOLD, &Model::StressCold);
        internal(FRACTIONROOTSLOGGED, &Model::FractionRootsLogged);
        internal(RESUTIL, &Model::ResUtil);
        internal(KCTOT, &Model::KcTot);
        internal(NBJAS, &Model::NbJas);

        external(TMAX, &Model::TMax);
        external(TMIN, &Model::TMin);
        external(TMOY, &Model::TMoy);
        external(HMAX, &Model::HMax);
        external(HMIN, &Model::HMin);
        external(HMOY, &Model::HMoy);
        external(WIND, &Model::Vent);
        external(INS, &Model::Ins);
        external(RG, &Model::Rg);
        external(Etp, &Model::ETP);
        external(RAIN, &Model::Rain);
    }

    virtual ~Model()
    { }

    void init(double t, const model::models::ModelParameters& parameters);

    double NullValue;

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
    double TxRuSurfGermi;
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
    double Cstr;
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
    double Decli;
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
    double Lr;
    double LTRkdf;
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
    double PPSens;
    double ProfRu;
    double Rain;
    double RayExtra;
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
    double RgCalc;
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
    double RUEgreen;
    double RuRac;
    double RuSurf;
    double SDJCorPhase4;
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

protected:
    void initCrop();
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
    void evalTranspi();
    void evalETRETM();
    void evalConversion();
    void evolSomDegresJour();
    void mortality();
    void evalSimAnthesis50();
    void evalDateGermination();
    void evalColdStress();
    void evalSimEmergence();
    void evalSimPanIni();
    void evalSimStartGermin();
    void evalSimStartMatu2();
    void evalSimStartPSP();
    void evalSDJPhase4();
    void phyllochron();
    void evolKcpKceBilhy();
    void evalEvapPot();
    void evalTMaxMoy();
    void evalTMinMoy();
    void evalFtswMoy();
    void evalSterility();
    void evalVitesseRacinaire();
    void evalCstrAssim();
    void evalAssim();
    void evalRespMaint();
    void evalRelPotLeafLength();
    void evolPlantLeafNumTot();
    void evalDemandStructSheath();
    void evolGrowthStructSheathPop();
    void evolGrowthStructRootPop();
    void evolGrowthStructPanPop();
    void evalMaximumLai();
    void evalSlaMitch();
    void evalRootFront();
    void evolPSPMVMD(double t);
    void evolSomDegresJourCor();
    void evalNbJas(double t);

    double calculeLaMoyenne(double laValeur, double leCompteur,
                            double laMoyenne);

};

} // namespace samara

#endif
