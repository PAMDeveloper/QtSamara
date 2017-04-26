unit Ecopalm2_2;

interface
uses Math, SysUtils, DateUtils;

implementation
uses ModelsManage, Main, GestionDesErreurs, Palmier;

// ##################################################################
// ######               Ecopalm 2_2                           ########
// ##################################################################
{On va caluler une OffreNetteJour = OffreBruteJour - RespMaintJour et gérer quotidiennement
les réserves puisque leur contenu joue sur OB. La part <0 de OBJ est prélevée dans les réserves0.
Si ONJ < CroisPot on réalise CroisPot * Cstr en prélevant si nécessaire dans les réserves.
En dessous d'un seuil de réserves/reservesMax on ne réalloue pas pour la croissance.
L'offreReproJour = OB - RM - CJ. Elle est affectée provisoirement aux réserves
mais en sera déduit sans KRealloc en fin de mois.}

//----------------------------------------------------------------------------------
//                   1.Initialisation des Reserves
//-----------------------------------------------------------------------------------

procedure IniCulturePerenne2_2(const DateMaturite, NumPhase: double;
  var ReserveMax, ReserveJour: double);
{Proc 202}
begin
  // Cette procédure ne sert qu'à adapter SarraH v3 optimisé aux cultures pérennes
  // en déclarant des variables (telles que la date de maturité) qui sont
  // nécéssaire au bon fonctionnement de SarraH v3  Initialise les réserves
  try
    ReserveMax := 13000;
    ReserveJour := ReserveMax / 2;

    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°202, Initialisation des cultures pérènnes effectuée');
  except
    AfficheMessageErreur('IniCulturePerenne2_2', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       2.Offre Brute Jour
 //-----------------------------------------------------------------------------------

procedure EvalOffreBruteJour2_2(const ReserveMax, ReserveJour, KEpsiB, Par, Ltr,
  Cstr: double;
  {Proc 205}var OffreBruteJour: Double);
// calcul journalier de la photosynthèse
// avec frein ou accélération lié aux réserves
var
  Conversion: Double;
begin
  try
    begin
      if ReserveJour / ReserveMax <= 0.6 then
        Conversion := max(1, 1.3 - ReserveJour / ReserveMax)
      else
        Conversion := (1 - min(1, ReserveJour / ReserveMax)) / (1 - 0.6);
    end;
    OffreBruteJour := Par * KEpsiB * Conversion * Cstr * 10 * (1 - LTR);
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°205, Offre brute du jour: ' +
        FloatToStr(OffreBruteJour));
  except
    AfficheMessageErreur('EvalOffreBruteJour2_2 ', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       3.Offre Nette Jour
 //-----------------------------------------------------------------------------------

procedure EvalOffreNetteJour2_2(const OffreBruteJour, RespMaintJour: Double;
  {Proc 207}var OffreNetteJour: Double);
// Calcul mensuel de l'offre nette dédiée à la reproduction

begin
  try
    OffreNetteJour := OffreBruteJour - RespMaintJour;
    //L'offre nette est ce qui reste éventuellement pour le rendement après déduction de la respiration de maintenance;

    // L'offre nette peut être < 0 ; on prélève dans les réserves ; on ne prend que la partie >0 dans DeltaOffreDemande
  except
    AfficheMessageErreur('valOffreNetteJour2_2', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       4.Croissance Jour
 //-----------------------------------------------------------------------------------

procedure EvalCroissanceJour2_2(const CroisVegAn, Cstr, OffreNetteJour: double;
  var CroissanceJour: Double);
// calcul journalier de la Croissance

begin
  try
    begin
      if OffreNetteJour >= CroisVegAn * 10000 * 1.45 / 365 then
        CroissanceJour := CroisVegAn * 10000 * 1.45 / 365
      else
        CroissanceJour := Cstr * CroisVegAn * 10000 * 1.45 / 365;
    end;
  except
    AfficheMessageErreur('EvalCroissanceJour2_2', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       5.Offre Repro Jour
 //-----------------------------------------------------------------------------------

procedure EvalOffreReproJour2_2(const CroissanceJour, OffreNetteJour: double;
  var OffreReproJour: Double);

begin
  try
    OffreReproJour := OffreNetteJour - CroissanceJour;
      //peut être <0 si ON<CroissanceJour
  except
    AfficheMessageErreur('EvalOffreReproJour2_2', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       6.Reserve Jour et Reallocation
 //-----------------------------------------------------------------------------------

procedure EvolReserveJour2_2(const OffreReproJour, DemandeReproJour,
  KReallocReserve,
  CoutRealloc, ReserveMax, OffreNetteJour: double;
  {Proc 215}var ReserveJour, Reallocation: double);
// Calcule le stock de réserve mensuel

var
  DeltaOffreDemande, Ratio: double;
begin
  try
    ReserveJour := ReserveJour + min(0, OffreReproJour);
    //la RM non couverte par OB est consommée sur les réserves, idem pour la croissance
    Ratio := ReserveJour / ReserveMax;
      //réserve sur réserve max pour freiner la réallocation
    DeltaOffreDemande := max(0, OffreReproJour) - DemandeReproJour;

    if DeltaOffreDemande < 0 then
    begin
      if Ratio < 0.3 then
        Reallocation := Min(ReserveJour / CoutRealloc, -DeltaOffreDemande *
          KReallocReserve * Ratio / 0.3)
      else
        Reallocation := Min(ReserveJour / CoutRealloc, -DeltaOffreDemande *
          KReallocReserve);
      // On ne peut pas réallouer plus que les réserves
      ReserveJour := Max(0, ReserveJour - OffreReproJour - Reallocation *
        CoutRealloc);
      //revoir à tête reposée.... il ne faudrait pas cumuler l'offre repro dans Réserve Jour ? Ou tout faire au pas de temps journalier ?
    end
    else
    begin
      Reallocation := 0;
      ReserveJour := ReserveJour + DeltaOffreDemande;
    end;
  except
    AfficheMessageErreur('EvolReserveJour2_2', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       7.Demande Repro Jour et mois
 //-----------------------------------------------------------------------------------

procedure EvalDemandeReproJour2_2(const thisDate: TDateTime; const
  ProdAnRegPotPF,
  KTeneurEau, KChimique, KPondIc1, KPondIc2,
  KPondIc3, KPondIc4, MoisIc1, MoisIc2, MoisIc3, MoisIc4: Double;
  var DemandeReproMois, DemandeReproPot, DemandeReproJour: Double);
// Calcul de la demande reproductive mensuelle pondérée par les Ic

var
  LesFreins, F4, F3, F2, F1, PotentielFloraison: Double;
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
    begin
      PotentielFloraison := TabDemReproMois[MonthOf(thisDate)];
      DemandeReproPot := PotentielFloraison * (ProdAnRegPotPF * 10000 * (1 -
        KTeneurEau) * KChimique * 1.45) / 365;
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
      DemandeReproMois := max(0.001, DemandeReproPot * LesFreins);
      DemandeReproJour := DemandeReproMois / DaysInAMonth(Year, Month);
    end;
  except
    AfficheMessageErreur('EvalDemandeReproJour2_2', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       8.RendementJour
 //-----------------------------------------------------------------------------------

procedure EvalRendementJour2_2(const thisDate: TDateTime; const
  DemandeReproJour, OffreNetteJour,
  Reallocation, KChimique, KTeneurHuile, KTeneurEau: double;
  var RendementRegimePSJour, RendementRegimeJour, RendementHuileJour: double);
// calcule le rendement en poids sec de régime et en poids d'huile
// par hectare et par mois
// remplace une partie de RepartiAssimilats Proc 65
begin
  try
    RendementRegimePSJour := Min(DemandeReproJour, max(0, OffreNetteJour) +
      Reallocation) / (KChimique * 1.45);
    RendementRegimeJour := RendementRegimePSJour / (1 - KTeneurEau);
      //rendement exprimé en Poids Frais JCC 13/09/04
    RendementHuileJour := RendementRegimeJour * KTeneurHuile;
  except
    AfficheMessageErreur('EvalRendementJour2_2 ', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       9.Offre Repro Mois
 //-----------------------------------------------------------------------------------

procedure EvolOffreReproMois2_2(const OffreReproJour: Double; const thisDate:
  TDateTime;
  {Proc 206}var OffreReproMois: Double);
// Calcul mensuel de l'offre Repro
// remplace CumulMois
var
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
      OffreReproMois := 0;
    OffreReproMois := OffreReproMois + max(0, OffreReproJour);
  except
    AfficheMessageErreur('EvolOffreReproMois2_2 ', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       10.Offre Nette Mois
 //-----------------------------------------------------------------------------------

procedure EvolOffreNetteMois2_2(const OffreNetteJour: Double; const thisDate:
  TDateTime;
  {Proc 206}var OffreNetteMois: Double);
// Calcul mensuel de l'offre nette
// remplace CumulMois
var
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
      OffreNetteMois := 0;
    OffreNetteMois := OffreNetteMois + OffreNetteJour;
  except
    AfficheMessageErreur('EvolOffreNetteMois2_2 ', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       11.Croissance Mois
 //-----------------------------------------------------------------------------------

procedure EvolCroissanceMois2_2(const CroissanceJour: Double; const thisDate:
  TDateTime;
  {Proc 206}var CroissanceMois: Double);
// Calcul mensuel de l'offre nette
// remplace CumulMois
var
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
      CroissanceMois := 0;
    CroissanceMois := CroissanceMois + CroissanceJour;
  except
    AfficheMessageErreur('EvolCroissanceMois2_2 ', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       12.Rendement Regime Mois
 //-----------------------------------------------------------------------------------

procedure EvolRendementRegimeMois2_2(const RendementRegimeJour,
  RendementRegimePSJour: Double;
  const thisDate: TDateTime;
  {Proc 206}var RendementRegimeMois, RendementRegimePSMois: Double);
// remplace CumulMois
var
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
    begin
      RendementRegimePSMois := 0;
      RendementRegimeMois := 0;
    end;
    RendementRegimeMois := RendementRegimeMois + RendementRegimeJour;
    RendementRegimePSMois := RendementRegimePSMois + RendementRegimePSJour;
  except
    AfficheMessageErreur('EvolRendementRegimeMois2_2 ', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       13.Rendement Huile Mois
 //-----------------------------------------------------------------------------------

procedure EvolRendementHuileMois2_2(const RendementHuileJour: Double; const
  thisDate: TDateTime;
  {Proc 206}var RendementHuile: Double);
// Calcul mensuel de l'offre nette
// remplace CumulMois
var
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
      RendementHuile := 0;
    RendementHuile := RendementHuile + RendementHuileJour;
  except
    AfficheMessageErreur('EvolRendementHuileMois2_2 ', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                      14. Réallocation Mois
 //-----------------------------------------------------------------------------------

procedure EvolReallocationMois2_2(const Reallocation: Double; const thisDate:
  TDateTime;
  {Proc 206}var ReallocationMois: Double);
// Calcul mensuel de l'offre nette
// remplace CumulMois
var
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
      ReallocationMois := 0;
    ReallocationMois := ReallocationMois + Reallocation;
  except
    AfficheMessageErreur('EvolReallocationMois2_2 ', UEcopalm2_2);
  end;
end;

//----------------------------------------------------------------------------------
 //                       15.Réserve Mois
 //-----------------------------------------------------------------------------------

procedure EvalReserveMois2_2(const ReserveJour: Double;
  {Proc 206}var ReserveMois: Double);
// Calcul mensuel de l'offre nette
// remplace CumulMois
begin
  try
    ReserveMois := ReserveJour;
  except
    AfficheMessageErreur('EvalReserveMois2_2 ', UEcopalm2_2);
  end;
end;
//----------------------------------------------------------------------------------
//                       16. Conversion et VPDMax (a placer avant EvalOffreBruteJour)
//-----------------------------------------------------------------------------------

procedure EvalConversionVPDMax(const VPDMax, KEpsiB: double;
  {Proc 205}var KEpsiBcor: Double);
// calcul journalier de la variation de KEpsiB avec le VPDMax

begin
  try
    KEpsiBcor := KEpsiB * (1 - 0.255 * VPDMax);
  except
    AfficheMessageErreur('EvalOffreBruteJour2_2 ', UEcopalm2_2);
  end;
end;

////////////////////////////////////////////////////////////////////

//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// LISTE DES PROCEDURES DYNAMIQUES
//
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

procedure IniCulturePerenne2_2Dyn(var T: TPointeurProcParam);
begin
  IniCulturePerenne2_2(T[0], T[1], T[2], T[3]);
end;

procedure EvalOffreBruteJour2_2Dyn(var T: TPointeurProcParam);
begin
  EvalOffreBruteJour2_2(T[0], T[1], T[2], T[3], T[4], T[5], T[6]);
end;

procedure EvalOffreNetteJour2_2Dyn(var T: TPointeurProcParam);
begin
  EvalOffreNetteJour2_2(T[0], T[1], T[2]);
end;

procedure EvalCroissanceJour2_2Dyn(var T: TPointeurProcParam);
begin
  EvalCroissanceJour2_2(T[0], T[1], T[2], T[3]);
end;

procedure EvalOffreReproJour2_2Dyn(var T: TPointeurProcParam);
begin
  EvalOffreReproJour2_2(T[0], T[1], T[2]);
end;

procedure EvolReserveJour2_2Dyn(var T: TPointeurProcParam);
begin
  EvolReserveJour2_2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7]);
end;

procedure EvalDemandeReproJour2_2Dyn(var T: TPointeurProcParam);
begin
  EvalDemandeReproJour2_2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9], T[10], T[11], T[12], T[13], T[14]);
end;

procedure EvalRendementJour2_2Dyn(var T: TPointeurProcParam);
begin
  EvalRendementJour2_2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9]);
end;

procedure EvolOffreReproMois2_2Dyn(var T: TPointeurProcParam);
begin
  EvolOffreReproMois2_2(T[0], T[1], T[2]);
end;

procedure EvolOffreNetteMois2_2Dyn(var T: TPointeurProcParam);
begin
  EvolOffreNetteMois2_2(T[0], T[1], T[2]);
end;

procedure EvolCroissanceMois2_2Dyn(var T: TPointeurProcParam);
begin
  EvolCroissanceMois2_2(T[0], T[1], T[2]);
end;

procedure EvolRendementRegimeMois2_2Dyn(var T: TPointeurProcParam);
begin
  EvolRendementRegimeMois2_2(T[0], T[1], T[2], T[3], T[4]);
end;

procedure EvolRendementHuileMois2_2Dyn(var T: TPointeurProcParam);
begin
  EvolRendementHuileMois2_2(T[0], T[1], T[2]);
end;

procedure EvolReallocationMois2_2Dyn(var T: TPointeurProcParam);
begin
  EvolReallocationMois2_2(T[0], T[1], T[2]);
end;

procedure EvalReserveMois2_2Dyn(var T: TPointeurProcParam);
begin
  EvalReserveMois2_2(T[0], T[1]);
end;

procedure EvalConversionVPDMaxDyn(var T: TPointeurProcParam);
begin
  EvalConversionVPDMax(T[0], T[1], T[2]);
end;

initialization
  TabProc.AjoutProc('IniCulturePerenne2_2', IniCulturePerenne2_2Dyn);
  TabProc.AjoutProc('EvalOffreBruteJour2_2', EvalOffreBruteJour2_2Dyn);
  TabProc.AjoutProc('EvalOffreNetteJour2_2', EvalOffreNetteJour2_2Dyn);
  TabProc.AjoutProc('EvalCroissanceJour2_2', EvalCroissanceJour2_2Dyn);
  TabProc.AjoutProc('EvalOffreReproJour2_2', EvalOffreReproJour2_2Dyn);
  TabProc.AjoutProc('EvolReserveJour2_2', EvolReserveJour2_2Dyn);
  TabProc.AjoutProc('EvalDemandeReproJour2_2', EvalDemandeReproJour2_2Dyn);
  TabProc.AjoutProc('EvalRendementJour2_2', EvalRendementJour2_2Dyn);
  TabProc.AjoutProc('EvolOffreReproMois2_2', EvolOffreReproMois2_2Dyn);
  TabProc.AjoutProc('EvolOffreNetteMois2_2', EvolOffreNetteMois2_2Dyn);
  TabProc.AjoutProc('EvolCroissanceMois2_2', EvolCroissanceMois2_2Dyn);
  TabProc.AjoutProc('EvolRendementRegimeMois2_2',
    EvolRendementRegimeMois2_2Dyn);
  TabProc.AjoutProc('EvolRendementHuileMois2_2', EvolRendementHuileMois2_2Dyn);
  TabProc.AjoutProc('EvolReallocationMois2_2', EvolReallocationMois2_2Dyn);
  TabProc.AjoutProc('EvalReserveMois2_2', EvalReserveMois2_2Dyn);
  TabProc.AjoutProc('EvalConversionVPDMax', EvalConversionVPDMaxDyn);

end.

