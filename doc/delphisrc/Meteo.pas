unit Meteo;

interface

//Procedure InitSite(Const : double; var :double);

// 49
procedure EvalPar(const RG, Kpar: Double;
  var Par: Double);
// 50
procedure EvalDecli(const aDate: TDateTime;
  var Decli: Double);
// 51
procedure EvalSunPosi(const LatRad, Decli: Double;
  var SunPosi: Double);
// 52
procedure EvalDayLength(const SunPosi: Double;
  var DayLength: Double);
// 53
procedure EvalSunDistance(const aDate: TDatetime;
  var SunDistance: Double);
// 54
procedure EvalRayExtra(const SunPosi, Decli, SunDistance, LatRad: Double;
  var RayExtra: Double);

// 55
procedure EvalRgMax(const RayExtra, Alt: Double;
  var RgMax: Double);

// 102
procedure DegToRad(const Lat: Double; var LatRad: Double);

// 57
procedure InsToRg(const DayLength, Ins, RayExtra, RgMax, RGLue: Double;
  var RGCalc: Double);

// 33104
procedure AVGTempHum(const TMin, TMax, HMin, HMax, TMoy, HMoy: Double;
  var TMoyCalc, HMoyCalc: Double);

// 56
procedure EToFao(const ETP, Alt, RgMax, RayGlobal, TMin, TMax,
  HrMin, HrMax, HrMoy, Tmoy, Vent: Double;
  var ETo, TMoyPrec, VPD: Double);
// Rayonnement voir basculer dans meteo
procedure SucrosPAR(Rg, LatRad, Decli, SunDistance, DayLength: double;
  var PAR: double);

procedure SucrosPARh(Rg, LatRad, Decli, SunDistance, DayLength, aDate, th:
  double;
  var PARdfh, PARdrh, sinusbetah: double);

// Changer en proc et un objet meteo qui
// met a nil ces variables qd on change de date puis qui
// calcul les valeurs de ses attributs s'ils sont a nil

implementation
uses Sysutils, Math, DateUtils, ModelsManage, Main, GestionDesErreurs, dialogs;

{ TODO : Fonction integreGauss }

function integreheure(duree, th: double): extended;

var
  p: integer;

begin
  try
    case trunc(th) of
      1:begin
          p := -1;result := 12 + 0.5 * duree * (0.5 + p * sqrt(0.15));
        end;
      2:begin
          p := 0;result := 12 + 0.5 * duree * (0.5 + p * sqrt(0.15));
        end;
      3:begin
          p := 1; result := 12 + 0.5 * duree * (0.5 + p * sqrt(0.15));
        end;
      
    end;
    
   // result := 12 + 0.5 * duree * (0.5 + p * sqrt(0.15));
      //intégration gaussienne pour trois heures
  except
    AfficheMessageErreur('integreheure', UMeteo);
    result:=-999;
  end;
  result:=-999;
end;

procedure AVGTempHum(const TMin, TMax, HMin, HMax, TMoy, HMoy: Double;
  var TMoyCalc, HMoyCalc: Double);
begin
  try
    if ((TMin <> NullValue) and (TMax <> NullValue)) then
    begin
      TMoyCalc := (TMax + TMin) / 2;
    end
    else
    begin
      TMoyCalc := TMoy;
    end;
    if ((HMin <> NullValue) and (HMax <> NullValue)) then
    begin
      HMoyCalc := (HMax + HMin) / 2;
    end
    else
    begin
      HMoyCalc := HMoy;
    end;
  except
    AfficheMessageErreur('AVGTempHum', UMeteo);
  end;
end;

procedure DegToRad(const Lat: Double; var LatRad: Double);
begin
  try
    LatRad := Lat * PI / 180;
  except
    AfficheMessageErreur('DegToRad', UMeteo);
  end;
end;

procedure EvalPar(const RG, KPar: Double; var Par: Double);
begin
  try
    Par := KPar * Rg;
  except
    AfficheMessageErreur('EvalPar', UMeteo);
  end;
end;

procedure EvalDecli(const aDate: TDateTime; var Decli: Double);
begin
  try
    Decli := 0.409 * Sin(0.0172 * DayOfTheYear(aDate) - 1.39);
  except
    AfficheMessageErreur('EvalDecli', UMeteo);
  end;
end;

