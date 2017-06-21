#ifndef INCLUDED_MILBILANCARBONE_H
#define INCLUDED_MILBILANCARBONE_H


// Cette unité regroupe l'ensemble des procédures permettant
// le calcul de l'évolution du bilan carbonné pour la culture
// annuelle du Mil. Ces procédures sont basées sur
// l'unité biomasse.pas (de C. Baron) avec quelques modifications
// pour permettre une meilleur interchangabilité des modules du modèle.
// Toute variable préfixée "Delta" représente l'augmentation ou la diminution
// de la valeur qu'elle représente.
// Exemple: DeltaBiomasseTotale représente la quantité d'assimilat créé dans la
// journée par la culture, ce n'est pas un cumul.
//
// Auteur(s)       : V. BONNAL d'après J.C. Combres
// Unité(s) à voir : BilEau.pas, Biomasse.pas
// Date de commencement          : 21/06/2004
// Date de derniere modification : 25/06/2004
// Etat : en cours d'implémentation


#include <samara_defines.h>

using namespace std;

namespace MilBilanCarbone {

void InitiationCulture(double const &SeuilTempLevee, double const &SeuilTempBVP, double const &SeuilTempRPR, double const &SeuilTempMatu1, double const &SeuilTempMatu2,
                       double &SommeDegresJourMaximale, double &NumPhase, double &BiomasseAerienne, double &BiomasseVegetative, double &BiomasseTotale, double &BiomasseTiges, double &BiomasseRacinaire, double &BiomasseFeuilles, double &SommeDegresJour, double &DeltaBiomasseTotale, double &SeuilTempPhaseSuivante, double &Lai) {
  try {
    NumPhase = 0;
    SommeDegresJourMaximale = SeuilTempLevee + SeuilTempBVP + SeuilTempRPR + SeuilTempMatu1 + SeuilTempMatu2;
    SommeDegresJour = 0;
    BiomasseAerienne = 0;
    BiomasseVegetative = 0;
    BiomasseTotale = 0;
    BiomasseTiges = 0;
    BiomasseRacinaire = 0;
    BiomasseFeuilles = 0;
    DeltaBiomasseTotale = 0;
    SeuilTempPhaseSuivante = 0;
    Lai = 0;

  } catch (...) {
    error_message("InitiationCulture", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalDateLevee(double const &StockRacinaire,
                   double &NumPhase, double &ChangePhase)    /* TODO : Jamis utilisé ds le modèle... voir avec JCC) */
// à ce jour, cette procédure ne fait que vérifier la viabilité du semi
{
  try {
    if (((NumPhase == 1) && (StockRacinaire == 0))) {
      NumPhase = 7; // tuer la culture, fin de simulation { TODO : mettre un avertissement et vérifier qu'on arrête bien...}
      ChangePhase = 0;
//      mainForm.memDeroulement.Lines.Add(TimeToStr(Time) + (char)9 + "####### LEVEE IMPOSSIBLE ######");
    }

  } catch (...) {
    error_message("EvalDateLevee | StockRacinaire: " + FloatToStr(StockRacinaire) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------
// La température moyenne calculée est bornée à Topt
void EvalDegresJourTOpt(double const &TOpt, double const &TMoy, double const &TBase,   double &DegresDuJour) {
  try {
    DegresDuJour = max(min(TOpt, TMoy), TBase) - TBase;

  } catch (...) {
    error_message("EvalDegresJour", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------
// Procédure classique en bornant la température maxi à Topt et la mini à Tbase
// avec cette proc. saisir Topt1 = Topt2
void EvalDegresJourTborne(double const &TMax, double const &TMin, double const &TBase, double const &TOpt,   double &DegresDuJour) {
  try {
    DegresDuJour = (max(TMin, TBase) + min(TOpt, TMax)) * 1.0 / 2 - TBase;

  } catch (...) {
    error_message("EvalDegresJourTborne", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------
/* On calcule la vitesse moyenne de développement en considérant qu'elle est linéaire
croissante de 0 à 1 entre Tbase et Topt1, puis constante et égale à 1 entre Topt1 et Topt2
puis décroissante de 1 à 0 entre Topt2 et Tlethale.
Puis on calcule la température équivalente comprise entre Tbase et Topt1 donnant la même
vitesse de développement.
On calcule les degrés jours à partir de cette température équivalente
 */
void EvalDegresJourVitMoy(double const &TMax, double const &TMin, double const &TBase, double const &TOpt1, double const &TOpt2, double const &TL,
                          double &DegresDuJour) {
  double v; double v1; double v2;

  try {
    v1 = ((max(TMin, TBase) + min(TOpt1, min(TL, TMax))) * 1.0 / 2 - TBase) * 1.0 / (TOpt1 - TBase);
    v2 = (TL - (max(TMax, TOpt2) + TOpt2) * 1.0 / 2) * 1.0 / (TL - TOpt2);
    v = (v1 * (min(min(TL, TMax), TOpt1) - TMin) + (min(TOpt2, max(TOpt1, min(TL, TMax))) - TOpt1) + v2 * (max(TOpt2, min(TL, TMax)) - TOpt2)) * 1.0 / (min(TL, TMax) - max(TMin, TBase));
    DegresDuJour = v * (TOpt1 - TBase);

  } catch (...) {
    error_message("EvalDegresJourVitMoy | TMax=" + FloatToStr(TMax) +
                         " TMin=" + FloatToStr(TMin) + "TBase=" + FloatToStr(TBase) + " TOpt1=" + FloatToStr(TOpt1) +
                         " TOpt2=" + FloatToStr(TOpt2) + " TL=" + FloatToStr(TL) + " DegresDuJour=" + FloatToStr(DegresDuJour), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvolPhenologieMilv2(double const &SeuilPP, double const &SommeDegresJour, double const &SeuilTempLevee, double const &SeuilTempBVP, double const &SeuilTempRPR, double const &SeuilTempMatu1, double const &SeuilTempMatu2, double const &StockSurface, double const &PourcRuSurfGermi, double const &RuSurf, double const &SlaMax, double const &BiomasseTotale, double const &DateDuJour, double const &DateSemis, double const &stRu,
                         double &SumPP, double &NumPhase, double &SommeDegresJourPhasePrec, double &SeuilTempPhaseSuivante, double &BiomasseTotaleStadeIp, double &BiomasseTotaleStadeFlo, double &CycleReel, double &ChangePhase, double &DateMaturite)
// Cette procédure est appelée en début de journée et fait évoluer les phase
// phénologiques. Pour celà, elle incrémente les numéro de phase et change la
// valeur du seuil de la phase suivante. ChangePhase est un booléen permettant
// d'informer le modèle pour connaître si un jour est un jour de changement
// de phase. Celà permets d'initialiser les variables directement dans les
// modules spécifiques.
// --> Stades phénologiques pour le modèle Mil réécrit:
// 0 : du jour de semis au début des conditions favorables pour la germination
// 1 : du début des conditions favorables pour la germination au jour de la levée
// 2 : du jour de la levée au début de la floraison
// 3 : du début de la floraison au début de la photosensibilité
// 4 : du début de la photosensibilité au début de la maturation
// 5 : du début de la maturation au début du séchage
// 6 : du début du séchage au jour de récolte
// 7 : du jour de récolte à la fin de la simulation
{
  bool ChangementDePhase;  // on passe en variable un pseudo booléen et non directement ce booléen (pb de moteur)

  try {
    ChangePhase = 0; // l'initialisation quotidiènne de cette variable à faux permet de stopper le marquage d'une journée de changement de phase
    //mainForm.memDeroulement.Lines.Add('phase n°'+FloatToStr(NumPhase)+' StockSurface='+FloatToStr(StockSurface));
    if (NumPhase == 0) { // la culture a été semée mais n'a pas germé
      if (((StockSurface >= PourcRuSurfGermi * RuSurf) || (stRu > StockSurface))) {
        // on commence ds les conditions favo aujourd'hui
        NumPhase = 1;
        //mainForm.memDeroulement.Lines.Add('------> phase n°'+FloatToStr(NumPhase)+' (car StockSurface='+FloatToStr(StockSurface)+')');
        ChangePhase = 1;
        SeuilTempPhaseSuivante = SeuilTempLevee;  /* TODO : à vérif par JCC, le déclencheur étant en phase 0 les conditions favorables et non SeuilTempGermi */
      }
//   else    { TODO : Pas logique: inutile donc à revoir... }
//     begin // on vérifie ne pas être arrivé à la fin de la simulation
//     if SommeDegresJour>=SommeDegresJourMaximale then
//       NumPhase:=7; // pas de germination réalisée  { TODO : mettre un avertissement }
//       ChangePhase:=True;
//     end;
    } // fin du if NumPhase=0
    else {
      // vérification d'un éventuel changement de phase
      if (NumPhase != 3)
        ChangementDePhase = (SommeDegresJour >= SeuilTempPhaseSuivante); else   // Phase photopériodique
        ChangementDePhase = (SumPP >= SeuilPP);

      if (ChangementDePhase) { // on a changé de phase
        ChangePhase = 1;
        NumPhase = NumPhase + 1;
        //mainForm.memDeroulement.Lines.Add('------> phase n°'+FloatToStr(NumPhase));
        SommeDegresJourPhasePrec = SeuilTempPhaseSuivante; // utilisé dans EvalConversion
        switch ((int)std::trunc(NumPhase)) {
        case 2:  {   // BVP Developpement vegetatif
          SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempBVP;
          break;
        }
        case 3:
          SumPP = 0;   // PSP Photoperiode
          break;
        case 4:  {   // RPR Stade initiation paniculaire
          SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempRPR;
          BiomasseTotaleStadeIp = BiomasseTotale;
          break;
        }
        case 5:  {   // Matu1 remplissage grains
          SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu1;
          BiomasseTotaleStadeFlo = BiomasseTotale;
          break;
        }
        case 6:
          SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu2; // Matu2 dessication
          break;
        case 7:  { // Recolte
          CycleReel = DateDuJour - DateSemis;
          DateMaturite = DateDuJour;
          break;
          // sortie de RendementDeLaCulture
        }
        }  // Case NumPhase
      }  // end change
    }

  } catch (...) {
    error_message("EvolPhenologieTemperature | NumPhase: " + FloatToStr(NumPhase) +
                         " SommeDegresJour: " + FloatToStr(SommeDegresJour) +
                         " SeuilTempPhaseSuivante: " + FloatToStr(SeuilTempPhaseSuivante), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalConversion(double const &NumPhase, double const &EpsiB, double const &AssimBVP, double const &SommeDegresJour, double const &SommeDegresJourPhasePrecedente, double const &AssimMatu1, double const &AssimMatu2, double const &SeuilTempPhaseSuivante,
                    double &Conversion) {
  double KAssim;

  try {
    switch ((int)std::trunc(NumPhase)) {
    case 2:  KAssim = 1; break;
    case 3:  KAssim = AssimBVP; break;
    case 4:  KAssim = AssimBVP; break;
    case 5:  KAssim = AssimBVP + (SommeDegresJour - SommeDegresJourPhasePrecedente) *
                        (AssimMatu1 - AssimBVP) * 1.0 / (SeuilTempPhaseSuivante - SommeDegresJourPhasePrecedente); break;
    case 6:  KAssim = AssimMatu1 + (SommeDegresJour - SommeDegresJourPhasePrecedente) *
                        (AssimMatu2 - AssimMatu1) * 1.0 / (SeuilTempPhaseSuivante - SommeDegresJourPhasePrecedente); break;
    default:
      KAssim = 0; break;
    }
    Conversion = KAssim * EpsiB;

  } catch (...) {
    error_message("EvalConversion | NumPhase: " + FloatToStr(NumPhase) +
                         " SommeDegresJour: " + FloatToStr(SommeDegresJour), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalParIntercepte(double const &PAR, double const &LTR,   double &PARIntercepte) {
  try {
    PARIntercepte = PAR * (1 - LTR);

  } catch (...) {
    error_message("EvalParIntercepte | PAR: " + FloatToStr(PAR) +
                         " LTR: " + FloatToStr(LTR), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalAssimPot(double const &PARIntercepte, double const &Conversion,   double &AssimPot) {
  try {
    AssimPot = PARIntercepte * Conversion * 10;

  } catch (...) {
    error_message("EvalAssimPot | PAR Intercepté: " + FloatToStr(PARIntercepte) +
                         " Conversion: " + FloatToStr(Conversion), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalCstrAssim(double const &Cstr,   double &CstrAssim)
// formule simple permettant un éventuel changement de calcul (pour arachide par ex)
{
  try {
    CstrAssim = Cstr;

  } catch (...) {
    error_message("EvalCstrAssim | Cstr: " + FloatToStr(Cstr), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalVitesseRacinaire(double const &VRacLevee, double const &RootSpeedBVP, double const &RootSpeedRPR, double const &RootSpeedPSP, double const &RootSpeedMatu1, double const &RootSpeedMatu2, double const &NumPhase,
                          double &VitesseRacinaire)
//Modif JCC du 15/03/2005 pour inclure VracLevée différente de VRacBVP

{
  try {
    switch ((int)std::trunc(NumPhase)) {
    case 1:  VitesseRacinaire = VRacLevee; break;
    case 2:  VitesseRacinaire = RootSpeedBVP; break;
    case 3:  VitesseRacinaire = RootSpeedPSP; break;
    case 4:  VitesseRacinaire = RootSpeedRPR; break;
    case 5:  VitesseRacinaire = RootSpeedMatu1; break; /* TODO : attention en cas de gestion du champ vide... */
    case 6:  VitesseRacinaire = RootSpeedMatu2; break;
    default:
      VitesseRacinaire = 0; break;
    }

  } catch (...) {
    error_message("EvalVitesseRacinaire | NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------
void EvalAssim(double const &AssimPot, double const &CstrAssim,   double &Assim) {
  try {
    Assim = AssimPot * CstrAssim;

  } catch (...) {
    error_message("EvalAssim | AssimPot: " + FloatToStr(AssimPot) +
                         " CstrAssim: " + FloatToStr(CstrAssim), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalDeltaBiomTot(double const &Assimilats, double const &RespMaint, double const &NumPhase,
                      double &DeltaBiomasseTotale) {
  try {
    if (NumPhase >= 2)
      DeltaBiomasseTotale = Assimilats - RespMaint;
    else
      DeltaBiomasseTotale = 0;

  } catch (...) {
    error_message("EvalBiomasseTotaleDuJour | Assim: " + FloatToStr(Assimilats) +
                         " RespMaint: " + FloatToStr(RespMaint) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalRdtPotTotGra(double const &KRdt, double const &BiomTotStadeFloraison, double const &BiomTotStadeIp, double const &NumPhase, double const &KrdtB,
                      double &RdtPot) {
  try {
    if (NumPhase < 5)
      RdtPot = 0;
    else
      RdtPot = KRdt * (BiomTotStadeFloraison - BiomTotStadeIp) + KrdtB;

  } catch (...) {
    error_message("EvalRdtPotTotGra | KRdt: " + FloatToStr(KRdt) +
                         " BiomTotStadeFloraison: " + FloatToStr(BiomTotStadeFloraison) +
                         " BiomTotStadeIp: " + FloatToStr(BiomTotStadeIp) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalDRdtPot(double const &NumPhase, double const &RdtPot, double const &DegresDuJour, double const &SeuilTempMatu1,
                 double &RdtPotDuJour) {
  try {
    if (NumPhase == 5)
      RdtPotDuJour = RdtPot * (DegresDuJour * 1.0 / SeuilTempMatu1);
    else
      RdtPotDuJour = 0;

  } catch (...) {
    error_message("EvalDRdtPot | NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalReallocation(double const &RdtPotDuJour, double const &DeltaBiomasseAerienne, double const &KRealloc, double const &NumPhase,
                      double &Reallocation, double &ManqueAssim) {
  try {
    if (NumPhase == 5) {
      ManqueAssim = max(0., (RdtPotDuJour - max(0., DeltaBiomasseAerienne)));
      if (DeltaBiomasseAerienne < 0)
        Reallocation = 0;      //JCC 15/11/04
      //Reallocation:= ManqueAssim*KRealloc*0.5
      else
        Reallocation = ManqueAssim * KRealloc;    /*il faudra borner la réallocation à BiomFeuilles-10*/
    } else {
      ManqueAssim = 0;
      Reallocation = 0;
    }

  } catch (...) {
    error_message("EvalRealloc | RdtPotDujour: " + FloatToStr(RdtPotDuJour) +
                         " DeltaBiomasseAerienne: " + FloatToStr(DeltaBiomasseAerienne) +
                         " KRealloc: " + FloatToStr(KRealloc) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalReallocation2(double const &RdtPotDuJour, double const &DeltaBiomasseAerienne, double const &KRealloc, double const &NumPhase, double const &PenalAlloc,
                       double &Reallocation, double &ManqueAssim) {
  try {
    if (NumPhase == 5) {
      ManqueAssim = max(0., (RdtPotDuJour - max(0., DeltaBiomasseAerienne)));
      if (DeltaBiomasseAerienne < 0)
        //Reallocation:=0   //JCC 15/11/04
        Reallocation = ManqueAssim * KRealloc * PenalAlloc;
      else
        Reallocation = ManqueAssim * KRealloc;    /*il faudra borner la réallocation à BiomFeuilles-10*/
    } else {
      ManqueAssim = 0;
      Reallocation = 0;
    }

  } catch (...) {
    error_message("EvalRealloc2 | RdtPotDujour: " + FloatToStr(RdtPotDuJour) +
                         " DeltaBiomasseAerienne: " + FloatToStr(DeltaBiomasseAerienne) +
                         " KRealloc: " + FloatToStr(KRealloc) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvolBiomasseTotale(double const &DeltaBiomasseTotale, double const &NumPhase, double const &Densite, double const &KResGrain, double const &BiomasseGrain, double const &ChangePhase,
                        double &BiomasseTotale) {
  try {
    if (((NumPhase == 2) && (ChangePhase == 1)))      // initialisation
      BiomasseTotale = Densite * KResGrain * BiomasseGrain * 1.0 / 1000; //Biomasse initiale au jour de la levée
    else
      BiomasseTotale = BiomasseTotale + DeltaBiomasseTotale; // pas de gestion de phase,car gérée en amont

  } catch (...) {
    error_message("EvolBiomTot | BiomasseTotale: " + FloatToStr(BiomasseTotale) +
                         " DeltaBiomasseTotale: " + FloatToStr(DeltaBiomasseTotale), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvolBiomasseAerienne(double const &DeltaBiomasseTotale, double const &RatioAero, double const &BiomasseTotale, double const &NumPhase, double const &KPenteAero, double const &KBaseAero, double const &ChangePhase,
                          double &DeltaBiomasseAerienne, double &BiomasseAerienne) {
  double BiomasseAeriennePrec;

  try {
    if (NumPhase <= 1) { // la levée n'a pas eu lieu encore
      DeltaBiomasseAerienne = 0;
      BiomasseAerienne = 0;
    } else {
      // BVP et sup
      if (((NumPhase == 2) && (ChangePhase == 1))) {    // initialisation
        //BiomasseAerienne := Min(0.9, KPenteAero * BiomasseTotale + KBaseAero)* BiomasseTotale; JCC 15/11/04
        BiomasseAerienne = min(0.9, KBaseAero) * BiomasseTotale;   /* TODO -oViB : passer ce 0,9 en paramètre? */
        DeltaBiomasseAerienne = BiomasseAerienne;
      } else {
        BiomasseAeriennePrec = BiomasseAerienne;
        if (DeltaBiomasseTotale < 0)
          BiomasseAerienne = max(0., BiomasseAerienne + DeltaBiomasseTotale);
        else
          BiomasseAerienne = RatioAero * BiomasseTotale;
        DeltaBiomasseAerienne = BiomasseAerienne - BiomasseAeriennePrec;
      }
    }

  } catch (...) {
    error_message("EvolBiomAero | DeltaBiomasseTotale: " + FloatToStr(DeltaBiomasseTotale) +
                         " RatioAero: " + FloatToStr(RatioAero) +
                         " BiomasseTotale: " + FloatToStr(BiomasseTotale) +
                         " BiomasseAerienne: " + FloatToStr(BiomasseAerienne), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalBiomasseRacinair(double const &BiomasseTotale, double const &BiomasseAerienne,   double &BiomasseRacinaire) {
  try {
    BiomasseRacinaire = BiomasseTotale - BiomasseAerienne;

  } catch (...) {
    error_message("EvolBiomasseRacinair | BiomasseTotale: " + FloatToStr(BiomasseTotale) +
                         " BiomasseAerienne: " + FloatToStr(BiomasseAerienne) +
                         " BiomasseRacinaire: " + FloatToStr(BiomasseRacinaire), UMilBilanCarbone);
  }
}
////////------------------------------------------------------------------------

void EvalAllomTotAer(double const &KPenteAero, double const &BiomTot, double const &KBaseAero,
                     double &RatioAero) {
  try {
    RatioAero = min(0.9, KPenteAero * BiomTot + KBaseAero);

  } catch (...) {
    error_message("EvolAllomTotAer | KPenteAero: " + FloatToStr(KPenteAero) +
                         " BiomTot: " + FloatToStr(BiomTot) +
                         " KBaseAero: " + FloatToStr(KBaseAero), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalAllomAeroFeuilV1(double const &NumPhase, double const &kBaseLaiDev, double const &kPenteLaiDev, double const &BiomasseAerienne,
                          double &RatioFeuilles)
// BiomFeuille=RatioFeuilles * BiomAero
{
  double bM; double CM;

  try {
    if (((NumPhase > 1) && (NumPhase <= 4))) {   // donc compris entre la phase d'emergence et reproductive inclus
      bM =  kBaseLaiDev - 0.1;
      CM = ((kPenteLaiDev * 1000) * 1.0 / bM + 0.78) * 1.0 / 0.75;
      RatioFeuilles = (0.1 + bM * std::pow(CM, BiomasseAerienne * 1.0 / 1000)); /* TODO : qu'est ce  ce / 1000? */
    } else
      RatioFeuilles = 0;

  } catch (...) {
    error_message("EvolAllomAeroFeuilV1", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalAllomAeroFeuilV2(double const &Ma, double const &Mb, double const &Mc, double const &BiomasseAerienne, double const &Densite, double const &NumPhase,
                          double &RatioFeuilles) {
  try {
    if (((NumPhase >= 1) && (NumPhase <= 4))) {
      //RatioFeuilles := (Ma + Mb * std::pow(Mc,BiomasseAerienne*12.346/Densite));
      RatioFeuilles = (Ma + Mb * std::pow(Mc, BiomasseAerienne * 1.0 / 1000));
    } else
      RatioFeuilles = 0;

  } catch (...) {
    error_message("EvolAllomAeroFeuilV2", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvolBiomasseFeuilles(double const &DeltaBiomasseTotale, double const &PartFeuillesTiges, double const &NumPhase, double const &RatioFeuilles, double const &Reallocation, double const &BiomasseAerienne, double const &ChangePhase, double const &kBaseLaiDev, double const &kPenteLaiDev,
                          double &DeltaBiomFeuilles, double &BiomFeuilles) {
  double BiomasseFeuillePrec;

  try {
    BiomasseFeuillePrec = BiomFeuilles;
    if (NumPhase <= 1) // la levée n'a pas eu lieu encore
      BiomFeuilles = 0;
    else {
      if (((NumPhase == 2) && (ChangePhase == 1)))
        //BiomFeuilles:=(kBaseLaiDev+KPenteLaiDev*BiomasseAerienne)*BiomasseAerienne   JCC 15/11/04
        BiomFeuilles = kBaseLaiDev * BiomasseAerienne;
      else {
        if (NumPhase <= 4) { // de la levée à la phase reproductive
          if (DeltaBiomasseTotale < 0)
            BiomFeuilles = max(0., BiomFeuilles + DeltaBiomasseTotale * PartFeuillesTiges);
          else
            BiomFeuilles = RatioFeuilles * BiomasseAerienne;
        } else // de la Maturation à la fin de la simulation { TODO -oViB : que se passe t'il en phase 7 après la récolte? }
          BiomFeuilles = max(10., BiomFeuilles - (Reallocation - min(0., DeltaBiomasseTotale)) * PartFeuillesTiges);
        //JCC 18/10/05 max (0,..) remplacé par max(10,..) pour éviter division par 0 dans Sla
      }
    }
    DeltaBiomFeuilles = BiomFeuilles - BiomasseFeuillePrec;


  } catch (...) {
    error_message("EvolBiomFeuille | DeltaBiomasseTotale: " + FloatToStr(DeltaBiomasseTotale) +
                         " BiomasseAerienne: " + FloatToStr(BiomasseAerienne) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}


////////////////////////////////////////////////////////////////////////////////
void EvolBiomasseFeuilles2(double const &DeltaBiomasseTotale, double const &PartFeuillesTiges, double const &NumPhase, double const &RatioFeuilles, double const &Reallocation, double const &BiomasseAerienne, double const &ChangePhase, double const &kBaseLaiDev, double const &kPenteLaiDev,
                           double &DeltaBiomFeuilles, double &BiomFeuilles) {
  double BiomasseFeuillePrec;

  try {
    BiomasseFeuillePrec = BiomFeuilles;
    if (NumPhase <= 1) // la levée n'a pas eu lieu encore
      BiomFeuilles = 0;
    else {
      if (((NumPhase == 2) && (ChangePhase == 1)))
        //BiomFeuilles:=(kBaseLaiDev+KPenteLaiDev*BiomasseAerienne)*BiomasseAerienne   JCC 15/11/04
        BiomFeuilles = kBaseLaiDev * BiomasseAerienne;
      else {
        if (NumPhase <= 4) { // de la levée à la phase reproductive
          if (DeltaBiomasseTotale < 0)
            BiomFeuilles = max(10., BiomFeuilles + DeltaBiomasseTotale * PartFeuillesTiges);
          //JCC 08/06/06 max (10.. oublié ici correction ce jour;
          else
            BiomFeuilles = max(10., RatioFeuilles * BiomasseAerienne);
          //JCC 08/06/06 max (10.. oublié ici correction ce jour
        } else // de la Maturation à la fin de la simulation { TODO -oViB : que se passe t'il en phase 7 après la récolte? }
          BiomFeuilles = max(10., BiomFeuilles - (Reallocation - min(0., DeltaBiomasseTotale)) * PartFeuillesTiges);
        //JCC 18/10/05 max (0,..) remplacé par max(10,..) pour éviter division par 0 dans Sla
      }
    }
    DeltaBiomFeuilles = BiomFeuilles - BiomasseFeuillePrec;


  } catch (...) {
    error_message("EvolBiomFeuille2 | DeltaBiomasseTotale: " + FloatToStr(DeltaBiomasseTotale) +
                         " BiomasseAerienne: " + FloatToStr(BiomasseAerienne) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalSlaRapBiomV2(double const &SlaBVP, double const &SlaRPR, double const &KpenteSla, double const &dayBiomLeaf, double const &BiomLeaf, double const &NumPhase, double const &ChangePhase,
                      double &sla) {
  try {
    if (((NumPhase == 2) && (ChangePhase == 1)))
      sla = SlaBVP; // soit SlaMax...
    else {
      if (NumPhase >= 2) {
        //If dayBiomLeaf >0 then        Modif JCC du 07/04/05 suite à modif CB
        //sla := (sla - KpenteSla * (sla- SlaRPR))* (BiomLeaf- dayBiomLeaf)/BiomLeaf+ SlaBVP * (dayBiomLeaf/BiomLeaf);
        //JCC 05/11/04 nouvelle formule de MD et CB
        sla = (sla - KpenteSla * (sla - SlaRPR)) * (BiomLeaf - dayBiomLeaf) * 1.0 / BiomLeaf + (SlaBVP + sla) * 1.0 / 2 * (dayBiomLeaf * 1.0 / BiomLeaf);
        sla = min(SlaBVP, max(SlaRPR, sla));
      } else
        sla = 0;
    }

  } catch (...) {
    error_message("EvalSlaRapBiomV2", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvolBiomasseTiges(double const &DeltaBiomasseTotale, double const &NumPhase, double const &PartFeuillesTiges, double const &Reallocation, double const &BiomasseFeuilles, double const &BiomasseAerienne,
                       double &BiomasseTiges) {
  try {
    if (NumPhase <= 1) // la levée n'a pas eu lieu encore
      BiomasseTiges = 0;
    else {
      if (NumPhase <= 4) { // de la phase germi à la phase reproductive
        if (DeltaBiomasseTotale < 0)
          BiomasseTiges = max(0., BiomasseTiges + DeltaBiomasseTotale * (1 - PartFeuillesTiges));
        else
          BiomasseTiges = BiomasseAerienne - BiomasseFeuilles;
      } else // de la photosensible à la fin de la simulation { TODO -oViB : que se passe t'il en phase 7 après la récolte? }
        BiomasseTiges = max(0., BiomasseTiges - (Reallocation - min(0., DeltaBiomasseTotale)) * (1 - PartFeuillesTiges));    // ce qui est réalloué est pris dans les feuilles et ds les tiges
    }                                          /* TODO : KRealloc ou Reallocation ??? */

  } catch (...) {
    error_message("EvolBiomasseTiges", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalBiomasseVegetati(double const &BiomasseTiges, double const &BiomasseFeuilles, double const &NumPhase,
                          double &BiomasseVegetative) {
  try {
    BiomasseVegetative = BiomasseTiges  + BiomasseFeuilles;

  } catch (...) {
    error_message("EvolBiomasseVegetati | BiomasseTiges: " + FloatToStr(BiomasseTiges) +
                         " BiomasseFeuille: " + FloatToStr(BiomasseFeuilles), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalDeltaRdt(double const &DeltaBiomasseAerienne, double const &Reallocation, double const &NumPhase, double const &RdtPotDuJour,
                  double &DRdt) {
  try {
    if (NumPhase == 5)
      DRdt = min(RdtPotDuJour, max(0., DeltaBiomasseAerienne) + Reallocation);
    else
      DRdt = 0;

  } catch (...) {
    error_message("EvalRdtDuJour | DeltaBiomasseAerienne: " + FloatToStr(DeltaBiomasseAerienne) +
                         " Reallocation: " + FloatToStr(Reallocation) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvolRdtV2(double const &DRdt, double const &NumPhase,   double &Rdt) {
  try {
    if (NumPhase < 5)
      Rdt = 0;
    else
      Rdt = Rdt + DRdt;

  } catch (...) {
    error_message("EvolRdtV2 | DRdt: " + FloatToStr(DRdt) +
                         " Rdt: " + FloatToStr(Rdt), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvolSomDegresJour(double const &DegresDuJour, double const &NumPhase,
                       double &SommeDegresJour) {
  try {
    if ((NumPhase >= 1)) {
      SommeDegresJour = SommeDegresJour + DegresDuJour;
    } else {
      SommeDegresJour = 0;
    }

  } catch (...) {
    error_message("EvolSommeDegresJour | DegresDuJour: " + FloatToStr(DegresDuJour) +
                         " Phase n°" + FloatToStr(NumPhase) +
                         " SommeDegresJour: " + FloatToStr(SommeDegresJour) +
                         " SommeDegresJour: " + FloatToStr(SommeDegresJour), UMilBilanCarbone);
  }
}
////////------------------------------------------------------------------------

void EvolPhenoGraminees(double const &PPSens, double const &SommeDegresJour, double const &SeuilTempLevee, double const &SeuilTempBVP, double const &SeuilTempRPR, double const &SeuilTempMatu1, double const &SeuilTempMatu2, double const &StockSurface, double const &PourcRuSurfGermi, double const &RuSurf, double const &DateDuJour, double const &DateSemis, double const &stRu,
                        double &SumPP, double &NumPhase, double &SommeDegresJourPhasePrec, double &SeuilTempPhaseSuivante, double &ChangePhase)
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
// 5 : du début de la maturation au début du séchage
// 6 : du début du séchage au jour de récolte
// 7 : le jour de la récolte
{
  bool ChangementDePhase;  // on passe en variable un pseudo booléen et non directement ce booléen (pb de moteur)

  try {
    ChangePhase = 0; // l'initialisation quotidiènne de cette variable à faux permet de stopper le marquage d'une journée de changement de phase
    //mainForm.memDeroulement.Lines.Add('phase n°'+FloatToStr(NumPhase)+' StockSurface='+FloatToStr(StockSurface));
    if (NumPhase == 0) { // la culture a été semée mais n'a pas germé
      if (((StockSurface >= PourcRuSurfGermi * RuSurf) || (stRu > StockSurface))) {
        // on commence ds les conditions favo aujourd'hui
        NumPhase = 1;
        //mainForm.memDeroulement.Lines.Add('------> phase n°'+FloatToStr(NumPhase)+' (car StockSurface='+FloatToStr(StockSurface)+')');
        ChangePhase = 1;
        SeuilTempPhaseSuivante = SeuilTempLevee;  /* TODO : à vérif par JCC, le déclencheur étant en phase 0 les conditions favorables et non SeuilTempGermi */
      }
//   else    { TODO : Pas logique: inutile donc à revoir... }
//     begin // on vérifie ne pas être arrivé à la fin de la simulation
//     if SommeDegresJour>=SommeDegresJourMaximale then
//       NumPhase:=7; // pas de germination réalisée  { TODO : mettre un avertissement }
//       ChangePhase:=True;
//     end;
    } // fin du if NumPhase=0
    else {
      // vérification d'un éventuel changement de phase
      if (NumPhase != 3)
        ChangementDePhase = (SommeDegresJour >= SeuilTempPhaseSuivante); else   // Phase photopériodique
        ChangementDePhase = (SumPP <= PPSens);

      if (ChangementDePhase) { // on a changé de phase
        ChangePhase = 1;
        NumPhase = NumPhase + 1;
        //mainForm.memDeroulement.Lines.Add('------> phase n°'+FloatToStr(NumPhase));
        SommeDegresJourPhasePrec = SeuilTempPhaseSuivante; // utilisé dans EvalConversion
        switch ((int)std::trunc(NumPhase)) {
        case 2:  {   // BVP Developpement vegetatif

          SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempBVP;
          break;
        }
        case 3:  SumPP = 0;   // PSP Photoperiode
          break;
        case 4:  {   // RPR Stade initiation paniculaire
          SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempRPR;
          break;
          //BiomasseTotaleStadeIp := BiomasseTotale;
        }
        case 5:  {   // Matu1 remplissage grains
          SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu1;
          break;
          //BiomasseTotaleStadeFlo:=BiomasseTotale;
        }
        case 6:  SeuilTempPhaseSuivante = SeuilTempPhaseSuivante + SeuilTempMatu2; // Matu2 dessication
          break;

        }  // Case NumPhase
      }  // end change
    }

  } catch (...) {
    error_message("EvolPhenologieTemperature | NumPhase: " + FloatToStr(NumPhase) +
                         " SommeDegresJour: " + FloatToStr(SommeDegresJour) +
                         " SeuilTempPhaseSuivante: " + FloatToStr(SeuilTempPhaseSuivante), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------
/* JCC le14/03/2005 on va faire calculer BiomTotStadeFloraison et BiomTotStadeIp dans ce module spécifique graminées
au lieu de le calculer dans EvonPhenoTemp   */
void EvalRdtPotGramin(double const &KRdt, double const &BiomasseTotale, double const &NumPhase, double const &ChangePhase, double const &KrdtB,
                      double &RdtPot, double &BiomTotStadeFloraison, double &BiomTotStadeIp) {
  try {
    if (((NumPhase == 4) && (ChangePhase == 1)))
      BiomTotStadeIp = BiomasseTotale;
    if (NumPhase < 5)
      RdtPot = 0;
    else {
      if (((NumPhase == 5) && (ChangePhase == 1)))
        BiomTotStadeFloraison = BiomasseTotale;
      RdtPot = KRdt * (BiomTotStadeFloraison - BiomTotStadeIp) + KrdtB;

    }

  } catch (...) {
    error_message("EvalRdtPotGraminees | KRdt: " + FloatToStr(KRdt) +
                         " BiomasseTotale: " + FloatToStr(BiomasseTotale) +
                         " NumPhase: " + FloatToStr(NumPhase) +
                         " ChangePhase: " + FloatToStr(ChangePhase) +
                         " KrdtB: " + FloatToStr(KrdtB), UMilBilanCarbone);
  }
}
//---------------------------------------------------------------------------------------
void EvolLAIPhases(double const &NumPhase, double const &BiomasseFeuilles, double const &sla,
                   double &Lai)

{
  try {
    if ((NumPhase <= 1)) {
      Lai = 0;
    } else {
      if ((NumPhase <= 6)) {
        Lai = BiomasseFeuilles * sla;
      } else {
        Lai = 0;
      }
    }

  } catch (...) {
    error_message("EvolLAIPhases: ", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalDRdtPotcstr(double const &NumPhase, double const &RdtPot, double const &DegresDuJour, double const &SeuilTempMatu1, double const &Cstr,
                     double &RdtPotDuJour)
/*On pondère la demande journalière par cstr comme Christian Baron*/
{
  try {
    if (NumPhase == 5)
      RdtPotDuJour = RdtPot * (DegresDuJour * 1.0 / SeuilTempMatu1) * Cstr;
    else
      RdtPotDuJour = 0;

  } catch (...) {
    error_message("EvalDRdtPotcstr | NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvolSomDegresJourcstr(double const &stRuSurf, double const &RuSurf, double const &DegresDuJour, double const &NumPhase,
                           double &SommeDegresJour)
/* on va multiplier DJ par cstr pour tenter de traduire le retard à la levée en cas de sécheresse*/
{
//try
  if (NumPhase < 1) // on ne cumule qu'après la germination
    SommeDegresJour = 0;
  else if (NumPhase == 1)
    SommeDegresJour = SommeDegresJour + DegresDuJour * max(0.3, min(1., stRuSurf * 1.0 / (0.7 * RuSurf)));
  else
    SommeDegresJour = SommeDegresJour + DegresDuJour;
//except
//  error_message('EvolSommeDegresJour | DegresDuJour: '+FloatToStr(DegresDuJour)+
//                       ' Phase n°'+FloatToStr(NumPhase)+
//                      ' SommeDegresJour: '+FloatToStr(SommeDegresJour)+
//                       ' SommeDegresJour: '+FloatToStr(SommeDegresJour),UMilBilanCarbone);
//end;
}

////////------------------------------------------------------------------------

void EvalSlaRapBiom2(double const &SlaBVP, double const &SlaRPR, double const &KpenteSla, double const &BiomLeaf, double const &dayBiomLeaf, double const &NumPhase, double const &ChangePhase,
                     double &sla) {
  try {
    //if ((NumPhase=2) and (ChangePhase=1)) then                    //  JCC 18/10/05
    if (NumPhase == 2)
      sla = SlaBVP; // soit SlaMax...
    else {
      //if NumPhase>=2 then
      if (NumPhase > 2)
        /*Begin
         if NumPhase<5 then*/
      {
        sla = min(sla, (sla - KpenteSla * min((SlaBVP - SlaRPR) * 1.0 / 3.5, (sla - SlaRPR))) *
                  (BiomLeaf - max(0., dayBiomLeaf)) * 1.0 / BiomLeaf + (SlaBVP + sla) * 1.0 / 3 * max(0., dayBiomLeaf) * 1.0 / BiomLeaf);
        sla = min(SlaBVP, max(SlaRPR, sla));
      }
      /* else
        Sla:= SlaRPR
      end */
      else
        sla = 0;
    }

  } catch (...) {
    error_message("EvalSlaRapBiom2", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------

void EvalAlloAeroFeuilLin(double const &NumPhase, double const &FeuilAeroBase, double const &FeuilAeroPente, double const &BiomasseAerienne,
                          double &RatioFeuilles)
// BiomFeuille=RatioFeuilles * BiomAero

{
  try {
    if (((NumPhase > 1) && (NumPhase <= 4)))     // donc compris entre la phase d'emergence et reproductive inclus
      RatioFeuilles = max(0.1, FeuilAeroBase + FeuilAeroPente * BiomasseAerienne);
    else
      RatioFeuilles = 0;

  } catch (...) {
    error_message("EvalAlloAeroFeuilLin", UMilBilanCarbone);
  }
}

////////------------------------------------------------------------------------



void EvalReallocationSorgho(double const &RdtPotDuJour, double const &DeltaBiomasseAerienne, double const &TxRealloc, double const &NumPhase, double const &PenalAlloc,
                            double &Reallocation, double &ManqueAssim, double &ReallocationMax) {
  try {
    if (NumPhase == 5) {
      ManqueAssim = max(0., (RdtPotDuJour - max(0., DeltaBiomasseAerienne)));
      if (DeltaBiomasseAerienne < 0) {
        if (ReallocationMax > 0) { // il y a encore des réserves Carbonnées à mobiliser
          Reallocation = ManqueAssim * TxRealloc * PenalAlloc;
          ReallocationMax = ReallocationMax - Reallocation; // on diminue les réserves
        } else
          Reallocation = 0; // on a consommé toute la  réserve on ne peut réalouer
        //BiomTotStadeFloraison:=BiomTotStadeFloraison-Reallocation  ;
      } else
        Reallocation = ManqueAssim * TxRealloc;    /*il faudra borner la réallocation à BiomFeuilles-10*/
    } else {
      ManqueAssim = 0;
      Reallocation = 0;
    }

  } catch (...) {
    error_message("EvalReallocationSorgho | RdtPotDujour: " + FloatToStr(RdtPotDuJour) +
                         " DeltaBiomasseAerienne: " + FloatToStr(DeltaBiomasseAerienne) +
                         " TxRealloc: " + FloatToStr(TxRealloc) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}

void EvalReallocationSorgho2(double const &RdtPotDuJour, double const &DeltaBiomasseAerienne, double const &TxRealloc, double const &NumPhase, double const &PenalAlloc,
                             double &Reallocation, double &ManqueAssim, double &ReallocationMax) {
  try {
    if (NumPhase == 5) {
      ManqueAssim = max(0., (RdtPotDuJour - max(0., DeltaBiomasseAerienne)));
      if (DeltaBiomasseAerienne < 0) {
        if (ReallocationMax > 0) { // il y a encore des réserves Carbonnées à mobiliser
          Reallocation = min(ManqueAssim * TxRealloc * PenalAlloc, ReallocationMax);
        } else
          Reallocation = 0; // on a consommé toute la  réserve on ne peut réalouer
        //BiomTotStadeFloraison:=BiomTotStadeFloraison-Reallocation  ;
      } else
        Reallocation = min(ManqueAssim * TxRealloc, ReallocationMax);
    } else {
      ManqueAssim = 0;
      Reallocation = 0;
    }
    ReallocationMax = max(0., ReallocationMax - Reallocation); // on diminue les réserves

  } catch (...) {
    error_message("EvalReallocationSorgho2 | RdtPotDujour: " + FloatToStr(RdtPotDuJour) +
                         " DeltaBiomasseAerienne: " + FloatToStr(DeltaBiomasseAerienne) +
                         " TxRealloc: " + FloatToStr(TxRealloc) +
                         " NumPhase: " + FloatToStr(NumPhase), UMilBilanCarbone);
  }
}
////////////////////////////////////////////////////////////////////////////////



void EvalRdtPotSorgho(double const &KRdt, double const &BiomasseTotale, double const &NumPhase, double const &ChangePhase, double const &KrdtB, double const &StressTMin, double const &StressTMax, double const &StressCstr, double const &KRdtBiom, double const &KReallocMax,
                      double &RdtPot, double &BiomTotStadeFloraison, double &BiomTotStadeIp, double &ReallocationMax) {
  try {
    if (((NumPhase == 4) && (ChangePhase == 1)))
      BiomTotStadeIp = BiomasseTotale;
    if (NumPhase < 5) {
      RdtPot = 0;
      ReallocationMax = 0;
    } else {
      if (((NumPhase == 5) && (ChangePhase == 1))) {
        BiomTotStadeFloraison = BiomasseTotale;
        ReallocationMax = BiomTotStadeFloraison * KReallocMax; // permet de définir le réservoir disponible et mobilisable
      }
      RdtPot = (KRdt * (BiomTotStadeFloraison - BiomTotStadeIp) + KrdtB) * (StressTMin * StressTMax * StressCstr) + KRdtBiom * BiomTotStadeFloraison;
    }

  } catch (...) {
    error_message("EvalRdtPotSorgho", UMilBilanCarbone);
  }
}

void EvalRdtPotSorgho2(double const &BiomasseTotale, double const &NumPhase, double const &ChangePhase, double const &KRdtBiom, double const &KReallocMax,
                       double &RdtPot, double &BiomTotStadeFloraison, double &BiomTotStadeIp, double &ReallocationMax) {
  try {
    if (((NumPhase == 4) && (ChangePhase == 1)))
      BiomTotStadeIp = BiomasseTotale;
    if (NumPhase < 5) {
      RdtPot = 0;
      ReallocationMax = 0;
    } else {
      if (((NumPhase == 5) && (ChangePhase == 1))) {
        BiomTotStadeFloraison = BiomasseTotale;
        ReallocationMax = BiomTotStadeFloraison * KReallocMax; // permet de définir le réservoir disponible et mobilisable
      }
      RdtPot = KRdtBiom * BiomTotStadeFloraison;
    }

  } catch (...) {
    error_message("EvalRdtPotSorgho2", UMilBilanCarbone);
  }
}

} //namespace
#endif//INCLUDED_MILBILANCARBONE_H
//END
