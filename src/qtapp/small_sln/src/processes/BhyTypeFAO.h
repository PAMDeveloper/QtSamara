#ifndef INCLUDED_BHYTYPEFAO_H
#define INCLUDED_BHYTYPEFAO_H

#include <samara_defines.h>
using namespace std;

namespace BhyTypeFAO {

//--------------------------------------------------------
//Procédure d'initialisation de la parcelle
//-----------------------------------------------------------
void InitParcelle(double const &StockIniSurf, double const &StockIniProf, double const &Ru, double const &EpaisseurSurf, double const &EpaisseurProf, double const &HumPF, double const &PEvap, double const &DateSemis,
                  double &StTot, double &Ltr, double &Hum, double &RuSurf, double &ProfRU, double &StRuMax, double &CapaREvap, double &CapaRFE, double &CapaRDE, double &ValRSurf, double &ValRDE, double &ValRFE, double &StockSurface) {
  double Stockini2;
  double Stockini1;

  try {
    //MainForm.memDeroulement.Lines.Add('----Procedure InitParcelle----');
// épaisseurs
    ProfRU = EpaisseurSurf + EpaisseurProf; // à supprimer ultérieurement dans les différents modules
// capacité des réservoirs
    RuSurf = Ru * EpaisseurSurf * 1.0 / 1000;
    CapaREvap = 0.5 * EpaisseurSurf * HumPF;
    CapaRFE = PEvap * (CapaREvap + RuSurf);
    CapaRDE = RuSurf - CapaRFE;
    StRuMax = Ru * ProfRU * 1.0 / 1000;
// bornage des stockIni
    Stockini1 = std::min(StockIniSurf, CapaREvap + RuSurf);
    Stockini2 = std::min(StockIniProf, Ru * EpaisseurProf * 1.0 / 1000);
// Contenu des réservoirs
    ValRSurf = std::min(Stockini1, CapaREvap + CapaRDE);
    ValRDE = std::max(0., ValRSurf - CapaREvap);
    ValRFE = std::max(0., Stockini1 - (CapaREvap + CapaRDE));
    StockSurface = ValRDE + ValRFE;
    StTot = StockSurface + Stockini2;//transpirable
    Hum = StTot;
// initialisation marquant l'absence de culture
    Ltr = 1;


  } catch (...) {
    error_message("InitParcelle", UBhyTypeFAO);
  }
}
/*------------------------------------------------*/

//##############################################################################
//        Calcul des degrés jours du jour
//##############################################################################


void EvalDJOpt(double const &TMoy, double const &TBase, double const &TOpt,
               double &DJ)

{
//DJ:=std::max(std::min(TOpt,TMoy),TBase)-Tbase;
}

//##############################################################################
//        Calcul de la hauteur du couvert en fonction de DJ et cstr
//##############################################################################
void EvolHauteur_SDJ_cstr(double const &Phase,
                          double const &HautMax, double const &Hautmin, double const &SDJMid, double const &SDJ, double const &SDJIni, double const &DJ, double const &cstr, double const &SeuilTempLevee,
                          double &Hauteur)

{
  double dayHauteur;
// Calcule la variation journalière de hauteur en fonction de DJ (du jour) et de cstr
//(en phase de croissance rapide). Pas d'effet de cstr sur croissance lente de début



  if (Phase < 2)
    Hauteur = 0;
  else if (SDJ < SDJIni + SeuilTempLevee) {
    dayHauteur = Hautmin * DJ * 1.0 / SDJIni;
    Hauteur = Hauteur + dayHauteur;
  } else {
    dayHauteur = ((HautMax - Hautmin) * 1.0 / (SDJMid - SDJIni)) * DJ * cstr;
    Hauteur = std::min(HautMax, Hauteur + dayHauteur);
  }
}
//##############################################################################
//  Procédure alternative de la précédente sans cstr
//##############################################################################

void EvolHauteur_SDJ(double const &Phase,
                     double const &HautMax, double const &Hautmin, double const &SDJMid, double const &SDJ, double const &SDJIni, double const &DJ, double const &SeuilTempGermi,
                     double &Hauteur)

{
  double dayHauteur;
// Calcule la variation journalière de hauteur en fonction de DJ (du jour)


  try {
    if (Phase < 2)
      Hauteur = 0;
    else if (SDJ < SDJIni + SeuilTempGermi) {
      dayHauteur = Hautmin * DJ * 1.0 / SDJIni;
      Hauteur = Hauteur + dayHauteur;
    } else {
      dayHauteur = ((HautMax - Hautmin) * 1.0 / (SDJMid - SDJIni)) * DJ;
      Hauteur = std::min(HautMax, Hauteur + dayHauteur);
    }

  } catch (...) {
    error_message("EvolHauteur_SDJ", UBhyTypeFAO);
  }
}

//##############################################################################
// Calcul de Kcp FAO basé sur somme de degrés jours (était dans arachide)
//##############################################################################

void EvolKcpFaoSDJ(double const &Phase,
                   double const &SDJlevee, double const &KcMax, double const &KcIni, double const &KcEnd, double const &SDJMid, double const &SDJEnd, double const &SDJIni,
                   double &SDJMax, double &SDJ, double &Kcp)

{
  try {
    SDJ = SDJ - SDJlevee;//JCC 18/04/05 les sommes de DJ pour Kcp débutent à la levée
    SDJMax = SDJMax - SDJlevee;
    if (Phase <= 1)
      Kcp = 0;
    // jcc avril 2004 0 remplacé par 0.001;
    else if (SDJ < SDJIni) {
      Kcp = KcIni;
    } else {
      if (SDJ < SDJEnd)
        Kcp = std::min(KcMax, KcIni + (KcMax - KcIni) * (SDJ - SDJIni) * 1.0 / (SDJMid - SDJIni));
      else
        Kcp = std::min(KcMax, KcMax - (KcMax - KcEnd) * (SDJ - SDJEnd) * 1.0 / (SDJMax - SDJEnd));
    }

  } catch (...) {
    error_message("EvolKcpFaoSDJ", UBhyTypeFAO);
  }
}

//#############################################################################
// Procédure alternative avec DJ et cstr
//##############################################################################
void EvolKcpFaoSDJ_cstr(double const &Phase,
                        double const &SDJlevee, double const &KcMax, double const &KcIni, double const &KcEnd, double const &SDJMid, double const &SDJEnd, double const &SDJIni, double const &DJ, double const &cstr,
                        double &SDJMax, double &SDJ, double &Kcp)

{
  double DayKcp;
// Calcule la variation journalière de Kcp en fonction de DJ (du jour)
//et de cstr (en phase de croissance rapide)


  SDJ = SDJ - SDJlevee;//JCC 18/04/05 les sommes de DJ pour Kcp débutent à la levée
  SDJMax = SDJMax - SDJlevee;
  DayKcp = 0;
  if (Phase <= 1)
    Kcp = 0;
  // jcc avril 2004 0 remplacé par 0.001;else
  if (SDJ < SDJIni)
    Kcp = KcIni;
  else {
    if (SDJ < SDJEnd)
      DayKcp = ((KcMax - KcIni) * 1.0 / (SDJMid - SDJIni)) * DJ * cstr;
    else
      DayKcp = (KcEnd - KcMax) * DJ * 1.0 / (SDJMax - SDJEnd);
  }
  Kcp = std::min(KcMax, Kcp + DayKcp);
  //TO DO :vérifier si correct
}

//##############################################################################
//          Calcul de Kce potentiel selon les procédures FAO 56
//##############################################################################

void EvalKcePotFAO(double const &Mulch, double const &vent, double const &HRmin, double const &Kcp, double const &Hauteur,
                   double &Kce) {
  double Kborne;
// avec cette procédure fixer Mulch = 120

  try {
    if (Hauteur == 0)
      //Kce:=std::max(0.12,1.2+0.04*(vent-2)-0.004*(HRmin-45))    18/04/05
      Kce = std::max(0.12, Mulch * 1.0 / 100 + 0.04 * (vent - 2) - 0.004 * (HRmin - 45));
    else {
      //Kborne:=std::max(Kcp+0.05,1.2+(0.04*(vent-2)-0.004*(HRmin-45))*power(0.3,Hauteur/3));    18/04/05
      Kborne = std::max(Kcp + 0.05, Mulch * 1.0 / 100 + (0.04 * (vent - 2) - 0.004 * (HRmin - 45)) * std::pow(Hauteur * 1.0 / 3, 0.3));
      Kce = Kborne - Kcp;
    }

  } catch (...) {
    error_message("EvalKcePotFAO", UBhyTypeFAO);
  }
}

//##############################################################################
//          Calcul de l'évaporation potentielle (si couche de surface saturée)
//##############################################################################

void EvalEvapPot(double const &Etp, double const &Kce,
                 double &EvapPot) {
  try {
    EvapPot = Kce * Etp;

  } catch (...) {
    error_message("EvalEvapPot", UBhyTypeFAO);
  }
}

//##############################################################################
//        Calcul de l'évaporation réelle avec modèle à remplissage prioritaire
//    de la réserve facilement évaporable (RFE).
//
//##############################################################################

//Les capacités de réservoirs sont calculés comme dans FAO 56 ainsi que la décroissance de
//l'évaporation lorsque RFE est épuisée

void EvolEvapSurfRFE_RDE(double const &EvapPot, double const &CapaREvap, double const &CapaRDE, double const &StRurMax, double const &RuSurf,
                         double &Evap, double &ValRSurf, double &ValRFE, double &ValRDE, double &StRur, double &StRu, double &Kr) {
  double ValRSurfPrec; double EvapRU;
  double Evap1; double Evap2;

  try {
    ValRSurfPrec = ValRSurf;
    // ValRSurf est l'eau contenue dans les réservoirs Revap (non transpirable) et RDE (transpirable et évaporable
    if (ValRFE > 0) {
      if (ValRFE < EvapPot) {
        Evap1 = ValRFE;
        Evap2 = std::max(0., std::min(ValRSurf, ((EvapPot - ValRFE) * ValRSurf) * 1.0 / (CapaREvap + CapaRDE)));   // borné à 0 et ValRSurf le 27/04/05
      } else {
        Evap1 = EvapPot;
        Evap2 = 0;
      }
    } else {
      Evap1 = 0;
      Evap2 = std::max(0., std::min(ValRSurf, EvapPot * ValRSurf * 1.0 / (CapaREvap + CapaRDE))); // borné à 0 et ValRSurf le 27/04/05
    }

    Evap = Evap1 + Evap2;
    ValRFE = ValRFE - Evap1;
    ValRSurf = ValRSurf - Evap2;
    ValRDE = std::max(0., ValRSurf - CapaREvap);
    if (EvapPot == 0)
      Kr = 0;
    else
      Kr = Evap * 1.0 / EvapPot;

    // part de l'évaporation prélevée dans les réservoirs RFE et RDE
    if (ValRSurf >= CapaREvap)
      EvapRU = Evap;
    else {
      if (ValRSurfPrec <= CapaREvap)
        EvapRU = Evap1;
      else
        EvapRU = Evap1 + ValRSurfPrec - CapaREvap;
    }

    //Evaporation de Ru et Rur, MAJ
    if (StRurMax <= RuSurf)
      // quand les racines n'ont pas dépassé la première couche
      StRur = std::max(0., StRur - EvapRU * StRurMax * 1.0 / RuSurf);
    else
      StRur = std::max(0., StRur - EvapRU);

    StRu = StRu - EvapRU;

  } catch (...) {
    error_message("EvalEvapPot", UBhyTypeFAO);
  }
}

//###############################################################################
//             calcul du Kc FAO = Kce*Kr+Kcp
//###############################################################################

void EvalKcFAO(double const &Kce, double const &Kr, double const &Kcp,
               double &Kctot)

{
  try {
    Kctot = Kce * Kr + Kcp;

  } catch (...) {
    error_message("EvalKcFAO", UBhyTypeFAO);
  }
}

//##############################################################################
//               calcul de FTSW pour mémoire dans BilEau
//###############################################################################

//##############################################################################
//               calcul de cstrpfactorFAO
//##############################################################################

void EvalCstrPFactorFAO(double const &PFactor, double const &FTSW, double const &ETo, double const &Kc,
                        double &cstr)
//calcule cstr en fonction de l'ET culture
/*TO DO : inclure la texture si sol très sableux pFact := ParPFact+0.1, sol sableux
pFact := ParPFact+0.05, sol argileux pFact := ParPFact-0.05, très argileux
pFact := ParPFact-0.1 ; A inclure avant correction climatique. Nécessaire si l'on veut
comparer des sols sans modifier le pFactor*/
{
  double pFact;

  try {
    pFact = PFactor + 0.04 * (5 - Kc * ETo);
    pFact = std::max(0., pFact);
    pFact = std::min(0.8, pFact);
    cstr = std::min((FTSW * 1.0 / (1 - pFact)), 1.);
    cstr = max(0., cstr);

  } catch (...) {
    error_message("EvalCstrPFactorFAO", UBhyTypeFAO);
  }
}

//##############################################################################
//                Calcul de la transpiration potentielle selon FAO
//                      pour mémoire = DemandePlante  de Bileau
//##############################################################################

//##############################################################################
//                Calcul de la transpiration réelle
//                      pour mémoire = EvalTranspi de Bileau
//##############################################################################

//##############################################################################
//                Calcul de l'évapotranspiration maximale et réelle
//
//##############################################################################
void EvalETRETM(double const &Evap, double const &Tr, double const &Trpot,   double &ETM, double &ETR) {
  try {
    ETM = Evap + Trpot;
    ETR = Evap + Tr;

  } catch (...) {
    error_message("EvalETRETM", UBhyTypeFAO);
  }
}

//##############################################################################
//  Calcul de la consommation dans les différents réservoirs par transpiration
//##############################################################################

void EvolConsoResRDE_RFE(double const &RuRac, double const &RuSurf, double const &CapaREvap, double const &Tr,
                         double &StockRac, double &StockSurface, double &StockTotal, double &ValRFE, double &ValRDE, double &ValRSurf) {
  double TrSurf;

  try {
    // le calcul de cstr et de Tr doit intervenir après l'évaporation
    //calcul de la part de transpiration affectée aux réservoirs de surface
    if ((RuRac != 0)) {
      if ((RuRac <= RuSurf))          //correction JCC le 21/08/02 de stRurMax<=RuSurf/stRurMax
        TrSurf = Tr;
      else
        //TrSurf:=Tr*RuSurf/stRurMax;// on peut pondérer ici pour tenir compte du % racines dans chaque couche
        if ((StockRac != 0))
          TrSurf = Tr * StockSurface * 1.0 / StockRac; // modif du 15/04/05  pondération par les stocks et non les capacités, sinon on n'extrait pas Tr si stock nul
    } else
      TrSurf = 0;

    // MAJ des réservoirs de surface en répartissant TrSurf entre RFE et RDE
    ValRDE = std::max(0., ValRSurf - CapaREvap);
    if ((ValRDE + ValRFE < TrSurf)) {
      ValRFE = 0;
      ValRSurf = ValRSurf - ValRDE;
      ValRDE = 0;
    } else {
      if ((ValRFE > TrSurf))
        ValRFE = ValRFE - TrSurf;// priorité à la RFU
      else {
        ValRSurf = ValRSurf - (TrSurf - ValRFE);
        ValRDE = ValRDE - (TrSurf - ValRFE);
        ValRFE = 0;
      }
    }

    StockSurface = ValRFE + ValRDE;
    StockRac = std::max(0., StockRac - Tr);       // 18/04/05 déplacé en fin de procédure, car utilisé pour le calcul de Trsurf
    StockTotal = std::max(0., StockTotal - Tr);


  } catch (...) {
    error_message("EvolConsoResRDE_RFE", UBhyTypeFAO);
  }
}

//##############################################################################
//  Calcul du ruissellement de la pluie méthode seuil
//le ruissellement est egal a un pourcentage de la quantite de pluie supérieure
//  au seuil defini
//##############################################################################

void EvalRuiss_Seuil(double const &Rain, double const &seuilRuiss, double const &PourcRuiss, double const &Irrig,
                     double &EauDispo, double &Lr) {
  try {
    Lr = 0;
    if (Rain > seuilRuiss)
      Lr = (Rain - seuilRuiss) * PourcRuiss * 1.0 / 100;
    if (Irrig == NilValue)
      EauDispo = Rain - Lr;
    else
      EauDispo = Rain + Irrig - Lr;

  } catch (...) {
    error_message("EvalRuiss_Seuil", UBhyTypeFAO);
  }
}

//##############################################################################
//  Calcul du remplissage des différents réservoirs par EauDispo
//##############################################################################

void EvolRempliResRFE_RDE(double const &RuSurf, double const &EauDispo, double const &StRurMax, double const &CapaRFE, double const &CapaREvap, double const &CapaRDE, double const &StRuMax,
                          double &StRu, double &StRur, double &Hum, double &stRuSurf, double &Dr, double &ValRDE, double &ValRFE, double &ValRSurf)
// Procédure JCC - 19/07/2002
{
  double EauReste; double ValRSurfPrec; double EauTranspi;


  try {
    // Remplissage de la RFE:
    EauReste = 0;
    ValRFE = ValRFE + EauDispo; //non borné
    if (ValRFE > CapaRFE) {
      // bornage
      EauReste = ValRFE - CapaRFE;
      ValRFE = CapaRFE;
    }

    // Remplissage des autres réservoirs de surface :
    //Il faut dissocier l'eau qui va dans le réservoir Revap (non transpirable)
    //et l'eau qui va dans lesréservoirs de transpiration
    ValRSurfPrec = ValRSurf;
    ValRSurf = ValRSurf + EauReste;   //non borné
    /* On calcule la quantité d'eau qui ira dans les réservoirs racinaire, total et hum
    après déduction de l'eau qui va dans le réservoir uniquement évaporable*/
    if (ValRSurfPrec < CapaREvap)
      EauTranspi = EauDispo - (std::min(CapaREvap, ValRSurf) - ValRSurfPrec);
    else
      EauTranspi = EauDispo;

    if (ValRSurf > (CapaREvap + CapaRDE)) {      //bornage ValRSurf
      ValRSurf = CapaREvap + CapaRDE;
      ValRDE = CapaRDE;         //JCC 20/9/02
    } else {                      //JCC 20/9/02
      if (ValRSurf <= CapaREvap)      //JCC 20/9/02
        ValRDE = 0;                   //JCC 20/9/02                            //JCC 20/9/02
      else
        ValRDE = ValRSurf - CapaREvap; //JCC 20/9/02
    }

    stRuSurf = ValRFE + ValRDE;                 //JCC 20/9/02
    //stRuSurf:= std::min(stRuSurf+EauTranspi,RuSurf);  //JCC 20/9/02

    // remplissage des autres réservoirs par CB    { TODO : stRuSurf n'est plus modifié par la fonction: attention ! }
    // rempli res en eau, estime drainage, et evolution du front d'humectation
    StRu = StRu + EauTranspi;
    if (StRu > StRuMax) {
      Dr = StRu - StRuMax;
      StRu = StRuMax;
    } else {
      Dr = 0;
    }
    /* Modif JCC du 21/04/05 pour que Hum se réduise par évaporation tant qu'il est < RuSurf*/
    if (Hum < (CapaRFE + CapaRDE))
      Hum = stRuSurf;
    else
      Hum = std::max(Hum, StRu);
    // Rempli reservoir racines
    StRur = std::min(StRur + EauTranspi, StRurMax);
    //StRur:= std::min(StRur , StRu);       JCC 27/04/05

  } catch (...) {
    error_message("EvolRempliResRFE_RDE", UBhyTypeFAO);
  }
}

//##############################################################################
// Calcul de l'évolution de la RU Racinaire en fonction des réservoirs de surface
//##############################################################################



void EvolRurRFE_RDE(double const &Vrac, double const &Hum, double const &Ru, double const &stRuSurf, double const &RuSurf, double const &ProfRacIni, double const &Ze, double const &Z2, double const &ValRDE, double const &ValRFE, double const &NumPhase, double const &ChangePhase,
                    double &StRurMax, double &StRur, double &StRu) {
  double DeltaRur;

  try {
    if (NumPhase == 0) {
      StRurMax = 0;
      StRur = 0;
    } else {
      if (((NumPhase == 1) && (ChangePhase == 1))) {    // les conditions de germination sont atteinte et nous sommes le jour même
        StRurMax = Ru * std::min(ProfRacIni, (Ze + Z2)) * 1.0 / 1000;
        if (ProfRacIni <= Ze)
          StRur = (ValRDE + ValRFE) * ProfRacIni * 1.0 / Ze;
        else
          StRur = StRu * std::min(ProfRacIni * 1.0 / (Ze + Z2), 1.);
      } else {
        if ((Hum - StRurMax) < (Vrac * 1.0 / 1000 * Ru))
          DeltaRur = Hum - StRurMax;
        else
          DeltaRur = Vrac * 1.0 / 1000 * Ru;
        StRurMax = StRurMax + DeltaRur;
        if (StRurMax > RuSurf)
          StRur = StRur + DeltaRur;
        else
          StRur = (ValRDE + ValRFE) * (StRurMax * 1.0 / RuSurf);
      }
    }

  } catch (...) {
    error_message("EvolRurRFE_RDE", UBhyTypeFAO);
  }
}
////////////////////////////////////////////////////////////////////////////////

//##############################################################################
//#  Procedures specifiques pour comparer aux autres modeles                   #
//##############################################################################

////////////////////////////////////////////////////////////////////////////////


void EvolRurPalmier(double const &StRuMax, double const &StRu,
                    double &StRurMax, double &StRur)
/*pas de croissance racinaire*/
{
  try {
    //stRurMax:=Ru * (Ze+Z2)/1000;
    StRurMax = StRuMax;
    StRur = std::min(StRu, StRurMax);

  } catch (...) {
    error_message("EvolRurPalmier", UBhyTypeFAO);
  }
}
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void EvalFESW_RFE_RDE(double const &CapaREvap, double const &CapaRFE, double const &CapaRDE, double const &ValRSurf, double const &ValRFE,
                      double &FESW)
/*Pour utiliser ce Bhy avec module FESW² de Christian*/
{
  try {
    FESW = (ValRSurf + ValRFE) * 1.0 / (CapaREvap + CapaRFE + CapaRDE);

  } catch (...) {
    error_message("EvalFESW_RFE_RDE", UBhyTypeFAO);
  }
}
//##############################################################################

void EvolEvapSurfFESW(double const &EvapPot, double const &FESW, double const &CapaREvap, double const &CapaRDE, double const &StRurMax, double const &RuSurf,
                      double &Evap, double &ValRSurf, double &ValRFE, double &ValRDE, double &StRur, double &StRu) {
  double ValRSurfPrec; double EvapRU;
  double Evap1; double Evap2;

  /*Les capacités de réservoirs sont calculés comme dans FAO 56 mais l'évaporation
  est fonction de FESW² (modèle CB) */


  try {
    ValRSurfPrec = ValRSurf;
    // ValRSurf est l'eau contenue dans les réservoirs Revap (non transpirable) et RDE (transpirable et évaporable
    Evap = EvapPot * FESW * FESW;

    if (ValRFE > 0) {
      if (ValRFE < Evap) {
        Evap1 = ValRFE;
        Evap2 = Evap - ValRFE;
      } else {
        Evap1 = Evap;
        Evap2 = 0;
      }
    } else {
      Evap1 = 0;
      Evap2 = Evap;
    }

    ValRFE = ValRFE - Evap1;
    ValRSurf = ValRSurf - Evap2;
    ValRDE = std::max(0., ValRSurf - CapaREvap);


    // part de l'évaporation prélevée dans les réservoirs RFE et RDE
    if (ValRSurf >= CapaREvap)
      EvapRU = Evap;
    else {
      if (ValRSurfPrec <= CapaREvap)
        EvapRU = Evap1;
      else
        EvapRU = Evap1 + ValRSurfPrec - CapaREvap;
    }

    //Evaporation de Ru et Rur, MAJ
    if (StRurMax <= RuSurf)
      // quand les racines n'ont pas dépassé la première couche
      StRur = std::max(0., StRur - EvapRU * StRurMax * 1.0 / RuSurf);
    else
      StRur = std::max(0., StRur - EvapRU);

    StRu = StRu - EvapRU;

  } catch (...) {
    error_message("EvalEvapPot", UBhyTypeFAO);
  }
}
/////////////////////////////////////////////////////////////////////////////////
void EvolRurRFE_RDEcstr(double const &Vrac, double const &Hum, double const &Ru, double const &stRuSurf, double const &RuSurf, double const &ProfRacIni, double const &Ze, double const &Z2, double const &ValRDE, double const &ValRFE, double const &NumPhase, double const &ChangePhase, double const &cstr,
                        double &StRurMax, double &StRur, double &StRu)
/*fait évoluer RUR en fonction de cstr (CB)*/
{
  double DeltaRur;

  try {
    if (NumPhase == 0) {
      StRurMax = 0;
      StRur = 0;
    } else {
      if (((NumPhase == 1) && (ChangePhase == 1))) {    // les conditions de germination sont atteinte et nous sommes le jour même
        StRurMax = Ru * std::min(ProfRacIni, (Ze + Z2)) * 1.0 / 1000;
        if (ProfRacIni <= Ze)
          StRur = (ValRDE + ValRFE) * ProfRacIni * 1.0 / Ze;
        else
          StRur = StRu * std::min(ProfRacIni * 1.0 / (Ze + Z2), 1.);
      } else {
        /*if (Hum-StRurMax) < (VRac/1000*RU) Then
          DeltaRur := Hum-StRurMax
        else
          DeltaRur := VRac/1000*RU; Partie modifiée par CB*/
        if ((Hum - StRurMax) < (std::max(Vrac * cstr, Vrac * 0.7) * 1.0 / 1000 * Ru)) DeltaRur = Hum - StRurMax; else DeltaRur = std::max(Vrac * cstr, Vrac * 0.7) * 1.0 / 1000 * Ru;
        StRurMax = StRurMax + DeltaRur;
        if (StRurMax > RuSurf)
          StRur = StRur + DeltaRur;
        else
          StRur = (ValRDE + ValRFE) * (StRurMax * 1.0 / RuSurf);
      }
    }

  } catch (...) {
    error_message("EvolRurRFE_RDEcstr", UBhyTypeFAO);
  }
}
/*---------------------------------------------------------*/
void EvalCstrExpon(double const &FExpCstrTr, double const &GExpCstrTr, double const &FTSW,
                   double &cstr)
/*
Estimation de l'indice de contrainte de consommation d'eau
Sadras et MilRoy 1996
Lacape 1999
Paramètre specifique espece/variete
// Parametres
IN:
ftsw : mm
Par1ExpCSTR :
Par2ExpCSTR :
OUT:
cstr : %
*/
{
  cstr = 1 * 1.0 / (1 + exp(-(FTSW - FExpCstrTr) * 1.0 / GExpCstrTr));
}
/*------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
//Modules Kce et Kcp alternatifs
////////////////////////////////////////////////////////////////////////////////

//##############################################################################
//          Calcul de Kce potentiel fonction du Rn au sol et borne FAO
//##############################################################################

void EvalKcePotLTRRn(double const &Mulch, double const &vent, double const &HRmin, double const &Kcp, double const &Hauteur, double const &Lai,
                     double &Kce) {
  double Kborne;
// avec cette procédure fixer Mulch = 120
  /* JCC le 21/09/2005 : la borne est identique à FAO mais on introduit le Rn
  transmis au sol au lieu de Kcp*/