procedure EvalDecli2(const aDate: TDateTime;
  var Decli: Double);
{
In :
   aDate : date du jour
Out :
   Decli : déclinaison du soleil
}
begin
  try
    Decli := -arcsin(sin(23.45 * PI / 180) *
      cos(2 * PI * (DayOfTheYear(aDate) + 10) / 365));
  except
    AfficheMessageErreur('EvalDecli2', UMeteo);
  end;
end;

procedure EvalSunPosi(const LatRad, Decli: Double; var SunPosi: Double);
begin
  try
    SunPosi := Arccos(-Tan(LatRad) * Tan(Decli));
  except
    AfficheMessageErreur('EvalSunPosi', UMeteo);
  end;
end;

procedure EvalDayLength(const SunPosi: Double; var DayLength: Double);
begin
  try
    DayLength := 7.64 * SunPosi;
  except
    AfficheMessageErreur('EvalDayLength', UMeteo);
  end;
end;

procedure EvalDayLength2(const LatRad, Decli: Double;
  var DayLength: Double);
{
In :
   aDate : date du jour
   Lat : en radian
Out :
   DayLength : duree du jour en heure, minute decimal
 }

begin
  try
    DayLength := 12 + 24 / 180 * arcsin(tan(LATRad) - tan(decli));
  except
    AfficheMessageErreur('EvalDayLength2', UMeteo);
  end;
end;

procedure EvalSunDistance(const aDate: TDatetime; var SunDistance: Double);
begin
  try
    SunDistance := 1 + 0.033 * Cos(2 * PI / 365 * DayOfTheYear(aDate));
  except
    AfficheMessageErreur('EvalSunDistance', UMeteo);
  end;
end;

procedure EvalSunDistance2(const Decli, LatRad, DayLength: TDatetime;
  var SunDistance: Double);
{
In :
   aDate : date du jour
Out :
   SunDistance: distance relative terre soleil
}
var
  SinLD, CosLD: Double;
begin
  try
    sinLD := sin(LATRad) * sin(decli);
    cosLD := cos(LATRad) * cos(decli);
    SunDistance := 3600 *
      (DayLength * (sinLD + 0.4 * (sqr(sinLD) + sqr(cosLD) * 0.5))
      + 12 / PI * cosLD *
      (2 + 3 * 0.4 * sinLD) * sqrt(1 - sqr(sinLD / cosLD)));
  except
    AfficheMessageErreur('EvalSunDistance2', UMeteo);
  end;
end;

procedure EvalRayExtra(const SunPosi, Decli, SunDistance, LatRad: Double; var
  RayExtra: Double);
begin
  try
    RayExtra := 24 * 60 * 0.0820 / PI * SunDistance *
      (SunPosi * Sin(Decli) * Sin(LatRad) +
      Cos(Decli) * Cos(LatRad) * Sin(SunPosi));
  except
    AfficheMessageErreur('EvalRayExtra', UMeteo);
  end;
end;

procedure EvalRgMax(const RayExtra, Alt: Double; var RgMax: Double);
begin
  try
    RgMax := (0.75 + 0.00002 * Alt) * RayExtra;
  except
    AfficheMessageErreur('EvalRgMax', UMeteo);
  end;
end;

procedure EToFAO(const ETP, Alt, RgMax, RayGlobal, TMin, TMax, HrMin, HrMax,
  HrMoy, Tmoy, Vent: Double; var ETo, TMoyPrec, VPD: Double);
var
  eActual, eSat,
    RgRgMax, TLat, delta, KPsy,
    Eaero, Erad, Rn, G: Double;
