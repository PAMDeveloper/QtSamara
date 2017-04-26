//
// Cette unite permet de gerer une simulation
// La procedure BilanParcelle est la plus importante
//
// Auteur(s)       :
// Unité(s) à voir : <unité ayant un lien fort>
//
// Date de commencement          :
// Date de derniere modification : 11/02/04
// Etat : fini, teste
//

//{$DEFINE AffichageComplet}

unit ClasseSimule;

interface

uses ModelsManage, SysUtils, DB, DBTables, Variants, DateUtils, Dialogs,
{ajout VN contraintes} Contrainte, Parser10, Classes;

type
  ///Concept : caractérise une simulation.
  ///Description : Cet objet représente une simulation. Il permet de gérer celle-ci. Les méthodes de TSimule servent à construire le modèle et l'appel des modules le composant.
  TSimule = class(TEntityInstance)
    {Ajout VN save}ContextSave: TstringList;
    {Ajout VN save}TabAppelSave: TStringList; {Ajout VN save}
      //ContextSave:array of TContext;
    {Ajout VN contraintes}Pars: Parser10.TParser;
      /// Parser Mathématique, pour le calcul des expression des contraintes.

    thisDate: TDateTime; /// représente la date en cours pendant la simulation
    calendaire: Boolean;
      /// permet de spécifier si on calcule les dates en tenant compte du calendrier
    minorStep: Real;
      /// représente le pas de temps le plus petit de la simulation -toujours la journée- .
    {Ajout VN simcons}procedure Clean();
    procedure InitSimulation(var anDebutSimul, anFinSimul, nbAnSim,
      dateDebutSimul, dateFinSimul, annee, maxNbjSimule, dateEnCours, nbjSemis,
      dateSemisCalc: double; const nbjas, codeParcelle: double);
      /// Initialisation des divers paramètres d'entrée. Cette procédure correspond au module 0.
    procedure InitYear(const numSimul: integer);
      /// Initialisation des variables, création des instances initiales (comme l'instance Simulation, sol, etc... qui n'apparaissent pas en cours de simulation)
    procedure SetList;
      /// Construction de la liste des appels (construit une fois pour toute, cette liste ne contiendra pas les modules d'initialisation)
    //AD     procedure BilanParcelle(codeSimule : Integer);    ///La méthode BilanParcelle est la méthode appelée par la méthode ExecuterSimulation de la classe TScenario, permet de gérer le déroulement de la simulation
    procedure BilanParcelle(idSimule: string);
    procedure SimulAn; /// Permet la Gestion de la date de semis
    procedure CreationClasses;
      /// Requete pour création automatique des classes(site, crop ....)
    procedure RunModule(numModule: Integer);
      /// Procedure d'appel d'un module avec son numéro et le contexte associé à la simulation
    procedure Aller(const numSimule: integer);
      /// Gère le déroullement de la simulation :
    procedure StepExe;
      /// Execute le module si la date d'execution correspond à la date en cours
    procedure CalculNextExe;
      /// Calcule la date suivante et met à jour le tableau des appels.
    procedure ConfirmeSemis; /// Gestion de la date de semis
    procedure MajBookmarks(const numSimule: integer);
      /// Mise à jour des bookmark pour chaque table si la date correspond :
    function Conversion(x: string): TEnumTypeParam;
      /// Renvoie le type du parametre
    {Ajout VN Contraintes}procedure MajInVar_ToPars(InIndice: array of Shortint;
      TabParam: array of ModelsManage.TParam; ParamVal:
      ModelsManage.TPointeurProcParam); // Met a jour les variables du parser
    {Ajout VN Contraintes}procedure MajOutVar_ToProcParam(OutIndice: array of
      Shortint; TabParam: array of ModelsManage.TParam; var ParamVal:
      ModelsManage.TPointeurProcParam);
    {Ajout VN simcons}//procedure InitInstances();
    {Ajout VN simcons}procedure InitDatesSimule(const numSimul: Integer);
    {Ajout VN simcons}procedure InitDatesSemis(const numSimul: Integer);
    {Ajout VN save}procedure majNbJas(dateSemis, dateDebut, datefin: TDateTime);

  end;
var {ajout APas}
  numSimule2: integer;
    //represente le numéro de la simulation en cours pour simulations pluriannuelles

implementation

uses DBModule, GestionDesTables, main, LectDonnee, GestionDesErreurs, Forms,
ClasseScenario;

//----------------------------------------------------------------------------//
//VN simcons

procedure TSimule.Clean();
begin
  try
    Pars.free;
    while ContextSave.Count <> 0 do //on nettoit les sauvegardes
    begin
      TContextSave(ContextSave.Objects[0]).Free;
      ContextSave.Delete(0);
    end;
    ContextSave.free;

    while TabAppelSave.Count <> 0 do
    begin
      TTabAppelsSave(TabAppelSave.Objects[0]).Free;
      TabAppelSave.Delete(0);
    end;
    TabAppelSave.free;
  except
    AfficheMessageErreur('TSimule.Clean', UClasseSimule);
  end;
end;

procedure TSimule.CreationClasses;
// Requete pour création automatique des classes(site, crop ....)
// La requete trie les entités en fonction de leur hierarchie et celles
// dont la hierarchie est = 0 ne sont pas retenues. Les entités sont ensuite
// crées dans cet ordre.
var
  aClass: TEntityClass;
