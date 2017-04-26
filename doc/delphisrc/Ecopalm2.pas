unit Ecopalm2;

interface
uses Math, SysUtils, DateUtils;

implementation
uses ModelsManage, Main, GestionDesErreurs, Palmier;

// ##################################################################
// ######               Ecopalm 2                           ########
// ##################################################################

procedure EvalCroissanceJour(const CroisVegAn, Cstr: double;
  var CroissanceJour: Double);
// calcul journalier de la Croissance

begin
  try
    CroissanceJour := Cstr * CroisVegAn / 365;
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. , Croissance du jour: ' +
        FloatToStr(CroissanceJour));
  except
    AfficheMessageErreur('EvalCroissanceJour', UEcopalm2);
  end;
end;

////////////////////////////////////////////////////////////////////

procedure EvolCroissanceMois(const CroissanceJour: Double; const thisDate:
  TDateTime;
  {Proc 206}var CroissanceMois: Double);
// Calcul mensuel de la Croissance

var
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
      CroissanceMois := 0;
    CroissanceMois := CroissanceMois + CroissanceJour;
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. (' + DateToStr(thisDate) +
        '), Croissance du mois: ' + FloatToStr(CroissanceMois));
  except
    AfficheMessageErreur('CroissanceMois  ', UEcopalm2);
  end;
end;

procedure EvalRespMaintJour(const KrespMaint, TmoyCalc: double;
  {Proc 205}var RespMaintJour: Double);
// calcul journalier de la Respiration de Maintenance

begin
  try
    RespMaintJour := KrespMaint * power(2, (TmoyCalc - 25) / 10);
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. , Respiration de Maintenance du jour: ' + FloatToStr(RespMaintJour));
  except
    AfficheMessageErreur('EvalRespMaintJour ', UEcopalm2);
  end;
end;

////////////////////////////////////////////////////////////////////

procedure EvolRespMaintMois(const RespMaintJour: Double; const thisDate:
  TDateTime;
  {Proc 206}var RespMaintMois: Double);
// Calcul mensuel de la Respiration de Maintenance

var
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
      RespMaintMois := 0;
    RespMaintMois := RespMaintMois + RespMaintJour;
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. (' + DateToStr(thisDate) +
        '), Respiration de Maintenance du mois: ' + FloatToStr(RespMaintMois));
  except
    AfficheMessageErreur('RespMaintMois ', UEcopalm2);
  end;
end;

//////////////////////////////////////////////////////////////////

procedure CalculeCstrMois(const DateDuJour: TDateTime; const Cstr: Double; var
  CstrMois: Double);
{Proc 245}
// sert à calculer le CSTR et FTSW moyen mensuel
var
  Annee, Mois, Jour: Word;
begin
  DecodeDate(DateDuJour, Annee, Mois, Jour);
  if Jour = 1 then
    CstrMois := 0;
  CstrMois := CstrMois + (Cstr / DaysInAMonth(Annee, Mois));
end;
///////////////////////////////////////////////////////////////////////////////////////////

procedure EvalOffreNetteMois2(const OffreBruteMois, RespMaintMois,
  CroissanceMois: Double;
  {Proc 207}var OffreNetteMois: Double);
// Calcul mensuel de l'offre nette dédiée à la reproduction

begin
  try
    OffreNetteMois := OffreBruteMois - RespMaintMois - CroissanceMois;
    //L'offre nette est ce qui reste éventuellement pour le rendement après déduction de la respiration et de la croissance prioritaires;
    // L'offre nette peut être < 0 ; on prélève dans les réserves ; on ne prend que la partie >0 dans DeltaOffreDemande
  except
    AfficheMessageErreur('EvalOffreNetteMois2', UEcopalm2);
  end;
end;

/////////////////////////////////////////////////////////

procedure EvalDemandeReproPot(const thisDate: TDateTime; const ProdAnRegPotPF,
  KTeneurEau, KChimique: Double;
  var DemandeReproPot: Double);
var
  PotentielFloraison: Double;
