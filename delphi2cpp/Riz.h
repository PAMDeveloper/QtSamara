/**   @file
     @brief
*/
#pragma once
#ifndef INCLUDED_RIZ_H
#define INCLUDED_RIZ_H

#include "delphi_defines.h"
using namespace std;

namespace Riz {

int MonCompteur;

void CalculeLaMoyenne(double const &LaValeur,   int const &LeCompteur,
                      double &LaMoyenne) {
  try {
    LaMoyenne = (LaMoyenne * (LeCompteur - 1) + LaValeur) * 1.0 / (LeCompteur);

  } catch (...) {
    AfficheMessageErreur("CalculeLaMoyenne", URiz);
  }
}

//################################################################################
//#
//#     Procédures spécifiques
//################################################################################
////////------------------------------------------------------------------------

void EvolPhenoStress(double const &SeuilPP, double const &SommeDegresJour, double const &SeuilTempLevee, double const &SeuilTempBVP, double const &SeuilTempRPR, double const &SeuilTempMatu1, double const &SeuilTempMatu2, double const &StockSurface, double const &PourcRuSurfGermi, double const &RuSurf, double const &DateDuJour, double const &DateSemis, double const &stRu,
                     double &SumPP, double &NumPhase, double &SommeDegresJourPhasePrec, double &SeuilTempPhaseSuivante, double &ChangePhase, double &SeuilTempSousPhaseSuivante, double &ChangeSousPhase, double &NumSousPhase)
// Cette procédure est appelée en début de journée et fait évoluer les phase
// phénologiques. Pour celà, elle incrémente les numéro de phase et change la
// valeur du seuil de la phase suivante. ChangePhase est un booléen permettant
// d'informer le modèle pour connaître si un jour est un jour de changement
// de phase. Celà permets d'initialiser les variables directement dans les
// modules spécifiques.
// --> Stades phénologiques pour le modèle Mil réécrit:
// 0 : du jour de semis au début des conditions favorables pour la germination et de la récolte à la fin de simulation (pas de culture)
// 1 : du début des conditions favorables pour la germination au jour de la levée
// 2 : du jour de la levée au début de la phase photopériodique
// 3 : du début de la phase photopériodiqueau début de la phase reproductive
// 4 : du début de la phase reproductive au début de la maturation
//      sousphase1  de début RPR à RPR/4
//      sousphase2  de RPR/4 à RPR/2
//      sousphase3 de RPR/2 à 3/4 RPR
//      sousphase4 de 3/4 RPR à fin RPR
// 5 : du début de la maturation au début du séchage
// 6 : du début du séchage au jour de récolte
// 7 : le jour de la récolte
{
  bool ChangementDePhase; bool ChangementDeSousPhase;
  // on passe en variable un pseudo booléen et non directement ce booléen (pb de moteur)

  try {
    ChangePhase = 0;
    ChangeSousPhase = 0;
    // l'initialisation quotidienne de cette variable à faux permet de stopper le marquage d'une journée de changement de phase
    //mainForm.memDeroulement.Lines.Add('phase n°'+FloatToStr(NumPhase)+' StockSurface='+FloatToStr(StockSurface));
    if (std::trunc(NumPhase) == 0) {   // la culture a été semée mais n'a pas germé
      if (((StockSurface >= PourcRuSurfGermi * RuSurf) || (stRu > StockSurface))) {
        // on commence ds les conditions favo aujourd'hui
        NumPhase = 1;
        //mainForm.memDeroulement.Lines.Add('------> phase n°'+FloatToStr(NumPhase)+' (car StockSurface='+FloatToStr(StockSurface)+')');
        ChangePhase = 1;
        SeuilTempPhaseSuivante = SeuilTempLevee;
        /* TODO : à vérif par JCC, le déclencheur étant en phase 0 les conditions favorables et non SeuilTempGermi */
      }
    } // fin du if NumPhase=0
    else {
      // vérification d'un éventuel changement de phase
      if (((std::trunc(NumPhase) == 2) && (SommeDegresJour >= SeuilTempPhaseSuivante)))           //si on change de phase de BVP à PSP aujourd'hui
        ChangementDePhase = true;
      else {
        //sinon
        if (std::trunc(NumPhase) != 3)
          ChangementDePhase = (SommeDegresJour >= SeuilTempPhaseSuivante);
        else
          ChangementDePhase = (SumPP >= SeuilPP);
        // true=on quittera la phase photopériodique
      }

      // on a changé de phase
      if (ChangementDePhase) {
        ChangePhase = 1;
        NumPhase = NumPhase + 1;
        SommeDegresJourPhasePrec = SeuilTempPhaseSuivante;
        // utilisé dans EvalConversion
        switch ((int)std::trunc(NumPhase)) {
        case 2:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempBVP;
        // BVP Developpement vegetatif
        case 3:  SumPP = 0; // PSP Photoperiode
        case 4: {
          // gestion de l'initialisation des sous-phases
          SeuilTempSousPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempRPR
                                       * 1.0 / 4; // initialisation de la somme des DJ de la 1ère sous phase
          NumSousPhase = 1; // initialisation du n° de sous phase
          MonCompteur = 0; // on est bien le 1er jour de la 1ere sous phase
          ChangeSousPhase = 1;
          // on est bien un jour de changement de sous phase (en locurence, la première...)
          // gestion du seuil de la phase suivante
          SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempRPR;
          // RPR Stade initiation paniculaire
        }
        case 5:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu1;
        // Matu1 remplissage grains
        case 6:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu2;
          // Matu2 dessication
        } // Case NumPhase
      } // end change

      // gestion des sous-phases de la phase RPR (4)
      if (std::trunc(NumPhase) == 4) {
        ChangementDeSousPhase = (SommeDegresJour >=
                                 SeuilTempSousPhaseSuivante);
        if (ChangementDeSousPhase) {
          SeuilTempSousPhaseSuivante = SeuilTempSousPhaseSuivante + SeuilTempRPR
                                       * 1.0 / 4;
          NumSousPhase = NumSousPhase + 1;
          MonCompteur = 1;
          ChangeSousPhase = 1;
        } else
          ++MonCompteur;
      } // fin du if std::trunc(NumPhase)=4 then
    }

//    mainform.memDeroulement.Lines.Add(FloatToStr(SeuilTempPhaseSuivante));


  } catch (...) {
    AfficheMessageErreur("EvolPhenoStress | NumPhase: " + FloatToStr(NumPhase) +
                         " SommeDegresJour: " + FloatToStr(SommeDegresJour) +
                         " SeuilTempPhaseSuivante: " + FloatToStr(SeuilTempPhaseSuivante), URiz);
  }
}

////////////////////////////////////////////////////////////////////////////////

void EvalTMinCstr(double const &TMin, double const &Cstr, double const &NumPhase, double const &NumSousPhase,
                  double &TMinMoy, double &CstrMoy) {
  try {
    if (((NumPhase == 4) && (NumSousPhase == 3))) {
      CalculeLaMoyenne(TMin, MonCompteur, TMinMoy);
      CalculeLaMoyenne(Cstr, MonCompteur, CstrMoy);
    } else {
      if (NumPhase < 4) {
        TMinMoy = 0;
        CstrMoy = 0;
      }
    }

  } catch (...) {
    AfficheMessageErreur("EvalTMinCstr", URiz);
  }
}

////////////////////////////////////////////////////////////////////////////////

void EvalTMaxMoy(double const &TMax, double const &NumPhase, double const &NumSousPhase,   double &TMaxMoy) {
  try {
    if (((NumPhase == 4) && (NumSousPhase == 4)))
      CalculeLaMoyenne(TMax, MonCompteur, TMaxMoy); else if (NumPhase < 4)
      TMaxMoy = 0;

  } catch (...) {
    AfficheMessageErreur("EvalTMaxMoy", URiz);
  }
}

////////////////////////////////////////////////////////////////////////////////

void EvalStressTMin(double const &TStress, double const &TSterilite, double const &TMinMoy, double const &NumPhase, double const &ChangePhase,
                    double &StressTMin) {
  try {
    if (NumPhase <= 4)
      StressTMin = 1;
    if (((NumPhase == 5) && (ChangePhase == 1)))
      StressTMin = (std::max(0., std::min(TStress, TMinMoy) - TSterilite)) * 1.0 / std::max(0.0001
                   , TStress - TSterilite);

  } catch (...) {
    AfficheMessageErreur("EvalStressTMin", URiz);
  }
}

////////////////////////////////////////////////////////////////////////////////

void EvalStressTMax(double const &TLim, double const &TMaxMoy, double const &NumPhase, double const &ChangePhase,
                    double &StressTMax) {
  try {
    if (NumPhase <= 4)
      StressTMax = 1;
    if (((NumPhase == 5) && (ChangePhase == 1)))
      StressTMax = std::min(1., (TLim - std::max(TMaxMoy, 35.)) * 1.0 / (TLim - 35));

  } catch (...) {
    AfficheMessageErreur("EvalStressTMax", URiz);
  }
}

////////////////////////////////////////////////////////////////////////////////

void EvalStressCstr(double const &CstrMoy, double const &NumPhase, double const &ChangePhase,
                    double &StressCstr) {
  try {
    if (NumPhase <= 4)
      StressCstr = 1;
    if (((NumPhase == 5) && (ChangePhase == 1)))
      StressCstr = std::min(1., CstrMoy * 1.0 / 0.5);

  } catch (...) {
    AfficheMessageErreur("StressCstr", URiz);
  }
}

////////////////////////////////////////////////////////////////////////////////

void EvalSterilite(double const &StressTMin, double const &StressTMax, double const &StressCstr, double const &NumPhase, double const &ChangePhase,
                   double &Sterilite) {
  try {
    if (NumPhase < 5)
      Sterilite = 0;
    if (((NumPhase == 5) && (ChangePhase == 1))) {
      Sterilite = 100 * (1 - StressTMin * StressTMax * StressCstr);
    }

  } catch (...) {
    AfficheMessageErreur("EvalSterilite", URiz);
  }
}

////////////////////////////////////////////////////////////////////////////////

void EvalRdtPotRiz(double const &KRdt, double const &BiomasseTotale, double const &NumPhase, double const &ChangePhase, double const &KrdtB, double const &StressTMin, double const &StressTMax, double const &StressCstr,
                   double &RdtPot, double &BiomTotStadeFloraison, double &BiomTotStadeIp) {
  try {
    if (((NumPhase == 4) && (ChangePhase == 1)))
      BiomTotStadeIp = BiomasseTotale;
    if (NumPhase < 5)
      RdtPot = 0;
    else {
      if (((NumPhase == 5) && (ChangePhase == 1)))
        BiomTotStadeFloraison = BiomasseTotale;
      RdtPot = (KRdt * (BiomTotStadeFloraison - BiomTotStadeIp) + KrdtB) *
               (StressTMin * StressTMax * StressCstr);
    }

  } catch (...) {
    AfficheMessageErreur("EvalRdtPotRiz", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalAlloAeroFeuilSDJ(double const &NumPhase, double const &FeuilAeroBase, double const &FeuilAeroPente, double const &SeuilTempLevee, double const &SeuilTempBVP, double const &SDJmax, double const &SDJ,
                          double &RatioFeuilles)
// BiomFeuille=RatioFeuilles * BiomAero

{
  try {
    if (NumPhase == 2)
      RatioFeuilles = FeuilAeroBase; else if (((NumPhase > 2) && (NumPhase < 5)))
      // donc compris entre la phase d'emergence et reproductive inclus
      RatioFeuilles = std::max(0.05, FeuilAeroBase + FeuilAeroPente * (SDJ -
                                                                       (SeuilTempLevee + SeuilTempBVP)) * 1.0 / (SDJmax - (SeuilTempLevee +
                                                                           SeuilTempBVP)));
    else
      RatioFeuilles = 0;

  } catch (...) {
    AfficheMessageErreur("EvalAlloAeroFeuilSDJ", URiz);
  }
}
////////------------------------------------------------------------------------

void EvalPartitionTotAer(double const &KPenteAero, double const &BiomTot, double const &KBaseAero,
                         double &RatioAero) {
  try {
    RatioAero = std::min(1., KBaseAero + 2 * KPenteAero * BiomTot);

  } catch (...) {
    AfficheMessageErreur("EvalPartitionTotAer | KPenteAero: " +
                         FloatToStr(KPenteAero) +
                         " BiomTot: " + FloatToStr(BiomTot) +
                         " KBaseAero: " + FloatToStr(KBaseAero), URiz);
  }
}

////////------------------------------------------------------------------------

void EvolPartBiomasseAerienne(double const &DeltaBiomasseTotale, double const &RatioAero, double const &BiomasseTotale, double const &NumPhase, double const &KPenteAero, double const &KBaseAero, double const &ChangePhase,
                              double &DeltaBiomasseAerienne, double &BiomasseAerienne) {
  double BiomasseAeriennePrec;

  try {
    if (NumPhase <= 1) { // la levée n'a pas eu lieu encore
      DeltaBiomasseAerienne = 0;
      BiomasseAerienne = 0;
    } else {
      // BVP et sup
      if (((NumPhase == 2) && (ChangePhase == 1))) {       // initialisation
        //BiomasseAerienne := std::min(0.9, KPenteAero * BiomasseTotale + KBaseAero)* BiomasseTotale; JCC 15/11/04
        BiomasseAerienne = KBaseAero * BiomasseTotale;
        DeltaBiomasseAerienne = BiomasseAerienne;
      } else {
        BiomasseAeriennePrec = BiomasseAerienne;
        if (DeltaBiomasseTotale < 0)
          BiomasseAerienne = std::max(0., BiomasseAerienne + DeltaBiomasseTotale);
        else
          BiomasseAerienne = BiomasseAerienne + RatioAero * DeltaBiomasseTotale;

        DeltaBiomasseAerienne = BiomasseAerienne - BiomasseAeriennePrec;
      }
    }

  } catch (...) {
    AfficheMessageErreur("EvolBiomAero | DeltaBiomasseTotale: " +
                         FloatToStr(DeltaBiomasseTotale) +
                         " RatioAero: " + FloatToStr(RatioAero) +
                         " BiomasseTotale: " + FloatToStr(BiomasseTotale) +
                         " BiomasseAerienne: " + FloatToStr(BiomasseAerienne), URiz);
  }
}

////////------------------------------------------------------------------------

void EvolSlaSDJ(double const &DeltaDegresJour, double const &SDJBVP, double const &SDJRPR, double const &SlaBVP, double const &SlaRPR, double const &BiomLeaf, double const &dayBiomLeaf, double const &NumPhase, double const &ChangePhase,
                double &sla) {
  try {
    if (((NumPhase == 2) && (ChangePhase == 1)))
      sla = SlaBVP;  // soit SlaMax...
    else {
      if (NumPhase >= 2) {
        if (NumPhase < 5)
          //Sla:= std::max(SlaRPR, SlaBVP + KpenteSla* BiomLeaf/1000 + 0.0031*power(BiomLeaf/1000,2))
          //sla := std::max(SlaRPR,(sla + KpenteSla*BiomLeaf/1000 ))* (BiomLeaf- dayBiomLeaf)/BiomLeaf + (SlaBVP+sla)/2 * (dayBiomLeaf/BiomLeaf)
          //sla := std::min(sla,(sla - KpenteSla *(sla - SlaRPR)/(SlaBVP-SlaRPR))*
          sla = std::max(SlaRPR, (sla - (DeltaDegresJour * 1.0 / (SDJBVP + SDJRPR)) *
                                  (SlaBVP - SlaRPR)) *
                         (BiomLeaf - dayBiomLeaf) * 1.0 / BiomLeaf + (SlaBVP + sla) * 1.0 / 3.2 *
                         dayBiomLeaf * 1.0 / BiomLeaf);
        else
          sla = SlaRPR;
      } else
        sla = 0;
    }

  } catch (...) {
    AfficheMessageErreur("EvolSlaSDJ", URiz);
  }
}

////////------------------------------------------------------------------------

void EvolPhenoPSPStress(double const &SumPP, double const &PPsens, double const &SommeDegresJour, double const &SeuilTempLevee, double const &SeuilTempBVP, double const &SeuilTempRPR, double const &SeuilTempMatu1, double const &SeuilTempMatu2, double const &StockSurface, double const &PourcRuSurfGermi, double const &RuSurf, double const &DateDuJour, double const &DateSemis, double const &stRu,
                        double &NumPhase, double &SommeDegresJourPhasePrec, double &SeuilTempPhaseSuivante, double &ChangePhase, double &SeuilTempSousPhaseSuivante, double &ChangeSousPhase, double &NumSousPhase)
/*Procedure speciale pour inclure le module photoperiodique de Vaksman & Dingkuhn
qui fonctionne en degres jours et declanche IP lorsque SumPP est inferieur à PPsens*/
//JCC le 21/09/05
// Cette procédure est appelée en début de journée et fait évoluer les phase
// phénologiques. Pour celà, elle incrémente les numéro de phase et change la
// valeur du seuil de la phase suivante. ChangePhase est un booléen permettant
// d'informer le modèle pour connaître si un jour est un jour de changement
// de phase. Celà permets d'initialiser les variables directement dans les
// modules spécifiques.
// --> Stades phénologiques pour le modèle Mil réécrit:
// 0 : du jour de semis au début des conditions favorables pour la germination et de la récolte à la fin de simulation (pas de culture)
// 1 : du début des conditions favorables pour la germination au jour de la levée
// 2 : du jour de la levée au début de la phase photopériodique
// 3 : du début de la phase photopériodique au début de la phase reproductive
// 4 : du début de la phase reproductive au début de la maturation
//      sousphase1  de début RPR à RPR/4
//      sousphase2  de RPR/4 à RPR/2
//      sousphase3 de RPR/2 à 3/4 RPR
//      sousphase4 de 3/4 RPR à fin RPR
// 5 : du début de la maturation au début du séchage
// 6 : du début du séchage au jour de récolte
// 7 : le jour de la récolte
{
  bool ChangementDePhase; bool ChangementDeSousPhase;
  // on passe en variable un pseudo booléen et non directement ce booléen (pb de moteur)


  try {
    ChangePhase = 0;
    ChangeSousPhase = 0;
    // l'initialisation quotidienne de cette variable à faux permet de stopper le marquage d'une journée de changement de phase
    if ((std::trunc(NumPhase) == 0)) {     // la culture a été semée mais n'a pas germé
      if (((StockSurface >= PourcRuSurfGermi * RuSurf) || (stRu > StockSurface))) {
        // on commence ds les conditions favo aujourd'hui
        NumPhase = 1;
        ChangePhase = 1;
        SeuilTempPhaseSuivante = SeuilTempLevee;
      }
    } // fin du if NumPhase=0
    else {
      // vérification d'un éventuel changement de phase
      if (((std::trunc(NumPhase) == 1) && (SommeDegresJour >= SeuilTempPhaseSuivante)))           //si on change de phase de BVP à PSP aujourd'hui
        ChangementDePhase = true;
      else {
        //sinon
        if ((std::trunc(NumPhase) != 3)) {
          ChangementDePhase = (SommeDegresJour >= SeuilTempPhaseSuivante);
        } else {
          ChangementDePhase = (SumPP <= PPsens);
          // true=on quittera la phase photopériodique
        }
      }
      // on a changé de phase
      if (ChangementDePhase) {
        ChangePhase = 1;
        NumPhase = NumPhase + 1;
        SommeDegresJourPhasePrec = SeuilTempPhaseSuivante;
        // utilisé dans EvalConversion
        switch ((int)std::trunc(NumPhase)) {
        case 2:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempBVP;
        // BVP Developpement vegetatif
        case 4: {
          // gestion de l'initialisation des sous-phases
          SeuilTempSousPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempRPR
                                       * 1.0 / 4; // initialisation de la somme des DJ de la 1ère sous phase
          NumSousPhase = 1; // initialisation du n° de sous phase
          MonCompteur = 0; // on est bien le 1er jour de la 1ere sous phase
          ChangeSousPhase = 1;
          // on est bien un jour de changement de sous phase (en locurence, la première...)
          // gestion du seuil de la phase suivante
          SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempRPR;
          // RPR Stade initiation paniculaire
        }
        case 5:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu1;
        // Matu1 remplissage grains
        case 6:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu2;
          // Matu2 dessication
        } // Case NumPhase
      } // end change
      // gestion des sous-phases de la phase RPR (4)
      if ((std::trunc(NumPhase) == 4)) {
        ChangementDeSousPhase = (SommeDegresJour >=
                                 SeuilTempSousPhaseSuivante);
        if (ChangementDeSousPhase) {
          SeuilTempSousPhaseSuivante = SeuilTempSousPhaseSuivante + SeuilTempRPR
                                       * 1.0 / 4;
          NumSousPhase = NumSousPhase + 1;
          MonCompteur = 1;
          ChangeSousPhase = 1;
        } else
          ++MonCompteur;
      } // fin du if std::trunc(NumPhase)=4 then
    }

  } catch (...) {
    AfficheMessageErreur("EvolPhenoStress | NumPhase: " + FloatToStr(NumPhase) +
                         " SommeDegresJour: " + FloatToStr(SommeDegresJour) +
                         " SeuilTempPhaseSuivante: " + FloatToStr(SeuilTempPhaseSuivante), URiz);
  }
}

////////////////////////////////////////////////////////////////////////////////

void EvolPSPMVMD(double const &NumPhase, double const &ChangePhase, double const &SomDegresJour, double const &DegresDuJour, double const &SeuilPP, double const &PPCrit, double const &DureeDuJour, double const &PPExp,
                 double &SumPP, double &SeuilTempPhasePrec, double &SeuilTempPhaseSuivante)

{
  double SDJPSP;
  /*Procedure speciale Vaksman Dingkuhn valable pour tous types de sensibilite
  photoperiodique et pour les varietes non photoperiodique. PPsens varie de 0,4
  a 1,2. Pour PPsens > 2,5 = variété non photoperiodique. SeuilPP = 13.5 PPcrit = 12
  SumPP est dans ce cas une variable quotidienne (et non un cumul) testee dans
  EvolPhenoPhotoperStress*/

  if (NumPhase == 3) {
    if (ChangePhase == 1) {
      SumPP = 100; //valeur arbitraire d'initialisation >2.5
      //SeuilTempPhasePrec:= SeuilTempPhaseSuivante;   a déjà changé dans evolpheno
      SDJPSP = std::max(0.01, DegresDuJour);
    } else
      SDJPSP = SomDegresJour - SeuilTempPhasePrec + std::max(0.01, DegresDuJour);

    SumPP = std::pow((1000 * 1.0 / SDJPSP), PPExp) * std::max(0., (DureeDuJour - PPCrit)) * 1.0 /
            (SeuilPP - PPCrit);
    SeuilTempPhaseSuivante = SeuilTempPhasePrec + SDJPSP;
  }
}

void RS_EvalTMinMoy(double const &TMin, double const &NumPhase, double const &NumSousPhase,
                    double &TMinMoy) {
  try {
    if (((NumPhase == 4) && (NumSousPhase == 3))) {
      CalculeLaMoyenne(TMin, MonCompteur, TMinMoy);
    } else {
      if (NumPhase < 4) {
        TMinMoy = 0;
      }
    }

  } catch (...) {
    AfficheMessageErreur("RS_EvalTMinMoy", URiz);
  }
}

void RS_EvalFtswMoy(double const &Ftsw, double const &NumPhase, double const &NumSousPhase,
                    double &FtswMoy) {
  try {
    if (((NumPhase == 4) && (NumSousPhase == 4))) {
      CalculeLaMoyenne(Ftsw, MonCompteur, FtswMoy);
    } else {
      if (NumPhase < 4) {
        FtswMoy = 0;
      }
    }

  } catch (...) {
    AfficheMessageErreur("RS_EvalFtswMoy", URiz);
  }
}

void RS_EvalTMaxMoy(double const &TMax, double const &NumPhase, double const &NumSousPhase,
                    double &TMaxMoy) {
  try {
    if (((NumPhase == 4) && (NumSousPhase == 4)))
      CalculeLaMoyenne(TMax, MonCompteur, TMaxMoy); else if (NumPhase < 4)
      TMaxMoy = 0;

  } catch (...) {
    AfficheMessageErreur("RS_EvalTMaxMoy", URiz);
  }
}


void RS_EvalTMaxMoy_V2_3(double const &TMax, double const &TMin, double const &HMax, double const &HMin, double const &NumPhase, double const &NumSousPhase,
                         double &TMaxMoy) {
  double TPanToa;

  try {
    if (((NumPhase == 4) && (NumSousPhase == 4))) {
      // 0.888 reduced to 0.8 09-02-2016
      TPanToa = (0.8 * (TMax - TMin) + TMin) - (8.32 - 0.118 * ((1 - 0.8) * (HMax - HMin) + HMin));

      CalculeLaMoyenne(TPanToa, MonCompteur, TMaxMoy);
      //  TPanToa is panicle T at time of anthesis (TOA); TOA is estimated at 10:30h and Tair at this time is estimated to be at 88.8% of the diurnal T amplitude; RH effect on air-panicle T difference (TD) is TD=8.32-0.118RH, and this term is subtracted from the air temperature; RH is thereby estimated to be at (100-88.8)% of the diurnal RH amplitude; data from Julia.
    } else if (NumPhase < 4)
      TMaxMoy = 0;

  } catch (...) {
    AfficheMessageErreur("RS_EvalTMaxMoy_V2_3", URiz);
  }
}

} //namespace



#endif//INCLUDED_RIZ_H
//END
