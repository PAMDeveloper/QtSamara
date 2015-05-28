/**
 * @file model/models/samara/Risocas.hpp
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

#include <model/models/samara/Model.hpp>
#include <model/models/samara/Model2.hpp>
#include <model/models/samara/Model2_1.hpp>
#include <model/models/samara/Model2_2.hpp>

#include <utils/DateTime.hpp>

#include <algorithm>
#include <cmath>

namespace model { namespace models { namespace samara {

void Model::evalDegreeDay()
{
    double V, V1, V3;
    double S1, S2, S3;
    double Tn, Tx;

    if (TMax != TMin) {
        if (TMax <= TBase or TMin >= TLet) {
            V = 0;
        } else {
            Tn = std::max(TMin, TBase);
            Tx = std::min(TMax, TLet);
            V1 = ((Tn + std::min(TOpt1, Tx)) / 2 - TBase) / (TOpt1 - TBase);
            S1 = V1 * std::max(0., std::min(TOpt1, Tx) - Tn);
            S2 = 1 * std::max(0., std::min(Tx, TOpt2) - std::max(Tn, TOpt1));
            V3 = (TLet - (std::max(Tx, TOpt2) + std::max(TOpt2, Tn)) / 2) /
                (TLet - TOpt2);
            S3 = V3 * std::max(0., Tx - std::max(TOpt2, Tn));
            V = (S1 + S2 + S3) / (TMax - TMin);
        }
    } else {
        if (TMax < TOpt1) {
            V = (TMax - TBase) / (TOpt1 - TBase);
        } else {
            if (TMax < TOpt2) {
                V = 1;
            } else {
                V = (TLet - TMax) / (TLet - TOpt2);
            }
        }
    }
    DegresDuJour = V * (TOpt1 - TBase);
    if (NumPhase > 1 and NumPhase < 5)
    {
        DegresDuJourCor = DegresDuJour * std::pow(std::max(Cstr, 0.00000001),
                                                  DEVcstr);
    } else {
        DegresDuJourCor = DegresDuJour;
    }
    DegresDuJourCor = DegresDuJourCor * StressCold;
    // std::cout << "TMin = " << TMin << std::endl;
    // std::cout << "TMax = " << TMax << std::endl;
    // std::cout << "TBase = " << TBase << std::endl;
    // std::cout << "TLet = " << TLet << std::endl;
    // std::cout << "TOpt1 = " << TOpt1 << std::endl;
    // std::cout << "TOpt2 = " << TOpt2 << std::endl;
    // std::cout << "DegresDuJour = " << DegresDuJour << std::endl;
    // std::cout << "DegresDuJourCor = " << DegresDuJourCor << std::endl;
}


void Model::phyllochron()
{
    if (NumPhase > 1 and NumPhase < 5) {
        if ((NumPhase > 3 or HaunIndex > 20) and NumPhase < 5) {
            PhaseStemElongation = 1;
        } else {
            PhaseStemElongation = 0;
        }

        if (PhaseStemElongation == 0)  {
            HaunGain = DegresDuJourCor / Phyllo;
            if (HaunIndex < 3) {
                HaunGain = HaunGain * 2;
            }
        } else {
            if (PhaseStemElongation == 1) {
                HaunGain = RelPhylloPhaseStemElong * (DegresDuJourCor / Phyllo);
            }
        }
        HaunIndex = HaunIndex + HaunGain;
    } else if (NumPhase == 7 and ChangePhase == 1){
        HaunIndex = 0;
    } else {
        HaunGain = 0;
        PhaseStemElongation = 0;
    }

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "Phyllo = " << Phyllo << std::endl;
    // std::cout << "RelPhylloPhaseStemElong = " << RelPhylloPhaseStemElong << std::endl;
    // std::cout << "DegresDuJourCor = " << DegresDuJourCor << std::endl;
    // std::cout << "PhaseStemElongation = " << PhaseStemElongation << std::endl;
    // std::cout << "HaunGain = " << HaunGain << std::endl;
    // std::cout << "HaunIndex = " << HaunIndex << std::endl;

}

void Model::evalCstrAssim()
{
    CstrAssim = std::pow(std::max(Cstr, 0.00000001), ASScstr);

    // std::cout << "cstr = " << cstr << std::endl;
    // std::cout << "ASScstr = " << ASScstr << std::endl;
    // std::cout << "CstrAssim = " << CstrAssim << std::endl;
}

void Model::evalRespMaint()
{
    double RespMaintLeafPop;
    double RespMaintSheathPop;
    double RespMaintRootPop;
    double RespMaintInternodePop;
    double RespMaintPaniclePop;
    double CoeffQ10;

    CoeffQ10 = std::pow(CoefficientQ10, (TMoyCalc - KTempMaint) / 10);
    RespMaintLeafPop = KRespMaintLeaf * DryMatStructLeafPop * CoeffQ10;
    RespMaintSheathPop = KRespMaintSheath * DryMatStructSheathPop * CoeffQ10;
    RespMaintRootPop = KRespMaintRoot * DryMatStructRootPop * CoeffQ10;
    RespMaintInternodePop = KRespInternode * DryMatStructInternodePop *
        CoeffQ10;
    RespMaintPaniclePop = KRespPanicle * DryMatStructPaniclePop * CoeffQ10;
    RespMaintTot = RespMaintLeafPop + RespMaintSheathPop + RespMaintRootPop +
        RespMaintInternodePop + RespMaintPaniclePop;

    // std::cout << "CoefficientQ10 = " << CoefficientQ10 << std::endl;
    // std::cout << "TMoyCalc = " << TMoyCalc << std::endl;
    // std::cout << "KTempMaint = " << KTempMaint << std::endl;
    // std::cout << "CoeffQ10 = " << CoeffQ10 << std::endl;
    // std::cout << "KRespMaintLeaf = " << KRespMaintLeaf << std::endl;
    // std::cout << "KRespMaintSheath = " << KRespMaintSheath << std::endl;
    // std::cout << "KRespMaintRoot = " << KRespMaintRoot << std::endl;
    // std::cout << "KRespInternode = " << KRespInternode << std::endl;
    // std::cout << "KRespPanicle = " << KRespPanicle << std::endl;
    // std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << std:: endl;
    // std::cout << "DryMatStructSheathPop = " << DryMatStructSheathPop << std:: endl;
    // std::cout << "DryMatStructRootPop = " << DryMatStructRootPop << std:: endl;
    // std::cout << "DryMatStructInternodePop = " << DryMatStructInternodePop << std:: endl;
    // std::cout << "DryMatStructPaniclePop = " << DryMatStructPaniclePop << std:: endl;
    // std::cout << "RespMaintTot = " << RespMaintTot << std::endl;


}

void Model2::evolPlantTilNumTot()
{
    double TilNewPlant;

    if (NumPhase == 1 and ChangePhase == 1) {
        CulmsPerHill = PlantsPerHill;
    } else {
        if (NumPhase == 2 and ChangePhase == 1) {
            CulmsPerPlant = 1;
            CulmsPop = CulmsPerPlant * Density * PlantsPerHill;
        } else {
            if (NumPhase > 1 and NumPhase < 4 and
                                            HaunIndex > HaunCritTillering) {
                TilNewPlant = Cstr * std::min(std::max(0., (Ic - IcTillering) *
                                                       TilAbility) *
                                              std::sqrt(LTRkdfcl), 2.);
                CulmsPerPlant = CulmsPerPlant + TilNewPlant;
                CulmsPerHill = CulmsPerPlant * PlantsPerHill;
                CulmsPop = CulmsPerHill * Density;
            } else {
                CulmsPerPlant = CulmsPerPlant;
                CulmsPop = CulmsPop;
                CulmsPerHill = CulmsPerHill;
            }
        }
    }

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "ChangePhase = " << ChangePhase << std::endl;
    // std::cout << "Density = " << Density << std::endl;
    // std::cout << "PlantsPerHill = " << PlantsPerHill << std::endl;
    // std::cout << "HaunIndex = " << HaunIndex << std::endl;
    // std::cout << "HaunCritTillering = " << HaunCritTillering << std::endl;
    // std::cout << "Ic = " << Ic << std::endl;
    // std::cout << "IcTillering = " << IcTillering << std::endl;
    // std::cout << "TilAbility = " << TilAbility << std::endl;
    // std::cout << "LTRkdfcl = " << LTRkdfcl << std::endl;
    // std::cout << "TilNewPlant = " << TilNewPlant << std::endl;
    // std::cout << "CulmsPerPlant = " << CulmsPerPlant << std::endl;
    // std::cout << "CulmsPerHill  = " << CulmsPerHill << std::endl;
    // std::cout << "CulmsPop = " << CulmsPop << std::endl;


}

void Model::evolPlantLeafNumTot()
{
    if (NumPhase == 0) {
        PlantLeafNumNew = 0.;
        PlantLeafNumTot = 0.;
    } else {
        if (NumPhase > 1 and NumPhase < 5) {
            PlantLeafNumNew = HaunGain * CulmsPerHill;
            PlantLeafNumTot = PlantLeafNumTot + PlantLeafNumNew;
        } else {
            PlantLeafNumNew = PlantLeafNumNew;
            PlantLeafNumTot = PlantLeafNumTot;
        }
    }

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "HaunGain = " << HaunGain << std::endl;
    // std::cout << "CulmsPerHill = " << CulmsPerHill << std::endl;
    // std::cout << "PlantLeafNumNew = " << PlantLeafNumNew << std::endl;
    // std::cout << "PlantLeafNumTot = " << PlantLeafNumTot << std::endl;

}

void Model::evalRelPotLeafLength()
{
    if (NumPhase > 1) {
        RelPotLeafLength = std::min((0.1 + 0.9 * HaunIndex / RankLongestLeaf),
                                    1.);
    }

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "HaunIndex = " << HaunIndex << std::endl;
    // std::cout << "RankLongestLeaf = " << RankLongestLeaf << std::endl;
    // std::cout << "RelPotLeafLength = " << RelPotLeafLength << std::endl;

}

void Model::evalDemandStructSheath()
{
    if (NumPhase > 1 and NumPhase < 5) {
        DemStructSheathPop = (1 + ((SlaMax - Sla) / (SlaMax - SlaMin))) * 0.5 *
            DemStructLeafPop / WtRatioLeafSheath *
            std::max(0.00001, StressCold);

        // std::cout << "SlaMax = " << SlaMax << std::endl;
        // std::cout << "Sla = " << Sla << std::endl;
        // std::cout << "SlaMin = " << SlaMin << std::endl;
        // std::cout << "DemStructLeafPop = " << DemStructLeafPop << std::endl;
        // std::cout << "WtRatioLeafSheath = " << WtRatioLeafSheath << std::endl;
        // std::cout << "StressCold = " << StressCold << std::endl;
        // std::cout << "DemStructSheathPop = " << DemStructSheathPop << std::endl;

    }
}

void Model2::evalDemandStructRoot()
{
    RootSystSoilSurfPop = std::min(RootFront * RootFront * Density / 1000000,
                                   10000.);
    RootSystVolPop = RootSystSoilSurfPop * RootFront / 1000;
    if (NumPhase > 1 and NumPhase < 5) {
        GainRootSystSoilSurfPop = RootSystSoilSurfPop - RootSystSoilSurfPopOld;
        GainRootSystVolPop = RootSystVolPop - RootSystVolPopOld;
        DemStructRootPop =
            std::min((DemStructLeafPop + DemStructSheathPop) *
                     RootPartitMax, std::max(0., CoeffRootMassPerVolMax *
                                             RootSystVolPop -
                                             DryMatStructRootPop));
        DemStructRootPlant = DemStructRootPop * 1000 / Density;
        RootSystSoilSurfPopOld = RootSystSoilSurfPop;
        RootFrontOld = RootFront;
        RootSystVolPopOld = RootSystVolPop;
    }
}

void Model2::evalDemandStructPanicle()
{
    if (NumPhase == 4) {
        DemStructPaniclePlant = CoeffPanicleMass * CulmsPerHill *
            std::sqrt(std::min(Ic, 1.)) *
            std::sqrt(std::max(0.00001, StressCold));
        PanStructMass = 1000 *
            DryMatStructPaniclePop / (Density * CulmsPerHill);
        if (PanStructMass > PanStructMassMax) {
            DemStructPaniclePlant = 0;
        }
        DemStructPaniclePop = DemStructPaniclePlant * Density / 1000;
    }
}

void Model::evolGrowthStructSheathPop()
{
    if (NumPhase > 1 and NumPhase < 5) {
        if (Ic < 1) {
            GrowthStructSheathPop = SupplyTot * (DemStructSheathPop /
                                                 DemStructTotPop);
        } else  {
            GrowthStructSheathPop = DemStructSheathPop;
        }
    }
}

void Model::evolGrowthStructRootPop()
{
    if (NumPhase > 1 and NumPhase < 5) {
        if (Ic < 1) {
            GrowthStructRootPop = SupplyTot * (DemStructRootPop /
                                               DemStructTotPop);
        } else {
            GrowthStructRootPop = DemStructRootPop;
        }
    }
}

void Model::evolGrowthStructPanPop()
{
    if (NumPhase > 1 and NumPhase < 5) {
        if (Ic < 1) {
            GrowthStructPaniclePop = SupplyTot * (DemStructPaniclePop /
                                                  DemStructTotPop);
        } else {
            GrowthStructPaniclePop = DemStructPaniclePop;
        }
    }
}

void Model::evalSlaMitch()
{
    if (NumPhase > 1) {
        SlaMitch = SlaMin + (SlaMax - SlaMin) *
            std::pow(AttenMitch, (SommeDegresJour - SDJLevee));
        SlaNew = SlaMin + (SlaMitch - SlaMin) *
            std::pow(DegresDuJour / (TOpt1 - TBase), TempSla);
        Sla = ((Sla * DryMatStructLeafPop) + (SlaNew * GrowthStructLeafPop)) /
            (DryMatStructLeafPop + GrowthStructLeafPop);
    } else {
        SlaMitch = 0;
        SlaNew = 0;
        Sla = SlaMax;
    }
}

void Model::evolKcpKceBilhy()
{
    Kcp = std::min((1 - LTRkdfcl) * KcMax, KcMax);
    Kcp = std::min(Kcp, KcMax);
    Kce = LTRkdfcl * 1 * (Mulch / 100);
    KcTot = Kcp + Kce;

    // std::cout << "LTRkdfcl = " << LTRkdfcl << std::endl;
    // std::cout << "KcMax = " << KcMax << std::endl;
    // std::cout << "Mulch = " << Mulch << std::endl;
    // std::cout << "Kcp = " << Kcp << std::endl;
    // std::cout << "Kce = " << Kce << std::endl;
    // std::cout << "KcTot = " << KcTot << std::endl;

}

void Model2::evolConsRes_Flood()
{
    double TrSurf;
    double WaterDeficit;

    TrSurf = 0;
    StockSurface = ValRFE + ValRDE;
    if (FloodwaterDepth + StockMacropores == 0 or NumPhase == 0) {

        // std::cout << "On passe par la " << std::endl;
        // std::cout << "RuRac = " << RuRac << std::endl;
        // std::cout << "StockRac = " << StockRac << std::endl;



        if (RuRac != 0) {
            if (RuRac <= RuSurf) {
                TrSurf = Tr;
            } else {
                if (StockRac != 0) {
                    TrSurf = Tr * StockSurface / StockRac;
                }
            }
        } else {
            TrSurf = 0;
        }

        // std::cout << "TrSurf = " << TrSurf << std::endl;


        ValRDE = std::max(0., ValRSurf - CapaREvap);
        if (ValRDE + ValRFE < TrSurf) {
            ValRFE = 0;
            ValRSurf = ValRSurf - ValRDE;
            ValRDE = 0;
        } else {
            if (ValRFE > TrSurf) {
                ValRFE = ValRFE - TrSurf;
            } else {
                ValRSurf = ValRSurf - (TrSurf - ValRFE);
                ValRDE = ValRDE - (TrSurf - ValRFE);
                ValRFE = 0;
            }
        }
        StockSurface = ValRFE + ValRDE;
        StockRac = std::max(0., StockRac - Tr);
        StockTotal = std::max(0., StockTotal - Tr);

        // std::cout << "evolConsRes_Flood()" << std::endl;
        // std::cout << "StockTotal = " << StockTotal << std::endl;

        StockRac = std::min(StockRac, StockTotal);
    }
    if (StockMacropores + FloodwaterDepth > 0 and
        (StockMacropores + FloodwaterDepth <= Tr + Evap) and NumPhase > 0) {
        WaterDeficit = (Tr + Evap) - (StockMacropores + FloodwaterDepth);
        StockMacropores = 0;
        FloodwaterDepth = 0;
        StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 -
            WaterDeficit;
        StockRac = RuRac - WaterDeficit;
        StockRac = std::min(StockRac, StockTotal);
        StockSurface = std::max(EpaisseurSurf * ResUtil / 1000 - WaterDeficit,
                                0.);
        ValRFE = std::max(StockSurface - ValRDE - WaterDeficit, 0.);
        ValRDE = ValRDE;
        ValRSurf = ValRFE + ValRDE;
    } else {
        if (StockMacropores + FloodwaterDepth > Tr + Evap and NumPhase > 0) {
            FloodwaterDepth = FloodwaterDepth - (Tr + Evap);
            StockMacropores = StockMacropores + std::min(0., FloodwaterDepth);
            FloodwaterDepth = std::max(FloodwaterDepth, 0.);
            StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
                StockMacropores;
            StockRac = RuRac + StockMacropores;
            StockRac = std::min(StockRac, StockTotal);
            StockSurface =
                std::max(EpaisseurSurf * ResUtil / 1000 +
                         StockMacropores * (EpaisseurSurf /
                                            (EpaisseurSurf + EpaisseurProf)),
                         0.);
            ValRFE = std::max(StockSurface - ValRDE, 0.);
            ValRDE = ValRDE;
        }
    }
}

void Model::evalConversion()
{
    double KAssim;

    switch ((int)(NumPhase))
    {
    case 2: KAssim = 1; break;
    case 3:
    case 4: KAssim = TxAssimBVP; break;
    case 5: KAssim = TxAssimBVP + (SommeDegresJourCor -
                                   SommeDegresJourPhasePrecedente) *
            (TxAssimMatu1 - TxAssimBVP) / (SeuilTempPhaseSuivante -
                                           SommeDegresJourPhasePrecedente);
        break;
    case 6: KAssim = TxAssimMatu1 + (SommeDegresJourCor -
                                     SommeDegresJourPhasePrecedente) *
            (TxAssimMatu2 - TxAssimMatu1) / (SeuilTempPhaseSuivante -
                                             SommeDegresJourPhasePrecedente);
        break;
    default:
        KAssim = 0;
    }
    Conversion = KAssim * TxConversion;

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "TxAssimBVP = " << TxAssimBVP << std::endl;
    // std::cout << "SommeDegresJour = " << SommeDegresJour << std::endl;
    // std::cout << "SommeDegresJourPhasePrecedente = " << SommeDegresJourPhasePrecedente << std::endl;
    // std::cout << "TxAssimMatu1 = " << TxAssimMatu1 << std::endl;
    // std::cout << "TxAssimMatu2 = " << TxAssimMatu2 << std::endl;
    // std::cout << "KAssim = " << KAssim << std::endl;
    // std::cout << "TxConversion = " << TxConversion << std::endl;
    // std::cout << "Conversion = " << Conversion << std::endl;

}

void Model::evolPSPMVMD(double t)
{
    if (t >= DateSemis) {

        /*std::cout << "-----" << std::endl;
        std::cout << "AVANT" << std::endl;
        std::cout << "-----" << std::endl;
        std::cout << "NumPhase = " << NumPhase << std::endl;
        std::cout << "ChangePhase = " << ChangePhase << std::endl;
        std::cout << "DegresDuJourCor = " << DegresDuJourCor << std::endl;
        std::cout << "DegresDuJour = " << DegresDuJour << std::endl;
        std::cout << "DureeDuJour = " << DayLength << std::endl;
        std::cout << "SommeDegresJourCor = " << SommeDegresJourCor << std::endl;
        std::cout << "PPExp = " << PPExp << std::endl;
        std::cout << "SeuilPP = " << SeuilPP << std::endl;
        std::cout << "PPCrit = " << PPCrit << std::endl;
        std::cout << "SommeDegresJourPhasePrecedente = " << SommeDegresJourPhasePrecedente << std::endl;*/




        double SDJPSP;

        if (NumPhase == 3) {
            if (ChangePhase == 1) {
                SumPP = 100;
                SDJPSP = std::max(0.01, DegresDuJourCor);
                /*std::cout << "Ligne 1" << std::endl;
                std::cout << "SumPP = " << SumPP << std::endl;
                std::cout << "SDJPSP = " << SDJPSP << std::endl;*/
            } else {
                SDJPSP = SommeDegresJourCor - SommeDegresJourPhasePrecedente +
                    std::max(0.01, DegresDuJourCor);
                /*std::cout << "Ligne 2" << std::endl;
                  std::cout << "SDJPSP = " << SDJPSP << std::endl;*/
            }

            /*std::cout << "std::pow((1000 / SDJPSP), PPExp) = " << std::pow((1000 / SDJPSP), PPExp) << std::endl;
            std::cout << "DureeDuJour - PPCrit =" << DayLength - PPCrit << std::endl;
            std::cout <<  "std::max(0., (DureeDuJour - PPCrit)) = " << std::max(0., (DureeDuJour - PPCrit)) << std::endl;*/



            SumPP = std::pow((1000 / SDJPSP), PPExp) *
                std::max(0., (DayLength - (double) PPCrit)) / (SeuilPP - PPCrit);
            SeuilTempPhaseSuivante = SommeDegresJourPhasePrecedente + SDJPSP;
        }

        /*std::cout << "-----" << std::endl;
        std::cout << "APRES" << std::endl;
        std::cout << "-----" << std::endl;
        std::cout << "SDJPSP = " << SDJPSP << std::endl;
        std::cout << "SumPP = " << SumPP << std::endl;
        std::cout << "SeuilTempPhaseSuivante = " << SeuilTempPhaseSuivante << std::endl;*/



    }
}


