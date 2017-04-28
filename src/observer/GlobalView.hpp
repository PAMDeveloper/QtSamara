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
//    selector("ApexHeight", artis::kernel::DOUBLE, { SamaraModel::APEXHEIGHT });
//    selector("Assim", artis::kernel::DOUBLE, { SamaraModel::ASSIM });
//    selector("AssimNotUsed", artis::kernel::DOUBLE, { SamaraModel::ASSIMNOTUSED });
//    selector("AssimPot", artis::kernel::DOUBLE, { SamaraModel::ASSIMPOT });
//    selector("AssimSurplus", artis::kernel::DOUBLE, { SamaraModel::ASSIMSURPLUS });
//    selector("Cstr", artis::kernel::DOUBLE, { SamaraModel::CSTR });
//    selector("CstrAssim", artis::kernel::DOUBLE, { SamaraModel::CSTRASSIM });
//    selector("CulmsPerHill", artis::kernel::DOUBLE, { SamaraModel::CULMSPERHILL });
//    selector("CulmsPerPlant", artis::kernel::DOUBLE, { SamaraModel::CULMSPERPLANT });
//    selector("CulmsPop", artis::kernel::DOUBLE, { SamaraModel::CULMSPOP });
//    selector("DeadLeafdrywtPop", artis::kernel::DOUBLE, { SamaraModel::DEADLEAFDRYWTPOP });
//    selector("Dr", artis::kernel::DOUBLE, { SamaraModel::DR });
//    selector("DryMatAboveGroundPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATABOVEGROUNDPOP });
//    selector("DryMatPanicleTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATPANICLETOTPOP });
//    selector("DryMatResInternodePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATRESINTERNODEPOP });
//    selector("DryMatStemPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTEMPOP });
//    selector("DryMatStructInternodePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTINTERNODEPOP });
//    selector("DryMatStructLeafPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTLEAFPOP });
//    selector("DryMatStructPaniclePop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTPANICLEPOP });
//    selector("DryMatStructRootPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTROOTPOP });
//    selector("DryMatStructSheathPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTSHEATHPOP });
//    selector("DryMatStructTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATSTRUCTTOTPOP });
//    selector("DryMatTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATTOTPOP });
//    selector("DryMatVegeTotPop", artis::kernel::DOUBLE, { SamaraModel::DRYMATVEGETOTPOP });
//    selector("ETM", artis::kernel::DOUBLE, { SamaraModel::Etm });
//    selector("ETR", artis::kernel::DOUBLE, { SamaraModel::Etr });
//    selector("Evap", artis::kernel::DOUBLE, { SamaraModel::EVAP });
//    selector("EvapPot", artis::kernel::DOUBLE, { SamaraModel::EVAPPOT });
//    selector("FTSW", artis::kernel::DOUBLE, { SamaraModel::Ftsw });
//    selector("FertSpikeNumPop", artis::kernel::DOUBLE, { SamaraModel::FERTSPIKENUMPOP });
//    selector("FractionPlantHeightSubmer", artis::kernel::DOUBLE, { SamaraModel::FRACTIONPLANTHEIGHTSUBMER });
//    selector("FractionRootsLogged", artis::kernel::DOUBLE, { SamaraModel::FRACTIONROOTSLOGGED });
//    selector("FtswMoy", artis::kernel::DOUBLE, { SamaraModel::FTSWMOY });
//    selector("GrainFillingStatus", artis::kernel::DOUBLE, { SamaraModel::GRAINFILLINGSTATUS });
//    selector("GrainYieldPanicle", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPANICLE });
//    selector("GrainYieldPop", artis::kernel::DOUBLE, { SamaraModel::GRAINYIELDPOP });
//    selector("HarvestIndex", artis::kernel::DOUBLE, { SamaraModel::HARVESTINDEX });
//    selector("Haungain", artis::kernel::DOUBLE, { SamaraModel::HAUNGAIN });
//    selector("Ic", artis::kernel::DOUBLE, { SamaraModel::IC });
//    selector("InternodeResStatus", artis::kernel::DOUBLE, { SamaraModel::INTERNODERESSTATUS });
//    selector("KcTot", artis::kernel::DOUBLE, { SamaraModel::KCTOT });
//    selector("Kce", artis::kernel::DOUBLE, { SamaraModel::KCE });
//    selector("KceReal", artis::kernel::DOUBLE, { SamaraModel::KCEREAL });
//    selector("Kcp", artis::kernel::DOUBLE, { SamaraModel::KCP });
//    selector("LIRkdfcl", artis::kernel::DOUBLE, { SamaraModel::LIRKDFCL });
//    selector("LTRkdfcl", artis::kernel::DOUBLE, { SamaraModel::LTRKDFCL });
//    selector("Lai", artis::kernel::DOUBLE, { SamaraModel::LAI });
//    selector("LaiDead", artis::kernel::DOUBLE, { SamaraModel::LAIDEAD });
//    selector("NumPhase", artis::kernel::DOUBLE, { SamaraModel::NUMPHASE });
//    selector("PanicleNumPlant", artis::kernel::DOUBLE, { SamaraModel::PANICLENUMPLANT });
//    selector("PanicleNumPop", artis::kernel::DOUBLE, { SamaraModel::PANICLENUMPOP });
//    selector("ParIntercepte", artis::kernel::DOUBLE, { SamaraModel::PARINTERCEPTE });
//    selector("PlantHeight", artis::kernel::DOUBLE, { SamaraModel::PLANTHEIGHT });
//    selector("PlantLeafNumTot", artis::kernel::DOUBLE, { SamaraModel::PLANTLEAFNUMTOT });
//    selector("PlantWidth", artis::kernel::DOUBLE, { SamaraModel::PLANTWIDTH });
    selector("RUE", artis::kernel::DOUBLE, { SamaraModel::Rue });
