/**
 * @file samara/model/observer/LAIView.hpp
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

#ifndef MODEL_OBSERVER_GLOBAL_VIEW_HPP
#define MODEL_OBSERVER_GLOBAL_VIEW_HPP

#include <defines.hpp>
#include <artis/kernel/Any.hpp>
#include <models/SamaraModel.hpp>

namespace observer {

class GlobalView : public View {
public:
  GlobalView() {

     /********** NO REFERENCE**********/
//      selector("ChangeNurseryStatus", artis::kernel::INT, { SamaraModel::CHANGENURSERYSTATUS });
//      selector("ChangeSsPhase", artis::kernel::INT, { SamaraModel::CHANGESSPHASE });
//      selector("Decli", artis::kernel::DOUBLE, { SamaraModel::DECLI });
//      selector("DemLeafAreaPlant", artis::kernel::DOUBLE, { SamaraModel::DEMLEAFAREAPLANT });
//      selector("DemPanicleFillPop", artis::kernel::DOUBLE, { SamaraModel::DEMPANICLEFILLPOP });
//      selector("DemStructInternodePlant", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTINTERNODEPLANT });
//      selector("DemStructInternodePop", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTINTERNODEPOP });
//      selector("DemStructLeafPlant", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTLEAFPLANT });
//      selector("DemStructLeafPop", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTLEAFPOP });
//      selector("DemStructPaniclePlant", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTPANICLEPLANT });
//      selector("DemStructPaniclePop", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTPANICLEPOP });
//      selector("DemStructRootPlant", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTROOTPLANT });
//      selector("DemStructRootPop", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTROOTPOP });
//      selector("DemStructSheathPop", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTSHEATHPOP });
//      selector("DemStructTotPop", artis::kernel::DOUBLE, { SamaraModel::DEMSTRUCTTOTPOP });
//      selector("GainRootSystSoilSurfPop", artis::kernel::DOUBLE, { SamaraModel::GAINROOTSYSTSOILSURFPOP });
//      selector("GainRootSystVolPop", artis::kernel::DOUBLE, { SamaraModel::GAINROOTSYSTVOLPOP });
//      selector("GrowthResInternodePop", artis::kernel::DOUBLE, { SamaraModel::GROWTHRESINTERNODEPOP });
//      selector("GrowthStructDeficit", artis::kernel::DOUBLE, { SamaraModel::GROWTHSTRUCTDEFICIT });
//      selector("GrowthStructInternodePop", artis::kernel::DOUBLE, { SamaraModel::GROWTHSTRUCTINTERNODEPOP });
//      selector("GrowthStructLeafPop", artis::kernel::DOUBLE, { SamaraModel::GROWTHSTRUCTLEAFPOP });
//      selector("GrowthStructPaniclePop", artis::kernel::DOUBLE, { SamaraModel::GROWTHSTRUCTPANICLEPOP });
//      selector("GrowthStructRootPop", artis::kernel::DOUBLE, { SamaraModel::GROWTHSTRUCTROOTPOP });
//      selector("GrowthStructSheathPop", artis::kernel::DOUBLE, { SamaraModel::GROWTHSTRUCTSHEATHPOP });
//      selector("GrowthStructTotPop", artis::kernel::DOUBLE, { SamaraModel::GROWTHSTRUCTTOTPOP });
//      selector("Hum", artis::kernel::DOUBLE, { SamaraModel::HUM });
//      selector("IncreaseResInternodePop", artis::kernel::DOUBLE, { SamaraModel::INCREASERESINTERNODEPOP });
//      selector("Irrigation", artis::kernel::DOUBLE, { SamaraModel::IRRIGATION });
//      selector("Kr", artis::kernel::DOUBLE, { SamaraModel::KR });
//      selector("LatRad", artis::kernel::DOUBLE, { SamaraModel::LATRAD });
//      selector("LeafDeathPop", artis::kernel::DOUBLE, { SamaraModel::LEAFDEATHPOP });
//      selector("MobiliLeafDeath", artis::kernel::DOUBLE, { SamaraModel::MOBILILEAFDEATH });
//      selector("NbDaysSinceGermination", artis::kernel::DOUBLE, { SamaraModel::NBDAYSSINCEGERMINATION });
//      selector("PanicleFilPop", artis::kernel::DOUBLE, { SamaraModel::PANICLEFILPOP });
//      selector("PanicleSinkPop", artis::kernel::DOUBLE, { SamaraModel::PANICLESINKPOP });
//      selector("PanStructMass", artis::kernel::DOUBLE, { SamaraModel::PANSTRUCTMASS });
//      selector("PlantLeafNumNew", artis::kernel::DOUBLE, { SamaraModel::PLANTLEAFNUMNEW });
//      selector("ProfRu", artis::kernel::DOUBLE, { SamaraModel::PROFRU });
//      selector("ResInternodeMobiliDayPot", artis::kernel::DOUBLE, { SamaraModel::RESINTERNODEMOBILIDAYPOT });
//      selector("RgMax", artis::kernel::DOUBLE, { SamaraModel::RGMAX });
//      selector("RUEgreen", artis::kernel::DOUBLE, { SamaraModel::RUEGREEN });
//      selector("RuSurf", artis::kernel::DOUBLE, { SamaraModel::RUSURF });
//      selector("SDJCorPhase4", artis::kernel::DOUBLE, { SamaraModel::SDJCORPHASE4 });
//      selector("SeuilCstrMortality", artis::kernel::DOUBLE, { SamaraModel::SEUILCSTRMORTALITY });
//      selector("StRuMax", artis::kernel::DOUBLE, { SamaraModel::STRUMAX });
//      selector("SunDistance", artis::kernel::DOUBLE, { SamaraModel::SUNDISTANCE });
//      selector("SunPosi", artis::kernel::DOUBLE, { SamaraModel::SUNPOSI });
//      selector("TempLai", artis::kernel::DOUBLE, { SamaraModel::TEMPLAI });
//      selector("TMoyPrec", artis::kernel::DOUBLE, { SamaraModel::TMOYPREC });
//      selector("ValRDE", artis::kernel::DOUBLE, { SamaraModel::VALRDE });
//      selector("ValRFE", artis::kernel::DOUBLE, { SamaraModel::VALRFE });
//      selector("ValRSurf", artis::kernel::DOUBLE, { SamaraModel::VALRSURF });
//      selector("VolRelMacropores", artis::kernel::DOUBLE, { SamaraModel::VOLRELMACROPORES });
//      selector("NurseryStatus", artis::kernel::INT, { SamaraModel::NURSERYSTATUS });
//      selector("PanicleFilDeficit", artis::kernel::DOUBLE, { SamaraModel::PANICLEFILDEFICIT });
//      selector("RootSystSoilSurfPop", artis::kernel::DOUBLE, { SamaraModel::ROOTSYSTSOILSURFPOP });
//      selector("RootSystSoilSurfPopOld", artis::kernel::DOUBLE, { SamaraModel::ROOTSYSTSOILSURFPOPOLD });
//      selector("RootSystVolPop", artis::kernel::DOUBLE, { SamaraModel::ROOTSYSTVOLPOP });
//      selector("RootSystVolPopOld", artis::kernel::DOUBLE, { SamaraModel::ROOTSYSTVOLPOPOLD });


      /********** EXACT **********/