void Model::evolSomDegresJourCor()
{
    if (NumPhase >= 1) {
        SommeDegresJourCor = SommeDegresJourCor + DegresDuJourCor;
    } else {
        SommeDegresJourCor = 0;
    }

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "DegresDuJourCor = " << DegresDuJourCor << std::endl;
    // std::cout << "SommeDegresJourCor = " << SommeDegresJourCor << std::endl;

}


void Model::evalMaximumLai()
{
    if (Lai > TempLai) {
        TempLai = Lai;
    }
    if (NumPhase != 7) {
        MaximumLai = 0;
    } else if (NumPhase == 7 and ChangePhase == 1) {
        MaximumLai = TempLai;
    }
}

void Model::evalVitesseRacinaire()
{

    // std::cout << "NumPhase = " << NumPhase << std::endl;


    switch ((int)(NumPhase))
    {
    case 1:
        VitesseRacinaire = VRacLevee;
        break;
    case 2:
        VitesseRacinaire = VRacBVP;
        break;
    case 3:
        VitesseRacinaire = VRacPSP;
        break;
    case 4:
        VitesseRacinaire = VRacRPR;
        break;
    case 5:
        VitesseRacinaire = VRacMatu1;
        break;
    case 6:
        VitesseRacinaire = VRacMatu2;
        break;
    default:
        VitesseRacinaire = 0;
    }
    VitesseRacinaireDay = VitesseRacinaire * DegresDuJourCor *
        std::pow(Cstr, RootCstr);

    // std::cout << "VitesseRacinaire = " << VitesseRacinaire << std::endl;
    // std::cout << "DegresDuJourCor = " << DegresDuJourCor << std::endl;
    // std::cout << "RootCstr = " << RootCstr << std::endl;
    // std::cout << "VitesseRacinaireDay = " << VitesseRacinaireDay << std::endl;


}
void Model::evalRootFront()
{
    if (NumPhase > 0) {

        // std::cout << "ResUtil = " << ResUtil << std::endl;
        // std::cout << "RuRac = " << RuRac << std::endl;

        RootFront = ((1 / ResUtil) * RuRac) * 1000;

        // std::cout << "RootFront = " << RootFront << std::endl;

    }
}

void Model::evalSDJPhase4()
{

    // std::cout << "NumPhase = " << NumPhase << std::endl;

    if (NumPhase == 4) {
        SDJCorPhase4 = SDJCorPhase4 + DegresDuJourCor;
    }

    // std::cout << "SDJCorPhase4 = " << SDJCorPhase4 << std::endl;

}

void Model::evalDateGermination()
{
    if (NumPhase < 1 or (NumPhase == 1 and ChangePhase == 1)) {
        NbDaysSinceGermination = 0;
    } else {
        NbDaysSinceGermination = NbDaysSinceGermination + 1;
    }

    // std::cout << "NbDaysSinceGermination = " << NbDaysSinceGermination << std::endl;
}

