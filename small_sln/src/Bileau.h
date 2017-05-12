/**   @file
     @brief
*/
#pragma once
#ifndef INCLUDED_BILEAU_H
#define INCLUDED_BILEAU_H
#include "delphi_defines.h"
using namespace std;

namespace Bileau {

//ok
void EvalRunOffScale(double const &Rain, double const &Irrig, double const &seuilRuiss, double const &PourcRuiss,
                     double &EauDispo, double &Lr);
//ok
void RempliRes(double const &Ru, double const &ProfRu, double const &RuSurf, double const &EauDispo, double const &StRurMax,
               double &stRu, double &stRUR, double &Hum, double &stRuSurf, double &Dr);
void RempliResDr_VN(double const &VitDr, double const &Ru, double const &ProfRu, double const &RuSurf, double const &EauDispo, double const &StRurMax,
                    double &stRu, double &stRUR, double &Hum, double &stRuSurf, double &Dr);
//ok
void EvalFTSW(double const &RuRac, double const &StockRac,
              double &ftsw);
//ok
void EvalFESW(double const &stRuSurf, double const &RuSurf,
              double &fesw);
//ok
void EvolRur(double const &Vrac, double const &Hum, double const &Ru, double const &stRuSurf, double const &RuSurf,
             double &StRurMax, double &stRUR);
//ok
void EvalKce(double const &Mulch, double const &Ltr,
             double &Kce);
//ok
void EvalKcTot(double const &Kce, double const &Kcp,
               double &KcTot);
//ok
void DemandePlante(double const &Kcp, double const &ETo,
                   double &TrPot);
//ok
void DemandePlante_V2_1(double const &Kcp, double const &ETo, double const &Ca, double const &CO2SlopeTr,
                        double &TrPot, double &CoeffCO2Tr);
//ok
void DemandeSol(double const &Etp, double const &Kce,
                double &evapPot);
//ok
void EvapRuSurf(double const &evapPot, double const &fesw, double const &stRuSurf,
                double &evap);
//ok
void SeuilCSTR(double const &TrPot, double const &ParSeuilCSTR, double const &ftsw,
               double &tr);
//ok
void ExpCSTR(double const &Par1ExpCSTR, double const &Par2ExpCSTR, double const &ftsw,
             double &cstr);
//ok
void KHR(double const &ParKhr, double const &ftsw,
         double &cstr);
//ok
void CstrEagle(double const &Etp, double const &ftsw,
               double &cstr);
//ok
void CstrIsohydrique(double const &Etp, double const &ftsw, double const &Kexponent,
                     double &cstr);
//ok
void CstrPFactor(double const &Kcp, double const &ParPFact, double const &ftsw, double const &Etp,
                 double &cstr);
//ok
void EvalTranspi(double const &TrPot, double const &cstr,
                 double &tr);
//ok
void EtrMinMax(double const &TrPot, double const &tr, double const &evap, double const &evapPot,
               double &etr, double &etm);
//ok
void ConsoRes(double const &etr, double const &RuSurf, double const &etm, double const &StRurMax,
              double &stRUR, double &stRu, double &stRuSurf, double &tr, double &TrPot);
//ok
void ConsoResSep(double const &StRurMax, double const &RuSurf, double const &evap, double const &TrPot, double const &evapPot,
                 double &tr, double &stRUR, double &stRuSurf, double &stRu, double &etr, double &etm);

//  Procedure InitPlot(Const StockIni,Ru,ProfRacIni,Profondeur,DateSemis: double; var StSurf,StTot,Ltr,Hum,StRurMax,RuSurf, ProfRU:double);
void InitPlot(double const &StockIniSurf, double const &StockIniProf, double const &Ru, double const &ProfRacIni, double const &EpaisseurSurf, double const &EpaisseurProf, double const &DateSemis,   double &StSurf, double &StTot, double &Ltr, double &Hum, double &StRurMax, double &RuSurf, double &ProfRu);


/************************************************************************/

//##############################################################################
//-------------------------------------------------------------------------//
// Initialisation des variables de plot
//-------------------------------------------------------------------------//
// Correction VB: Auparavent, les sols étient au nombre de deux. Il y a maintenant un unique sol avec deux profondeurs
// Procedure InitPlot(Const StockIni,Ru,ProfRacIni,Profondeur,DateSemis: double; var StSurf,StTot,Ltr,Hum,StRurMax,RuSurf, ProfRU:double);
void InitPlot(double const &StockIniSurf, double const &StockIniProf, double const &Ru, double const &ProfRacIni, double const &EpaisseurSurf, double const &EpaisseurProf, double const &DateSemis,   double &StSurf, double &StTot, double &Ltr, double &Hum, double &StRurMax, double &RuSurf, double &ProfRu) {
  try {
    StSurf = StockIniSurf;
    Ltr = 1;
    StRurMax = Ru * ProfRacIni * 1.0 / 1000;
    RuSurf = EpaisseurSurf * 1.0 / 1000 * Ru;
    StTot = StockIniSurf + StockIniProf;
    ProfRu = EpaisseurSurf + EpaisseurProf;
    Hum = max(StTot, StRurMax);

  } catch (...) {
    AfficheMessageErreur("InitPlot", UBilEau);
  }
}
/*------------------------------------------------*/
void EvalRunOffScale(double const &Rain, double const &Irrig, double const &seuilRuiss, double const &PourcRuiss,
                     double &EauDispo, double &Lr) {
  double EnPlus;

  try {
    Lr = 0;
    if ((Irrig == NilValue)) {
      EnPlus = Rain;
    } else {
      EnPlus = Rain + Irrig;
    }
    if ((EnPlus > seuilRuiss)) {
      Lr = (EnPlus - seuilRuiss) * PourcRuiss * 1.0 / 100;
    }
    EauDispo = EnPlus - Lr;

  } catch (...) {
    AfficheMessageErreur("EvalRunOffScale", UBilEau);
  }
}

/*------------------------------------------------*/
void EvalRunOffIrrigAuto(double const &Rain, double const &Irrigation, double const &IrrigAuto, double const &IrrigAutoTarget, double const &MaxIrrig, double const &Precision, double const &StockRac, double const &RuRac, double const &seuilRuiss, double const &PourcRuiss,
                         double &EauDispo, double &Lr, double &IrrigTotDay) {
  double EnPlus;
  double CorrectedIrrigation;

  try {
    IrrigTotDay = 0;
    Lr = 0;
    if ((Irrigation == NilValue)) {
      CorrectedIrrigation = 0;
    } else {
      CorrectedIrrigation = Irrigation;
    }
    if (((IrrigAuto == 1) && (RuRac > 0))) {
      //mainForm.memDeroulement.Lines.Add('StockRac : ' + FloatToStr(StockRac));
      //mainForm.memDeroulement.Lines.Add('Rurac : ' + FloatToStr(Rurac));
      //mainForm.memDeroulement.Lines.Add('CorrectedIrrigation : ' + FloatToStr(CorrectedIrrigation));

      if (((StockRac * 1.0 / RuRac) < IrrigAutoTarget)) {
        IrrigTotDay = (RuRac - StockRac) * 0.8;
        //IrrigTotDay := Rurac - Stockrac;
        //mainForm.memDeroulement.Lines.Add('IrrigTotDay : ' + FloatToStr(IrrigTotDay));
        IrrigTotDay = max(0., IrrigTotDay - CorrectedIrrigation);
        //mainForm.memDeroulement.Lines.Add('IrrigTotDay : ' + FloatToStr(IrrigTotDay));
        IrrigTotDay = min(IrrigTotDay, MaxIrrig);
        //mainForm.memDeroulement.Lines.Add('IrrigTotDay : ' + FloatToStr(IrrigTotDay));
        IrrigTotDay = round(IrrigTotDay * 1.0 / Precision) * Precision;
        //mainForm.memDeroulement.Lines.Add('IrrigTotDay : ' + FloatToStr(IrrigTotDay));
        //showmessage('stop');
      }
    }
    IrrigTotDay = CorrectedIrrigation + IrrigTotDay;
    EnPlus = Rain + IrrigTotDay;
    if ((EnPlus > seuilRuiss)) {
      Lr = (EnPlus - seuilRuiss) * PourcRuiss * 1.0 / 100;
    }
    EauDispo = EnPlus - Lr;

  } catch (...) {
    AfficheMessageErreur("EvalRunOffScale", UBilEau);
  }
}

/*------------------------------------------------*/
/*      Procedure CalculRuisold;
{
A modifier
Calcul le ruissellement
1) methode des seuils : le ruissellement est egal a un pourcentage
de la quantite de pluie supérieure au seuil definie
2) methode Orstom(IRD) : calibre au senegal, evalue le ruissellement
sur un sol nu en fonction du type de sol en surface (rugosite...) de la pente
definissant les 3 paramètres. Il tient compte aussi de l'interval de temps
entre 2 evennements pluvieux (jours)
VOIR ajouter une methode reduisant le ruissellement par le biais de
ltr, lors de la croissance de la plante
}
      Var
        EnPlus     : Double;

      Begin
        TRes[Lr]:=0;
        Enplus := TJ[NBJSimul,PP]+TJ[NBJSimul,Irr];
        Case Parcelle.TypeRuiss of
          2 : If Enplus>Parcelle.SeuilRuiss
              then TRes[Lr]:=(Enplus - Parcelle.SeuilRuiss) * Parcelle.PourcRuiss/100;
          3 : Begin
                If Parcelle.CodeRuiss < 3 Then
                Begin
                  TRes[Lr]:= TParamRuiss[Parcelle.CodeRuiss,1]*Enplus+
                             TParamRuiss[Parcelle.CodeRuiss,2]*TRes[Ik]+
                             TParamRuiss[Parcelle.CodeRuiss,3]*TRes[Ik]*Enplus
                             - TParamRuiss[Parcelle.CodeRuiss,4];
                End
                Else
                Begin
                  TRes[Lr]:= Parcelle.RuissPerso[1]*Enplus+
                             Parcelle.RuissPerso[2]*TRes[Ik]+
                             Parcelle.RuissPerso[3]*TRes[Ik]*Enplus
                             - Parcelle.RuissPerso[4];
                End;
                If TRes[Lr] < 0 then TRes[Lr] := 0;
                TRes[ik] := (TRes[ik]+(Enplus - TRes[Lr]))* exp(-0.5)
              End;
        End;
        TRes[EauDispo]:= Enplus-TRes[Lr];
      End;
*/
/*------------------------------------------------*/
void RempliRes(double const &Ru, double const &ProfRu, double const &RuSurf, double const &EauDispo, double const &StRurMax,
               double &stRu, double &stRUR, double &Hum, double &stRuSurf, double &Dr)
/*
Hypotheses :
On a une dynamique de l'eau represente par un remplissage par le haut
et une evolution des tailles de reservoirs quand ce remplissage est sup
a la quantite maximum de la taille en cours (front d'humectation).
Quand on a atteind la taille maximum par remplissage on considere
que c'est du drainage.
A l'interieur d'un reservoir l'eau est repartie de maniere homogene
(peu etre considere valable jusqu'a 2m de profondeur, dixit CB
 d'apres d'autres sources).

3 representation des reservoirs permettant de simuler 3 reservoirs:
1)ensemble des reservoirs en eau, evoluant en profondeur en fonction
 du front d'humectation
2) reservoir de surface (taille fixe)ou s'effectue l'evaporation et une part de la
transpiration en presence de racines
Modif : on a ajoute l'evaporation en dessous du seuil Pf4.2 estime a
la moitie de la RU.
3) reservoir contenant les racines, evoluant en fonction du front racinaire
REMARQUE : Ces reservoirs se chevauche
Au lieu de gerer des profondeurs on gere des stocks d'eau
(stRuMax stock d'eau maxi (RU * prof. Max)*/
{
  double StRuMax;
  double StRuSurfPrec; double EauTranspi;

  try {
    StRuMax = Ru;
    StRuMax = StRuMax * ProfRu * 1.0 / 1000;
    // Rempli Res surface
    StRuSurfPrec = stRuSurf;
    stRuSurf = min(stRuSurf + EauDispo, RuSurf + RuSurf * 1.0 / 10);
    // enleve la qte d'eau remplissant la partie uniquement evaporable
    if ((StRuSurfPrec < RuSurf * 1.0 / 10)) {
      EauTranspi = max(0., EauDispo - (RuSurf * 1.0 / 10 - StRuSurfPrec));
    } else {
      EauTranspi = EauDispo;
    }
    // remplissage des autres réservoirs
    // rempli res en eau, estime drainage, et evolution du front d'humectation
    stRu = stRu + EauTranspi;
    if ((stRu > StRuMax)) {
      Dr = stRu - StRuMax;
      stRu = StRuMax;
    } else {
      Dr = 0;
    }
    Hum = max(Hum, stRu);
    // Rempli res racines
    stRUR = min(stRUR + EauTranspi, StRurMax);
    stRUR = min(stRUR, stRu);

  } catch (...) {
    AfficheMessageErreur("RempliRes", UBilEau);
  }
}



/*----------------------VN--------------------------*/
void RempliResDr_VN(double const &VitDr, double const &Ru, double const &ProfRu, double const &RuSurf, double const &EauDispo, double const &StRurMax,
                    double &stRu, double &stRUR, double &Hum, double &stRuSurf, double &Dr) {
  double StRuMax;
  double StRuSurfPrec; double EauTranspi;

  try {
    StRuMax = Ru;
    StRuMax = StRuMax * ProfRu * 1.0 / 1000;
    // Rempli Res surface
    StRuSurfPrec = stRuSurf;
    stRuSurf = min(stRuSurf + EauDispo, RuSurf + RuSurf * 1.0 / 10);
    // enleve la qte d'eau remplissant la partie uniquement evaporable
    if (StRuSurfPrec < RuSurf * 1.0 / 10)
      EauTranspi = max(0., EauDispo - (RuSurf * 1.0 / 10 - StRuSurfPrec)); else EauTranspi = EauDispo;

    // remplissage des autres réservoirs
    // rempli res en eau, estime drainage, et evolution du front d'humectation
    stRu = stRu + EauTranspi;
    if (stRu > StRuMax) {
      Dr = min(stRu - StRuMax, VitDr);

      stRu = stRu - Dr;

    } else
      Dr = 0;
    Hum = max(Hum, stRu);
    // Rempli res racines
    stRUR = min(stRUR + EauTranspi, StRurMax);
    stRUR = min(stRUR, stRu);

  } catch (...) {
    AfficheMessageErreur("RempliResVitDr", UBilEau);
  }
}
///////////////////////////////////////////////////////////////////////////////

void EvalFTSW(double const &RuRac, double const &StockRac,
              double &ftsw)
/*
Estimation de la fraction d'eau transpirable, rapporte donc au reservoir
contenant les racines
// Parametres
IN:
RuRac : mm
StockRac : mm
OUT:
ftsw : mm
*/
{
  try {
    if ((RuRac > 0)) {
      ftsw =  StockRac * 1.0 / RuRac;
    } else {
      ftsw = 0;
    }

  } catch (...) {
    AfficheMessageErreur("EvalFTSW | StRurMax: " + FloatToStr(RuRac) + " StRur: " + FloatToStr(StockRac) + " ftsw: " + FloatToStr(ftsw), UBilEau);
  }
}
/*------------------------------------------------*/
void EvalFESW(double const &stRuSurf, double const &RuSurf,
              double &fesw)
/*
Estimation de la fraction d'eau evaporable, rapporte donc au reservoir
de surface, RuSurf est le stock d'eau maxi disponible pour la plante
sur ce reservoir
Modif : on considere que pour l'évaporation la moitie de cette
valeur doit etre ajoutée.
// Parametres
IN:
StRusurf : mm
RuSurf : mm
OUT:
fesw : mm
*/
{
  try {
    fesw = stRuSurf * 1.0 / (RuSurf + RuSurf * 1.0 / 10);

  } catch (...) {
    AfficheMessageErreur("EvalFESW", UBilEau);
  }
}
/*------------------------------------------------*/
void EvolRur(double const &Vrac, double const &Hum, double const &Ru, double const &stRuSurf, double const &RuSurf,
             double &StRurMax, double &stRUR)
/*
La vitesse d'enracinement potentielle de la plante peut etre bloque
par manque d'eau en profondeur (Hum). La profondeur d'humectation
est convertie en quantite d'eau maximum equivalente
// Parametres
IN:
Vrac : mm
Hum : mm
StRuSurf : mm
RU : mm/m
RuSurf : mm/m
INOUT:
stRurMax : mm
stRur : mm
*/
{
  double DeltaRur;


  try {
// on prend une partie si blocage ou toute la nouvelle prof
// avec une quantite d'eau au max.
    if ((Hum - StRurMax) < (Vrac * 1.0 / 1000 * Ru)) DeltaRur = Hum - StRurMax; else DeltaRur = Vrac * 1.0 / 1000 * Ru;
    StRurMax = StRurMax + DeltaRur;
// si les racines vont au dela du stock de surface
// on ajoute le deltaRUR
// sinon on prend au prorata de la profondeur et du stock de surface
    if (StRurMax > RuSurf)
      stRUR = stRUR + DeltaRur;
    else
      stRUR = max((stRuSurf - RuSurf * 1.0 / 10) * (StRurMax * 1.0 / RuSurf), 0.);

  } catch (...) {
    AfficheMessageErreur("EvolRur | RuSurf: " + FloatToStr(RuSurf), UBilEau);
  }
}

/*------------------------------------------------*/
void EvolRurCstr(double const &Vrac, double const &Hum, double const &Ru, double const &stRuSurf, double const &RuSurf, double const &cstr,
                 double &StRurMax, double &stRUR)
/*
La vitesse d'enracinement potentielle de la plante peut etre bloque
par manque d'eau en profondeur (Hum). La profondeur d'humectation
est convertie en quantite d'eau maximum equivalente
// Parametres
IN:
Vrac : mm
Hum : mm
StRuSurf : mm
RU : mm/m
RuSurf : mm/m
INOUT:
stRurMax : mm
stRur : mm
*/
{
  double DeltaRur; double DayVrac;

  try {
    // on prend une partie si blocage ou toute la nouvelle prof
    // avec une quantite d'eau au max.
    DayVrac = (Vrac * min(cstr + 0.3, 1.)) * 1.0 / 1000 * Ru;
    if (((Hum - StRurMax) < DayVrac)) {
      DeltaRur = Hum - StRurMax;
    } else {
      DeltaRur = DayVrac;
    }
    StRurMax = StRurMax + DeltaRur;
    // si les racines vont au dela du stock de surface
    // on ajoute le deltaRUR
    // sinon on prend au prorata de la profondeur et du stock de surface
    if ((StRurMax > RuSurf)) {
      stRUR = stRUR + DeltaRur;
    } else {
      stRUR = max((stRuSurf - RuSurf * 1.0 / 10) * (StRurMax * 1.0 / RuSurf), 0.);
    }

  } catch (...) {
    AfficheMessageErreur("EvolRur | RuSurf: " + FloatToStr(RuSurf), UBilEau);
  }
}

//////////////////////////////////////////////////////////////////////////

void EvalKce(double const &Mulch, double const &Ltr,
             double &Kce) {
  try {
    Kce = Mulch * 1.0 / 100 * Ltr;

  } catch (...) {
    AfficheMessageErreur("EvalKce", UBilEau);
  }
}

//////////////////////////////////////////////////////////////////////////
/*------------------------------------------------*/
void EvalKcTot(double const &Kce, double const &Kcp,
               double &KcTot) {
  try {
    if ((Kcp == NilValue)) {
      KcTot = Kce;
    } else {
      KcTot = Kce + Kcp;
    }

  } catch (...) {
    AfficheMessageErreur("EvalKcTot", UBilEau);
  }
}

///////////////////////////////////////////////////////////////////////////

void DemandePlante(double const &Kcp, double const &ETo,   double &TrPot) {
  try {
    TrPot = Kcp * ETo;

  } catch (...) {
    AfficheMessageErreur("DemandePlante", UBilEau);
  }
}
/*------------------------------------------------*/

void DemandePlante_V2_1(double const &Kcp, double const &ETo, double const &Ca, double const &CO2SlopeTr,   double &TrPot, double &CoeffCO2Tr) {
  try {
    TrPot = Kcp * ETo;
    CoeffCO2Tr = Ca * CO2SlopeTr - 400 * CO2SlopeTr + 1; // Coefficient for TrPot response to ambient CO2 (Ca), set to 1 for Ca=400ppm (ambient 2013)
    TrPot = TrPot * CoeffCO2Tr;

  } catch (...) {
    AfficheMessageErreur("DemandePlante_V2_1", UBilEau);
  }
}
/*------------------------------------------------*/

void DemandeSol(double const &Etp, double const &Kce,
                double &evapPot)
/*
Estimation de l'evaporation potentielle du sol, on ne tient pas
compte d'une variation de l'evaporation en fonction d'une humectation
differente entre le haut et le bas du reservoir, on a un parametre
mulch qui peu traduire le phenomene d'auto mulching (defaut : 0.7)
qui peu aussi traduire un mulch par couverture vegetale ou...
La reduction de l'evaporation par l'evolution de la couverture
du sol par la plante est traduit par ltr.

// Parametres
IN:
ETo : mm
Kce : %
OUT:
evapPot : mm
*/
{
  try {
    evapPot = Etp * Kce;

  } catch (...) {
    AfficheMessageErreur("DemandeSol", UBilEau);
  }
}
/*------------------------------------------------*/
void EvapRuSurf(double const &evapPot, double const &fesw, double const &stRuSurf,
                double &evap)
/*
Estimation de l'evaporation relle, rapporte a la fraction d'eau evaporable
// Parametres
IN:
fesw : mm
evapPot : mm
stRuSurf : mm
OUT:
evap : mm
*/
{
  try {
    evap = evapPot * fesw * fesw;
    evap = min(evap, stRuSurf);

  } catch (...) {
    AfficheMessageErreur("EvapRuSurf", UBilEau);
  }
}

/*------------------------------------------------*/
void EvapRuSurfFesw(double const &evapPot, double const &fesw, double const &stRuSurf,
                    double &evap)
/*
Estimation de l'evaporation relle, rapporte a la fraction d'eau evaporable
// Parametres
IN:
fesw : mm
evapPot : mm
stRuSurf : mm
OUT:
evap : mm
*/
{
  try {
    evap = evapPot * fesw;
    evap = min(evap, stRuSurf);

  } catch (...) {
    AfficheMessageErreur("EvapRuSurfFesw", UBilEau);
  }
}
//////////////////////////////////////////////////////////////////////

/*
Ensemble de procedure permettant d'estimer l'indice de contrainte
de consommation d'eau par transpiration, fonction du sol (fesw)
et de la plante
*/

void SeuilCSTR(double const &TrPot, double const &ParSeuilCSTR, double const &ftsw,
               double &tr) {
  try {
    tr = min(TrPot, TrPot * ParSeuilCSTR * ftsw);

  } catch (...) {
    AfficheMessageErreur("SeuilCSTR", UBilEau);
  }
}
/*------------------------------------------------*/
void ExpCSTR(double const &Par1ExpCSTR, double const &Par2ExpCSTR, double const &ftsw,
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
  try {
    cstr = 1 * 1.0 / (1 + exp(-(ftsw - Par1ExpCSTR) * 1.0 / Par2ExpCSTR));

  } catch (...) {
    AfficheMessageErreur("ExpCSTR", UBilEau);
  }
}
/*------------------------------------------------*/
void KHR(double const &ParKhr, double const &ftsw,
         double &cstr) {
  try {
    cstr = min(1., ParKhr * ftsw * ftsw);

  } catch (...) {
    AfficheMessageErreur("KHR", UBilEau);
  }
}
/*------------------------------------------------*/
void CstrEagle(double const &Etp, double const &ftsw,
               double &cstr)
/*
Estimation de l'indice de contrainte de consommation d'eau
// Parametres
IN:
ftsw : mm
ETo : mm
OUT:
cstr : %
*/
{
  try {
    cstr = ((-0.05 + 0.732 * 1.0 / Etp)
            + (4.97 - 0.66 * Etp) * ftsw
            + (-8.57 + 1.56 * Etp) * ftsw * ftsw
            + (4.35 - 0.88 * Etp)
            * std::pow(ftsw, 3));
    // bornage pour les faibles ETP CSTR > 1 vers ETP < 3 mm/j
    if (cstr > 1)
      cstr = 1;

  } catch (...) {
    AfficheMessageErreur("KHR", UBilEau);
  }
}
/*------------------------------------------------*/
void CstrIsohydrique(double const &Etp, double const &ftsw, double const &Kexponent,
                     double &cstr) {
  try {
    cstr = std::min(1., 2 * ftsw) * std::pow((1 * 1.0 / (Etp + 1)), Kexponent);

  } catch (...) {
    AfficheMessageErreur("CstrIsohydrique", UBilEau);
  }
}

/*------------------------------------------------*/
void CstrPFactor(double const &Kcp, double const &ParPFact, double const &ftsw, double const &Etp,   double &cstr) {
  double pFact;

  try {
    pFact = ParPFact + 0.04 * (5 - max(Kcp, 1.) * Etp);
    pFact = max(0.1, pFact);
    pFact = min(0.8, pFact);
    cstr = min((ftsw * 1.0 / (1 - pFact)), 1.);
    cstr = max(0., cstr);

  } catch (...) {
    AfficheMessageErreur("CstrPFactor | pFact: " + FloatToStr(pFact), UBilEau);
  }
}
/*------------------------------------------------*/
void CstrPFactorTropDo(double const &Kcp, double const &ParPFact, double const &ftsw, double const &Etp, double const &SeuilFTSW,   double &cstr)
/*
Estimation de l'indice de contrainte de consommation d'eau
Modif :
// Parametres
IN:
Kctot :
parPfact :
ftsw : mm
ETo : mm
OUT:
cstr : %
*/
{
  double pFact;

  try {
    /* Eagle et trPot= Kc*Etp*/
    //        pFact := ParPFact + 0.04 * (5 - Kctot * ETP);
    pFact = ParPFact + 0.04 * (5 - max(Kcp, 1.) * Etp);
    pFact = max(0.1, pFact);
    pFact = min(0.8, pFact);
    cstr = min((ftsw * 1.0 / (1 - pFact)), 1.);
    cstr = max(0., cstr);
//  If (cstr = 1) and (ftsw > SeuilFTSW) Then cstr := max (1-ftsw, 0);


  } catch (...) {
    AfficheMessageErreur("CstrPFactorTropDo | pFact: " + FloatToStr(pFact), UBilEau);
  }
}
/*------------------------------------------------*/
void EvalTranspi(double const &TrPot, double const &cstr,   double &tr) {
  try {
    tr = TrPot * cstr;

  } catch (...) {
    AfficheMessageErreur("EvalTranspi", UBilEau);
  }
}

/*------------------------------------------------*/
void EtrMinMax(double const &TrPot, double const &tr, double const &evap, double const &evapPot,
               double &etr, double &etm)
/*
Non separation de tr et evap on prend la plus grande valeur
estimee (evapotranspiration du reservoir de surface ou
evapotranspiration de la plante/sol Kc plante et sol)
pour definir la valeur de l'etr qui sera consomme sur les reservoirs
les valeurs tr et evap doivent ici etre comprises comme :
tr = etr plante sol et evap = etr du reservoir de surface considere
comme un sol nu (ie ltr = 1) pour pouvoir reste compatible avec
la version de SARRA
// Parametres
IN:
tr : mm
trPot : mm
evap : mm
evapPot : mm
OUT:
etr : mm
etm : mm
*/
{
  try {
    if (tr > evap) {
      etr = tr;
      etm = TrPot;
    } else {
      etr = evap;
      etm = evapPot;
    }

  } catch (...) {
    AfficheMessageErreur("EtrMinMax", UBilEau);
  }
}
/*------------------------------------------------*/
void ConsoRes(double const &etr, double const &RuSurf, double const &etm, double const &StRurMax,
              double &stRUR, double &stRu, double &stRuSurf, double &tr, double &TrPot)
/*
Non separation de tr et evap, consommation d'eau sur les reservoirs
// Parametres
IN:
etm : mm
etr : mm
RuSurf : mm
stRurMax :mm
INOUT:
stRur : mm
stRu : mm
stRuSurf : mm
Out:
Tr : mm
trPot : mm
*/
{
  try {
    stRu = max(stRu - etr, 0.);
    stRuSurf = max(stRuSurf - etr, 0.);
    if (StRurMax >=  RuSurf)
      stRUR = max(stRUR - etr, 0.);
    else
      stRUR = max((stRuSurf - RuSurf * 1.0 / 10) * StRurMax * 1.0 / RuSurf, 0.);
    tr = etr;
    TrPot = etm;

  } catch (...) {
    AfficheMessageErreur("ConsoRes", UBilEau);
  }
}
/*------------------------------------------------*/
void ConsoResSep(double const &StRurMax, double const &RuSurf, double const &evap, double const &TrPot, double const &evapPot,
                 double &tr, double &stRUR, double &stRuSurf, double &stRu, double &etr, double &etm)
/*
Separation de tr et evap. Consommation de l'eau sur les reservoirs
Hypothese : l'evaporation est le processus le plus rapide, retranche
en premier sur le reservoir de surface. Comme reservoir de surface
et reservoirs racinaires se chevauchent, il nous faut aussi calcule sur
le reservoir ayant des racines la part deja extraite pour l'evaporation.
Quand la profondeur des racines est inferieur au reservoir de surface
on ne consomme en evaporation que la fraction correspondant a cette
profondeur sur celle du reservoir de surface (consoRur).
Les estimations d'evaporation et de transpirations sont effectues
separemment, on peut ainsi avoir une consommation legerement superieure
a l'eau disponible. On diminuera donc la transpiration en consequence.

Modif : Pour les stock d'eau on tient compte de la partie rajoutee au
reservoir de surface qui ne peut etre que evapore (air dry)
// Parametres
IN:
stRurMax : mm
RuSurf : mm
evap : mm
trPot : mm
evaPot : mm
INOUT :
stRuSurf : mm
tr : mm
stRur : mm
stRu : mm
OUT:
etr : mm
etm : mm
*/
{
  double ConsoRur; double TrSurf;

  try {
    // part transpirable sur le reservoir de surface
    TrSurf = max(0., stRuSurf - RuSurf * 1.0 / 10);
    // qte d'eau evapore a consommer sur le reservoir de surface
    stRuSurf = max(stRuSurf - evap, 0.);
    // qte d'eau evapore a retirer sur la part transpirable
    if ((evap > TrSurf)) {
      ConsoRur = TrSurf;
    } else {
      ConsoRur = evap;
    }
    stRu = max(0., stRu - ConsoRur);
    // fraction d'eau evapore sur la part transpirable qd les racines sont moins
    // profondes que le reservoir de surface, mise a jour des stocks transpirables
    if ((StRurMax <  RuSurf)) {
      ConsoRur = evap * stRUR * 1.0 / RuSurf;
    }
    stRUR = max(0., stRUR - ConsoRur);
    // reajustement de la qte transpirable considerant que l'evap a eu lieu avant
    // mise a jour des stocks transpirables
    if ((tr > stRUR)) {
      tr = max(stRUR - tr, 0.);
    }
    if ((stRUR > 0)) {
      stRuSurf = max(stRuSurf - (tr * min(TrSurf * 1.0 / stRUR, 1.)), 0.);
    }
    stRUR = max(0., stRUR - tr);
    stRu = max(0., stRu - tr);
    etr = tr + evap;
    etm = TrPot + evap;

  } catch (...) {
    AfficheMessageErreur("ConsoResSep", UBilEau);
  }
}
////////////////////////////////////////////////////////////////////////////////



void EvalRUE(double const &ParIntercepte, double const &BiomasseAerienne, double const &ChangePhase, double const &NumPhase,
             double &CumPar, double &RUE) {
  try {
    if ((NumPhase < 2) || ((NumPhase == 2) && (ChangePhase == 1))) {
      CumPar = 0;
      RUE = 0;
    } else {
      CumPar = CumPar + ParIntercepte;
      RUE = BiomasseAerienne * 1.0 / CumPar;
    }

  } catch (...) {
    AfficheMessageErreur("EvalRUE", UBilEau);
  }
}

} //namespace

#endif//INCLUDED_BILEAU_H
//END