begin
  try
    PotentielFloraison := TabDemReproMois[MonthOf(thisDate)];
    //DemandeReproPot:=PotentielFloraison*(ProdAnRegPotPF *(1- KTeneurEau) * KChimique*1.5) /365;
    DemandeReproPot := PotentielFloraison * (ProdAnRegPotPF * (1 - KTeneurEau) *
      KChimique) / 365; //JCC 09/06/05 pour être conforme au calcul du rendement
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc.  (' + DateToStr(thisDate) +
        '), DemandeReproPot: ' + FloatToStr(DemandeReproPot));
  except
    AfficheMessageErreur('EvalDemandeReproPot', UEcopalm2);
  end;
end;

////////////////////////////////////////////////////////////////////////////////////////////////////

procedure EvalDemandeMois2(const thisDate: TDateTime; const KPondIc4, KPondIc3,
  KPondIc2, KPondIc1,
  {Proc 209}DemandeReproPot, MoisIc1, MoisIc2, MoisIc3, MoisIc4: Double;
  var DemandeReproMois: Double);
// Calcul de la demande reproductive mensuelle pondérée par les Ic

var
  LesFreins, F4, F3, F2, F1: Double;

begin
  try
    if RechercheIcDecale(thisDate, Trunc(MoisIc4)) = -999 then
      F4 := 1
    else
      F4 := RechercheIcDecale(thisDate, Trunc(MoisIc4));
    if RechercheIcDecale(thisDate, Trunc(MoisIc3)) = -999 then
      F3 := 1
    else
      F3 := RechercheIcDecale(thisDate, Trunc(MoisIc3));
    if RechercheIcDecale(thisDate, Trunc(MoisIc2)) = -999 then
      F2 := 1
    else
      F2 := RechercheIcDecale(thisDate, Trunc(MoisIc2));
    if RechercheIcDecale(thisDate, Trunc(MoisIc1)) = -999 then
      F1 := 1
    else
      F1 := RechercheIcDecale(thisDate, Trunc(MoisIc1));
    LesFreins := min(1, F4 / max(0.001, KPondIc4))
      * min(1, F3 / max(0.001, KPondIc3))
      * min(1, F2 / max(0.001, KPondIc2))
      * min(1, F1 / max(0.001, KPondIc1));
    {LesFreins := min(1,KPondIc3 * F3 + (1-KPondIc3))
                  * min(1,KPondIc2 * F2 + (1-KPondIc2))
                  * min(1,KPondIc1 * F1 + (1-KPondIc1)); }
    DemandeReproMois := max(0.001, DemandeReproPot * LesFreins);
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. (' + DateToStr(thisDate) +
        '), DemandeReproMois: ' + FloatToStr(DemandeReproMois));
  except
    AfficheMessageErreur('EvalDemandeMois2', UEcopalm2);
  end;
end;

/////////////////////////////////////////////////////////////////////

procedure EvolReserveMois2(const OffreNetteMois, DemandeReproMois,
  KReallocReserve, CoutRealloc, CstrMois: double;

  {Proc 215}var ReserveMois, Reallocation: double);
// Calcule le stock de réserve mensuel
// remplace une partie de RepartiAssimilats Proc 65
var
  DeltaOffreDemande, Ratio: double;
begin
  try
    // Quand l'offre nette est négative on consomme des réserves prioritairement;
    ReserveMois := max(0, ReserveMois + min(0, OffreNetteMois));
      // On consomme d'abord les réserves quand OffreNetteMois < 0
    DeltaOffreDemande := max(0, OffreNetteMois) - DemandeReproMois;
    Ratio := ReserveMois / 20000;
      //réserve sur réserve max pour freiner la réallocation
    if DeltaOffreDemande < 0 then
    begin
      if Ratio < 0.4 then
        Reallocation := Min(ReserveMois / CoutRealloc, -DeltaOffreDemande *
          KReallocReserve * Ratio / 0.4)
      else
        Reallocation := Min(ReserveMois / CoutRealloc, -DeltaOffreDemande *
          KReallocReserve);
      // On ne peut pas réallouer plus que les réserves
     //Reallocation := Min(ReserveMois/CoutRealloc, -DeltaOffreDemande*min(1,ReserveMois/12000));  //JCC 9/06/05
      ReserveMois := Max(0, ReserveMois - Reallocation * CoutRealloc);
    end
    else
    begin
      Reallocation := 0;
      ReserveMois := ReserveMois + DeltaOffreDemande;
    end;
  except
    AfficheMessageErreur('EvolReserveMois2', UEcopalm2);
  end;