begin
  try
    if (ETP = NullValue) then
    begin
      eSat := 0.3054 * (Exp(17.27 * TMax / (TMax + 237.3)) +
        exp(17.27 * TMin / (TMin + 237.3)));
      if (HrMax = NullValue) then
        eActual := eSat * HrMoy / 100
      else
        eActual := 0.3054 * (Exp(17.27 * TMax / (TMax + 237.3)) *
          HrMin / 100 + Exp(17.27 * TMin / (TMin + 237.3)) *
          HrMax / 100);
      VPD := eSat - eActual;
      RgRgMax := RayGlobal / RgMax;
      if (RgRgMax > 1) then
        RgRgMax := 1;
      Rn := 0.77 * RayGlobal - (1.35 * RgRgMax - 0.35) *
        (0.34 - 0.14 * Power(eActual, 0.5)) *
        (Power(TMax + 273.16, 4) + Power(TMin + 273.16, 4)) * 2.45015 * Power(10,
          -9);
      // chaleur latente de vaporisation de l'eau
      Tlat := 2.501 - 2.361 * power(10, -3) * Tmoy;
      //  pente de la courbe de pression de vapeur saturante en kPa/°C
      delta := 4098 * (0.6108 * Exp(17.27 * Tmoy / (Tmoy + 237.3))) / Power(Tmoy
        + 237.3, 2);
      // constante psychrométrique en kPa/°C
      Kpsy := 0.00163 * 101.3 * power(1 - (0.0065 * Alt / 293), 5.26) / TLat;
      // Radiative
      G := 0.38 * (Tmoy - TmoyPrec);
      Erad := 0.408 * (Rn - G) * delta / (delta + Kpsy * (1 + 0.34 * Vent));
      // Partie évaporative de ET0 = Eaéro
      Eaero := (900 / (Tmoy + 273.16)) * ((eSat - eActual) * Vent) * Kpsy /
        (delta + Kpsy * (1 + 0.34 * Vent));
      Eto := Erad + Eaero;
    end
    else
    begin
      Eto := ETP;
    end;
    TMoyPrec := TMoy;
  except
    AfficheMessageErreur('EToFAO', UMeteo);
  end;
end;

procedure InsToRg(const DayLength, Ins, RayExtra, RgMax, RGLue: Double; var
  RGCalc: Double);
begin
  try
    if (RGLue = NullValue) then
    begin
      RGCalc := (0.25 + 0.50 * Min(Ins / DayLength, 1)) * RayExtra;
    end
    else
    begin
      RGCalc := RGLue;
    end;
  except
    AfficheMessageErreur('InsToRg', UMeteo);
  end;
end;

// Rayonnement voir meteo

procedure SucrosPAR(Rg, LatRad, Decli, SunDistance, DayLength: double;
  var PAR: double);
// Calcul le PAR totale journalier
// In:
//    LAT: latitude du site (U: radians) (lu dans station.DB)
//    Rg: rayonnement global (U: J/m2/s) (lu dans meteo.DB en MJ/m2/s, transformé
//    en J/m2/s dans gestionmodele)
//    jour: jour calendaire (calculé dans Uchaines à partir de la date)
// InOut:
// Out:
//    PAR: PAR total sur la journée (U: micromoles/m2/j)(calculé pour gs);

var
  sinusbeta, SinLD, CosLD: double;
  // decli: déclinaison du soleil en radians (Goudriaan et van Laar, 1994);
  // sinusbeta: sinus de l'élévation journalière du soleil;

begin
  try
    { TODO : Attention convertion de RG MJ en J/m2 }
    sinLD := sin(LATRad) * sin(decli);
    cosLD := cos(LATRad) * cos(decli);
    sinusbeta := max(0, sinLD + cosLD * cos(2 * PI * (+12) / 24));
    PAR := 0.5 * Rg * sinusbeta * (1 + 0.4 * sinusbeta) / SunDistance;
    PAR := PAR / 0.2177;
  except
    AfficheMessageErreur('SucrosPAR', UMeteo);
  end;
end;

// Attention procedure appele trois fois par jour ...
// ? gestion diff du temps heure, jour, mois ...

procedure SucrosPARh(Rg, LatRad, Decli, SunDistance, DayLength, aDate, th:
  double;
  var PARdfh, PARdrh, sinusbetah: double);
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
var
  R0h, SinLD, CosLD: double;
  //R0h: rayonnement extra-terrestre instantanné en J/m2/s
  PARhtot, atm, frdf: double;
  // PARhtotal: calculé en micromoles/m2/s et
  // atm: transmission atmospérique
  // frdf: fraction de diffus

