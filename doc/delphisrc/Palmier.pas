unit Palmier;
// unité dédiée à Ecopalm
// crée le 09/12/2002 en remplacement d'EcoPalmMois implémentée par CB
interface

uses SysUtils, Math, Dialogs, Variants, DateUtils;
type
  IcMensuel = record
    Mois, Annee: Word;
    ValeurIc: double;
  end;
procedure DecaleTab(var Tab: array of Double; Decal: Integer);
function RechercheIcDecale(laDate: TDateTime; leDecalage: Integer): Double;

var
  TabDemReproMois: array[1..12] of double;
    // tableau d'indices de récolte potentiels issues du nombre de floraison possible pour d'un site
var
  tabIcMensuel: array of IcMensuel;

implementation

uses ModelsManage, Main, GestionDesErreurs;

// #################################################################
// ######            Initialisation des palmiers            ########
// #################################################################

procedure IniPalmier(const SeuilPP, Latitude, PPCrit, LaiIni: Double;
  {Proc 200}var Lai: Double);
// ########    Initialisation de la culture Palmier   ##########
// Calcul du potentiel de récolte pour un site donné basé sur le nombre de floraison femelle possible
// Le résultat constitue un tableau de 12 valeurs (ces valeurs sont le nb de floraison donc
// de récolte pour un mois donné sur le site donné). Les calculs du potentiel
// sur l'année 1999
var

  DateDuJour: TDateTime;
  I: Integer;
  NbFlo: Double;
  PPefficace, SumPP: Double;
  DureeDuJour, PositionDuSoleil, LatitudeEnRadiant, DeclinaisonDuSoleil: Double;
begin
  try
    if ModeDebug then
      MainForm.memDeroulement.Lines.Add(TimeToStr(Time) + #9 +
        'Proc. n°200, Initialisation du palmier...');
    //Initialisation ViB 07/01/04
    SumPP := 0;
    SetLength(tabIcMensuel, 0); // ViB le 10/09/2004
    for I := 1 to 12 do
      TabDemReproMois[I] := 0;
    NbFlo := 0;
    for I := 1 to 365 do // Parcours des 365 jours de l'année 1999
    begin
      DateDuJour := EncodeDate(1997, 01, 01) - 1 + I;
      while NbFlo = 0 do
      begin
        // Calcul de la demande potentielle sur un site pour une espèce donnée.
        DeclinaisonDuSoleil := 0.409 * Sin(0.0172 * DayOfTheYear(DateDuJour) -
          1.39);
        LatitudeEnRadiant := Latitude * PI / 180;
        PositionDuSoleil := ArcCos(-Tan(LatitudeEnRadiant) *
          Tan(DeclinaisonDuSoleil));
        DureeDuJour := 7.64 * PositionDuSoleil;
        PPefficace := max(0.01, DureeDuJour - PPCrit);
        SumPP := SumPP + (1 / PPefficace);
        if SumPP >= SeuilPP then
        begin
          NbFlo := NbFlo + 1;
          SumPP := 0;
        end
        else
          DateDuJour := DateDuJour + 1;
      end;

      // si on est sorti du while, c'est qu'on a trouvé une florescence dans Proc 208
      // ... donc on ajoute dans le tableau au mois où celle-ci est sortie
      //  aPlante.TabDemReproMois[MonthOf(aSimule.thisDate)]:= aPlante.TabDemReproMois[MonthOf(aSimule.thisDate)]+1; //ViB Modif 07/01/04
      TabDemReproMois[MonthOf(DateDuJour)] :=
        TabDemReproMois[MonthOf(DateDuJour)] + 1;
      NbFlo := 0;
    end;

    DecaleTab(TabDemReproMois, 6);
      // décale de 6 mois les floraisons car une fleur donne 6 mois après un régime
    if ModeDebug then // on affiche le tableau de la demande potentielle
    begin
      MainForm.memDeroulement.Lines.Add('-- Récapitulatif demande potentielle--');
      for i := 1 to 12 do
        MainForm.memDeroulement.Lines.Add('  mois n°' + IntToStr(i) + ': ' +
          floattostr(TabDemReproMois[i]));
    end;
    Lai := LaiIni;
  except
    AfficheMessageErreur('IniPalmier', UPalmier);
  end;
end;

