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

        //    /********** NO REFERENCE**********/
        //      selector("CapaRDE",artis::kernel::DOUBLE, {SamaraModel::CAPARDE});
        //      selector("CapaREvap",artis::kernel::DOUBLE, {SamaraModel::CAPAREVAP});
        //      selector("CapaRFE",artis::kernel::DOUBLE, {SamaraModel::CAPARFE});
        //      selector("ChangePhase",artis::kernel::DOUBLE, {SamaraModel::CHANGEPHASE});
        //      selector("CoeffStressLogging",artis::kernel::DOUBLE, {SamaraModel::COEFFSTRESSLOGGING});
        //      selector("CounterNursery",artis::kernel::DOUBLE, {SamaraModel::COUNTERNURSERY});
        //      selector("DAF",artis::kernel::DOUBLE, {SamaraModel::_DAF});
        //      selector("FloodwaterGain",artis::kernel::DOUBLE, {SamaraModel::FLOODWATERGAIN});
        //      selector("GrowthDryMatPop",artis::kernel::DOUBLE, {SamaraModel::GROWTHDRYMATPOP});
        //      selector("NumSsPhase",artis::kernel::DOUBLE, {SamaraModel::NUMSOUSPHASE});
        //      selector("SommeDegresJourMax",artis::kernel::DOUBLE, {SamaraModel::SOMMEDEGRESJOURMAXIMALE});
        //      selector("SeuilTempPhaseSuivante",artis::kernel::DOUBLE, {SamaraModel::SEUILTEMPPHASESUIVANTE});
        //      selector("SeuilTempSousPhaseSuivante",artis::kernel::DOUBLE, {SamaraModel::SEUILTEMPSOUSPHASESUIVANTE});
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

        //      /********** FLAT REFERENCE **********/
        //      selector("VDPCalc", artis::kernel::DOUBLE, { SamaraModel::_VPD });
        //      selector("ApexHeightGain", artis::kernel::DOUBLE, { SamaraModel::APEXHEIGHTGAIN });
        //      selector("CumPAR", artis::kernel::DOUBLE, { SamaraModel::CUMPAR });
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

        //      /********** DOUBLE FLAT **********/
        //      selector("CumIrrig", artis::kernel::DOUBLE, { SamaraModel::CUMIRRIG });
        //      selector("CumIrrigFin", artis::kernel::DOUBLE, { SamaraModel::CUMIRRIGFIN });
        //      selector("FloodwaterDepth", artis::kernel::DOUBLE, { SamaraModel::FLOODWATERDEPTH });
        //      selector("FractionPlantHeightSubmer", artis::kernel::DOUBLE, { SamaraModel::FRACTIONPLANTHEIGHTSUBMER });
        //      selector("IrrigAutoDay", artis::kernel::DOUBLE, { SamaraModel::IRRIGAUTODAY });
        //      selector("IrrigTotDay", artis::kernel::DOUBLE, { SamaraModel::IRRIGTOTDAY });
        //      selector("Kcl", artis::kernel::DOUBLE, { SamaraModel::KCL });
        //      selector("SimEndCycle", artis::kernel::DOUBLE, { SamaraModel::SIMENDCYCLE });
        //      selector("SimStartGermin", artis::kernel::DOUBLE, { SamaraModel::SIMSTARTGERMIN });
        //      selector("SterilityCold", artis::kernel::DOUBLE, { SamaraModel::STERILITYCOLD });
        //      selector("SterilityHeat", artis::kernel::DOUBLE, { SamaraModel::STERILITYHEAT });


        /********** EXACT **********/
              selector("CoeffCO2Assim", artis::kernel::DOUBLE, { SamaraModel::COEFFCO2ASSIM });
              selector("CoeffCO2Tr", artis::kernel::DOUBLE, { SamaraModel::COEFFCO2TR });
              selector("CumLr", artis::kernel::DOUBLE, { SamaraModel::CUMLR });
              selector("CumWReceived", artis::kernel::DOUBLE, { SamaraModel::CUMWRECEIVED });
              selector("Density", artis::kernel::DOUBLE, { SamaraModel::DENSITY });