begin
  try
    { TODO : Attention convertion de RG MJ en J/m2 }
    SinLD := sin(LATRad) * sin(decli);
    cosLD := cos(LATRad) * cos(decli);
    sinusbetah := max(0, sinLD + cosLD * cos(2 * PI * (integreheure(DayLength,
      th) + 12) / 24));
    R0h := 1370 * sinusbetah * (1 + 0.033 * cos(2 * PI * (DayOfTheYear(aDate) -
      10) / 365));
    // 1370 = constante solaire en J/m2/s = W/m2

    PARhtot := 0.5 * Rg * sinusbetah * (1 + 0.4 * sinusbetah) / SunDistance;
    atm := PARhtot / (0.5 * R0h);

    if atm <= 0.22 then
      frdf := 1 // ciel complètement nuageux
    else if atm <= 0.35 then
      frdf := 1 - 6.4 * sqr(atm - 0.22)
    else
      frdf := 1.47 - 1.66 * atm;
    frdf := max(frdf, 0.15 + 0.85 * (1 - exp(-0.1 / sinusbetah)));
    PARhtot := PARhtot / 0.2177;
    PARdfh := PARhtot * frdf;
    PARdrh := PARhtot - PARdfh;
  except
    AfficheMessageErreur('SucrosPARh', UMeteo);
  end;
end;

{
3. Calculs préliminaires
 Tm = (Tx + Tn) / 2
//!!! unité unique et reference a 2 m dans le stockage des donnees Vent dans la bdd
 Vitesse du vent à 2m en m/s avec correction de hauteur de mesure = u (m/s) = [vent total (Km/j)/86.4] * [4.87/ LN (67.8*H - 5.42)]
}

procedure EToFAO2(const Alt, RgMax, RayGlobal, TMin, TMax,
  HrMin, HrMax, HrMoy, Tmoy, Vent: Double;
  var ETo, TMoyPrec, VPD, VPDMax: Double);
{es = 0,3054 *( exp [ 17,27 . Tx / ( Tx + 237,3) ]
      + exp [ 17,27 . Tn / ( Tn + 237,3) ])
 : pression de vapeur saturante en kPa

ed = 0,3054 * (exp [ 17,27 . Tx / ( Tx + 237,3) ] * Hn/100
                + exp [ 17,27 . Tn / ( Tn + 237,3) ] * Hx / 100)
 : pression réelle de vapeur d'eau en kPa
Si pas de donnees HrMin et Max mais seulement HrMoy
ed = es * Hm/100
 Si Rg/Rso > 1 alors alerte Rg faux et Rg/Rso = 1
 Rn = 0.77 * Rg - [1.35 * Rg/Rso - 0.35] *
      [0.34 - 0.14 * power(ed,0.5) ] *
      [ power(Tx + 273.16,4) + power(Tn + 273.16,4) ] * 2.45015 * power(10,-9)

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

}
var
  eActual, eSat,
    RgRgMax, TLat, delta, KPsy,
    Eaero, Erad, Rn, G: Double;
begin
  try
    eSat := 0.3054 * (exp(17.27 * TMax / (TMax + 237.3))
      + exp(17.27 * TMin / (TMin + 237.3)));
    // Si pas de donnees min max alors HrMax a 0 controle a lecture
    //!!! voir -999 absent?
    if HrMax = NullValue then
      eActual := eSat * HrMoy / 100
    else
      eActual := 0.3054 * (exp(17.27 * TMax / (TMax + 237.3)) * HrMin / 100
        + exp(17.27 * TMin / (TMin + 237.3)) * HrMax / 100);
    VPD := eSat - eActual;
    VPDMax := 0.3054 * exp(17.27 * TMax / (TMax + 237.3)) * (1 - HrMin / 100);
    RgRgMax := RayGlobal / RgMax;
    //!!! situation anormal Rg faux faire une sortie PB
    if RgRgMax > 1 then
      RgRgMax := 1;
    Rn := 0.77 * RayGlobal - (1.35 * RgRgMax - 0.35) *
      (0.34 - 0.14 * power(eActual, 0.5)) *
      (power(TMax + 273.16, 4) + power(TMin + 273.16, 4)) * 2.45015 * power(10,
        -9);

    // chaleur latente de vaporisation de l'eau
    Tlat := 2.501 - 2.361 * power(10, -3) * Tmoy;

    //  pente de la courbe de pression de vapeur saturante en kPa/°C
    delta := 4098 * (0.6108 * exp(17.27 * Tmoy / (Tmoy + 237.3))) / power(Tmoy +
      237.3, 2);

    // constante psychrométrique en kPa/°C
    Kpsy := 0.00163 * 101.3 * power(1 - (0.0065 * Alt / 293), 5.26) / TLat;

    // Radiative
    G := 0.38 * (Tmoy - TmoyPrec);
    Erad := 0.408 * (Rn - G) * delta / (delta + Kpsy * (1 + 0.34 * Vent));

    // Partie évaporative de ET0 = Eaéro
    Eaero := (900 / (Tmoy + 273.16)) * ((eSat - eActual) * Vent) * Kpsy
      / (delta + Kpsy * (1 + 0.34 * Vent));

    Eto := Erad + Eaero;
    TMoyPrec := TMoy;
  except
    AfficheMessageErreur('EToFAO', UMeteo);
  end;