procedure IniCulturePerenne(const DateMaturite, NumPhase: double);
{Proc 202}
begin
  // Cette procédure ne sert qu'à adapter SarraH v3 optimisé aux cultures pérennes
  // en déclarant des variables (telles que la date de maturité) qui sont
  // nécéssaire au bon fonctionnement de SarraH v3
  try
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°202, Initialisation des cultures pérènnes effectuée');
  except
    AfficheMessageErreur('IniCulturePerenne', UPalmier);
  end;
end;

// ##################################################################
// ######               Ecopalm V1                           ########
// ##################################################################

procedure EvalOffreBruteJour(const KEpsiB, Par, Ltr, Cstr: double;
  {Proc 205}var OffreBruteJour: Double);
// calcul journalier de la photosynthèse
// remplace Biomasse.Assimilat Proc76
begin
  try
    OffreBruteJour := Par * KEpsiB * Cstr * 10 * (1 - LTR);
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°205, Offre brute du jour: ' +
        FloatToStr(OffreBruteJour));
  except
    AfficheMessageErreur('EvalOffreBruteJour n°205 ', UPalmier);
  end;
end;

////////////////////////////////////////////////////////////////////

procedure EvolOffreBruteMois(const OffreBruteJour: Double; const thisDate:
  TDateTime;
  {Proc 206}var OffreBruteMois: Double);
// Calcul mensuel de la photosynthèse
// remplace CumulMois
var
  Year, Month, Day: Word;
begin
  try
    DecodeDate(thisdate, Year, Month, Day);
    if Day = 1 then
      OffreBruteMois := 0;
    OffreBruteMois := OffreBruteMois + OffreBruteJour;
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°206 (' + DateToStr(thisDate) +
        '), Offre brute du mois: ' + FloatToStr(OffreBruteMois));
  except
    AfficheMessageErreur('EvolOffreBruteMois n°206 ', UPalmier);
  end;
end;

////////////////////////////////////////////////////////////////////

procedure EvalOffreNetteMois(const thisDate: TDateTime; const OffreBruteMois,
  KRespiration: Double;
  {Proc 207}var OffreNetteMois: Double);
// Calcul mensuel de l'offre nette dédiée à la reproduction
// remplace partiellement EvalOffreMois
begin
  try
    OffreNetteMois := max(0, OffreBruteMois - KRespiration);
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°207 (' + DateToStr(thisDate) +
        '), Offre nette du mois: ' + FloatToStr(OffreNetteMois));
  except
    AfficheMessageErreur('EvalOffreNetteMois n°207 ', UPalmier);
  end;
end;

/////////////////////////////////////////////////////////////////////

function RechercheIcDecale(laDate: TDateTime; leDecalage: Integer): Double;
var
  i: Integer;
  laDateTemp: TDateTime;
  LaDateDebutSimul: TDateTime;
begin
  try
    Result := -999; // par défaut, on renvoi -999
    laDateTemp := EndOfTheMonth(IncMonth(laDate, -leDecalage));
      // on récupère la fin de mois décalée de leDecalage mois
    if length(tabIcMensuel) <> 0 then
    begin
      LaDateDebutSimul := EndOfTheMonth(EncodeDate(tabIcMensuel[0].Annee,
        tabIcMensuel[0].Mois, 1));
        // ceci représente le dernier jour du premier mois de la simulation
      if laDateTemp < LaDateDebutSimul then
        Result := -999
      else
      begin
        for i := 0 to length(tabIcMensuel) - 1 do
        begin
          if ((tabIcMensuel[i].Mois = MonthOf(laDateTemp)) and
            (tabIcMensuel[i].Annee = YearOf(laDateTemp))) then
          begin
            Result := tabicMensuel[i].ValeurIc;
          end;
        end;
      end; // fin du else if laDateTemp<LaDateDebutSimul then
    end; // fin du If length(tabIcMensuel)<>0 then
  except
    AfficheMessageErreur('RechercheIcDecale ', UPalmier);
  end;
end;

/////////////////////////////////////////////////////////////////////

procedure EvalDemandeMois(const thisDate: TDateTime; const KPondIc3, KPondIc2,
  KPondIc1,
  {Proc 209}KRendementPot, MoisIc1, MoisIc2, MoisIc3: Double;
  var DemandeReproMois, DemandeReproPot: Double);
