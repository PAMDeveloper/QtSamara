#ifndef METEO_H
#define METEO_H
#include "parameters.h"

double TMax = 0;
double TMin = 0;
double TMoy = 0;
double HMax = 0;
double HMin = 0;
double HMoy = 0;
double Vt = 0;
double Ins = 0;
double Rg = 0;
double ETP = 0;
double Pluie = 0;

namespace meteo {
const samara::ModelParameters * parameters;

void init_meteo(const samara::ModelParameters& p) {
    parameters = &p;
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
}


void AVGTempHum() {
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

void EvalPar(double t) {
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


void EvalPar(double const &RG, double const &Kpar,   double &Par) {
  try {


  } catch (...) {
    AfficheMessageErreur("EvalPar", UMeteo);
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
    AfficheMessageErreur("EToFAO", UMeteo);
  }
}

}


#endif // METEO_H
