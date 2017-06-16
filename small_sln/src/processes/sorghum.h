#ifndef SORGHUM_H
#define SORGHUM_H

#include <samara_defines.h>
using namespace std;

namespace Sorghum {
static double tabCstr[5]; // utilisé dans SorghumMortality()
static int tabCstrIndiceCourant = 0; // utilisé dans SorghumMortality()
static int NbJourCompte = 0;

//##############################################################################
/// Ce module permet de suivre l'évolution des 5 derniers jours de Cstr afin de
/// moyenner la valeur des stress. Si la moyenne des Cstr est inférieure à
/// un seuil SorghumMortality, la culture décède (NumPhase=8)
/// Demande MD du 28/09/06
//##############################################################################

void SorghumMortality(double const &cstr, double const &SeuilCstrMortality,   double &NumPhase) {
  int i;
  double MoyenneCstr;

  try {
    if ((NumPhase >= 2)) {
      NbJourCompte = NbJourCompte + 1;
      // gestion de l'indice...
      if ((tabCstrIndiceCourant == 5)) {
        tabCstrIndiceCourant = 1;
        tabCstr[ tabCstrIndiceCourant ] = cstr;
      } else {
        tabCstrIndiceCourant = tabCstrIndiceCourant + 1;
        tabCstr[ tabCstrIndiceCourant ] = cstr;
      }
      // gestion de la mortalité
      if ((NbJourCompte >= 5)) {
        // on peut moyenner...
        MoyenneCstr = 0;
        {
          long i_end = 6 ; for (i = 1 ; i < i_end ; ++i) {
            MoyenneCstr = MoyenneCstr + tabCstr[ i ];
          }
        }
        if (((MoyenneCstr * 1.0 / 5) <= SeuilCstrMortality)) {
          NumPhase = 7;
        }
      }
    }

  } catch (...) {
    error_message("SorghumMortality", URiz);
  }
}

//##############################################################################
///  Calcul de l'infiltration
//##############################################################################

void EvalInfiltration(double const &Pluie, double const &Lr,
                      double &Infiltration) {
  try {
    Infiltration = Pluie - Lr;

  } catch (...) {
    error_message("EvalInfiltration", USorghum);
  }
}

//##############################################################################
///  Calcul du drainage
//##############################################################################

void EvalDrainage(double const &Pluie, double const &Infiltration, double const &StockRacinaire,
                  double &StockRacinairePrec, double &Drainage) {
  try {
    Drainage = max(0., Pluie - (StockRacinaire - StockRacinairePrec) - Infiltration);
    StockRacinairePrec = StockRacinaire;

  } catch (...) {
    error_message("EvalDrainage", USorghum);
  }
}

void EvalDrainageSansCulture(double const &Pluie, double const &Infiltration,
                             double &Drainage) {
  try {
    Drainage = max(0., Pluie - Infiltration);

  } catch (...) {
    error_message("EvalDrainage", USorghum);
  }
}

//##############################################################################
///  Calcul de cumul général d'une valeur entre la deux phase PhaseDebutCumul et PhaseFinCumul
//##############################################################################

void CumulGenerique(double const &ValeurACumuler, double const &NumPhase, double const &PhaseDebutCumul, double const &PhaseFinCumul,
                    double &Cumul) {
  try {
    if (PhaseDebutCumul < 0) {
      if (NumPhase <= PhaseFinCumul)
        Cumul = Cumul + ValeurACumuler;
      else
        Cumul = 0;
    } else {
      if (((NumPhase >= PhaseDebutCumul) && (NumPhase <= PhaseFinCumul)))
        Cumul = Cumul + ValeurACumuler;
      else
        Cumul = 0;
    }

  } catch (...) {
    error_message("CumulGenerique", USorghum);
  }
}

void CumulGeneriqueComplet(double const &ValeurACumuler,
                           double &Cumul) {
  try {
    Cumul = Cumul + ValeurACumuler;

  } catch (...) {
    error_message("CumulGeneriqueComplet", USorghum);
  }
}

//##############################################################################
///  Calcul du nombre de jour de pluie entre la phase PhaseDebutNbPluie et PhaseFinNbPluie
//##############################################################################

void CompteJourPluvieux(double const &Pluie, double const &NumPhase, double const &PhaseDebutNbPluie, double const &PhaseFinNbPluie,
                        double &NbJourPluvieux) {
  try {
    if (((NumPhase >= PhaseDebutNbPluie) && (NumPhase <= PhaseFinNbPluie))) {
      if (Pluie != 0)
        NbJourPluvieux = NbJourPluvieux + 1;
    } /* TODO : voir l'initialisation si PhaseDebutNbPluie=0 */

  } catch (...) {
    error_message("CompteJourPluivieux", USorghum);
  }
}

//##############################################################################
///  Calcul du sla suivant méthodologie Mitchterlich
//##############################################################################

void EvalSlaMitch(double const &SlaMax, double const &SlaMin, double const &AttenMitch, double const &SDJ, double const &SDJLevee, double const &NumPhase,
                  double &sla) {
  try {
    if (NumPhase > 1)
      sla = SlaMin + (SlaMax - SlaMin) * pow(AttenMitch, (SDJ - SDJLevee));
    else
      sla = 0;

  } catch (...) {
    error_message("EvalSlaMitch", USorghum);
  }
}

}
#endif // SORGHUM_H
