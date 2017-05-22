/**   @file
     @brief
*/
#pragma once
#ifndef INCLUDED_METEO_H
#define INCLUDED_METEO_H

#include "delphi_defines.h"
using namespace  std;

namespace Meteo {

//Procedure InitSite(Const : double; var :double);

// 49
void EvalPar(double const &RG, double const &Kpar,
             double &Par);
// 50
void EvalDecli(double const &aDate,
               double &Decli);
// 51
void EvalSunPosi(double const &LatRad, double const &Decli,
                 double &SunPosi);
// 52
void EvalDayLength(double const &SunPosi,
                   double &DayLength);
// 53
void EvalSunDistance(double const &aDate,
                     double &SunDistance);
// 54
void EvalRayExtra(double const &SunPosi, double const &Decli, double const &SunDistance, double const &LatRad,
                  double &RayExtra);

// 55
void EvalRgMax(double const &RayExtra, double const &Alt,
               double &RgMax);

// 102
void DegToRad(double const &Lat,   double &LatRad);

// 57
void InsToRg(double const &DayLength, double const &Ins, double const &RayExtra, double const &RgMax, double const &RGLue,
             double &RGCalc);

// 33104
void AVGTempHum(double const &TMin, double const &TMax, double const &HMin, double const &HMax, double const &TMoy, double const &HMoy,
                double &TMoyCalc, double &HMoyCalc);

// 56
void EToFao(double const &ETP, double const &Alt, double const &RgMax, double const &RayGlobal, double const &TMin, double const &TMax, double const &HrMin, double const &HrMax, double const &HrMoy, double const &TMoy, double const &Vent,
            double &ETo, double &TMoyPrec, double &VPD);
// Rayonnement voir basculer dans meteo
void SucrosPAR(double RG, double LatRad, double Decli, double SunDistance, double DayLength,
               double &Par);

void SucrosPARh(double RG, double LatRad, double Decli, double SunDistance, double DayLength, double aDate, double th,
                double &PARdfh, double &PARdrh, double &sinusbetah);

// Changer en proc et un objet meteo qui
// met a nil ces variables qd on change de date puis qui
// calcul les valeurs de ses attributs s'ils sont a nil


/*****************************************************************/

/* TODO : Fonction integreGauss */

double integreheure(double duree, double th)

{
  double result;
  int p;


  try {
    switch ((int)std::trunc(th)) {
    case 1: {
      p = -1; result = 12 + 0.5 * duree * (0.5 + p * sqrt(0.15));
      break;
    }
    case 2: {
      p = 0; result = 12 + 0.5 * duree * (0.5 + p * sqrt(0.15));
      break;
    }
    case 3: {
      p = 1; result = 12 + 0.5 * duree * (0.5 + p * sqrt(0.15));
      break;
    }

    }

    // result := 12 + 0.5 * duree * (0.5 + p * sqrt(0.15));
    //intégration gaussienne pour trois heures

  } catch (...) {
    error_message("integreheure", UMeteo);
    result = -999;
  }
  result = -999;
  return result;
}

void AVGTempHum(double const &TMin, double const &TMax, double const &HMin, double const &HMax, double const &TMoy, double const &HMoy,
                double &TMoyCalc, double &HMoyCalc) {
  try {
    if (((TMin != NilValue) && (TMax != NilValue))) {
      TMoyCalc = (TMax + TMin) * 1.0 / 2;
    } else {
      TMoyCalc = TMoy;
    }
    if (((HMin != NilValue) && (HMax != NilValue))) {
      HMoyCalc = (HMax + HMin) * 1.0 / 2;
    } else {
      HMoyCalc = HMoy;
    }

  } catch (...) {
    error_message("AVGTempHum", UMeteo);
  }
}

void DegToRad(double const &Lat,   double &LatRad) {
  try {
    LatRad = Lat * M_PI * 1.0 / 180;

  } catch (...) {
    error_message("DegToRad", UMeteo);
  }
}

void EvalPar(double const &RG, double const &Kpar,   double &Par) {
  try {
    Par = Kpar * RG;

  } catch (...) {
    error_message("EvalPar", UMeteo);
  }
}

void EvalDecli(double const &aDate,   double &Decli) {
  try {
    Decli = 0.409 * sin(0.0172 * DayOfTheYear(aDate) - 1.39);

  } catch (...) {
    error_message("EvalDecli", UMeteo);
  }
}

void EvalDecli2(double const &aDate,
                double &Decli)
/*
In :
   aDate : date du jour
Out :
   Decli : déclinaison du soleil
*/
{
  try {
    Decli = -asin(sin(23.45 * M_PI * 1.0 / 180) *
                  cos(2 * M_PI * (DayOfTheYear(aDate) + 10) * 1.0 / 365));

  } catch (...) {
    error_message("EvalDecli2", UMeteo);
  }
}

void EvalSunPosi(double const &LatRad, double const &Decli,   double &SunPosi) {
  try {
    SunPosi = acos(-tan(LatRad) * tan(Decli));

  } catch (...) {
    error_message("EvalSunPosi", UMeteo);
  }
}

void EvalDayLength(double const &SunPosi,   double &DayLength) {
  try {
    DayLength = 7.64 * SunPosi;

  } catch (...) {
    error_message("EvalDayLength", UMeteo);
  }
}

void EvalDayLength2(double const &LatRad, double const &Decli,
                    double &DayLength)
/*
In :
   aDate : date du jour
   Lat : en radian
Out :
   DayLength : duree du jour en heure, minute decimal
 */

{
  try {
    DayLength = 12 + 24 * 1.0 / 180 * asin(tan(LatRad) - tan(Decli));

  } catch (...) {
    error_message("EvalDayLength2", UMeteo);
  }
}

void EvalSunDistance(double const &aDate,   double &SunDistance) {
  try {
    SunDistance = 1 + 0.033 * cos(2 * M_PI * 1.0 / 365 * DayOfTheYear(aDate));

  } catch (...) {
    error_message("EvalSunDistance", UMeteo);
  }
}

void EvalSunDistance2(double const &Decli, double const &LatRad, double const &DayLength,
                      double &SunDistance)
/*
In :
   aDate : date du jour
Out :
   SunDistance: distance relative terre soleil
*/
{
  double SinLD; double CosLD;

  try {
    SinLD = sin(LatRad) * sin(Decli);
    CosLD = cos(LatRad) * cos(Decli);
    SunDistance = 3600 *
                  (DayLength * (SinLD + 0.4 * (sqr(SinLD) + sqr(CosLD) * 0.5))
                   + 12 * 1.0 / M_PI * CosLD *
                   (2 + 3 * 0.4 * SinLD) * sqrt(1 - sqr(SinLD * 1.0 / CosLD)));

  } catch (...) {
    error_message("EvalSunDistance2", UMeteo);
  }
}

void EvalRayExtra(double const &SunPosi, double const &Decli, double const &SunDistance, double const &LatRad,
                  double &RayExtra) {
  try {
    RayExtra = 24 * 60 * 0.0820 * 1.0 / M_PI * SunDistance *
               (SunPosi * sin(Decli) * sin(LatRad) +
                cos(Decli) * cos(LatRad) * sin(SunPosi));

  } catch (...) {
    error_message("EvalRayExtra", UMeteo);
  }
}

void EvalRgMax(double const &RayExtra, double const &Alt,   double &RgMax) {
  try {
    RgMax = (0.75 + 0.00002 * Alt) * RayExtra;

  } catch (...) {
    error_message("EvalRgMax", UMeteo);
  }
}

void EToFao(double const &ETP, double const &Alt, double const &RgMax, double const &RayGlobal, double const &TMin, double const &TMax, double const &HrMin, double const &HrMax, double const &HrMoy, double const &TMoy, double const &Vent,   double &ETo, double &TMoyPrec, double &VPD) {
  double eActual; double eSat; double RgRgMax; double TLat; double delta; double KPsy; double Eaero; double Erad; double Rn; double G;

  try {
    if ((ETP == NilValue)) {
      eSat = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) +
                       exp(17.27 * TMin * 1.0 / (TMin + 237.3)));
      if ((HrMax == NilValue))
        eActual = eSat * HrMoy * 1.0 / 100;
      else
        eActual = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) *
                            HrMin * 1.0 / 100 + exp(17.27 * TMin * 1.0 / (TMin + 237.3)) *
                            HrMax * 1.0 / 100);
      VPD = eSat - eActual;
      RgRgMax = RayGlobal * 1.0 / RgMax;
      if ((RgRgMax > 1))
        RgRgMax = 1;
      Rn = 0.77 * RayGlobal - (1.35 * RgRgMax - 0.35) *
           (0.34 - 0.14 * std::pow(eActual, 0.5)) *
           (pow(TMax + 273.16, 4) + std::pow(TMin + 273.16, 4)) * 2.45015 * std::pow(10
                                                                                     , -9);
      // chaleur latente de vaporisation de l'eau
      TLat = 2.501 - 2.361 * std::pow(10, -3) * TMoy;
      //  pente de la courbe de pression de vapeur saturante en kPa/°C
      delta = 4098 * (0.6108 * exp(17.27 * TMoy * 1.0 / (TMoy + 237.3))) * 1.0 / std::pow(TMoy
              + 237.3, 2);
      // constante psychrométrique en kPa/°C
      KPsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Alt * 1.0 / 293), 5.26) * 1.0 / TLat;
      // Radiative
      G = 0.38 * (TMoy - TMoyPrec);
      Erad = 0.408 * (Rn - G) * delta * 1.0 / (delta + KPsy * (1 + 0.34 * Vent));
      // Partie évaporative de ET0 = Eaéro
      Eaero = (900 * 1.0 / (TMoy + 273.16)) * ((eSat - eActual) * Vent) * KPsy * 1.0 /
              (delta + KPsy * (1 + 0.34 * Vent));
      ETo = Erad + Eaero;
    } else {
      ETo = ETP;
    }
    TMoyPrec = TMoy;

  } catch (...) {
    error_message("EToFAO", UMeteo);
  }
}

