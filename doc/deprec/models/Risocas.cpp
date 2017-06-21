/**
 * @file models/Risocas.hpp
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

#include <models/SamaraModel.hpp>
#include <models/Model2.hpp>
#include <models/Model2_1.hpp>
#include <models/Model2_2.hpp>

#include <artis/utils/DateTime.hpp>

#include <algorithm>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <cmath>

using namespace artis;

void SamaraModel2::evalDegreeDay()
{
    double V, V1, V3;
    double S1, S2, S3;
    double Tn, Tx;

    if (TMax != TMin) {
        if (TMax <= TBase || TMin >= TLet) {
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
    if (NumPhase > 1 && NumPhase < 5)
    {
        DegresDuJourCor = DegresDuJour * std::pow(std::max(Cstr, 0.00000001),
                                                  DEVcstr);
    } else {
        DegresDuJourCor = DegresDuJour;
    }
    DegresDuJourCor = DegresDuJourCor * StressCold;
    //std::cout << "StressCold = " << StressCold << " - line " << __LINE__ << std::endl;
    //std::cout << "TMin = " << TMin << " - line " << __LINE__ << std::endl;
    //std::cout << "TMax = " << TMax << " - line " << __LINE__ << std::endl;
    //std::cout << "TBase = " << TBase << " - line " << __LINE__ << std::endl;
    //std::cout << "TLet = " << TLet << " - line " << __LINE__ << std::endl;
    //std::cout << "TOpt1 = " << TOpt1 << " - line " << __LINE__ << std::endl;
    //std::cout << "TOpt2 = " << TOpt2 << " - line " << __LINE__ << std::endl;
    //std::cout << "DegresDuJour = " << DegresDuJour << " - line " << __LINE__ << std::endl;
    //std::cout << "DegresDuJourCor = " << DegresDuJourCor << " - line " << __LINE__ << std::endl;
}


void SamaraModel::phyllochron()
{
    if (NumPhase > 1 && NumPhase < 5) {
        if ((NumPhase > 3 || HaunIndex > 20) && NumPhase < 5) {
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
        //    } else if (NumPhase == 7 && ChangePhase == 1){      //  avt present ds le code
        //        HaunIndex = 0;
    } else {
        HaunGain = 0;
        PhaseStemElongation = 0;
    }

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "Phyllo = " << Phyllo << " - line " << __LINE__ << std::endl;
    //std::cout << "RelPhylloPhaseStemElong = " << RelPhylloPhaseStemElong << " - line " << __LINE__ << std::endl;
    //std::cout << "DegresDuJourCor = " << DegresDuJourCor << " - line " << __LINE__ << std::endl;
    //std::cout << "PhaseStemElongation = " << PhaseStemElongation << " - line " << __LINE__ << std::endl;
    //std::cout << "HaunGain = " << HaunGain << " - line " << __LINE__ << std::endl;
    //std::cout << "HaunIndex = " << HaunIndex << " - line " << __LINE__ << std::endl;

}

void SamaraModel::evalCstrAssim()
{
    CstrAssim = std::pow(std::max(Cstr, 0.00000001), ASScstr);

    //std::cout << "cstr = " << Cstr << " - line " << __LINE__ << std::endl;
    //std::cout << "ASScstr = " << ASScstr << " - line " << __LINE__ << std::endl;
    //std::cout << "CstrAssim = " << CstrAssim << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalRespMaint()
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

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "CoefficientQ10 = " << CoefficientQ10 << " - line " << __LINE__ << std::endl;
    //std::cout << "TMoyCalc = " << TMoyCalc << " - line " << __LINE__ << std::endl;
    //std::cout << "KTempMaint = " << KTempMaint << " - line " << __LINE__ << std::endl;
    //std::cout << "CoeffQ10 = " << CoeffQ10 << " - line " << __LINE__ << std::endl;
    //std::cout << "KRespMaintLeaf = " << KRespMaintLeaf << " - line " << __LINE__ << std::endl;
    //std::cout << "KRespMaintSheath = " << KRespMaintSheath << " - line " << __LINE__ << std::endl;
    //std::cout << "KRespMaintRoot = " << KRespMaintRoot << " - line " << __LINE__ << std::endl;
    //std::cout << "KRespInternode = " << KRespInternode << " - line " << __LINE__ << std::endl;
    //std::cout << "KRespPanicle = " << KRespPanicle << " - line " << __LINE__ << std::endl;
    //std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << std:: endl;
    //std::cout << "DryMatStructSheathPop = " << DryMatStructSheathPop << std:: endl;
    //std::cout << "DryMatStructRootPop = " << DryMatStructRootPop << std:: endl;
    //std::cout << "DryMatStructInternodePop = " << DryMatStructInternodePop << std:: endl;
    //std::cout << "DryMatStructPaniclePop = " << DryMatStructPaniclePop << std:: endl;
    //std::cout << "RespMaintTot = " << RespMaintTot << " - line " << __LINE__ << std::endl;
}

void SamaraModel2_2::evalRespMaint()
{
    double RespMaintLeafPop;
    double RespMaintSheathPop;
    double RespMaintRootPop;
    double RespMaintInternodePop;
    double RespMaintPaniclePop;
    double CoeffQ10;
    double correction;

    CoeffQ10 = std::pow(CoefficientQ10, (TMoyCalc - KTempMaint) / 10);
    correction = (0.3 + 0.7 * std::min(PAR, 5.) / 5);

    RespMaintLeafPop = KRespMaintLeaf * DryMatStructLeafPop *
            CoeffQ10 * correction;
    RespMaintSheathPop = KRespMaintSheath * DryMatStructSheathPop *
            CoeffQ10 * correction;
    RespMaintRootPop = KRespMaintRoot * DryMatStructRootPop *
            CoeffQ10 * correction;
    RespMaintInternodePop = KRespInternode * DryMatStructInternodePop *
            CoeffQ10 * correction;
    RespMaintPaniclePop = KRespPanicle * DryMatStructPaniclePop *
            CoeffQ10 * correction;
    RespMaintTot = RespMaintLeafPop + RespMaintSheathPop + RespMaintRootPop +
            RespMaintInternodePop + RespMaintPaniclePop;
}

void SamaraModel2::evolPlantTilNumTot()
{
    double TilNewPlant;

    //std::cout << "avt chgt" << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    ////     //std::cout << "ChangePhase = " << ChangePhase << " - line " << __LINE__ << std::endl;
    ////     //std::cout << "Density = " << Density << " - line " << __LINE__ << std::endl;
    //std::cout << "PlantsPerHill = " << PlantsPerHill << " - line " << __LINE__ << std::endl;
    ////     //std::cout << "HaunIndex = " << HaunIndex << " - line " << __LINE__ << std::endl;
    ////     //std::cout << "HaunCritTillering = " << HaunCritTillering << " - line " << __LINE__ << std::endl;
    //std::cout << "Ic = " << Ic << " - line " << __LINE__ << std::endl;
    //std::cout << "IcTillering = " << IcTillering << " - line " << __LINE__ << std::endl;
    //std::cout << "TilAbility = " << TilAbility << " - line " << __LINE__ << std::endl;
    //std::cout << "LTRkdfcl = " << LTRkdfcl << " - line " << __LINE__ << std::endl;
    //std::cout << "TilNewPlant = " << TilNewPlant << " - line " << __LINE__ << std::endl;
    //std::cout << "CulmsPerPlant = " << CulmsPerPlant << " - line " << __LINE__ << std::endl;
    //std::cout << "CulmsPerHill  = " << CulmsPerHill << " - line " << __LINE__ << std::endl;

    if (NumPhase == 1 && ChangePhase == 1) {
        CulmsPerHill = PlantsPerHill;
    } else {
        if (NumPhase == 2 && ChangePhase == 1) {
            CulmsPerPlant = 1;
            CulmsPop = CulmsPerPlant * Density * PlantsPerHill;
        } else {
            if (NumPhase > 1 && NumPhase < 4 &&
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
    //std::cout << "aprs chgt" << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    ////     //std::cout << "ChangePhase = " << ChangePhase << " - line " << __LINE__ << std::endl;
    ////     //std::cout << "Density = " << Density << " - line " << __LINE__ << std::endl;
    //std::cout << "PlantsPerHill = " << PlantsPerHill << " - line " << __LINE__ << std::endl;
    ////     //std::cout << "HaunIndex = " << HaunIndex << " - line " << __LINE__ << std::endl;
    ////     //std::cout << "HaunCritTillering = " << HaunCritTillering << " - line " << __LINE__ << std::endl;
    //std::cout << "Ic = " << Ic << " - line " << __LINE__ << std::endl;
    //std::cout << "IcTillering = " << IcTillering << " - line " << __LINE__ << std::endl;
    //std::cout << "TilAbility = " << TilAbility << " - line " << __LINE__ << std::endl;
    //std::cout << "LTRkdfcl = " << LTRkdfcl << " - line " << __LINE__ << std::endl;
    //std::cout << "TilNewPlant = " << TilNewPlant << " - line " << __LINE__ << std::endl;
    //std::cout << "CulmsPerPlant = " << CulmsPerPlant << " - line " << __LINE__ << std::endl;
    //std::cout << "CulmsPerHill  = " << CulmsPerHill << " - line " << __LINE__ << std::endl;
    ////     //std::cout << "CulmsPop = " << CulmsPop << " - line " << __LINE__ << std::endl;


}

void SamaraModel2_2::evolPlantTilNumTot()
{

    double TilNewPlant;

    if (NumPhase == 1 && ChangePhase == 1) {
        CulmsPerHill = PlantsPerHill;
    } else {
        if (NumPhase == 2 && ChangePhase == 1) {
            CulmsPerPlant = 1;
            CulmsPop = CulmsPerPlant * Density * PlantsPerHill;
            CulmsPerHill = CulmsPerPlant * PlantsPerHill;
        } else {
            if (NumPhase > 1 && NumPhase < 4 && HaunIndex > HaunCritTillering) {
                TilNewPlant = Cstr * std::min(std::max(0., (Ic - IcTillering) * TilAbility) *
                                              LTRkdfcl, CulmsPerPlant * 0.1);
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
}

void SamaraModel::evolPlantLeafNumTot()
{
    //std::cout << "avt chg plntleafnumtot " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "HaunGain = " << HaunGain << " - line " << __LINE__ << std::endl;
    //std::cout << "CulmsPerHill = " << CulmsPerHill << " - line " << __LINE__ << std::endl;
    //std::cout << "PlantLeafNumNew = " << PlantLeafNumNew << " - line " << __LINE__ << std::endl;
    //std::cout << "PlantLeafNumTot = " << PlantLeafNumTot << " - line " << __LINE__ << std::endl;

    //    if (NumPhase == 0) {
    //        PlantLeafNumNew = 0.;            // avt instruction inclus ds le code
    //        PlantLeafNumTot = 0.;
    //    } else {
    if (NumPhase > 1 && NumPhase < 5) {
        PlantLeafNumNew = HaunGain * CulmsPerHill;
        PlantLeafNumTot = PlantLeafNumTot + PlantLeafNumNew;
    } else {
        PlantLeafNumNew = PlantLeafNumNew;
        PlantLeafNumTot = PlantLeafNumTot;
    }
    //   }
    //
    //std::cout << "aprs chg plntleafnumtot " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "HaunGain = " << HaunGain << " - line " << __LINE__ << std::endl;
    //std::cout << "CulmsPerHill = " << CulmsPerHill << " - line " << __LINE__ << std::endl;
    //std::cout << "PlantLeafNumNew = " << PlantLeafNumNew << " - line " << __LINE__ << std::endl;
    //std::cout << "PlantLeafNumTot = " << PlantLeafNumTot << " - line " << __LINE__ << std::endl;

}

void SamaraModel::evalRelPotLeafLength()
{
    if (NumPhase > 1) {
        RelPotLeafLength = std::min((0.1 + 0.9 * HaunIndex / RankLongestLeaf),
                                    1.);
    }

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "HaunIndex = " << HaunIndex << " - line " << __LINE__ << std::endl;
    //std::cout << "RankLongestLeaf = " << RankLongestLeaf << " - line " << __LINE__ << std::endl;
    //std::cout << "RelPotLeafLength = " << RelPotLeafLength << " - line " << __LINE__ << std::endl;

}

void SamaraModel2_2::evalRelPotLeafLength()
{
    if (NumPhase > 1) {
        RelPotLeafLength = std::min((0.10 + 0.90 * HaunIndex / RankLongestLeaf),
                                    1.);
    }
}


void SamaraModel::evalDemandStructSheath()
{
    //std::cout << "avt chg dem struct sheath = " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructSheathPop = " << DemStructSheathPop << " - line " << __LINE__ << std::endl;
    if (NumPhase > 1 && NumPhase < 5) {
        DemStructSheathPop = (1 + ((SlaMax - Sla) / (SlaMax - SlaMin))) * 0.5 *
                DemStructLeafPop / WtRatioLeafSheath *
                std::max(0.00001, StressCold);

        //std::cout << "aprs chg dem struct sheath = " << " - line " << __LINE__ << std::endl;
        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "SlaMax = " << SlaMax << " - line " << __LINE__ << std::endl;
        //std::cout << "Sla = " << Sla << " - line " << __LINE__ << std::endl;
        //std::cout << "SlaMin = " << SlaMin << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructLeafPop = " << DemStructLeafPop << " - line " << __LINE__ << std::endl;
        //std::cout << "WtRatioLeafSheath = " << WtRatioLeafSheath << " - line " << __LINE__ << std::endl;
        //std::cout << "StressCold = " << StressCold << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructSheathPop = " << DemStructSheathPop << " - line " << __LINE__ << std::endl;

    }
}

void SamaraModel2::evalDemandStructRoot()
{

    //std::cout << "avt chg dem struct root = " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "RootFront = " << RootFront << " - line " << __LINE__ << std::endl;
    //std::cout << "Density = " << Density << " - line " << __LINE__ << std::endl;
    //std::cout << "RootSystSoilSurfPop = " << RootSystSoilSurfPop << " - line " << __LINE__ << std::endl;
    //std::cout << "RootSystVolPop = " << RootSystVolPop << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructLeafPop = " << DemStructLeafPop << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructSheathPop = " << DemStructSheathPop << " - line " << __LINE__ << std::endl;
    //std::cout << "RootPartitMax = " << RootPartitMax << " - line " << __LINE__ << std::endl;
    //std::cout << "CoeffRootMassPerVolMax = " << CoeffRootMassPerVolMax << " - line " << __LINE__ << std::endl;
    //std::cout << "DryMatStructRootPop = " << DryMatStructRootPop << " - line " << __LINE__ << std::endl;

    RootSystSoilSurfPop = std::min(RootFront * RootFront * Density / 1000000,
                                   10000.);
    RootSystVolPop = RootSystSoilSurfPop * RootFront / 1000;
    if (NumPhase > 1 && NumPhase < 5) {
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

        //std::cout << "aprs chg dem struct root = " << " - line " << __LINE__ << std::endl;
        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "RootFront = " << RootFront << " - line " << __LINE__ << std::endl;
        //std::cout << "Density = " << Density << " - line " << __LINE__ << std::endl;
        //std::cout << "RootSystSoilSurfPop = " << RootSystSoilSurfPop << " - line " << __LINE__ << std::endl;
        //std::cout << "RootSystVolPop = " << RootSystVolPop << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructLeafPop = " << DemStructLeafPop << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructSheathPop = " << DemStructSheathPop << " - line " << __LINE__ << std::endl;
        //std::cout << "RootPartitMax = " << RootPartitMax << " - line " << __LINE__ << std::endl;
        //std::cout << "CoeffRootMassPerVolMax = " << CoeffRootMassPerVolMax << " - line " << __LINE__ << std::endl;
        //std::cout << "DryMatStructRootPop = " << DryMatStructRootPop << " - line " << __LINE__ << std::endl;
    }
}

void SamaraModel2::evalDemandStructPanicle()
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

void SamaraModel::evolGrowthStructSheathPop()
{
    if (NumPhase > 1 && NumPhase < 5) {
        if (Ic < 1 && DemStructTotPop != 0) { //  avt instruction non dans le code ajout de la condit. demstructtotpop != 0
            GrowthStructSheathPop = SupplyTot * (DemStructSheathPop /
                                                 DemStructTotPop);
        } else  {
            GrowthStructSheathPop = DemStructSheathPop;
        }
    }
}

void SamaraModel::evolGrowthStructRootPop()
{
    if (NumPhase > 1 && NumPhase < 5) {
        if (Ic < 1 && DemStructTotPop != 0) {
            GrowthStructRootPop = SupplyTot * (DemStructRootPop /
                                               DemStructTotPop);
        } else {
            GrowthStructRootPop = DemStructRootPop;
        }
    }
}

void SamaraModel::evolGrowthStructPanPop()
{
    if (NumPhase > 1 && NumPhase < 5) {
        if (Ic < 1 && DemStructTotPop != 0) {  //  avt instruction non dans le code ajout de la condit. demstructtotpop != 0
            GrowthStructPaniclePop = SupplyTot * (DemStructPaniclePop /
                                                  DemStructTotPop);
        } else {
            GrowthStructPaniclePop = DemStructPaniclePop;
        }
    }
}

void SamaraModel::evalSlaMitch()
{
    //std::cout << "avt chg eval sla = " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "SlaMin = " << SlaMin << " - line " << __LINE__ << std::endl;
    //std::cout << "SlaMax = " << SlaMax << " - line " << __LINE__ << std::endl;
    //std::cout << "SlaMitch = " << SlaMitch << " - line " << __LINE__ << std::endl;
    //std::cout << "SlaNew = " << SlaNew << " - line " << __LINE__ << std::endl;
    //std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << " - line " << __LINE__ << std::endl;
    //std::cout << "GrowthStructLeafPop = " << GrowthStructLeafPop << " - line " << __LINE__ << std::endl;
    //std::cout << "Sla = " << Sla << " - line " << __LINE__ << std::endl;
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
    //std::cout << "aprs chg eval sla = " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "SlaMin = " << SlaMin << " - line " << __LINE__ << std::endl;
    //std::cout << "SlaMax = " << SlaMax << " - line " << __LINE__ << std::endl;
    //std::cout << "SlaMitch = " << SlaMitch << " - line " << __LINE__ << std::endl;
    //std::cout << "SlaNew = " << SlaNew << " - line " << __LINE__ << std::endl;
    //std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << " - line " << __LINE__ << std::endl;
    //std::cout << "GrowthStructLeafPop = " << GrowthStructLeafPop << " - line " << __LINE__ << std::endl;
    //std::cout << "Sla = " << Sla << " - line " << __LINE__ << std::endl;
}

void SamaraModel2_2::evalSlaMitch()
{
    if (NumPhase > 1) {
        SlaMitch = SlaMin + (SlaMax - SlaMin) * std::pow(AttenMitch, (SommeDegresJour -
                                                                      SDJLevee));
        SlaNew = SlaMin + (SlaMitch - SlaMin) * std::pow(DegresDuJour / (TOpt1 -
                                                                         TBase), TempSla);
        SlaNew = SlaNew + (SlaNew * 0.8 * (1 - std::min(PAR / 6, 1.)));
        Sla = ((Sla * DryMatStructLeafPop) + (SlaNew * GrowthStructLeafPop)) /
                (DryMatStructLeafPop + GrowthStructLeafPop);
    } else {
        SlaMitch = 0;
        SlaNew = 0;
        Sla = SlaMax;
    }
}

void SamaraModel::evolKcpKceBilhy()
{
    Kcp = std::min((1 - LTRkdfcl) * KcMax, KcMax);
    Kcp = std::min(Kcp, KcMax);
    Kce = LTRkdfcl * 1 * (Mulch / 100);
    KcTot = Kcp + Kce;

    //std::cout << "LTRkdfcl = " << LTRkdfcl << " - line " << __LINE__ << std::endl;
    //std::cout << "KcMax = " << KcMax << " - line " << __LINE__ << std::endl;
    //std::cout << "Mulch = " << Mulch << " - line " << __LINE__ << std::endl;
    //std::cout << "Kcp = " << Kcp << " - line " << __LINE__ << std::endl;
    //std::cout << "Kce = " << Kce << " - line " << __LINE__ << std::endl;
    //std::cout << "KcTot = " << KcTot << " - line " << __LINE__ << std::endl;

}

void SamaraModel2::evolConsRes_Flood()
{
    double TrSurf;
    double WaterDeficit;

    TrSurf = 0;
    StockSurface = ValRFE + ValRDE;
    if (FloodwaterDepth + StockMacropores == 0 || NumPhase == 0) {

        //std::cout << "On passe par la " << " - line " << __LINE__ << std::endl;
        //std::cout << "RuRac = " << RuRac << " - line " << __LINE__ << std::endl;
        //std::cout << "StockRac = " << StockRac << " - line " << __LINE__ << std::endl;



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

        //std::cout << "TrSurf = " << TrSurf << " - line " << __LINE__ << std::endl;


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

        //std::cout << "evolConsRes_Flood()" << " - line " << __LINE__ << std::endl;
        //std::cout << "StockTotal = " << StockTotal << " - line " << __LINE__ << std::endl;

        StockRac = std::min(StockRac, StockTotal);
    }
    if (StockMacropores + FloodwaterDepth > 0 &&
            (StockMacropores + FloodwaterDepth <= Tr + Evap) && NumPhase > 0) {
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
        if (StockMacropores + FloodwaterDepth > Tr + Evap && NumPhase > 0) {
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

void SamaraModel::evalConversion()
{
    double KAssim;

    switch ((int)(NumPhase))
    {
    case 2: KAssim = 1; break;
    case 3: KAssim = TxAssimBVP; break;  //avt non present
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

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "TxAssimBVP = " << TxAssimBVP << " - line " << __LINE__ << std::endl;
    //std::cout << "SommeDegresJour = " << SommeDegresJour << " - line " << __LINE__ << std::endl;
    //std::cout << "SommeDegresJourPhasePrecedente = " << SommeDegresJourPhasePrecedente << " - line " << __LINE__ << std::endl;
    //std::cout << "TxAssimMatu1 = " << TxAssimMatu1 << " - line " << __LINE__ << std::endl;
    //std::cout << "TxAssimMatu2 = " << TxAssimMatu2 << " - line " << __LINE__ << std::endl;
    //std::cout << "KAssim = " << KAssim << " - line " << __LINE__ << std::endl;
    //std::cout << "TxConversion = " << TxConversion << " - line " << __LINE__ << std::endl;
    //std::cout << "Conversion = " << Conversion << " - line " << __LINE__ << std::endl;

}

void SamaraModel::evolPSPMVMD(double t)
{
    if (t >= DateSemis) {

        /*//std::cout << "-----" << " - line " << __LINE__ << std::endl;
        //std::cout << "AVANT" << " - line " << __LINE__ << std::endl;
        //std::cout << "-----" << " - line " << __LINE__ << std::endl;
        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "ChangePhase = " << ChangePhase << " - line " << __LINE__ << std::endl;
        //std::cout << "DegresDuJourCor = " << DegresDuJourCor << " - line " << __LINE__ << std::endl;
        //std::cout << "DegresDuJour = " << DegresDuJour << " - line " << __LINE__ << std::endl;
        //std::cout << "DureeDuJour = " << DayLength << " - line " << __LINE__ << std::endl;
        //std::cout << "SommeDegresJourCor = " << SommeDegresJourCor << " - line " << __LINE__ << std::endl;
        //std::cout << "PPExp = " << PPExp << " - line " << __LINE__ << std::endl;
        //std::cout << "SeuilPP = " << SeuilPP << " - line " << __LINE__ << std::endl;
        //std::cout << "PPCrit = " << PPCrit << " - line " << __LINE__ << std::endl;
        //std::cout << "SommeDegresJourPhasePrecedente = " << SommeDegresJourPhasePrecedente << " - line " << __LINE__ << std::endl;*/




        double SDJPSP;

        if (NumPhase == 3) {
            if (ChangePhase == 1) {
                SumPP = 100;
                SDJPSP = std::max(0.01, DegresDuJourCor);
                /*//std::cout << "Ligne 1" << " - line " << __LINE__ << std::endl;
                //std::cout << "SumPP = " << SumPP << " - line " << __LINE__ << std::endl;
                //std::cout << "SDJPSP = " << SDJPSP << " - line " << __LINE__ << std::endl;*/
            } else {
                SDJPSP = SommeDegresJourCor - SommeDegresJourPhasePrecedente +
                        std::max(0.01, DegresDuJourCor);
                /*//std::cout << "Ligne 2" << " - line " << __LINE__ << std::endl;
                  //std::cout << "SDJPSP = " << SDJPSP << " - line " << __LINE__ << std::endl;*/
            }

            /*//std::cout << "std::pow((1000 / SDJPSP), PPExp) = " << std::pow((1000 / SDJPSP), PPExp) << " - line " << __LINE__ << std::endl;
            //std::cout << "DureeDuJour - PPCrit =" << DayLength - PPCrit << " - line " << __LINE__ << std::endl;
            //std::cout <<  "std::max(0., (DureeDuJour - PPCrit)) = " << std::max(0., (DureeDuJour - PPCrit)) << " - line " << __LINE__ << std::endl;*/



            SumPP = std::pow((1000 / SDJPSP), PPExp) *
                    std::max(0., (DayLength - PPCrit)) / (SeuilPP - PPCrit);
            SeuilTempPhaseSuivante = SommeDegresJourPhasePrecedente + SDJPSP;
        }
        //
        //std::cout << "-----" << " - line " << __LINE__ << std::endl;
        //std::cout << "APRES" << " - line " << __LINE__ << std::endl;
        //std::cout << "-----" << " - line " << __LINE__ << std::endl;
        //std::cout << "SDJPSP = " << SDJPSP << " - line " << __LINE__ << std::endl;
        //std::cout << "SumPP = " << SumPP << " - line " << __LINE__ << std::endl;
        //std::cout << "SeuilTempPhaseSuivante = " << SeuilTempPhaseSuivante << " - line " << __LINE__ << std::endl;



    }
}