//    selector("RURac", artis::kernel::DOUBLE, { SamaraModel::RURAC });
//    selector("ResCapacityInternodePop", artis::kernel::DOUBLE, { SamaraModel::RESCAPACITYINTERNODEPOP });
//    selector("RespMaintDebt", artis::kernel::DOUBLE, { SamaraModel::RESPMAINTDEBT });
//    selector("RespMaintTot", artis::kernel::DOUBLE, { SamaraModel::RESPMAINTTOT });
//    selector("RootMassPerVol", artis::kernel::DOUBLE, { SamaraModel::ROOTMASSPERVOL });
//    selector("RootShootRatio", artis::kernel::DOUBLE, { SamaraModel::ROOTSHOOTRATIO });
//    selector("Sla", artis::kernel::DOUBLE, { SamaraModel::SLA });
//    selector("SpikeNumPanicle", artis::kernel::DOUBLE, { SamaraModel::SPIKENUMPANICLE });
//    selector("SpikeNumPop", artis::kernel::DOUBLE, { SamaraModel::SPIKENUMPOP });
    selector("SterilityDrought", artis::kernel::DOUBLE, { SamaraModel::STERILITYDROUGHT });
    selector("SterilityTot", artis::kernel::DOUBLE, { SamaraModel::STERILITYTOT });
//    selector("StockMacropores", artis::kernel::DOUBLE, { SamaraModel::STOCKMACROPORES });
//    selector("StockRac", artis::kernel::DOUBLE, { SamaraModel::STOCKRAC });
//    selector("StockSurface", artis::kernel::DOUBLE, { SamaraModel::STOCKSURFACE });
//    selector("StockTotal", artis::kernel::DOUBLE, { SamaraModel::STOCKTOTAL });
//    selector("SumDDPhasePrec", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOURPHASEPRECEDENTE });
//    selector("SumDegresDay", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOUR });
//    selector("SupplyTot", artis::kernel::DOUBLE, { SamaraModel::SUPPLYTOT });
//    selector("TillerDeathPop", artis::kernel::DOUBLE, { SamaraModel::TILLERDEATHPOP });
//    selector("Tr", artis::kernel::DOUBLE, { SamaraModel::TR });
    selector("TrEff", artis::kernel::DOUBLE, { SamaraModel::TREFF });
    selector("TrEffInst", artis::kernel::DOUBLE, { SamaraModel::TREFFINST });
//    selector("TrPot", artis::kernel::DOUBLE, { SamaraModel::TRPOT });
    selector("WueEt", artis::kernel::DOUBLE, { SamaraModel::WUEET });
    selector("WueTot", artis::kernel::DOUBLE, { SamaraModel::WUETOT });