begin
  // Recuperation de la liste des entites necessaires pour ce modele
  // Voici un exemple de la requete DbModule1.RequeteEntites :
  //    REPONSE CodeEntity      NomEntite       Hierarchie
  //    1       3               Site	        1
  //    2       1               Plot            2
  //    3       4               Soil            2
  //    4       2               Crop            3
  //    5       5               Plante          3
  try
    DbModule1.RequeteEntites.Active := true;

    DbModule1.RequeteEntites.First;
    while not DbModule1.RequeteEntites.Eof do
    begin { TODO -ofred : dans le diag de classe 'ajouter' est une function, cela serait plus simple ? }
      tabClass.ajouter(DbModule1.RequeteEntitesNomEntite.Value);
      aClass := tabClass.Trouver(DbModule1.RequeteEntitesNomEntite.Value);
      aClass.AttributeFromTable(DbModule1.RequeteEntitesNomEntite.Value);
      DbModule1.RequeteEntites.Next;
        // 16/03/04 casse ici apres avoir fait un forcage qui fonctionne sur un autre modele FREEED
    end;

    DbModule1.RequeteEntites.Active := false;
  except
    AfficheMessageErreur('TSimule.CreationClasses', UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//

procedure TSimule.InitSimulation(var anDebutSimul, anFinSimul, nbAnSim,
  dateDebutSimul, dateFinSimul, annee,
  maxNbjSimule, dateEnCours, nbjSemis,
  dateSemisCalc: double;
  const nbjas, codeParcelle: double);
// Initialisation des divers paramètres d'entrée. Cette procédure correspond au module 0.
begin
  try
    // récupération des dates et écriture dans rescrop(ou tabValues) à la sortie de la procedure
    anDebutSimul := DBModule1.Simule.findfield('AnDebutSimul').Value;
    anFinSimul := DBModule1.Simule.findfield('AnFinSimul').Value;
    nbAnSim := DBModule1.Simule.findfield('NbAnSim').Value;
    dateFinSimul := DBModule1.Simule.findfield('FinSimul').Value;
    dateDebutSimul := DBModule1.Simule.findfield('DebutSimul').Value;
    annee := DBModule1.Simule.findfield('AnDebutSimul').Value;
    dateEnCours := DBModule1.Simule.findfield('DebutSimul').Value;
    maxNBJSimule := Trunc(DateFinSimul - DateDebutSimul) + 1;
    nbjSemis := NullValue;
    dateSemisCalc := NullValue;
  except
    AfficheMessageErreur('TSimule.InitSimulation', UClasseSimule);
  end;
end;

//----------------------------VN simcons-------------------------------------//

procedure TSimule.InitDatesSimule(const numSimul: Integer);
var
  jour, mois, an: Word;
begin
  try
    MainForm.affiche((TimeToStr(Time) + #9 +
      'TSimule2.InitYear  numSimul : ' + FloatToStr(numSimul),0);

    // initialisation des variables temporelles pour la nouvelle année
    SetVal('Annee', GetVal('AnDebutSimul') + numSimul);
    DecodeDate(GetVal('DebutSimul'), an, mois, jour);
    SetVal('DebutSimul', EncodeDate(trunc(GetVal('AnDebutSimul')) + NumSimul,
      mois, jour));
    SetVal('FinSimul', GetVal('DebutSimul') + GetVal('MaxNbjSimule'));

    thisDate := GetVal('DebutSimul');
    //ShowMessage(DateToStr(thisDate)+' '+DateToStr(GetVal('FinSimul')));
  except
    AfficheMessageErreur('TSimule.InitDatesSimule | Classe ', UClasseSimule);
  end;
end;

//----------------------------VN simcons-------------------------------------//

procedure TSimule.InitDatesSemis(const numSimul: Integer);
var
  jour, mois, an: Word;
begin
  try
    //Si la date de semis est definie par l'utilisateur 3
    //alors on initialise DateSemisCalc à cette valeur (+le nbre d'années simulées)
    //sinon DateSemisCalc prend la valeur 0
    if contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis') = nullValue
      then
      SetVal('DateSemisCalc', 0)
    else
    begin
      DecodeDate(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis'),
        An, Mois, Jour);
      SetVal('DateSemisCalc', EncodeDate(An + NumSimul, Mois, Jour));
    end;
    //Si la date de semis existe on l'initialise correctement
    //sinon on la met à nullvalue
    if contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis') <> nullValue
      then
      SetVal('NbJAS', thisdate - GetVal('DateSemisCalc'))
    else
      SetVal('NbJAS', nullValue);

    //ShowMessage(DateToStr(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis'))+' '+DateToStr(thisdate)+' '+DateToStr(GetVal('DateSemisCalc')));
  except
    AfficheMessageErreur('TSimule.InitDatesSemis | Classe ', UClasseSimule);
  end;
end;
//--------------------------------------------------------------------------//

procedure TSimule.InitYear(const numSimul: Integer);
// Initialisation des variables qui changent pour chaque années et création
// de toutes les instances la première année sauf Crop (ou celle qui apparaissent
// en cours de simulation
var
  jour, mois, an: Word;
  i: Integer;
begin
  try
    MainForm.affiche((TimeToStr(Time) + #9 +
      'TSimule2.InitYear  numSimul : ' + FloatToStr(numSimul),0);

    // initialisation des variables temporelles pour la nouvelle année
    SetVal('Annee', GetVal('AnDebutSimul') + numSimul);
    DecodeDate(GetVal('DebutSimul'), an, mois, jour);
    SetVal('DebutSimul', EncodeDate(trunc(GetVal('AnDebutSimul')) + NumSimul,
      mois, jour));
    SetVal('FinSimul', GetVal('DebutSimul') + GetVal('MaxNbjSimule'));

    thisDate := GetVal('DebutSimul');

    //ShowMessage(DateToStr(thisDate));
    //contextObjet.ResetContextInstance(thisDate);
    //contextObjet.GereCreationInstances(thisDate);

    if numSimul = 0 then
    begin
      // creation des instances d'entite et Initialiser
      DbModule1.RequeteInstance.Active := true;
      DbModule1.RequeteInstance.First;
      for i := 0 to DbModule1.RequeteInstance.RecordCount - 1 do
      begin
        // on créé une instance puis on la place dans le context
        contextObjet.SetCurrentInstance(TEntityInstance.create(DBModule1.RequeteInstanceNomEntite.Value, thisDate, ''));
        DbModule1.RequeteInstance.Next;
      end;
      DbModule1.RequeteInstance.Active := false;
    end

      // suppression de l'entite crop si on n'est pas à l'année 1
    else // numSimul <> 0
    begin
      if contextObjet.GetCurrentInstance('Crop') <> nil then
        contextObjet.FreeEntityInstance('Crop');
      if DBLocateDate(thisDate) then
      begin

        for i := 0 to length(contextObjet.currentInstances) - 1 do
        begin
          if contextObjet.currentInstances[i] <> nil then
          begin
            contextObjet.currentInstances[i].MAJBookmark(thisDate);
            contextObjet.currentInstances[i].MiseAZero(0);
            contextObjet.currentInstances[i].myClass.MAJTappels(thisDate);
          end;
        end;
      end;
    end;
    //VN Remplacement
       {DecodeDate(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis'),An,Mois,Jour);
       SetVal('DateSemisCalc',EncodeDate(trunc(GetVal('AnDebutSimul'))+ NumSimul,Mois,Jour));

       if contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis') = nullValue then
              begin { TODO : Gestion des dates de semis inconnues a finaliser }{ TODO -oVB : à faire oui...
         contextObjet.GetCurrentInstance('Plot').SetVal('DateSemis',GetVal('FinSimul'));
         //contextObjet.GetCurrentInstance('Plot').SetVal('TypeSemis',2);
         end
       else
         begin
         //contextObjet.GetCurrentInstance('Plot').SetVal('TypeSemis',1);
         DecodeDate(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis'),An,Mois,Jour);
         contextObjet.GetCurrentInstance('Plot').SetVal('DateSemis',EncodeDate(trunc(GetVal('AnDebutSimul'))+ NumSimul,Mois,Jour));
         end;

       SetVal('NbJAS',thisDate - contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis') );}

    if contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis') = nullValue
      then
      SetVal('DateSemisCalc', 0)
    else
    begin
      DecodeDate(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis'),
        An, Mois, Jour);
      SetVal('DateSemisCalc', EncodeDate(An + NumSimul, Mois, Jour));
    end;

    if contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis') <> nullValue
      then
      SetVal('NbJAS', thisdate - GetVal('DateSemisCalc'))
    else
      SetVal('NbJAS', nullValue);

    //AD   SetVal('CodeParcelle',DBModule1.SimuleCodeParcelle.Value);
        //ShowMessage(DateToStr(GetVal('FinSimul'))+' '+IntToStr(numSimul)+' '+DateToStr(thisDate)+' '+DateToStr(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis')));

    contextObjet.AfficheLesTEClasses();
    contextObjet.AfficheLesTEInstances();
  except
    AfficheMessageErreur('TSimule.InitYear | Numéro de simulation ' +
      inttostr(numSimul), UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//
/// Construit la liste des appels de procédures ainsi que la récupération
/// des paramètres nécessaires à chacun des appels de procédure ainsi créée.
{ TODO -oViB : Pourquoi SetList est rattaché à TSimule, il est mieux qu'il soit géré dasn TAppel }

procedure TSimule.SetList;
// Construction de la liste des appels (construit une fois pour toute). Cette liste ne contiendra pas les modules d'initialisation
var
  aCallProc: TAppel;
  j: Integer;


begin
  try
    // on ajoute l'appel au module 0, le module d'initialisation général
    DBModule1.Module.Locate('CodeModule', 0, []);
    aCallProc := TAppel.Create(DBModule1.Module.fieldbyname('CodeModule').Value,
      DBModule1.Module.fieldbyname('CodeEntity').Value, 0,
      DBModule1.Module.fieldbyname('NomModule').Value);
    TabAppel.ajouter(aCallProc);

    DBModule1.ModuleVar.First;
    for j := 0 to DBModule1.ModuleVar.RecordCount - 1 do
    begin
      aCallProc.DefArgument(DBModule1.ModuleVarOrdre.Value,
        TArgument.Create(DBModule1.EntiteNomEntite.Value,
        DBModule1.DictionnaireVarNOMCOURS.Value));
      aCallProc.DefParam(j, DBModule1.DictionnaireVarNOMCOURS.Value,
        conversion(DBModule1.ModuleVarTypeParam.Value));
      DBModule1.ModuleVar.Next;
    end;

    // ajout de tous les modules propres au modèle  { TODO -oViB : Comment être assuré qu'on a déjà positionné le pointeur de la table modèle sur le bon modèle pour que la relation maitre détail filtre les modules? }
    DBModule1.ModeleModule.First;

    // Lecture de toutes les lignes de la table DBModele.Module .
    // Pour chaque ligne :
    // - trouve dans DBModule.Module la ligne ayant le meme CodeModule
    // - cree l'instance TAppel correspondant
    // - recupere les variables necessaires dans DBModuleVar (table dynamique)
    while not dBModule1.ModeleModule.Eof do
    begin
      DBModule1.Module.Locate('CodeModule',
        DBModule1.ModeleModuleCodeModule.Value, []);

      aCallProc :=
        TAppel.Create(DBModule1.Module.fieldbyname('CodeModule').Value,
        DBModule1.Module.fieldbyname('CodeEntity').Value,
        DBModule1.ModeleModule.fieldbyname('Step').Value,
        DBModule1.Module.fieldbyname('NomModule').Value);

      tabAppel.ajouter(aCallProc);

      // Parcours des variables du module
      // (car action du DBModule1.Module.Locate précédent soit application de la
      // relation maitre détail entre ModuleVar et Module)
      DBModule1.ModuleVar.First;
      for j := 0 to DBModule1.ModuleVar.RecordCount - 1 do
      begin
        aCallProc.DefArgument(DBModule1.ModuleVarOrdre.Value,
          TArgument.Create(DBModule1.EntiteNomEntite.Value,
          DBModule1.DictionnaireVarNOMCOURS.Value));
        aCallProc.DefParam(j, DBModule1.DictionnaireVarNOMCOURS.Value,
          conversion(DBModule1.ModuleVarTypeParam.Value));
        DBModule1.ModuleVar.Next;
      end;

      DBModule1.ModeleModule.Next;
    end;

    //DBModule1.ModeleModule.Next;  { TODO -oViB : ké ké c'est que ça????????? }
  except
    AfficheMessageErreur('TSimule.SetList', UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//

//AD procedure TSimule.BilanParcelle(codeSimule : Integer);

procedure TSimule.BilanParcelle(idSimule: string);
// Gestion du déroullement de la simulation :
// - Création des différentes classe
// - Création de la liste des appels
// - Initialisation de la simulation (module 0)
// - lancement de la simulation pour chaque année
var
  i, j: Integer;


begin
  try
    CreationClasses; // Creation des classes (site, soil ...)
    //tabClass.CreationClasses;
    //SetTabAppels(idSimule,0);

    SetList; // Création de liste des Tappels

    //AssignFile(f,'D:\'+DBModule1.Simule.fieldbyname('Id').AsString+'0.txt');
    //Rewrite(f);
    //close(f);

    RunModule(0); // Initialisation de l'objet simule

    { TODO -oViB : ATTENTION: il existe un UpdateData (dans DBModule) de la table
    Simule qui met à 1 le nombre d'année si l'année de la date de fin de
    simulation est égal à l'anné de fin de simulation }
    j := Trunc(GetVal('NbAnSim')) - 1;

    //Assign(f,'D:\Init.txt');
    //Rewrite(f);
    //close(f);

    for i := 0 to j do // tant qu'on a pas atteint la derniere année
    begin
      Application.ProcessMessages;
      if mainForm.butStopSimulation.Caption <> 'Arrêt en cours' then
      begin
        //tabAppel.writeAppels(i);
        InitYear(i);
        //InitDates(i);
        //contextObjet.RunInitInstances;
        //SimulAn;
        numSimule2 := i + 1;
        Aller(i + 1);

        //VN save
        while ContextSave.Count <> 0 do //on nettois les sauvegardes
        begin
          TContextSave(ContextSave.Objects[0]).Destroy;
          ContextSave.Delete(0);
        end;
        while TabAppelSave.Count <> 0 do
        begin
          TTabAppelsSave(TabAppelSave.Objects[0]).Destroy;
          TabAppelSave.Delete(0);
        end;

        //Append(f);
        //WriteLn(f, '.');
        //WriteLn(f);
        //close(f);
    //contextObjet.EmptyContextInstance;
      end;
    end;
  except
    AfficheMessageErreur('TSimule.BilanParcelle | Simulation ' + (idSimule),
      UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//

procedure TSimule.SimulAn;
// Permet la Gestion de la date de semis
begin
  try
    if GetVal('TypeSemis') = 2 then
    begin
      {if not PlanteTypeArachide then
        begin
          TrouveSemis;
          ConfirmeSemis;
        end
      else
        begin
          TrouveDateSemisSurStock(DBModule1.SiteSeuilPluie.Value);
          aSoil.InitDB(DBModule1.SimuleCodeParcelle.Value);
        end;
      aPlot.InitDB(DBModule1.SimuleCodeParcelle.Value);
      aCrop.Harvest;
      aSimule.myModele.SetList;}
    end;
  except
    AfficheMessageErreur('TSimule.SimulAn', UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//

procedure TSimule.ConfirmeSemis;
// Gestion de la date de semis
begin
  try
    {SetVal('Semis',thisDate);
    If (GetVal('Semis')+ aPlante.TabState[cyclePot]
              > aSimule.TabState[DateFinSimul])
    then aSimule.TabState[Semis]:=
         aSimule.TabState[DateFinSimul] -aPlante.TabState[cyclePot]; }
  except
    AfficheMessageErreur('TSimule.ConfirmeSemis', UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//

procedure TSimule.RunModule(numModule: Integer);
// Procedure d'appel d'un module avec son numéro et le contexte associé à la simulation
begin
  try
    tabAppel.Executer(numModule, contextObjet);
  except
    AfficheMessageErreur('TSimule.RunModule | Module ' +
      VarToStr(dBModule1.Module.Lookup('CodeModule', numModule, 'NomCours')) + ' ('
      + inttostr(numModule) + ')', UClasseSimule);
    //on E: Exception do AfficheMessageErreur(E.message+'TSimule.RunModule | Module '+VarToStr(dBModule1.Module.Lookup('CodeModule',numModule,'NomCours'))+' ('+inttostr(numModule)+')',UClasseSimule);

  end;
end;

//----------------------------------------------------------------------//
// Execute le module si la date d'execution correspond à la date en
// cours
//----------------------------------------------------------------------//

procedure TSimule.StepExe;
var
  i: Integer;



begin
  try
    for i := low(tabAppel.listAppel) to high(tabAppel.listAppel) do
    begin
      //ShowMessage(DateToStr(tabAppel.listAppel[i].nextExe)+' '+DateToStr(thisDate));
      if tabAppel.listAppel[i].nextExe = thisDate then
        // si la date correspond à la date en cours alors on appelle le module
      begin
        //VN test
          //if tabAppel.listAppel[i].stepExe=0 then
          //begin
           //Assign(f,'D:\test.txt');
            //Append(f);
            //WriteLn(f, inttostr(tabAppel.listAppel[i].id)+#9+datetostr(tabAppel.listAppel[i].nextExe)+#9+datetostr(thisDate)+#9+tabAppel.listAppel[i].classe.name+#9+dbmodule1.module.lookup('CodeModule',tabAppel.listAppel[i].id,'NomCours'));
            //close(f);
          //end;
          //show:=(thisDate=EncodeDate(1950,02,14));
          //if (tabAppel.listAppel[i].id=33229)and (thisDate=EncodeDate(1950,02,14))
          //then ShowMessage('##########'+inttostr(tabAppel.listAppel[i].id)+' '+DateToStr(contextObjet.getcurrentInstance('Simulation').GetVal('DATEENCOURS')));
          //ShowMessage(DateToStr(thisdate)+' '+IntToStr(i)+'/'+IntToStr(high(tabAppel.listAppel)));
        runModule(tabAppel.listAppel[i].id);
        //if (tabAppel.listAppel[i].id=33229) and (thisDate=EncodeDate(1997,09,15))
        //if show
        //then ShowMessage(inttostr(tabAppel.listAppel[i].id)+' '+DateToStr(contextObjet.getcurrentInstance('Simulation').GetVal('DATEENCOURS'))+' '+DateToStr(thisdate));

      end;
    end;
  except
    AfficheMessageErreur('TSimule.StepExe', UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//

procedure TSimule.CalculNextExe;
// Calcule la date suivante et met à jour le tableau des appels.
// Pour chaque module dont la date d'execution correspond à la date en
// cours, on fait passer la date d'execution au prochain pas de temps
// Ex: pour des modules mensuels, le 31/03 donne 30/04
var
  i: Integer;
  step: Real;
  DateExe: TdateTime;
  Year, Month, Day: Word;

begin
  try
    for i := 0 to length(tabAppel.listAppel) - 1 do
    begin
      // si la date correspond à la date en cours alors on calcule la prochaine date d'execution en fonction du step
      if tabAppel.listAppel[i].nextExe = thisDate then
      begin
        DateExe := thisDate; //Edit VN debug
        step := tabAppel.listAppel[i].stepExe;
        if (Step = 1) then //  DateExe := IncDay(DateExe,trunc(Step));
          DateExe := IncrementeDate(tabAppel.listAppel[i].nextExe, step,
            calendaire);
        //if (step >1) and (step <> 0) then  //
        if step > 1 then // ViB le 27/02/2004
        begin { TODO : Fait péter toute gestion d'un pas différent de mensuel, revoir la condition pour le 5, 10 et 15 jours) }
          //if (DayOfTheMonth(IncDay(thisDate,Trunc(Step))) > 27) then  // ViB le 01/03/2004
          begin
            if ModeDebug then
              mainForm.affiche(TimeToStr(Time) + #9 +
                '####  ---> Date: ' + datetostr(thisDate) + ' pas: ' +
                floattostr(step),0);
            //DecodeDate(EndOfTheMonth(IncDay(thisDate,Trunc(Step))),Year,Month,Day); // ViB 26/02/2004
            DecodeDate(EndOfTheMonth(thisDate), Year, Month, Day);
            Inc(Month); // ViB le 01/03/2004
            if Month = 13 then // ViB le 01/03/2004
            begin // ViB le 01/03/2004
              Month := 1; // ViB le 01/03/2004
              Inc(Year); // ViB le 01/03/2004
            end; // ViB le 01/03/2004
            Day := DayOfTheMonth(EndOfAMonth(Year, Month)); // ViB le 01/03/2004
            DateExe := EncodeDate(Year, Month, Day);
            //if modeDebugPrecis then mainForm.memDeroulement.Lines.add(tabAppel.listAppel[i].theProc.name+ '--> prochaine exécution: '+datetostr(DateExe)); // ViB le 01/03/2004
          end;
          //          else
          {             DateExe := IncDay(DateExe,trunc(Step));
                      DecodeDate(DateExe,Year,Month,Day);
                      Day := Trunc(DayOfTheMonth(DateExe)+step-1);
                      If Day > 27 then Day := DayOfTheMonth(EndOfAMonth(Year,Month));
                      DateExe := EncodeDate(Year,Month,Day);    }
        end; // fin if (step >1) and (step <> 0) then
        tabAppel.listAppel[i].nextExe := DateExe;

        {if step=0 then   //VN debug simcons
          begin
            Assign(f,'D:\init next exe.txt');
            Append(f);
            WriteLn(f, inttostr(tabAppel.listAppel[i].id)+#9+datetostr(tabAppel.listAppel[i].nextExe)+#9+datetostr(thisDate)+#9+tabAppel.listAppel[i].classe.name+#9+dbmodule1.module.lookup('CodeModule',tabAppel.listAppel[i].id,'NomCours'));
            close(f);
          end;}

      end // fin if tabAppel.listAppel[i].nextExe = thisDate then
        //else
          //if tabAppel.listAppel[i].id=33226 then ShowMessage(dateToStr(tabAppel.listAppel[i].nextExe)+' '+dateToStr(thisDate));
    end; // fin for i:= 0 to length(tabAppel.listAppel)-1 do
  except
    AfficheMessageErreur('TSimule.CalculNextExe', UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//

procedure TSimule.Aller(const numSimule: integer);
// Gère le déroullement de la simulation :
// On récupère d'abord le plus petit pas de temps de tous les modules
// Ensuite tant qu'on a pas atteint la date de fin de simulation :
//  - on execute les modules
//  - on change la date de la prochaine execution
//  - on incremente la date en cours avec le plus petit pas de temps
//  - on fait passer les bookmarks sur l'enregistrement suivant

var
  dateFin, semis: TDateTime;
  i: Integer;

  test: Double;
  jour, mois, an, anPrec: Word;


begin
  try
    // recuperation de la date de fin de la simulation
    dateFin := GetVal('FinSimul');
    //VN déplacé dans la boucle
    //semis   := GetVal('DateSemisCalc');
    anPrec := 0;

    // requete qui renvoie le plus petit pas de temps
    DBModule1.RequeteStep.Active := true;
    DBModule1.RequeteStep.First;
    minorStep := DBModule1.RequeteStepMINOFstep.Value;
    DBModule1.RequeteStep.Active := false;

    //ShowMessage(floattostr(contextObjet.GetCurrentInstance('Soil').GetVal('ArriveeEauJour')));
    //ShowMessage('###'+IntToStr(tabAppel.listappel[0].id)+' '+IntToStr(tabAppel.listappel[0].stepExe)+' '+dateToStr(tabAppel.listappel[0].nextExe)+' '+DateToStr(thisDate));

    // boucle tant que la date courante thisDate n'a pas atteind la date de fin
    while thisDate <= dateFin do
    begin
      DecodeDate(thisDate, an, mois, jour);
      //VN contraintes
      Pars.SetVariable('Dateencours', thisDate);
      Pars.SetVariable('DebutSimul', thisdate);
      Pars.SetVariable('finsimul', dateFin);
      semis := GetVal('DateSemisCalc');
      //VN contraintes

      //if contextObjet.GetCurrentInstance('Soil').GetVal('ArriveeEauJour') = 0 then ShowMessage('zero');
      if anPrec <> an then
        // si l'annee precedente est differente de l'annee courante on l'initialise comme etant égal a l'annee courante
      begin
        anPrec := an;
        mainForm.Label2.Caption := FormatDateTime('hh:mm:ss', Time - Duree) +
          ' - Num. simulation: ' + IntToStr(NumSimulation) + ' - Simulation: ' +
          DBModule1.Simule.findfield('Id').AsString + #9 + ' - Année: ' +
          IntToStr(an);
      end;
      //VN saveSemis
      //if  (thisDate = semis) then
      if (contextObjet.GetCurrentInstance('Crop') = nil) and (thisDate = semis)
        then // Création de crop si on est arrivé à la date de semis on place l'instance de Crop dans le context
      begin
        //ShowMessage(DateToStr(GetVal('DateSemisCalc')));
        contextObjet.SetCurrentInstance(TEntityInstance.Create('Crop', thisDate,
          dbmodule1.Variete.findfield('id').AsVariant));
      end;

      // S'il existe une instance de type Crop <=> a partir du moment ou il y a eu le semis
      if not (contextObjet.GetCurrentInstance('Crop') = nil) then
      begin
        test := contextObjet.GetCurrentInstance('Crop').GetVal('NumPhase');
        //dateEnStr := FormatDateTime('dd:mm',contextObjet.GetCurrentInstance('Crop').GetVal('DateMaturite'));

        // Si le numero de la phase de l'instance de Crop est 7
        // Destruction de l'entité Crop car elle est a Maturité on place l'instance de Crop dans le context
        if contextObjet.GetCurrentInstance('Crop').GetVal('NumPhase') = 7 then
          contextObjet.FreeEntityInstance('Crop');
      end; // fin : test sur existence d'une instance de Crop

      //Ajout VN simcons
      contextObjet.RunInitInstances;
        //Lance les modules d'initialisation si il faut le faire.

      //tabAppel.writeAppels(0);
       {Assign(f,'D:\Suivi.txt');
       Append(f);
       acall:=tabAppel.listappel[5];
       WriteLn(f, inttostr(acall.stepExe)+#9+inttostr(acall.id)+#9+datetostr(acall.nextExe)+#9+datetostr(TSimule(contextObjet.GetCurrentInstance('Simulation')).thisDate)+#9+acall.classe.name+#9+dbmodule1.module.lookup('CodeModule',acall.id,'NomCours'));
       close(f);}

     //ShowMessage(DBModule1.Resjour.FieldByName('ArriveeEauJour_out').AsString+' '+floattostr(contextObjet.GetCurrentInstance('Soil').GetVal('ArriveeEauJour'))+' '+floattostr(contextObjet.GetCurrentInstance('Plot').GetVal('ArriveeEauJour_out')));

      StepExe; // parcours du tableau et execution des modules selon leur pas de temps
      CalculNextExe; // parcours du tableau et mise à jours de la prochaine date d'execution des modules

      // Passage à l'enregistrement suivant A modifier pour adapter au pas de temps
      if DBModule1.Forcage.RecordCount <> 0 then
      begin
        for i := 0 to length(DBModule1.DynamicTables) - 1 do
          DBModule1.DynamicTables[i].Next;
      end;

      thisDate := thisDate + MinorStep; // passage au pas de temps suivant

      //VN simcons (permet la manipulation de la fin de simul dans les modules)
      DateFin := GetVal('FinSimul');

      if thisDate <> DateFin then
      begin
        //if (thisDate - Semis>0) and (thisDate - Semis<10) then ShowMessage(DateToStr(thisDate)+' '+DateToStr(semis));
        SetVal('DateEnCours', thisDate);
        //ShowMessage(DateToStr(thisDate));
        MAJBookmarks(numSimule); // Mise à jour des TAccessRecords
        SetVal('NbJAS', thisDate - Semis);
      end;
      //ShowMessage(DateToStr(thisDate)+' '+DateToStr(GetVal('DateEnCours')));
      Application.ProcessMessages;
      if mainForm.butStopSimulation.Caption = 'Arrêt en cours' then
        thisDate := DateFin + 1;
    end; // fin du while thisDate <= dateFin do

    //VN save: Dans le cas de semis calculé on remet à jour la variable nbjas dans resjour
    // Finaliser le choix forcer ou calculer une date de semis....
// Modif CB 02/06 ne fiare MajNbjAs que si date semis calculee
    if contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis') = nullValue
      then
      // Modif CB 02/06
      majNbJas(GetVal('DateSemisCalc'), getval('DebutSimul'), dateFin - 1);
  except
    on e: Exception do
      AfficheMessageErreur(e.message + 'TSimule.Aller', UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//

procedure TSimule.MajBookmarks(const numSimule: integer);
// Mise à jour des bookmark pour chaque table si la date correspond :
// - si la table est en read only : on passe à l'enregistrement suivant et on met à jour le bookmark
// - si la table n'est pas en read only : on recopie le dernier enregistrement et on met à jour le bookmark
// Enfin, on met à jour la date du prochain changement
var
  i, j: Integer;
  jour, mois, anprec, an: Word;
  DateRel: TDateTime;
begin
  { TODO : Il faut faire un test d'existance d'enregistrement lors du chgt de bookmark
  s'il n'y a pas d'enregistrement pour cette date cas de l'irrigation
  on filtre la table et par defaut elle pointe sur une enregistrement vide
  ce qui n'est pas le cas du Locate.... }
  try
    // boucle sur la liste des TEntityInstance presente dans contextObjet
    //ShowMessage(DateToStr(thisdate));
    for i := 0 to length(contextObjet.currentInstances) - 1 do
    begin
      if contextObjet.currentInstances[i] <> nil then
        // si l'instance n'est pas à nil (crop créée plus tard)
      begin
        for j := 0 to length(contextObjet.currentInstances[i].InOutTables) - 1 do
          // parcours des tables utilisées par les paramètres des champs
        begin
          //ShowMessage(contextObjet.currentInstances[i].InOutTables[j].myTable.TableName);
          with (contextObjet.currentInstances[i].InOutTables[j]) do
          begin
            //If MyStep=-2 then ShowMessage(DateToStr(nextDate)+' '+DateToStr(thisDate));
            if (myStep <> 0) and (NextDate = thisDate) then
              // si la date en cours correspond a la date de lecture dans la bdd et qu'on travaille bien sur une table que l'on doit lire à chaque pas de simulation (car dans les autres cas, Step=0 pour Espèce, Variété, Station...)
            begin
              if ReadOnly then
                // si la table est en mode ReadOnly (on est danc dans le cas de la climato, l'irrigation, ObsParcelle...)
              begin
                // le pas n'est pas continu et c'est la date du champ "Jour" qui devient la référence (donc forcément il existe un champ "Jour")

                //Edit VN
                {if MyTable.FieldByName('Jour').AsDateTime = Thisdate then

                    NextDate := IncrementeDate(NextDate,MinorStep,true) { TODO : Revoir le calendaire, tjs forcé à vrai!
                  else
                    begin
                    myTable.Next;
                    If (myStep=-2) and (myTable.RecNo<>myTable.RecordCount) then   //VN v2.0
                    begin
                      myTable.Next;
                      nextDate := MyTable.FieldByName('Jour').AsDateTime;
                      myTable.Prior;
                    end
                    else nextDate := MyTable.FieldByName('Jour').AsDateTime;
                    //MyTable.FieldByName('Jour').AsDateTime;
                    myBookMark := myTable.GetBookmark;
                    end;}
                
                if MyStep < 0 then
                begin

                  //VN debug  ajout du if, on est jamais trop prudent
                  if myStep = -1 then
                    myTable.Filtered := false;
                  myTable.GotoBookmark(myBookMark);

                  //If (myStep=-2) then ShowMessage(DateToStr(MyTable.FieldByName('Jour').AsDateTime));

                  if (mystep = -1) then
                  begin
                    if (MyTable.FieldByName('Jour').AsDateTime = Thisdate) then
                      NextDate := IncrementeDate(NextDate, MinorStep, true)
                        { TODO : Revoir le calendaire, tjs forcé à vrai! }
                    else
                    begin
                      myTable.Next;
                      nextDate := MyTable.FieldByName('Jour').AsDateTime;
                      //MyTable.FieldByName('Jour').AsDateTime;
                      //myTable.FreeBookmark(myBookMark);
                      myBookMark := myTable.GetBookmark;
                    end;
                  end
                  else if (myStep = -2) then //VN Table
                  begin
                    myTable.Next;
                    //on passe à l'enregistrement suivant (qui normalement est le bon)

                    if myTable.Eof then
                      myTable.First;
                    //si c'est la dernière date de la table, on fait boucler.
                    //myTable.FreeBookmark(myBookMark);

                    myBookMark := myTable.GetBookmark;
                    //On mémorise le nouvel enregistrement.

                    //........................................................................
                    //on va alors chercher la date de prochaine execution
                    anprec := yearof(MyTable.FieldByName('Jour').AsDateTime);
                    myTable.Next;
                    if myTable.Eof then
                      myTable.First;

                    DecodeDate(MyTable.FieldByName('Jour').AsDateTime, an, mois,
                      jour);
                    DateRel := EncodeDate(yearof(Thisdate) + an - anprec, mois,
                      jour);
                    //on récupere la date relative
                    //ShowMessage(DateToStr(MyTable.FieldByName('Jour').AsDateTime)+' '+DateToStr(DateRel)+' '+DateToStr(thisdate));

                    nextDate := DateRel;
                    //on met à jour la date de prochaine mise a jour

                    myTable.GotoBookmark(myBookMark);
                    //puis on retourne au bon enregistrement
                  end; //if mystep=-2

                  if myTable.Name = 'Irrigation' then
                    myTable.Filter := 'Id=''' +
                      dBModule1.ItineraireTechnique.fieldByName('IdIrrigation').Value + ''' and Jour=' + QuotedStr(DateToStr(thisDate));
                  //AD                else myTable.Filter :='Id='''+dBModule1.Simule.fieldByName('IdDonnesObs').Value+''' and Jour='+QuotedStr(DateToStr(thisDate));

                                  //AD myTable.Filter := Jour = ' + QuotedStr(DateToStr(thisDate));
                  if MyStep = -1 then
                    myTable.Filtered := True; //VN table
                end
                else // le pas est constant (MyStep <> -1) { TODO : D'autres cas existent.... step=0... quelle conséquence même s'il on ne doit jamais se trouver dans ce cas... }
                begin // on passe simplement à l'enregistrement suivant (espérons donc que les enregistrements sont continu)
                  // temp: modifs annulées pour tester la dérivation vers les 2 tempPluvio et tempMeteo
                  //[AVANT MODIF] // mis en comentaire ViB 10/01/2008 on ne veut pas de next ssi on est sur Pluviométrie (pb de ralentissement lorsqu'on dérive la climato)
                  myTable.next; // Test de correspondance des dates
                  //[APRES MODIF]
                  //if mytable.Name<>'Climatologie' then myTable.next;   // ajout ViB 10/01/2008
                  //[FIN MODIF]
                  if (myTable.FieldByName('Jour').AsDateTime = thisDate) then
                  begin
                    //                  myTable.FreeBookmark(myBookMark);
                    
                    myBookMark := myTable.GetBookmark;
                  end
                  else
                  begin { TODO -cb: FINALISER test données absentes et quel contrôles ??? }
                    // faire un locate et si cela ne marche pas on pointe sur Nil
                    MainForm.affiche(TimeToStr(Time) + #9 +
                      '##---' + DateToStr(myTable.FieldByName('Jour').AsDateTime) +
                      ' ' + DateToStr(thisDate) + ' PAS DE DONNEES POUR la table '
                      + myTable.name + ' pour le ' + DateTimeToStr(thisDate),0);
                      Main.erreur := true;
                    // MyBookMark := Nil;  // voir a utiliser en test { TODO -oViB: ah oui...? et quelle conséquences si on fait ça (nil) ??? }
                  end;

                  NextDate := IncrementeDate(NextDate, MyStep, true);
                    { TODO : Revoir le calendaire, forcé à vrai! }
                end; // If MyStep = -1
              end //if ReadOnly
              else // La table MyTable n'est pas en lecture seule (ReadOnly=false) donc c'est ResJour, ResCum etc...
              begin
                // ##### On recopie le dernier enregistrement de MyTable en incrementant l'index (forcement une date car ReadOnly=faux et donc il s'agit de ResJour/ResCum... fin non  :-( )
                MyTable.gotoBookmark(MyBookMark);
                RecopieEnreg(MyTable, thisDate, myStep, numsimulation);
                  // voir apres comment gerer le pb la on fait comme pour la sensibilite Resjour dans DBAnalyse peut pas rester ainsi cause 20000 itereations!!!!
                //              myTable.FreeBookmark(myBookMark);
                myBookMark := myTable.GetBookmark;
                  // nécessaire car on a forcement bougé d'enregistrement dans RecopieEnreg vu qu'on vient de créer un nouvel enregistrement
                NextDate := IncrementeDate(NextDate, MyStep, true);
                  { TODO : Revoir le calendaire, tjsforcé à vrai! }
              end; // calcul de la prochaine date de lecture/ecriture dans la bdd  améliorer IncrementeDate
            end; // if (Mystep <> 0) and (NextDate = thisDate)
          end; // With (contextObjet.currentInstances[i].InOutTables[j] do
        end; // for j:= 0 to length(contextObjet.currentInstances[i].InOutTables)-1 do
      end; //if contextObjet.currentInstances[i] <> nil then
    end; //For i := 0 to length(contextObjet.currentInstances)-1 do
  except
    on E: Exception do
      AfficheMessageErreur(E.message + ' TSimule.MajBookmarks', UClasseSimule);
    //AfficheMessageErreur('TSimule.MajBookmarks',UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//

function TSimule.conversion(x: string): TEnumTypeParam;
  { TODO -oViB : Pourquoi est-ce un méthode de TSimule???? }
// Renvoie le type du parametre
begin
  try
    if lowercase(x) = 'in' then
      result := KIn;
    if lowercase(x) = 'out' then
      result := KOut;
    if lowercase(x) = 'inout' then
      result := KInOut;
  except
    AfficheMessageErreur('TSimule.conversion | Type de paramètre ' + x,
      UClasseSimule);
  end;
end;

//----------------------------------------------------------------------------//
//Ajout VN contraintes

procedure TSimule.MajInVar_ToPars(InIndice: array of Shortint; TabParam: array of
  ModelsManage.TParam; ParamVal: ModelsManage.TPointeurProcParam);
var
  i: integer;
begin
  for i := 0 to high(InIndice) do
  begin
    //ShowMessage(TabParam[InIndice[i]].Name+' '+FloatToStr(ParamVal[InIndice[i]]));
    pars.SetVariable(TabParam[InIndice[i]].getName, ParamVal[InIndice[i]]);
  end;
end;

procedure TSimule.MajOutVar_ToProcParam(OutIndice: array of Shortint; TabParam:
  array of ModelsManage.TParam; var ParamVal: ModelsManage.TPointeurProcParam);
var
  i: integer;
begin
  for i := 0 to high(OutIndice) do
  begin
    ParamVal[OutIndice[i]] := pars.GetVariable(TabParam[OutIndice[i]].getName);
  end;
end;

procedure TSimule.majNbJas(dateSemis, dateDebut, datefin: TDateTime);

  //Met à jour la variable nbJas pour l'années simulée
  //Utile surtout aprés un calcul automatique de la date de semis
begin
  try
    DBModule1.queGenerique.Close;
    DBModule1.queGenerique.SQL.Clear;
    DBModule1.queGenerique.SQL.Add('Update '':DBResult:Resjour.db'' Set nbJas = jour-''' + FormatDateTime('mm/dd/yyyy', dateSemis) + ''' where jour>=''' +
      FormatDateTime('mm/dd/yyyy', dateDebut) + '''and jour<=''' +
      FormatDateTime('mm/dd/yyyy', dateFin) + ''';');
    DBModule1.queGenerique.ExecSQL;
  except
    on e: exception do
      AfficheMessageErreur('setNbJas ' + e.Message, UClasseSimule);
  end
end;

//Fin Ajout VN
//-------------------------------------------------------------------------//
// Cette procédure va permettre d'appeler toute la chaîne d'initialisation
// des variables se rapportant à l'objet simulation
//-------------------------------------------------------------------------//

procedure InitializationDyn(var tabParam: TPointeurProcParam);
begin
  try
    { TODO -ofred : trouver un moyen pour enlever aSimulation dans aSimulation.InitSimulation }
    aSimulation.InitSimulation(tabParam[0], tabParam[1], tabParam[2],
      tabParam[3], tabParam[4], tabParam[5], tabParam[6],
      tabParam[7], tabParam[8], tabParam[9], tabParam[10], tabParam[11]);
  except
    AfficheMessageErreur('InitializationDyn', UClasseScenario);
  end;
end;

//Ajout VN contraintes
//Charge les contraintes dans TabProc

procedure InitProcContrainte();
var


  req: TQuery;
begin
  try

    req := TQuery.Create(nil);
    req.Active := false;
    req.SQL.Clear;
    req.SQL.add('Select m.nomCours,m.NomModule,m.librairie');
    req.SQL.add('from '':DBModele:Module.db'' m');
    req.SQL.add('where m.Librairie not in (select NomUnite from '':DBModele:ListeLibrairie.db'');');
    req.Active := true;

    req.First;

    while not req.Eof do
    begin
      tabProc.AjoutProcContrainte(req.findfield('NomModule').AsString,
        LoadConstraintFromFile(req.findfield('NomCours').AsString,
        req.findfield('librairie').AsString));
      req.Next;
    end;
    req.Destroy;

    Clean;
    //nettoie les variable utilisés par l'unité contrainte.

  except
    AfficheMessageErreur('InitProcContrainte', UClasseSimule);
  end;
end;

procedure SaveSimulation(var DoSave: Double; const CodeSave: Double);
var
  index: integer;


begin
  try
    if DoSave = 1 then
    begin
      //ShowMessage('save '+DateToStr(CodeSave)+' '+DateToStr(aSimulation.thisDate));
      DoSave := 0;
      aSimulation.SetVal('DoSave', 0);
      //Avant de sauvegarder on remet la demande de sauvegarde  à 0

      index := aSimulation.ContextSave.IndexOf(FloatToStr(CodeSave));
      if index >= 0 then
      begin
        TContextSave(aSimulation.ContextSave.objects[index]).Free;
        aSimulation.ContextSave.Delete(index);
        TTabAppelsSave(aSimulation.TabAppelSave.objects[index]).Free;
        aSimulation.TabAppelSave.Delete(index);
      end;
      //On efface une éventuelle sauvegarde qui aurait le même code

      aSimulation.ContextSave.AddObject(FloatToStr(CodeSave),
        TContextSave.Create(contextObjet));
      aSimulation.TabAppelSave.AddObject(FloatToStr(CodeSave),
        TTabAppelsSave.Create(tabAppel));
      //Création des sauvegardes à partir des objets à sauvegarder
    end;
  except
    on E: Exception do
      AfficheMessageErreur('SaveSimulation ' + E.Message, UClasseSimule);
  end;
end;

procedure LoadSimulation(var DoLoad, nbLoaded: Double; const CodeSave: Double);
var
  index: integer;
  saveCtxte: TContextSave;
  saveTabappel: TTabAppelsSave;
begin
  try
    if DoLoad = 1 then
    begin
      //ShowMessage('load '+DateToStr(CodeSave)+' '+DateToStr(aSimulation.thisDate));
      index := aSimulation.ContextSave.IndexOf(FloatToStr(CodeSave));
      if index >= 0 then
      begin
        //ShowMessage(DateToStr(aSimulation.thisdate));
        saveCtxte := TContextSave(aSimulation.ContextSave.objects[index]);
        //on récupere la sauvegarde du Tcontext qui correspond au code de sauvegarde

        saveCtxte.UseSave(contextObjet);
        //on utilise cette sauvegarde pour écrase les valeurs du contexte en cours

        saveTabappel := TTabAppelsSave(aSimulation.TabAppelSave.objects[index]);
        //on récupere la sauvegarde du TtabAppel qui correspond au code de sauvegarde

        saveTabappel.UseSave(tabAppel);
        //on utilise cette sauvegarde pour écrase les valeurs du contexte en cours

        aSimulation.thisDate := aSimulation.GetVal('DATEENCOURS');
        //on met à jour la variable indépendant des TentityInstance sujette à modification

        aSimulation.Pars.SetVariable('DATEENCOURS',
          aSimulation.GetVal('DATEENCOURS'));
        //maj de la date en cours du parser

        nbLoaded := nbLoaded + 1;
        //on incrémente le nombre de chargement effectué
        //ShowMessage(DateToStr(aSimulation.thisdate));
      end;
      //else AfficheMessageErreur('LoadSimulation | sauvegarde '+ FloatToStr(CodeSave) +'inexistante',UClasseSimule);

      DoLoad := 0;
      //on efface la demande de chargement pour ne pas recharger

    end
    else
      nbLoaded := 0;
  except
    on E: Exception do
      AfficheMessageErreur('LoadSimulation ' + e.Message, UClasseSimule);
  end;
end;

//calcule la date de semis et déclanche la sauvegarde ou le chargement

procedure CalcSemisAuto(var nbJStress, DoLoad, Dosave, DateSemisCalc: Double;
  const nbJTestSemis, StressResistance, seuilStress, Cstr, DateEnCours,
  SeuilEauSemis, eauDispo: Double);
begin
  try
    //Si la plante n'existe pas et que le seuil de pluie est atteint
    //alors on lance la sauvegarde et on initialise la date de semis
    // Nota permet
    // des semis consécutifs sur une longue saison pluvieuse cas Benin cote d'ivoire

    if (contextObjet.GetCurrentInstance('Crop') = nil) then
    begin

      if (eauDispo > SeuilEauSemis) then
      begin
        DoSave := 1;
        DateSemisCalc := DateEnCours;
        nbJStress := 0;
      end;
    end
    else
    begin
      if (DateEnCours < DateSemisCalc + nbJTestSemis) then
      begin
        if (Cstr < seuilStress) then
          nbJStress := nbJStress + 1;
        if nbJStress > StressResistance then
        begin
          DoLoad := 1;
        end;
      end
        // A resoudre MAjNbJAS  si test Ok... Fait dans proc TSimule.Aller
        //    if (DateEnCours=DateSemisCalc+nbJTestSemis) then majNbJas(DateEnCours);
    //Si on a trouvé la bonne date de semis on met à jour les nbjas
    end;

  except
    on E: Exception do
      AfficheMessageErreur('CalcSemisAuto ' + e.Message, UClasseSimule);
  end;
end;

//seme si la date de Semis est égale à la date courante

procedure Seme(const DateSemisCalc, DateEnCours: Double);
begin
  try
    if DateSemisCalc = DateEnCours then
      //si on est à une date de semis alors on seme
    begin
      contextObjet.SetCurrentInstance(TEntityInstance.Create('Crop',
        DateEnCours, dbmodule1.Variete.findfield('id').AsVariant));
      //showMessage(DateToStr(DateSemisCalc));
    end;
  except
    on E: Exception do
      AfficheMessageErreur('Seme ' + e.Message, UClasseSimule);
  end;
end;

procedure SemeDyn(var tabParam: TPointeurProcParam);
begin
  Seme(tabParam[0], tabParam[1]);
end;

procedure CalcSemisAutoDyn(var tabParam: TPointeurProcParam);
begin
  CalcSemisAuto(tabParam[0], tabParam[1], tabParam[2], tabParam[3], tabParam[4],
    tabParam[5], tabParam[6], tabParam[7], tabParam[8], tabParam[9],
    tabParam[10]);
end;

procedure LoadSimulationDyn(var tabParam: TPointeurProcParam);
begin
  LoadSimulation(tabParam[0], tabParam[1], tabParam[2]);
end;

procedure SaveSimulationDyn(var tabParam: TPointeurProcParam);
begin
  SaveSimulation(tabParam[0], tabParam[1]);
end;
//Fin Ajout VN
//-------------------------------------------------------------------------//
// chargement de la procédure d'initialisation
//-------------------------------------------------------------------------//

initialization
  {AjoutVN}InitProcContrainte();
  tabProc.AjoutProc('Initialization', InitializationDyn);
  {AjoutVN}tabProc.AjoutProc('SaveSimulation', SaveSimulationDyn);
  {AjoutVN}tabProc.AjoutProc('LoadSimulation', LoadSimulationDyn);
  {AjoutVN}tabProc.AjoutProc('CalcSemisAuto', CalcSemisAutoDyn);
  {AjoutVN}tabProc.AjoutProc('Seme', SemeDyn);

end.