//              selector("DurGermFlow", artis::kernel::DOUBLE, { SamaraModel::DURGERMFLOW });
//              selector("DurGermMat", artis::kernel::DOUBLE, { SamaraModel::DURGERMMAT });
//              selector("EauDispo", artis::kernel::DOUBLE, { SamaraModel::EAUDISPO });
//              selector("HMoyCalc", artis::kernel::DOUBLE, { SamaraModel::HMOYCALC });
              selector("KRolling", artis::kernel::DOUBLE, { SamaraModel::KROLLING });
              selector("Lr", artis::kernel::DOUBLE, { SamaraModel::LR });
//              selector("NbJas", artis::kernel::DOUBLE, { SamaraModel::NBJAS });
//              selector("NumPhase", artis::kernel::DOUBLE, { SamaraModel::NUMPHASE });
//              selector("Par", artis::kernel::DOUBLE, { SamaraModel::_PAR });
//              selector("PhaseStemElongation", artis::kernel::DOUBLE, { SamaraModel::PHASESTEMELONGATION });
//              selector("ResUtil", artis::kernel::DOUBLE, { SamaraModel::RESUTIL });
//              selector("RgCalc", artis::kernel::DOUBLE, { SamaraModel::RGCALC });
//              selector("RURac", artis::kernel::DOUBLE, { SamaraModel::RURAC });
//              selector("RootFront", artis::kernel::DOUBLE, { SamaraModel::ROOTFRONT });
              selector("StressCold", artis::kernel::DOUBLE, { SamaraModel::STRESSCOLD });
              selector("TMoyCalc", artis::kernel::DOUBLE, { SamaraModel::TMOYCALC });
              selector("VitesseRacinaire", artis::kernel::DOUBLE, { SamaraModel::VITESSERACINAIRE });
              selector("VolMacropores", artis::kernel::DOUBLE, { SamaraModel::VOLMACROPORES });

        /********** DIVERGENT SUM < 1% **********/
            selector("LastLeafLengthPot", artis::kernel::DOUBLE, { SamaraModel::LASTLEAFLENGTHPOT });
            selector("Tr", artis::kernel::DOUBLE, { SamaraModel::TR });
            selector("SumDegresDay", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOUR });
            selector("Sla", artis::kernel::DOUBLE, { SamaraModel::SLA });
            selector("KceReal", artis::kernel::DOUBLE, { SamaraModel::KCEREAL });
            selector("CumWUseFin", artis::kernel::DOUBLE, { SamaraModel::CUMWUSEFIN });
            selector("CumTr", artis::kernel::DOUBLE, { SamaraModel::CUMTR });
            selector("A_DemStructLeaf", artis::kernel::DOUBLE, { SamaraModel::A_DEMSTRUCTLEAF });
            selector("CumEt", artis::kernel::DOUBLE, { SamaraModel::CUMET });
            selector("ApexHeight", artis::kernel::DOUBLE, { SamaraModel::APEXHEIGHT });
            selector("Conversion", artis::kernel::DOUBLE, { SamaraModel::CONVERSION });
            selector("Cstr", artis::kernel::DOUBLE, { SamaraModel::CSTR });
            selector("CstrAssim", artis::kernel::DOUBLE, { SamaraModel::CSTRASSIM });
            selector("CstrCum", artis::kernel::DOUBLE, { SamaraModel::CSTRCUMUL });
            selector("CulmsPerHill", artis::kernel::DOUBLE, { SamaraModel::CULMSPERHILL });
            selector("CulmsPerHillFin", artis::kernel::DOUBLE, { SamaraModel::CULMSPERHILLFIN });
            selector("CulmsPerHillMax", artis::kernel::DOUBLE, { SamaraModel::CULMSPERHILLMAX });
            selector("CulmsPerPlant", artis::kernel::DOUBLE, { SamaraModel::CULMSPERPLANT });
