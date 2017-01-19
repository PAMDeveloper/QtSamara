/**
 * @file model/observer/LAIView.hpp
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

#include <artis/kernel/Value.hpp>

#include <model/kernel/Model.hpp>
#include <model/models/samara/Model.hpp>
#include <model/observer/View.hpp>

namespace model { namespace observer {

class GlobalView : public model::observer::View
{
public:
    GlobalView()
    {
        selector("LAI", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::LAI });
        selector("NumPhase", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::NUMPHASE });
        selector("DegresDuJour", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DEGRESDUJOUR });
        selector("DegresDuJourCor", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DEGRESDUJOURCOR });
        selector("FTSW", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::Ftsw });
        selector("Cstr", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::CSTR });
        selector("RootFront", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::ROOTFRONT });
        selector("Eto", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::ETO });
        selector("ETM", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::Etm });
        selector("ETR", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::Etr });
        selector("RUE", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::Rue });
        selector("ConversionEff", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::CONVERSIONEFF });
        selector("Ic", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::IC });
        selector("CulmsPerPlant", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::CULMSPERPLANT });
        selector("CulmsPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::CULMSPOP });
        selector("CulmsPerHill", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::CULMSPERHILL });
        selector("GrainYieldPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::GRAINYIELDPOP });
        selector("DryMatStructLeafPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DRYMATSTRUCTLEAFPOP });
        selector("DryMatStructSheathPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA,
                         samara::Model::DRYMATSTRUCTSHEATHPOP });
        selector("DryMatStructRootPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DRYMATSTRUCTROOTPOP });
        selector("DryMatStructInternodePop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA,
                         samara::Model::DRYMATSTRUCTINTERNODEPOP });
        selector("DryMatResInternodePop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA,
                         samara::Model::DRYMATRESINTERNODEPOP });
        selector("DryMatStructPaniclePop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA,
                         samara::Model::DRYMATSTRUCTPANICLEPOP });
        selector("DryMatStructTotPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DRYMATSTRUCTTOTPOP });
        selector("DryMatVegeTotPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DRYMATVEGETOTPOP });
        selector("DryMatPanicleTotPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DRYMATPANICLETOTPOP });
        selector("DryMatStemPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DRYMATSTEMPOP });
        selector("DryMatAboveGroundPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA,
                         samara::Model::DRYMATABOVEGROUNDPOP });
        selector("DryMatTotPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DRYMATTOTPOP });
        selector("SterilityCold", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::STERILITYCOLD });
        selector("SterilityHeat", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::STERILITYHEAT });
        selector("SterilityDrought", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::STERILITYDROUGHT });
        selector("SterilityTot", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::STERILITYTOT });
        selector("HarvestIndex", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::HARVESTINDEX });
        selector("PanicleNumPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::PANICLENUMPOP });
        selector("PanicleNumPlant", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::PANICLENUMPLANT });
        selector("GrainYieldPanicle", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::GRAINYIELDPANICLE });
        selector("SpikeNumPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::SPIKENUMPOP });
        selector("SpikeNumPanicle", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::SPIKENUMPANICLE });
        selector("FertSpikeNumPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::FERTSPIKENUMPOP });
        selector("GrainFillingStatus", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::GRAINFILLINGSTATUS });
        selector("PhaseStemElongation", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::PHASESTEMELONGATION });
        selector("Sla", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::SLA });
        selector("HaunIndex", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::HAUNINDEX });
        selector("ApexHeight", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::APEXHEIGHT });
        selector("PlantHeight", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::PLANTHEIGHT });
        selector("PlantWidth", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::PLANTWIDTH });
        selector("KRolling", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::KROLLING });
        selector("LIRkdfcl", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::LIRKDFCL });
        selector("LTRkdfcl", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::LTRKDFCL });
        selector("AssimPot", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::ASSIMPOT });
        selector("Assim", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::ASSIM });
        selector("RespMaintTot", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::RESPMAINTTOT });
        selector("SupplyTot", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::SUPPLYTOT });
        selector("AssimSurplus", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::ASSIMSURPLUS });
        selector("AssimNotUsed", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::ASSIMNOTUSED });
        selector("TillerDeathPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::TILLERDEATHPOP });
        selector("PlantLeafNumTot", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::PLANTLEAFNUMTOT });
        selector("DeadLeafDrywtPop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DEADLEAFDRYWTPOP });
        selector("LaiDead", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::LAIDEAD });
        selector("ResCapacityInternodePop", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA,
                         samara::Model::RESCAPACITYINTERNODEPOP });
        selector("InternodeResStatus", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::INTERNODERESSTATUS });
        selector("DayLength", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DAYLENGTH });
        selector("PAR", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::Par });
        selector("RgCalc", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::RGCALC });
        selector("VPD", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::VPDCALC });
        selector("TMoyCalc", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::TMOYCALC });
        selector("HMoyCalc", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::HMOYCALC });
        selector("KceReal", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::KCEREAL });
        selector("StockTotal", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::STOCKTOTAL });
        selector("EauDispo", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::EAUDISPO });
        selector("StockSurface", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::STOCKSURFACE });
        selector("StockRac", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::STOCKRAC });
        selector("RuRac", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::RURAC });
        selector("Kcp", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::KCP });
        selector("Kce", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::KCE });
        selector("EvapPot", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::EVAPPOT });
        selector("Evap", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::EVAP });
        selector("TrPot", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::TRPOT });
        selector("Tr", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::TR });
        selector("Lr", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::LR });
        selector("Dr", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::DR });
        selector("PARIntercepte", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::PARINTERCEPTE });
        selector("SommeDegresJour", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::SOMMEDEGRESJOUR });
        selector("VitesseRacinaire", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::VITESSERACINAIRE });
        selector("CstrAssim", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::CSTRASSIM });
        selector("RayExtra", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::RAYEXTRA });
        selector("CumPar", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::CUMPAR });
        selector("SommeDegresJourCor", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::SOMMEDEGRESJOURCOR });
        selector("SumPP", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA, samara::Model::SUMPP });
        selector("SommeDegresJourPhasePrecedente", artis::kernel::DOUBLE,
                 { kernel::Model::SAMARA,
                         samara::Model::SOMMEDEGRESJOURPHASEPRECEDENTE });
    selector("RespMaintDebt", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::RESPMAINTDEBT });
    selector("TMinMoy", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::TMINMOY });
    selector("TMaxMoy", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::TMAXMOY });
    selector("FtswMoy", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::FTSWMOY });
    selector("RootShootRatio", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::ROOTSHOOTRATIO });
    selector("TrEffInst", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::TREFFINST });
    selector("TrEff", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::TREFF });
    selector("WueEt", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::WUEET });
    selector("WueTot", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::WUETOT });
    selector("FloodwaterDepth", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::FLOODWATERDEPTH });
    selector("IrrigAutoDay", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::IRRIGAUTODAY });
    selector("IrrigTotDay", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::IRRIGTOTDAY });
    selector("FractionPlantHeightSubmer", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA,
                     samara::Model::FRACTIONPLANTHEIGHTSUBMER });
    selector("Density", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::DENSITY });
    selector("RootMassPerVol", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::ROOTMASSPERVOL });
    selector("VolMacropores", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::VOLMACROPORES });
    selector("StockMacropores", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::STOCKMACROPORES });
    selector("RelPotLeafLength", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::RELPOTLEAFLENGTH });
    selector("ApexHeightGain", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::APEXHEIGHTGAIN });
    selector("HaunGain", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::HAUNGAIN });
    selector("Conversion", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::CONVERSION });
    selector("StressCold", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::STRESSCOLD });
    selector("FractionRootsLogged", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::FRACTIONROOTSLOGGED });
    selector("ResUtil", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::RESUTIL });
    selector("KcTot", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::KCTOT });
    selector("NbJas", artis::kernel::DOUBLE,
             { kernel::Model::SAMARA, samara::Model::NBJAS });
    }

    virtual ~GlobalView()
    { }
};

} }

#endif