void Model::evalSterility()
{

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "ChangePhase = " << ChangePhase << std::endl;


    if (NumPhase == 5 and ChangePhase == 1) {
        SterilityCold =
            std::max(0.,
                     (std::min(1.,
                               KCritSterCold1 / (KCritSterCold1 -
                                                 KCritSterCold2) - TMinMoy /
                               (KCritSterCold1 - KCritSterCold2))));

        // std::cout << "SterilityCold = " << SterilityCold << std::endl;


        SterilityHeat =
            1. - std::max(0.,
                          (std::min(1., KCritSterHeat2 /
                                    (KCritSterHeat2 -
                                     KCritSterHeat1) - TMaxMoy /
                                    (KCritSterHeat2 - KCritSterHeat1))));

        // std::cout << "SterilityHeat = " << SterilityHeat << std::endl;


        SterilityDrought =
            std::max(0.,
                     (std::min(1., KCritSterFtsw1 / (KCritSterFtsw1 -
                                                     KCritSterFtsw2) -
                               FtswMoy / (KCritSterFtsw1 - KCritSterFtsw2))));

        // std::cout << "SterilityDrought = " << SterilityDrought << std::endl;
        // std::cout << "FtswMoy = " << FtswMoy << std::endl;



    }
    SterilityTot = std::min(0.999, 1 - ((1 - SterilityCold) *
                                        (1 - SterilityHeat) *
                                        (1 - SterilityDrought)));

    // std::cout << "SterilityTot = " << SterilityTot << std::endl;


}

void Model2::initParcelle()
{
    double Stockini2;
    double Stockini1;

    // std::cout << "HumSat = " << HumSat << std::endl;
    // std::cout << "HumFC = " << HumFC << std::endl;
    // std::cout << "EpaisseurSurf = " << EpaisseurSurf << std::endl;
    // std::cout << "EpaisseurProf = " << EpaisseurProf << std::endl;
    // std::cout << "PEvap = " << PEvap << std::endl;
    // std::cout << "StockIniSurf = " << StockIniSurf << std::endl;
    // std::cout << "StockIniProf = " << StockIniProf << std::endl;

    VolRelMacropores = 100 * (HumSat - HumFC);
    ResUtil = (HumFC - HumPF) * 1000;
    ProfRu = EpaisseurSurf + EpaisseurProf;
    RuSurf = ResUtil * EpaisseurSurf / 1000;
    CapaREvap = 0.5 * EpaisseurSurf * HumPF;
    CapaRFE = PEvap * (CapaREvap + RuSurf);
    CapaRDE = RuSurf - CapaRFE;
    StRuMax = ResUtil * ProfRu / 1000;
    Stockini1 = std::min(StockIniSurf, CapaREvap + RuSurf);
    Stockini2 = std::min(StockIniProf, ResUtil * EpaisseurProf / 1000);
    ValRSurf = std::min(Stockini1, CapaREvap + CapaRDE);
    ValRDE = std::max(0., ValRSurf - CapaREvap);
    ValRFE = std::max(0., Stockini1 - (CapaREvap + CapaRDE));
    StockSurface = ValRDE + ValRFE;
    StockTotal = StockSurface + Stockini2; //transpirable
    Hum = StockTotal;
    LTRkdfcl = 1;
    LIRkdf = 0;
    LTRkdf = 0;
    CounterNursery = 0;
    VolMacropores = VolRelMacropores * (EpaisseurSurf + EpaisseurProf) / 100;
}

void Model2_2::initParcelle()
{
    double Stockini2;
    double Stockini1;

    // std::cout << "HumSat = " << HumSat << std::endl;
    // std::cout << "HumFC = " << HumFC << std::endl;
    // std::cout << "EpaisseurSurf = " << EpaisseurSurf << std::endl;
    // std::cout << "EpaisseurProf = " << EpaisseurProf << std::endl;
    // std::cout << "PEvap = " << PEvap << std::endl;
    // std::cout << "StockIniSurf = " << StockIniSurf << std::endl;
    // std::cout << "StockIniProf = " << StockIniProf << std::endl;

    VolRelMacropores = 100 * (HumSat - HumFC);
    ResUtil = (HumFC - HumPF) * 1000;
    ProfRu = EpaisseurSurf + EpaisseurProf;
    RuSurf = ResUtil * EpaisseurSurf / 1000;
    CapaREvap = 0.5 * EpaisseurSurf * HumPF;
    CapaRFE = PEvap * (CapaREvap + RuSurf);
    CapaRDE = RuSurf - CapaRFE;
    StRuMax = ResUtil * ProfRu / 1000;
    Stockini1 = std::min(StockIniSurf, CapaREvap + RuSurf);
    Stockini2 = std::min(StockIniProf, ResUtil * EpaisseurProf / 1000);
    ValRSurf = std::min(Stockini1, CapaREvap + CapaRDE);
    ValRDE = std::max(0., ValRSurf - CapaREvap);
    ValRFE = std::max(0., Stockini1 - (CapaREvap + CapaRDE));
    StockSurface = ValRDE + ValRFE;
    StockTotal = StockSurface + Stockini2; //transpirable
    Hum = StockTotal;
    LTRkdfcl = 1;
    LIRkdf = 0;
    LTRkdf = 0;
    CounterNursery = 0;
    VolMacropores = VolRelMacropores * (EpaisseurSurf + EpaisseurProf) / 100;
}

void Model::evalEvapPot()
{
    EvapPot = Kce * Eto;

    // std::cout << "Kce = " << Kce << std::endl;
    // std::cout << "Eto = " << Eto << std::endl;
    // std::cout << "EvapPot = " << EvapPot << std::endl;
}

void Model2::transplanting()
{
    double DensityChange;

    if (Transplanting == 0) {
        // std::cout << "Transplanting == 0" << std::endl;
    } else {
        // std::cout << "Transplanting == 1" << std::endl;
    }


    DensityChange = DensityField / (DensityNursery / PlantsPerHill);
    if (Transplanting == 1 and NumPhase >= 1) {
        CounterNursery = CounterNursery + 1;
    }
    if (Transplanting == 1 and CounterNursery < DurationNursery and
        ChangeNurseryStatus == 0) {
        NurseryStatus = 0;
        ChangeNurseryStatus = 0;
    } else {
        if (Transplanting == 1 and CounterNursery >= DurationNursery and
            ChangeNurseryStatus == 0 and NurseryStatus == 0) {
            NurseryStatus = 1;
            ChangeNurseryStatus = 1;
        } else {
            NurseryStatus = 1;
            ChangeNurseryStatus = 0;
        }
    }
    if (NurseryStatus == 1) {
        Density = DensityField;
    } else {
        Density = DensityNursery / PlantsPerHill;
    }
    if (ChangeNurseryStatus == 1) {
        DryMatStructLeafPop = DryMatStructLeafPop * DensityChange;
        DryMatStructSheathPop = DryMatStructSheathPop * DensityChange;
        DryMatStructRootPop = DryMatStructRootPop * DensityChange;
        DryMatStructInternodePop = DryMatStructInternodePop * DensityChange;
        DryMatStructPaniclePop = DryMatStructPaniclePop * DensityChange;
        DryMatResInternodePop = DryMatResInternodePop * DensityChange;
    }
}

void Model2::transplantingShock()
{
    if (CounterNursery > 0 and CounterNursery < 8) {
        Assim = Assim * CoeffTransplantingShock;
    } else {
        Assim = Assim;
    }

    // std::cout << "Assim = " << Assim << std::endl;

}

void Model2::evalRuiss_FloodDyna()
{
    double CorrectedIrrigation;
    double CorrectedBundHeight;

    Lr = 0;
    CorrectedBundHeight = BundHeight;
    // implement lifesaving drainage
    if (LifeSavingDrainage == 1 and FloodwaterDepth > 0.5 * PlantHeight and
        PlantHeight > 0 and NumPhase > 1 and BundHeight > 0) {
        CorrectedBundHeight = 0.5 * PlantHeight;
        Lr = Lr + std::max(0., FloodwaterDepth - (0.5 * PlantHeight));
        FloodwaterDepth = std::min(FloodwaterDepth, (0.5 * PlantHeight));
        if (FloodwaterDepth + StockMacropores > 0) {
            EauDispo = FloodwaterDepth + StockMacropores;
        }
    }
    // implement terminal drainage
    if (NumPhase > 4 and NumPhase < 7 and DAF > PlotDrainageDAF and
        BundHeight > 0) {
        CorrectedBundHeight = 0;
        Lr = Lr + FloodwaterDepth;
        FloodwaterDepth = 0;
        if (FloodwaterDepth + StockMacropores > 0) {
            EauDispo = StockMacropores;
        } else {
            EauDispo = Rain;
        }
    }
    // define corrected irrigation
    if (Irrigation == NullValue) {
        CorrectedIrrigation = 0;
    } else {
        CorrectedIrrigation = Irrigation;
    }
    // implement runoff and EauDispo under terminal drainage
    if (CorrectedBundHeight == 0 and BundHeight != CorrectedBundHeight) {
      if ((StockMacropores + FloodwaterDepth) == 0) {
        EauDispo = Rain + CorrectedIrrigation;
      } else {
          StockMacropores = StockMacropores + Rain + CorrectedIrrigation;
          Lr = Lr + std::max(0., StockMacropores - VolMacropores);
          StockMacropores = StockMacropores - std::max(0., StockMacropores -
                                                       VolMacropores);
          EauDispo = StockMacropores;
      }
    }
    // implement classical upland runoff (SARRAH)
    if (BundHeight == 0) {
        if (Rain > SeuilRuiss) {
            Lr = Lr + (Rain + CorrectedIrrigation - SeuilRuiss) *
                PourcRuiss / 100;
            EauDispo = Rain + CorrectedIrrigation - Lr;
        } else {
            EauDispo = Rain + CorrectedIrrigation;
        }
    }
    // implement bunded-plot style water ponding and runoff, regular
    // situation w/o drainage
    if (CorrectedBundHeight > 0) {
        if (StockMacropores + FloodwaterDepth == 0) {
            Lr = Lr + std::max((Rain + CorrectedIrrigation - BundHeight -
                                VolMacropores), 0.);
            EauDispo = std::min(Rain + CorrectedIrrigation,
                                BundHeight + VolMacropores);
        } else {
            StockMacropores = StockMacropores + Rain + CorrectedIrrigation;
            FloodwaterDepth = FloodwaterDepth + std::max(0., StockMacropores -
                                                         VolMacropores);
            StockMacropores = std::min(VolMacropores, StockMacropores);
            Lr = Lr + std::max(0., FloodwaterDepth - CorrectedBundHeight);
            FloodwaterDepth = std::min(FloodwaterDepth, CorrectedBundHeight);
            EauDispo = StockMacropores + FloodwaterDepth;
        }
    }
}

void Model2::evolRempliResRFE_RDE()
{
    double EauReste;
    double ValRSurfPrec;
    double EauTranspi;

    Dr = 0;
    EauTranspi = 0;
    if (StockMacropores + FloodwaterDepth == 0) {
        EauReste = 0;
        ValRFE = ValRFE + EauDispo;

        // std::cout << "ValRFE = " << ValRFE << std::endl;
        // std::cout << "EauDispo = " << EauDispo << std::endl;
        // std::cout << "CapaRFE = " << CapaRFE << std::endl;

        if (ValRFE > CapaRFE) {
            EauReste = ValRFE - CapaRFE;
            ValRFE = CapaRFE;
        }
        ValRSurfPrec = ValRSurf;
        ValRSurf = ValRSurf + EauReste;

        // std::cout << "ValRSurf = " << ValRSurf << std::endl;
        // std::cout << "ValRSurfPrec = " << ValRSurfPrec << std::endl;
        // std::cout << "CapaREvap = " << CapaREvap << std::endl;

        if (ValRSurfPrec < CapaREvap) {
            EauTranspi = EauDispo - (std::min(CapaREvap, ValRSurf) -
                                     ValRSurfPrec);
        } else {
            EauTranspi = EauDispo;
        }

        // std::cout << "EauTranspi = " << EauTranspi << std::endl;

        if (ValRSurf > CapaREvap + CapaRDE) {
            ValRSurf = CapaREvap + CapaRDE;
            ValRDE = CapaRDE;
        } else {
            if (ValRSurf <= CapaREvap) {
                ValRDE = 0;
            } else {
                ValRDE = ValRSurf - CapaREvap;
            }
        }
        StockSurface = ValRFE + ValRDE;

        // std::cout << "StockSurface = " << StockSurface << std::endl;

        StockTotal = StockTotal + EauTranspi;

        // std::cout << "evolRempliResRFE_RDE()" << std::endl;
        // std::cout << "StockTotal = " << StockTotal << std::endl;


        // std::cout << "StockTotal = " << StockTotal << std::endl;
        // std::cout << "StRuMax = " << StRuMax << std::endl;

        if (StockTotal > StRuMax) {
            Dr = StockTotal - StRuMax;
            StockTotal = StRuMax;

            // std::cout << "evolRempliResRFE_RDE()" << std::endl;
            // std::cout << "StockTotal = " << StockTotal << std::endl;


        } else {
            Dr = 0;
        }

        // std::cout << "Dr = " << Dr << std::endl;

        if (Hum < CapaRFE + CapaRDE) {
            Hum = StockSurface;
        } else {
            Hum = std::max(Hum, StockTotal);
        }

        // std::cout << "Hum = " << Hum << std::endl;

    }
    StockRac = std::min(StockRac + EauTranspi, RuRac);

    // std::cout << "StockRac = " << StockRac << std::endl;

    // Shifting non-percolating Dr back to macropores & floodwater if plot is bunded
    if (BundHeight > 0) {
        // Shifting non-percolating Dr to Floodwater
        StockMacropores = StockMacropores + std::max(0., Dr - PercolationMax);
        Dr = std::min(Dr, PercolationMax);
        if (StockMacropores > VolMacropores) {
            FloodwaterDepth = FloodwaterDepth +
                (StockMacropores - VolMacropores);
            StockMacropores = VolMacropores;
        }
        // Implementing Dr
        if (FloodwaterDepth >= PercolationMax) {
            Dr = PercolationMax;
            FloodwaterDepth = FloodwaterDepth - Dr;
            StockMacropores = VolMacropores;
        } else {
            if (FloodwaterDepth < PercolationMax and
                FloodwaterDepth + StockMacropores >= PercolationMax) {
                Dr = PercolationMax;
                FloodwaterDepth = FloodwaterDepth - Dr;
                StockMacropores = StockMacropores + FloodwaterDepth;
                FloodwaterDepth = 0;
            } else {
                Dr = std::min(PercolationMax,
                              FloodwaterDepth + StockMacropores + Dr);
                FloodwaterDepth = 0;
                StockMacropores = 0;
            }
        }
    }
}