//            selector("CulmsPerPlantFin", artis::kernel::DOUBLE, { SamaraModel::CULMSPERPLANTFIN });
//            selector("CulmsPerPlantMax", artis::kernel::DOUBLE, { SamaraModel::CULMSPERPLANTMAX });
//            selector("CulmsPop", artis::kernel::DOUBLE, { SamaraModel::CULMSPOP });
//            selector("DayLength", artis::kernel::DOUBLE, { SamaraModel::DAYLENGTH });
//            selector("DegresDuJour", artis::kernel::DOUBLE, { SamaraModel::DEGRESDUJOUR });
//            selector("DegresDuJourCor", artis::kernel::DOUBLE, { SamaraModel::DEGRESDUJOURCOR });
//            selector("Dr", artis::kernel::DOUBLE, { SamaraModel::DR });
//            selector("DryMatStructRootPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTROOTPOP });
//            selector("Eto", artis::kernel::DOUBLE, { SamaraModel::ETO });
//            selector("ETR", artis::kernel::DOUBLE, { SamaraModel::_ETR });
//            selector("Evap", artis::kernel::DOUBLE, { SamaraModel::EVAP });
//            selector("EvapPot", artis::kernel::DOUBLE, { SamaraModel::EVAPPOT });
//            selector("FractionRootsLogged", artis::kernel::DOUBLE, { SamaraModel::FRACTIONROOTSLOGGED });
//            selector("FTSW", artis::kernel::DOUBLE, { SamaraModel::_FTSW });
//            selector("FtswMoy", artis::kernel::DOUBLE, { SamaraModel::FTSWMOY });
//            selector("HarvestIndex", artis::kernel::DOUBLE, { SamaraModel::HARVESTINDEX });
//            selector("HaunIndex", artis::kernel::DOUBLE, { SamaraModel::HAUNINDEX });
//            selector("KcTot", artis::kernel::DOUBLE, { SamaraModel::KCTOT });
//            selector("PanicleNumPlant", artis::kernel::DOUBLE, { SamaraModel::PANICLENUMPLANT });
            selector("PanicleNumPop", artis::kernel::DOUBLE, { SamaraModel::PANICLENUMPOP });
            selector("PlantHeight", artis::kernel::DOUBLE, { SamaraModel::PLANTHEIGHT });
            selector("PlantLeafNumTot", artis::kernel::DOUBLE, { SamaraModel::PLANTLEAFNUMTOT });
            selector("PlantWidth", artis::kernel::DOUBLE, { SamaraModel::PLANTWIDTH });
//            selector("RayExtra", artis::kernel::DOUBLE, { SamaraModel::RAYEXTRA });
//            selector("RelPotLeafLength", artis::kernel::DOUBLE, { SamaraModel::RELPOTLEAFLENGTH });
//            selector("RootMassPerVol", artis::kernel::DOUBLE, { SamaraModel::ROOTMASSPERVOL });
//            selector("SeuilTemp",artis::kernel::DOUBLE, {SamaraModel::SEUILTEMPPHASEPREC});
//            selector("SumDegreDayCor", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOURCOR });
//            selector("SumDDPhasePrec", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOURPHASEPRECEDENTE });
//            selector("StockMacropores", artis::kernel::DOUBLE, { SamaraModel::STOCKMACROPORES });
//            selector("StockRac", artis::kernel::DOUBLE, { SamaraModel::STOCKRAC });
//            selector("StockSurface", artis::kernel::DOUBLE, { SamaraModel::STOCKSURFACE });
//            selector("StockTotal", artis::kernel::DOUBLE, { SamaraModel::STOCKTOTAL });
//            selector("SumPP", artis::kernel::DOUBLE, { SamaraModel::SUMPP });
//            selector("TillerDeathPop", artis::kernel::DOUBLE, { SamaraModel::TILLERDEATHPOP });
//            selector("TMaxMoy", artis::kernel::DOUBLE, { SamaraModel::TMAXMOY });
//            selector("TMinMoy", artis::kernel::DOUBLE, { SamaraModel::TMINMOY });
//            selector("CumWUse", artis::kernel::DOUBLE, { SamaraModel::CUMWUSE });
//            selector("A_DemStructTot", artis::kernel::DOUBLE, { SamaraModel::A_DEMSTRUCTTOT });
//            selector("ConversionEff", artis::kernel::DOUBLE, { SamaraModel::CONVERSIONEFF });
//            selector("CstrMean", artis::kernel::DOUBLE, { SamaraModel::CSTRMEAN });
//            selector("ETM", artis::kernel::DOUBLE, { SamaraModel::_ETM });
//            selector("IcMean", artis::kernel::DOUBLE, { SamaraModel::ICMEAN });
//            selector("InternodeResStatus", artis::kernel::DOUBLE, { SamaraModel::INTERNODERESSTATUS });
//            selector("Kce", artis::kernel::DOUBLE, { SamaraModel::KCE });
//            selector("LastLeafLength", artis::kernel::DOUBLE, { SamaraModel::LASTLEAFLENGTH });
//            selector("LTRkdfcl", artis::kernel::DOUBLE, { SamaraModel::LTRKDFCL });
//            selector("RUE", artis::kernel::DOUBLE, { SamaraModel::_RUE });

            /********** DIVERGENT > 1% **********/
            selector("CumDr", artis::kernel::DOUBLE, { SamaraModel::CUMDR });
            selector("A_GrowthStructLeaf", artis::kernel::DOUBLE, { SamaraModel::A_GROWTHSTRUCTLEAF });
            selector("A_GrowthStructTot", artis::kernel::DOUBLE, { SamaraModel::A_GROWTHSTRUCTTOT });
            selector("Assim", artis::kernel::DOUBLE, { SamaraModel::ASSIM });
            selector("AssimPot", artis::kernel::DOUBLE, { SamaraModel::ASSIMPOT });