// Calcul de la demande repoductive mensuelle pondérée
// remplace intégralement EvalDemandeReproMoisKPond Proc 64
var
  LesFreins, F3, F2, F1: Double;
  PotentielFloraison: Double;
begin
  try
    PotentielFloraison := TabDemReproMois[MonthOf(thisDate)];
    DemandeReproPot := (PotentielFloraison / 365) * KRendementPot;
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
    LesFreins := min(1, KPondIc3 * F3 + (1 - KPondIc3))
      * min(1, KPondIc2 * F2 + (1 - KPondIc2))
      * min(1, KPondIc1 * F1 + (1 - KPondIc1));
    DemandeReproMois := max(0.001, DemandeReproPot * LesFreins);
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°209 (' + DateToStr(thisDate) +
        '), DemandeReproMois: ' + FloatToStr(DemandeReproMois));
  except
    AfficheMessageErreur('EvalDemandeMois n°209 ', UPalmier);
  end;
end;

/////////////////////////////////////////////////////////////////////

procedure EvolReserveMois(const thisDate: TDateTime; const OffreNetteMois,
  DemandeReproMois, KReallocReserve: double;
  {Proc 215}var ReserveMois, Reallocation: double);
// Calcule le stock de réserve mensuel
// remplace une partie de RepartiAssimilats Proc 65
var
  DeltaOffreDemande: double;
begin
  try
    DeltaOffreDemande := OffreNetteMois - DemandeReproMois;
    if DeltaOffreDemande < 0 then
    begin
      Reallocation := Min(ReserveMois, -DeltaOffreDemande * KReallocReserve);
      ReserveMois := Max(0, ReserveMois - Reallocation);
    end
    else
    begin
      Reallocation := 0;
      ReserveMois := ReserveMois + DeltaOffreDemande;
    end;
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°215 (' + DateToStr(thisDate) +
        '),  Réserves du mois (' + IntToStr(Monthof(thisDate)) + '/' +
        IntToStr(Yearof(thisDate)) + '): ' + FloatToStr(ReserveMois) +
        ' | Réallocation: ' + floattostr(Reallocation));
  except
    AfficheMessageErreur('EvolReserveMois n°215 ', UPalmier);
  end;
end;

/////////////////////////////////////////////////////////////////////

procedure EvalRendementMois(const thisDate: TDateTime; const DemandeReproMois,
  OffreNetteMois, Reallocation,
  {Proc 217}KChimique, KTeneurHuile, KTeneurEau: double;
  var RendementRegimePS, RendementRegime, RendementHuile: double);
// calcule le rendement en poids sec de régime et en poids d'huile
// par hectare et par mois
// remplace une partie de RepartiAssimilats Proc 65
begin
  try
    RendementRegimePS := Min(DemandeReproMois, max(0, OffreNetteMois) +
      Reallocation) / KChimique;
    RendementRegime := RendementRegimePS / (1 - KTeneurEau);
      //rendement exprimé en Poids Frais JCC 13/09/04
    //RendementHuile := (RendementRegime+(RendementRegime*KTeneurEau))*KTeneurHuile;
    RendementHuile := RendementRegime * KTeneurHuile;
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°217 (' + DateToStr(thisDate) +
        '),  Rendement régime: ' + FloatToStr(RendementRegime) +
        ' | Rendement huile: ' + floattostr(RendementHuile));
  except
    AfficheMessageErreur('EvalRendementMois n°217 ', UPalmier);
  end;
end;

/////////////////////////////////////////////////////////////////////

procedure EvalIcMois(const thisDate: TDateTime; const DemandeReproMois,
  OffreNetteMois, Reallocation: double;
  {Proc 218}var IcMois: double);
// calcul de l'indice de compétition mensuel
// remplace une partie de RepartiAssimilats Proc 65
begin
  try
    IcMois := Min(DemandeReproMois, OffreNetteMois + Reallocation) /
      DemandeReproMois;
    // mémorisation de l'Ic dans le tableau...
    SetLength(tabIcMensuel, length(tabIcMensuel) + 1);
    tabIcMensuel[High(tabIcMensuel)].Mois := MonthOf(thisDate);
    tabIcMensuel[High(tabIcMensuel)].Annee := YearOf(thisDate);
    tabIcMensuel[High(tabIcMensuel)].ValeurIc := IcMois;
    if modeDebugPrecis then
      MainForm.memDeroulement.Lines.Add('Proc. n°218 (' + DateToStr(thisDate) +
        '), Indice de compétition du mois: ' + FloatToStr(IcMois));
  except
    AfficheMessageErreur('EvalIcMois n°218 ', UPalmier);
  end;