//      selector("A_GrowthStructLeaf", artis::kernel::DOUBLE, { SamaraModel::A_GROWTHSTRUCTLEAF });
//      selector("A_GrowthStructTot", artis::kernel::DOUBLE, { SamaraModel::A_GROWTHSTRUCTTOT });
//      selector("ApexHeight", artis::kernel::DOUBLE, { SamaraModel::APEXHEIGHT });
//      selector("Assim", artis::kernel::DOUBLE, { SamaraModel::ASSIM });
//      selector("AssimPot", artis::kernel::DOUBLE, { SamaraModel::ASSIMPOT });
//      selector("AssimSurplus", artis::kernel::DOUBLE, { SamaraModel::ASSIMSURPLUS });
//      selector("Conversion", artis::kernel::DOUBLE, { SamaraModel::CONVERSION });
//      selector("Cstr", artis::kernel::DOUBLE, { SamaraModel::CSTR });
//      selector("CstrAssim", artis::kernel::DOUBLE, { SamaraModel::CSTRASSIM });
//      selector("CstrCum", artis::kernel::DOUBLE, { SamaraModel::CSTRCUMUL });
//      selector("CulmsPerHill", artis::kernel::DOUBLE, { SamaraModel::CULMSPERHILL });
//      selector("CulmsPerHillFin", artis::kernel::DOUBLE, { SamaraModel::CULMSPERHILLFIN });
//      selector("CulmsPerHillMax", artis::kernel::DOUBLE, { SamaraModel::CULMSPERHILLMAX });
//      selector("CulmsPerPlant", artis::kernel::DOUBLE, { SamaraModel::CULMSPERPLANT });
//      selector("CulmsPerPlantFin", artis::kernel::DOUBLE, { SamaraModel::CULMSPERPLANTFIN });
//      selector("CulmsPerPlantMax", artis::kernel::DOUBLE, { SamaraModel::CULMSPERPLANTMAX });
//      selector("CulmsPop", artis::kernel::DOUBLE, { SamaraModel::CULMSPOP });
//      selector("DayLength", artis::kernel::DOUBLE, { SamaraModel::DAYLENGTH });
//      selector("DegresDuJour", artis::kernel::DOUBLE, { SamaraModel::DEGRESDUJOUR });
//      selector("DegresDuJourCor", artis::kernel::DOUBLE, { SamaraModel::DEGRESDUJOURCOR });
//      selector("Density", artis::kernel::DOUBLE, { SamaraModel::DENSITY });
//      selector("Dr", artis::kernel::DOUBLE, { SamaraModel::DR });
//      selector("DryMatStructRootPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTROOTPOP });
//      selector("DurGermFlow", artis::kernel::DOUBLE, { SamaraModel::DURGERMFLOW });
//      selector("DurGermMat", artis::kernel::DOUBLE, { SamaraModel::DURGERMMAT });
//      selector("EauDispo", artis::kernel::DOUBLE, { SamaraModel::EAUDISPO });
//      selector("Eto", artis::kernel::DOUBLE, { SamaraModel::ETO });
//      selector("ETR", artis::kernel::DOUBLE, { SamaraModel::_ETR });
//      selector("Evap", artis::kernel::DOUBLE, { SamaraModel::EVAP });
//      selector("EvapPot", artis::kernel::DOUBLE, { SamaraModel::EVAPPOT });
//      selector("FloodwaterDepth", artis::kernel::DOUBLE, { SamaraModel::FLOODWATERDEPTH });
//      selector("FractionPlantHeightSubmer", artis::kernel::DOUBLE, { SamaraModel::FRACTIONPLANTHEIGHTSUBMER });
//      selector("FractionRootsLogged", artis::kernel::DOUBLE, { SamaraModel::FRACTIONROOTSLOGGED });
//      selector("FTSW", artis::kernel::DOUBLE, { SamaraModel::_FTSW });
//      selector("FtswMoy", artis::kernel::DOUBLE, { SamaraModel::FTSWMOY });
//      selector("HarvestIndex", artis::kernel::DOUBLE, { SamaraModel::HARVESTINDEX });
//      selector("HaunIndex", artis::kernel::DOUBLE, { SamaraModel::HAUNINDEX });
//      selector("HMoyCalc", artis::kernel::DOUBLE, { SamaraModel::HMOYCALC });
//      selector("IrrigAutoDay", artis::kernel::DOUBLE, { SamaraModel::IRRIGAUTODAY });
//      selector("IrrigTotDay", artis::kernel::DOUBLE, { SamaraModel::IRRIGTOTDAY });
//      selector("Kcl", artis::kernel::DOUBLE, { SamaraModel::KCL });
//      selector("KcTot", artis::kernel::DOUBLE, { SamaraModel::KCTOT });
//      selector("KRolling", artis::kernel::DOUBLE, { SamaraModel::KROLLING });
//      selector("Lr", artis::kernel::DOUBLE, { SamaraModel::LR });
//      selector("MaxLai", artis::kernel::DOUBLE, { SamaraModel::MAXIMUMLAI });
//      selector("NbJas", artis::kernel::DOUBLE, { SamaraModel::NBJAS });
//      selector("NumPhase", artis::kernel::DOUBLE, { SamaraModel::NUMPHASE });
//      selector("PanicleNumPlant", artis::kernel::DOUBLE, { SamaraModel::PANICLENUMPLANT });
//      selector("PanicleNumPop", artis::kernel::DOUBLE, { SamaraModel::PANICLENUMPOP });
//      selector("Par", artis::kernel::DOUBLE, { SamaraModel::_PAR });
//      selector("PhaseStemElongation", artis::kernel::DOUBLE, { SamaraModel::PHASESTEMELONGATION });
//      selector("PlantHeight", artis::kernel::DOUBLE, { SamaraModel::PLANTHEIGHT });
//      selector("PlantLeafNumTot", artis::kernel::DOUBLE, { SamaraModel::PLANTLEAFNUMTOT });
//      selector("PlantWidth", artis::kernel::DOUBLE, { SamaraModel::PLANTWIDTH });
//      selector("RayExtra", artis::kernel::DOUBLE, { SamaraModel::RAYEXTRA });
//      selector("RelPotLeafLength", artis::kernel::DOUBLE, { SamaraModel::RELPOTLEAFLENGTH });
//      selector("ResUtil", artis::kernel::DOUBLE, { SamaraModel::RESUTIL });
//      selector("RgCalc", artis::kernel::DOUBLE, { SamaraModel::RGCALC });
//      selector("RootFront", artis::kernel::DOUBLE, { SamaraModel::ROOTFRONT });
//      selector("RootMassPerVol", artis::kernel::DOUBLE, { SamaraModel::ROOTMASSPERVOL });
//      selector("RootShootRatio", artis::kernel::DOUBLE, { SamaraModel::ROOTSHOOTRATIO });
//      selector("RURac", artis::kernel::DOUBLE, { SamaraModel::RURAC });
//      selector("SimEndCycle", artis::kernel::DOUBLE, { SamaraModel::SIMENDCYCLE });
//      selector("SimStartGermin", artis::kernel::DOUBLE, { SamaraModel::SIMSTARTGERMIN });
//      selector("SumDegreDayCor", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOURCOR });
//      selector("SumDDPhasePrec", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOURPHASEPRECEDENTE });
//      selector("SterilityCold", artis::kernel::DOUBLE, { SamaraModel::STERILITYCOLD });
//      selector("SterilityHeat", artis::kernel::DOUBLE, { SamaraModel::STERILITYHEAT });
//      selector("StockMacropores", artis::kernel::DOUBLE, { SamaraModel::STOCKMACROPORES });
//      selector("StockRac", artis::kernel::DOUBLE, { SamaraModel::STOCKRAC });
//      selector("StockSurface", artis::kernel::DOUBLE, { SamaraModel::STOCKSURFACE });
//      selector("StockTotal", artis::kernel::DOUBLE, { SamaraModel::STOCKTOTAL });
//      selector("StressCold", artis::kernel::DOUBLE, { SamaraModel::STRESSCOLD });
//      selector("SumPP", artis::kernel::DOUBLE, { SamaraModel::SUMPP });
//      selector("TillerDeathPop", artis::kernel::DOUBLE, { SamaraModel::TILLERDEATHPOP });
//      selector("TMaxMoy", artis::kernel::DOUBLE, { SamaraModel::TMAXMOY });
//      selector("TMinMoy", artis::kernel::DOUBLE, { SamaraModel::TMINMOY });
//      selector("TMoyCalc", artis::kernel::DOUBLE, { SamaraModel::TMOYCALC });
//      selector("VitesseRacinaire", artis::kernel::DOUBLE, { SamaraModel::VITESSERACINAIRE });
//      selector("VolMacropores", artis::kernel::DOUBLE, { SamaraModel::VOLMACROPORES });
//      selector("CumDr", artis::kernel::DOUBLE, { SamaraModel::CUMDR });
//      selector("CumWUse", artis::kernel::DOUBLE, { SamaraModel::CUMWUSE });

      /********** DIVERGENT **********/