void InsToRg(double const &DayLength, double const &Ins, double const &RayExtra, double const &RgMax, double const &RGLue,
             double &RGCalc) {
  try {
    if ((RGLue == NilValue)) {
      RGCalc = (0.25 + 0.50 * std::min(Ins * 1.0 / DayLength, 1.)) * RayExtra;
    } else {
      RGCalc = RGLue;
    }

  } catch (...) {
    error_message("InsToRg", UMeteo);
  }
}

// Rayonnement voir meteo

void SucrosPAR(double RG, double LatRad, double Decli, double SunDistance, double DayLength,
               double &Par)
// Calcul le PAR totale journalier
// In:
//    LAT: latitude du site (U: radians) (lu dans station.DB)
//    Rg: rayonnement global (U: J/m2/s) (lu dans meteo.DB en MJ/m2/s, transformé
//    en J/m2/s dans gestionmodele)
//    jour: jour calendaire (calculé dans Uchaines à partir de la date)
// InOut:
// Out:
//    PAR: PAR total sur la journée (U: micromoles/m2/j)(calculé pour gs);

{
  double sinusbeta; double SinLD; double CosLD;
  // decli: déclinaison du soleil en radians (Goudriaan et van Laar, 1994);
  // sinusbeta: sinus de l'élévation journalière du soleil;


  try {
    /* TODO : Attention convertion de RG MJ en J/m2 */
    SinLD = sin(LatRad) * sin(Decli);
    CosLD = cos(LatRad) * cos(Decli);
    sinusbeta = std::max(0., SinLD + CosLD * cos(2 * M_PI * (+12) * 1.0 / 24));
    Par = 0.5 * RG * sinusbeta * (1 + 0.4 * sinusbeta) * 1.0 / SunDistance;
    Par = Par * 1.0 / 0.2177;

  } catch (...) {
    error_message("SucrosPAR", UMeteo);
  }
}