void SamaraModel::evolSomDegresJourCor()
{
    if (NumPhase >= 1) {
        SommeDegresJourCor = SommeDegresJourCor + DegresDuJourCor;
    } else {
        SommeDegresJourCor = 0;
    }

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "DegresDuJourCor = " << DegresDuJourCor << " - line " << __LINE__ << std::endl;
    //std::cout << "SommeDegresJourCor = " << SommeDegresJourCor << " - line " << __LINE__ << std::endl;

}


void SamaraModel::evalMaximumLai()
{
    if (Lai > TempLai) {
        TempLai = Lai;
    }
    if (NumPhase != 7) {
        MaximumLai = 0;
    } else if (NumPhase == 7 && ChangePhase == 1) {
        MaximumLai = TempLai;
    }
}

void SamaraModel::evalVitesseRacinaire()
{

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;


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

    //std::cout << "VitesseRacinaire = " << VitesseRacinaire << " - line " << __LINE__ << std::endl;
    //std::cout << "DegresDuJourCor = " << DegresDuJourCor << " - line " << __LINE__ << std::endl;
    //std::cout << "RootCstr = " << RootCstr << " - line " << __LINE__ << std::endl;
    //std::cout << "VitesseRacinaireDay = " << VitesseRacinaireDay << " - line " << __LINE__ << std::endl;


}
void SamaraModel::evalRootFront()
{
    //std::cout << "avt chg rootfront = " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "ResUtil = " << ResUtil << " - line " << __LINE__ << std::endl;
    //std::cout << "RuRac = " << RuRac << " - line " << __LINE__ << std::endl;
    //std::cout << "RootFront = " << RootFront << " - line " << __LINE__ << std::endl;
    if (NumPhase > 0) {


        RootFront = ((1 / ResUtil) * RuRac) * 1000;

        //std::cout << "aprs chg rootfront = " << " - line " << __LINE__ << std::endl;
        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "ResUtil = " << ResUtil << " - line " << __LINE__ << std::endl;
        //std::cout << "RuRac = " << RuRac << " - line " << __LINE__ << std::endl;
        //std::cout << "RootFront = " << RootFront << " - line " << __LINE__ << std::endl;

    }
}