end;

// ##################################################################
// ###### D. Autres Procédures (outils spécifiques palmier)  ########
// ##################################################################

procedure DecaleTab(var Tab: array of Double; Decal: Integer);
//ex d'appel : DecaleTab(aPlante.TabDemReproMois,6);
// DecaleTab permets de décaler de Decal indice
var
  I, Posi: Integer;
  Tab2: array[0..11] of Double;
begin
  try
    //  Setlength(Tab2, High (Tab)+1);
    for I := Low(Tab) to High(Tab) do
      Tab2[I] := Tab[I]; // copie le tableau Tab dans Tab2
    for I := Low(Tab) to High(Tab) do
    begin
      Posi := I + decal; // décale de Decal (ici 6 mois) les valeurs dans Tab
      if Posi > High(Tab) then
        Posi := Posi - (High(Tab) + 1);
      Tab[I] := Tab2[Posi];
    end;
  except
    AfficheMessageErreur('DecaleTab ', UPalmier);
  end;
end;

//-------------Spécifique PALMIER--------------------

procedure IniPalmierFichier(const SeuilPP, Latitude, PPCrit, TMoy, TempPhase,
  LaiIni: Double;
  {Proc 221}var SumPP, Lai: Double);
// ########    Initialisation de la culture Palmier   ##########
// Calcul du potentiel de récolte pour un site donné basé sur le nombre de floraison femelle possible
// Le résultat constitue un tableau de 12 valeurs (ces valeurs sont le nb de floraison donc
// de récolte pour un mois donné sur le site donné)
// Gestion du fichier de sortie: un fichier est généré pour noter les dates
// d'initiation d'inflorescence et celle d'épanouissement.
var
  DateDuJour, DateIni: TDateTime;
  I: Integer;
  MonFichier: TextFile;
  MaLigne: string;
  NbFlo: Double;
  PPefficace: Double;
  DureeDuJour, PositionDuSoleil, LatitudeEnRadiant, DeclinaisonDuSoleil: Double;