void Model2::plantSubmergence()
{
    FractionPlantHeightSubmer = std::min(
        std::max(0., FloodwaterDepth / std::max(PlantHeight, 0.1)), 1.);
}

void Model2::excessAssimilToRoot()
{
    if (NumPhase > 1) {
        RootMassPerVol = DryMatStructRootPop / RootSystVolPop;
    }
    if (ExcessAssimToRoot == 1) {
        if (NumPhase < 5 and NumPhase > 1 and AssimNotUsed > 0) {
            if (RootMassPerVol < CoeffRootMassPerVolMax) {
                GrowthStructRootPop = GrowthStructRootPop + AssimNotUsed;
                AssimNotUsed = 0;
            }
        }
    }
}

void Model2::evolRempliMacropores()
{
    if (StockMacropores + FloodwaterDepth > 0) {
        StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
            StockMacropores;
        Hum = StockTotal;
        StockSurface = EpaisseurSurf * ResUtil / 1000 +
            (EpaisseurSurf / (EpaisseurSurf + EpaisseurProf)) * StockMacropores;
        StockRac = RootFront * ResUtil / 1000 +
            (RootFront / (EpaisseurSurf + EpaisseurProf)) * StockMacropores;
        ValRDE = CapaRDE;
        ValRFE = CapaRFE;
        ValRSurf = EpaisseurSurf * ResUtil / 1000;
    }
}

void Model2::evalFTSW()
{
    // std::cout << "StockRac (avant) = " << StockRac << std::endl;

    StockRac = std::min(StockRac, (RuRac +
                                   (StockMacropores * RuRac / StRuMax)));
    StockRac = std::min(StockRac, StockTotal);
    if (RuRac > 0) {
        FTSW = StockRac / RuRac;
    } else {
        FTSW = 0;
    }

    // std::cout << "StockRac = " << StockRac << std::endl;
    // std::cout << "RuRac = " << RuRac << std::endl;
    // std::cout << "StockMacropores = " << StockMacropores << std::endl;
    // std::cout << "StRuMax = " << StRuMax << std::endl;
    // std::cout << "FTSW = " << FTSW << std::endl;
}

void Model2::evalDAF()
{
    if (NumPhase > 4) {
        DAF = DAF + 1;
    } else {
        DAF = DAF;
    }
}

void Model::evalSimStartGermin()
{
    if (NumPhase == 1 and ChangePhase == 1) {
        SimStartGermin = NbJas;
    }
}

void Model::evalSimEmergence()
{
    if (NumPhase == 2 and ChangePhase == 1) {
        SimEmergence = NbJas;
    }
}

void Model::evalSimStartPSP()
{
    if (NumPhase == 3 and ChangePhase == 1) {
        SimStartPSP = NbJas;
    }
}

void Model::evalSimPanIni()
{
    if (NumPhase == 4 and ChangePhase == 1) {
        SimPanIni = NbJas;
    }
}

void Model::evalSimAnthesis50()
{
    if (NumPhase == 5 and ChangePhase == 1) {
        SimAnthesis50 = NbJas;
    }
}

void Model::evalSimStartMatu2()
{
    if (NumPhase == 6 and ChangePhase == 1) {
        SimStartMatu2 = NbJas;
    }
}

void Model::evalColdStress()
{
    StressCold = 1 -
        std::max(0.,
                 std::min(1.,
                          KCritStressCold1 / (KCritStressCold1 -
                                              KCritStressCold2) - TMin /
                          (KCritStressCold1 - KCritStressCold2)));
    StressCold = std::max(0.00001, StressCold);
    //std::cout << "StressCold = " << StressCold << std::endl;
}

void Model::evalAssim()
{
    Assim = std::max(AssimPot * CstrAssim, 0.);

    // std::cout << "AssimPot = " << AssimPot << std::endl;
    // std::cout << "CstrAssim = " << CstrAssim << std::endl;
    // std::cout << "Assim = " << Assim << std::endl;

}

void Model2::evolWaterLoggingUpland()
{
    if (Dr > PercolationMax and BundHeight == 0) {
        StockMacropores = StockMacropores + (Dr - PercolationMax);
        Lr = Lr + std::max(0., StockMacropores - VolMacropores);
        Dr = PercolationMax;
        StockMacropores = std::min(StockMacropores, VolMacropores);
    }
}

void Model2::evalStressWaterLogging()
{
    if (StockMacropores > 0 and RootFront > 0) {
        FractionRootsLogged =
            (std::max(0., RootFront - ((VolMacropores - StockMacropores) /
                                       VolMacropores) * (EpaisseurSurf +
                                                         EpaisseurProf))) /
            RootFront;
        CoeffStressLogging = 1 - (FractionRootsLogged *
                                  std::min(1., WaterLoggingSens));
    }
}

void Model2::evalCstrPFactorFAO()
{
    double pFact;

    pFact = PFactor + 0.04 * (5 - KcTot * Eto);
    pFact = std::max(0., pFact);
    pFact = std::min(0.8, pFact);
    Cstr = std::min((FTSW / (1 - pFact)), 1.);
    Cstr = std::max(0., Cstr);
    if (StockMacropores > 0) {
        Cstr = Cstr * CoeffStressLogging;
    }

    // std::cout << "pFact = " << pFact << std::endl;
    // std::cout << "FTSW = " << FTSW << std::endl;
    // std::cout << "Cstr = " << Cstr << std::endl;
}

void Model2_1::evolHauteur_SDJ_cstr()
{
    double CorrectedCstrMean;

    // std::cout << "PhaseStemElongation = " << PhaseStemElongation << std::endl;

    if (PhaseStemElongation == 1) {

        // std::cout << "HaunGain = " << HaunGain << std::endl;
        // std::cout << "Ic = " << Ic << std::endl;
        // std::cout << "Cstr = " << Cstr << std::endl;
        // std::cout << "StressCold = " << StressCold << std::endl;
        // std::cout << "InternodeLengthMax = " << InternodeLengthMax << std::endl;
        // std::cout << "CoeffInternodeNum = " << CoeffInternodeNum << std::endl;

        ApexHeightGain = HaunGain * std::min(std::pow(std::min(Ic, 1.), 0.5),
                                             Cstr) * StressCold * InternodeLengthMax;
        ApexHeightGain = ApexHeightGain * CoeffInternodeNum;

        /*std::cout << "ApexHeightGain = " << ApexHeightGain << std::endl;*/

    } else {
        ApexHeightGain = 0;
    }

    if (NumPhase == 7 and ChangePhase == 1) {
        ApexHeight = 0;
    }

    ApexHeight = ApexHeight + ApexHeightGain;

    // std::cout << "ApexHeight = " << ApexHeight << std::endl;

    if (CstrMean <= 0) {
        CorrectedCstrMean = 1;
    } else {
        CorrectedCstrMean = CstrMean;
    }

    if (NumPhase == 7 and ChangePhase == 1) {

        PlantHeight = 0;
        PlantWidth = 0;
    } else {
        // std::cout << "RelPotLeafLength = " << RelPotLeafLength << std::endl;
        // std::cout << "LeafLengthMax = " << LeafLengthMax << std::endl;
        // std::cout << "IcMean = " << IcMean << std::endl;
        // std::cout << "CorrectedCstrMean = " << CorrectedCstrMean << std::endl;
        // std::cout << "WtRatioLeafSheath = " << WtRatioLeafSheath << std::endl;
        // std::cout << "ApexHeight = " << ApexHeight << std::endl;


        PlantHeight = ApexHeight + (1.5 * (1 - Kdf) * RelPotLeafLength *
                                    LeafLengthMax * std::sqrt(IcMean) *
                                    CorrectedCstrMean * (1 + 1 / WtRatioLeafSheath));

        // std::cout << " PlantHeight = " << PlantHeight << std::endl;
        // std::cout << "Kdf = " << Kdf << std::endl;
        // std::cout << "IcMean = " << IcMean << std::endl;
        // std::cout << "RelPotLeafLength = " << RelPotLeafLength << std::endl;
        // std::cout << "LeafLengthMax = " << LeafLengthMax << std::endl;

        PlantWidth = std::pow(Kdf, 1.5) * 2 * std::sqrt(IcMean) * RelPotLeafLength *
            LeafLengthMax;

        // std::cout << "PlantHeight = " << PlantHeight << std::endl;
        // std::cout << "PlantWidth = " << PlantWidth << std::endl;
    }

}

void Model2_1::evalParIntercepte()
{
    if (Lai > 0 and LIRkdfcl == 0) {
        LIRkdfcl = (1 - std::exp(-Kdf * Lai));
    }
    PARIntercepte = PAR * LIRkdfcl;

    // std::cout << "Lai = " << Lai << std::endl;
    // std::cout << "LIRkdfcl = " << LIRkdfcl << std::endl;
    // std::cout << "Kdf = " << Kdf << std::endl;
    // std::cout << "PAR = " << PAR << std::endl;
    // std::cout << "PARIntercepte = " << PARIntercepte << std::endl;

}

void Model2_1::evolMobiliTillerDeath()
{
    if (NumPhase == 3 or (NumPhase == 4 and SDJCorPhase4 <= 0.7 * SDJRPR
                          and CulmsPerPlant >= 1)) {
        TillerDeathPop = (1 - (std::min(Ic, 1.))) * CulmsPop * CoeffTillerDeath;
        CulmsPop = CulmsPop - TillerDeathPop;
        CulmsPerPlant = CulmsPop / (Density * PlantsPerHill);
        CulmsPerHill = CulmsPerPlant * PlantsPerHill;
        DryMatStructPaniclePop = DryMatStructPaniclePop *
            std::max(0., CulmsPop) / (CulmsPop + TillerDeathPop);

        // std::cout << "NumPhase = " << NumPhase << std::endl;
        // std::cout << "SDJCorPhase4 = " << SDJCorPhase4 << std::endl;
        // std::cout << "SDJRPR = " << SDJRPR << std::endl;
        // std::cout << "CulmsPerPlant = " << CulmsPerPlant << std::endl;
        // std::cout << "Ic = " << Ic << std::endl;
        // std::cout << "CulmsPop = " << CulmsPop << std::endl;
        // std::cout << "CoeffTillerDeath = " << CoeffTillerDeath << std::endl;

    }
}

void Model2_1::evolMobiliLeafDeath()
{
    if (NumPhase > 1) {
        LeafDeathPop = (1 - (std::min(Ic, 1.))) * DryMatStructLeafPop *
            CoeffLeafDeath;
        DryMatStructLeafPop = DryMatStructLeafPop - LeafDeathPop;
        MobiliLeafDeath = 0.25 * LeafDeathPop;
        DeadLeafDrywtPop = DeadLeafDrywtPop + (0.75 * LeafDeathPop);
        LaiDead = DeadLeafDrywtPop * Sla;

        // std::cout << "NumPhase = " << NumPhase << std::endl;
        // std::cout << "Ic = " << Ic << std::endl;
        // std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << std::endl;
        // std::cout << "CoeffLeafDeath = " << CoeffLeafDeath << std::endl;
        // std::cout << "DeadLeafDrywtPop = " << DeadLeafDrywtPop << std::endl;
        // std::endl;

        // std::cout << "LeafDeathPop = " << LeafDeathPop << std::endl;

    }
}