// Attention procedure appele trois fois par jour ...
// ? gestion diff du temps heure, jour, mois ...

void SucrosPARh(double RG, double LatRad, double Decli, double SunDistance, double DayLength, double aDate, double th,
                double &PARdfh, double &PARdrh, double &sinusbetah)
// Calcul les PAR diffus et direct horaires
// In:
//    Rg: rayonnement global (U: kJ/m2/s)
//    duree: durée du jour (U: en heures);
//    haut: élévation du soleil
//    jour: jour calendaire (calculé dans Uchaines à partir de la date)
//    th: heure (calculée selon la méthode d'intégration gaussienne à partir
//    de la durée du jour)
// InOut:
// Out:
//    PARdfh : PAR diffus horaire (U: micromoles/m2/s)
//    PARdrh: PAR direct horaire (U: micromoles/m2/s)
//    sinusbetah: sinus de l'élévation horaire du soleil (Goudriaan et van Laar, 1994)
{
  double R0h; double SinLD; double CosLD;
  //R0h: rayonnement extra-terrestre instantanné en J/m2/s
  double PARhtot; double atm; double frdf;
  // PARhtotal: calculé en micromoles/m2/s et
  // atm: transmission atmospérique
  // frdf: fraction de diffus


  try {
    /* TODO : Attention convertion de RG MJ en J/m2 */
    SinLD = sin(LatRad) * sin(Decli);
    CosLD = cos(LatRad) * cos(Decli);
    sinusbetah = std::max(0., SinLD + CosLD * cos(2 * M_PI * (integreheure(DayLength
                                                                           , th) + 12) * 1.0 / 24));
    R0h = 1370 * sinusbetah * (1 + 0.033 * cos(2 * M_PI * (DayOfTheYear(aDate) -
                                                           10) * 1.0 / 365));
    // 1370 = constante solaire en J/m2/s = W/m2

    PARhtot = 0.5 * RG * sinusbetah * (1 + 0.4 * sinusbetah) * 1.0 / SunDistance;
    atm = PARhtot * 1.0 / (0.5 * R0h);

    if (atm <= 0.22)
      frdf = 1; // ciel complètement nuageux;
    else if (atm <= 0.35)
      frdf = 1 - 6.4 * sqr(atm - 0.22);
    else
      frdf = 1.47 - 1.66 * atm;
    frdf = std::max(frdf, 0.15 + 0.85 * (1 - exp(-0.1 * 1.0 / sinusbetah)));
    PARhtot = PARhtot * 1.0 / 0.2177;
    PARdfh = PARhtot * frdf;
    PARdrh = PARhtot - PARdfh;

  } catch (...) {
    error_message("SucrosPARh", UMeteo);
  }
}