begin
  try
    if ModeDebug then
      MainForm.memDeroulement.Lines.Add('Proc. n°221, Initialisation du palmier avec sortie fichier...');
    // Initialisation de mon fichier de sortie
    AssignFile(MonFichier, 'Floraison.txt');
    if FileExists('Floraison.txt') then
    begin
      Erase(MonFichier);
    end;
    Rewrite(MonFichier);
    Maligne := 'KSeuilPP=' + FloatToStr(SeuilPP) + #9;
    MaLigne := MaLigne + 'KPPCrit=' + FloatToStr(PPCrit);
    Writeln(MonFichier, MaLigne);
    Writeln(MonFichier,
      'DateInitiation'#9'NbPalme'#9'DateEpanouissement'#9'NB jour');

    // Parcours des 365 jours de l'année 1999
    //Initialisation ViB 07/01/04
    for I := 1 to 12 do
      TabDemReproMois[I] := 0;
    NbFlo := 0;
    for I := 1 to 365 do // Parcours des 365 jours de l'année 1999
    begin
      DateDuJour := EncodeDate(1999, 01, 01) - 1 + I;
      DateIni := DateDuJour;
      while NbFlo = 0 do
      begin
        // Calcul de la demande potentielle sur un site pour une espèce donnée.
        DeclinaisonDuSoleil := 0.409 * Sin(0.0172 * DayOfTheYear(DateDuJour) -
          1.39);
        LatitudeEnRadiant := Latitude * PI / 180;
        PositionDuSoleil := ArcCos(-Tan(LatitudeEnRadiant) *
          Tan(DeclinaisonDuSoleil));
        DureeDuJour := 7.64 * PositionDuSoleil;
        PPefficace := max(0.01, DureeDuJour - PPCrit);
        SumPP := SumPP + (1 / PPefficace);
        if SumPP >= SeuilPP then
        begin
          NbFlo := NbFlo + 1;
          SumPP := 0;
        end;
        DateDuJour := DateDuJour + 1;
      end;
      MaLigne := DateToStr(DateIni) + #9 +
        FloatToStr((TMoy - TempPhase) * (143 / 247.7)) + #9 +
        DateToStr(DateDuJour) + #9 +
        IntToStr(DaysBetween(DateDuJour, DateIni));
      Writeln(MonFichier, MaLigne);
      // si on est sorti du while, c'est qu'on a trouvé une florescence dans Proc 208
      // ... donc on ajoute dans le tableau au mois où celle-ci est sortie
      //  aPlante.TabDemReproMois[MonthOf(aSimule.thisDate)]:= aPlante.TabDemReproMois[MonthOf(aSimule.thisDate)]+1; //ViB Modif 07/01/04
      TabDemReproMois[MonthOf(DateDuJour)] :=
        TabDemReproMois[MonthOf(DateDuJour)] + 1;
      NbFlo := 0;
    end;

    CloseFile(MonFichier);

    DecaleTab(TabDemReproMois, 6);
      // décale de 6 mois les floraisons car une fleur donne 6 mois après un régime
    SumPP := 0;
    if ModeDebug then // on affiche le tableau de la demande potentielle
    begin
      MainForm.memDeroulement.Lines.Add('-- Récapitulatif demande potentielle--');
      for i := 1 to 12 do
        MainForm.memDeroulement.Lines.Add('  mois n°' + IntToStr(i) + ': ' +
          floattostr(TabDemReproMois[i]));
    end;
    Lai := LaiIni;
  except
    AfficheMessageErreur('IniPalmierFichier n°221 ', UPalmier);
  end;
end;
{
Procedure CohortePalmier;
//Proc 220
// Cette procédure est une procédure de test réalisée pour JCC
// Elle doit logguer la phénologie dans Ecopalm pour tester une hypothèse.
var
SeuilPP,PPCrit:Double;
StadeDebutCroissFeuille,StadeSortieFleche,StadeAvortement,
StadeFinCroissFeuille,StadeAnthese,StadeDebutRemplissage,
StadeDebutOleasynth,StadeRecolte:Double; // en degres jours
MonPointeurDate:TDateTime;
IniInflo,DebutCroissFeuille,SortieFleche,Avortement,FinCroissFeuille,Anthese,
DebutRemplissage,DebutOleasynth,Recolte:Boolean;
MonFichier:TextFile;
MaLigne:string;
TBase,Densite,SomDJPalme:Double;
begin
// Paramétrage
SeuilPP:=1040;
PPCrit:=11.5;
StadeDebutCroissFeuille:=6250;
StadeSortieFleche:=7560;
StadeAvortement:=8230;
StadeFinCroissFeuille:=10490;
StadeAnthese:=11100;
StadeDebutRemplissage:=11680;
StadeDebutOleasynth:=13130;
StadeRecolte:=14000;
TBase:=10;
Densite:=143;
SomDJPalme:=290;

// Initialisation de mon fichier de sortie
AssignFile(MonFichier,'CohortePalmier.txt');
if FileExists('CohortePalmier.txt') then
  begin
  Erase(MonFichier);
  end;
Rewrite(MonFichier);
Writeln(MonFichier,'DateCohorte'#9'NbPalmeParHa'#9'IniInflo'#9'DebCroisFeuille'#9'SortieFleche'#9'Avortement'#9'FinCroisFeuille'#9'Anthese'#9'DebRemplissage'#9'DebOleaSynth'#9'Recolte');
MonPointeurDate:=StrToDate(MainForm.dbediDebutSimule.Text);

// bouclage du premier au dernier jour de simulation
while MonPointeurDate<=StrToDate(MainForm.dbediFinSimule.Text) do // sera utilisé pour revenir au bon jour dans le premier cycle
  begin
  MainForm.memDeroulement.Lines.Add('Cohorte traitée: '+datetostr(MonPointeurDate));
  MainForm.memDeroulement.SetFocus;
  Application.ProcessMessages;
  aSimule.thisDate:=MonPointeurDate;
  aSite.LitVar(aSimule.thisDate, 2);
  MaLigne:=DateToStr(asimule.thisDate)+#9+FloatToStr((aSite.TabState[TMoy]-TBase)/SomDJPalme*Densite)+#9;
  // Initialisation des variables
  IniInflo:=False;
  DebutCroissFeuille:=False;
  SortieFleche:=False;
  Avortement:=False;
  FinCroissFeuille:=False;
  Anthese:=False;
  DebutRemplissage:=False;
  DebutOleasynth:=False;
  Recolte:=False;
  aCrop.TabState[SumDD]:=0;
  aCrop.TabState[SumPP]:=0;

  // bouclage du jour courant (début de la cohorte) à la récolte de celle-ci.
  while not Recolte do
    begin
    aSite.LitVar(aSimule.thisDate, 2);
    //---> évolution de la photopériode (jusqu'au stade de l'initiation de la floraison)
    if not IniInflo then // on est encore en phase post intitiation floraison, la PP agit.
      begin
      if aCrop.TabState[SumPP]<=SeuilPP then
        begin // on fait évoluer la photopériode (PP)
        runModule(50);  // decli
        runModule(51);  // SunPosi
        runModule(52);  // DayLength
        aCrop.TabState[SumPP]:=aCrop.TabState[SumPP]+ 1/(max(0.01, aSite.TabState[DayLength] - PPCrit));
        end;
      if aCrop.TabState[SumPP]>=SeuilPP then
        begin
        MaLigne:=MaLigne+datetostr(asimule.thisDate)+#9;
        IniInflo:=True;
        end;
      end;

    //---> évolution de la somme des degrès jours
    if IniInflo then
      begin
      aCrop.TabState[SumDD]:=aCrop.TabState[SumDD]+aSite.TabState[TMoy]-TBase;
      end;

    if not DebutCroissFeuille then
      begin
      if acrop.TabState[SumDD]>=StadeDebutCroissFeuille then
        begin
        DebutCroissFeuille:=True;
        MaLigne:=MaLigne+datetostr(asimule.thisDate)+#9;
        end;
      end;
    if not SortieFleche then
      begin
      if acrop.TabState[SumDD]>=StadeSortieFleche then
        begin
        SortieFleche:=True;
        MaLigne:=MaLigne+datetostr(asimule.thisDate)+#9;
        end;
      end;
    if not Avortement then
      begin
      if acrop.TabState[SumDD]>=StadeAvortement then
        begin
        Avortement:=True;
        MaLigne:=MaLigne+datetostr(asimule.thisDate)+#9;
        end;
      end;
    if not FinCroissFeuille then
      begin
      if acrop.TabState[SumDD]>=StadeFinCroissFeuille then
        begin
        FinCroissFeuille:=True;
        MaLigne:=MaLigne+datetostr(asimule.thisDate)+#9;
        end;
      end;
    if not Anthese then
      begin
      if acrop.TabState[SumDD]>=StadeAnthese then
        begin
        Anthese:=True;
        MaLigne:=MaLigne+datetostr(asimule.thisDate)+#9;
        end;
      end;
    if not DebutRemplissage then
      begin
      if acrop.TabState[SumDD]>=StadeDebutRemplissage then
        begin
        DebutRemplissage:=True;
        MaLigne:=MaLigne+datetostr(asimule.thisDate)+#9;
        end;
      end;
    if not DebutOleasynth then
      begin
      if acrop.TabState[SumDD]>=StadeDebutOleasynth then
        begin
        DebutOleasynth:=True;
        MaLigne:=MaLigne+datetostr(asimule.thisDate)+#9;
        end;
      end;
    if not Recolte then
      begin
      if acrop.TabState[SumDD]>=StadeRecolte then
        begin
        Recolte:=True;
        MaLigne:=MaLigne+datetostr(asimule.thisDate);
        end;
      end;
    aSimule.thisDate:=aSimule.thisDate+1;
    end;
  Writeln(MonFichier,MaLigne);
  MonPointeurDate:=MonPointeurDate+1;
  end;
CloseFile(MonFichier);
ShowMessage('Fin');
end;
//-------------Fin Spécifique PALMIER-----------------

// ##############   Appel des procédures  ######################
    200 : IniPalmier;
    205 : EvolOffreBruteJour(aPlante.TabState[KEpsiB],
                               aSite.TabState[Par],
                               aPlot.TabState[Ltr],
                               aCrop.TabState[cstr],
                               aCrop.TabState[dayBiomTot]);
    206 : EvolOffreBruteMois(aCrop.TabState[dayBiomTot],
                               aSimule.thisDate,
                               aCrop.TabState[AssimilatMois]);
    207 : EvalOffreNetteMois(aCrop.TabState[AssimilatMois],
                               aPlante.TabState[KRespiration],
                               aCrop.TabState[OffreReproMois]);
    208 : EvalDemandePot(aPlante.TabState[KSeuilPP],
                               aSite.TabState[DayLength],
                               aPlante.TabState[KPPCrit],
                               aPlante.TabState[NbFlo],
                               aSite.TabState[SitePP]);
    209 : EvalDemandeMois(aPlante.TabState[KPondIc30],
                               aCrop.TabState[Ic30],
                               aPlante.TabState[KPondIc6],
                               aCrop.TabState[Ic6],
                               aPlante.TabState[KPondIc1],
                               aCrop.TabState[Ic1],
                               aPlante.TabDemReproMois[MonthOf(aSimule.thisDate)],
                               aPlante.TabState[KRendementPot],
                               aCrop.TabState[DemReproMois]);
    215 : EvolReserveMois(aCrop.TabState[OffreReproMois],
                               aCrop.TabState[DemReproMois],
                               aPlante.TabState[KReallocReserve],
                               aCrop.TabState[ReserveMois],
                               aCrop.TabState[Reallocation]);
    217 : EvalRendementMois(aCrop.TabState[DemReproMois],
                               aCrop.TabState[OffreReproMois],
                               aCrop.TabState[Reallocation],
                               aPlante.TabState[KChimique],
                               aPlante.TabState[KTeneurHuile],
                               aPlante.TabState[KTeneurEau],
                               aCrop.TabState[RdtMois],
                               aCrop.TabState[RendementHuile]);
    218 : EvalIcMois(aCrop.TabState[DemReproMois],
                               aCrop.TabState[OffreReproMois],
                               aCrop.TabState[Reallocation],
                               aCrop.TabState[IcReproMois]);
    220 : CohortePalmier;

    //208 : PalmierEnCours;
    //210 : InitPalmierEncours;
}

procedure IniCulturePerenneDyn(var T: TPointeurProcParam);
begin
  IniCulturePerenne(T[0], T[1]);
end;

procedure IniPalmierDyn(var T: TPointeurProcParam);
begin
  IniPalmier(T[0], T[1], T[2], T[3], T[4]);
end;

procedure EvalOffreBruteJourDyn(var T: TPointeurProcParam);
begin
  EvalOffreBruteJour(T[0], T[1], T[2], T[3], T[4]);
end;

procedure EvolOffreBruteMoisDyn(var T: TPointeurProcParam);
begin
  EvolOffreBruteMois(T[0], T[1], T[2]);
end;

procedure EvalOffreNetteMoisDyn(var T: TPointeurProcParam);
begin
  EvalOffreNetteMois(T[0], T[1], T[2], T[3]);
end;

procedure EvalDemandeMoisDyn(var T: TPointeurProcParam);
begin
  EvalDemandeMois(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9]);
end;

procedure EvolReserveMoisDyn(var T: TPointeurProcParam);
begin
  EvolReserveMois(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure EvalRendementMoisDyn(var T: TPointeurProcParam);
begin
  EvalRendementMois(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9]);
end;

procedure EvalIcMoisDyn(var T: TPointeurProcParam);
begin
  EvalIcMois(T[0], T[1], T[2], T[3], T[4]);
end;

procedure IniPalmierFichierDyn(var T: TPointeurProcParam);
begin
  IniPalmierFichier(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7]);
end;

initialization
  TabProc.AjoutProc('IniCulturePerenne', IniCulturePerenneDyn);
  TabProc.AjoutProc('IniPalmier', IniPalmierDyn);
  TabProc.AjoutProc('EvalOffreBruteJour', EvalOffreBruteJourDyn);
  TabProc.AjoutProc('EvolOffreBruteMois', EvolOffreBruteMoisDyn);
  TabProc.AjoutProc('EvalOffreNetteMois', EvalOffreNetteMoisDyn);
  TabProc.AjoutProc('EvalDemandeMois', EvalDemandeMoisDyn);
  TabProc.AjoutProc('EvolReserveMois', EvolReserveMoisDyn);
  TabProc.AjoutProc('EvalRendementMois', EvalRendementMoisDyn);
  TabProc.AjoutProc('EvalIcMois', EvalIcMoisDyn);
  TabProc.AjoutProc('IniPalmierFichier', IniPalmierFichierDyn);
end.

