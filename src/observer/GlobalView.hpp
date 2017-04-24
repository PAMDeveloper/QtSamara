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
        selector("LAI", artis::kernel::DOUBLE,
        { SamaraModel::LAI });
        selector("NumPhase", artis::kernel::DOUBLE,
        { SamaraModel::NUMPHASE });
        selector("DegresDuJour", artis::kernel::DOUBLE,
        { SamaraModel::DEGRESDUJOUR });
        selector("DegresDuJourCor", artis::kernel::DOUBLE,
        { SamaraModel::DEGRESDUJOURCOR });
        selector("FTSW", artis::kernel::DOUBLE,
        { SamaraModel::Ftsw });
        selector("Cstr", artis::kernel::DOUBLE,
        { SamaraModel::CSTR });
        selector("RootFront", artis::kernel::DOUBLE,
        { SamaraModel::ROOTFRONT });
        selector("Eto", artis::kernel::DOUBLE,
        { SamaraModel::ETO });
        selector("ETM", artis::kernel::DOUBLE,
        { SamaraModel::Etm });
        selector("ETR", artis::kernel::DOUBLE,
        { SamaraModel::Etr });
        selector("RUE", artis::kernel::DOUBLE,
        { SamaraModel::Rue });
        selector("ConversionEff", artis::kernel::DOUBLE,
        { SamaraModel::CONVERSIONEFF });
        selector("Ic", artis::kernel::DOUBLE,
        { SamaraModel::IC });
        selector("CulmsPerPlant", artis::kernel::DOUBLE,
        { SamaraModel::CULMSPERPLANT });
        selector("CulmsPop", artis::kernel::DOUBLE,
        { SamaraModel::CULMSPOP });
        selector("CulmsPerHill", artis::kernel::DOUBLE,
        { SamaraModel::CULMSPERHILL });
        selector("GrainYieldPop", artis::kernel::DOUBLE,
        { SamaraModel::GRAINYIELDPOP });
        selector("DryMatStructLeafPop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATSTRUCTLEAFPOP });
        selector("DryMatStructSheathPop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATSTRUCTSHEATHPOP });
        selector("DryMatStructRootPop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATSTRUCTROOTPOP });
        selector("DryMatStructInternodePop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATSTRUCTINTERNODEPOP });
        selector("DryMatResInternodePop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATRESINTERNODEPOP });
        selector("DryMatStructPaniclePop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATSTRUCTPANICLEPOP });
        selector("DryMatStructTotPop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATSTRUCTTOTPOP });
        selector("DryMatVegeTotPop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATVEGETOTPOP });
        selector("DryMatPanicleTotPop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATPANICLETOTPOP });
        selector("DryMatStemPop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATSTEMPOP });
        selector("DryMatAboveGroundPop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATABOVEGROUNDPOP });
        selector("DryMatTotPop", artis::kernel::DOUBLE,
        { SamaraModel::DRYMATTOTPOP });
        selector("SterilityCold", artis::kernel::DOUBLE,
        { SamaraModel::STERILITYCOLD });
        selector("SterilityHeat", artis::kernel::DOUBLE,
        { SamaraModel::STERILITYHEAT });
        selector("SterilityDrought", artis::kernel::DOUBLE,
        { SamaraModel::STERILITYDROUGHT });
        selector("SterilityTot", artis::kernel::DOUBLE,
        { SamaraModel::STERILITYTOT });
        selector("HarvestIndex", artis::kernel::DOUBLE,
        { SamaraModel::HARVESTINDEX });
        selector("PanicleNumPop", artis::kernel::DOUBLE,
        { SamaraModel::PANICLENUMPOP });
        selector("PanicleNumPlant", artis::kernel::DOUBLE,
        { SamaraModel::PANICLENUMPLANT });
        selector("GrainYieldPanicle", artis::kernel::DOUBLE,
        { SamaraModel::GRAINYIELDPANICLE });
        selector("SpikeNumPop", artis::kernel::DOUBLE,
        { SamaraModel::SPIKENUMPOP });
        selector("SpikeNumPanicle", artis::kernel::DOUBLE,
        { SamaraModel::SPIKENUMPANICLE });
        selector("FertSpikeNumPop", artis::kernel::DOUBLE,
        { SamaraModel::FERTSPIKENUMPOP });
        selector("GrainFillingStatus", artis::kernel::DOUBLE,
        { SamaraModel::GRAINFILLINGSTATUS });
        selector("PhaseStemElongation", artis::kernel::DOUBLE,
        { SamaraModel::PHASESTEMELONGATION });
        selector("Sla", artis::kernel::DOUBLE,
        { SamaraModel::SLA });
        selector("HaunIndex", artis::kernel::DOUBLE,
        { SamaraModel::HAUNINDEX });
        selector("ApexHeight", artis::kernel::DOUBLE,
        { SamaraModel::APEXHEIGHT });
        selector("PlantHeight", artis::kernel::DOUBLE,
        { SamaraModel::PLANTHEIGHT });
        selector("PlantWidth", artis::kernel::DOUBLE,
        { SamaraModel::PLANTWIDTH });
        selector("KRolling", artis::kernel::DOUBLE,
        { SamaraModel::KROLLING });
        selector("LIRkdfcl", artis::kernel::DOUBLE,
        { SamaraModel::LIRKDFCL });
        selector("LTRkdfcl", artis::kernel::DOUBLE,
        { SamaraModel::LTRKDFCL });
        selector("AssimPot", artis::kernel::DOUBLE,
        { SamaraModel::ASSIMPOT });
        selector("Assim", artis::kernel::DOUBLE,
        { SamaraModel::ASSIM });
        selector("RespMaintTot", artis::kernel::DOUBLE,
        { SamaraModel::RESPMAINTTOT });
        selector("SupplyTot", artis::kernel::DOUBLE,
        { SamaraModel::SUPPLYTOT });
        selector("AssimSurplus", artis::kernel::DOUBLE,
        { SamaraModel::ASSIMSURPLUS });
        selector("AssimNotUsed", artis::kernel::DOUBLE,
        { SamaraModel::ASSIMNOTUSED });
        selector("TillerDeathPop", artis::kernel::DOUBLE,
        { SamaraModel::TILLERDEATHPOP });
        selector("PlantLeafNumTot", artis::kernel::DOUBLE,
        { SamaraModel::PLANTLEAFNUMTOT });
        selector("DeadLeafDrywtPop", artis::kernel::DOUBLE,
        { SamaraModel::DEADLEAFDRYWTPOP });
        selector("LaiDead", artis::kernel::DOUBLE,
        { SamaraModel::LAIDEAD });
        selector("ResCapacityInternodePop", artis::kernel::DOUBLE,
        { SamaraModel::RESCAPACITYINTERNODEPOP });
        selector("InternodeResStatus", artis::kernel::DOUBLE,
        { SamaraModel::INTERNODERESSTATUS });
        selector("DayLength", artis::kernel::DOUBLE,
        { SamaraModel::DAYLENGTH });
        selector("PAR", artis::kernel::DOUBLE,
        { SamaraModel::Par });
        selector("RgCalc", artis::kernel::DOUBLE,
        { SamaraModel::RGCALC });
        selector("VPD", artis::kernel::DOUBLE,
        { SamaraModel::VPDCALC });
        selector("TMoyCalc", artis::kernel::DOUBLE,
        { SamaraModel::TMOYCALC });
        selector("HMoyCalc", artis::kernel::DOUBLE,
        { SamaraModel::HMOYCALC });
        selector("KceReal", artis::kernel::DOUBLE,
        { SamaraModel::KCEREAL });
        selector("StockTotal", artis::kernel::DOUBLE,
        { SamaraModel::STOCKTOTAL });
        selector("EauDispo", artis::kernel::DOUBLE,
        { SamaraModel::EAUDISPO });
        selector("StockSurface", artis::kernel::DOUBLE,
        { SamaraModel::STOCKSURFACE });
        selector("StockRac", artis::kernel::DOUBLE,
        { SamaraModel::STOCKRAC });
        selector("RuRac", artis::kernel::DOUBLE,
        { SamaraModel::RURAC });
        selector("Kcp", artis::kernel::DOUBLE,
        { SamaraModel::KCP });
        selector("Kce", artis::kernel::DOUBLE,
        { SamaraModel::KCE });
        selector("EvapPot", artis::kernel::DOUBLE,
        { SamaraModel::EVAPPOT });
        selector("Evap", artis::kernel::DOUBLE,
        { SamaraModel::EVAP });
        selector("TrPot", artis::kernel::DOUBLE,
        { SamaraModel::TRPOT });
        selector("Tr", artis::kernel::DOUBLE,
        { SamaraModel::TR });
        selector("Lr", artis::kernel::DOUBLE,
        { SamaraModel::LR });
        selector("Dr", artis::kernel::DOUBLE,
        { SamaraModel::DR });
        selector("PARIntercepte", artis::kernel::DOUBLE,
        { SamaraModel::PARINTERCEPTE });
        selector("SommeDegresJour", artis::kernel::DOUBLE,
        { SamaraModel::SOMMEDEGRESJOUR });
        selector("VitesseRacinaire", artis::kernel::DOUBLE,
        { SamaraModel::VITESSERACINAIRE });
        selector("CstrAssim", artis::kernel::DOUBLE,
        { SamaraModel::CSTRASSIM });
        selector("RayExtra", artis::kernel::DOUBLE,
        { SamaraModel::RAYEXTRA });
        selector("CumPar", artis::kernel::DOUBLE,
        { SamaraModel::CUMPAR });
        selector("SommeDegresJourCor", artis::kernel::DOUBLE,
        { SamaraModel::SOMMEDEGRESJOURCOR });
        selector("SumPP", artis::kernel::DOUBLE,
        { SamaraModel::SUMPP });
        selector("SommeDegresJourPhasePrecedente", artis::kernel::DOUBLE,
        { SamaraModel::SOMMEDEGRESJOURPHASEPRECEDENTE });
        selector("RespMaintDebt", artis::kernel::DOUBLE,
        { SamaraModel::RESPMAINTDEBT });
        selector("TMinMoy", artis::kernel::DOUBLE,
        { SamaraModel::TMINMOY });
        selector("TMaxMoy", artis::kernel::DOUBLE,
        { SamaraModel::TMAXMOY });
        selector("FtswMoy", artis::kernel::DOUBLE,
        { SamaraModel::FTSWMOY });
        selector("RootShootRatio", artis::kernel::DOUBLE,
        { SamaraModel::ROOTSHOOTRATIO });
        selector("TrEffInst", artis::kernel::DOUBLE,
        { SamaraModel::TREFFINST });
        selector("TrEff", artis::kernel::DOUBLE,
        { SamaraModel::TREFF });
        selector("WueEt", artis::kernel::DOUBLE,
        { SamaraModel::WUEET });
        selector("WueTot", artis::kernel::DOUBLE,
        { SamaraModel::WUETOT });
        selector("FloodwaterDepth", artis::kernel::DOUBLE,
        { SamaraModel::FLOODWATERDEPTH });
        selector("IrrigAutoDay", artis::kernel::DOUBLE,
        { SamaraModel::IRRIGAUTODAY });
        selector("IrrigTotDay", artis::kernel::DOUBLE,
        { SamaraModel::IRRIGTOTDAY });
        selector("FractionPlantHeightSubmer", artis::kernel::DOUBLE,
        { SamaraModel::FRACTIONPLANTHEIGHTSUBMER });
        selector("Density", artis::kernel::DOUBLE,
        { SamaraModel::DENSITY });
        selector("RootMassPerVol", artis::kernel::DOUBLE,
        { SamaraModel::ROOTMASSPERVOL });
        selector("VolMacropores", artis::kernel::DOUBLE,
        { SamaraModel::VOLMACROPORES });
        selector("StockMacropores", artis::kernel::DOUBLE,
        { SamaraModel::STOCKMACROPORES });
        selector("RelPotLeafLength", artis::kernel::DOUBLE,
        { SamaraModel::RELPOTLEAFLENGTH });
        selector("ApexHeightGain", artis::kernel::DOUBLE,
        { SamaraModel::APEXHEIGHTGAIN });
        selector("HaunGain", artis::kernel::DOUBLE,
        { SamaraModel::HAUNGAIN });
        selector("Conversion", artis::kernel::DOUBLE,
        { SamaraModel::CONVERSION });
        selector("StressCold", artis::kernel::DOUBLE,
        { SamaraModel::STRESSCOLD });
        selector("FractionRootsLogged", artis::kernel::DOUBLE,
        { SamaraModel::FRACTIONROOTSLOGGED });
        selector("ResUtil", artis::kernel::DOUBLE,
        { SamaraModel::RESUTIL });
        selector("KcTot", artis::kernel::DOUBLE,
        { SamaraModel::KCTOT });
        selector("NbJas", artis::kernel::DOUBLE,
        { SamaraModel::NBJAS });
    }

    virtual ~GlobalView()
    { }
};

}

#endif