void Model2_1::evalSupplyTot()
{
    SupplyTot = Assim + MobiliLeafDeath - RespMaintTot -
        std::max(0., RespMaintDebt);
    if (NumPhase < 7) {
        CumSupplyTot = CumSupplyTot + SupplyTot - MobiliLeafDeath ;
    } else {
        CumSupplyTot = 0;
    }
    if (SupplyTot <= 0) {
        RespMaintDebt = 0 - SupplyTot;
        SupplyTot = 0;
    } else {
        RespMaintDebt = 0;
    }

    // std::cout << "Assim = " << Assim << std::endl;
    // std::cout << "MobiliLeafDeath = " << MobiliLeafDeath << std::endl;
    // std::cout << "RespMaintTot = " << RespMaintTot << std::endl;
    // std::cout << "RespMaintDebt = " << RespMaintDebt << std::endl;
    // std::cout << "SupplyTot = " << SupplyTot << std::endl;
    // std::cout << "CumSupplyTot = " << CumSupplyTot << std::endl;
    // std::cout << "RespMaintDebt = " << RespMaintDebt << std::endl;

}

void Model2_1::evalDemandStructLeaf()
{
  double CorrectedSla;

    if (NumPhase > 1 and NumPhase < 5) {
        DemLeafAreaPlant = (std::pow((RelPotLeafLength * LeafLengthMax), 2) *
                            CoeffLeafWLRatio * 0.725 * PlantLeafNumNew /
                            1000000) * std::min(Cstr, StressCold);
        if (SlaNew == 0) {
            CorrectedSla = SlaMax;
        } else {
            CorrectedSla = SlaNew;
        }
        DemStructLeafPlant = DemLeafAreaPlant * 0.1 / CorrectedSla;
        DemStructLeafPop = DemStructLeafPlant * Density / 1000;
        A_DemStructLeaf = DemStructLeafPlant * Density / 1000;

        // std::cout << "RelPotLeafLength = " << RelPotLeafLength << std::endl;
        // std::cout << "LeafLengthMax = " << LeafLengthMax << std::endl;
        // std::cout << "CoeffLeafWLRatio = " << CoeffLeafWLRatio << std::endl;
        // std::cout << "PlantLeafNumNew = " << PlantLeafNumNew << std::endl;
        // std::cout << "Cstr = " << Cstr << std::endl;
        // std::cout << "StressCold = " << StressCold << std::endl;
        // std::cout << "DemLeafAreaPlant = " << DemLeafAreaPlant << std::endl;

    }
}

void Model2_1::evalDemandStructIN()
{
    if (PhaseStemElongation == 1) {
        DemStructInternodePlant = std::pow(std::min(Ic, 1.), 0.5) *
            ApexHeightGain * CulmsPerHill * CoeffInternodeMass;
        DemStructInternodePop = DemStructInternodePlant * Density / 1000;
    }
    if (NumPhase > 1 and NumPhase < 5) {
        DemResInternodePop = (ResCapacityInternodePop -
                              DryMatResInternodePop) * CoeffReserveSink;
    }
}

void Model2_1::evalDemandTotAndIcPreFlow()
{
    if (NumPhase > 1 and NumPhase < 5) {
        DemStructTotPop = DemStructLeafPop + DemStructSheathPop +
            DemStructRootPop + DemStructInternodePop +
            DemStructPaniclePop + DemResInternodePop;
        A_DemStructTot = DemStructLeafPop + DemStructSheathPop +
            DemStructRootPop + DemStructInternodePop +
            DemStructPaniclePop + DemResInternodePop;

        // std::cout << "SupplyTot = " << SupplyTot << std::endl;
        // std::cout << "DemStructTotPop = " << DemStructTotPop << std::endl;


        Ic = SupplyTot / DemStructTotPop;
        if (Ic <= 0) {
            Ic = 0;
        }
      if (PlantHeight == 0) {
          Ic = 1;
      }
      IcCumul = IcCumul + std::min(Ic, 1.);
      IcMean = IcCumul / NbDaysSinceGermination;
      CstrCumul = CstrCumul + Cstr;
      CstrMean = CstrCumul / NbDaysSinceGermination;

      // std::cout << "Ic = " << Ic << std::endl;
      // std::cout << "IcCumul = " << IcCumul << std::endl;
      // std::cout << "NbDaysSinceGermination = " << NbDaysSinceGermination << std::endl;
      // std::cout << "IcMean = " << IcMean << std::endl;


    }
    if (NumPhase == 5 or NumPhase == 6) {
        IcCumul = IcCumul + std::min(Ic, 1.);
        IcMean = IcCumul / NbDaysSinceGermination;
        CstrCumul = CstrCumul + Cstr;
        CstrMean = CstrCumul / NbDaysSinceGermination;
    }
}

void Model2_1::evolGrowthStructLeafPop()
{
    if (NumPhase > 1 and NumPhase < 5) {
        if (Ic < 1) {
            GrowthStructLeafPop = SupplyTot *
                (DemStructLeafPop / DemStructTotPop);
            A_GrowthStructLeaf = SupplyTot *
                (DemStructLeafPop / DemStructTotPop);
        } else {
            GrowthStructLeafPop = DemStructLeafPop;
            A_GrowthStructLeaf = DemStructLeafPop;
        }
    }
}

void Model2_1::evolGrowthStructINPop()
{
    if (NumPhase > 1 and NumPhase < 5) {
        if (Ic < 1) {
            GrowthStructInternodePop = SupplyTot *
                (DemStructInternodePop / DemStructTotPop);
            GrowthResInternodePop = SupplyTot *
                (DemResInternodePop / DemStructTotPop);
        } else {
            GrowthStructInternodePop = DemStructInternodePop;
            GrowthResInternodePop = DemResInternodePop;
        }
    }
}

void Model2_1::addResToGrowthStructPop()
{
    if (NumPhase > 1) {
        if (Ic < 1 and DemStructTotPop > 0) {
            ResInternodeMobiliDay = std::min(ResInternodeMobiliDayPot,
                                             GrowthStructDeficit);
            A_ResInternodeMobiliDay = std::min(ResInternodeMobiliDayPot,
                                               GrowthStructDeficit);
            GrowthStructLeafPop = GrowthStructLeafPop + ResInternodeMobiliDay *
                (DemStructLeafPop / DemStructTotPop);
            A_GrowthStructLeaf = GrowthStructLeafPop;
            GrowthStructSheathPop = GrowthStructSheathPop +
                ResInternodeMobiliDay * (DemStructSheathPop / DemStructTotPop);
            GrowthStructRootPop = GrowthStructRootPop + ResInternodeMobiliDay *
                (DemStructRootPop / DemStructTotPop);
            GrowthStructInternodePop = GrowthStructInternodePop +
                ResInternodeMobiliDay *
                (DemStructInternodePop / DemStructTotPop);
            GrowthStructPaniclePop = GrowthStructPaniclePop +
                ResInternodeMobiliDay * (DemStructPaniclePop / DemStructTotPop);
            GrowthStructTotPop = GrowthStructLeafPop + GrowthStructSheathPop +
                GrowthStructRootPop + GrowthStructInternodePop +
                GrowthStructPaniclePop + GrowthResInternodePop;
            A_GrowthStructTot = GrowthStructTotPop;
        }
    }
}

void Model2_1::evolDemPanFilPopAndIcPFlow()
{
    if (NumPhase == 5) {
        PanicleSinkPop = DryMatStructPaniclePop * CoeffPanSinkPop *
            (1 - SterilityTot);
        DemPanicleFillPop = (DegresDuJourCor / SDJMatu1) * PanicleSinkPop
            * std::sqrt(std::max(0.00001, StressCold));
        Ic = SupplyTot / std::max(DemPanicleFillPop, 0.0000001);
        if (Ic <= 0) {
            Ic = 0;
        }
    }
    if (NumPhase == 6) {
        Ic = Assim / RespMaintTot;
        if (Ic >= 1) {
            AssimSurplus = std::max(0., Assim - RespMaintTot);
            A_AssimSurplus = std::max(0., Assim - RespMaintTot);
        } else {
            AssimSurplus = 0;
            A_AssimSurplus = 0;
        }
        if (Ic < 0) {
            Ic = 0;
        }
    }
}

void Model2_1::evolPanicleFilPop()
{
    if (NumPhase == 5) {
        ResInternodeMobiliDayPot = RelMobiliInternodeMax *
            DryMatResInternodePop;
        if (Ic > 1) {
            PanicleFilPop = std::max(DemPanicleFillPop, 0.);
            PanicleFilDeficit = 0;
            AssimSurplus = SupplyTot - PanicleFilPop;
            A_AssimSurplus = SupplyTot - PanicleFilPop;
        } else {
            if (Ic <= 1) {
                PanicleFilDeficit = std::max((DemPanicleFillPop -
                                              std::max(SupplyTot, 0.)), 0.);
                ResInternodeMobiliDay = std::max(
                    std::min(ResInternodeMobiliDayPot, 0.5 *
                             PanicleFilDeficit), 0.);
                A_ResInternodeMobiliDay = std::max(
                    std::min( ResInternodeMobiliDayPot, 0.5 *
                              PanicleFilDeficit), 0.);
                PanicleFilPop = std::max((SupplyTot + ResInternodeMobiliDay),
                                         0.);
                AssimSurplus = 0;
                A_AssimSurplus = 0;
            }
        }
        GrainYieldPop = GrainYieldPop + PanicleFilPop;
    } else {
        if (NumPhase == 6) {
            AssimSurplus = Assim - RespMaintTot;
            A_AssimSurplus = Assim - RespMaintTot;
            ResInternodeMobiliDay = std::min(std::max(0., RespMaintTot - Assim),
                                             DryMatResInternodePop);
            A_ResInternodeMobiliDay = std::min(std::max(0.,
                                                        RespMaintTot - Assim),
                                               DryMatResInternodePop);

        } else {
            if (NumPhase > 6) {
                ResInternodeMobiliDay = 0;
                A_ResInternodeMobiliDay = 0;
            }
        }
    }
}

void Model2_1::evolGrowthReserveInternode()
{
    if (NumPhase >= 2) {
        DryMatResInternodePopOld = DryMatResInternodePop;
        ResCapacityInternodePop = (DryMatStructInternodePop +
                                   DryMatStructSheathPop) *
            CoeffResCapacityInternode;
        DryMatResInternodePop = DryMatResInternodePop + GrowthResInternodePop;
        IncreaseResInternodePop = std::min(
            std::max(AssimSurplus, 0.), std::max((ResCapacityInternodePop -
                                                  DryMatResInternodePop), 0.));
        A_IncreaseResInternodePop = std::min(
            std::max(AssimSurplus, 0.),
            std::max((ResCapacityInternodePop - DryMatResInternodePop), 0.));
        GrowthResInternodePop = IncreaseResInternodePop - ResInternodeMobiliDay;
        DryMatResInternodePop = DryMatResInternodePop + GrowthResInternodePop;
        AssimNotUsed = std::max((AssimSurplus - IncreaseResInternodePop), 0.);
        AssimNotUsedCum = AssimNotUsedCum + AssimNotUsed;
    }
}

void Model2_1::evolGrowthTot()
{
    if (NumPhase < 5) {
        GrowthStructTotPop = GrowthStructLeafPop + GrowthStructSheathPop +
            GrowthStructRootPop + GrowthStructInternodePop +
            GrowthStructPaniclePop;
        A_GrowthStructTot = GrowthStructTotPop;
    } else {
        GrowthStructTotPop = 0;
        A_GrowthStructTot = GrowthStructTotPop;
    }
    GrowthDryMatPop = GrowthStructTotPop +
        (DryMatResInternodePop -  DryMatResInternodePopOld) + PanicleFilPop;
}

void Model2_1::evalLai()
{
    double CorrectedSla;

    if (NumPhase == 2 and ChangePhase == 1) {
        CorrectedSla = SlaMax;
    } else {
        CorrectedSla = Sla;
        LastLeafLengthPot = RelPotLeafLength * LeafLengthMax;
        if (GrowthStructTotPop > 0) {
            LastLeafLength = LastLeafLengthPot *
                std::sqrt(std::max(GrowthStructLeafPop, 0.) / DemStructLeafPop);
        }
    }
    Lai = DryMatStructLeafPop * CorrectedSla;

    // std::cout << "SlaMax = " << SlaMax << std::endl;

    // std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << std::endl;



}

void Model2_1::evalClumpAndLightInter()
{
    double RolledLai;

    if (NumPhase > 1 and PlantWidth > 0) {
        RolledLai = Lai * KRolling * std::pow((1 - FractionPlantHeightSubmer),
                                              0.3);
        LIRkdf = 1 - std::exp(-Kdf * RolledLai);
        LIRkdfcl = (1 - std::exp(-Kdf * RolledLai * 10000 /
                                 std::min(10000., Density * M_PI *
                                          std::pow(PlantWidth / 2000, 2)))) *
            (std::min(10000., Density * M_PI *
                      std::pow(PlantWidth / 2000, 2)) / 10000);
        LTRkdf = 1 - LIRkdf;
        LTRkdfcl = 1 - LIRkdfcl;

        // std::cout << "Lai = " << Lai << std::endl;
        // std::cout << "KRolling = " << KRolling << std::endl;
        // std::cout << "FractionPlantHeightSubmer = " << FractionPlantHeightSubmer << std::endl;
        // std::cout << "RolledLai = " << RolledLai << std::endl;
        // std::cout << "LTRkdfcl = " << LTRkdfcl << std::endl;

    }
}