end;

/////////////////////////////////////////////////////////////////////

procedure EvalIcMois2(const thisDate: TDateTime; const OffreBruteMois,
  RendementRegimePS, RespiMois,
  CroissanceMois: double;
  {Proc 218}var IcMois: double);
// calcul de l'indice de compétition mensuel
// JCC calcule Ic = offre brute : (demande repro + croissance + respiration )
begin
  try
    IcMois := OffreBruteMois / max(0.001, RendementRegimePS + RespiMois +
      CroissanceMois);
    SetLength(tabIcMensuel, length(tabIcMensuel) + 1);
    tabIcMensuel[High(tabIcMensuel)].Mois := MonthOf(thisDate);
    tabIcMensuel[High(tabIcMensuel)].Annee := YearOf(thisDate);
    tabIcMensuel[High(tabIcMensuel)].ValeurIc := IcMois;
  except
    AfficheMessageErreur('EvalIcMois2', UEcopalm2);
  end;
end;

/////////////////////////////////////////////////////////////////////

procedure EvolReserveMois21(const CroisVegAn, CroissanceMois, OffreNetteMois,
  DemandeReproMois,
  KReallocReserve, CoutRealloc, CstrMois: double;

  {Proc 215}var ReserveMois, Reallocation: double);
// Calcule le stock de réserve mensuel
// remplace une partie de RepartiAssimilats Proc 65
var
  DeltaOffreDemande: double;
begin
  try
    // Quand l'offre nette est négative on consomme des réserves prioritairement;
    // La croissance végétative étant constante, ce qui manque est prélevé dans les réserves
    //ReserveMois := max(0,ReserveMois+min(0,OffreNetteMois)+min(0,(CroissanceMois-CroisVegAn/12)*CoutRealloc));   // On consomme d'abord les réserves quand OffreNetteMois < 0
    ReserveMois := max(0, ReserveMois + min(0, OffreNetteMois));
    DeltaOffreDemande := max(0, OffreNetteMois) - DemandeReproMois;
    if DeltaOffreDemande < 0 then
    begin
      Reallocation := Min(ReserveMois / CoutRealloc, -DeltaOffreDemande *
        KReallocReserve); // On ne peut pas réallouer plus que les réserves
      ReserveMois := Max(0, ReserveMois - Reallocation * CoutRealloc);
    end
    else
    begin
      Reallocation := 0;
      ReserveMois := ReserveMois + DeltaOffreDemande;
    end;
  except
    AfficheMessageErreur('EvolReserveMois2', UEcopalm2);
  end;
end;

/////////////////////////////////////////////////////////////////////

procedure EvalIcMois21(const thisDate: TDateTime; const OffreBruteMois,
  DemandeReproMois, RespiMois,
  CroissanceMois: double;
  {Proc 218}var IcMois: double);
// calcul de l'indice de compétition mensuel
// JCC calcule Ic = offre brute : (demande repro + croissance + respiration )
begin
  try
    IcMois := OffreBruteMois / max(0.001, DemandeReproMois + RespiMois +
      CroissanceMois);
    SetLength(tabIcMensuel, length(tabIcMensuel) + 1);
    tabIcMensuel[High(tabIcMensuel)].Mois := MonthOf(thisDate);
    tabIcMensuel[High(tabIcMensuel)].Annee := YearOf(thisDate);
    tabIcMensuel[High(tabIcMensuel)].ValeurIc := IcMois;
  except
    AfficheMessageErreur('EvalIcMois2', UEcopalm2);
  end;
end;

// ##################################################################

procedure EvalOffreBruteJour2(const ReserveMois, KEpsiB, Par, Ltr, Cstr: double;
  {Proc 205}var OffreBruteJour: Double);