end;

procedure EvalEtpCalc(const ETP: Double; var EtpCalc: Double);
begin
  try
    EtpCalc := ETP;
  except
    AfficheMessageErreur('EvalEtpCalc', UMeteo);
  end;
end;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
{Liste de toutes les procedures redef en dyn de l'unite   }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

procedure AVGTempHumDyn(var TabParam: TPointeurProcParam);
begin
  AVGTempHum(TabParam[0], TabParam[1], TabParam[2], TabParam[3], TabParam[4],
    TabParam[5], TabParam[6], TabParam[7]);
end;

procedure EvalParDyn(var TabParam: TPointeurProcParam);
begin
  EvalPar(TabParam[0], TabParam[1], TabParam[2]);
end;

procedure EvalDecliDyn(var TabParam: TPointeurProcParam);
begin
  EvalDecli(TabParam[0], TabParam[1]);
end;

procedure EvalSunPosiDyn(var TabParam: TPointeurProcParam);
begin
  EvalSunPosi(TabParam[0], TabParam[1], TabParam[2]);
end;

procedure EvalDayLengthDyn(var TabParam: TPointeurProcParam);
begin
  EvalDayLength(TabParam[0], TabParam[1]);
end;

procedure EvalSunDistanceDyn(var TabParam: TPointeurProcParam);
begin
  EvalSunDistance(TabParam[0], TabParam[1]);
end;

procedure EvalRayExtraDyn(var TabParam: TPointeurProcParam);
begin
  EvalRayExtra(TabParam[0], TabParam[1], TabParam[2], TabParam[3], TabParam[4]);
end;

procedure EvalRgMaxDyn(var TabParam: TPointeurProcParam);
begin
  EvalRgMax(TabParam[0], TabParam[1], TabParam[2]);
end;

procedure InsToRgDyn(var TabParam: TPointeurProcParam);
begin
  InsToRg(TabParam[0], TabParam[1], TabParam[2], TabParam[3], TabParam[4],
    TabParam[5]);
end;

procedure EToFaoDyn(var TabParam: TPointeurProcParam);
begin
  EToFao(TabParam[0], TabParam[1], TabParam[2], TabParam[3], TabParam[4],
    TabParam[5], TabParam[6], TabParam[7], TabParam[8], TabParam[9], TabParam[10],
    TabParam[11], TabParam[12], TabParam[13]);
end;

procedure DegToRadDyn(var TabParam: TPointeurProcParam);
begin
  DegToRad(TabParam[0], TabParam[1]);
end;

procedure EToFAO2Dyn(var T: TPointeurProcParam);
begin
  EToFAO2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9], T[10],
    T[11], T[12], T[13]);
end;

procedure EvalEtpCalcDyn(var T: TPointeurProcParam);
begin
  EvalEtpCalc(T[0], T[1]);
end;

initialization

  TabProc.AjoutProc('AVGTempHum', AVGTempHumDyn);
  TabProc.AjoutProc('EvalPar', EvalParDyn);
  TabProc.AjoutProc('EvalDecli', EvalDecliDyn);
  TabProc.AjoutProc('EvalSunPosi', EvalSunPosiDyn);
  TabProc.AjoutProc('EvalDayLength', EvalDayLengthDyn);
  TabProc.AjoutProc('EvalSunDistance', EvalSunDistanceDyn);
  TabProc.AjoutProc('EvalRayExtra', EvalRayExtraDyn);
  TabProc.AjoutProc('EvalRgMax', EvalRgMaxDyn);
  TabProc.AjoutProc('InsToRg', InsToRgDyn);
  TabProc.AjoutProc('EToFao', EToFaoDyn);
  TabProc.AjoutProc('DegToRad', DegToRadDyn);
  TabProc.AjoutProc('EToFAO2', EToFAO2Dyn);
  TabProc.AjoutProc('EvalEtpCalc', EvalEtpCalcDyn);
end.

