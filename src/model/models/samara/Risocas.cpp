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

#include <utils/DateTime.hpp>

#include <algorithm>
#include <cmath>

namespace model { namespace models { namespace samara {

void Model::evalDegreeDay(double t)
{
    if (t >= DateSemis) {

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
            DegresDuJourCor = DegresDuJour * std::pow(std::max(cstr, 0.00000001),
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
    } else {
        DegresDuJour = 0.;
        DegresDuJourCor = 0.;
    }
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
    } else {
        HaunGain = 0;
        PhaseStemElongation = 0;
    }
    // std::cout << "PhaseStemElongation = " << PhaseStemElongation << std::endl;
    // std::cout << "HaunIndex = " << HaunIndex << std::endl;
}

void Model::evalCstrAssim()
{
    CstrAssim = std::pow(std::max(cstr, 0.00000001), ASScstr);
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
}

void Model::evolPlantTilNumTot()
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
                TilNewPlant = cstr * std::min(std::max(0., (Ic - IcTillering) *
                                                       TilAbility) *
                                              std::sqrt(LtrKdfcl), 2.);
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

void Model::evolPlantLeafNumTot()
{
    if (NumPhase > 1 and NumPhase < 5) {
        PlantLeafNumNew = HaunGain * CulmsPerHill;
        PlantLeafNumTot = PlantLeafNumTot + PlantLeafNumNew;
    } else {
        PlantLeafNumNew = PlantLeafNumNew;
        PlantLeafNumTot = PlantLeafNumTot;
    }
}

void Model::evalRelPotLeafLength()
{
    if (NumPhase > 1) {
        RelPotLeafLength = std::min((0.1 + 0.9 * HaunIndex / RankLongestLeaf),
                                    1.);
    }
}

void Model::evalDemandStructSheath()
{
    if (NumPhase > 1 and NumPhase < 5) {
        DemStructSheathPop = (1 + ((SlaMax - Sla) / (SlaMax - SlaMin))) * 0.5 *
            DemStructLeafPop / WtRatioLeafSheath *
            std::max(0.00001, StressCold);
    }
}

void Model::evalDemandStructRoot()
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

void Model::evalDemandStructPanicle()
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
}

void Model::evolConsRes_Flood()
{
    double TrSurf;
    double WaterDeficit;

    TrSurf = 0;
    StockSurface = ValRFE + ValRDE;
    if (FloodwaterDepth + StockMacropores == 0 or NumPhase == 0) {
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
        std::cout << "PPCrit = " << PPCrit << std::endl;*/




        double SDJPSP;

        if (NumPhase == 3) {
            if (ChangePhase == 1) {
                SumPP = 100;
                SDJPSP = std::max(0.01, DegresDuJourCor);
                /*std::cout << "Ligne 1" << std::endl;
                std::cout << "SumPP = " << SumPP << std::endl;
                std::cout << "SDJPSP = " << SDJPSP << std::endl;*/
            } else {
                SDJPSP = SommeDegresJourCor - SeuilTempPhasePrec +
                    std::max(0.01, DegresDuJourCor);
                /*std::cout << "Ligne 2" << std::endl;
                  std::cout << "SDJPSP = " << SDJPSP << std::endl;*/
            }

            /*std::cout << "std::pow((1000 / SDJPSP), PPExp) = " << std::pow((1000 / SDJPSP), PPExp) << std::endl;
            std::cout << "DureeDuJour - PPCrit =" << DayLength - PPCrit << std::endl;
            std::cout <<  "std::max(0., (DureeDuJour - PPCrit)) = " << std::max(0., (DureeDuJour - PPCrit)) << std::endl;*/



            SumPP = std::pow((1000 / SDJPSP), PPExp) *
                std::max(0., (DayLength - (double) PPCrit)) / (SeuilPP - PPCrit);
            SeuilTempPhaseSuivante = SeuilTempPhasePrec + SDJPSP;
        }

        /*std::cout << "-----" << std::endl;
        std::cout << "APRES" << std::endl;
        std::cout << "-----" << std::endl;
        std::cout << "SDJPSP = " << SDJPSP << std::endl;
        std::cout << "SumPP = " << SumPP << std::endl;*/


    }
}


void Model::evolSomDegresJourCor(double t)
{
    if (t >= DateSemis) {
        if (NumPhase >= 1) {
            SommeDegresJourCor = SommeDegresJourCor + DegresDuJourCor;
        } else {
            SommeDegresJourCor = 0;
        }
    }
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
    switch ((int)(NumPhase))
    {
    case 1: VitesseRacinaire = VRacLevee; break;
    case 2: VitesseRacinaire = VRacBVP; break;
    case 3: VitesseRacinaire = VRacPSP; break;
    case 4: VitesseRacinaire = VRacRPR; break;
    case 5: VitesseRacinaire = VRacMatu1; break;
    case 6: VitesseRacinaire = VRacMatu2; break;
    default: VitesseRacinaire = 0;
    }
    VitesseRacinaireDay = VitesseRacinaire * DegreDuJourCor *
        std::pow(cstr, RootCstr);
}

void Model::evalRootFront()
{
    if (NumPhase > 0) {
        RootFront = ((1 / ResUtil) * RuRac) * 1000;
    }
}

void Model::evalSDJPhase4()
{
    if (NumPhase == 4) {
        SDJPhase4 = SDJPhase4 + DegreDuJourCor;
    }
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
    if (NumPhase == 5 and ChangePhase == 1) {
        SterilityCold =
            std::max(0.,
                     (std::min(1.,
                               KCritSterCold1 / (KCritSterCold1 -
                                                 KCritSterCold2) - TMinMoy /
                               (KCritSterCold1 - KCritSterCold2))));
        SterilityHeat =
            1. - std::max(0.,
                          (std::min(1., KCritSterHeat2 /
                                    (KCritSterHeat2 -
                                     KCritSterHeat1) - TMaxMoy /
                                    (KCritSterHeat2 - KCritSterHeat1))));
        SterilityDrought =
            std::max(0.,
                     (std::min(1., KCritSterFtsw1 / (KCritSterFtsw1 -
                                                     KCritSterFtsw2) -
                               FtswMoy / (KCritSterFtsw1 - KCritSterFtsw2))));
    } else {
        SterilityCold = SterilityCold;
        SterilityHeat = SterilityHeat;
        SterilityDrought = SterilityDrought;
    }
    SterilityTot = std::min(0.999, 1 - ((1 - SterilityCold) *
                                        (1 - SterilityHeat) *
                                        (1 - SterilityDrought)));
}

void Model::initParcelle()
{
    double Stockini2;
    double Stockini1;

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
    EvapPot = Kce * ETP;
}

void Model::transplanting()
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

void Model::transplantingShock()
{
    if (CounterNursery > 0 and CounterNursery < 8) {
        Assim = Assim * CoeffTransplantingShock;
    } else {
        Assim = Assim;
    }
}

void Model::evalRuiss_FloodDyna()
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
        FloodWaterDepth = 0;
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

void Model::evolRempliResRFE_RDE()
{
    double EauReste;
    double ValRSurfPrec;
    double EauTranspi;

    Dr = 0;
    EauTranspi = 0;
    if (StockMacropores + FloodwaterDepth == 0) {
        EauReste = 0;
        ValRFE = ValRFE + EauDispo;
        if (ValRFE > CapaRFE) {
            EauReste = ValRFE - CapaRFE;
            ValRFE = CapaRFE;
        }
        ValRSurfPrec = ValRSurf;
        ValRSurf = ValRSurf + EauReste;
        if (ValRSurfPrec < CapaREvap) {
            EauTranspi = EauDispo - (std::min(CapaREvap, ValRSurf) -
                                     ValRSurfPrec);
        } else {
            EauTranspi = EauDispo;
        }
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
        StockTotal = StockTotal + EauTranspi;
        if (StockTotal > StRuMax) {
            Dr = StockTotal - StRuMax;
            StockTotal = StRuMax;
        } else {
            Dr = 0;
        }
        if (Hum < CapaRFE + CapaRDE) {
            Hum = StockSurface;
        } else {
            Hum = std::max(Hum, StockTotal);
        }
    }
    StockRac = std::min(StockRac + EauTranspi, RuRac);
    // Shifting non-percolating Dr back to macropores & floodwater if plot is bunded
    if (BundHeight > 0) {
        // Shifting non-percolating Dr to Floodwater
        StockMacropores = StockMacropores + std::max(0., Dr - PercolationMax);
        Dr = std::min(Dr, PercolationMax);
        if (StockMacropores > VolMacropores) {
            FloodWaterDepth = FloodWaterDepth +
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

void Model::plantSubmergence()
{
    FractionPlantHeightSubmer = std::min(
        std::max(0., FloodwaterDepth / std::max(PlantHeight, 0.1)), 1.);
}

void Model::excessAssimilToRoot()
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

void Model::evolRempliMacropores()
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

void Model::evalFTSW()
{
    StockRac = std::min(StockRac, (RuRac +
                                   (StockMacropores * RuRac / StRuMax)));
    StockRac = std::min(StockRac, StockTotal);
    if (RuRac > 0) {
        FTSW = StockRac / RuRac;
    } else {
        FTSW = 0;
    }
}

void Model::evalDAF()
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
    // std::cout << "StressCold = " << StressCold << std::endl;
}

void Model::evalAssim()
{
    Assim = std::max(AssimPot * CstrAssim, 0.);
}

void Model::evolWaterLoggingUpland()
{
    if (Dr > PercolationMax and BundHeight == 0) {
        StockMacropores = StockMacropores + (Dr - PercolationMax);
        Lr = Lr + std::max(0., StockMacropores - VolMacropores);
        Dr = PercolationMax;
        StockMacropores = std::min(StockMacropores, VolMacropores);
    }
}

void Model::evalStressWaterLogging()
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

void Model::evalCstrPFactorFAO()
{
    double pFact;

    pFact = PFactor + 0.04 * (5 - KcTot * Eto);
    pFact = std::max(0., pFact);
    pFact = std::min(0.8, pFact);
    cstr = std::min((FTSW / (1 - pFact)), 1.);
    cstr = std::max(0., cstr);
    if (StockMacropores > 0) {
        cstr = cstr * CoeffStressLogging;
    }
}

void Model::evolHauteur_SDJ_cstr()
{
  double CorrectedCstrMean;

    if (PhaseStemElongation == 1) {
        ApexHeightGain = HaunGain * std::min(std::pow(std::min(Ic, 1.), 0.5),
                                             cstr) * StressCold
            * InternodeLengthMax;
        ApexHeightGain = ApexHeightGain * CoeffInternodeNum;
    } else {
        ApexHeightGain = 0;
    }
    ApexHeight = ApexHeight + ApexHeightGain;
    if (CstrMean <= 0) {
        CorrectedCstrMean = 1;
    } else {
        CorrectedCstrMean = CstrMean;
    }
    PlantHeight = ApexHeight + (1.5 * (1 - Kdf) * RelPotLeafLength *
                                LeafLengthMax * std::sqrt(IcMean) *
                                CorrectedCstrMean * (1 + 1 /
                                                     WtRatioLeafSheath));
    PlantWidth = std::pow(Kdf, 1.5) * 2 * std::sqrt(IcMean) * RelPotLeafLength *
        LeafLengthMax;
}

void Model::evalParIntercepte()
{
    if (Lai > 0 and LIRkdfcl == 0) {
        LIRkdfcl = (1 - std::exp(-Kdf * Lai));
    }
    PARIntercepte = PAR * LIRkdfcl;
}

void Model::evolMobiliTillerDeath()
{
    if (NumPhase == 3 or (NumPhase == 4 and SDJCorPhase4 <= 0.7 * SDJRPR
                          and CulmsPerPlant >= 1)) {
        TillerDeathPop = (1 - (std::min(Ic, 1.))) * CulmsPop * CoeffTillerDeath;
        CulmsPop = CulmsPop - TillerDeathPop;
        CulmsPerPlant = CulmsPop / (Density * PlantsPerHill);
        CulmsPerHill = CulmsPerPlant * PlantsPerHill;
        DryMatStructPaniclePop = DryMatStructPaniclePop *
            std::max(0., CulmsPop) / (CulmsPop + TillerDeathPop);
    }
}

void Model::evolMobiliLeafDeath()
{
    if (NumPhase > 1) {
        LeafDeathPop = (1 - (std::min(Ic, 1.))) * DryMatStructLeafPop *
            CoeffLeafDeath;
        DryMatStructLeafPop = DryMatStructLeafPop - LeafDeathPop;
        MobiliLeafDeath = 0.25 * LeafDeathPop;
        DeadLeafDrywtPop = DeadLeafDrywtPop + (0.75 * LeafDeathPop);
        LaiDead = DeadLeafDrywtPop * Sla;
    }
}

void Model::evalSupplyTot()
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
}

void Model::evalDemandStructLeaf()
{
  double CorrectedSla;

    if (NumPhase > 1 and NumPhase < 5) {
        DemLeafAreaPlant = (std::pow((RelPotLeafLength * LeafLengthMax), 2) *
                            CoeffLeafWLRatio * 0.725 * PlantLeafNumNew /
                            1000000) * std::min(cstr, StressCold);
        if (SlaNew == 0) {
            CorrectedSla = SlaMax;
        } else {
            CorrectedSla = SlaNew;
        }
        DemStructLeafPlant = DemLeafAreaPlant * 0.1 / CorrectedSla;
        DemStructLeafPop = DemStructLeafPlant * Density / 1000;
        A_DemStructLeaf = DemStructLeafPlant * Density / 1000;
    }
}

void Model::evalDemandStructIN()
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

void Model::evalDemandTotAndIcPreFlow()
{
    if (NumPhase > 1 and NumPhase < 5) {
        DemStructTotPop = DemStructLeafPop + DemStructSheathPop +
            DemStructRootPop + DemStructInternodePop +
            DemStructPaniclePop + DemResInternodePop;
        A_DemStructTot = DemStructLeafPop + DemStructSheathPop +
            DemStructRootPop + DemStructInternodePop +
            DemStructPaniclePop + DemResInternodePop;
        Ic = SupplyTot / DemStructTotPop;
        if (Ic <= 0) {
            Ic = 0;
        }
      if (PlantHeight == 0) {
          Ic = 1;
      }
      IcCumul = IcCumul + std::min(Ic, 1.);
      IcMean = IcCumul / NbDaysSinceGermination;
      CstrCumul = CstrCumul + cstr;
      CstrMean = CstrCumul / NbDaysSinceGermination;
    }
    if (NumPhase == 5 or NumPhase == 6) {
        IcCumul = IcCumul + std::min(Ic, 1.);
        IcMean = IcCumul / NbDaysSinceGermination;
        CstrCumul = CstrCumul + cstr;
        CstrMean = CstrCumul / NbDaysSinceGermination;
    }
}

void Model::evolGrowthStructLeafPop()
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

void Model::evolGrowthStructINPop()
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

void Model::addResToGrowthStructPop()
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

void Model::evolDemPanFilPopAndIcPFlow()
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

void Model::evolPanicleFilPop()
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

void Model::evolGrowthReserveInternode()
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

void Model::evolGrowthTot()
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

void Model::evalLai()
{
    double CorrectedSla;

    if (NumPhase == 2 and ChangePhase == 1) {
        CorrectedSla = SlaMax;
    } else {
        CorrectedSla = Sla;
        LastLeafLengthPot = RelPotLeafLength * LeafLengthMax;
        if (GrowthStructTotPop > 0) {
            LastLeafLength = LastLeafLengthPot *
                std::sqrt(GrowthStructLeafPop / DemStructLeafPop);
        }
    }
    Lai = DryMatStructLeafPop * CorrectedSla;
}

void Model::evalClumpAndLightInter()
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
    }
}

void Model::evalRUE()
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
        if (Tr > 0 and NbJas > 20 and NumPhase > 1) {
            TrEffInst = (SupplyTot - AssimNotUsed) / (Tr * 10000);
            TrEff = DryMatTotPop / (CumTr * 10000);
            WueEt = DryMatTotPop / (CumEt * 10000);
            WueTot = DryMatTotPop / (CumWuse * 10000);
            RUE = ( DryMatAboveGroundTotPop / std::max(CumPar, 0.00001)) / 10;
        }
    }
}