//    selector("ApexHeightGain", artis::kernel::DOUBLE, { SamaraModel::APEXHEIGHTGAIN });
//    selector("ConversionEff", artis::kernel::DOUBLE, { SamaraModel::CONVERSIONEFF });
//    selector("CumPAR", artis::kernel::DOUBLE, { SamaraModel::CUMPAR });

////    Give same results

//    selector("Eto", artis::kernel::DOUBLE, { SamaraModel::ETO });
//    selector("Lr", artis::kernel::DOUBLE, { SamaraModel::LR });

//    selector("Conversion", artis::kernel::DOUBLE, { SamaraModel::CONVERSION });
//    selector("DayLength", artis::kernel::DOUBLE, { SamaraModel::DAYLENGTH });
//    selector("DegresDuJour", artis::kernel::DOUBLE, { SamaraModel::DEGRESDUJOUR });
//    selector("DegresDuJourCor", artis::kernel::DOUBLE, { SamaraModel::DEGRESDUJOURCOR });
//    selector("Density", artis::kernel::DOUBLE, { SamaraModel::DENSITY });
//    selector("EauDispo", artis::kernel::DOUBLE, { SamaraModel::EAUDISPO });
//    selector("FloodwaterDepth", artis::kernel::DOUBLE, { SamaraModel::FLOODWATERDEPTH });
//    selector("HMoyCalc", artis::kernel::DOUBLE, { SamaraModel::HMOYCALC });
//    selector("HaunIndex", artis::kernel::DOUBLE, { SamaraModel::HAUNINDEX });
//    selector("IrrigAutoDay", artis::kernel::DOUBLE, { SamaraModel::IRRIGAUTODAY });
//    selector("IrrigTotDay", artis::kernel::DOUBLE, { SamaraModel::IRRIGTOTDAY });
//    selector("KRolling", artis::kernel::DOUBLE, { SamaraModel::KROLLING });
//    selector("NbJas", artis::kernel::DOUBLE, { SamaraModel::NBJAS });
//    selector("Par", artis::kernel::DOUBLE, { SamaraModel::Par });
//    selector("PhaseStemElongation", artis::kernel::DOUBLE, { SamaraModel::PHASESTEMELONGATION });
//    selector("RayExtra", artis::kernel::DOUBLE, { SamaraModel::RAYEXTRA });
//    selector("RelPotLeafLength", artis::kernel::DOUBLE, { SamaraModel::RELPOTLEAFLENGTH });
//    selector("ResUtil", artis::kernel::DOUBLE, { SamaraModel::RESUTIL });
//    selector("RgCalc", artis::kernel::DOUBLE, { SamaraModel::RGCALC });
//    selector("RootFront", artis::kernel::DOUBLE, { SamaraModel::ROOTFRONT });
//    selector("SterilityCold", artis::kernel::DOUBLE, { SamaraModel::STERILITYCOLD });
//    selector("SterilityHeat", artis::kernel::DOUBLE, { SamaraModel::STERILITYHEAT });
//    selector("StressCold", artis::kernel::DOUBLE, { SamaraModel::STRESSCOLD });
//    selector("SumDegreDayCor", artis::kernel::DOUBLE, { SamaraModel::SOMMEDEGRESJOURCOR });
//    selector("SumPP", artis::kernel::DOUBLE, { SamaraModel::SUMPP });
//    selector("TMaxMoy", artis::kernel::DOUBLE, { SamaraModel::TMAXMOY });
//    selector("TMinMoy", artis::kernel::DOUBLE, { SamaraModel::TMINMOY });
//    selector("TMoyCalc", artis::kernel::DOUBLE, { SamaraModel::TMOYCALC });
//    selector("VDPCalc", artis::kernel::DOUBLE, { SamaraModel::VPDCALC });
//    selector("VitesseRacinaire", artis::kernel::DOUBLE, { SamaraModel::VITESSERACINAIRE });
//    selector("VolMacropores", artis::kernel::DOUBLE, { SamaraModel::VOLMACROPORES });
  }

  virtual ~GlobalView()
  { }
};

}

#endif