  try {
    if (((Hauteur == 0) || (Lai == 0)))
      Kce = std::max(0.12, Mulch * 1.0 / 100 + 0.04 * (vent - 2) - 0.004 * (HRmin - 45));
    else {
      Kborne = std::max(Kcp + 0.05, Mulch * 1.0 / 100 + (0.04 * (vent - 2) - 0.004 * (HRmin - 45)) * std::pow(Hauteur * 1.0 / 3, 0.3));
      Kce = Kborne * exp(-0.7 * Lai);
    }

  } catch (...) {
    error_message("EvalKcePotLTRRn", UBhyTypeFAO);
  }
}

//##############################################################################
// Calcul de Kcp FAO basé sur somme de degrés jours (était dans arachide)
//##############################################################################

/*Procedure EvolKcpLai2(Const NumPhase:Double;
                        Const KcMax, Lai:Double;
                        Var Kcp, MaxLai : Double);

Begin
try
  If Lai > MaxLai then MaxLai:= Lai;
  If ((NumPhase<=1) or (Lai = 0)) then
        begin
          Kcp:=0 ;
          MaxLai:=0 ;
        end
  else
   { If NumPhase< 5    then */
//begin
//Kcp:= std::min(0.62*power(Lai,0.3),(0.7+(KcMax-0.7)*std::max(0.,Lai-1.5)/1.5));
//Kcp:=KcMax*std::min(1,power(Lai/3,0.5));
//Kcp:= min (Kcp,KcMax);
//end
/*else
  Kcp:= std::min(KcMax,power(Lai/std::max(0.01,MaxLai),2));   */