// calcul journalier de la photosynthèse
// remplace Biomasse.Assimilat Proc76
var
  ReserveMax, Conversion: Double;
begin
  try
    begin
      ReserveMax := 20000;
      if ReserveMois / ReserveMax <= 0.6 then
        Conversion := max(1, 1.5 - ReserveMois / ReserveMax)
      else
        Conversion := (1 - min(1, ReserveMois / ReserveMax)) / (1 - 0.6);
    end;
    OffreBruteJour := Par * KEpsiB * Conversion * Cstr * 10 * (1 - LTR);
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°205, Offre brute du jour: ' +
        FloatToStr(OffreBruteJour));
  except
    AfficheMessageErreur('EvalOffreBruteJour n°205 ', UPalmier);
  end;
end;

////////////////////////////////////////////////////////////////////

//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// LISTE DES PROCEDURES DYNAMIQUES
//
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

procedure EvalCroissanceJourDyn(var T: TPointeurProcParam);
begin
  EvalCroissanceJour(T[0], T[1], T[2]);
end;

procedure EvalDemandeMois2Dyn(var T: TPointeurProcParam);
begin
  EvalDemandeMois2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10]);
end;

procedure EvalDemandeReproPotDyn(var T: TPointeurProcParam);
begin
  EvalDemandeReproPot(T[0], T[1], T[2], T[3], T[4]);
end;

procedure EvalIcMois2Dyn(var T: TPointeurProcParam);
begin
  EvalIcMois2(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure EvalOffreNetteMois2Dyn(var T: TPointeurProcParam);
begin
  EvalOffreNetteMois2(T[0], T[1], T[2], T[3]);
end;

procedure EvalRespMaintJourDyn(var T: TPointeurProcParam);
begin
  EvalRespMaintJour(T[0], T[1], T[2]);
end;

procedure EvolCroissanceMoisDyn(var T: TPointeurProcParam);
begin
  EvolCroissanceMois(T[0], T[1], T[2]);
end;

procedure EvolReserveMois2Dyn(var T: TPointeurProcParam);
begin
  EvolReserveMois2(T[0], T[1], T[2], T[3], T[4], T[5], T[6]);
end;

procedure EvolRespMaintMoisDyn(var T: TPointeurProcParam);
begin
  EvolRespMaintMois(T[0], T[1], T[2]);
end;

procedure CalculeCstrMoisDyn(var T: TPointeurProcParam);
begin
  CalculeCstrMois(T[0], T[1], T[2]);
end;

procedure EvolReserveMois21Dyn(var T: TPointeurProcParam);
begin
  EvolReserveMois21(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8]);
end;

procedure EvalOffreBruteJour2Dyn(var T: TPointeurProcParam);
begin
  EvalOffreBruteJour2(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

initialization
  TabProc.AjoutProc('EvalCroissanceJour', EvalCroissanceJourDyn);
  TabProc.AjoutProc('EvalDemandeMois2', EvalDemandeMois2Dyn);
  TabProc.AjoutProc('EvalDemandeReproPot', EvalDemandeReproPotDyn);
  TabProc.AjoutProc('EvalIcMois2', EvalIcMois2Dyn);
  TabProc.AjoutProc('EvalOffreNetteMois2', EvalOffreNetteMois2Dyn);
  TabProc.AjoutProc('EvalRespMaintJour', EvalRespMaintJourDyn);
  TabProc.AjoutProc('EvolCroissanceMois', EvolCroissanceMoisDyn);
  TabProc.AjoutProc('EvolReserveMois2', EvolReserveMois2Dyn);
  TabProc.AjoutProc('EvolRespMaintMois', EvolRespMaintMoisDyn);
  TabProc.AjoutProc('CalculeCstrMois', CalculeCstrMoisDyn);
  TabProc.AjoutProc('EvolReserveMois21', EvolReserveMois21Dyn);
  TabProc.AjoutProc('EvalOffreBruteJour2', EvalOffreBruteJour2Dyn);

end.

