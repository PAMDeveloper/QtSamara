unit Sorghum;

interface
uses Math, SysUtils;

var
  StockRacinairePrec: Double = 0;

implementation
uses ModelsManage, GestionDesErreurs, Dialogs;

var
  tabCstr: array[1..5] of Double; // utilisé dans SorghumMortality()
var
  tabCstrIndiceCourant: Integer = 0; // utilisé dans SorghumMortality()

var
  NbJourCompte: integer = 0;

  //##############################################################################
  /// Ce module permet de suivre l'évolution des 5 derniers jours de Cstr afin de
  /// moyenner la valeur des stress. Si la moyenne des Cstr est inférieure à
  /// un seuil SorghumMortality, la culture décède (NumPhase=8)
  /// Demande MD du 28/09/06
  //##############################################################################

procedure SorghumMortality(const cstr, SeuilCstrMortality: Double; var NumPhase:
  double);
var
  i: Integer;
  MoyenneCstr: Double;
begin
  try
    if (NumPhase >= 2) then
    begin
      NbJourCompte := NbJourCompte + 1;
      // gestion de l'indice...
      if (tabCstrIndiceCourant = 5) then
      begin
        tabCstrIndiceCourant := 1;
        tabCstr[tabCstrIndiceCourant] := cstr;
      end
      else
      begin
        tabCstrIndiceCourant := tabCstrIndiceCourant + 1;
        tabCstr[tabCstrIndiceCourant] := cstr;
      end;
      // gestion de la mortalité
      if (NbJourCompte >= 5) then
      begin // on peut moyenner...
        MoyenneCstr := 0;
        for i := 1 to 5 do
        begin
          MoyenneCstr := MoyenneCstr + tabCstr[i];
        end;
        if ((MoyenneCstr / 5) <= SeuilCstrMortality) then
        begin
          NumPhase := 7;
        end;
      end;
    end;
  except
    AfficheMessageErreur('SorghumMortality', URiz);
  end;
end;

//##############################################################################
///  Calcul de l'infiltration
//##############################################################################

procedure EvalInfiltration(const Pluie, Lr: Double;
  var Infiltration: Double);
begin
  try
    Infiltration := Pluie - Lr;
  except
    AfficheMessageErreur('EvalInfiltration', USorghum);
  end;
end;

//##############################################################################
///  Calcul du drainage
//##############################################################################

procedure EvalDrainage(const Pluie, Infiltration, StockRacinaire: Double;
  var Drainage: Double);
begin
  try
    Drainage := max(0, Pluie - (StockRacinaire - StockRacinairePrec) -
      Infiltration);
    StockRacinairePrec := StockRacinaire;
  except
    AfficheMessageErreur('EvalDrainage', USorghum);
  end;
end;

procedure EvalDrainageSansCulture(const Pluie, Infiltration: Double;
  var Drainage: Double);
begin
  try
    Drainage := max(0, Pluie - Infiltration);
  except
    AfficheMessageErreur('EvalDrainage', USorghum);
  end;
end;

//##############################################################################
///  Calcul de cumul général d'une valeur entre la deux phase PhaseDebutCumul et PhaseFinCumul
//##############################################################################

procedure CumulGenerique(const ValeurACumuler, NumPhase, PhaseDebutCumul,
  PhaseFinCumul: double;
  var Cumul: Double);
begin
  try
    if PhaseDebutCumul < 0 then
    begin
      if NumPhase <= PhaseFinCumul then
        Cumul := Cumul + ValeurACumuler
      else
        Cumul := 0;
    end
    else
    begin
      if ((NumPhase >= PhaseDebutCumul) and (NumPhase <= PhaseFinCumul)) then
        Cumul := Cumul + ValeurACumuler
      else
        Cumul := 0;
    end;
  except
    AfficheMessageErreur('CumulGenerique', USorghum);
  end;
end;

procedure CumulGeneriqueComplet(const ValeurACumuler: double;
  var Cumul: Double);
begin
  try
    Cumul := Cumul + ValeurACumuler;
  except
    AfficheMessageErreur('CumulGeneriqueComplet', USorghum);
  end;
end;

//##############################################################################
///  Calcul du nombre de jour de pluie entre la phase PhaseDebutNbPluie et PhaseFinNbPluie
//##############################################################################

procedure CompteJourPluvieux(const Pluie, NumPhase, PhaseDebutNbPluie,
  PhaseFinNbPluie: double;
  var NbJourPluvieux: Double);
begin
  try
    if ((NumPhase >= PhaseDebutNbPluie) and (NumPhase <= PhaseFinNbPluie)) then
    begin
      if Pluie <> 0 then
        NbJourPluvieux := NbJourPluvieux + 1;
    end; { TODO : voir l'initialisation si PhaseDebutNbPluie=0 }
  except
    AfficheMessageErreur('CompteJourPluivieux', USorghum);
  end;
end;

//##############################################################################
///  Calcul du sla suivant méthodologie Mitchterlich
//##############################################################################

procedure EvalSlaMitch(const SlaMax, SlaMin, AttenMitch, SDJ, SDJLevee,
  NumPhase: double;
  var sla: Double);
begin
  try
    if NumPhase > 1 then
      sla := SlaMin + (SlaMax - SlaMin) * Power(AttenMitch, (SDJ - SDJLevee))
    else
      sla := 0;
  except
    AfficheMessageErreur('EvalSlaMitch', USorghum);
  end;
end;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

procedure SorghumMortalityDyn(var T: TPointeurProcParam);
begin
  SorghumMortality(T[0], T[1], T[2]);
end;

procedure EvalInfiltrationDyn(var T: TPointeurProcParam);
begin
  EvalInfiltration(T[0], T[1], T[2]);
end;

procedure EvalDrainageDyn(var T: TPointeurProcParam);
begin
  EvalDrainage(T[0], T[1], T[2], T[3]);
end;

procedure EvalDrainageSansCultureDyn(var T: TPointeurProcParam);
begin
  EvalDrainageSansCulture(T[0], T[1], T[2]);
end;

procedure CumulGeneriqueDyn(var T: TPointeurProcParam);
begin
  CumulGenerique(T[0], T[1], T[2], T[3], T[4]);
end;

procedure CumulGeneriqueCompletDyn(var T: TPointeurProcParam);
begin
  CumulGeneriqueComplet(T[0], T[1]);
end;

procedure CompteJourPluvieuxDyn(var T: TPointeurProcParam);
begin
  CompteJourPluvieux(T[0], T[1], T[2], T[3], T[4]);
end;

procedure EvalSlaMitchDyn(var T: TPointeurProcParam);
begin
  EvalSlaMitch(T[0], T[1], T[2], T[3], T[4], T[5], T[6]);
end;

////////////////////////////////////////////////////////////////////////////////
initialization
  TabProc.AjoutProc('SorghumMortality', SorghumMortalityDyn);
  TabProc.AjoutProc('EvalInfiltration', EvalInfiltrationDyn);
  TabProc.AjoutProc('EvalDrainage', EvalDrainageDyn);
  TabProc.AjoutProc('EvalDrainageSansCulture', EvalDrainageSansCultureDyn);
  TabProc.AjoutProc('CumulGenerique', CumulGeneriqueDyn);
  TabProc.AjoutProc('CumulGeneriqueComplet', CumulGeneriqueCompletDyn);
  TabProc.AjoutProc('CompteJourPluvieux', CompteJourPluvieuxDyn);
  TabProc.AjoutProc('EvalSlaMitch', EvalSlaMitchDyn);
end.

