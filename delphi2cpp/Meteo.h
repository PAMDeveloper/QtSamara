#ifndef METEO_H
#define METEO_H
#include "parameters.h"

//global variables
extern double DayLength;
extern double Decli;
extern double Par;
extern double RayExtra;
extern double RgCalc;
extern double RgMax;
extern double SunPosi;
extern double TmaxMoy;
extern double TminMoy;
extern double TMoyCalc;
extern double TMoyPrec;
extern double ETo;
extern double HMoyCalc;
extern double LatRad;
extern double SunDistance;
extern double VPDCalc;


//every steps
extern double TMax;
extern double TMin;
extern double TMoy;
extern double HMax;
extern double HMin;
extern double HMoy;
extern double Vt;
extern double Ins;
extern double Rg;
extern double ETP;
extern double Pluie;

namespace meteo {
const samara::ModelParameters * parameters;

void init_meteo(const samara::ModelParameters& p) {
    parameters = &p;
    DayLength = 0;
    Decli = 0;
    ETo = 0;
    HMoyCalc = 0;
    LatRad = 0;
    Par = 0;
    RayExtra = 0;
    RgCalc = 0;
    RgMax = 0;
    SunDistance = 0;
    SunPosi = 0;
    TmaxMoy = 0;
    TminMoy = 0;
    TMoyCalc = 0;
    TMoyPrec = 0;
    VPDCalc = 0;
}

void update_meteo(double t){
    TMax = parameters->get(t).TMax;
    TMin = parameters->get(t).TMin;
    TMoy = parameters->get(t).TMoy;
    HMax = parameters->get(t).HMax;
    HMin = parameters->get(t).HMin;
    HMoy = parameters->get(t).HMoy;
    Vt = parameters->get(t).Vt;
    Ins = parameters->get(t).Ins;
    Rg = parameters->get(t).Rg;
    ETP = parameters->get(t).ETP;
    Pluie = parameters->get(t).Rain;

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
      AfficheMessageErreur("AVGTempHum", UMeteo);
    }
}


void eval_Par(double t) {
  try {
    Decli = 0.409 * sin(0.0172 * DayOfTheYear(t) - 1.39);
    SunPosi = acos(-tan(LatRad) * tan(Decli));
    DayLength = 7.64 * SunPosi;
    RayExtra = 24 * 60 * 0.0820 * 1.0 / M_PI * SunDistance *
               (SunPosi * sin(Decli) * sin(LatRad) +
                cos(Decli) * cos(LatRad) * sin(SunPosi));
    RgMax = (0.75 + 0.00002 * Altitude) * RayExtra;

    if ((Rg == NilValue)) {
      RgCalc = (0.25 + 0.50 * std::min(Ins * 1.0 / DayLength, 1.)) * RayExtra;
    } else {
      RgCalc = Rg;
    }

     Par = KPar * RgCalc;
  } catch (...) {
    AfficheMessageErreur("EvalRg", UMeteo);
  }
}

void EToFao() {

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

  try {
    if ((ETP == NilValue)) {
      eSat = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) +
                       exp(17.27 * TMin * 1.0 / (TMin + 237.3)));
      if ((HMax == NilValue))
        eActual = eSat * HMoyCalc * 1.0 / 100;
      else
        eActual = 0.3054 * (exp(17.27 * TMax * 1.0 / (TMax + 237.3)) *
                            HMin * 1.0 / 100 + exp(17.27 * TMin * 1.0 / (TMin + 237.3)) *
                            HMax * 1.0 / 100);
      VPDCalc = eSat - eActual;
      RgRgMax = RgCalc * 1.0 / RgMax;
      if ((RgRgMax > 1))
        RgRgMax = 1;
      Rn = 0.77 * RgCalc - (1.35 * RgRgMax - 0.35) *
           (0.34 - 0.14 * std::pow(eActual, 0.5)) *
           (pow(TMax + 273.16, 4) + std::pow(TMin + 273.16, 4)) * 2.45015 * std::pow(10
                                                                                     , -9);
      // chaleur latente de vaporisation de l'eau
      TLat = 2.501 - 2.361 * std::pow(10, -3) * TMoy;
      //  pente de la courbe de pression de vapeur saturante en kPa/°C
      delta = 4098 * (0.6108 * exp(17.27 * TMoy * 1.0 / (TMoy + 237.3))) * 1.0 / std::pow(TMoy
              + 237.3, 2);
      // constante psychrométrique en kPa/°C
      KPsy = 0.00163 * 101.3 * std::pow(1 - (0.0065 * Altitude * 1.0 / 293), 5.26) * 1.0 / TLat;
      // Radiative
      G = 0.38 * (TMoy - TMoyPrec);
      Erad = 0.408 * (Rn - G) * delta * 1.0 / (delta + KPsy * (1 + 0.34 * Vt));
      // Partie évaporative de ET0 = Eaéro
      Eaero = (900 * 1.0 / (TMoy + 273.16)) * ((eSat - eActual) * Vt) * KPsy * 1.0 /
              (delta + KPsy * (1 + 0.34 * Vt));
      ETo = Erad + Eaero;
    } else {
      ETo = ETP;
    }
    TMoyPrec = TMoy;

  } catch (...) {
    AfficheMessageErreur("EToFAO", UMeteo);
  }
}

}


#endif // METEO_H