//            selector("MaxLai", artis::kernel::DOUBLE, { SamaraModel::MAXIMUMLAI });
//            selector("RootShootRatio", artis::kernel::DOUBLE, { SamaraModel::ROOTSHOOTRATIO });
//            selector("CumCarbonUsedPop", artis::kernel::DOUBLE, { SamaraModel::CUMCARBONUSEDPOP });
//            selector("CumGrowthPop", artis::kernel::DOUBLE, { SamaraModel::CUMGROWTHPOP });
//            selector("CumSupplyTot", artis::kernel::DOUBLE, { SamaraModel::CUMSUPPLYTOT });
//            selector("DryMatAboveGroundPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATABOVEGROUNDPOP });
//            selector("DryMatAboveGroundTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATABOVEGROUNDTOTPOP });
//            selector("DryMatPanicleTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATPANICLETOTPOP });
//            selector("DryMatResInternodePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATRESINTERNODEPOP });
//            selector("DryMatResInternodePopOld", artis::kernel::DOUBLE, { SamaraModel::DRYMATRESINTERNODEPOPOLD });
//            selector("DryMatStemPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTEMPOP });
//            selector("DryMatStructInternodePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTINTERNODEPOP });
//            selector("DryMatStructLeafPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTLEAFPOP });
//            selector("DryMatStructPaniclePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTPANICLEPOP });
//            selector("DryMatStructSheathPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTSHEATHPOP });
//            selector("DryMatStructTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTTOTPOP });
//            selector("DryMatTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATTOTPOP });
//            selector("DryMatVegeTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATVEGETOTPOP });
//            selector("FertSpikeNumPop", artis::kernel::DOUBLE, { SamaraModel::FERTSPIKENUMPOP });
//            selector("GrainFillingStatus", artis::kernel::DOUBLE, { SamaraModel::GRAINFILLINGSTATUS });
//            selector("GrainYieldPanicle", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPANICLE });
//            selector("GrainYieldPop", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPOP });
            selector("Ic", artis::kernel::DOUBLE, { SamaraModel::IC });
            selector("IcCum", artis::kernel::DOUBLE, { SamaraModel::ICCUMUL });
            selector("Kcp", artis::kernel::DOUBLE, { SamaraModel::KCP });
            selector("Lai", artis::kernel::DOUBLE, { SamaraModel::LAI });
            selector("LIRkdfcl", artis::kernel::DOUBLE, { SamaraModel::LIRKDFCL });
            selector("ParIntercepte", artis::kernel::DOUBLE, { SamaraModel::PARINTERCEPTE });
