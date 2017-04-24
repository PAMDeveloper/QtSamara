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

class GlobalView : public View
{
public:
    GlobalView()
    {
//        selector("LAI", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::LAI });
//        selector("NumPhase", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::NUMPHASE });
//        selector("DegresDuJour", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DEGRESDUJOUR });
//        selector("DegresDuJourCor", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DEGRESDUJOURCOR });
//        selector("FTSW", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::Ftsw });
//        selector("Cstr", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::CSTR });
//        selector("RootFront", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::ROOTFRONT });
//        selector("Eto", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::ETO });
//        selector("ETM", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::Etm });
//        selector("ETR", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::Etr });
//        selector("RUE", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::Rue });
//        selector("ConversionEff", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::CONVERSIONEFF });
//        selector("Ic", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::IC });
//        selector("CulmsPerPlant", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::CULMSPERPLANT });
//        selector("CulmsPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::CULMSPOP });
//        selector("CulmsPerHill", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::CULMSPERHILL });
//        selector("GrainYieldPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::GRAINYIELDPOP });
//        selector("DryMatStructLeafPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DRYMATSTRUCTLEAFPOP });
//        selector("DryMatStructSheathPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA,
//                         samara::SamaraModel::DRYMATSTRUCTSHEATHPOP });
//        selector("DryMatStructRootPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DRYMATSTRUCTROOTPOP });
//        selector("DryMatStructInternodePop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA,
//                         samara::SamaraModel::DRYMATSTRUCTINTERNODEPOP });
//        selector("DryMatResInternodePop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA,
//                         samara::SamaraModel::DRYMATRESINTERNODEPOP });
//        selector("DryMatStructPaniclePop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA,
//                         samara::SamaraModel::DRYMATSTRUCTPANICLEPOP });
//        selector("DryMatStructTotPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DRYMATSTRUCTTOTPOP });
//        selector("DryMatVegeTotPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DRYMATVEGETOTPOP });
//        selector("DryMatPanicleTotPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DRYMATPANICLETOTPOP });
//        selector("DryMatStemPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DRYMATSTEMPOP });
//        selector("DryMatAboveGroundPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA,
//                         samara::SamaraModel::DRYMATABOVEGROUNDPOP });
//        selector("DryMatTotPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DRYMATTOTPOP });
//        selector("SterilityCold", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::STERILITYCOLD });
//        selector("SterilityHeat", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::STERILITYHEAT });
//        selector("SterilityDrought", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::STERILITYDROUGHT });
//        selector("SterilityTot", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::STERILITYTOT });
//        selector("HarvestIndex", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::HARVESTINDEX });
//        selector("PanicleNumPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::PANICLENUMPOP });
//        selector("PanicleNumPlant", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::PANICLENUMPLANT });
//        selector("GrainYieldPanicle", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::GRAINYIELDPANICLE });
//        selector("SpikeNumPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::SPIKENUMPOP });
//        selector("SpikeNumPanicle", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::SPIKENUMPANICLE });
//        selector("FertSpikeNumPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::FERTSPIKENUMPOP });
//        selector("GrainFillingStatus", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::GRAINFILLINGSTATUS });
//        selector("PhaseStemElongation", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::PHASESTEMELONGATION });
//        selector("Sla", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::SLA });
//        selector("HaunIndex", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::HAUNINDEX });
//        selector("ApexHeight", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::APEXHEIGHT });
//        selector("PlantHeight", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::PLANTHEIGHT });
//        selector("PlantWidth", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::PLANTWIDTH });
//        selector("KRolling", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::KROLLING });
//        selector("LIRkdfcl", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::LIRKDFCL });
//        selector("LTRkdfcl", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::LTRKDFCL });
//        selector("AssimPot", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::ASSIMPOT });
//        selector("Assim", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::ASSIM });
//        selector("RespMaintTot", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::RESPMAINTTOT });
//        selector("SupplyTot", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::SUPPLYTOT });
//        selector("AssimSurplus", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::ASSIMSURPLUS });
//        selector("AssimNotUsed", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::ASSIMNOTUSED });
//        selector("TillerDeathPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::TILLERDEATHPOP });
//        selector("PlantLeafNumTot", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::PLANTLEAFNUMTOT });
//        selector("DeadLeafDrywtPop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DEADLEAFDRYWTPOP });
//        selector("LaiDead", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::LAIDEAD });
//        selector("ResCapacityInternodePop", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA,
//                         samara::SamaraModel::RESCAPACITYINTERNODEPOP });
//        selector("InternodeResStatus", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::INTERNODERESSTATUS });
//        selector("DayLength", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DAYLENGTH });
//        selector("PAR", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::Par });
//        selector("RgCalc", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::RGCALC });
//        selector("VPD", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::VPDCALC });
//        selector("TMoyCalc", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::TMOYCALC });
//        selector("HMoyCalc", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::HMOYCALC });
//        selector("KceReal", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::KCEREAL });
//        selector("StockTotal", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::STOCKTOTAL });
//        selector("EauDispo", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::EAUDISPO });
//        selector("StockSurface", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::STOCKSURFACE });
//        selector("StockRac", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::STOCKRAC });
//        selector("RuRac", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::RURAC });
//        selector("Kcp", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::KCP });
//        selector("Kce", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::KCE });
//        selector("EvapPot", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::EVAPPOT });
//        selector("Evap", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::EVAP });
//        selector("TrPot", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::TRPOT });
//        selector("Tr", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::TR });
//        selector("Lr", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::LR });
//        selector("Dr", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::DR });
//        selector("PARIntercepte", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::PARINTERCEPTE });
//        selector("SommeDegresJour", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::SOMMEDEGRESJOUR });
//        selector("VitesseRacinaire", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::VITESSERACINAIRE });
//        selector("CstrAssim", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::CSTRASSIM });
//        selector("RayExtra", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::RAYEXTRA });
//        selector("CumPar", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::CUMPAR });
//        selector("SommeDegresJourCor", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::SOMMEDEGRESJOURCOR });
//        selector("SumPP", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA, samara::SamaraModel::SUMPP });
//        selector("SommeDegresJourPhasePrecedente", artis::kernel::DOUBLE,
//                 { kernel::KernelModel::SAMARA,
//                         samara::SamaraModel::SOMMEDEGRESJOURPHASEPRECEDENTE });
//    selector("RespMaintDebt", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::RESPMAINTDEBT });
//    selector("TMinMoy", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::TMINMOY });
//    selector("TMaxMoy", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::TMAXMOY });
//    selector("FtswMoy", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::FTSWMOY });
//    selector("RootShootRatio", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::ROOTSHOOTRATIO });
//    selector("TrEffInst", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::TREFFINST });
//    selector("TrEff", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::TREFF });
//    selector("WueEt", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::WUEET });
//    selector("WueTot", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::WUETOT });
//    selector("FloodwaterDepth", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::FLOODWATERDEPTH });
//    selector("IrrigAutoDay", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::IRRIGAUTODAY });
//    selector("IrrigTotDay", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::IRRIGTOTDAY });
//    selector("FractionPlantHeightSubmer", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA,
//                     samara::SamaraModel::FRACTIONPLANTHEIGHTSUBMER });
//    selector("Density", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::DENSITY });
//    selector("RootMassPerVol", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::ROOTMASSPERVOL });
//    selector("VolMacropores", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::VOLMACROPORES });
//    selector("StockMacropores", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::STOCKMACROPORES });
//    selector("RelPotLeafLength", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::RELPOTLEAFLENGTH });
//    selector("ApexHeightGain", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::APEXHEIGHTGAIN });
//    selector("HaunGain", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::HAUNGAIN });
//    selector("Conversion", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::CONVERSION });
//    selector("StressCold", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::STRESSCOLD });
//    selector("FractionRootsLogged", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::FRACTIONROOTSLOGGED });
//    selector("ResUtil", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::RESUTIL });
//    selector("KcTot", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::KCTOT });
//    selector("NbJas", artis::kernel::DOUBLE,
//             { kernel::KernelModel::SAMARA, samara::SamaraModel::NBJAS });
    }

    virtual ~GlobalView()
    { }
};

}

#endif