void Model2_1::evalRUE()
{
    double CorrectedIrrigation;

    if (NumPhase < 1 or (NumPhase == 1 and ChangePhase == 1) or
        Density == DensityNursery) {
        CumPar = 0;
        RUE = 0;
        CumTr = 0.00001;
        CumEt = 0.00001;
        CumWUse = 0.00001;
        CumWReceived = 0;
        CumIrrig = 0;
        CumDr = 0;
        CumLr = 0;
    } else {
        if (Transplanting == 0 or NurseryStatus == 1) {
            CumPar = CumPar + PARIntercepte;
            CumTr = CumTr + Tr;
            CumEt = CumEt + Tr + Evap;
            CumWUse = CumWUse + Tr + Evap + Dr + Lr;
        }
        if (Irrigation == NullValue) {
            CorrectedIrrigation = 0;
        } else  {
            CorrectedIrrigation = Irrigation;
        }
        if (Transplanting == 0 or NurseryStatus == 1) {
            CumWReceived = CumWReceived + Rain + CorrectedIrrigation +
                IrrigAutoDay;
            CumIrrig = CumIrrig + CorrectedIrrigation + IrrigAutoDay;
            CumDr = CumDr + Dr;
            CumLr = CumLr + Lr;
        }
        if (AssimPot != 0) {
            ConversionEff = Conversion * Assim /
                (PARIntercepte * Conversion * 10);
        }

        // std::cout << "Tr = " << Tr << std::endl;
        // std::cout << "NbJas = " << NbJas << std::endl;
        // std::cout << "NumPhase = " << NumPhase << std::endl;

        if (Tr > 0 and NbJas > 20 and NumPhase > 1) {
            TrEffInst = (SupplyTot - AssimNotUsed) / (Tr * 10000);
            TrEff = DryMatTotPop / (CumTr * 10000);
            WueEt = DryMatTotPop / (CumEt * 10000);
            WueTot = DryMatTotPop / (CumWuse * 10000);
            RUE = (DryMatAboveGroundTotPop / std::max(CumPar, 0.00001)) / 10;

            // std::cout << "CumPAR = " << CumPar << std::endl;
            // std::cout << "RUE = " << RUE << std::endl;

        }
    }
}