void Model::evolEvapSurfRFE_RDE()
{
    double ValRSurfPrec;
    double EvapRU;
    double Evap1;
    double Evap2;

    if (StockMacropores + FloodwaterDepth == 0) {
        ValRSurfPrec = ValRSurf;
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
        }
        Evap = Evap1 + Evap2;
        ValRFE = ValRFE - Evap1;
        ValRSurf = ValRSurf - Evap2;
        ValRDE = std::max(0., ValRSurf - CapaREvap);
        if (EvapPot == 0) {
            Kr = 0;
        } else {
            Kr = Evap / EvapPot;
        }
        if (ValRSurf >= CapaREvap) {
            EvapRU = Evap;
        } else {
            if (ValRSurfPrec <= CapaREvap) {
                EvapRU = Evap1;
            } else {
                EvapRU = Evap1 + ValRSurfPrec - CapaREvap;
            }
        }
        if (RuRac <= RuSurf) {
            StockRac = std::max(0., StockRac - EvapRU * RuRac / RuSurf);
        } else {
            StockRac = std::max(0., StockRac - EvapRU);
        }
        StockTotal = StockTotal - EvapRU;
        StockRac = std::min(StockRac, StockTotal);
        KceReal = Kce * Kr;
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

void Model::evolDryMatTot()
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
    if (NumPhase == 2 and ChangePhase == 1) {
        DryMatTotPop = Density * PlantsPerHill * TxResGrain * PoidsSecGrain /
            1000;
        DryMatStructLeafPop = DryMatTotPop * 0.5;
    } else {
        if (NumPhase > 1) {
            DryMatStructLeafPop = DryMatStructLeafPop + GrowthStructLeafPop;
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
                DryMatStructRootPop + DeadLeafDryWtPop;
            DryMatPanicleTotPop = DryMatStructPaniclePop + GrainYieldPop;
            DryMatTotPop = DryMatVegeTotPop + DryMatPanicleTotPop;
            DryMatAboveGroundPop = DryMatTotPop - DryMatStructRootPop -
                DeadLeafDryWtPop;
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

void Model::evolGrowthStructTot()
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

void Model::leafRolling()
{
    if (NumPhase > 1) {
        KRolling = RollingBase + (1 - RollingBase) *
            std::pow(FTSW,  std::max(0.0000001, Eto * RollingSens));
        if (KRolling > 1) {
            KRolling = 1;
        }
    }
}

void Model::evalAssimPot()
{
    if (-CO2Exp != 0 and CO2Cp != 0) {
        CoeffCO2Assim = (1 - std::exp(-CO2Exp * (Ca - CO2Cp))) /
            (1 - std::exp(-CO2Exp * (400 - CO2Cp)));
    }
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

void Model::automaticIrrigation()
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
            IrrigAutoDay = (IrrigAutoTarget * BundHeight) + VolMacropores +
                RuSurf + PercolationMax;
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

void Model::evolRurRFE_RDE()
{
  double DeltaRur;

    if (NumPhase == 0) {
        RuRac = 0;
        StockRac = 0;
    } else {
        if (NumPhase == 1 and ChangePhase == 1) {
            RuRac = ResUtil * std::min(ProfRacIni,
                                       (EpaisseurSurf + EpaisseurProf)) / 1000;
            if (ProfRacIni <= EpaisseurSurf) {
                StockRac = (ValRDE + ValRFE) * ProfRacIni / EpaisseurSurf;
            } else {
                StockRac = StockTotal *
                    std::min(ProfRacIni /  (EpaisseurSurf + EpaisseurProf), 1.);
            }
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
            RuRac = RuRac + DeltaRur;
            if (RuRac > RuSurf) {
                StockRac = StockRac + DeltaRur;
            } else {
                StockRac = (ValRDE + ValRFE) * (RuRac / RuSurf);
            }
        }
    }
    if (NumPhase != 0) {
        RootFront = ((1 / ResUtil) * RuRac) * 1000;
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
    if (StockMacropores + FloodwaterDepth > 0) {
        StockRac = RootFront * ResUtil / 1000 +
            (RootFront / (EpaisseurSurf + EpaisseurProf)) * StockMacropores;
        StockRac = std::min(StockRac, StockTotal);
    }
}

void Model::evalSimEndCycle()

{
    if (NumPhase == 7 and ChangePhase == 1) {
        SimEndCycle = NbJas;
    }
}

void Model::priority2GrowthPanStrctPop()
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

void Model::keyResults()
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
        CumCstrPhase2 = CumCstrPhase2 + cstr;
        CumFtswPhase2 = CumFtswPhase2 + FTSW;
        CumIcPhase2 = CumIcPhase2 + Ic;
    }
    if (NumPhase == 3) {
        DurPhase3 = DurPhase3 + 1;
        CumCstrPhase3 = CumCstrPhase3 + cstr;
        CumFtswPhase3 = CumFtswPhase3 + FTSW;
        CumIcPhase3 = CumIcPhase3 + Ic;
    }
    if (NumPhase == 4) {
        DurPhase4 = DurPhase4 + 1;
        CumCstrPhase4 = CumCstrPhase4 + cstr;
        CumFtswPhase4 = CumFtswPhase4 + FTSW;
        CumIcPhase4 = CumIcPhase4 + Ic;
    }
    if (NumPhase == 5) {
        DurPhase5 = DurPhase5 + 1;
        CumCstrPhase5 = CumCstrPhase5 + cstr;
        CumFtswPhase5 = CumFtswPhase5 + FTSW;
        CumIcPhase5 = CumIcPhase5 + Ic;
    }
    if (NumPhase == 6) {
        DurPhase6 = DurPhase6 + 1;
        CumCstrPhase6 = CumCstrPhase6 + cstr;
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
    if (RGLue == NullValue) {
        RGCalc = (0.25 + 0.50 * std::min(Ins / DayLength, 1.)) * RayExtra;
    } else {
        RGCalc = RGLue;
    }
    // std::cout << "RGCalc = " << RGCalc << std::endl;
}

void Model::evalPAR()
{
    PAR = KPar * Rg;
    // std::cout << "PAR = " << PAR << std::endl;
}

void Model::etoFAO()
{
    double eActual;
    double eSat;
    double RgRgMax;
    double TLat;
    double delta;
    double KPsy;
    double Eaero;
    double Erad;
    double Rn;
    double G;

    if (ETP == NullValue) {
        eSat = 0.3054 * (std::exp(17.27 * TMax / (TMax + 237.3)) +
                         std::exp(17.27 * TMin / (TMin + 237.3)));
        if (HrMax == NullValue) {
            eActual = eSat * HrMoy / 100;
        } else {
            eActual = 0.3054 * (std::exp(17.27 * TMax / (TMax + 237.3)) *
                                HrMin / 100 + std::exp(17.27 * TMin /
                                                       (TMin + 237.3)) *
                                HrMax / 100);
        }
        VPD = eSat - eActual;
        RgRgMax = RayGlobal / RgMax;
        if (RgRgMax > 1) {
            RgRgMax = 1;
        }
        Rn = 0.77 * RayGlobal - (1.35 * RgRgMax - 0.35) *
            (0.34 - 0.14 * std::pow(eActual, 0.5)) *
            (std::pow(TMax + 273.16, 4) + std::pow(TMin + 273.16, 4)) *
            2.45015 * std::pow(10, -9);
        Tlat = 2.501 - 2.361 * std::pow(10, -3) * TMoy;
        delta = 4098 * (0.6108 * std::exp(17.27 * TMoy / (TMoy + 237.3))) /
            std::pow(TMoy + 237.3, 2);
        Kpsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Altitude / 293),
                                          5.26) / TLat;
        G = 0.38 * (TMoy - TMoyPrec);
        Erad = 0.408 * (Rn - G) * delta / (delta + Kpsy * (1 + 0.34 * Vent));
        Eaero = (900 / (TMoy + 273.16)) * ((eSat - eActual) * Vent) * Kpsy /
            (delta + Kpsy * (1 + 0.34 * Vent));
        Eto = Erad + Eaero;
    } else {
        Eto = ETP;
    }
    TMoyPrec = TMoy;
    // std::cout << "Eto = " << Eto << std::endl;
}

void Model::evolPhenoStress(double t)
{
    if (t >= DateSemis) {

        bool ChangementDePhase = false;
        bool ChangementDeSousPhase = false;

        std::cout << "-----" << std::endl;
        std::cout << "DEBUT" << std::endl;
        std::cout << "-----" << std::endl;
        std::cout << "NumPhase = " << NumPhase << std::endl;
        std::cout << "StockSurface = " << StockSurface << std::endl;
        std::cout << "PourcRuSurfGermi = " << PourcRuSurfGermi << std::endl;
        std::cout << "RuSurf = " << RuSurf << std::endl;
        std::cout << "StRu = " << StRu << std::endl;


        ChangePhase = 0;
        ChangeSousPhase = 0;
        if ((int)(NumPhase) == 0) {
            if (StockSurface >= PourcRuSurfGermi * RuSurf or StRu > StockSurface) {
                NumPhase = 1;
                ChangePhase = 1;
                SeuilTempPhaseSuivante = SDJLevee;

                std::cout << "Ligne 1" << std::endl;

            }
        } else {
            if ((int)(NumPhase) == 2 and
                SommeDegresJour >= SeuilTempPhaseSuivante) {
                ChangementDePhase = true;

                std::cout << "Ligne 2" << std::endl;

            } else {
                if ((int)(NumPhase) != 3) {
                    ChangementDePhase = (SommeDegresJour >= SeuilTempPhaseSuivante);

                    std::cout << "Ligne 3" << std::endl;

                } else {
                    ChangementDePhase = (SumPP >= SeuilPP);

                    std::cout << "SumPP = " << SumPP << std::endl;
                    std::cout << "SeuilPP = " << SeuilPP << std::endl;
                    std::cout << "Ligne 4" << std::endl;

                }
            }
        }
        if (ChangementDePhase) {

            std::cout << "Ligne 5" << std::endl;

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
            if ((int)(NumPhase) == 4) {
                ChangementDeSousPhase = (SommeDegresJour >=
                                         SeuilTempSousPhaseSuivante);
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
        std::cout << "---" << std::endl;
        std::cout << "FIN" << std::endl;
        std::cout << "---" << std::endl;
        std::cout << "NumPhase = " << NumPhase << std::endl;
    } else {
        NumPhase = 0;
    }
}



void Model::demandePlante()
{
    TrPot = Kcp * Eto;
    CoeffCO2Tr = Ca * CO2SlopeTr - 400 * CO2SlopeTr + 1;
    TrPot = TrPot * CoeffCO2Tr;
}

void Model::evalTranspi()
{
    Tr = TrPot * cstr;
}

void Model::evalETRETM()
{
    ETM = Evap + TrPot;
    ETR = Evap + Tr;
}

void Model::evolSomDegresJour()
{
    if (NumPhase >= 1) {
        SommeDegresJour = SommeDegresJour + DegresDuJour;
    } else {
        SommeDegresJour = 0;
    }
}

void Model::mortality()
{
    double MoyenneCstr;

    if (NumPhase >= 2) {
        NbJourCompte = NbJourCompte + 1;
        if (tabCstrIndiceCourant == 5) {
            tabCstrIndiceCourant = 1;
            tabCstr[tabCstrIndiceCourant] = cstr;
        } else {
            tabCstrIndiceCourant = tabCstrIndiceCourant + 1;
            tabCstr[tabCstrIndiceCourant] = cstr;
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
  if (NumPhase == 4 and NumSousPhase == 4) {
    TMaxMoy = calculeLaMoyenne(TMax, MonCompteur, TMaxMoy);
  } else if (NumPhase < 4) {
    TMaxMoy = 0;
  }
}

void Model::evalTMinMoy() {
  if (NumPhase == 4 and NumSousPhase == 3) {
    TMinMoy = calculeLaMoyenne(TMin, MonCompteur, TMinMoy);
  } else if (NumPhase < 4) {
    TMinMoy = 0;
  }
}

void Model::evalFtswMoy() {
  if (NumPhase == 4 and NumSousPhase == 4) {
    FtswMoy = calculeLaMoyenne(FTSW, MonCompteur, FtswMoy);
  } else if (NumPhase < 4) {
    FtswMoy = 0;
  }
}

double Model::calculeLaMoyenne(double laValeur, double leCompteur, double laMoyenne) {
  double moyenne;

  moyenne = (laMoyenne * (leCompteur - 1) + laValeur) / leCompteur;
  return moyenne;
}

} } } // namespace model models samara