//            selector("ResCapacityInternodePop", artis::kernel::DOUBLE, { SamaraModel::RESCAPACITYINTERNODEPOP });
//            selector("SpikeNumPanicle", artis::kernel::DOUBLE, { SamaraModel::SPIKENUMPANICLE });
//            selector("SpikeNumPop", artis::kernel::DOUBLE, { SamaraModel::SPIKENUMPOP });
//            selector("SupplyTot", artis::kernel::DOUBLE, { SamaraModel::SUPPLYTOT });
//            selector("TrEff", artis::kernel::DOUBLE, { SamaraModel::TREFF });
//            selector("TrPot", artis::kernel::DOUBLE, { SamaraModel::TRPOT });
//            selector("WueEt", artis::kernel::DOUBLE, { SamaraModel::WUEET });
//            selector("WueTot", artis::kernel::DOUBLE, { SamaraModel::WUETOT });

        /********** DIVERGENT SUM > 5% **********/
        selector("AssimNotUsed", artis::kernel::DOUBLE, { SamaraModel::ASSIMNOTUSED });
        selector("DeadLeafdrywtPop", artis::kernel::DOUBLE, { SamaraModel::DEADLEAFDRYWTPOP });
        selector("DryMatTotPopFin", artis::kernel::DOUBLE, { SamaraModel::DRYMATTOTPOPFIN });
        selector("GrainFillingStatusFin", artis::kernel::DOUBLE, { SamaraModel::GRAINFILLINGSTATUSFIN });
        selector("GrainYieldPopFin", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPOPFIN });
        selector("GrowthPop", artis::kernel::DOUBLE, { SamaraModel::GROWTHPOP });
        selector("GrowthDryMatPop_V2_1", artis::kernel::DOUBLE, { SamaraModel::GROWTHDRYMATPOP });
        selector("RespMaintTot", artis::kernel::DOUBLE, { SamaraModel::RESPMAINTTOT });
        selector("TrEffInst", artis::kernel::DOUBLE, { SamaraModel::TREFFINST });

        /********** DIVERGENT SUM > 10% **********/
        selector("DryMatAboveGroundPopFin", artis::kernel::DOUBLE, { SamaraModel::DRYMATABOVEGROUNDPOPFIN });
        selector("A_AssimSurplus", artis::kernel::DOUBLE, { SamaraModel::A_ASSIMSURPLUS });
        selector("A_IncreaseResInternodePop", artis::kernel::DOUBLE, { SamaraModel::A_INCREASERESINTERNODEPOP });
//        selector("A_ResInternodeMobiliDay", artis::kernel::DOUBLE, { SamaraModel::A_RESINTERNODEMOBILIDAY });
//        selector("AssimNotUsedCum", artis::kernel::DOUBLE, { SamaraModel::ASSIMNOTUSEDCUM });
//        selector("AssimSurplus", artis::kernel::DOUBLE, { SamaraModel::ASSIMSURPLUS });
//        selector("DemResInternodePop", artis::kernel::DOUBLE, { SamaraModel::DEMRESINTERNODEPOP });
//        selector("LaiDead", artis::kernel::DOUBLE, { SamaraModel::LAIDEAD });
//        selector("LaiFin", artis::kernel::DOUBLE, { SamaraModel::LAIFIN });
//        selector("SterilityDrought", artis::kernel::DOUBLE, { SamaraModel::STERILITYDROUGHT });
//        selector("SterilityTot", artis::kernel::DOUBLE, { SamaraModel::STERILITYTOT });
//        selector("SterilityTotFin", artis::kernel::DOUBLE, { SamaraModel::STERILITYTOTFIN });
//        selector("RespMaintDebt", artis::kernel::DOUBLE, { SamaraModel::RESPMAINTDEBT });
//        selector("ReservePopFin", artis::kernel::DOUBLE, { SamaraModel::RESERVEPOPFIN });

    }

    virtual ~GlobalView()
    { }
};

}

#endif
