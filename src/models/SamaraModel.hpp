/**
 * @file models/Model.hpp
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

#include <defines.hpp>

class SamaraModel : public CoupledModel < SamaraModel > {
public:
  void writeResultInFile(const std::string idsimu, double jour);
  void writeResultInDB(const std::string idsimu, double jour);

  enum internals {A_ASSIMSURPLUS,A_DEMSTRUCTLEAF,A_DEMSTRUCTTOT,A_GROWTHSTRUCTLEAF,
                  A_GROWTHSTRUCTTOT,A_INCREASERESINTERNODEPOP,APEXHEIGHT,APEXHEIGHTGAIN,
                  A_RESINTERNODEMOBILIDAY,ASSIM,ASSIMNOTUSED,ASSIMNOTUSEDCUM,ASSIMPOT,
                  ASSIMSURPLUS,CAPARDE,CAPAREVAP,CAPARFE,CHANGEPHASE,CHANGESOUSPHASE,
                  CO2SLOPETR,COEFFCO2ASSIM,COEFFCO2TR,COEFFSTRESSLOGGING,CONVERSION,
                  CONVERSIONEFF,COUNTERNURSERY,CSTR,CSTRASSIM,CSTRCUMUL,CSTRMEAN,
                  CSTRPHASE2,CSTRPHASE3,CSTRPHASE4,CSTRPHASE5,CSTRPHASE6,CULMSPERHILL,
                  CULMSPERHILLFIN,CULMSPERHILLMAX,CULMSPERPLANT,CULMSPERPLANTFIN,
                  CULMSPERPLANTMAX,CULMSPOP,CUMCARBONUSEDPOP,CUMCSTRPHASE2,CUMCSTRPHASE3,
                  CUMCSTRPHASE4,CUMCSTRPHASE5,CUMCSTRPHASE6,CUMDR,CUMET,CUMFTSWPHASE2,
                  CUMFTSWPHASE3,CUMFTSWPHASE4,CUMFTSWPHASE5,CUMFTSWPHASE6,CUMGROWTHPOP,
                  CUMICPHASE2,CUMICPHASE3,CUMICPHASE4,CUMICPHASE5,CUMICPHASE6,CUMIRRIG,
                  CUMIRRIGFIN,CUMLR,CUMPAR,CUMSUPPLYTOT,CUMTR,CUMWRECEIVED,CUMWUSE,
                  CUMWUSEFIN,_DAF,DATESEMIS,DAYLENGTH,DEADLEAFDRYWTPOP,DECLI,DEGRESDUJOUR,
                  DEGRESDUJOURCOR,DEMLEAFAREAPLANT,DEMPANICLEFILLPOP,DEMRESINTERNODEPOP,
                  DEMSTRUCTINTERNODEPLANT,DEMSTRUCTINTERNODEPOP,DEMSTRUCTLEAFPLANT,
                  DEMSTRUCTLEAFPOP,DEMSTRUCTPANICLEPLANT,DEMSTRUCTPANICLEPOP,DEMSTRUCTROOTPLANT,
                  DEMSTRUCTROOTPOP,DEMSTRUCTSHEATHPOP,DEMSTRUCTTOTPOP,DENSITY,DR,
                  DRYMATABOVEGROUNDPOP,DRYMATABOVEGROUNDPOPFIN,DRYMATABOVEGROUNDTOTPOP,
                  DRYMATPANICLETOTPOP,DRYMATRESINTERNODEPOP,DRYMATRESINTERNODEPOPOLD,
                  DRYMATSTEMPOP,DRYMATSTRUCTINTERNODEPOP,DRYMATSTRUCTLEAFPOP,DRYMATSTRUCTPANICLEPOP,
                  DRYMATSTRUCTROOTPOP,DRYMATSTRUCTSHEATHPOP,DRYMATSTRUCTSTEMPOP,DRYMATSTRUCTTOTPOP,
                  DRYMATTOTPOP,DRYMATTOTPOPFIN,DRYMATVEGETOTPOP,DUREEDUJOUR,DURGERMFLOW,DURGERMMAT,
                  DURPHASE1,DURPHASE2,DURPHASE3,DURPHASE4,DURPHASE5,DURPHASE6,EAUDISPO,_ETM,ETO,_ETP,
                  _ETR,EVAP,EVAPPOT,FERTSPIKENUMPOP,FERTSPIKEPOP,FLOODWATERDEPTH,FLOODWATERGAIN,
                  FRACTIONPLANTHEIGHTSUBMER,FRACTIONROOTSLOGGED,_FTSW,FTSWMOY,FTSWPHASE2,FTSWPHASE3,
                  FTSWPHASE4,FTSWPHASE5,FTSWPHASE6,GAINROOTSYSTSOILSURFPOP,GAINROOTSYSTVOLPOP,
                  GRAINFILLINGSTATUS,GRAINFILLINGSTATUSFIN,GRAINYIELDPANICLE,GRAINYIELDPOP,
                  GRAINYIELDPOPFIN,GROWTHDRYMATPOP,GROWTHPOP,GROWTHRESINTERNODEPOP,GROWTHSTRUCTDEFICIT,
                  GROWTHSTRUCTINTERNODEPOP,GROWTHSTRUCTLEAFPOP,GROWTHSTRUCTPANICLEPOP,
                  GROWTHSTRUCTROOTPOP,GROWTHSTRUCTSHEATHPOP,GROWTHSTRUCTTOTPOP,HARVESTINDEX,
                  HAUNGAIN,HAUNINDEX,HMAX,HMIN,HMOY,HMOYCALC,HUM,IC,ICCUMUL,ICMEAN,ICPHASE2,
                  ICPHASE3,ICPHASE4,ICPHASE5,ICPHASE6,INCREASERESINTERNODEPOP,INS,INTERNODERESSTATUS,
                  IRRIGATION,IRRIGAUTODAY,IRRIGTOTDAY,KCE,KCEREAL,KCL,KCP,KCTOT,KPAR,KR,KROLLING,LAI,
                  LAIDEAD,LAIFIN,LASTLEAFLENGTH,LASTLEAFLENGTHPOT,LATRAD,LEAFDEATHPOP,LIRKDF,LIRKDFCL,
                  LR,LTRKDF,LTRKDFCL,MAXIMUMLAI,MOBILILEAFDEATH,MONCOMPTEUR,NBDAYSSINCEGERMINATION,
                  NBJAS,NUMPHASE,NUMSOUSPHASE,PANICLEFILDEFICIT,PANICLEFILPOP,PANICLENUMPLANT,
                  PANICLENUMPOP,PANICLESINKPOP,PANSTRUCTMASS,_PAR,PARINTERCEPTE,PEVAP,PHASESTEMELONGATION,
                  PLANTHEIGHT,PLANTLEAFNUMNEW,PLANTLEAFNUMTOT,PLANTWIDTH,PPSENS,PROFRU,RAIN,RAYEXTRA,
                  RELPHYLLOPHASESTEMELONG,RELPOTLEAFLENGTH,RESCAPACITYINTERNODEPOP,RESERVEPOPFIN,
                  RESINTERNODEMOBILIDAY,RESINTERNODEMOBILIDAYPOT,RESPMAINTDEBT,RESPMAINTTOT,RESUTIL,
                  RG,RGCALC,RGMAX,ROOTFRONT,ROOTFRONTOLD,ROOTMASSPERVOL,ROOTSHOOTRATIO,ROOTSYSTSOILSURFPOP,
                  ROOTSYSTSOILSURFPOPOLD,ROOTSYSTVOLPOP,ROOTSYSTVOLPOPOLD,_RUE,RUEGREEN,RURAC,RUSURF,
                  SDJCORPHASE4,SEUILCSTRMORTALITY,SEUILTEMPPHASEPREC,SEUILTEMPPHASESUIVANTE,
                  SEUILTEMPSOUSPHASESUIVANTE,SIMANTHESIS50,SIMEMERGENCE,SIMENDCYCLE,SIMPANINI,
                  SIMSTARTGERMIN,SIMSTARTMATU2,SIMSTARTPSP,SLA,SLAMITCH,SLANEW,SOMMEDEGRESJOUR,
                  SOMMEDEGRESJOURCOR,SOMMEDEGRESJOURMAXIMALE,SOMMEDEGRESJOURPHASEPRECEDENTE,
                  SPIKENUMPANICLE,SPIKENUMPOP,STERILITYCOLD,STERILITYDROUGHT,STERILITYHEAT,STERILITYTOT,
                  STERILITYTOTFIN,STOCKMACROPORES,STOCKRAC,STOCKSURFACE,STOCKTOTAL,STRESSCOLD,STRU,
                  STRUMAX,SUMPP,SUNDISTANCE,SUNPOSI,SUPPLYTOT,TEMPLAI,TILLERDEATHPOP,TMAX,TMAXMOY,
                  TMIN,TMINMOY,TMOY,TMOYCALC,TMOYPREC,TR,TREFF,TREFFINST,TRPOT,VALRDE,VALRFE,VALRSURF,
                  VENT,VITESSERACINAIRE,VITESSERACINAIREDAY,VOLMACROPORES,VOLRELMACROPORES,_VPD,WUEET,
                  WUETOT,CHANGENURSERYSTATUS,CHANGESSPHASE,NBJOURCOMPTE,NURSERYSTATUS,TABCSTRINDICECOURANT};

//  enum internals { LAI, NUMPHASE, DEGRESDUJOUR, DEGRESDUJOURCOR, Ftsw,
//                   CSTR, ROOTFRONT, ETO, Etm, Etr, Rue, CONVERSIONEFF, IC,
//                   CULMSPERPLANT, CULMSPOP, CULMSPERHILL, GRAINYIELDPOP,
//                   DRYMATSTRUCTLEAFPOP, DRYMATSTRUCTSHEATHPOP,
//                   DRYMATSTRUCTROOTPOP, DRYMATSTRUCTINTERNODEPOP,
//                   DRYMATRESINTERNODEPOP, DRYMATSTRUCTPANICLEPOP,
//                   DRYMATSTRUCTTOTPOP, DRYMATVEGETOTPOP, DRYMATPANICLETOTPOP,
//                   DRYMATSTEMPOP, DRYMATABOVEGROUNDPOP,
//                   DRYMATTOTPOP, STERILITYCOLD, STERILITYHEAT,
//                   STERILITYDROUGHT, STERILITYTOT, HARVESTINDEX,
//                   PANICLENUMPOP, PANICLENUMPLANT, GRAINYIELDPANICLE,
//                   SPIKENUMPOP, SPIKENUMPANICLE, FERTSPIKENUMPOP,
//                   GRAINFILLINGSTATUS, PHASESTEMELONGATION, SLA,
//                   HAUNINDEX, APEXHEIGHT, PLANTHEIGHT, PLANTWIDTH, KROLLING,
//                   LIRKDFCL, LTRKDFCL, ASSIMPOT, ASSIM, RESPMAINTTOT,
//                   SUPPLYTOT, ASSIMSURPLUS, ASSIMNOTUSED, TILLERDEATHPOP,
//                   PLANTLEAFNUMTOT, DEADLEAFDRYWTPOP, LAIDEAD,
//                   RESCAPACITYINTERNODEPOP, INTERNODERESSTATUS,
//                   DAYLENGTH, Par, RGCALC, VPDCALC, TMOYCALC, HMOYCALC,
//                   KCEREAL, STOCKTOTAL, EAUDISPO, STOCKSURFACE, STOCKRAC,
//                   RURAC, KCP, KCE, EVAPPOT, EVAP, TRPOT, TR, LR, DR,
//                   PARINTERCEPTE, SOMMEDEGRESJOUR, VITESSERACINAIRE,
//                   CSTRASSIM, RAYEXTRA, CUMPAR, SOMMEDEGRESJOURCOR,
//                   SUMPP, SOMMEDEGRESJOURPHASEPRECEDENTE, RESPMAINTDEBT,
//                   TMINMOY, TMAXMOY, FTSWMOY, ROOTSHOOTRATIO, TREFFINST,
//                   TREFF, WUEET, WUETOT, FLOODWATERDEPTH, IRRIGAUTODAY,
//                   IRRIGTOTDAY, FRACTIONPLANTHEIGHTSUBMER, DENSITY,
//                   ROOTMASSPERVOL, VOLMACROPORES, STOCKMACROPORES,
//                   RELPOTLEAFLENGTH, APEXHEIGHTGAIN, HAUNGAIN, CONVERSION,
//                   STRESSCOLD, FRACTIONROOTSLOGGED, RESUTIL, KCTOT, NBJAS
//                 };
//  enum externals { TMAX, TMIN, TMOY, HMAX, HMIN, HMOY, WIND, INS, RG, Etp,
//                   RAIN
//                 };

  SamaraModel() :
    CoupledModel < SamaraModel >() {

      Internal(A_ASSIMSURPLUS, &SamaraModel::A_AssimSurplus);
      Internal(A_DEMSTRUCTLEAF, &SamaraModel::A_DemStructLeaf);
      Internal(A_DEMSTRUCTTOT, &SamaraModel::A_DemStructTot);
      Internal(A_GROWTHSTRUCTLEAF, &SamaraModel::A_GrowthStructLeaf);
      Internal(A_GROWTHSTRUCTTOT, &SamaraModel::A_GrowthStructTot);
      Internal(A_INCREASERESINTERNODEPOP, &SamaraModel::A_IncreaseResInternodePop);
      Internal(APEXHEIGHT, &SamaraModel::ApexHeight);
      Internal(APEXHEIGHTGAIN, &SamaraModel::ApexHeightGain);
      Internal(A_RESINTERNODEMOBILIDAY, &SamaraModel::A_ResInternodeMobiliDay);
      Internal(ASSIM, &SamaraModel::Assim);
      Internal(ASSIMNOTUSED, &SamaraModel::AssimNotUsed);
      Internal(ASSIMNOTUSEDCUM, &SamaraModel::AssimNotUsedCum);
      Internal(ASSIMPOT, &SamaraModel::AssimPot);
      Internal(ASSIMSURPLUS, &SamaraModel::AssimSurplus);
      Internal(CAPARDE, &SamaraModel::CapaRDE);
      Internal(CAPAREVAP, &SamaraModel::CapaREvap);
      Internal(CAPARFE, &SamaraModel::CapaRFE);
      Internal(CHANGEPHASE, &SamaraModel::ChangePhase);
      Internal(CHANGESOUSPHASE, &SamaraModel::ChangeSousPhase);
      Internal(CO2SLOPETR, &SamaraModel::CO2SlopeTr);
      Internal(COEFFCO2ASSIM, &SamaraModel::CoeffCO2Assim);
      Internal(COEFFCO2TR, &SamaraModel::CoeffCO2Tr);
      Internal(COEFFSTRESSLOGGING, &SamaraModel::CoeffStressLogging);
      Internal(CONVERSION, &SamaraModel::Conversion);
      Internal(CONVERSIONEFF, &SamaraModel::ConversionEff);
      Internal(COUNTERNURSERY, &SamaraModel::CounterNursery);
      Internal(CSTR, &SamaraModel::Cstr);
      Internal(CSTRASSIM, &SamaraModel::CstrAssim);
      Internal(CSTRCUMUL, &SamaraModel::CstrCumul);
      Internal(CSTRMEAN, &SamaraModel::CstrMean);
      Internal(CSTRPHASE2, &SamaraModel::CstrPhase2);
      Internal(CSTRPHASE3, &SamaraModel::CstrPhase3);
      Internal(CSTRPHASE4, &SamaraModel::CstrPhase4);
      Internal(CSTRPHASE5, &SamaraModel::CstrPhase5);
      Internal(CSTRPHASE6, &SamaraModel::CstrPhase6);
      Internal(CULMSPERHILL, &SamaraModel::CulmsPerHill);
      Internal(CULMSPERHILLFIN, &SamaraModel::CulmsPerHillFin);
      Internal(CULMSPERHILLMAX, &SamaraModel::CulmsPerHillMax);
      Internal(CULMSPERPLANT, &SamaraModel::CulmsPerPlant);
      Internal(CULMSPERPLANTFIN, &SamaraModel::CulmsPerPlantFin);
      Internal(CULMSPERPLANTMAX, &SamaraModel::CulmsPerPlantMax);
      Internal(CULMSPOP, &SamaraModel::CulmsPop);
      Internal(CUMCARBONUSEDPOP, &SamaraModel::CumCarbonUsedPop);
      Internal(CUMCSTRPHASE2, &SamaraModel::CumCstrPhase2);
      Internal(CUMCSTRPHASE3, &SamaraModel::CumCstrPhase3);
      Internal(CUMCSTRPHASE4, &SamaraModel::CumCstrPhase4);
      Internal(CUMCSTRPHASE5, &SamaraModel::CumCstrPhase5);
      Internal(CUMCSTRPHASE6, &SamaraModel::CumCstrPhase6);
      Internal(CUMDR, &SamaraModel::CumDr);
      Internal(CUMET, &SamaraModel::CumEt);
      Internal(CUMFTSWPHASE2, &SamaraModel::CumFtswPhase2);
      Internal(CUMFTSWPHASE3, &SamaraModel::CumFtswPhase3);
      Internal(CUMFTSWPHASE4, &SamaraModel::CumFtswPhase4);
      Internal(CUMFTSWPHASE5, &SamaraModel::CumFtswPhase5);
      Internal(CUMFTSWPHASE6, &SamaraModel::CumFtswPhase6);
      Internal(CUMGROWTHPOP, &SamaraModel::CumGrowthPop);
      Internal(CUMICPHASE2, &SamaraModel::CumIcPhase2);
      Internal(CUMICPHASE3, &SamaraModel::CumIcPhase3);
      Internal(CUMICPHASE4, &SamaraModel::CumIcPhase4);
      Internal(CUMICPHASE5, &SamaraModel::CumIcPhase5);
      Internal(CUMICPHASE6, &SamaraModel::CumIcPhase6);
      Internal(CUMIRRIG, &SamaraModel::CumIrrig);
      Internal(CUMIRRIGFIN, &SamaraModel::CumIrrigFin);
      Internal(CUMLR, &SamaraModel::CumLr);
      Internal(CUMPAR, &SamaraModel::CumPar);
      Internal(CUMSUPPLYTOT, &SamaraModel::CumSupplyTot);
      Internal(CUMTR, &SamaraModel::CumTr);
      Internal(CUMWRECEIVED, &SamaraModel::CumWReceived);
      Internal(CUMWUSE, &SamaraModel::CumWUse);
      Internal(CUMWUSEFIN, &SamaraModel::CumWUseFin);
      Internal(_DAF, &SamaraModel::DAF);
      Internal(DATESEMIS, &SamaraModel::DateSemis);
      Internal(DAYLENGTH, &SamaraModel::DayLength);
      Internal(DEADLEAFDRYWTPOP, &SamaraModel::DeadLeafDrywtPop);
      Internal(DECLI, &SamaraModel::Decli);
      Internal(DEGRESDUJOUR, &SamaraModel::DegresDuJour);
      Internal(DEGRESDUJOURCOR, &SamaraModel::DegresDuJourCor);
      Internal(DEMLEAFAREAPLANT, &SamaraModel::DemLeafAreaPlant);
      Internal(DEMPANICLEFILLPOP, &SamaraModel::DemPanicleFillPop);
      Internal(DEMRESINTERNODEPOP, &SamaraModel::DemResInternodePop);
      Internal(DEMSTRUCTINTERNODEPLANT, &SamaraModel::DemStructInternodePlant);
      Internal(DEMSTRUCTINTERNODEPOP, &SamaraModel::DemStructInternodePop);
      Internal(DEMSTRUCTLEAFPLANT, &SamaraModel::DemStructLeafPlant);
      Internal(DEMSTRUCTLEAFPOP, &SamaraModel::DemStructLeafPop);
      Internal(DEMSTRUCTPANICLEPLANT, &SamaraModel::DemStructPaniclePlant);
      Internal(DEMSTRUCTPANICLEPOP, &SamaraModel::DemStructPaniclePop);
      Internal(DEMSTRUCTROOTPLANT, &SamaraModel::DemStructRootPlant);
      Internal(DEMSTRUCTROOTPOP, &SamaraModel::DemStructRootPop);
      Internal(DEMSTRUCTSHEATHPOP, &SamaraModel::DemStructSheathPop);
      Internal(DEMSTRUCTTOTPOP, &SamaraModel::DemStructTotPop);
      Internal(DENSITY, &SamaraModel::Density);
      Internal(DR, &SamaraModel::Dr);
      Internal(DRYMATABOVEGROUNDPOP, &SamaraModel::DryMatAboveGroundPop);
      Internal(DRYMATABOVEGROUNDPOPFIN, &SamaraModel::DryMatAboveGroundPopFin);
      Internal(DRYMATABOVEGROUNDTOTPOP, &SamaraModel::DryMatAboveGroundTotPop);
      Internal(DRYMATPANICLETOTPOP, &SamaraModel::DryMatPanicleTotPop);
      Internal(DRYMATRESINTERNODEPOP, &SamaraModel::DryMatResInternodePop);
      Internal(DRYMATRESINTERNODEPOPOLD, &SamaraModel::DryMatResInternodePopOld);
      Internal(DRYMATSTEMPOP, &SamaraModel::DryMatStemPop);
      Internal(DRYMATSTRUCTINTERNODEPOP, &SamaraModel::DryMatStructInternodePop);
      Internal(DRYMATSTRUCTLEAFPOP, &SamaraModel::DryMatStructLeafPop);
      Internal(DRYMATSTRUCTPANICLEPOP, &SamaraModel::DryMatStructPaniclePop);
      Internal(DRYMATSTRUCTROOTPOP, &SamaraModel::DryMatStructRootPop);
      Internal(DRYMATSTRUCTSHEATHPOP, &SamaraModel::DryMatStructSheathPop);
      Internal(DRYMATSTRUCTSTEMPOP, &SamaraModel::DryMatStructStemPop);
      Internal(DRYMATSTRUCTTOTPOP, &SamaraModel::DryMatStructTotPop);
      Internal(DRYMATTOTPOP, &SamaraModel::DryMatTotPop);
      Internal(DRYMATTOTPOPFIN, &SamaraModel::DryMatTotPopFin);
      Internal(DRYMATVEGETOTPOP, &SamaraModel::DryMatVegeTotPop);
      Internal(DUREEDUJOUR, &SamaraModel::DureeDuJour);
      Internal(DURGERMFLOW, &SamaraModel::DurGermFlow);
      Internal(DURGERMMAT, &SamaraModel::DurGermMat);
      Internal(DURPHASE1, &SamaraModel::DurPhase1);
      Internal(DURPHASE2, &SamaraModel::DurPhase2);
      Internal(DURPHASE3, &SamaraModel::DurPhase3);
      Internal(DURPHASE4, &SamaraModel::DurPhase4);
      Internal(DURPHASE5, &SamaraModel::DurPhase5);
      Internal(DURPHASE6, &SamaraModel::DurPhase6);
      Internal(EAUDISPO, &SamaraModel::EauDispo);
      Internal(_ETM, &SamaraModel::ETM);
      Internal(ETO, &SamaraModel::Eto);
      Internal(_ETP, &SamaraModel::ETP);
      Internal(_ETR, &SamaraModel::ETR);
      Internal(EVAP, &SamaraModel::Evap);
      Internal(EVAPPOT, &SamaraModel::EvapPot);
      Internal(FERTSPIKENUMPOP, &SamaraModel::FertSpikeNumPop);
      Internal(FERTSPIKEPOP, &SamaraModel::FertSpikePop);
      Internal(FLOODWATERDEPTH, &SamaraModel::FloodwaterDepth);
      Internal(FLOODWATERGAIN, &SamaraModel::FloodWaterGain);
      Internal(FRACTIONPLANTHEIGHTSUBMER, &SamaraModel::FractionPlantHeightSubmer);
      Internal(FRACTIONROOTSLOGGED, &SamaraModel::FractionRootsLogged);
      Internal(_FTSW, &SamaraModel::FTSW);
      Internal(FTSWMOY, &SamaraModel::FtswMoy);
      Internal(FTSWPHASE2, &SamaraModel::FtswPhase2);
      Internal(FTSWPHASE3, &SamaraModel::FtswPhase3);
      Internal(FTSWPHASE4, &SamaraModel::FtswPhase4);
      Internal(FTSWPHASE5, &SamaraModel::FtswPhase5);
      Internal(FTSWPHASE6, &SamaraModel::FtswPhase6);
      Internal(GAINROOTSYSTSOILSURFPOP, &SamaraModel::GainRootSystSoilSurfPop);
      Internal(GAINROOTSYSTVOLPOP, &SamaraModel::GainRootSystVolPop);
      Internal(GRAINFILLINGSTATUS, &SamaraModel::GrainFillingStatus);
      Internal(GRAINFILLINGSTATUSFIN, &SamaraModel::GrainFillingStatusFin);
      Internal(GRAINYIELDPANICLE, &SamaraModel::GrainYieldPanicle);
      Internal(GRAINYIELDPOP, &SamaraModel::GrainYieldPop);
      Internal(GRAINYIELDPOPFIN, &SamaraModel::GrainYieldPopFin);
      Internal(GROWTHDRYMATPOP, &SamaraModel::GrowthDryMatPop);
      Internal(GROWTHPOP, &SamaraModel::GrowthPop);
      Internal(GROWTHRESINTERNODEPOP, &SamaraModel::GrowthResInternodePop);
      Internal(GROWTHSTRUCTDEFICIT, &SamaraModel::GrowthStructDeficit);
      Internal(GROWTHSTRUCTINTERNODEPOP, &SamaraModel::GrowthStructInternodePop);
      Internal(GROWTHSTRUCTLEAFPOP, &SamaraModel::GrowthStructLeafPop);
      Internal(GROWTHSTRUCTPANICLEPOP, &SamaraModel::GrowthStructPaniclePop);
      Internal(GROWTHSTRUCTROOTPOP, &SamaraModel::GrowthStructRootPop);
      Internal(GROWTHSTRUCTSHEATHPOP, &SamaraModel::GrowthStructSheathPop);
      Internal(GROWTHSTRUCTTOTPOP, &SamaraModel::GrowthStructTotPop);
      Internal(HARVESTINDEX, &SamaraModel::HarvestIndex);
      Internal(HAUNGAIN, &SamaraModel::HaunGain);
      Internal(HAUNINDEX, &SamaraModel::HaunIndex);
      Internal(HMAX, &SamaraModel::HMax);
      Internal(HMIN, &SamaraModel::HMin);
      Internal(HMOY, &SamaraModel::HMoy);
      Internal(HMOYCALC, &SamaraModel::HMoyCalc);
      Internal(HUM, &SamaraModel::Hum);
      Internal(IC, &SamaraModel::Ic);
      Internal(ICCUMUL, &SamaraModel::IcCumul);
      Internal(ICMEAN, &SamaraModel::IcMean);
      Internal(ICPHASE2, &SamaraModel::IcPhase2);
      Internal(ICPHASE3, &SamaraModel::IcPhase3);
      Internal(ICPHASE4, &SamaraModel::IcPhase4);
      Internal(ICPHASE5, &SamaraModel::IcPhase5);
      Internal(ICPHASE6, &SamaraModel::IcPhase6);
      Internal(INCREASERESINTERNODEPOP, &SamaraModel::IncreaseResInternodePop);
      Internal(INS, &SamaraModel::Ins);
      Internal(INTERNODERESSTATUS, &SamaraModel::InternodeResStatus);
      Internal(IRRIGATION, &SamaraModel::Irrigation);
      Internal(IRRIGAUTODAY, &SamaraModel::IrrigAutoDay);
      Internal(IRRIGTOTDAY, &SamaraModel::IrrigTotDay);
      Internal(KCE, &SamaraModel::Kce);
      Internal(KCL, &SamaraModel::Kcl);
      Internal(KCP, &SamaraModel::Kcp);
      Internal(KCTOT, &SamaraModel::KcTot);
      Internal(KPAR, &SamaraModel::KPar);
      Internal(KR, &SamaraModel::Kr);
      Internal(KROLLING, &SamaraModel::KRolling);
      Internal(LAI, &SamaraModel::Lai);
      Internal(LAIDEAD, &SamaraModel::LaiDead);
      Internal(LAIFIN, &SamaraModel::LaiFin);
      Internal(LATRAD, &SamaraModel::LatRad);
      Internal(LEAFDEATHPOP, &SamaraModel::LeafDeathPop);
      Internal(LIRKDF, &SamaraModel::LIRkdf);
      Internal(LIRKDFCL, &SamaraModel::LIRkdfcl);
      Internal(LR, &SamaraModel::Lr);
      Internal(LTRKDF, &SamaraModel::LTRkdf);
      Internal(LTRKDFCL, &SamaraModel::LTRkdfcl);
      Internal(MAXIMUMLAI, &SamaraModel::MaximumLai);
      Internal(MOBILILEAFDEATH, &SamaraModel::MobiliLeafDeath);
      Internal(MONCOMPTEUR, &SamaraModel::MonCompteur);
      Internal(NBDAYSSINCEGERMINATION, &SamaraModel::NbDaysSinceGermination);
      Internal(NBJAS, &SamaraModel::NbJas);
      Internal(NUMPHASE, &SamaraModel::NumPhase);
      Internal(NUMSOUSPHASE, &SamaraModel::NumSousPhase);
      Internal(PANICLEFILDEFICIT, &SamaraModel::PanicleFilDeficit);
      Internal(PANICLEFILPOP, &SamaraModel::PanicleFilPop);
      Internal(PANICLENUMPLANT, &SamaraModel::PanicleNumPlant);
      Internal(PANICLENUMPOP, &SamaraModel::PanicleNumPop);
      Internal(PANICLESINKPOP, &SamaraModel::PanicleSinkPop);
      Internal(PANSTRUCTMASS, &SamaraModel::PanStructMass);
      Internal(_PAR, &SamaraModel::PAR);
      Internal(PARINTERCEPTE, &SamaraModel::PARIntercepte);
      Internal(PEVAP, &SamaraModel::PEvap);
      Internal(PHASESTEMELONGATION, &SamaraModel::PhaseStemElongation);
      Internal(PLANTHEIGHT, &SamaraModel::PlantHeight);
      Internal(PLANTLEAFNUMNEW, &SamaraModel::PlantLeafNumNew);
      Internal(PLANTLEAFNUMTOT, &SamaraModel::PlantLeafNumTot);
      Internal(PLANTWIDTH, &SamaraModel::PlantWidth);
      Internal(PPSENS, &SamaraModel::PPSens);
      Internal(PROFRU, &SamaraModel::ProfRu);
      Internal(RAIN, &SamaraModel::Rain);
      Internal(RAYEXTRA, &SamaraModel::RayExtra);
      Internal(RELPHYLLOPHASESTEMELONG, &SamaraModel::RelPhylloPhaseStemElong);
      Internal(RELPOTLEAFLENGTH, &SamaraModel::RelPotLeafLength);
      Internal(RESCAPACITYINTERNODEPOP, &SamaraModel::ResCapacityInternodePop);
      Internal(RESERVEPOPFIN, &SamaraModel::ReservePopFin);
      Internal(RESINTERNODEMOBILIDAY, &SamaraModel::ResInternodeMobiliDay);
      Internal(RESINTERNODEMOBILIDAYPOT, &SamaraModel::ResInternodeMobiliDayPot);
      Internal(RESPMAINTDEBT, &SamaraModel::RespMaintDebt);
      Internal(RESPMAINTTOT, &SamaraModel::RespMaintTot);
      Internal(RESUTIL, &SamaraModel::ResUtil);
      Internal(RG, &SamaraModel::Rg);
      Internal(RGCALC, &SamaraModel::RgCalc);
      Internal(RGMAX, &SamaraModel::RgMax);
      Internal(ROOTFRONT, &SamaraModel::RootFront);
      Internal(ROOTFRONTOLD, &SamaraModel::RootFrontOld);
      Internal(ROOTMASSPERVOL, &SamaraModel::RootMassPerVol);
      Internal(ROOTSHOOTRATIO, &SamaraModel::RootShootRatio);
      Internal(ROOTSYSTSOILSURFPOP, &SamaraModel::RootSystSoilSurfPop);
      Internal(ROOTSYSTSOILSURFPOPOLD, &SamaraModel::RootSystSoilSurfPopOld);
      Internal(ROOTSYSTVOLPOP, &SamaraModel::RootSystVolPop);
      Internal(ROOTSYSTVOLPOPOLD, &SamaraModel::RootSystVolPopOld);
      Internal(_RUE, &SamaraModel::RUE);
      Internal(RUEGREEN, &SamaraModel::RUEgreen);
      Internal(RURAC, &SamaraModel::RuRac);
      Internal(RUSURF, &SamaraModel::RuSurf);
      Internal(SDJCORPHASE4, &SamaraModel::SDJCorPhase4);
      Internal(SEUILCSTRMORTALITY, &SamaraModel::SeuilCstrMortality);
      Internal(SEUILTEMPPHASEPREC, &SamaraModel::SeuilTempPhasePrec);
      Internal(SEUILTEMPPHASESUIVANTE, &SamaraModel::SeuilTempPhaseSuivante);
      Internal(SEUILTEMPSOUSPHASESUIVANTE, &SamaraModel::SeuilTempSousPhaseSuivante);
      Internal(SIMANTHESIS50, &SamaraModel::SimAnthesis50);
      Internal(SIMEMERGENCE, &SamaraModel::SimEmergence);
      Internal(SIMENDCYCLE, &SamaraModel::SimEndCycle);
      Internal(SIMPANINI, &SamaraModel::SimPanIni);
      Internal(SIMSTARTGERMIN, &SamaraModel::SimStartGermin);
      Internal(SIMSTARTMATU2, &SamaraModel::SimStartMatu2);
      Internal(SIMSTARTPSP, &SamaraModel::SimStartPSP);
      Internal(SLA, &SamaraModel::Sla);
      Internal(SLAMITCH, &SamaraModel::SlaMitch);
      Internal(SLANEW, &SamaraModel::SlaNew);
      Internal(SOMMEDEGRESJOUR, &SamaraModel::SommeDegresJour);
      Internal(SOMMEDEGRESJOURCOR, &SamaraModel::SommeDegresJourCor);
      Internal(SOMMEDEGRESJOURMAXIMALE, &SamaraModel::SommeDegresJourMaximale);
      Internal(SOMMEDEGRESJOURPHASEPRECEDENTE, &SamaraModel::SommeDegresJourPhasePrecedente);
      Internal(SPIKENUMPANICLE, &SamaraModel::SpikeNumPanicle);
      Internal(SPIKENUMPOP, &SamaraModel::SpikeNumPop);
      Internal(STERILITYCOLD, &SamaraModel::SterilityCold);
      Internal(STERILITYDROUGHT, &SamaraModel::SterilityDrought);
      Internal(STERILITYHEAT, &SamaraModel::SterilityHeat);
      Internal(STERILITYTOT, &SamaraModel::SterilityTot);
      Internal(STERILITYTOTFIN, &SamaraModel::SterilityTotFin);
      Internal(STOCKMACROPORES, &SamaraModel::StockMacropores);
      Internal(STOCKRAC, &SamaraModel::StockRac);
      Internal(STOCKSURFACE, &SamaraModel::StockSurface);
      Internal(STOCKTOTAL, &SamaraModel::StockTotal);
      Internal(STRESSCOLD, &SamaraModel::StressCold);
      Internal(STRU, &SamaraModel::StRu);
      Internal(STRUMAX, &SamaraModel::StRuMax);
      Internal(SUMPP, &SamaraModel::SumPP);
      Internal(SUNDISTANCE, &SamaraModel::SunDistance);
      Internal(SUNPOSI, &SamaraModel::SunPosi);
      Internal(SUPPLYTOT, &SamaraModel::SupplyTot);
      Internal(TEMPLAI, &SamaraModel::TempLai);
      Internal(TILLERDEATHPOP, &SamaraModel::TillerDeathPop);
      Internal(TMAX, &SamaraModel::TMax);
      Internal(TMAXMOY, &SamaraModel::TMaxMoy);
      Internal(TMIN, &SamaraModel::TMin);
      Internal(TMINMOY, &SamaraModel::TMinMoy);
      Internal(TMOY, &SamaraModel::TMoy);
      Internal(TMOYCALC, &SamaraModel::TMoyCalc);
      Internal(TMOYPREC, &SamaraModel::TMoyPrec);
      Internal(TR, &SamaraModel::Tr);
      Internal(TREFF, &SamaraModel::TrEff);
      Internal(TREFFINST, &SamaraModel::TrEffInst);
      Internal(TRPOT, &SamaraModel::TrPot);
      Internal(VALRDE, &SamaraModel::ValRDE);
      Internal(VALRFE, &SamaraModel::ValRFE);
      Internal(VALRSURF, &SamaraModel::ValRSurf);
      Internal(VENT, &SamaraModel::Vent);
      Internal(VITESSERACINAIRE, &SamaraModel::VitesseRacinaire);
      Internal(VITESSERACINAIREDAY, &SamaraModel::VitesseRacinaireDay);
      Internal(VOLMACROPORES, &SamaraModel::VolMacropores);
      Internal(VOLRELMACROPORES, &SamaraModel::VolRelMacropores);
      Internal(_VPD, &SamaraModel::VPD);
      Internal(WUEET, &SamaraModel::WueEt);
      Internal(WUETOT, &SamaraModel::WueTot);
      Internal(CHANGENURSERYSTATUS, &SamaraModel::ChangeNurseryStatus);
      Internal(CHANGESSPHASE, &SamaraModel::ChangeSsPhase);
      Internal(NBJOURCOMPTE, &SamaraModel::NbJourCompte);
      Internal(NURSERYSTATUS, &SamaraModel::NurseryStatus);
      Internal(TABCSTRINDICECOURANT, &SamaraModel::tabCstrIndiceCourant);

//    Internal(LAI, &SamaraModel::Lai);
//    Internal(NUMPHASE, &SamaraModel::NumPhase);
//    Internal(DEGRESDUJOUR, &SamaraModel::DegresDuJour);
//    Internal(DEGRESDUJOURCOR, &SamaraModel::DegresDuJourCor);
//    Internal(Ftsw, &SamaraModel::FTSW);
//    Internal(CSTR, &SamaraModel::Cstr);
//    Internal(ROOTFRONT, &SamaraModel::RootFront);
//    Internal(ETO, &SamaraModel::Eto);
//    Internal(Etm, &SamaraModel::ETM);
//    Internal(Etr, &SamaraModel::ETR);
//    Internal(Rue, &SamaraModel::RUE);
//    Internal(CONVERSIONEFF, &SamaraModel::ConversionEff);
//    Internal(IC, &SamaraModel::Ic);
//    Internal(CULMSPERPLANT, &SamaraModel::CulmsPerPlant);
//    Internal(CULMSPOP, &SamaraModel::CulmsPop);
//    Internal(CULMSPERHILL, &SamaraModel::CulmsPerHill);
//    Internal(GRAINYIELDPOP, &SamaraModel::GrainYieldPop);
//    Internal(DRYMATSTRUCTLEAFPOP, &SamaraModel::DryMatStructLeafPop);
//    Internal(DRYMATSTRUCTSHEATHPOP, &SamaraModel::DryMatStructSheathPop);
//    Internal(DRYMATSTRUCTROOTPOP, &SamaraModel::DryMatStructRootPop);
//    Internal(DRYMATSTRUCTINTERNODEPOP, &SamaraModel::DryMatStructInternodePop);
//    Internal(DRYMATRESINTERNODEPOP, &SamaraModel::DryMatResInternodePop);
//    Internal(DRYMATSTRUCTPANICLEPOP, &SamaraModel::DryMatStructPaniclePop);
//    Internal(DRYMATSTRUCTTOTPOP, &SamaraModel::DryMatStructTotPop);
//    Internal(DRYMATVEGETOTPOP, &SamaraModel::DryMatVegeTotPop);
//    Internal(DRYMATPANICLETOTPOP, &SamaraModel::DryMatPanicleTotPop);
//    Internal(DRYMATSTEMPOP, &SamaraModel::DryMatStemPop);
//    Internal(DRYMATABOVEGROUNDPOP, &SamaraModel::DryMatAboveGroundPop);
//    Internal(DRYMATTOTPOP, &SamaraModel::DryMatTotPop);
//    Internal(STERILITYCOLD, &SamaraModel::SterilityCold);
//    Internal(STERILITYHEAT, &SamaraModel::SterilityHeat);
//    Internal(STERILITYDROUGHT, &SamaraModel::SterilityDrought);
//    Internal(STERILITYTOT, &SamaraModel::SterilityTot);
//    Internal(HARVESTINDEX, &SamaraModel::HarvestIndex);
//    Internal(PANICLENUMPOP, &SamaraModel::PanicleNumPop);
//    Internal(PANICLENUMPLANT, &SamaraModel::PanicleNumPlant);
//    Internal(GRAINYIELDPANICLE, &SamaraModel::GrainYieldPanicle);
//    Internal(SPIKENUMPOP, &SamaraModel::SpikeNumPop);
//    Internal(SPIKENUMPANICLE, &SamaraModel::SpikeNumPanicle);
//    Internal(FERTSPIKENUMPOP, &SamaraModel::FertSpikeNumPop);
//    Internal(GRAINFILLINGSTATUS, &SamaraModel::GrainFillingStatus);
//    Internal(PHASESTEMELONGATION, &SamaraModel::PhaseStemElongation);
//    Internal(SLA, &SamaraModel::Sla);
//    Internal(HAUNINDEX, &SamaraModel::HaunIndex);
//    Internal(APEXHEIGHT, &SamaraModel::ApexHeight);
//    Internal(PLANTHEIGHT, &SamaraModel::PlantHeight);
//    Internal(PLANTWIDTH, &SamaraModel::PlantWidth);
//    Internal(KROLLING, &SamaraModel::KRolling);
//    Internal(LIRKDFCL, &SamaraModel::LIRkdfcl);
//    Internal(LTRKDFCL, &SamaraModel::LTRkdfcl);
//    Internal(ASSIMPOT, &SamaraModel::AssimPot);
//    Internal(ASSIM, &SamaraModel::Assim);
//    Internal(RESPMAINTTOT, &SamaraModel::RespMaintTot);
//    Internal(SUPPLYTOT, &SamaraModel::SupplyTot);
//    Internal(ASSIMSURPLUS, &SamaraModel::AssimSurplus);
//    Internal(ASSIMNOTUSED, &SamaraModel::AssimNotUsed);
//    Internal(TILLERDEATHPOP, &SamaraModel::TillerDeathPop);
//    Internal(PLANTLEAFNUMTOT, &SamaraModel::PlantLeafNumTot);
//    Internal(DEADLEAFDRYWTPOP, &SamaraModel::DeadLeafDrywtPop);
//    Internal(LAIDEAD, &SamaraModel::LaiDead);
//    Internal(RESCAPACITYINTERNODEPOP, &SamaraModel::ResCapacityInternodePop);
//    Internal(INTERNODERESSTATUS, &SamaraModel::InternodeResStatus);
//    Internal(DAYLENGTH, &SamaraModel::DayLength);
//    Internal(Par, &SamaraModel::PAR);
//    Internal(RGCALC, &SamaraModel::RgCalc);
//    Internal(VPDCALC, &SamaraModel::VPD);
//    Internal(TMOYCALC, &SamaraModel::TMoyCalc);
//    Internal(HMOYCALC, &SamaraModel::HMoyCalc);
//    Internal(KCEREAL, &SamaraModel::KceReal);
//    Internal(STOCKTOTAL, &SamaraModel::StockTotal);
//    Internal(EAUDISPO, &SamaraModel::EauDispo);
//    Internal(STOCKSURFACE, &SamaraModel::StockSurface);
//    Internal(STOCKRAC, &SamaraModel::StockRac);
//    Internal(RURAC, &SamaraModel::RuRac);
//    Internal(KCP, &SamaraModel::Kcp);
//    Internal(KCE, &SamaraModel::Kce);
//    Internal(EVAPPOT, &SamaraModel::EvapPot);
//    Internal(EVAP, &SamaraModel::Evap);
//    Internal(TRPOT, &SamaraModel::TrPot);
//    Internal(TR, &SamaraModel::Tr);
//    Internal(LR, &SamaraModel::Lr);
//    Internal(DR, &SamaraModel::Dr);
//    Internal(PARINTERCEPTE, &SamaraModel::PARIntercepte);
//    Internal(SOMMEDEGRESJOUR, &SamaraModel::SommeDegresJour);
//    Internal(VITESSERACINAIRE, &SamaraModel::VitesseRacinaire);
//    Internal(CSTRASSIM, &SamaraModel::CstrAssim);
//    Internal(RAYEXTRA, &SamaraModel::RayExtra);
//    Internal(CUMPAR, &SamaraModel::CumPar);
//    Internal(SOMMEDEGRESJOURCOR, &SamaraModel::SommeDegresJourCor);
//    Internal(SUMPP, &SamaraModel::SumPP);
//    Internal(SOMMEDEGRESJOURPHASEPRECEDENTE,
//             &SamaraModel::SommeDegresJourPhasePrecedente);
//    Internal(RESPMAINTDEBT, &SamaraModel::RespMaintDebt);
//    Internal(TMINMOY, &SamaraModel::TMinMoy);
//    Internal(TMAXMOY, &SamaraModel::TMaxMoy);
//    Internal(FTSWMOY, &SamaraModel::FtswMoy);
//    Internal(ROOTSHOOTRATIO, &SamaraModel::RootShootRatio);
//    Internal(TREFFINST, &SamaraModel::TrEffInst);
//    Internal(TREFF, &SamaraModel::TrEff);
//    Internal(WUEET, &SamaraModel::WueEt);
//    Internal(WUETOT, &SamaraModel::WueTot);
//    Internal(FLOODWATERDEPTH, &SamaraModel::FloodwaterDepth);
//    Internal(IRRIGAUTODAY, &SamaraModel::IrrigAutoDay);
//    Internal(IRRIGTOTDAY, &SamaraModel::IrrigTotDay);
//    Internal(FRACTIONPLANTHEIGHTSUBMER, &SamaraModel::FractionPlantHeightSubmer);
//    Internal(DENSITY, &SamaraModel::Density);
//    Internal(ROOTMASSPERVOL, &SamaraModel::RootMassPerVol);
//    Internal(VOLMACROPORES, &SamaraModel::VolMacropores);
//    Internal(STOCKMACROPORES, &SamaraModel::StockMacropores);
//    Internal(RELPOTLEAFLENGTH, &SamaraModel::RelPotLeafLength);
//    Internal(APEXHEIGHTGAIN, &SamaraModel::ApexHeightGain);
//    Internal(HAUNGAIN, &SamaraModel::HaunGain);
//    Internal(CONVERSION, &SamaraModel::Conversion);
//    Internal(STRESSCOLD, &SamaraModel::StressCold);
//    Internal(FRACTIONROOTSLOGGED, &SamaraModel::FractionRootsLogged);
//    Internal(RESUTIL, &SamaraModel::ResUtil);
//    Internal(KCTOT, &SamaraModel::KcTot);
//    Internal(NBJAS, &SamaraModel::NbJas);
  }

  virtual ~SamaraModel()
  { }

  void init(double t, const samara::ModelParameters &parameters);

  samara::ModelParameters _parameters;
  // parameters
  double Altitude = 0;
  double ASScstr = 0;
  double AttenMitch = 0;
  double BundHeight = 0;
  double Ca = 0;
  double CO2Cp = 0;
  double CO2Exp = 0;
  double CoeffAssimSla = 0;
  double CoefficientQ10 = 0;
  double CoeffInternodeMass = 0;
  double CoeffInternodeNum = 0;
  double CoeffLeafDeath = 0;
  double CoeffLeafWLRatio = 0;
  double CoeffPanicleMass = 0;
  double CoeffPanSinkPop = 0;
  double CoeffResCapacityInternode = 0;
  double CoeffReserveSink = 0;
  double CoeffRootMassPerVolMax = 0;
  double CoeffTillerDeath = 0;
  double CoeffTransplantingShock = 0;
  double DensityField = 0;
  double DensityNursery = 0;
  double DEVcstr = 0;
  double DurationNursery = 0;
  double EpaisseurProf = 0;
  double EpaisseurSurf = 0;
  double ExcessAssimToRoot = 0;
  double FTSWIrrig = 0;
  double HaunCritTillering = 0;
  double HumFC = 0;
  double HumPF = 0;
  double HumSat = 0;
  double IcTillering = 0;
  double InternodeLengthMax = 0;
  double IrrigAutoResume = 0;
  double IrrigAutoStop = 0;
  double IrrigAutoTarget = 0;
  double KcMax = 0;
  double KCritSterCold1 = 0;
  double KCritSterCold2 = 0;
  double KCritSterFtsw1 = 0;
  double KCritSterFtsw2 = 0;
  double KCritSterHeat1 = 0;
  double KCritSterHeat2 = 0;
  double KCritStressCold1 = 0;
  double KCritStressCold2 = 0;
  double Kdf = 0;
  double KRespInternode = 0;
  double KRespMaintLeaf = 0;
  double KRespMaintRoot = 0;
  double KRespMaintSheath = 0;
  double KRespPanicle = 0;
  double KTempMaint = 0;
  double Latitude = 0;
  double LeafLengthMax = 0;
  double LifeSavingDrainage = 0;
  double Mulch = 0;
  double PanStructMassMax = 0;
  double PercolationMax = 0;
  double PFactor = 0;
  double Phyllo = 0;
  double PlantsPerHill = 0;
  double PlotDrainageDAF = 0;
  double PoidsSecGrain = 0;
  double PourcRuiss = 0;
  double PPCrit = 0;
  double PPExp = 0;
  double PriorityPan = 0;
  double ProfRacIni = 0;
  double RankLongestLeaf = 0;
  double RelMobiliInternodeMax = 0;
  double RollingBase = 0;
  double RollingSens = 0;
  double RootCstr = 0;
  double RootFrontMax = 0;
  double RootPartitMax = 0;
  double SDJBVP = 0;
  double SDJLevee = 0;
  double SDJMatu1 = 0;
  double SDJMatu2 = 0;
  double SDJRPR = 0;
  double SeuilPP = 0;
  double SeuilRuiss = 0;
  double SlaMax = 0;
  double SlaMin = 0;
  double StockIniProf = 0;
  double StockIniSurf = 0;
  double TBase = 0;
  double TempSla = 0;
  double TilAbility = 0;
  double TLet = 0;
  double TOpt1 = 0;
  double TOpt2 = 0;
  double TransplantingDepth = 0;
  double TxAssimBVP = 0;
  double TxAssimMatu1 = 0;
  double TxAssimMatu2 = 0;
  double TxConversion = 0;
  double TxResGrain = 0;
  double TxRuSurfGermi = 0;
  double VRacBVP = 0;
  double VRacLevee = 0;
  double VRacMatu1 = 0;
  double VRacMatu2 = 0;
  double VRacPSP = 0;
  double VRacRPR = 0;
  double WaterLoggingSens = 0;
  double WtRatioLeafSheath = 0;
  int IrrigAuto = 0;
  int Transplanting = 0;

  // state variables
  double A_AssimSurplus = 0;
  double A_DemStructLeaf = 0;
  double A_DemStructTot = 0;
  double A_GrowthStructLeaf = 0;
  double A_GrowthStructTot = 0;
  double A_IncreaseResInternodePop = 0;
  double ApexHeight = 0;
  double ApexHeightGain = 0;
  double A_ResInternodeMobiliDay = 0;
  double Assim = 0;
  double AssimNotUsed = 0;
  double AssimNotUsedCum = 0;
  double AssimPot = 0;
  double AssimSurplus = 0;
  double CapaRDE = 0;
  double CapaREvap = 0;
  double CapaRFE = 0;
  double ChangePhase = 0;
  double ChangeSousPhase = 0;
  double CO2SlopeTr = 0;
  double CoeffCO2Assim = 0;
  double CoeffCO2Tr = 0;
  double CoeffStressLogging = 0;
  double Conversion = 0;
  double ConversionEff = 0;
  double CounterNursery = 0;
  double Cstr = 0;
  double CstrAssim = 0;
  double CstrCumul = 0;
  double CstrMean = 0;
  double CstrPhase2 = 0;
  double CstrPhase3 = 0;
  double CstrPhase4 = 0;
  double CstrPhase5 = 0;
  double CstrPhase6 = 0;
  double CulmsPerHill = 0;
  double CulmsPerHillFin = 0;
  double CulmsPerHillMax = 0;
  double CulmsPerPlant = 0;
  double CulmsPerPlantFin = 0;
  double CulmsPerPlantMax = 0;
  double CulmsPop = 0;
  double CumCarbonUsedPop = 0;
  double CumCstrPhase2 = 0;
  double CumCstrPhase3 = 0;
  double CumCstrPhase4 = 0;
  double CumCstrPhase5 = 0;
  double CumCstrPhase6 = 0;
  double CumDr = 0;
  double CumEt = 0;
  double CumFtswPhase2 = 0;
  double CumFtswPhase3 = 0;
  double CumFtswPhase4 = 0;
  double CumFtswPhase5 = 0;
  double CumFtswPhase6 = 0;
  double CumGrowthPop = 0;
  double CumIcPhase2 = 0;
  double CumIcPhase3 = 0;
  double CumIcPhase4 = 0;
  double CumIcPhase5 = 0;
  double CumIcPhase6 = 0;
  double CumIrrig = 0;
  double CumIrrigFin = 0;
  double CumLr = 0;
  double CumPar = 0;
  double CumSupplyTot = 0;
  double CumTr = 0;
  double CumWReceived = 0;
  double CumWuse = 0;
  double CumWUse = 0;
  double CumWUseFin = 0;
  double DAF = 0;
  double DateSemis = 0;
  double DayLength = 0;
  double DeadLeafDrywtPop = 0;
  double Decli = 0;
  double DegresDuJour = 0;
  double DegresDuJourCor = 0;
  double DemLeafAreaPlant = 0;
  double DemPanicleFillPop = 0;
  double DemResInternodePop = 0;
  double DemStructInternodePlant = 0;
  double DemStructInternodePop = 0;
  double DemStructLeafPlant = 0;
  double DemStructLeafPop = 0;
  double DemStructPaniclePlant = 0;
  double DemStructPaniclePop = 0;
  double DemStructRootPlant = 0;
  double DemStructRootPop = 0;
  double DemStructSheathPop = 0;
  double DemStructTotPop = 0;
  double Density = 0;
  double Dr = 0;
  double DryMatAboveGroundPop = 0;
  double DryMatAboveGroundPopFin = 0;
  double DryMatAboveGroundTotPop = 0;
  double DryMatPanicleTotPop = 0;
  double DryMatResInternodePop = 0;
  double DryMatResInternodePopOld = 0;
  double DryMatStemPop = 0;
  double DryMatStructInternodePop = 0;
  double DryMatStructLeafPop = 0;
  double DryMatStructPaniclePop = 0;
  double DryMatStructRootPop = 0;
  double DryMatStructSheathPop = 0;
  double DryMatStructStemPop = 0;
  double DryMatStructTotPop = 0;
  double DryMatTotPop = 0;
  double DryMatTotPopFin = 0;
  double DryMatVegeTotPop = 0;
  double DureeDuJour = 0;
  double DurGermFlow = 0;
  double DurGermMat = 0;
  double DurPhase1 = 0;
  double DurPhase2 = 0;
  double DurPhase3 = 0;
  double DurPhase4 = 0;
  double DurPhase5 = 0;
  double DurPhase6 = 0;
  double EauDispo = 0;
  double ETM = 0;
  double Eto = 0;
  double ETP = 0;
  double ETR = 0;
  double Evap = 0;
  double EvapPot = 0;
  double FertSpikeNumPop = 0;
  double FertSpikePop = 0;
  double FloodwaterDepth = 0;
  double FloodWaterGain = 0;
  double FractionPlantHeightSubmer = 0;
  double FractionRootsLogged = 0;
  double FTSW = 0;
  double FtswMoy = 0;
  double FtswPhase2 = 0;
  double FtswPhase3 = 0;
  double FtswPhase4 = 0;
  double FtswPhase5 = 0;
  double FtswPhase6 = 0;
  double GainRootSystSoilSurfPop = 0;
  double GainRootSystVolPop = 0;
  double GrainFillingStatus = 0;
  double GrainFillingStatusFin = 0;
  double GrainYieldPanicle = 0;
  double GrainYieldPop = 0;
  double GrainYieldPopFin = 0;
  double GrowthDryMatPop = 0;
  double GrowthPop = 0;
  double GrowthResInternodePop = 0;
  double GrowthStructDeficit = 0;
  double GrowthStructInternodePop = 0;
  double GrowthStructLeafPop = 0;
  double GrowthStructPaniclePop = 0;
  double GrowthStructRootPop = 0;
  double GrowthStructSheathPop = 0;
  double GrowthStructTotPop = 0;
  double HarvestIndex = 0;
  double HaunGain = 0;
  double HaunIndex = 0;
  double HMax = 0;
  double HMin = 0;
  double HMoy = 0;
  double HMoyCalc = 0;
  double Hum = 0;
  double Ic = 0;
  double IcCumul = 0;
  double IcMean = 0;
  double IcPhase2 = 0;
  double IcPhase3 = 0;
  double IcPhase4 = 0;
  double IcPhase5 = 0;
  double IcPhase6 = 0;
  double IncreaseResInternodePop = 0;
  double Ins = 0;
  double InternodeResStatus = 0;
  double Irrigation = 0;
  double IrrigAutoDay = 0;
  double IrrigTotDay = 0;
  double Kce = 0;
  double KceReal = 0; // parameter ????
  double Kcl = 0;
  double Kcp = 0;
  double KcTot = 0;
  double KPar = 0;
  double Kr = 0;
  double KRolling = 0;
  double Lai = 0;
  double LaiDead = 0;
  double LaiFin = 0;
  double LastLeafLength = 0; // parameter ???
  double LastLeafLengthPot = 0; // parameter ???
  double LatRad = 0;
  double LeafDeathPop = 0;
  double LIRkdf = 0;
  double LIRkdfcl = 0;
  double Lr = 0;
  double LTRkdf = 0;
  double LTRkdfcl = 0;
  double MaximumLai = 0;
  double MobiliLeafDeath = 0;
  double MonCompteur = 0;
  double NbDaysSinceGermination = 0;
  double NbJas = 0;
  double NumPhase = 0;
  double NumSousPhase = 0;
  double PanicleFilDeficit = 0;
  double PanicleFilPop = 0;
  double PanicleNumPlant = 0;
  double PanicleNumPop = 0;
  double PanicleSinkPop = 0;
  double PanStructMass = 0;
  double PAR = 0;
  double PARIntercepte = 0;
  double PEvap = 0;
  double PhaseStemElongation = 0;
  double PlantHeight = 0;
  double PlantLeafNumNew = 0;
  double PlantLeafNumTot = 0;
  double PlantWidth = 0;
  double PPSens = 0;
  double ProfRu = 0;
  double Rain = 0;
  double RayExtra = 0;
  double RelPhylloPhaseStemElong = 0;
  double RelPotLeafLength = 0;
  double ResCapacityInternodePop = 0;
  double ReservePopFin = 0;
  double ResInternodeMobiliDay = 0;
  double ResInternodeMobiliDayPot = 0;
  double RespMaintDebt = 0;
  double RespMaintTot = 0;
  double ResUtil = 0;
  double Rg = 0;
  double RgCalc = 0;
  double RgMax = 0;
  double RootFront = 0;
  double RootFrontOld = 0;
  double RootMassPerVol = 0;
  double RootShootRatio = 0;
  double RootSystSoilSurfPop = 0;
  double RootSystSoilSurfPopOld = 0;
  double RootSystVolPop = 0;
  double RootSystVolPopOld = 0;
  double RUE = 0;
  double RUEgreen = 0;
  double RuRac = 0;
  double RuSurf = 0;
  double SDJCorPhase4 = 0;
  double SeuilCstrMortality = 0;
  double SeuilTempPhasePrec = 0;
  double SeuilTempPhaseSuivante = 0;
  double SeuilTempSousPhaseSuivante = 0;
  double SimAnthesis50 = 0;
  double SimEmergence = 0;
  double SimEndCycle = 0;
  double SimPanIni = 0;
  double SimStartGermin = 0;
  double SimStartMatu2 = 0;
  double SimStartPSP = 0;
  double Sla = 0;
  double SlaMitch = 0;
  double SlaNew = 0;
  double SommeDegresJour = 0;
  double SommeDegresJourCor = 0;
  double SommeDegresJourMaximale = 0;
  double SommeDegresJourPhasePrecedente = 0;
  double SpikeNumPanicle = 0;
  double SpikeNumPop = 0;
  double SterilityCold = 0;
  double SterilityDrought = 0;
  double SterilityHeat = 0;
  double SterilityTot = 0;
  double SterilityTotFin = 0;
  double StockMacropores = 0;
  double StockRac = 0;
  double StockSurface = 0;
  double StockTotal = 0;
  double StressCold = 0;
  double StRu = 0;
  double StRuMax = 0;
  double SumPP = 0;
  double SunDistance = 0;
  double SunPosi = 0;
  double SupplyTot = 0;
  double tabCstr[5] = {0, 0, 0, 0, 0};
  double TempLai = 0;
  double TillerDeathPop = 0;
  double TMax = 0;
  double TMaxMoy = 0;
  double TMin = 0;
  double TMinMoy = 0;
  double TMoy = 0;
  double TMoyCalc = 0;
  double TMoyPrec = 0;
  double Tr = 0;
  double TrEff = 0;
  double TrEffInst = 0;
  double TrPot = 0;
  double ValRDE = 0;
  double ValRFE = 0;
  double ValRSurf = 0;
  double Vent = 0;
  double VitesseRacinaire = 0;
  double VitesseRacinaireDay = 0;
  double VolMacropores = 0;
  double VolRelMacropores = 0;
  double VPD = 0;
  double WueEt = 0;
  double WueTot = 0;
  int ChangeNurseryStatus = 0;
  int ChangeSsPhase = 0;
  int NbJourCompte = 0;
  int NurseryStatus = 0;
  int tabCstrIndiceCourant = 0;

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



  double lai() const
  { return Lai; }

  double numphase() const
  { return NumPhase; }

  double degresdujour() const
  { return DegresDuJour; }

  double degresdujourcor() const
  { return DegresDuJourCor; }

  double ftsw() const
  { return FTSW; }

  double cstr() const
  { return Cstr; }

  double rootfront() const
  { return RootFront; }

  double eto() const
  { return Eto; }

  double etm() const
  { return ETM; }

  double etr() const
  { return ETR; }

  double rue() const
  { return RUE; }

  double conversioneff() const
  { return ConversionEff; }

  double ic() const
  { return Ic; }

  double culmsperplant() const
  { return CulmsPerPlant; }

  double culmspop() const
  { return CulmsPop; }

  double culmsperhill() const
  { return CulmsPerHill; }

  double grainyieldpop() const
  { return GrainYieldPop; }

  double drymatstructleafpop() const
  { return DryMatStructLeafPop; }

  double drymatstructsheathpop() const
  { return DryMatStructSheathPop; }

  double drymatstructrootpop() const
  { return DryMatStructRootPop; }

  double drymatstructinternodepop() const
  { return DryMatStructInternodePop; }

  double drymatresinternodepop() const
  { return DryMatResInternodePop; }

  double drymatstructpaniclepop() const
  { return DryMatStructPaniclePop; }

  double drymatstructtotpop() const
  { return DryMatStructTotPop; }

  double drymatvegetotpop() const
  { return DryMatVegeTotPop; }

  double drymatpanicletotpop() const
  { return DryMatPanicleTotPop; }

  double drymatstempop() const
  { return DryMatStemPop; }

  double drymatabovegroundpop() const
  { return DryMatAboveGroundPop; }

  double drymattotpop() const
  { return DryMatTotPop; }

  double sterilitycold() const
  { return SterilityCold; }

  double sterilityheat() const
  { return SterilityHeat; }

  double sterilitydrought() const
  { return SterilityDrought; }

  double sterilitytot() const
  { return SterilityTot; }

  double harvestindex() const
  { return HarvestIndex; }

  double paniclenumpop() const
  { return PanicleNumPop; }

  double paniclenumplant() const
  { return PanicleNumPlant; }

  double grainyieldpanicle() const
  { return GrainYieldPanicle; }

  double spikenumpop() const
  { return SpikeNumPop; }

  double spikenumpanicle() const
  { return SpikeNumPanicle; }

  double fertspikenumpop() const
  { return FertSpikeNumPop; }

  double grainfillingstatus() const
  { return GrainFillingStatus; }

  double phasestemelongation() const
  { return PhaseStemElongation; }

  double sla() const
  { return Sla; }

  double haunindex() const
  { return HaunIndex; }

  double apexheight() const
  { return ApexHeight; }

  double plantheight() const
  { return PlantHeight; }

  double plantwidth() const
  { return PlantWidth; }

  double krolling() const
  { return KRolling; }

  double lirkdfcl() const
  { return LIRkdfcl; }

  double ltrkdfcl() const
  { return LTRkdfcl; }

  double assimpot() const
  { return AssimPot; }

  double assim() const
  { return Assim; }

  double respmainttot() const
  { return RespMaintTot; }

  double supplytot() const
  { return SupplyTot; }

  double assimsurplus() const
  { return AssimSurplus; }

  double assimnotused() const
  { return AssimNotUsed; }

  double tillerdeathpop() const
  { return TillerDeathPop; }

  double plantleafnumtot() const
  { return PlantLeafNumTot; }

  double deadleafdrywtpop() const
  { return DeadLeafDrywtPop; }

  double laidead() const
  { return LaiDead; }

  double rescapacityinternodepop() const
  { return ResCapacityInternodePop; }

  double internoderesstatus() const
  { return InternodeResStatus; }

  double daylength() const
  { return DayLength; }

  double par() const
  { return PAR; }

  double rgcalc() const
  { return RgCalc; }

  double vpdcalc() const
  { return VPD; }

  double tmoycalc() const
  { return TMoyCalc; }

  double hmoycalc() const
  { return HMoyCalc; }

  double kcereal() const
  { return KceReal; }

  double stocktotal() const
  { return StockTotal; }

  double eaudispo() const
  { return EauDispo; }

  double stocksurface() const
  { return StockSurface; }

  double stockrac() const
  { return StockRac; }

  double rurac() const
  { return RuRac; }

  double kcp() const
  { return Kcp; }

  double kce() const
  { return Kce; }

  double evappot() const
  { return EvapPot; }

  double evap() const
  { return Evap; }

  double trpot() const
  { return TrPot; }

  double tr() const
  { return Tr; }

  double lr() const
  { return Lr; }

  double dr() const
  { return Dr; }

  double parintercepte() const
  { return PARIntercepte; }

  double sommedegresjour() const
  { return SommeDegresJour; }

  double vitesseracinaire() const
  { return VitesseRacinaire; }

  double cstrassim() const
  { return CstrAssim; }

  double rayextra() const
  { return RayExtra; }

  double cumpar() const
  { return CumPar; }

  double sommedegresjourcor() const
  { return SommeDegresJourCor; }

  double sumpp() const
  { return SumPP; }

  double sommedegresjourphaseprecedente() const
  { return SommeDegresJourPhasePrecedente; }

  double respmaintdebt() const
  { return RespMaintDebt; }

  double tminmoy() const
  { return TMinMoy; }

  double tmaxmoy() const
  { return TMaxMoy; }

  double ftswmoy() const
  { return FtswMoy; }

  double rootshootratio() const
  { return RootShootRatio; }

  double treffinst() const
  { return TrEffInst; }

  double treff() const
  { return TrEff; }

  double wueet() const
  { return WueEt; }

  double wuetot() const
  { return WueTot; }

  double floodwaterdepth() const
  { return FloodwaterDepth; }

  double irrigautoday() const
  { return IrrigAutoDay; }

  double irrigtotday() const
  { return IrrigTotDay; }

  double fractionplantheightsubmer() const
  { return FractionPlantHeightSubmer; }

  double density() const
  { return Density; }

  double rootmasspervol() const
  { return RootMassPerVol; }

  double volmacropores() const
  { return VolMacropores; }

  double stockmacropores() const
  { return StockMacropores; }

  double relpotleaflength() const
  { return RelPotLeafLength; }

  double apexheightgain() const
  { return ApexHeightGain; }

  double haungain() const
  { return HaunGain; }

  double conversion() const
  { return Conversion; }

  double stresscold() const
  { return StressCold; }

  double fractionrootslogged() const
  { return FractionRootsLogged; }

  double resutil() const
  { return ResUtil; }

  double kctot() const
  { return KcTot; }

  double nbjas() const
  { return NbJas; }
};

#endif