//      selector("A_IncreaseResInternodePop", artis::kernel::DOUBLE, { SamaraModel::A_INCREASERESINTERNODEPOP });
//      selector("A_ResInternodeMobiliDay", artis::kernel::DOUBLE, { SamaraModel::A_RESINTERNODEMOBILIDAY });
//      selector("AssimNotUsed", artis::kernel::DOUBLE, { SamaraModel::ASSIMNOTUSED });
//      selector("AssimNotUsedCum", artis::kernel::DOUBLE, { SamaraModel::ASSIMNOTUSEDCUM });
      selector("CumEt", artis::kernel::DOUBLE, { SamaraModel::CUMET });
//      selector("CumSupplyTot", artis::kernel::DOUBLE, { SamaraModel::CUMSUPPLYTOT });
      selector("CumTr", artis::kernel::DOUBLE, { SamaraModel::CUMTR });
//      selector("CumWUseFin", artis::kernel::DOUBLE, { SamaraModel::CUMWUSEFIN });
//      selector("DeadLeafdrywtPop", artis::kernel::DOUBLE, { SamaraModel::DEADLEAFDRYWTPOP });
//      selector("DryMatAboveGroundPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATABOVEGROUNDPOP });
//      selector("DryMatAboveGroundPopFin", artis::kernel::DOUBLE, { SamaraModel::DRYMATABOVEGROUNDPOPFIN });
//      selector("DryMatAboveGroundTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATABOVEGROUNDTOTPOP });
//      selector("DryMatPanicleTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATPANICLETOTPOP });
//      selector("DryMatResInternodePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATRESINTERNODEPOP });
//      selector("DryMatStemPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTEMPOP });
//      selector("DryMatStructInternodePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTINTERNODEPOP });
//      selector("DryMatStructLeafPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTLEAFPOP });
//      selector("DryMatStructPaniclePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTPANICLEPOP });
//      selector("DryMatStructSheathPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTSHEATHPOP });
//      selector("DryMatStructTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTTOTPOP });
//      selector("DryMatTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATTOTPOP });
//      selector("DryMatTotPopFin", artis::kernel::DOUBLE, { SamaraModel::DRYMATTOTPOPFIN });
//      selector("DryMatVegeTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATVEGETOTPOP });
//      selector("GrainFillingStatus", artis::kernel::DOUBLE, { SamaraModel::GRAINFILLINGSTATUS });
//      selector("GrainFillingStatusFin", artis::kernel::DOUBLE, { SamaraModel::GRAINFILLINGSTATUSFIN });
//      selector("GrainYieldPanicle", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPANICLE });
//      selector("GrainYieldPop", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPOP });
//      selector("GrainYieldPopFin", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPOPFIN });
//      selector("Ic", artis::kernel::DOUBLE, { SamaraModel::IC });
//      selector("IcCum", artis::kernel::DOUBLE, { SamaraModel::ICCUMUL });
//      selector("InternodeResStatus", artis::kernel::DOUBLE, { SamaraModel::INTERNODERESSTATUS });
//      selector("Kce", artis::kernel::DOUBLE, { SamaraModel::KCE });
//      selector("Kcp", artis::kernel::DOUBLE, { SamaraModel::KCP });
//      selector("Lai", artis::kernel::DOUBLE, { SamaraModel::LAI });
//      selector("LaiDead", artis::kernel::DOUBLE, { SamaraModel::LAIDEAD });
//      selector("LaiFin", artis::kernel::DOUBLE, { SamaraModel::LAIFIN });
//      selector("LIRkdfcl", artis::kernel::DOUBLE, { SamaraModel::LIRKDFCL });
//      selector("LTRkdfcl", artis::kernel::DOUBLE, { SamaraModel::LTRKDFCL });
//      selector("ParIntercepte", artis::kernel::DOUBLE, { SamaraModel::PARINTERCEPTE });
//      selector("ResCapacityInternodePop", artis::kernel::DOUBLE, { SamaraModel::RESCAPACITYINTERNODEPOP });
//      selector("ReservePopFin", artis::kernel::DOUBLE, { SamaraModel::RESERVEPOPFIN });
//      selector("RespMaintDebt", artis::kernel::DOUBLE, { SamaraModel::RESPMAINTDEBT });
//      selector("RespMaintTot", artis::kernel::DOUBLE, { SamaraModel::RESPMAINTTOT });
//      selector("SpikeNumPanicle", artis::kernel::DOUBLE, { SamaraModel::SPIKENUMPANICLE });
//      selector("SpikeNumPop", artis::kernel::DOUBLE, { SamaraModel::SPIKENUMPOP });
//      selector("SterilityTotFin", artis::kernel::DOUBLE, { SamaraModel::STERILITYTOTFIN });
//      selector("SupplyTot", artis::kernel::DOUBLE, { SamaraModel::SUPPLYTOT });
//      selector("TrPot", artis::kernel::DOUBLE, { SamaraModel::TRPOT });
//      selector("WueEt", artis::kernel::DOUBLE, { SamaraModel::WUEET });
//      selector("CoeffCO2Tr", artis::kernel::DOUBLE, { SamaraModel::COEFFCO2TR });
//      selector("CstrMean", artis::kernel::DOUBLE, { SamaraModel::CSTRMEAN });
//      selector("Tr", artis::kernel::DOUBLE, { SamaraModel::TR });
//      selector("TrEffInst", artis::kernel::DOUBLE, { SamaraModel::TREFFINST });
//      selector("WueTot", artis::kernel::DOUBLE, { SamaraModel::WUETOT });
//      selector("A_AssimSurplus", artis::kernel::DOUBLE, { SamaraModel::A_ASSIMSURPLUS });
//      selector("CoeffCO2Assim", artis::kernel::DOUBLE, { SamaraModel::COEFFCO2ASSIM });
//      selector("CumCarbonUsedPop", artis::kernel::DOUBLE, { SamaraModel::CUMCARBONUSEDPOP });
//      selector("CumGrowthPop", artis::kernel::DOUBLE, { SamaraModel::CUMGROWTHPOP });
//      selector("DryMatResInternodePopOld", artis::kernel::DOUBLE, { SamaraModel::DRYMATRESINTERNODEPOPOLD });
//      selector("ETM", artis::kernel::DOUBLE, { SamaraModel::_ETM });
//      selector("GrowthDryMatPop_V2_1", artis::kernel::DOUBLE, { SamaraModel::GROWTHDRYMATPOP });
//      selector("GrowthPop", artis::kernel::DOUBLE, { SamaraModel::GROWTHPOP });
//      selector("IcMean", artis::kernel::DOUBLE, { SamaraModel::ICMEAN });
//      selector("Sla", artis::kernel::DOUBLE, { SamaraModel::SLA });
//      selector("SterilityDrought", artis::kernel::DOUBLE, { SamaraModel::STERILITYDROUGHT });
//      selector("SterilityTot", artis::kernel::DOUBLE, { SamaraModel::STERILITYTOT });
//      selector("SumDegresDay", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOUR });
//      selector("A_DemStructLeaf", artis::kernel::DOUBLE, { SamaraModel::A_DEMSTRUCTLEAF });
//      selector("A_DemStructTot", artis::kernel::DOUBLE, { SamaraModel::A_DEMSTRUCTTOT });
//      selector("DemResInternodePop", artis::kernel::DOUBLE, { SamaraModel::DEMRESINTERNODEPOP });