void SamaraModel::evalSDJPhase4()
{

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;

    if (NumPhase == 4) {
        SDJCorPhase4 = SDJCorPhase4 + DegresDuJourCor;
    }

    //std::cout << "SDJCorPhase4 = " << SDJCorPhase4 << " - line " << __LINE__ << std::endl;

}

void SamaraModel::evalDateGermination()
{
    if (NumPhase < 1 || (NumPhase == 1 && ChangePhase == 1)) {
        NbDaysSinceGermination = 0;
    } else {
        NbDaysSinceGermination = NbDaysSinceGermination + 1;
    }

    //std::cout << "NbDaysSinceGermination = " << NbDaysSinceGermination << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalSterility()
{
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "ChangePhase = " << ChangePhase << " - line " << __LINE__ << std::endl;

    if (NumPhase == 5 && ChangePhase == 1) {
        SterilityCold =
                std::max(0.,
                         (std::min(1.,
                                   KCritSterCold1 / (KCritSterCold1 -
                                                     KCritSterCold2) - TMinMoy /
                                   (KCritSterCold1 - KCritSterCold2))));

        //std::cout << "SterilityCold = " << SterilityCold << " - line " << __LINE__ << std::endl;

        SterilityHeat =
                1. - std::max(0.,
                              (std::min(1., KCritSterHeat2 /
                                        (KCritSterHeat2 -
                                         KCritSterHeat1) - TMaxMoy /
                                        (KCritSterHeat2 - KCritSterHeat1))));

        //std::cout << "SterilityHeat = " << SterilityHeat << " - line " << __LINE__ << std::endl;

        SterilityDrought =
                std::max(0.,
                         (std::min(1., KCritSterFtsw1 / (KCritSterFtsw1 -
                                                         KCritSterFtsw2) -
                                   FtswMoy / (KCritSterFtsw1 - KCritSterFtsw2))));

        //std::cout << "SterilityDrought = " << SterilityDrought << " - line " << __LINE__ << std::endl;
        //std::cout << "FtswMoy = " << FtswMoy << " - line " << __LINE__ << std::endl;

    } else {
        SterilityCold = SterilityCold;
        SterilityHeat = SterilityHeat;
        SterilityDrought = SterilityDrought;
    }
    SterilityTot = std::min(0.999, 1 - ((1 - SterilityCold) *
                                        (1 - SterilityHeat) *
                                        (1 - SterilityDrought)));

    //std::cout << "SterilityTot = " << SterilityTot << " - line " << __LINE__ << std::endl;

}

void SamaraModel2::initParcelle()
{
    double Stockini2;
    double Stockini1;


    //std::cout << "HumSat = " << HumSat << " - line " << __LINE__ << std::endl;
    //std::cout << "HumFC = " << HumFC << " - line " << __LINE__ << std::endl;
    //std::cout << "EpaisseurSurf = " << EpaisseurSurf << " - line " << __LINE__ << std::endl;
    //std::cout << "EpaisseurProf = " << EpaisseurProf << " - line " << __LINE__ << std::endl;
    //std::cout << "PEvap = " << PEvap << " - line " << __LINE__ << std::endl;
    //std::cout << "StockIniSurf = " << StockIniSurf << " - line " << __LINE__ << std::endl;
    //std::cout << "StockIniProf = " << StockIniProf << " - line " << __LINE__ << std::endl;
    //std::cout << "avt init parc " << " - line " << __LINE__ << std::endl;
    //std::cout << "Numphase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "ResUtil = " << ResUtil << " - line " << __LINE__ << std::endl;
    //std::cout << "HumSat = " << HumSat << " - line " << __LINE__ << std::endl;
    //std::cout << "HumPF = " << HumPF << " - line " << __LINE__ << std::endl;
    //std::cout << "HumFC = " << HumFC << " - line " << __LINE__ << std::endl;
    //std::cout << "EpaisseurSurf = " << EpaisseurSurf << " - line " << __LINE__ << std::endl;
    //std::cout << "EpaisseurProf = " << EpaisseurProf << " - line " << __LINE__ << std::endl;
    //std::cout << "ProfRu = " << ProfRu << " - line " << __LINE__ << std::endl;
    //std::cout << "StockSurface = " << StockSurface << " - line " << __LINE__ << std::endl;
    //std::cout << "StockTotal = " << StockTotal << " - line " << __LINE__ << std::endl;

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

    //std::cout << "aprs init parc " << " - line " << __LINE__ << std::endl;
    //std::cout << "Numphase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "ResUtil = " << ResUtil << " - line " << __LINE__ << std::endl;
    //std::cout << "HumSat = " << HumSat << " - line " << __LINE__ << std::endl;
    //std::cout << "HumPF = " << HumPF << " - line " << __LINE__ << std::endl;
    //std::cout << "HumFC = " << HumFC << " - line " << __LINE__ << std::endl;
    //std::cout << "EpaisseurSurf = " << EpaisseurSurf << " - line " << __LINE__ << std::endl;
    //std::cout << "EpaisseurProf = " << EpaisseurProf << " - line " << __LINE__ << std::endl;
    //std::cout << "ProfRu = " << ProfRu << " - line " << __LINE__ << std::endl;
    //std::cout << "StockSurface = " << StockSurface << " - line " << __LINE__ << std::endl;
    //std::cout << "StockTotal = " << StockTotal << " - line " << __LINE__ << std::endl;
}

void SamaraModel2_2::initParcelle()
{
    double Stockini2;
    double Stockini1;

    //std::cout << "HumSat = " << HumSat << " - line " << __LINE__ << std::endl;
    //std::cout << "HumFC = " << HumFC << " - line " << __LINE__ << std::endl;
    //std::cout << "EpaisseurSurf = " << EpaisseurSurf << " - line " << __LINE__ << std::endl;
    //std::cout << "EpaisseurProf = " << EpaisseurProf << " - line " << __LINE__ << std::endl;
    //std::cout << "PEvap = " << PEvap << " - line " << __LINE__ << std::endl;
    //std::cout << "StockIniSurf = " << StockIniSurf << " - line " << __LINE__ << std::endl;
    //std::cout << "StockIniProf = " << StockIniProf << " - line " << __LINE__ << std::endl;

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

void SamaraModel::evalEvapPot()
{
    EvapPot = Kce * Eto ; // ETP ds samara delphi

    //std::cout << "Kce = " << Kce << " - line " << __LINE__ << std::endl;
    //std::cout << "Eto = " << Eto << " - line " << __LINE__ << std::endl;
    //std::cout << "EvapPot = " << EvapPot << " - line " << __LINE__ << std::endl;
}

void SamaraModel2::transplanting()
{
    double DensityChange;
    DensityChange = DensityField / (DensityNursery / PlantsPerHill);

    if (Transplanting == 1 && NumPhase >= 1) {
        CounterNursery = CounterNursery + 1;
    }
    if (Transplanting == 1 && CounterNursery < DurationNursery &&
            ChangeNurseryStatus == 0) {
        NurseryStatus = 0;
        ChangeNurseryStatus = 0;
    } else {
        if (Transplanting == 1 && CounterNursery >= DurationNursery &&
                ChangeNurseryStatus == 0 && NurseryStatus == 0) {
            NurseryStatus = 1;
            ChangeNurseryStatus = 1;
        } else {
            NurseryStatus = 1;
            ChangeNurseryStatus = 0;
        }
    }

    //std::cout << "ChangeNurseryStatus =" << ChangeNurseryStatus << " - line " << __LINE__ << std::endl;

    if (NurseryStatus == 1) {
        Density = DensityField;
    } else {
        Density = DensityNursery / PlantsPerHill;
    }

    //std::cout << "DryMatStructLeafPop avt modif =" << DryMatStructLeafPop << " - line " << __LINE__ << std::endl;

    if (ChangeNurseryStatus == 1) {
        DryMatStructLeafPop = DryMatStructLeafPop * DensityChange;
        DryMatStructSheathPop = DryMatStructSheathPop * DensityChange;
        DryMatStructRootPop = DryMatStructRootPop * DensityChange;
        DryMatStructInternodePop = DryMatStructInternodePop * DensityChange;
        DryMatStructPaniclePop = DryMatStructPaniclePop * DensityChange;
        DryMatResInternodePop = DryMatResInternodePop * DensityChange;
    }
    //   //std::cout << "DensityChange =" << DensityChange << " - line " << __LINE__ << std::endl;
    //std::cout << "DryMatStructLeafPop aprs modif =" << DryMatStructLeafPop << " - line " << __LINE__ << std::endl;
}

void SamaraModel2_2::transplanting()
{

    double DensityChange;

    DensityChange = DensityField / (DensityNursery / PlantsPerHill);
    if (Transplanting == 1 && NumPhase >= 1) {
        CounterNursery = CounterNursery + 1;
    }
    if (Transplanting == 1 && CounterNursery < DurationNursery &&
            ChangeNurseryStatus == 0) {
        NurseryStatus = 0;
        ChangeNurseryStatus = 0;
    } else {
        if (Transplanting == 1 && CounterNursery >= DurationNursery &&
                ChangeNurseryStatus == 0 && NurseryStatus == 0) {
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
        DeadLeafDrywtPop = DeadLeafDrywtPop * DensityChange;
        ResCapacityInternodePop = ResCapacityInternodePop * DensityChange;
    }
}

void SamaraModel2::transplantingShock()
{
    if (CounterNursery > 0 && CounterNursery < 8) {
        Assim = Assim * CoeffTransplantingShock;
    } else {
        Assim = Assim;
    }

    //std::cout << "Assim = " << Assim << " - line " << __LINE__ << std::endl;

}

void SamaraModel2::evalRuiss_FloodDyna()
{
    double CorrectedIrrigation;
    double CorrectedBundHeight;

    Lr = 0;
    CorrectedBundHeight = BundHeight;
    // implement lifesaving drainage
    if (LifeSavingDrainage == 1 && FloodwaterDepth > 0.5 * PlantHeight &&
            PlantHeight > 0 && NumPhase > 1 && BundHeight > 0) {
        CorrectedBundHeight = 0.5 * PlantHeight;
        Lr = Lr + std::max(0., FloodwaterDepth - (0.5 * PlantHeight));
        FloodwaterDepth = std::min(FloodwaterDepth, (0.5 * PlantHeight));
        if (FloodwaterDepth + StockMacropores > 0) {
            EauDispo = FloodwaterDepth + StockMacropores;
        }
    }
    // implement terminal drainage
    if (NumPhase > 4 && NumPhase < 7 && DAF > PlotDrainageDAF &&
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
    if (CorrectedBundHeight == 0 && BundHeight != CorrectedBundHeight) {
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

void SamaraModel2::evolRempliResRFE_RDE()
{
    double EauReste;
    double ValRSurfPrec;
    double EauTranspi;

    Dr = 0;
    EauTranspi = 0;
    if (StockMacropores + FloodwaterDepth == 0) {
        EauReste = 0;
        ValRFE = ValRFE + EauDispo;

        //std::cout << "ValRFE = " << ValRFE << " - line " << __LINE__ << std::endl;
        //std::cout << "EauDispo = " << EauDispo << " - line " << __LINE__ << std::endl;
        //std::cout << "CapaRFE = " << CapaRFE << " - line " << __LINE__ << std::endl;

        if (ValRFE > CapaRFE) {
            EauReste = ValRFE - CapaRFE;
            ValRFE = CapaRFE;
        }
        ValRSurfPrec = ValRSurf;
        ValRSurf = ValRSurf + EauReste;

        //std::cout << "ValRSurf = " << ValRSurf << " - line " << __LINE__ << std::endl;
        //std::cout << "ValRSurfPrec = " << ValRSurfPrec << " - line " << __LINE__ << std::endl;
        //std::cout << "CapaREvap = " << CapaREvap << " - line " << __LINE__ << std::endl;

        if (ValRSurfPrec < CapaREvap) {
            EauTranspi = EauDispo - (std::min(CapaREvap, ValRSurf) -
                                     ValRSurfPrec);
        } else {
            EauTranspi = EauDispo;
        }

        //std::cout << "EauTranspi = " << EauTranspi << " - line " << __LINE__ << std::endl;

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

        //std::cout << "StockSurface = " << StockSurface << " - line " << __LINE__ << std::endl;

        StockTotal = StockTotal + EauTranspi;

        //std::cout << "evolRempliResRFE_RDE()" << " - line " << __LINE__ << std::endl;
        //std::cout << "StockTotal = " << StockTotal << " - line " << __LINE__ << std::endl;


        //std::cout << "StockTotal = " << StockTotal << " - line " << __LINE__ << std::endl;
        //std::cout << "StRuMax = " << StRuMax << " - line " << __LINE__ << std::endl;

        if (StockTotal > StRuMax) {
            Dr = StockTotal - StRuMax;
            StockTotal = StRuMax;

            //std::cout << "evolRempliResRFE_RDE()" << " - line " << __LINE__ << std::endl;
            //std::cout << "StockTotal = " << StockTotal << " - line " << __LINE__ << std::endl;


        } else {
            Dr = 0;
        }

        //std::cout << "Dr = " << Dr << " - line " << __LINE__ << std::endl;

        if (Hum < CapaRFE + CapaRDE) {
            Hum = StockSurface;
        } else {
            Hum = std::max(Hum, StockTotal);
        }

        //std::cout << "Hum = " << Hum << " - line " << __LINE__ << std::endl;

    }
    StockRac = std::min(StockRac + EauTranspi, RuRac);

    //std::cout << "StockRac = " << StockRac << " - line " << __LINE__ << std::endl;

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
            if (FloodwaterDepth < PercolationMax &&
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

void SamaraModel2::plantSubmergence()
{
    FractionPlantHeightSubmer = std::min(
                std::max(0., FloodwaterDepth / std::max(PlantHeight, 0.1)), 1.);
}

void SamaraModel2::excessAssimilToRoot()
{
    if (NumPhase > 1) {
        RootMassPerVol = DryMatStructRootPop / RootSystVolPop;
    }
    if (ExcessAssimToRoot == 1) {
        if (NumPhase < 5 && NumPhase > 1 && AssimNotUsed > 0) {
            if (RootMassPerVol < CoeffRootMassPerVolMax) {
                GrowthStructRootPop = GrowthStructRootPop + AssimNotUsed;
                AssimNotUsed = 0;
            }
        }
    }
}

void SamaraModel2::evolRempliMacropores()
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

void SamaraModel2::evalFTSW()
{
    //std::cout << "StockRac (avant) = " << StockRac << " - line " << __LINE__ << std::endl;

    StockRac = std::min(StockRac, (RuRac +
                                   (StockMacropores * RuRac / StRuMax)));
    StockRac = std::min(StockRac, StockTotal);
    if (RuRac > 0) {
        FTSW = StockRac / RuRac;
    } else {
        FTSW = 0;
    }

    //std::cout << "StockRac = " << StockRac << " - line " << __LINE__ << std::endl;
    //std::cout << "RuRac = " << RuRac << " - line " << __LINE__ << std::endl;
    //std::cout << "StockMacropores = " << StockMacropores << " - line " << __LINE__ << std::endl;
    //std::cout << "StRuMax = " << StRuMax << " - line " << __LINE__ << std::endl;
    //std::cout << "FTSW = " << FTSW << " - line " << __LINE__ << std::endl;
}

void SamaraModel2::evalDAF()
{
    if (NumPhase > 4) {
        DAF = DAF + 1;
    } else {
        DAF = DAF;
    }
}

void SamaraModel::evalSimStartGermin()
{
    if (NumPhase == 1 && ChangePhase == 1) {
        SimStartGermin = NbJas;
    }
}

void SamaraModel::evalSimEmergence()
{
    if (NumPhase == 2 && ChangePhase == 1) {
        SimEmergence = NbJas;
    }
}

void SamaraModel::evalSimStartPSP()
{
    if (NumPhase == 3 && ChangePhase == 1) {
        SimStartPSP = NbJas;
    }
}

void SamaraModel::evalSimPanIni()
{
    if (NumPhase == 4 && ChangePhase == 1) {
        SimPanIni = NbJas;
    }
}

void SamaraModel::evalSimAnthesis50()
{
    if (NumPhase == 5 && ChangePhase == 1) {
        SimAnthesis50 = NbJas;
    }
}

void SamaraModel::evalSimStartMatu2()
{
    if (NumPhase == 6 && ChangePhase == 1) {
        SimStartMatu2 = NbJas;
    }
}

void SamaraModel::evalColdStress()
{
    StressCold = 1 -
            std::max(0.,
                     std::min(1.,
                              KCritStressCold1 / (KCritStressCold1 -
                                                  KCritStressCold2) - TMin /
                              (KCritStressCold1 - KCritStressCold2)));
    StressCold = std::max(0.00001, StressCold);
    //std::cout << "StressCold = " << StressCold << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalAssim()
{
    Assim = std::max(AssimPot * CstrAssim, 0.);

    //std::cout << "AssimPot = " << AssimPot << " - line " << __LINE__ << std::endl;
    //std::cout << "CstrAssim = " << CstrAssim << " - line " << __LINE__ << std::endl;
    //std::cout << "Assim = " << Assim << " - line " << __LINE__ << std::endl;

}

void SamaraModel2::evolWaterLoggingUpland()
{
    if (Dr > PercolationMax && BundHeight == 0) {
        StockMacropores = StockMacropores + (Dr - PercolationMax);
        Lr = Lr + std::max(0., StockMacropores - VolMacropores);
        Dr = PercolationMax;
        StockMacropores = std::min(StockMacropores, VolMacropores);
    }
}

void SamaraModel2::evalStressWaterLogging()
{
    if (StockMacropores > 0 && RootFront > 0) {
        FractionRootsLogged =
                (std::max(0., RootFront - ((VolMacropores - StockMacropores) /
                                           VolMacropores) * (EpaisseurSurf +
                                                             EpaisseurProf))) /
                RootFront;
        CoeffStressLogging = 1 - (FractionRootsLogged *
                                  std::min(1., WaterLoggingSens));
    }
}

void SamaraModel2::evalCstrPFactorFAO()
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

    //std::cout << "pFact = " << pFact << " - line " << __LINE__ << std::endl;
    //std::cout << "FTSW = " << FTSW << " - line " << __LINE__ << std::endl;
    //std::cout << "Cstr = " << Cstr << " - line " << __LINE__ << std::endl;
}

void SamaraModel2_1::evolHauteur_SDJ_cstr()
{
    double CorrectedCstrMean;

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "PhaseStemElongation = " << PhaseStemElongation << " - line " << __LINE__ << std::endl;

    if (PhaseStemElongation == 1) {

        //std::cout << "HaunGain = " << HaunGain << " - line " << __LINE__ << std::endl;
        //std::cout << "Ic = " << Ic << " - line " << __LINE__ << std::endl;
        //std::cout << "Cstr = " << Cstr << " - line " << __LINE__ << std::endl;
        //std::cout << "StressCold = " << StressCold << " - line " << __LINE__ << std::endl;
        //std::cout << "InternodeLengthMax = " << InternodeLengthMax << " - line " << __LINE__ << std::endl;
        //std::cout << "CoeffInternodeNum = " << CoeffInternodeNum << " - line " << __LINE__ << std::endl;

        ApexHeightGain = HaunGain * std::min(std::pow(std::min(Ic, 1.), 0.5),
                                             Cstr) * StressCold * InternodeLengthMax;
        ApexHeightGain = ApexHeightGain * CoeffInternodeNum;

        /*//std::cout << "ApexHeightGain = " << ApexHeightGain << " - line " << __LINE__ << std::endl;*/

    } else {
        ApexHeightGain = 0;
    }

    //    if (NumPhase == 7 && ChangePhase == 1) {    //  avt instruc inclus ds le code
    //        ApexHeight = 0;
    //    }

    ApexHeight = ApexHeight + ApexHeightGain;

    //std::cout << "ApexHeight = " << ApexHeight << " - line " << __LINE__ << std::endl;

    if (CstrMean <= 0) {
        CorrectedCstrMean = 1;
    } else {
        CorrectedCstrMean = CstrMean;
    }

    //    if (NumPhase == 7 && ChangePhase == 1) {
    //         // avt instruct inclu ds le code
    //        PlantHeight = 0;
    //        PlantWidth = 0;
    //    } else {


    //std::cout << "RelPotLeafLength = " << RelPotLeafLength << " - line " << __LINE__ << std::endl;
    //std::cout << "LeafLengthMax = " << LeafLengthMax << " - line " << __LINE__ << std::endl;
    //std::cout << "IcMean = " << IcMean << " - line " << __LINE__ << std::endl;
    //std::cout << "CorrectedCstrMean = " << CorrectedCstrMean << " - line " << __LINE__ << std::endl;
    //std::cout << "WtRatioLeafSheath = " << WtRatioLeafSheath << " - line " << __LINE__ << std::endl;
    //std::cout << "ApexHeight = " << ApexHeight << " - line " << __LINE__ << std::endl;


    PlantHeight = ApexHeight + (1.5 * (1 - Kdf) * RelPotLeafLength *
                                LeafLengthMax * std::sqrt(IcMean) *
                                CorrectedCstrMean * (1 + 1 / WtRatioLeafSheath));

    //std::cout << " PlantHeight = " << PlantHeight << " - line " << __LINE__ << std::endl;
    //std::cout << "Kdf = " << Kdf << " - line " << __LINE__ << std::endl;
    //std::cout << "IcMean = " << IcMean << " - line " << __LINE__ << std::endl;
    //std::cout << "RelPotLeafLength = " << RelPotLeafLength << " - line " << __LINE__ << std::endl;
    //std::cout << "LeafLengthMax = " << LeafLengthMax << " - line " << __LINE__ << std::endl;

    PlantWidth = std::pow(Kdf, 1.5) * 2 * std::sqrt(IcMean) * RelPotLeafLength *
            LeafLengthMax;

    //std::cout << "PlantHeight = " << PlantHeight << " - line " << __LINE__ << std::endl;
    //std::cout << "PlantWidth = " << PlantWidth << " - line " << __LINE__ << std::endl;
    //}

}

void SamaraModel2_1::evalParIntercepte()
{
    if (Lai > 0 && LIRkdfcl == 0) {
        LIRkdfcl = (1 - std::exp(-Kdf * Lai));
    }
    PARIntercepte = PAR * LIRkdfcl;

    //std::cout << "Lai = " << Lai << " - line " << __LINE__ << std::endl;
    //std::cout << "LIRkdfcl = " << LIRkdfcl << " - line " << __LINE__ << std::endl;
    //std::cout << "Kdf = " << Kdf << " - line " << __LINE__ << std::endl;
    //std::cout << "PAR = " << PAR << " - line " << __LINE__ << std::endl;
    //std::cout << "PARIntercepte = " << PARIntercepte << " - line " << __LINE__ << std::endl;

}

void SamaraModel2_1::evolMobiliTillerDeath()
{
    if (NumPhase == 3 || (NumPhase == 4 && SDJCorPhase4 <= 0.7 * SDJRPR
                          && CulmsPerPlant >= 1)) {
        TillerDeathPop = (1 - (std::min(Ic, 1.))) * CulmsPop * CoeffTillerDeath;
        CulmsPop = CulmsPop - TillerDeathPop;
        CulmsPerPlant = CulmsPop / (Density * PlantsPerHill);
        CulmsPerHill = CulmsPerPlant * PlantsPerHill;
        DryMatStructPaniclePop = DryMatStructPaniclePop *
                std::max(0., CulmsPop) / (CulmsPop + TillerDeathPop);

        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "SDJCorPhase4 = " << SDJCorPhase4 << " - line " << __LINE__ << std::endl;
        //std::cout << "SDJRPR = " << SDJRPR << " - line " << __LINE__ << std::endl;
        //std::cout << "CulmsPerPlant = " << CulmsPerPlant << " - line " << __LINE__ << std::endl;
        //std::cout << "Ic = " << Ic << " - line " << __LINE__ << std::endl;
        //std::cout << "CulmsPop = " << CulmsPop << " - line " << __LINE__ << std::endl;
        //std::cout << "CoeffTillerDeath = " << CoeffTillerDeath << " - line " << __LINE__ << std::endl;

    }
}

void SamaraModel2_2::evolMobiliTillerDeath()
{
    if (NumPhase == 3 || (NumPhase == 4 && SDJCorPhase4 <= 0.7 * SDJRPR
                          && CulmsPerPlant >= 1)) {

        TillerDeathPop = std::min(((1 - (std::min(Ic, 1.))) * CoeffTillerDeath * CulmsPop), 0.06 * CulmsPop);
        CulmsPop = CulmsPop - TillerDeathPop;
        CulmsPerPlant = CulmsPop / (Density * PlantsPerHill);
        CulmsPerHill = CulmsPerPlant * PlantsPerHill;
        DryMatStructPaniclePop = DryMatStructPaniclePop * std::max(0., CulmsPop) /
                (CulmsPop + TillerDeathPop);
    }
}

void SamaraModel2_1::evolMobiliLeafDeath()
{
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "avt modif" << " - line " << __LINE__ << std::endl;
    //
    //std::cout << "Ic = " << Ic << " - line " << __LINE__ << std::endl;
    //std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << " - line " << __LINE__ << std::endl;
    //std::cout << "CoeffLeafDeath = " << CoeffLeafDeath << " - line " << __LINE__ << std::endl;
    //std::cout << "DeadLeafDrywtPop = " << DeadLeafDrywtPop << " - line " << __LINE__ << std::endl;
    //std::cout << "LeafDeathPop = " << LeafDeathPop << " - line " << __LINE__ << std::endl;

    if (NumPhase > 1) {
        LeafDeathPop = (1 - (std::min(Ic, 1.))) * DryMatStructLeafPop *
                CoeffLeafDeath;
        DryMatStructLeafPop = DryMatStructLeafPop - LeafDeathPop;
        MobiliLeafDeath = 0.25 * LeafDeathPop;
        DeadLeafDrywtPop = DeadLeafDrywtPop + (0.75 * LeafDeathPop);
        LaiDead = DeadLeafDrywtPop * Sla;

        //std::cout << "si numpahase > 1 apres modif" << " - line " << __LINE__ << std::endl;
        //std::cout << "Ic = " << Ic << " - line " << __LINE__ << std::endl;
        //std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << " - line " << __LINE__ << std::endl;
        //std::cout << "CoeffLeafDeath = " << CoeffLeafDeath << " - line " << __LINE__ << std::endl;
        //std::cout << "DeadLeafDrywtPop = " << DeadLeafDrywtPop << " - line " << __LINE__ << std::endl;
        //std::cout << "LeafDeathPop = " << LeafDeathPop << " - line " << __LINE__ << std::endl;

    }
}

void SamaraModel2_1::evalSupplyTot()
{
    SupplyTot = Assim + MobiliLeafDeath - RespMaintTot -
            std::max(0., RespMaintDebt);

    //std::cout << "SupplyTot1 = " << SupplyTot << " - line " << __LINE__ << std::endl;

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
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "Assim = " << Assim << " - line " << __LINE__ << std::endl;
    //std::cout << "MobiliLeafDeath = " << MobiliLeafDeath << " - line " << __LINE__ << std::endl;
    //std::cout << "RespMaintTot = " << RespMaintTot << " - line " << __LINE__ << std::endl;
    //std::cout << "RespMaintDebt = " << RespMaintDebt << " - line " << __LINE__ << std::endl;
    //std::cout << "SupplyTot2 = " << SupplyTot << " - line " << __LINE__ << std::endl;
    //std::cout << "CumSupplyTot = " << CumSupplyTot << " - line " << __LINE__ << std::endl;
    //std::cout << "RespMaintDebt = " << RespMaintDebt << " - line " << __LINE__ << std::endl;

}

void SamaraModel2_1::evalDemandStructLeaf()
{
    double CorrectedSla;

    //std::cout << "avt chg = " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "RelPotLeafLength = " << RelPotLeafLength << " - line " << __LINE__ << std::endl;
    //std::cout << "LeafLengthMax = " << LeafLengthMax << " - line " << __LINE__ << std::endl;
    //std::cout << "CoeffLeafWLRatio = " << CoeffLeafWLRatio << " - line " << __LINE__ << std::endl;
    //std::cout << "PlantLeafNumNew = " << PlantLeafNumNew << " - line " << __LINE__ << std::endl;
    //std::cout << "Cstr = " << Cstr << " - line " << __LINE__ << std::endl;
    //std::cout << "StressCold = " << StressCold << " - line " << __LINE__ << std::endl;
    //std::cout << "DemLeafAreaPlant = " << DemLeafAreaPlant << " - line " << __LINE__ << std::endl;
    //std::cout << "CorrectedSla = " << CorrectedSla << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructLeafPlant = " << DemStructLeafPlant << " - line " << __LINE__ << std::endl;
    //std::cout << "Density = " << Density << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructLeafPop = " << DemStructLeafPop << " - line " << __LINE__ << std::endl;

    if (NumPhase > 1 && NumPhase < 5) {
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
        //std::cout << "aprs chg = " << " - line " << __LINE__ << std::endl;
        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "RelPotLeafLength = " << RelPotLeafLength << " - line " << __LINE__ << std::endl;
        //std::cout << "LeafLengthMax = " << LeafLengthMax << " - line " << __LINE__ << std::endl;
        //std::cout << "CoeffLeafWLRatio = " << CoeffLeafWLRatio << " - line " << __LINE__ << std::endl;
        //std::cout << "PlantLeafNumNew = " << PlantLeafNumNew << " - line " << __LINE__ << std::endl;
        //std::cout << "Cstr = " << Cstr << " - line " << __LINE__ << std::endl;
        //std::cout << "StressCold = " << StressCold << " - line " << __LINE__ << std::endl;
        //std::cout << "DemLeafAreaPlant = " << DemLeafAreaPlant << " - line " << __LINE__ << std::endl;
        //std::cout << "CorrectedSla = " << CorrectedSla << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructLeafPlant = " << DemStructLeafPlant << " - line " << __LINE__ << std::endl;
        //std::cout << "Density = " << Density << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructLeafPop = " << DemStructLeafPop << " - line " << __LINE__ << std::endl;

    }
}

void SamaraModel2_1::evalDemandStructIN()
{
    if (PhaseStemElongation == 1) {
        DemStructInternodePlant = std::pow(std::min(Ic, 1.), 0.5) *
                ApexHeightGain * CulmsPerHill * CoeffInternodeMass;
        DemStructInternodePop = DemStructInternodePlant * Density / 1000;
    }
    if (NumPhase > 1 && NumPhase < 5) {
        DemResInternodePop = (ResCapacityInternodePop -
                              DryMatResInternodePop) * CoeffReserveSink;
    }
}

void SamaraModel2_1::evalDemandTotAndIcPreFlow()
{
    //std::cout << "avt chg evalDemandTotAndIcPreFlow"  << " - line " << __LINE__ << std::endl;
    //
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "Ic = " << Ic << " - line " << __LINE__ << std::endl;
    //std::cout << "SupplyTot = " << SupplyTot << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructTotPop = " << DemStructTotPop << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructLeafPop = " << DemStructLeafPop << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructSheathPop = " << DemStructSheathPop << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructRootPop = " << DemStructRootPop << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructInternodePop = " << DemStructInternodePop << " - line " << __LINE__ << std::endl;
    //std::cout << "DemStructPaniclePop = " << DemStructPaniclePop << " - line " << __LINE__ << std::endl;
    //std::cout << "DemResInternodePop = " << DemResInternodePop << " - line " << __LINE__ << std::endl;


    if (NumPhase > 1 && NumPhase < 5) {
        DemStructTotPop = DemStructLeafPop + DemStructSheathPop +
                DemStructRootPop + DemStructInternodePop +
                DemStructPaniclePop + DemResInternodePop;
        A_DemStructTot = DemStructLeafPop + DemStructSheathPop +
                DemStructRootPop + DemStructInternodePop +
                DemStructPaniclePop + DemResInternodePop;

        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "SupplyTot = " << SupplyTot << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructTotPop = " << DemStructTotPop << " - line " << __LINE__ << std::endl;


        //TODO CHECK THIS
        if(DemStructTotPop == 0) {
            Ic = 1;
        } else {
            Ic = SupplyTot / DemStructTotPop;
            if (Ic <= 0) {
                Ic = 0;
            }
            if (PlantHeight == 0){ //|| (DemStructTotPop == 0)) {
                Ic = 1;
            }
        }
        //        Ic = SupplyTot / DemStructTotPop;
        //        if (Ic <= 0) {
        //            Ic = 0;
        //        }
        //      if (PlantHeight == 0){ //|| (DemStructTotPop == 0)) {
        //          Ic = 1;
        //      }

        //std::cout << "aprs chgt evalDemandTotAndIcPreFlow" << " - line " << __LINE__ << std::endl;
        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "Ic = " << Ic << " - line " << __LINE__ << std::endl;
        //std::cout << "SupplyTot = " << SupplyTot << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructTotPop = " << DemStructTotPop << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructLeafPop = " << DemStructLeafPop << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructSheathPop = " << DemStructSheathPop << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructRootPop = " << DemStructRootPop << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructInternodePop = " << DemStructInternodePop << " - line " << __LINE__ << std::endl;
        //std::cout << "DemStructPaniclePop = " << DemStructPaniclePop << " - line " << __LINE__ << std::endl;
        //std::cout << "DemResInternodePop = " << DemResInternodePop << " - line " << __LINE__ << std::endl;


        IcCumul = IcCumul + std::min(Ic, 1.);
        IcMean = IcCumul / NbDaysSinceGermination;
        CstrCumul = CstrCumul + Cstr;
        CstrMean = CstrCumul / NbDaysSinceGermination;


        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "Ic = " << Ic << " - line " << __LINE__ << std::endl;
        //std::cout << "IcCumul = " << IcCumul << " - line " << __LINE__ << std::endl;
        //std::cout << "NbDaysSinceGermination = " << NbDaysSinceGermination << " - line " << __LINE__ << std::endl;
        //std::cout << "IcMean = " << IcMean << " - line " << __LINE__ << std::endl;


    }
    if (NumPhase == 5 || NumPhase == 6) {
        IcCumul = IcCumul + std::min(Ic, 1.);
        IcMean = IcCumul / NbDaysSinceGermination;
        CstrCumul = CstrCumul + Cstr;
        CstrMean = CstrCumul / NbDaysSinceGermination;
    }
}

void SamaraModel2_1::evolGrowthStructLeafPop()
{
    if (NumPhase > 1 && NumPhase < 5) {
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

void SamaraModel2_1::evolGrowthStructINPop()
{
    if (NumPhase > 1 && NumPhase < 5) {
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

void SamaraModel2_1::addResToGrowthStructPop()
{
    if (NumPhase > 1) {
        if (Ic < 1 && DemStructTotPop > 0) {
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

void SamaraModel2_1::evolDemPanFilPopAndIcPFlow()
{
    if (NumPhase == 5) {
        PanicleSinkPop = DryMatStructPaniclePop * CoeffPanSinkPop *
                (1 - SterilityTot);
        DemPanicleFillPop = (DegresDuJourCor / SDJMatu1) * PanicleSinkPop
                * std::sqrt(std::max(0.00001, StressCold));
        //std::cout << "*******" << " - line " << __LINE__ << std::endl;
        //std::cout << "*******" << DegresDuJourCor << " " << SDJMatu1 << " " << PanicleSinkPop << " " << StressCold << " - line " << __LINE__ << std::endl;
        Ic = SupplyTot / std::max(DemPanicleFillPop, 0.0000001);
        //std::cout << Ic << " " << DemPanicleFillPop << " l1821" <<"line " << __LINE__ << std::endl;
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

void SamaraModel2_1::evolPanicleFilPop()
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

void SamaraModel2_1::evolGrowthReserveInternode()
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

void SamaraModel2_1::evolGrowthTot()
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

void SamaraModel2_1::evalLai()
{
    double CorrectedSla;

    if (NumPhase == 2 && ChangePhase == 1) {
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

    //std::cout << "SlaMax = " << SlaMax << " - line " << __LINE__ << std::endl;

    //std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << " - line " << __LINE__ << std::endl;



}

void SamaraModel2_1::evalClumpAndLightInter()
{
    double RolledLai;

    if (NumPhase > 1 && PlantWidth > 0) {
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

        //std::cout << "Lai = " << Lai << " - line " << __LINE__ << std::endl;
        //std::cout << "KRolling = " << KRolling << " - line " << __LINE__ << std::endl;
        //std::cout << "FractionPlantHeightSubmer = " << FractionPlantHeightSubmer << " - line " << __LINE__ << std::endl;
        //std::cout << "RolledLai = " << RolledLai << " - line " << __LINE__ << std::endl;
        //std::cout << "LTRkdfcl = " << LTRkdfcl << " - line " << __LINE__ << std::endl;

    }
}

void SamaraModel2_1::evalRUE()
{
    double CorrectedIrrigation;

    if (NumPhase < 1 || (NumPhase == 1 && ChangePhase == 1) ||
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
        if (Transplanting == 0 || NurseryStatus == 1) {
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
        if (Transplanting == 0 || NurseryStatus == 1) {
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

        //std::cout << "Tr = " << Tr << " - line " << __LINE__ << std::endl;
        //std::cout << "NbJas = " << NbJas << " - line " << __LINE__ << std::endl;
        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;

        if (Tr > 0 && NbJas > 20 && NumPhase > 1) {
            TrEffInst = (SupplyTot - AssimNotUsed) / (Tr * 10000);
            TrEff = DryMatTotPop / (CumTr * 10000);
            WueEt = DryMatTotPop / (CumEt * 10000);
            WueTot = DryMatTotPop / (CumWuse * 10000);
            RUE = (DryMatAboveGroundTotPop / std::max(CumPar, 0.00001)) / 10;

            //std::cout << "CumPAR = " << CumPar << " - line " << __LINE__ << std::endl;
            //std::cout << "RUE = " << RUE << " - line " << __LINE__ << std::endl;

        }
    }
}

void SamaraModel2_2::evalRUE()
{

    double CorrectedIrrigation;

    if (NumPhase < 1 || (NumPhase == 1 && ChangePhase == 1) || Density == DensityNursery) {
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
        if (Transplanting == 0 || NurseryStatus == 1) {
            CumPar = CumPar + PARIntercepte;
            CumTr = CumTr + Tr;
            CumEt = CumEt + Tr + Evap;
            CumWUse = CumWUse + Tr + Evap + Dr + Lr;
        }
        if (Irrigation == NullValue) {
            CorrectedIrrigation = 0;
        } else {
            CorrectedIrrigation = Irrigation;
        }
        if (Transplanting == 0 || NurseryStatus == 1) {
            CumWReceived = CumWReceived + Rain + CorrectedIrrigation + IrrigAutoDay;
            CumIrrig = CumIrrig + CorrectedIrrigation + IrrigAutoDay;
            CumDr = CumDr + Dr;
            CumLr = CumLr + Lr;
        }
        if (AssimPot !=  0) {
            ConversionEff = Conversion * Assim / (PARIntercepte * Conversion * 10);
        }
        if (Tr > 0 && NbJas > 20 && NumPhase > 1) {
            TrEffInst = (SupplyTot - AssimNotUsed) / (Tr * 10000);
            TrEff = DryMatTotPop / (CumTr * 10000);
            WueEt = DryMatTotPop / (CumEt * 10000);
            WueTot = DryMatTotPop / (CumWuse * 10000);
            RUE = (DryMatAboveGroundTotPop / std::max(CumPar, 0.00001)) / 10;
            RUEgreen = (DryMatAboveGroundPop / std::max(CumPar, 0.00001)) / 10;
        }
    }
}

void SamaraModel2_1::evolEvapSurfRFE_RDE()
{
    double ValRSurfPrec;
    double EvapRU;
    double Evap1;
    double Evap2;

    //std::cout << "StockMacropores = " << StockMacropores << " - line " << __LINE__ << std::endl;
    //std::cout << "FloodwaterDepth = " << FloodwaterDepth << " - line " << __LINE__ << std::endl;

    if (StockMacropores + FloodwaterDepth == 0) {
        ValRSurfPrec = ValRSurf;

        //std::cout << "ValRSurf = " << ValRSurf << " - line " << __LINE__ << std::endl;
        //std::cout << "ValRFE = " << ValRFE << " - line " << __LINE__ << std::endl;

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

            //std::cout << "EvapPot = " << EvapPot << " - line " << __LINE__ << std::endl;
            //std::cout << "Evap1 = " << Evap1 << " - line " << __LINE__ << std::endl;
            //std::cout << "Evap2 = " << Evap2 << " - line " << __LINE__ << std::endl;


        }
        Evap = Evap1 + Evap2; // output variable
        ValRFE = ValRFE - Evap1;
        ValRSurf = ValRSurf - Evap2;
        ValRDE = std::max(0., ValRSurf - CapaREvap);

        //std::cout << "Evap = " << Evap << " - line " << __LINE__ << std::endl;
        //std::cout << "ValRFE = " << ValRFE << " - line " << __LINE__ << std::endl;
        //std::cout << "ValRSurf = " << ValRSurf << " - line " << __LINE__ << std::endl;
        //std::cout << "ValRDE = " << ValRDE << " - line " << __LINE__ << std::endl;
        //std::cout << "CapaREvap = " << CapaREvap << " - line " << __LINE__ << std::endl;


        if (EvapPot == 0) {
            Kr = 0;
        } else {
            Kr = Evap / EvapPot;
        }

        //std::cout << "Kr = " << Kr << " - line " << __LINE__ << std::endl;

        if (ValRSurf >= CapaREvap) {
            EvapRU = Evap;
        } else {
            if (ValRSurfPrec <= CapaREvap) {
                EvapRU = Evap1;
            } else {
                EvapRU = Evap1 + ValRSurfPrec - CapaREvap;
            }
        }

        //std::cout << "EvapRU = " << EvapRU << " - line " << __LINE__ << std::endl;

        if (RuRac <= RuSurf) {
            StockRac = std::max(0., StockRac - EvapRU * RuRac / RuSurf); // output variable
        } else {
            StockRac = std::max(0., StockRac - EvapRU); // output variable
        }

        //std::cout << "StockRac = " << StockRac << " - line " << __LINE__ << std::endl;

        StockTotal = StockTotal - EvapRU; // output variable

        //std::cout << "evolEvapSurfRFE_RDE()" << " - line " << __LINE__ << std::endl;
        //std::cout << "StockTotal = " << StockTotal << " - line " << __LINE__ << std::endl;


        //std::cout << "StockTotal = " << StockTotal << " - line " << __LINE__ << std::endl;

        StockRac = std::min(StockRac, StockTotal); // output variable

        //std::cout << "StockRac = " << StockRac << " - line " << __LINE__ << std::endl;

        KceReal = Kce * Kr;

        //std::cout << "KceReal = " << KceReal << " - line " << __LINE__ << std::endl;

    }
    if (StockMacropores + FloodwaterDepth > 0 && NumPhase > 0) {
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
    if (StockMacropores + FloodwaterDepth > 0 && NumPhase == 0) {
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

void SamaraModel2_1::evolDryMatTot()
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

    //std::cout << "ChangePhase = " << ChangePhase << " - line " << __LINE__ << std::endl;

    if (NumPhase == 2 && ChangePhase == 1) {
        DryMatTotPop = Density * PlantsPerHill * TxResGrain * PoidsSecGrain /
                1000;

        //std::cout << "Density = " << Density  << " - line " << __LINE__ << std::endl;
        //std::cout << "PlantsPerHill = " << PlantsPerHill << " - line " << __LINE__ << std::endl;
        //std::cout << "TxResGrain = "  << TxResGrain << " - line " << __LINE__ << std::endl;
        //std::cout << "PoidsSecGrain = " << PoidsSecGrain << " - line " << __LINE__ << std::endl;
        //std::cout << "DryMatTotPop = " << DryMatTotPop << " - line " << __LINE__ << std::endl;

        DryMatStructLeafPop = DryMatTotPop * 0.5;

        //std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << " - line " << __LINE__ << std::endl;

    } else {
        if (NumPhase > 1) {

            //std::cout << "DryMatStructLeafPop = " << DryMatStructLeafPop << " - line " << __LINE__ << std::endl;

            DryMatStructLeafPop = DryMatStructLeafPop + GrowthStructLeafPop;

            //std::cout << "GrowthStructLeafPop = " << GrowthStructLeafPop << " - line " << __LINE__ << std::endl;

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
            //GrainFillingStatus = 1000 * (GrainYieldPop / FertSpikeNumPop) / PoidsSecGrain;        avt
            GrainFillingStatus = 1000 * (GrainYieldPop / std::max(FertSpikeNumPop, 0.00000001)) /
                    PoidsSecGrain;
        }
    }
}

void SamaraModel2_1::evolGrowthStructTot()
{
    if (NumPhase > 1 && NumPhase < 5) {
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

void SamaraModel2_1::leafRolling()
{
    if (NumPhase > 1) {
        KRolling = RollingBase + (1 - RollingBase) *
                std::pow(FTSW,  std::max(0.0000001, Eto * RollingSens));
        if (KRolling > 1) {
            KRolling = 1;
        }
    }
}

void SamaraModel2_1::evalAssimPot()
{
    //std::cout << "DayLength = " << DayLength << " - line " << __LINE__ << std::endl;
    //std::cout << "CO2Exp = " << CO2Exp << " - line " << __LINE__ << std::endl;
    //std::cout << "CO2Cp = " << CO2Cp << " - line " << __LINE__ << std::endl;
    //std::cout << "Ca = " << Ca << " - line " << __LINE__ << std::endl;

    if (-CO2Exp != 0 && CO2Cp != 0) {
        CoeffCO2Assim = (1 - std::exp(-CO2Exp * (Ca - CO2Cp))) /
                (1 - std::exp(-CO2Exp * (400 - CO2Cp)));
    }

    //std::cout << "CoeffCO2Assim = " << CoeffCO2Assim << " - line " << __LINE__ << std::endl;

    //std::cout << "PARIntercepte = " << PARIntercepte << " - line " << __LINE__ << std::endl;
    //std::cout << "Conversion = " << Conversion << " - line " << __LINE__ << std::endl;
    //std::cout << "PAR = " << PAR << " - line " << __LINE__ << std::endl;

    AssimPot = PARIntercepte * Conversion * 10 * CoeffCO2Assim;
    AssimPot = AssimPot * std::min(((3 * TMax + TMin) / 4 - TBase) /
                                   (TOpt1 - TBase), 1.);
    AssimPot = AssimPot * std::sqrt(std::max(0.00001, StressCold));
    if (PARIntercepte != 0 && DayLength != 0) {
        AssimPot = AssimPot * std::pow((PAR / DayLength), 0.667) /
                (PAR / DayLength);
        AssimPot = AssimPot * std::pow((SlaMin / std::max(Sla, SlaMin)),
                                       CoeffAssimSla);
    }
}

void SamaraModel2_1::automaticIrrigation()
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
    if (NumPhase > 4 && NumPhase < 7 && DAF > PlotDrainageDAF) {
        CorrectedBundHeight = 0;
    }
    if (NbJas >= IrrigAutoStop && NbJas < IrrigAutoResume) {
        StressPeriod = 1;
    } else {
        StressPeriod = 0;
    }
    if (StressPeriod == 1 && FloodwaterDepth > 0) {
        Lr = Lr + FloodwaterDepth;
        FloodwaterDepth = 0;
    }
    if (NumPhase < 7 && DAF <= PlotDrainageDAF && IrrigAuto == 1 &&
            NumPhase > 0 && CorrectedBundHeight > 0 && FTSW <= FTSWIrrig &&
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
            if (VolMacropores > 0 && StockMacropores < VolMacropores &&
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
        if (NumPhase < 7 && DAF <= PlotDrainageDAF && IrrigAuto == 1 &&
                NumPhase > 0 && CorrectedBundHeight == 0) {
            FloodwaterDepth = 0;
        }
    }
    IrrigTotDay = CorrectedIrrigation + IrrigAutoDay;
}

void SamaraModel2_1::evolRurRFE_RDE()
{
    double DeltaRur;

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;

    if (NumPhase == 0) {
        RuRac = 0;
        StockRac = 0;
    } else {
        if (NumPhase == 1 && ChangePhase == 1) {

            //std::cout << "ResUtil = " << ResUtil << " - line " << __LINE__ << std::endl;
            //std::cout << "ProfRacIni = " << ProfRacIni << " - line " << __LINE__ << std::endl;
            //std::cout << "EpaisseurSurf = " << EpaisseurSurf << " - line " << __LINE__ << std::endl;
            //std::cout << "EpaisseurProf = " << EpaisseurProf << " - line " << __LINE__ << std::endl;
            //std::cout << "ResUtil = " << ResUtil << " - line " << __LINE__ << std::endl;

            RuRac = ResUtil * std::min(ProfRacIni,
                                       (EpaisseurSurf + EpaisseurProf)) / 1000;

            //std::cout << "RuRac = " << RuRac << " - line " << __LINE__ << std::endl;

            if (ProfRacIni <= EpaisseurSurf) {
                StockRac = (ValRDE + ValRFE) * ProfRacIni / EpaisseurSurf;
            } else {
                StockRac = StockTotal *
                        std::min(ProfRacIni /  (EpaisseurSurf + EpaisseurProf), 1.);
            }

            //std::cout << "StockRac = " << StockRac << " - line " << __LINE__ << std::endl;

        } else {
            if (Hum - StockMacropores - RuRac < VitesseRacinaire / 1000 *
                    ResUtil) {
                DeltaRur = std::max(0., Hum - StockMacropores - RuRac);
                if (RootFront >= EpaisseurSurf + EpaisseurProf ||
                        RootFront >= RootFrontMax) {
                    DeltaRur = 0;
                }
            } else {
                DeltaRur = VitesseRacinaire / 1000 * ResUtil;
                if (RootFront >= EpaisseurSurf + EpaisseurProf ||
                        RootFront >= RootFrontMax) {
                    DeltaRur = 0;
                }
            }

            //std::cout << "RootFront = " << RootFront << " - line " << __LINE__ << std::endl;
            //std::cout << "RootFrontMax = " << RootFrontMax << " - line " << __LINE__ << std::endl;
            //std::cout << "ResUtil = " << ResUtil << " - line " << __LINE__ << std::endl;
            //std::cout << "VitesseRacinaire = " << VitesseRacinaire << " - line " << __LINE__ << std::endl;
            //std::cout << "DeltaRur = " << DeltaRur << " - line " << __LINE__ << std::endl;

            RuRac = RuRac + DeltaRur;
            if (RuRac > RuSurf) {
                StockRac = StockRac + DeltaRur;
            } else {
                StockRac = (ValRDE + ValRFE) * (RuRac / RuSurf);
            }

            //std::cout << "StockRac = " << StockRac << " - line " << __LINE__ << std::endl;

        }
    }
    //std::cout << "avt chg rootfront = " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "ResUtil = " << ResUtil << " - line " << __LINE__ << std::endl;
    //std::cout << "RuRac = " << RuRac << " - line " << __LINE__ << std::endl;
    //std::cout << "RootFront = " << RootFront << " - line " << __LINE__ << std::endl;
    if (NumPhase != 0) {
        RootFront = ((1 / ResUtil) * RuRac) * 1000;

        //std::cout << "RootFront = " << RootFront << " - line " << __LINE__ << std::endl;

        if (ChangeNurseryStatus == 1 && Transplanting == 1) {
            RootFront = TransplantingDepth;
            if (RootFront < 40) {
                RootFront = 40;
            } else if (RootFront > 200) {
                RootFront = 200;
            }
            RuRac = RootFront * ResUtil / 1000;
        }
    }

    //std::cout << "RuRac = " << RuRac << " - line " << __LINE__ << std::endl;

    if (StockMacropores + FloodwaterDepth > 0) {
        StockRac = RootFront * ResUtil / 1000 +
                (RootFront / (EpaisseurSurf + EpaisseurProf)) * StockMacropores;
        StockRac = std::min(StockRac, StockTotal);
    }

    //std::cout << "aprs chg rootfront = " << " - line " << __LINE__ << std::endl;
    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "ResUtil = " << ResUtil << " - line " << __LINE__ << std::endl;
    //std::cout << "RuRac = " << RuRac << " - line " << __LINE__ << std::endl;
    //std::cout << "RootFront = " << RootFront << " - line " << __LINE__ << std::endl;
}

void SamaraModel2_1::evalSimEndCycle()

{
    if (NumPhase == 7 && ChangePhase == 1) {
        SimEndCycle = NbJas;
    }
}

void SamaraModel2_1::priority2GrowthPanStrctPop()
{
    double GrowthPanDeficit;
    double GrowthStructPaniclePlus;

    if (GrowthStructPaniclePop < DemStructPaniclePop && NumPhase == 4) {
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

void SamaraModel2_1::keyResults()
{
    if (NumPhase > 1 && NumPhase < 7) {
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

//void SamaraModel2_1::resetVariablesToZero()
//{
//    if ((NumPhase == 7) && (ChangePhase == 1)){
//        CulmsPerPlant = 0;
//        CulmsPerHill = 0;
//        CulmsPop = 0;
//        GrainYieldPop = 0;
//        DryMatStructLeafPop = 0;
//        DryMatStructSheathPop = 0;
//        DryMatStructRootPop = 0;
//        DryMatStructInternodePop = 0;
//        DryMatResInternodePop = 0;
//        DryMatStructPaniclePop = 0;
//        DryMatStructStemPop = 0;
//        DryMatStructTotPop = 0;
//        DryMatVegeTotPop = 0;
//        DryMatPanicleTotPop = 0;
//        DryMatAboveGroundPop = 0;
//        DryMatTotPop = 0;
//        HarvestIndex = 0;
//        PanicleNumPop = 0;
//        PanicleNumPlant = 0;
//        GrainYieldPanicle = 0;
//        SpikeNumPop = 0;
//        SpikeNumPanicle = 0;
//        FertSpikePop = 0;
//        GrainFillingStatus = 0;
//        PhaseStemElongation = 0;
//        Sla = 0;
//        HaunIndex = 0;
//        ApexHeight = 0;
//        PlantHeight = 0;
//        PlantWidth = 0;
//        VitesseRacinaireDay = 0;
//        Kcl = 0;
//        KRolling = 0;
//        LIRkdfcl = 0;
//        LTRkdfcl = 1;
//        AssimPot = 0;
//        Assim = 0;
//        RespMaintTot = 0;
//        SupplyTot = 0;
//        AssimSurplus = 0;
//        AssimNotUsed = 0;
//        AssimNotUsedCum = 0;
//        TillerDeathPop = 0;
//        DeadLeafDrywtPop = 0;
//        ResCapacityInternodePop = 0;
//        InternodeResStatus = 0;
//        Cstr = 0;
//        FTSW = 0;
//        DryMatAboveGroundTotPop = 0;
//    }
//}


void SamaraModel::degToRad()
{
    LatRad = Latitude * M_PI / 180;
    //std::cout << "LatRad = " << LatRad << " - line " << __LINE__ << std::endl;
}

void SamaraModel::avgTempHum()
{
    if (TMin != NullValue && TMax != NullValue) {
        TMoyCalc = (TMax + TMin) / 2;
    } else {
        TMoyCalc = TMoy;
    }
    if (HMin != NullValue && HMax != NullValue) {
        HMoyCalc = (HMax + HMin) / 2;
    } else {
        HMoyCalc = HMoy;
    }
    //std::cout << "TMoyCalc = " << TMoyCalc << " - line " << __LINE__ << std::endl;
    //std::cout << "HMoyCalc = " << HMoyCalc << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalDecli(double t)
{
    Decli = 0.409 * std::sin(0.0172 * artis::utils::DateTime::dayOfYear(t) - 1.39);
    //std::cout << "Decli = " << Decli << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalSunPosi()
{
    SunPosi = std::acos(-std::tan(LatRad) * std::tan(Decli));
    //std::cout << "SunPosi = " << SunPosi << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalDayLength()
{
    DayLength = 7.64 * SunPosi;
    //std::cout << "DayLength = " << DayLength << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalSunDistance(double t)
{
    SunDistance = 1 + 0.033 *
            std::cos(2 * M_PI / 365 * artis::utils::DateTime::dayOfYear(t));
    //std::cout << "SunDistance = " << SunDistance << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalRayExtra()
{
    RayExtra = 24 * 60 * 0.0820 / M_PI * SunDistance *
            (SunPosi * std::sin(Decli) * std::sin(LatRad) +
             std::cos(Decli) * std::cos(LatRad) * std::sin(SunPosi));
    //std::cout << "RayExtra = " << RayExtra << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalRgMax()
{
    RgMax = (0.75 + 0.00002 * Altitude) * RayExtra;
    //std::cout << "RgMax = " << RgMax << " - line " << __LINE__ << std::endl;
}

void SamaraModel::insToRg()
{
    if (Rg == NullValue) {
        RgCalc = (0.25 + 0.50 * std::min(Ins / DayLength, 1.)) * RayExtra;
    } else {
        RgCalc = Rg;
    }
    //std::cout << "RGCalc = " << RgCalc << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalPAR()
{
    PAR = KPar * RgCalc;
    //std::cout << "KPar = " << KPar << " - line " << __LINE__ << std::endl;
    //std::cout << "RgCalc = " << RgCalc << " - line " << __LINE__ << std::endl;
    //std::cout << "PAR = " << PAR << " - line " << __LINE__ << std::endl;
}

void SamaraModel::etoFAO()
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

    if (ETP == 0) {
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

void SamaraModel::evolPhenoStress(double t)
{

    if (t >= DateSemis) {

        bool ChangementDePhase = false;
        bool ChangementDeSousPhase = false;

        //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
        //std::cout << "SeuilTempPhaseSuivante = " << SeuilTempPhaseSuivante << " - line " << __LINE__ << std::endl;
        //std::cout << "SommeDegresJour = " << SommeDegresJour << " - line " << __LINE__ << std::endl;
        //std::cout << "StockSurface = " << StockSurface << " - line " << __LINE__ << std::endl;
        //std::cout << "TxRuSurfGermi = " << TxRuSurfGermi << " - line " << __LINE__ << std::endl;
        //std::cout << "RuSurf = " << RuSurf << " - line " << __LINE__ << std::endl;
        //std::cout << "StockTotal = " << StockTotal << " - line " << __LINE__ << std::endl;
        //std::cout << "SumPP = " << SumPP << " - line " << __LINE__ << std::endl;
        //std::cout << "PPSens = " << PPSens << " - line " << __LINE__ << std::endl;


        ChangePhase = 0;
        ChangeSousPhase = 0;
        if ((int)(NumPhase) == 0) {
            if (StockSurface >= TxRuSurfGermi * RuSurf || StRu > StockSurface) { //StockTotal > StockSurface) {  instruction precedente
                NumPhase = 1;
                ChangePhase = 1;
                SeuilTempPhaseSuivante = SDJLevee;
            }
        } else {
            if ((int)(NumPhase) == 1 && //NumPhase == 2 &&  instruc precedente
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
                NumPhase = 7;           //  avt instruction active
            } else {
                ChangePhase = 1;
                NumPhase = NumPhase + 1;
                SommeDegresJourPhasePrecedente = SeuilTempPhaseSuivante;
                switch ((int)(NumPhase)) {
                case 2: SeuilTempPhaseSuivante = SeuilTempPhaseSuivante +
                            SDJBVP; break;
                    //case 3: SumPP = 0; break; //  instruction active avt
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

            //std::cout << "SommeDegresJour = " << SommeDegresJour << " - line " << __LINE__ << std::endl;
            //std::cout << "SeuilTempSousPhaseSuivante = " << SeuilTempSousPhaseSuivante << " - line " << __LINE__ << std::endl;
            //std::cout << "ChangementDeSousPhase = " << ChangementDeSousPhase << " - line " << __LINE__ << std::endl;

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

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;

}



void SamaraModel2_1::demandePlante()
{
    TrPot = Kcp * Eto;
    CoeffCO2Tr = Ca * CO2SlopeTr - 400 * CO2SlopeTr + 1;
    TrPot = TrPot * CoeffCO2Tr;

    //std::cout << "Kcp = " << Kcp << " - line " << __LINE__ << std::endl;
    //std::cout << "Eto = " << Eto << " - line " << __LINE__ << std::endl;
    //std::cout << "Ca = " << Ca << " - line " << __LINE__ << std::endl;
    //std::cout << "CO2SlopeTr = " << CO2SlopeTr << " - line " << __LINE__ << std::endl;
    //std::cout << "CoeffCO2Tr = " << CoeffCO2Tr << " - line " << __LINE__ << std::endl;
    //std::cout << "TrPot = " << TrPot << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalTranspi()
{
    Tr = TrPot * Cstr;

    //std::cout << "Tr = " << Tr << " - line " << __LINE__ << std::endl;
}

void SamaraModel::evalETRETM()
{
    ETM = Evap + TrPot;
    ETR = Evap + Tr;

    //std::cout << "TrPot = " << TrPot << " - line " << __LINE__ << std::endl;
    //std::cout << "Tr = " << Tr << " - line " << __LINE__ << std::endl;
    //std::cout << "Evap = " << Evap << " - line " << __LINE__ << std::endl;
    //std::cout << "ETM = " << ETM << " - line " << __LINE__ << std::endl;
    //std::cout << "ETR = " << ETR << " - line " << __LINE__ << std::endl;

}

void SamaraModel::evolSomDegresJour()
{
    if (NumPhase >= 1) {
        SommeDegresJour = SommeDegresJour + DegresDuJour;
    } else {
        SommeDegresJour = 0;
    }

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "DegresDuJour = " << DegresDuJour << " - line " << __LINE__ << std::endl;
    //std::cout << "SommeDegresJour = " << SommeDegresJour << " - line " << __LINE__ << std::endl;

}

void SamaraModel::mortality()
{
    double MoyenneCstr;
    if(NumPhase == 1){
        NbJourCompte = 0;  //avt if non present
        tabCstrIndiceCourant = 0;
    }
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
            for(int i=1;i<=5;i++){    //for (int i = 0; i < 5; ++i) {  avt instruction definies comme tel
                MoyenneCstr = MoyenneCstr + tabCstr[i];
            }
            if (MoyenneCstr / 5 <= SeuilCstrMortality) {
                NumPhase = 7;
            }
        }
    }
}

void SamaraModel::evalTMaxMoy() {

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "NumSousPhase = " << NumSousPhase << " - line " << __LINE__ << std::endl;

    if (NumPhase == 4 && NumSousPhase == 4) {
        TMaxMoy = calculeLaMoyenne(TMax, MonCompteur, TMaxMoy);
    } else if (NumPhase < 4) {
        TMaxMoy = 0;
    }

    //std::cout << "TMaxMoy = " << TMaxMoy << " - line " << __LINE__ << std::endl;

}

void SamaraModel::evalTMinMoy() {

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "NumSousPhase = " << NumSousPhase << " - line " << __LINE__ << std::endl;

    if (NumPhase == 4 && NumSousPhase == 3) {
        TMinMoy = calculeLaMoyenne(TMin, MonCompteur, TMinMoy);
    } else if (NumPhase < 4) {
        TMinMoy = 0;
    }

    //std::cout << "TMinMoy = " << TMinMoy << " - line " << __LINE__ << std::endl;

}

void SamaraModel::evalFtswMoy() {

    //std::cout << "NumPhase = " << NumPhase << " - line " << __LINE__ << std::endl;
    //std::cout << "NumSousPhase = " << NumSousPhase << " - line " << __LINE__ << std::endl;

    if (NumPhase == 4 && NumSousPhase == 4) {
        FtswMoy = calculeLaMoyenne(FTSW, MonCompteur, FtswMoy);
    } else if (NumPhase < 4) {
        FtswMoy = 0;
    }

    //std::cout << "FtswMoy = " << FtswMoy << " - line " << __LINE__ << std::endl;

}

double SamaraModel::calculeLaMoyenne(double laValeur, double leCompteur, double laMoyenne) {
    double moyenne;

    moyenne = (laMoyenne * (leCompteur - 1) + laValeur) / leCompteur;
    return moyenne;
}

void SamaraModel::evalNbJas(double t) {

    NbJas = t - DateSemis;

    //std::cout << "NbJas (calcule) = " << NbJas << " - line " << __LINE__ << std::endl;
}