void Model2_1::evolEvapSurfRFE_RDE()
{
    double ValRSurfPrec;
    double EvapRU;
    double Evap1;
    double Evap2;

    // std::cout << "StockMacropores = " << StockMacropores << std::endl;
    // std::cout << "FloodwaterDepth = " << FloodwaterDepth << std::endl;

    if (StockMacropores + FloodwaterDepth == 0) {
        ValRSurfPrec = ValRSurf;

        // std::cout << "ValRSurf = " << ValRSurf << std::endl;
        // std::cout << "ValRFE = " << ValRFE << std::endl;

        if (ValRFE > 0) {
            if (ValRFE < EvapPot) {
                Evap1 = ValRFE;
                Evap2 = std::max(0., std::min(ValRSurf,
                                              ((EvapPot - ValRFE) * ValRSurf) /
                                              (CapaREvap + CapaRDE)));
            } else {
                Evap1 = EvapPot;
                Evap2 = 0;
            }
        } else {
            Evap1 = 0;
            Evap2 = std::max(0., std::min(ValRSurf, EvapPot * ValRSurf /
                                          (CapaREvap + CapaRDE)));

            // std::cout << "EvapPot = " << EvapPot << std::endl;
            // std::cout << "Evap1 = " << Evap1 << std::endl;
            // std::cout << "Evap2 = " << Evap2 << std::endl;


        }
        Evap = Evap1 + Evap2; // output variable
        ValRFE = ValRFE - Evap1;
        ValRSurf = ValRSurf - Evap2;
        ValRDE = std::max(0., ValRSurf - CapaREvap);

        // std::cout << "Evap = " << Evap << std::endl;
        // std::cout << "ValRFE = " << ValRFE << std::endl;
        // std::cout << "ValRSurf = " << ValRSurf << std::endl;
        // std::cout << "ValRDE = " << ValRDE << std::endl;
        // std::cout << "CapaREvap = " << CapaREvap << std::endl;


        if (EvapPot == 0) {
            Kr = 0;
        } else {
            Kr = Evap / EvapPot;
        }

        // std::cout << "Kr = " << Kr << std::endl;

        if (ValRSurf >= CapaREvap) {
            EvapRU = Evap;
        } else {
            if (ValRSurfPrec <= CapaREvap) {
                EvapRU = Evap1;
            } else {
                EvapRU = Evap1 + ValRSurfPrec - CapaREvap;
            }
        }

        // std::cout << "EvapRU = " << EvapRU << std::endl;

        if (RuRac <= RuSurf) {
            StockRac = std::max(0., StockRac - EvapRU * RuRac / RuSurf); // output variable
        } else {
            StockRac = std::max(0., StockRac - EvapRU); // output variable
        }

        // std::cout << "StockRac = " << StockRac << std::endl;

        StockTotal = StockTotal - EvapRU; // output variable

        // std::cout << "evolEvapSurfRFE_RDE()" << std::endl;
        // std::cout << "StockTotal = " << StockTotal << std::endl;


        // std::cout << "StockTotal = " << StockTotal << std::endl;

        StockRac = std::min(StockRac, StockTotal); // output variable

        // std::cout << "StockRac = " << StockRac << std::endl;

        KceReal = Kce * Kr;

        // std::cout << "KceReal = " << KceReal << std::endl;

    }
    if (StockMacropores + FloodwaterDepth > 0 and NumPhase > 0) {
        Evap = EvapPot;
        ValRSurf = CapaREvap + StockMacropores *
            (EpaisseurSurf /  (EpaisseurSurf + EpaisseurProf));
        ValRFE = CapaRFE + StockMacropores *
            (EpaisseurSurf / (EpaisseurSurf + EpaisseurProf));
        ValRDE = CapaRDE;
        StockRac = RuRac + StockMacropores *
            (RootFront / (EpaisseurSurf + EpaisseurProf));
        StockSurface = RuSurf + StockMacropores *
            (EpaisseurSurf / (EpaisseurSurf + EpaisseurProf));
        StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
            StockMacropores;
        StockRac = std::min(StockRac, StockTotal);
        Kr = 1;
        KceReal = Kce;
    }
    if (StockMacropores + FloodwaterDepth > 0 and NumPhase == 0) {
        Evap = EvapPot;
        FloodwaterDepth = FloodwaterDepth - Evap;
        if (FloodwaterDepth < 0) {
            StockMacropores = StockMacropores + FloodwaterDepth;
            FloodwaterDepth = 0;
            StockMacropores = std::max(StockMacropores, 0.);
        }
        ValRSurf = CapaREvap + StockMacropores *
            (EpaisseurSurf / (EpaisseurSurf + EpaisseurProf));
        ValRFE = CapaRFE + StockMacropores *
            (EpaisseurSurf / (EpaisseurSurf + EpaisseurProf));
        ValRDE = CapaRDE;
        StockSurface = RuSurf + StockMacropores *
            (EpaisseurSurf / (EpaisseurSurf + EpaisseurProf));
        StockTotal = (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
            StockMacropores;
        Kr = 1;
        KceReal = Kce;
    }
}

void Model2_1::evolDryMatTot()
{
    CumGrowthPop = CumGrowthPop + GrowthStructLeafPop + GrowthStructSheathPop +
        GrowthStructInternodePop + GrowthStructRootPop +
        GrowthStructPaniclePop + (DryMatResInternodePop -
                                  DryMatResInternodePopOld) + PanicleFilPop -
        MobiliLeafDeath;
    GrowthPop = GrowthStructLeafPop + GrowthStructSheathPop +
        GrowthStructInternodePop + GrowthStructRootPop +
        GrowthStructPaniclePop + (DryMatResInternodePop -
                                  DryMatResInternodePopOld) + PanicleFilPop -
        MobiliLeafDeath;

    // std::cout << "ChangePhase = " << ChangePhase << std::endl;

    if (NumPhase == 2 and ChangePhase == 1) {
        DryMatTotPop = Density * PlantsPerHill * TxResGrain * PoidsSecGrain /
            1000;

        // std::cout << "Density = " << Density  << std::endl;
        // std::cout << "PlantsPerHill = " << PlantsPerHill << std::endl;
        // std::cout << "TxResGrain = "  << TxResGrain << std::endl;
        // std::cout << "PoidsSecGrain = " << PoidsSecGrain << std::endl;
        // std::cout << "DryMatTotPop = " << DryMatTotPop << std::endl;

        DryMatStructLeafPop = DryMatTotPop * 0.5;

        // std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << std::endl;

    } else {
        if (NumPhase > 1) {

            // std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << std::endl;

            DryMatStructLeafPop = DryMatStructLeafPop + GrowthStructLeafPop;

            // std::cout << "GrowthStructLeafPop = " << GrowthStructLeafPop << std::endl;

            DryMatStructSheathPop = DryMatStructSheathPop +
                GrowthStructSheathPop;
            DryMatStructRootPop = DryMatStructRootPop + GrowthStructRootPop;
            DryMatStructInternodePop = DryMatStructInternodePop +
                GrowthStructInternodePop;
            DryMatStructPaniclePop = DryMatStructPaniclePop +
                GrowthStructPaniclePop;
            DryMatStemPop = DryMatStructSheathPop + DryMatStructInternodePop
                + DryMatResInternodePop;
            DryMatStructTotPop = DryMatStructLeafPop + DryMatStructSheathPop +
                DryMatStructRootPop + DryMatStructInternodePop +
                DryMatStructPaniclePop;
            DryMatVegeTotPop = DryMatStemPop + DryMatStructLeafPop +
                DryMatStructRootPop + DeadLeafDrywtPop;
            DryMatPanicleTotPop = DryMatStructPaniclePop + GrainYieldPop;
            DryMatTotPop = DryMatVegeTotPop + DryMatPanicleTotPop;
            DryMatAboveGroundPop = DryMatTotPop - DryMatStructRootPop -
                DeadLeafDrywtPop;
            DryMatAboveGroundTotPop = DryMatAboveGroundPop + DeadLeafDrywtPop;
            CumCarbonUsedPop = DryMatTotPop + AssimNotUsedCum;
            RootShootRatio = DryMatStructRootPop / DryMatAboveGroundPop;
            if (ResCapacityInternodePop == 0) {
                InternodeResStatus = 0;
            } else {
                InternodeResStatus = DryMatResInternodePop /
                    ResCapacityInternodePop;
            }
        }
        if (NumPhase > 4) {
            HarvestIndex = GrainYieldPop / DryMatAboveGroundTotPop;
            PanicleNumPlant = CulmsPerPlant;
            PanicleNumPop = CulmsPerPlant * Density * PlantsPerHill;
            GrainYieldPanicle = 1000 * GrainYieldPop / PanicleNumPop;
            SpikeNumPop = 1000 * DryMatStructPaniclePop * CoeffPanSinkPop /
                PoidsSecGrain;
            SpikeNumPanicle = SpikeNumPop / PanicleNumPop;
            FertSpikeNumPop = SpikeNumPop * (1 - SterilityTot);
            GrainFillingStatus = 1000 * (GrainYieldPop / FertSpikeNumPop) /
                PoidsSecGrain;
        }
    }
}

void Model2_1::evolGrowthStructTot()
{
    if (NumPhase > 1 and NumPhase < 5) {
        GrowthStructTotPop = GrowthStructLeafPop + GrowthStructSheathPop +
            GrowthStructRootPop + GrowthStructInternodePop +
            GrowthStructPaniclePop + GrowthResInternodePop;
        A_GrowthStructTot = GrowthStructTotPop;
        AssimSurplus = std::max((SupplyTot - GrowthStructTotPop), 0.);
        A_AssimSurplus = std::max((SupplyTot - GrowthStructTotPop), 0.);
    } else {
        GrowthStructLeafPop = 0;
        A_GrowthStructLeaf = GrowthStructLeafPop;
        GrowthStructSheathPop = 0;
        GrowthStructInternodePop = 0;
        GrowthStructRootPop = 0;
        GrowthStructPaniclePop = 0;
        GrowthStructTotPop = 0;
        A_GrowthStructTot = GrowthStructTotPop;
    }
}

void Model2_1::leafRolling()
{
    if (NumPhase > 1) {
        KRolling = RollingBase + (1 - RollingBase) *
            std::pow(FTSW,  std::max(0.0000001, Eto * RollingSens));
        if (KRolling > 1) {
            KRolling = 1;
        }
    }
}

void Model2_1::evalAssimPot()
{
    // std::cout << "DayLength = " << DayLength << std::endl;
    // std::cout << "CO2Exp = " << CO2Exp << std::endl;
    // std::cout << "CO2Cp = " << CO2Cp << std::endl;
    // std::cout << "Ca = " << Ca << std::endl;

    if (-CO2Exp != 0 and CO2Cp != 0) {
        CoeffCO2Assim = (1 - std::exp(-CO2Exp * (Ca - CO2Cp))) /
            (1 - std::exp(-CO2Exp * (400 - CO2Cp)));
    }

    // std::cout << "CoeffCO2Assim = " << CoeffCO2Assim << std::endl;

    // std::cout << "PARIntercepte = " << PARIntercepte << std::endl;
    // std::cout << "Conversion = " << Conversion << std::endl;
    // std::cout << "PAR = " << PAR << std::endl;

    AssimPot = PARIntercepte * Conversion * 10 * CoeffCO2Assim;
    AssimPot = AssimPot * std::min(((3 * TMax + TMin) / 4 - TBase) /
                                   (TOpt1 - TBase), 1.);
    AssimPot = AssimPot * std::sqrt(std::max(0.00001, StressCold));
    if (PARIntercepte != 0 and DayLength != 0) {
        AssimPot = AssimPot * std::pow((PAR / DayLength), 0.667) /
            (PAR / DayLength);
        AssimPot = AssimPot * std::pow((SlaMin / std::max(Sla, SlaMin)),
                                       CoeffAssimSla);
    }
}

void Model2_1::automaticIrrigation()
{
    double IrrigAutoTargetCor;
    double CorrectedIrrigation;
    double CorrectedBundHeight;
    double StressPeriod;

    CorrectedBundHeight = BundHeight;
    if (Irrigation == NullValue) {
        CorrectedIrrigation = 0;
    } else {
        CorrectedIrrigation = Irrigation;
    }
    if (NumPhase > 4 and NumPhase < 7 and DAF > PlotDrainageDAF) {
        CorrectedBundHeight = 0;
    }
    if (NbJas >= IrrigAutoStop and NbJas < IrrigAutoResume) {
        StressPeriod = 1;
    } else {
        StressPeriod = 0;
    }
    if (StressPeriod == 1 and FloodwaterDepth > 0) {
        Lr = Lr + FloodwaterDepth;
        FloodwaterDepth = 0;
    }
    if (NumPhase < 7 and DAF <= PlotDrainageDAF and IrrigAuto == 1 and
        NumPhase > 0 and CorrectedBundHeight > 0 and FTSW <= FTSWIrrig and
        StressPeriod == 0) {
        IrrigAutoTargetCor = std::min((IrrigAutoTarget * BundHeight),
                                      (0.5 * PlantHeight));
        if (NumPhase == 1) {
            IrrigAutoTargetCor = std::max(IrrigAutoTargetCor, BundHeight / 2);
        }
        IrrigAutoDay = std::max(0., (IrrigAutoTargetCor - FloodwaterDepth)) +
            + (VolMacropores - StockMacropores) + RuRac *
            (1 - (std::min(FTSW, 1.)));
        if (ChangeNurseryStatus == 1) {
            IrrigAutoDay = VolMacropores + RuSurf + PercolationMax;
        }
        if (StockMacropores + FloodwaterDepth == 0) {
            EauDispo = Rain + CorrectedIrrigation + IrrigAutoDay;
        } else {
            FloodwaterDepth = FloodwaterDepth + IrrigAutoDay;
            if (VolMacropores > 0 and StockMacropores < VolMacropores and
                FloodwaterDepth > 0) {
                StockMacropores = StockMacropores + FloodwaterDepth;
                FloodwaterDepth = std::max(0., StockMacropores - VolMacropores);
                StockMacropores = StockMacropores - FloodwaterDepth;
                if (RootFront == 0) {
                    RuRac = ResUtil * ProfRacIni / 1000;
                } else {
                    RuRac = ResUtil * RootFront / 1000;
                }
                if (RootFront == 0) {
                    StockRac = RuRac + StockMacropores * ProfRacIni /
                        (EpaisseurSurf + EpaisseurProf);
                } else {
                    StockRac = RuRac + StockMacropores * RootFront /
                        (EpaisseurSurf + EpaisseurProf);
                }
                FTSW = StockRac / RuRac;
            }
            EauDispo = StockMacropores + FloodwaterDepth;
        }
    } else {
        if (NumPhase < 7 and DAF <= PlotDrainageDAF and IrrigAuto == 1 and
            NumPhase > 0 and CorrectedBundHeight == 0) {
            FloodwaterDepth = 0;
        }
    }
    IrrigTotDay = CorrectedIrrigation + IrrigAutoDay;
}

void Model2_1::evolRurRFE_RDE()
{
    double DeltaRur;

    // std::cout << "NumPhase = " << NumPhase << std::endl;

    if (NumPhase == 0) {
        RuRac = 0;
        StockRac = 0;
    } else {
        if (NumPhase == 1 and ChangePhase == 1) {

            // std::cout << "ResUtil = " << ResUtil << std::endl;
            // std::cout << "ProfRacIni = " << ProfRacIni << std::endl;
            // std::cout << "EpaisseurSurf = " << EpaisseurSurf << std::endl;
            // std::cout << "EpaisseurProf = " << EpaisseurProf << std::endl;
            // std::cout << "ResUtil = " << ResUtil << std::endl;

            RuRac = ResUtil * std::min(ProfRacIni,
                                       (EpaisseurSurf + EpaisseurProf)) / 1000;

            // std::cout << "RuRac = " << RuRac << std::endl;

            if (ProfRacIni <= EpaisseurSurf) {
                StockRac = (ValRDE + ValRFE) * ProfRacIni / EpaisseurSurf;
            } else {
                StockRac = StockTotal *
                    std::min(ProfRacIni /  (EpaisseurSurf + EpaisseurProf), 1.);
            }

            // std::cout << "StockRac = " << StockRac << std::endl;

        } else {
            if (Hum - StockMacropores - RuRac < VitesseRacinaire / 1000 *
                ResUtil) {
                DeltaRur = std::max(0., Hum - StockMacropores - RuRac);
                if (RootFront >= EpaisseurSurf + EpaisseurProf or
                    RootFront >= RootFrontMax) {
                    DeltaRur = 0;
                }
            } else {
                DeltaRur = VitesseRacinaire / 1000 * ResUtil;
                if (RootFront >= EpaisseurSurf + EpaisseurProf or
                    RootFront >= RootFrontMax) {
                    DeltaRur = 0;
                }
            }

            // std::cout << "RootFront = " << RootFront << std::endl;
            // std::cout << "RootFrontMax = " << RootFrontMax << std::endl;
            // std::cout << "ResUtil = " << ResUtil << std::endl;
            // std::cout << "VitesseRacinaire = " << VitesseRacinaire << std::endl;
            // std::cout << "DeltaRur = " << DeltaRur << std::endl;

            RuRac = RuRac + DeltaRur;
            if (RuRac > RuSurf) {
                StockRac = StockRac + DeltaRur;
            } else {
                StockRac = (ValRDE + ValRFE) * (RuRac / RuSurf);
            }

            // std::cout << "StockRac = " << StockRac << std::endl;

        }
    }
    if (NumPhase != 0) {
        RootFront = ((1 / ResUtil) * RuRac) * 1000;

        // std::cout << "RootFront = " << RootFront << std::endl;

        if (ChangeNurseryStatus == 1 and Transplanting == 1) {
            RootFront = TransplantingDepth;
            if (RootFront < 40) {
                RootFront = 40;
            } else if (RootFront > 200) {
                RootFront = 200;
            }
            RuRac = RootFront * ResUtil / 1000;
        }
    }

    // std::cout << "RuRac = " << RuRac << std::endl;

    if (StockMacropores + FloodwaterDepth > 0) {
        StockRac = RootFront * ResUtil / 1000 +
            (RootFront / (EpaisseurSurf + EpaisseurProf)) * StockMacropores;
        StockRac = std::min(StockRac, StockTotal);
    }
}

void Model2_1::evalSimEndCycle()

{
    if (NumPhase == 7 and ChangePhase == 1) {
        SimEndCycle = NbJas;
    }
}

void Model2_1::priority2GrowthPanStrctPop()
{
    double GrowthPanDeficit;
    double GrowthStructPaniclePlus;

    if (GrowthStructPaniclePop < DemStructPaniclePop and NumPhase == 4) {
        GrowthPanDeficit = DemStructPaniclePop - GrowthStructPaniclePop;
        GrowthStructPaniclePlus = std::min(PriorityPan * GrowthPanDeficit,
                                           GrowthStructTotPop -
                                           GrowthStructPaniclePop);
        GrowthStructPaniclePop = GrowthStructPaniclePop +
            GrowthStructPaniclePlus;
        GrowthStructInternodePop = GrowthStructInternodePop -
            GrowthStructPaniclePlus * (DemStructInternodePop / DemStructTotPop);
        GrowthStructLeafPop = GrowthStructLeafPop - GrowthStructPaniclePlus *
            (DemStructLeafPop / DemStructTotPop);
        GrowthStructSheathPop = GrowthStructSheathPop -
            GrowthStructPaniclePlus * (DemStructSheathPop / DemStructTotPop);
        GrowthStructRootPop = GrowthStructRootPop - GrowthStructPaniclePlus *
            (DemStructRootPop / DemStructTotPop);
        GrowthResInternodePop = GrowthResInternodePop -
            GrowthStructPaniclePlus * (DemResInternodePop / DemStructTotPop);
    }
}

void Model2_1::keyResults()
{
    if (NumPhase > 1 and NumPhase < 7) {
        CulmsPerPlantMax = std::max(CulmsPerPlant, CulmsPerPlantMax);
        CulmsPerHillMax = std::max(CulmsPerHill, CulmsPerHillMax);
    }
    if (NumPhase == 1) {
        DurPhase1 = DurPhase1 + 1;
    }
    if (NumPhase == 2) {
        DurPhase2 = DurPhase2 + 1;
        CumCstrPhase2 = CumCstrPhase2 + Cstr;
        CumFtswPhase2 = CumFtswPhase2 + FTSW;
        CumIcPhase2 = CumIcPhase2 + Ic;
    }
    if (NumPhase == 3) {
        DurPhase3 = DurPhase3 + 1;
        CumCstrPhase3 = CumCstrPhase3 + Cstr;
        CumFtswPhase3 = CumFtswPhase3 + FTSW;
        CumIcPhase3 = CumIcPhase3 + Ic;
    }
    if (NumPhase == 4) {
        DurPhase4 = DurPhase4 + 1;
        CumCstrPhase4 = CumCstrPhase4 + Cstr;
        CumFtswPhase4 = CumFtswPhase4 + FTSW;
        CumIcPhase4 = CumIcPhase4 + Ic;
    }
    if (NumPhase == 5) {
        DurPhase5 = DurPhase5 + 1;
        CumCstrPhase5 = CumCstrPhase5 + Cstr;
        CumFtswPhase5 = CumFtswPhase5 + FTSW;
        CumIcPhase5 = CumIcPhase5 + Ic;
    }
    if (NumPhase == 6) {
        DurPhase6 = DurPhase6 + 1;
        CumCstrPhase6 = CumCstrPhase6 + Cstr;
        CumFtswPhase6 = CumFtswPhase6 + FTSW;
        CumIcPhase6 = CumIcPhase6 + Ic;
    }
    if (NumPhase == 7) {
        IcPhase2 = CumIcPhase2 / std::max(DurPhase2, 0.1);
        IcPhase3 = CumIcPhase3 / std::max(DurPhase3, 0.1);
        IcPhase4 = CumIcPhase4 / std::max(DurPhase4, 0.1);
        IcPhase5 = CumIcPhase5 / std::max(DurPhase5, 0.1);
        IcPhase6 = CumIcPhase6 / std::max(DurPhase6, 0.1);
        FtswPhase2 = CumFtswPhase2 / std::max(DurPhase2, 0.1);
        FtswPhase3 = CumFtswPhase3 / std::max(DurPhase3, 0.1);
        FtswPhase4 = CumFtswPhase4 / std::max(DurPhase4, 0.1);
        FtswPhase5 = CumFtswPhase5 / std::max(DurPhase5, 0.1);
        FtswPhase6 = CumFtswPhase6 / std::max(DurPhase6, 0.1);
        CstrPhase2 = CumCstrPhase2 / std::max(DurPhase2, 0.1);
        CstrPhase3 = CumCstrPhase3 / std::max(DurPhase3, 0.1);
        CstrPhase4 = CumCstrPhase4 / std::max(DurPhase4, 0.1);
        CstrPhase5 = CumCstrPhase5 / std::max(DurPhase5, 0.1);
        CstrPhase6 = CumCstrPhase6 / std::max(DurPhase6, 0.1);
        DurGermFlow = DurPhase2 + DurPhase3 + DurPhase4;
        DurGermMat = DurPhase2 + DurPhase3 + DurPhase4 + DurPhase5 + DurPhase6;
        LaiFin = Lai;
        CulmsPerHillFin = CulmsPerHill;
        CulmsPerPlantFin = CulmsPerPlant;
        GrainYieldPopFin = GrainYieldPop;
        DryMatAboveGroundPopFin = DryMatAboveGroundPop;
        ReservePopFin = DryMatResInternodePop;
        DryMatTotPopFin = DryMatTotPop;
        GrainFillingStatusFin = GrainFillingStatus;
        SterilityTotFin = SterilityTot;
        CumIrrigFin = CumIrrig;
        CumWUseFin = CumWUse;
    }
}

void Model::degToRad()
{
    LatRad = Latitude * M_PI / 180;
    // std::cout << "LatRad = " << LatRad << std::endl;
}

void Model::avgTempHum()
{
    if (TMin != NullValue and TMax != NullValue) {
        TMoyCalc = (TMax + TMin) / 2;
    } else {
        TMoyCalc = TMoy;
    }
    if (HMin != NullValue and HMax != NullValue) {
        HMoyCalc = (HMax + HMin) / 2;
    } else {
        HMoyCalc = HMoy;
    }
    // std::cout << "TMoyCalc = " << TMoyCalc << std::endl;
    // std::cout << "HMoyCalc = " << HMoyCalc << std::endl;
}

void Model::evalDecli(double t)
{
    Decli = 0.409 * std::sin(0.0172 * utils::DateTime::dayOfYear(t) - 1.39);
    // std::cout << "Decli = " << Decli << std::endl;
}

void Model::evalSunPosi()
{
    SunPosi = std::acos(-std::tan(LatRad) * std::tan(Decli));
    // std::cout << "SunPosi = " << SunPosi << std::endl;
}

void Model::evalDayLength()
{
    DayLength = 7.64 * SunPosi;
    // std::cout << "DayLength = " << DayLength << std::endl;
}

void Model::evalSunDistance(double t)
{
    SunDistance = 1 + 0.033 *
        std::cos(2 * M_PI / 365 * utils::DateTime::dayOfYear(t));
    // std::cout << "SunDistance = " << SunDistance << std::endl;
}

void Model::evalRayExtra()
{
    RayExtra = 24 * 60 * 0.0820 / M_PI * SunDistance *
        (SunPosi * std::sin(Decli) * std::sin(LatRad) +
         std::cos(Decli) * std::cos(LatRad) * std::sin(SunPosi));
    // std::cout << "RayExtra = " << RayExtra << std::endl;
}

void Model::evalRgMax()
{
    RgMax = (0.75 + 0.00002 * Altitude) * RayExtra;
    // std::cout << "RgMax = " << RgMax << std::endl;
}

void Model::insToRg()
{
    if (Rg == NullValue) {
        RgCalc = (0.25 + 0.50 * std::min(Ins / DayLength, 1.)) * RayExtra;
    } else {
        RgCalc = Rg;
    }
    // std::cout << "RGCalc = " << RGCalc << std::endl;
}

void Model::evalPAR()
{
    PAR = KPar * RgCalc;
    // std::cout << "KPar = " << KPar << std::endl;
    // std::cout << "RgCalc = " << RgCalc << std::endl;
    // std::cout << "PAR = " << PAR << std::endl;
}

void Model::etoFAO()
{
    double eActual;
    double eSat;
    double RgRgMax;
    double Tlat;
    double delta;
    double Kpsy;
    double Eaero;
    double Erad;
    double Rn;
    double G;

    if (ETP == NullValue) {
        eSat = 0.3054 * (std::exp(17.27 * TMax / (TMax + 237.3)) +
                         std::exp(17.27 * TMin / (TMin + 237.3)));
        if (HMax == NullValue) {
            eActual = eSat * HMoy / 100;
        } else {
            eActual = 0.3054 * (std::exp(17.27 * TMax / (TMax + 237.3)) *
                                HMin / 100 + std::exp(17.27 * TMin /
                                                       (TMin + 237.3)) *
                                HMax / 100);
        }
        VPD = eSat - eActual;
        RgRgMax = RgCalc / RgMax;
        if (RgRgMax > 1) {
            RgRgMax = 1;
        }
        Rn = 0.77 * RgCalc - (1.35 * RgRgMax - 0.35) *
            (0.34 - 0.14 * std::pow(eActual, 0.5)) *
            (std::pow(TMax + 273.16, 4) + std::pow(TMin + 273.16, 4)) *
            2.45015 * std::pow(10, -9);
        Tlat = 2.501 - 2.361 * std::pow(10, -3) * TMoyCalc;
        delta = 4098 * (0.6108 * std::exp(17.27 * TMoyCalc / (TMoyCalc + 237.3))) /
            std::pow(TMoyCalc + 237.3, 2);
        Kpsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Altitude / 293),
                                          5.26) / Tlat;
        G = 0.38 * (TMoyCalc - TMoyPrec);
        Erad = 0.408 * (Rn - G) * delta / (delta + Kpsy * (1 + 0.34 * Vent));
        Eaero = (900 / (TMoyCalc + 273.16)) * ((eSat - eActual) * Vent) * Kpsy /
            (delta + Kpsy * (1 + 0.34 * Vent));
        Eto = Erad + Eaero;
    } else {
        Eto = ETP;
    }
    TMoyPrec = TMoyCalc;
}

