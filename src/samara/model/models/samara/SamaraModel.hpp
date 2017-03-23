/**
 * @file samara/model/models/samara/Model.hpp
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

#include <samara/model/kernel/AbstractAtomicModel.hpp>

namespace samara {

class SamaraModel : public samara::AbstractAtomicModel < SamaraModel >
{
public:
    void writeResultInFile(const std::string idsimu,double jour);
    void writeResultInDB(const std::string idsimu,double jour);

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

    SamaraModel(const samara::AbstractModel* parent) :
        AbstractAtomicModel < SamaraModel >(parent)
    {
        internal(LAI, &SamaraModel::Lai);
        internal(NUMPHASE, &SamaraModel::NumPhase);
        internal(DEGRESDUJOUR, &SamaraModel::DegresDuJour);
        internal(DEGRESDUJOURCOR, &SamaraModel::DegresDuJourCor);
        internal(Ftsw, &SamaraModel::FTSW);
        internal(CSTR, &SamaraModel::Cstr);
        internal(ROOTFRONT, &SamaraModel::RootFront);
        internal(ETO, &SamaraModel::Eto);
        internal(Etm, &SamaraModel::ETM);
        internal(Etr, &SamaraModel::ETR);
        internal(Rue, &SamaraModel::RUE);
        internal(CONVERSIONEFF, &SamaraModel::ConversionEff);
        internal(IC, &SamaraModel::Ic);
        internal(CULMSPERPLANT, &SamaraModel::CulmsPerPlant);
        internal(CULMSPOP, &SamaraModel::CulmsPop);
        internal(CULMSPERHILL, &SamaraModel::CulmsPerHill);
        internal(GRAINYIELDPOP, &SamaraModel::GrainYieldPop);
        internal(DRYMATSTRUCTLEAFPOP, &SamaraModel::DryMatStructLeafPop);
        internal(DRYMATSTRUCTSHEATHPOP, &SamaraModel::DryMatStructSheathPop);
        internal(DRYMATSTRUCTROOTPOP, &SamaraModel::DryMatStructRootPop);
        internal(DRYMATSTRUCTINTERNODEPOP, &SamaraModel::DryMatStructInternodePop);
        internal(DRYMATRESINTERNODEPOP, &SamaraModel::DryMatResInternodePop);
        internal(DRYMATSTRUCTPANICLEPOP, &SamaraModel::DryMatStructPaniclePop);
        internal(DRYMATSTRUCTTOTPOP, &SamaraModel::DryMatStructTotPop);
        internal(DRYMATVEGETOTPOP, &SamaraModel::DryMatVegeTotPop);
        internal(DRYMATPANICLETOTPOP, &SamaraModel::DryMatPanicleTotPop);
        internal(DRYMATSTEMPOP, &SamaraModel::DryMatStemPop);
        internal(DRYMATABOVEGROUNDPOP, &SamaraModel::DryMatAboveGroundPop);
        internal(DRYMATTOTPOP, &SamaraModel::DryMatTotPop);
        internal(STERILITYCOLD, &SamaraModel::SterilityCold);
        internal(STERILITYHEAT, &SamaraModel::SterilityHeat);
        internal(STERILITYDROUGHT, &SamaraModel::SterilityDrought);
        internal(STERILITYTOT, &SamaraModel::SterilityTot);
        internal(HARVESTINDEX, &SamaraModel::HarvestIndex);
        internal(PANICLENUMPOP, &SamaraModel::PanicleNumPop);
        internal(PANICLENUMPLANT, &SamaraModel::PanicleNumPlant);
        internal(GRAINYIELDPANICLE, &SamaraModel::GrainYieldPanicle);
        internal(SPIKENUMPOP, &SamaraModel::SpikeNumPop);
        internal(SPIKENUMPANICLE, &SamaraModel::SpikeNumPanicle);
        internal(FERTSPIKENUMPOP, &SamaraModel::FertSpikeNumPop);
        internal(GRAINFILLINGSTATUS, &SamaraModel::GrainFillingStatus);
        internal(PHASESTEMELONGATION, &SamaraModel::PhaseStemElongation);
        internal(SLA, &SamaraModel::Sla);
        internal(HAUNINDEX, &SamaraModel::HaunIndex);
        internal(APEXHEIGHT, &SamaraModel::ApexHeight);
        internal(PLANTHEIGHT, &SamaraModel::PlantHeight);
        internal(PLANTWIDTH, &SamaraModel::PlantWidth);
        internal(KROLLING, &SamaraModel::KRolling);
        internal(LIRKDFCL, &SamaraModel::LIRkdfcl);
        internal(LTRKDFCL, &SamaraModel::LTRkdfcl);
        internal(ASSIMPOT, &SamaraModel::AssimPot);
        internal(ASSIM, &SamaraModel::Assim);
        internal(RESPMAINTTOT, &SamaraModel::RespMaintTot);
        internal(SUPPLYTOT, &SamaraModel::SupplyTot);
        internal(ASSIMSURPLUS, &SamaraModel::AssimSurplus);
        internal(ASSIMNOTUSED, &SamaraModel::AssimNotUsed);
        internal(TILLERDEATHPOP, &SamaraModel::TillerDeathPop);
        internal(PLANTLEAFNUMTOT, &SamaraModel::PlantLeafNumTot);
        internal(DEADLEAFDRYWTPOP, &SamaraModel::DeadLeafDrywtPop);
        internal(LAIDEAD, &SamaraModel::LaiDead);
        internal(RESCAPACITYINTERNODEPOP, &SamaraModel::ResCapacityInternodePop);
        internal(INTERNODERESSTATUS, &SamaraModel::InternodeResStatus);
        internal(DAYLENGTH, &SamaraModel::DayLength);
        internal(Par, &SamaraModel::PAR);
        internal(RGCALC, &SamaraModel::RgCalc);
        internal(VPDCALC, &SamaraModel::VPD);
        internal(TMOYCALC, &SamaraModel::TMoyCalc);
        internal(HMOYCALC, &SamaraModel::HMoyCalc);
        internal(KCEREAL, &SamaraModel::KceReal);
        internal(STOCKTOTAL, &SamaraModel::StockTotal);
        internal(EAUDISPO, &SamaraModel::EauDispo);
        internal(STOCKSURFACE, &SamaraModel::StockSurface);
        internal(STOCKRAC, &SamaraModel::StockRac);
        internal(RURAC, &SamaraModel::RuRac);
        internal(KCP, &SamaraModel::Kcp);
        internal(KCE, &SamaraModel::Kce);
        internal(EVAPPOT, &SamaraModel::EvapPot);
        internal(EVAP, &SamaraModel::Evap);
        internal(TRPOT, &SamaraModel::TrPot);
        internal(TR, &SamaraModel::Tr);
        internal(LR, &SamaraModel::Lr);
        internal(DR, &SamaraModel::Dr);
        internal(PARINTERCEPTE, &SamaraModel::PARIntercepte);
        internal(SOMMEDEGRESJOUR, &SamaraModel::SommeDegresJour);
        internal(VITESSERACINAIRE, &SamaraModel::VitesseRacinaire);
        internal(CSTRASSIM, &SamaraModel::CstrAssim);
        internal(RAYEXTRA, &SamaraModel::RayExtra);
        internal(CUMPAR, &SamaraModel::CumPar);
        internal(SOMMEDEGRESJOURCOR, &SamaraModel::SommeDegresJourCor);
        internal(SUMPP, &SamaraModel::SumPP);
        internal(SOMMEDEGRESJOURPHASEPRECEDENTE,
                 &SamaraModel::SommeDegresJourPhasePrecedente);
        internal(RESPMAINTDEBT, &SamaraModel::RespMaintDebt);
        internal(TMINMOY, &SamaraModel::TMinMoy);
        internal(TMAXMOY, &SamaraModel::TMaxMoy);
        internal(FTSWMOY, &SamaraModel::FtswMoy);
        internal(ROOTSHOOTRATIO, &SamaraModel::RootShootRatio);
        internal(TREFFINST, &SamaraModel::TrEffInst);
        internal(TREFF, &SamaraModel::TrEff);
        internal(WUEET, &SamaraModel::WueEt);
        internal(WUETOT, &SamaraModel::WueTot);
        internal(FLOODWATERDEPTH, &SamaraModel::FloodwaterDepth);
        internal(IRRIGAUTODAY, &SamaraModel::IrrigAutoDay);
        internal(IRRIGTOTDAY, &SamaraModel::IrrigTotDay);
        internal(FRACTIONPLANTHEIGHTSUBMER, &SamaraModel::FractionPlantHeightSubmer);
        internal(DENSITY, &SamaraModel::Density);
        internal(ROOTMASSPERVOL, &SamaraModel::RootMassPerVol);
        internal(VOLMACROPORES, &SamaraModel::VolMacropores);
        internal(STOCKMACROPORES, &SamaraModel::StockMacropores);
        internal(RELPOTLEAFLENGTH, &SamaraModel::RelPotLeafLength);
        internal(APEXHEIGHTGAIN, &SamaraModel::ApexHeightGain);
        internal(HAUNGAIN, &SamaraModel::HaunGain);
        internal(CONVERSION, &SamaraModel::Conversion);
        internal(STRESSCOLD, &SamaraModel::StressCold);
        internal(FRACTIONROOTSLOGGED, &SamaraModel::FractionRootsLogged);
        internal(RESUTIL, &SamaraModel::ResUtil);
        internal(KCTOT, &SamaraModel::KcTot);
        internal(NBJAS, &SamaraModel::NbJas);

        external(TMAX, &SamaraModel::TMax);
        external(TMIN, &SamaraModel::TMin);
        external(TMOY, &SamaraModel::TMoy);
        external(HMAX, &SamaraModel::HMax);
        external(HMIN, &SamaraModel::HMin);
        external(HMOY, &SamaraModel::HMoy);
        external(WIND, &SamaraModel::Vent);
        external(INS, &SamaraModel::Ins);
        external(RG, &SamaraModel::Rg);
        external(Etp, &SamaraModel::ETP);
        external(RAIN, &SamaraModel::Rain);
    }

    virtual ~SamaraModel()
    { }

    void init(double t, const model::models::ModelParameters& parameters);

    double NullValue = 0;

    // parameters
    double Altitude=0;
    double ASScstr=0;
    double AttenMitch=0;
    double BundHeight=0;
    double Ca=0;
    double CO2Cp=0;
    double CO2Exp=0;
    double CoeffAssimSla=0;
    double CoefficientQ10=0;
    double CoeffInternodeMass=0;
    double CoeffInternodeNum=0;
    double CoeffLeafDeath=0;
    double CoeffLeafWLRatio=0;
    double CoeffPanicleMass=0;
    double CoeffPanSinkPop=0;
    double CoeffResCapacityInternode=0;
    double CoeffReserveSink=0;
    double CoeffRootMassPerVolMax=0;
    double CoeffTillerDeath=0;
    double CoeffTransplantingShock=0;
    double DensityField=0;
    double DensityNursery=0;
    double DEVcstr=0;
    double DurationNursery=0;
    double EpaisseurProf=0;
    double EpaisseurSurf=0;
    double ExcessAssimToRoot=0;
    double FTSWIrrig=0;
    double HaunCritTillering=0;
    double HumFC=0;
    double HumPF=0;
    double HumSat=0;
    double IcTillering=0;
    double InternodeLengthMax=0;
    double IrrigAutoResume=0;
    double IrrigAutoStop=0;
    double IrrigAutoTarget=0;
    double KcMax=0;
    double KCritSterCold1=0;
    double KCritSterCold2=0;
    double KCritSterFtsw1=0;
    double KCritSterFtsw2=0;
    double KCritSterHeat1=0;
    double KCritSterHeat2=0;
    double KCritStressCold1=0;
    double KCritStressCold2=0;
    double Kdf=0;
    double KRespInternode=0;
    double KRespMaintLeaf=0;
    double KRespMaintRoot=0;
    double KRespMaintSheath=0;
    double KRespPanicle=0;
    double KTempMaint=0;
    double Latitude=0;
    double LeafLengthMax=0;
    double LifeSavingDrainage=0;
    double Mulch=0;
    double PanStructMassMax=0;
    double PercolationMax=0;
    double PFactor=0;
    double Phyllo=0;
    double PlantsPerHill=0;
    double PlotDrainageDAF=0;
    double PoidsSecGrain=0;
    double PourcRuiss=0;
    double PPCrit=0;
    double PPExp=0;
    double PriorityPan=0;
    double ProfRacIni=0;
    double RankLongestLeaf=0;
    double RelMobiliInternodeMax=0;
    double RollingBase=0;
    double RollingSens=0;
    double RootCstr=0;
    double RootFrontMax=0;
    double RootPartitMax=0;
    double SDJBVP=0;
    double SDJLevee=0;
    double SDJMatu1=0;
    double SDJMatu2=0;
    double SDJRPR=0;
    double SeuilPP=0;
    double SeuilRuiss=0;
    double SlaMax=0;
    double SlaMin=0;
    double StockIniProf=0;
    double StockIniSurf=0;
    double TBase=0;
    double TempSla=0;
    double TilAbility=0;
    double TLet=0;
    double TOpt1=0;
    double TOpt2=0;
    double TransplantingDepth=0;
    double TxAssimBVP=0;
    double TxAssimMatu1=0;
    double TxAssimMatu2=0;
    double TxConversion=0;
    double TxResGrain=0;
    double TxRuSurfGermi=0;
    double VRacBVP=0;
    double VRacLevee=0;
    double VRacMatu1=0;
    double VRacMatu2=0;
    double VRacPSP=0;
    double VRacRPR=0;
    double WaterLoggingSens=0;
    double WtRatioLeafSheath=0;
    int IrrigAuto=0;
    int Transplanting=0;

    // state variables
    double A_AssimSurplus=0;
    double A_DemStructLeaf=0;
    double A_DemStructTot=0;
    double A_GrowthStructLeaf=0;
    double A_GrowthStructTot=0;
    double A_IncreaseResInternodePop=0;
    double ApexHeight=0;
    double ApexHeightGain=0;
    double A_ResInternodeMobiliDay=0;
    double Assim=0;
    double AssimNotUsed=0;
    double AssimNotUsedCum=0;
    double AssimPot=0;
    double AssimSurplus=0;
    double CapaRDE=0;
    double CapaREvap=0;
    double CapaRFE=0;
    double ChangePhase=0;
    double ChangeSousPhase=0;
    double CO2SlopeTr=0;
    double CoeffCO2Assim=0;
    double CoeffCO2Tr=0;
    double CoeffStressLogging=0;
    double Conversion=0;
    double ConversionEff=0;
    double CounterNursery=0;
    double Cstr=0;
    double CstrAssim=0;
    double CstrCumul=0;
    double CstrMean=0;
    double CstrPhase2=0;
    double CstrPhase3=0;
    double CstrPhase4=0;
    double CstrPhase5=0;
    double CstrPhase6=0;
    double CulmsPerHill=0;
    double CulmsPerHillFin=0;
    double CulmsPerHillMax=0;
    double CulmsPerPlant=0;
    double CulmsPerPlantFin=0;
    double CulmsPerPlantMax=0;
    double CulmsPop=0;
    double CumCarbonUsedPop=0;
    double CumCstrPhase2=0;
    double CumCstrPhase3=0;
    double CumCstrPhase4=0;
    double CumCstrPhase5=0;
    double CumCstrPhase6=0;
    double CumDr=0;
    double CumEt=0;
    double CumFtswPhase2=0;
    double CumFtswPhase3=0;
    double CumFtswPhase4=0;
    double CumFtswPhase5=0;
    double CumFtswPhase6=0;
    double CumGrowthPop=0;
    double CumIcPhase2=0;
    double CumIcPhase3=0;
    double CumIcPhase4=0;
    double CumIcPhase5=0;
    double CumIcPhase6=0;
    double CumIrrig=0;
    double CumIrrigFin=0;
    double CumLr=0;
    double CumPar=0;
    double CumSupplyTot=0;
    double CumTr=0;
    double CumWReceived=0;
    double CumWuse=0;
    double CumWUse=0;
    double CumWUseFin=0;
    double DAF=0;
    double DateSemis=0;
    double DayLength=0;
    double DeadLeafDrywtPop=0;
    double Decli=0;
    double DegresDuJour=0;
    double DegresDuJourCor=0;
    double DemLeafAreaPlant=0;
    double DemPanicleFillPop=0;
    double DemResInternodePop=0;
    double DemStructInternodePlant=0;
    double DemStructInternodePop=0;
    double DemStructLeafPlant=0;
    double DemStructLeafPop=0;
    double DemStructPaniclePlant=0;
    double DemStructPaniclePop=0;
    double DemStructRootPlant=0;
    double DemStructRootPop=0;
    double DemStructSheathPop=0;
    double DemStructTotPop=0;
    double Density=0;
    double Dr=0;
    double DryMatAboveGroundPop=0;
    double DryMatAboveGroundPopFin=0;
    double DryMatAboveGroundTotPop=0;
    double DryMatPanicleTotPop=0;
    double DryMatResInternodePop=0;
    double DryMatResInternodePopOld=0;
    double DryMatStemPop=0;
    double DryMatStructInternodePop=0;
    double DryMatStructLeafPop=0;
    double DryMatStructPaniclePop=0;
    double DryMatStructRootPop=0;
    double DryMatStructSheathPop=0;
    double DryMatStructStemPop=0;
    double DryMatStructTotPop=0;
    double DryMatTotPop=0;
    double DryMatTotPopFin=0;
    double DryMatVegeTotPop=0;
    double DureeDuJour=0;
    double DurGermFlow=0;
    double DurGermMat=0;
    double DurPhase1=0;
    double DurPhase2=0;
    double DurPhase3=0;
    double DurPhase4=0;
    double DurPhase5=0;
    double DurPhase6=0;
    double EauDispo=0;
    double ETM=0;
    double Eto=0;
    double ETP=0;
    double ETR=0;
    double Evap=0;
    double EvapPot=0;
    double FertSpikeNumPop=0;
    double FertSpikePop=0;
    double FloodwaterDepth=0;
    double FloodWaterGain=0;
    double FractionPlantHeightSubmer=0;
    double FractionRootsLogged=0;
    double FTSW=0;
    double FtswMoy=0;
    double FtswPhase2=0;
    double FtswPhase3=0;
    double FtswPhase4=0;
    double FtswPhase5=0;
    double FtswPhase6=0;
    double GainRootSystSoilSurfPop=0;
    double GainRootSystVolPop=0;
    double GrainFillingStatus=0;
    double GrainFillingStatusFin=0;
    double GrainYieldPanicle=0;
    double GrainYieldPop=0;
    double GrainYieldPopFin=0;
    double GrowthDryMatPop=0;
    double GrowthPop=0;
    double GrowthResInternodePop=0;
    double GrowthStructDeficit=0;
    double GrowthStructInternodePop=0;
    double GrowthStructLeafPop=0;
    double GrowthStructPaniclePop=0;
    double GrowthStructRootPop=0;
    double GrowthStructSheathPop=0;
    double GrowthStructTotPop=0;
    double HarvestIndex=0;
    double HaunGain=0;
    double HaunIndex=0;
    double HMax=0;
    double HMin=0;
    double HMoy=0;
    double HMoyCalc=0;
    double Hum=0;
    double Ic=0;
    double IcCumul=0;
    double IcMean=0;
    double IcPhase2=0;
    double IcPhase3=0;
    double IcPhase4=0;
    double IcPhase5=0;
    double IcPhase6=0;
    double IncreaseResInternodePop=0;
    double Ins=0;
    double InternodeResStatus=0;
    double Irrigation=0;
    double IrrigAutoDay=0;
    double IrrigTotDay=0;
    double Kce=0;
    double KceReal=0; // parameter ????
    double Kcl=0;
    double Kcp=0;
    double KcTot=0;
    double KPar=0;
    double Kr=0;
    double KRolling=0;
    double Lai=0;
    double LaiDead=0;
    double LaiFin=0;
    double LastLeafLength=0; // parameter ???
    double LastLeafLengthPot=0; // parameter ???
    double LatRad=0;
    double LeafDeathPop=0;
    double LIRkdf=0;
    double LIRkdfcl=0;
    double Lr=0;
    double LTRkdf=0;
    double LTRkdfcl=0;
    double MaximumLai=0;
    double MobiliLeafDeath=0;
    double MonCompteur=0;
    double NbDaysSinceGermination=0;
    double NbJas=0;
    double NumPhase=0;
    double NumSousPhase=0;
    double PanicleFilDeficit=0;
    double PanicleFilPop=0;
    double PanicleNumPlant=0;
    double PanicleNumPop=0;
    double PanicleSinkPop=0;
    double PanStructMass=0;
    double PAR=0;
    double PARIntercepte=0;
    double PEvap=0;
    double PhaseStemElongation=0;
    double PlantHeight=0;
    double PlantLeafNumNew=0;
    double PlantLeafNumTot=0;
    double PlantWidth=0;
    double PPSens=0;
    double ProfRu=0;
    double Rain=0;
    double RayExtra=0;
    double RelPhylloPhaseStemElong=0;
    double RelPotLeafLength=0;
    double ResCapacityInternodePop=0;
    double ReservePopFin=0;
    double ResInternodeMobiliDay=0;
    double ResInternodeMobiliDayPot=0;
    double RespMaintDebt=0;
    double RespMaintTot=0;
    double ResUtil=0;
    double Rg=0;
    double RgCalc=0;
    double RgMax=0;
    double RootFront=0;
    double RootFrontOld=0;
    double RootMassPerVol=0;
    double RootShootRatio=0;
    double RootSystSoilSurfPop=0;
    double RootSystSoilSurfPopOld=0;
    double RootSystVolPop=0;
    double RootSystVolPopOld=0;
    double RUE=0;
    double RUEgreen=0;
    double RuRac=0;
    double RuSurf=0;
    double SDJCorPhase4=0;
    double SeuilCstrMortality=0;
    double SeuilTempPhasePrec=0;
    double SeuilTempPhaseSuivante=0;
    double SeuilTempSousPhaseSuivante=0;
    double SimAnthesis50=0;
    double SimEmergence=0;
    double SimEndCycle=0;
    double SimPanIni=0;
    double SimStartGermin=0;
    double SimStartMatu2=0;
    double SimStartPSP=0;
    double Sla=0;
    double SlaMitch=0;
    double SlaNew=0;
    double SommeDegresJour=0;
    double SommeDegresJourCor=0;
    double SommeDegresJourMaximale=0;
    double SommeDegresJourPhasePrecedente=0;
    double SpikeNumPanicle=0;
    double SpikeNumPop=0;
    double SterilityCold=0;
    double SterilityDrought=0;
    double SterilityHeat=0;
    double SterilityTot=0;
    double SterilityTotFin=0;
    double StockMacropores=0;
    double StockRac=0;
    double StockSurface=0;
    double StockTotal=0;
    double StressCold=0;
    double StRu=0;
    double StRuMax=0;
    double SumPP=0;
    double SunDistance=0;
    double SunPosi=0;
    double SupplyTot=0;
    double tabCstr[5]={0,0,0,0,0};
    double TempLai=0;
    double TillerDeathPop=0;
    double TMax=0;
    double TMaxMoy=0;
    double TMin=0;
    double TMinMoy=0;
    double TMoy=0;
    double TMoyCalc=0;
    double TMoyPrec=0;
    double Tr=0;
    double TrEff=0;
    double TrEffInst=0;
    double TrPot=0;
    double ValRDE=0;
    double ValRFE=0;
    double ValRSurf=0;
    double Vent=0;
    double VitesseRacinaire=0;
    double VitesseRacinaireDay=0;
    double VolMacropores=0;
    double VolRelMacropores=0;
    double VPD=0;
    double WueEt=0;
    double WueTot=0;
    int ChangeNurseryStatus=0;
    int ChangeSsPhase=0;
    int NbJourCompte=0;
    int NurseryStatus=0;
    int tabCstrIndiceCourant=0;

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