//      /********** INVALID **********/
      selector("CumIrrig", artis::kernel::DOUBLE, { SamaraModel::CUMIRRIG });
      selector("CumIrrigFin", artis::kernel::DOUBLE, { SamaraModel::CUMIRRIGFIN });
      selector("RUE", artis::kernel::DOUBLE, { SamaraModel::_RUE });
      selector("TrEff", artis::kernel::DOUBLE, { SamaraModel::TREFF });

//      /********** FLAT **********/
//      selector("ApexHeightGain", artis::kernel::DOUBLE, { SamaraModel::APEXHEIGHTGAIN });
//      selector("Haungain", artis::kernel::DOUBLE, { SamaraModel::HAUNGAIN });
//      selector("LIRkdf", artis::kernel::DOUBLE, { SamaraModel::LIRKDF });
//      selector("LTRkdf", artis::kernel::DOUBLE, { SamaraModel::LTRKDF });
//      selector("ResInternodeMobiliDay", artis::kernel::DOUBLE, { SamaraModel::RESINTERNODEMOBILIDAY });
//      selector("RootFrontOld", artis::kernel::DOUBLE, { SamaraModel::ROOTFRONTOLD });
//      selector("SimAnthesis50", artis::kernel::DOUBLE, { SamaraModel::SIMANTHESIS50 });
//      selector("SimEmergence", artis::kernel::DOUBLE, { SamaraModel::SIMEMERGENCE });
//      selector("SimPanIni", artis::kernel::DOUBLE, { SamaraModel::SIMPANINI });
//      selector("SimStartMatu2", artis::kernel::DOUBLE, { SamaraModel::SIMSTARTMATU2 });
//      selector("SimStartPSP", artis::kernel::DOUBLE, { SamaraModel::SIMSTARTPSP });
//      selector("SlaMitch", artis::kernel::DOUBLE, { SamaraModel::SLAMITCH });
//      selector("SlaNew", artis::kernel::DOUBLE, { SamaraModel::SLANEW });
//      selector("VitesseRacinaireDay", artis::kernel::DOUBLE, { SamaraModel::VITESSERACINAIREDAY });
//      selector("CumWReceived", artis::kernel::DOUBLE, { SamaraModel::CUMWRECEIVED });
//      selector("ConversionEff", artis::kernel::DOUBLE, { SamaraModel::CONVERSIONEFF });
//      selector("CumLr", artis::kernel::DOUBLE, { SamaraModel::CUMLR });
//      selector("CumPAR", artis::kernel::DOUBLE, { SamaraModel::CUMPAR });








      /********** OLD SMALL VERSION ********/