/*except
  error_message('EvolKcpLai2',UBhyTypeFAO);
end;
end;  */
/////////////////////////////////////////////////////////////////////////////////
void EvolKcpLai2(double const &NumPhase,
                 double const &KcMax, double const &Lai,
                 double &Kcp, double &MaxLai)

{
  try {
    if (Lai > MaxLai)
      MaxLai = Lai;
    if (((NumPhase <= 1) || (Lai == 0))) {
      Kcp = 0;
      MaxLai = 0;
    } else if (NumPhase < 5)
      Kcp = KcMax * std::pow(std::min(1., (Lai * 1.0 / 3)), 0.5);
    else {
      //Kcp:=KcMax*power(std::min(1,(Lai/3)),0.5);
      Kcp = KcMax * std::min(pow(std::min(1., Lai * 1.0 / 3), 0.5), std::pow(Lai * 1.0 / std::max(0.01, MaxLai), 1.5));
      Kcp = min(Kcp, KcMax);

    }

    /*else
      Kcp:= std::min(KcMax,power(Lai/std::max(0.01,MaxLai),2));   */

  } catch (...) {
    error_message("EvolKcpLai2", UBhyTypeFAO);
  }
}

} //namespace
#endif//INCLUDED_BHYTYPEFAO_H
//END