/*
3. Calculs préliminaires
 Tm = (Tx + Tn) / 2
//!!! unité unique et reference a 2 m dans le stockage des donnees Vent dans la bdd
 Vitesse du vent à 2m en m/s avec correction de hauteur de mesure = u (m/s) = [vent total (Km/j)/86.4] * [4.87/ LN (67.8*H - 5.42)]
*/

void EToFAO2(double const &Alt, double const &RgMax, double const &RayGlobal, double const &TMin, double const &TMax, double const &HrMin, double const &HrMax, double const &HrMoy, double const &TMoy, double const &Vent,
             double &ETo, double &TMoyPrec, double &VPD, double &VPDMax)
/*es = 0,3054 *( exp [ 17,27 . Tx / ( Tx + 237,3) ]
      + exp [ 17,27 . Tn / ( Tn + 237,3) ])
 : pression de vapeur saturante en kPa

ed = 0,3054 * (exp [ 17,27 . Tx / ( Tx + 237,3) ] * Hn/100
                + exp [ 17,27 . Tn / ( Tn + 237,3) ] * Hx / 100)
 : pression réelle de vapeur d'eau en kPa
Si pas de donnees HrMin et Max mais seulement HrMoy
ed = es * Hm/100
 Si Rg/Rso > 1 alors alerte Rg faux et Rg/Rso = 1
 Rn = 0.77 * Rg - [1.35 * Rg/Rso - 0.35] *
      [0.34 - 0.14 * std::pow(ed,0.5) ] *
      [ std::pow(Tx + 273.16,4) + std::pow(Tn + 273.16,4) ] * 2.45015 * std::pow(10,-9)

Voir a mettre en var function
 l = 2,501 - 2,361.10-3. Tm (°C) chaleur latente de vaporisation de l'eau
 D = 4098 . ( 0,6108 . exp [ 17,27 . Tm / ( Tm + 237,3) ] ) / ( Tm + 237,3 )²
  pente de la courbe de pression de vapeur saturante en kPa/°C
 g  = 0,00163 * P / l : constante psychrométrique en kPa/°C
Voir a mettre en var function

Calcul de ETo
 G = 0.38 * (Tm j - Tm j-1 )
 Erad (mm) = 0.408 * (Rn - G) * D  / [D + g * ( 1 + 0.34 * u)]

Partie évaporative de ET0 = Eaéro
 Eaéro (mm) =  [ 900 / (Tm + 273.16)] * [(es - ed) * u ] * g / [D + g * ( 1 + 0.34 * u)]

 ET0 = Erad + Eaéro

*/
{
  double eActual; double eSat; double RgRgMax; double TLat; double delta; double KPsy; double Eaero; double Erad; double Rn; double G;

  try {
    eSat = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3))
                     + exp(17.27 * TMin * 1.0 / (TMin + 237.3)));
    // Si pas de donnees min max alors HrMax a 0 controle a lecture
    //!!! voir -999 absent?
    if (HrMax == NilValue)
      eActual = eSat * HrMoy * 1.0 / 100;
    else
      eActual = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) * HrMin * 1.0 / 100
                          + exp(17.27 * TMin * 1.0 / (TMin + 237.3)) * HrMax * 1.0 / 100);
    VPD = eSat - eActual;
    VPDMax = 0.3054 * exp(17.27 * TMax * 1.0 / (TMax + 237.3)) * (1 - HrMin * 1.0 / 100);
    RgRgMax = RayGlobal * 1.0 / RgMax;
    //!!! situation anormal Rg faux faire une sortie PB
    if (RgRgMax > 1)
      RgRgMax = 1;
    Rn = 0.77 * RayGlobal - (1.35 * RgRgMax - 0.35) *
         (0.34 - 0.14 * std::pow(eActual, 0.5)) *
         (pow(TMax + 273.16, 4) + std::pow(TMin + 273.16, 4)) * 2.45015 * std::pow(10
                                                                                   , -9);

    // chaleur latente de vaporisation de l'eau
    TLat = 2.501 - 2.361 * std::pow(10, -3) * TMoy;

    //  pente de la courbe de pression de vapeur saturante en kPa/°C
    delta = 4098 * (0.6108 * exp(17.27 * TMoy * 1.0 / (TMoy + 237.3))) * 1.0 / std::pow(TMoy +
            237.3, 2);

    // constante psychrométrique en kPa/°C
    KPsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Alt * 1.0 / 293), 5.26) * 1.0 / TLat;

    // Radiative
    G = 0.38 * (TMoy - TMoyPrec);
    Erad = 0.408 * (Rn - G) * delta * 1.0 / (delta + KPsy * (1 + 0.34 * Vent));

    // Partie évaporative de ET0 = Eaéro
    Eaero = (900 * 1.0 / (TMoy + 273.16)) * ((eSat - eActual) * Vent) * KPsy
            * 1.0 / (delta + KPsy * (1 + 0.34 * Vent));

    ETo = Erad + Eaero;
    TMoyPrec = TMoy;

  } catch (...) {
    error_message("EToFAO", UMeteo);
  }
}

void EvalEtpCalc(double const &ETP,   double &EtpCalc) {
  try {
    EtpCalc = ETP;

  } catch (...) {
    error_message("EvalEtpCalc", UMeteo);
  }
}

} //namespace
#endif//INCLUDED_METEO_H
//END