////    selector("ApexHeight", artis::kernel::DOUBLE, { SamaraModel::APEXHEIGHT });
////    selector("Assim", artis::kernel::DOUBLE, { SamaraModel::ASSIM });
////    selector("AssimNotUsed", artis::kernel::DOUBLE, { SamaraModel::ASSIMNOTUSED });
////    selector("AssimPot", artis::kernel::DOUBLE, { SamaraModel::ASSIMPOT });
////    selector("AssimSurplus", artis::kernel::DOUBLE, { SamaraModel::ASSIMSURPLUS });
////    selector("Cstr", artis::kernel::DOUBLE, { SamaraModel::CSTR });
////    selector("CstrAssim", artis::kernel::DOUBLE, { SamaraModel::CSTRASSIM });
////    selector("CulmsPerHill", artis::kernel::DOUBLE, { SamaraModel::CULMSPERHILL });
////    selector("CulmsPerPlant", artis::kernel::DOUBLE, { SamaraModel::CULMSPERPLANT });
////    selector("CulmsPop", artis::kernel::DOUBLE, { SamaraModel::CULMSPOP });
////    selector("DeadLeafdrywtPop", artis::kernel::DOUBLE, { SamaraModel::DEADLEAFDRYWTPOP });
////    selector("Dr", artis::kernel::DOUBLE, { SamaraModel::DR });
////    selector("DryMatAboveGroundPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATABOVEGROUNDPOP });
////    selector("DryMatPanicleTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATPANICLETOTPOP });
////    selector("DryMatResInternodePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATRESINTERNODEPOP });
////    selector("DryMatStemPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTEMPOP });
////    selector("DryMatStructInternodePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTINTERNODEPOP });
////    selector("DryMatStructLeafPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTLEAFPOP });
////    selector("DryMatStructPaniclePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTPANICLEPOP });
////    selector("DryMatStructRootPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTROOTPOP });
////    selector("DryMatStructSheathPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTSHEATHPOP });
////    selector("DryMatStructTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTTOTPOP });
////    selector("DryMatTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATTOTPOP });
////    selector("DryMatVegeTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATVEGETOTPOP });
////    selector("ETM", artis::kernel::DOUBLE, { SamaraModel::Etm });
////    selector("ETR", artis::kernel::DOUBLE, { SamaraModel::Etr });
////    selector("Evap", artis::kernel::DOUBLE, { SamaraModel::EVAP });
////    selector("EvapPot", artis::kernel::DOUBLE, { SamaraModel::EVAPPOT });
////    selector("FTSW", artis::kernel::DOUBLE, { SamaraModel::Ftsw });
////    selector("FertSpikeNumPop", artis::kernel::DOUBLE, { SamaraModel::FERTSPIKENUMPOP });
////    selector("FractionPlantHeightSubmer", artis::kernel::DOUBLE, { SamaraModel::FRACTIONPLANTHEIGHTSUBMER });
////    selector("FractionRootsLogged", artis::kernel::DOUBLE, { SamaraModel::FRACTIONROOTSLOGGED });
////    selector("FtswMoy", artis::kernel::DOUBLE, { SamaraModel::FTSWMOY });
////    selector("GrainFillingStatus", artis::kernel::DOUBLE, { SamaraModel::GRAINFILLINGSTATUS });
////    selector("GrainYieldPanicle", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPANICLE });
////    selector("GrainYieldPop", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPOP });
////    selector("HarvestIndex", artis::kernel::DOUBLE, { SamaraModel::HARVESTINDEX });
////    selector("Haungain", artis::kernel::DOUBLE, { SamaraModel::HAUNGAIN });
////    selector("Ic", artis::kernel::DOUBLE, { SamaraModel::IC });
////    selector("InternodeResStatus", artis::kernel::DOUBLE, { SamaraModel::INTERNODERESSTATUS });
////    selector("KcTot", artis::kernel::DOUBLE, { SamaraModel::KCTOT });
////    selector("Kce", artis::kernel::DOUBLE, { SamaraModel::KCE });
////    selector("KceReal", artis::kernel::DOUBLE, { SamaraModel::KCEREAL });
////    selector("Kcp", artis::kernel::DOUBLE, { SamaraModel::KCP });
////    selector("LIRkdfcl", artis::kernel::DOUBLE, { SamaraModel::LIRKDFCL });
////    selector("LTRkdfcl", artis::kernel::DOUBLE, { SamaraModel::LTRKDFCL });
////    selector("Lai", artis::kernel::DOUBLE, { SamaraModel::LAI });
////    selector("LaiDead", artis::kernel::DOUBLE, { SamaraModel::LAIDEAD });
////    selector("NumPhase", artis::kernel::DOUBLE, { SamaraModel::NUMPHASE });
////    selector("PanicleNumPlant", artis::kernel::DOUBLE, { SamaraModel::PANICLENUMPLANT });
////    selector("PanicleNumPop", artis::kernel::DOUBLE, { SamaraModel::PANICLENUMPOP });
////    selector("ParIntercepte", artis::kernel::DOUBLE, { SamaraModel::PARINTERCEPTE });
////    selector("PlantHeight", artis::kernel::DOUBLE, { SamaraModel::PLANTHEIGHT });
////    selector("PlantLeafNumTot", artis::kernel::DOUBLE, { SamaraModel::PLANTLEAFNUMTOT });
////    selector("PlantWidth", artis::kernel::DOUBLE, { SamaraModel::PLANTWIDTH });
//    selector("RUE", artis::kernel::DOUBLE, { SamaraModel::Rue });
////    selector("RURac", artis::kernel::DOUBLE, { SamaraModel::RURAC });
////    selector("ResCapacityInternodePop", artis::kernel::DOUBLE, { SamaraModel::RESCAPACITYINTERNODEPOP });
////    selector("RespMaintDebt", artis::kernel::DOUBLE, { SamaraModel::RESPMAINTDEBT });
////    selector("RespMaintTot", artis::kernel::DOUBLE, { SamaraModel::RESPMAINTTOT });
////    selector("RootMassPerVol", artis::kernel::DOUBLE, { SamaraModel::ROOTMASSPERVOL });
////    selector("RootShootRatio", artis::kernel::DOUBLE, { SamaraModel::ROOTSHOOTRATIO });
////    selector("Sla", artis::kernel::DOUBLE, { SamaraModel::SLA });
////    selector("SpikeNumPanicle", artis::kernel::DOUBLE, { SamaraModel::SPIKENUMPANICLE });
////    selector("SpikeNumPop", artis::kernel::DOUBLE, { SamaraModel::SPIKENUMPOP });
//    selector("SterilityDrought", artis::kernel::DOUBLE, { SamaraModel::STERILITYDROUGHT });
//    selector("SterilityTot", artis::kernel::DOUBLE, { SamaraModel::STERILITYTOT });
////    selector("StockMacropores", artis::kernel::DOUBLE, { SamaraModel::STOCKMACROPORES });
////    selector("StockRac", artis::kernel::DOUBLE, { SamaraModel::STOCKRAC });
////    selector("StockSurface", artis::kernel::DOUBLE, { SamaraModel::STOCKSURFACE });
////    selector("StockTotal", artis::kernel::DOUBLE, { SamaraModel::STOCKTOTAL });
////    selector("SumDDPhasePrec", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOURPHASEPRECEDENTE });
////    selector("SumDegresDay", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOUR });
////    selector("SupplyTot", artis::kernel::DOUBLE, { SamaraModel::SUPPLYTOT });
////    selector("TillerDeathPop", artis::kernel::DOUBLE, { SamaraModel::TILLERDEATHPOP });
////    selector("Tr", artis::kernel::DOUBLE, { SamaraModel::TR });
//    selector("TrEff", artis::kernel::DOUBLE, { SamaraModel::TREFF });
//    selector("TrEffInst", artis::kernel::DOUBLE, { SamaraModel::TREFFINST });
////    selector("TrPot", artis::kernel::DOUBLE, { SamaraModel::TRPOT });
////    selector("WueEt", artis::kernel::DOUBLE, { SamaraModel::WUEET });
//    selector("WueTot", artis::kernel::DOUBLE, { SamaraModel::WUETOT });
////    selector("ApexHeightGain", artis::kernel::DOUBLE, { SamaraModel::APEXHEIGHTGAIN });
////    selector("ConversionEff", artis::kernel::DOUBLE, { SamaraModel::CONVERSIONEFF });
////    selector("CumPAR", artis::kernel::DOUBLE, { SamaraModel::CUMPAR });
////    selector("Eto", artis::kernel::DOUBLE, { SamaraModel::ETO });
////    selector("Lr", artis::kernel::DOUBLE, { SamaraModel::LR });
////    selector("Conversion", artis::kernel::DOUBLE, { SamaraModel::CONVERSION });
////    selector("DayLength", artis::kernel::DOUBLE, { SamaraModel::DAYLENGTH });
////    selector("DegresDuJour", artis::kernel::DOUBLE, { SamaraModel::DEGRESDUJOUR });
////    selector("DegresDuJourCor", artis::kernel::DOUBLE, { SamaraModel::DEGRESDUJOURCOR });
////    selector("Density", artis::kernel::DOUBLE, { SamaraModel::DENSITY });
////    selector("EauDispo", artis::kernel::DOUBLE, { SamaraModel::EAUDISPO });
////    selector("FloodwaterDepth", artis::kernel::DOUBLE, { SamaraModel::FLOODWATERDEPTH });
////    selector("HMoyCalc", artis::kernel::DOUBLE, { SamaraModel::HMOYCALC });
////    selector("HaunIndex", artis::kernel::DOUBLE, { SamaraModel::HAUNINDEX });
////    selector("IrrigAutoDay", artis::kernel::DOUBLE, { SamaraModel::IRRIGAUTODAY });
////    selector("IrrigTotDay", artis::kernel::DOUBLE, { SamaraModel::IRRIGTOTDAY });
////    selector("KRolling", artis::kernel::DOUBLE, { SamaraModel::KROLLING });
////    selector("NbJas", artis::kernel::DOUBLE, { SamaraModel::NBJAS });
////    selector("Par", artis::kernel::DOUBLE, { SamaraModel::Par });
////    selector("PhaseStemElongation", artis::kernel::DOUBLE, { SamaraModel::PHASESTEMELONGATION });
////    selector("RayExtra", artis::kernel::DOUBLE, { SamaraModel::RAYEXTRA });
////    selector("RelPotLeafLength", artis::kernel::DOUBLE, { SamaraModel::RELPOTLEAFLENGTH });
////    selector("ResUtil", artis::kernel::DOUBLE, { SamaraModel::RESUTIL });
////    selector("RgCalc", artis::kernel::DOUBLE, { SamaraModel::RGCALC });
////    selector("RootFront", artis::kernel::DOUBLE, { SamaraModel::ROOTFRONT });
////    selector("SterilityCold", artis::kernel::DOUBLE, { SamaraModel::STERILITYCOLD });
////    selector("SterilityHeat", artis::kernel::DOUBLE, { SamaraModel::STERILITYHEAT });
////    selector("StressCold", artis::kernel::DOUBLE, { SamaraModel::STRESSCOLD });
////    selector("SumDegreDayCor", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOURCOR });
////    selector("SumPP", artis::kernel::DOUBLE, { SamaraModel::SUMPP });
////    selector("TMaxMoy", artis::kernel::DOUBLE, { SamaraModel::TMAXMOY });
////    selector("TMinMoy", artis::kernel::DOUBLE, { SamaraModel::TMINMOY });
////    selector("TMoyCalc", artis::kernel::DOUBLE, { SamaraModel::TMOYCALC });
////    selector("VDPCalc", artis::kernel::DOUBLE, { SamaraModel::VPDCALC });
////    selector("VitesseRacinaire", artis::kernel::DOUBLE, { SamaraModel::VITESSERACINAIRE });
////    selector("VolMacropores", artis::kernel::DOUBLE, { SamaraModel::VOLMACROPORES });
  }

  virtual ~GlobalView()
  { }
};

}

#endif