void Model::evolPhenoStress(double t)
{

    if (t >= DateSemis) {

        bool ChangementDePhase = false;
        bool ChangementDeSousPhase = false;

        // std::cout << "NumPhase = " << NumPhase << std::endl;
        // std::cout << "SeuilTempPhaseSuivante = " << SeuilTempPhaseSuivante << std::endl;
        // std::cout << "SommeDegresJour = " << SommeDegresJour << std::endl;
        // std::cout << "StockSurface = " << StockSurface << std::endl;
        // std::cout << "TxRuSurfGermi = " << TxRuSurfGermi << std::endl;
        // std::cout << "RuSurf = " << RuSurf << std::endl;
        // std::cout << "StockTotal = " << StockTotal << std::endl;
        // std::cout << "SumPP = " << SumPP << std::endl;
        // std::cout << "PPSens = " << PPSens << std::endl;


        ChangePhase = 0;
        ChangeSousPhase = 0;
        if ((int)(NumPhase) == 0) {
            if (StockSurface >= TxRuSurfGermi * RuSurf or StockTotal > StockSurface) {
                NumPhase = 1;
                ChangePhase = 1;
                SeuilTempPhaseSuivante = SDJLevee;
            }
        } else {
            if ((int)(NumPhase) == 2 and
                SommeDegresJour >= SeuilTempPhaseSuivante) {
                ChangementDePhase = true;
            } else {
                if ((int)(NumPhase) != 3) {
                    ChangementDePhase = (SommeDegresJour >= SeuilTempPhaseSuivante);
                } else {
                    ChangementDePhase = (SumPP <= PPSens);
                }
            }
        }
        if (ChangementDePhase) {
            if ((NumPhase + 1) > 7) {
                NumPhase = 7;
            } else {
                ChangePhase = 1;
                NumPhase = NumPhase + 1;
                SommeDegresJourPhasePrecedente = SeuilTempPhaseSuivante;
                switch ((int)(NumPhase)) {
                case 2: SeuilTempPhaseSuivante = SeuilTempPhaseSuivante +
                        SDJBVP; break;
                case 3: SumPP = 0; break;
                case 4:
                    SeuilTempSousPhaseSuivante = SeuilTempPhaseSuivante + SDJRPR
                        / 4;
                    NumSousPhase = 1;
                    MonCompteur = 0;
                    ChangeSousPhase = 1;
                    SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SDJRPR;
                    break;
                case 5: SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SDJMatu1; break;
                case 6: SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SDJMatu2; break;
                }
            }
        }
        if ((int)(NumPhase) == 4) {
            ChangementDeSousPhase = (SommeDegresJour >=
                                     SeuilTempSousPhaseSuivante);

            // std::cout << "SommeDegresJour = " << SommeDegresJour << std::endl;
            // std::cout << "SeuilTempSousPhaseSuivante = " << SeuilTempSousPhaseSuivante << std::endl;
            // std::cout << "ChangementDeSousPhase = " << ChangementDeSousPhase << std::endl;

            if (ChangementDeSousPhase) {
                SeuilTempSousPhaseSuivante = SeuilTempSousPhaseSuivante +
                    SDJRPR / 4;
                NumSousPhase = NumSousPhase + 1;
                MonCompteur = 1;
                ChangeSousPhase = 1;
            } else {
                MonCompteur++;
            }
        }
    }

    // std::cout << "NumPhase = " << NumPhase << std::endl;

}



void Model2_1::demandePlante()
{
    TrPot = Kcp * Eto;
    CoeffCO2Tr = Ca * CO2SlopeTr - 400 * CO2SlopeTr + 1;
    TrPot = TrPot * CoeffCO2Tr;

     // std::cout << "Kcp = " << Kcp << std::endl;
     // std::cout << "Eto = " << Eto << std::endl;
     // std::cout << "Ca = " << Ca << std::endl;
     // std::cout << "CO2SlopeTr = " << CO2SlopeTr << std::endl;
     // std::cout << "CoeffCO2Tr = " << CoeffCO2Tr << std::endl;
     // std::cout << "TrPot = " << TrPot << std::endl;
}

void Model::evalTranspi()
{
    Tr = TrPot * Cstr;

    // std::cout << "Tr = " << Tr << std::endl;
}

void Model::evalETRETM()
{
    ETM = Evap + TrPot;
    ETR = Evap + Tr;

    // std::cout << "TrPot = " << TrPot << std::endl;
    // std::cout << "Tr = " << Tr << std::endl;
    // std::cout << "Evap = " << Evap << std::endl;
    // std::cout << "ETM = " << ETM << std::endl;
    // std::cout << "ETR = " << ETR << std::endl;

}

void Model::evolSomDegresJour()
{
    if (NumPhase >= 1) {
        SommeDegresJour = SommeDegresJour + DegresDuJour;
    } else {
        SommeDegresJour = 0;
    }

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "DegresDuJour = " << DegresDuJour << std::endl;
    // std::cout << "SommeDegresJour = " << SommeDegresJour << std::endl;

}

void Model::mortality()
{
    double MoyenneCstr;

    if (NumPhase >= 2) {
        NbJourCompte = NbJourCompte + 1;
        if (tabCstrIndiceCourant == 5) {
            tabCstrIndiceCourant = 1;
            tabCstr[tabCstrIndiceCourant] = Cstr;
        } else {
            tabCstrIndiceCourant = tabCstrIndiceCourant + 1;
            tabCstr[tabCstrIndiceCourant] = Cstr;
        }
        if (NbJourCompte >= 5) {
            MoyenneCstr = 0;
            for (int i = 0; i < 5; ++i) {
                MoyenneCstr = MoyenneCstr + tabCstr[i];
            }
            if (MoyenneCstr / 5 <= SeuilCstrMortality) {
                NumPhase = 7;
            }
        }
    }
}

void Model::evalTMaxMoy() {

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "NumSousPhase = " << NumSousPhase << std::endl;

    if (NumPhase == 4 and NumSousPhase == 4) {
        TMaxMoy = calculeLaMoyenne(TMax, MonCompteur, TMaxMoy);
    } else if (NumPhase < 4) {
        TMaxMoy = 0;
    }

    // std::cout << "TMaxMoy = " << TMaxMoy << std::endl;

}

void Model::evalTMinMoy() {

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "NumSousPhase = " << NumSousPhase << std::endl;

    if (NumPhase == 4 and NumSousPhase == 3) {
        TMinMoy = calculeLaMoyenne(TMin, MonCompteur, TMinMoy);
    } else if (NumPhase < 4) {
        TMinMoy = 0;
    }

    // std::cout << "TMinMoy = " << TMinMoy << std::endl;

}

void Model::evalFtswMoy() {

    // std::cout << "NumPhase = " << NumPhase << std::endl;
    // std::cout << "NumSousPhase = " << NumSousPhase << std::endl;

    if (NumPhase == 4 and NumSousPhase == 4) {
        FtswMoy = calculeLaMoyenne(FTSW, MonCompteur, FtswMoy);
    } else if (NumPhase < 4) {
        FtswMoy = 0;
    }

    // std::cout << "FtswMoy = " << FtswMoy << std::endl;

}

double Model::calculeLaMoyenne(double laValeur, double leCompteur, double laMoyenne) {
    double moyenne;

    moyenne = (laMoyenne * (leCompteur - 1) + laValeur) / leCompteur;
    return moyenne;
}

void Model2_1::evalNbJas(double t) {

    NbJas = t - DateSemis;

    // std::cout << "NbJas (calcule) = " << NbJas << std::endl;
}


} } } // namespace model models samara
