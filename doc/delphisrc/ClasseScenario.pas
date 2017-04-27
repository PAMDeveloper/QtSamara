{*------------------------------------------------------------------------------
  Classe Scenario
  Cette unité permet de construire un objet scénario. Cette objet a pour fonction
  de gérer tout le déroulement d'une ou plusieurs simulations. Il va permettre de
  gérer la création de toutes les variables utilisées ainsi que le forçage des
  valeurs d'entrées.
  Date de commencement          : avant 01/02/04 !
  Date de derniere modification : 08/03/04 ,27/06/2011

  Etat : fini, teste
  @Author  CIRAD
  @Version v_3.0
-------------------------------------------------------------------------------}

//{$DEFINE AffichageComplet}

unit ClasseScenario;

interface

uses Windows, SysUtils, StrUtils, DB, Dialogs , ClasseSimule,
Variants,HashMapTable;

type
  {*Concept : caractérise un ensemble de simulation.
    Description : Un scénario est une suite de simulation.
    Cet objet permet donc de gérer la façon dont doivent être
    exécutées les simulations du scénario.*}
  TScenario = class
    tabSensibilite: array of array of real; ///sert à stocker les valeurs des attributs pour l'étude de sensibilité ,tableau créée par la méthode RemplirTableauSensibilite.

    {*--------------------------------------------------------------------------
      Procédure permettant d'exécuter les simulations du scénario suivant un
      type d'exécution (pré-positionne également les pointeurs sur les
      tables utilisées dans les simulations)
      @Author  CIRAD
      @Version v_3.0
      @param  typeExe 0 = une exécution normale, sans forçage 1= correspond à
      une exécution avec forçage 2=correspond à une exécution avec etude de
      sensibilité  3=correspond à une exécution avec optimisation de paramètres
    ---------------------------------------------------------------------------}
    procedure ExecuterScenario(typeExe: Integer);


    {*--------------------------------------------------------------------------
      Procédure appelée par ExecuterScenario, lancera les simulations pour
      l'optimisation, le forçage et l'exécution standard.
      Execution d'une simulation :
      On créé d'abord la classe simulation puis on créé l'objet simulation,
      on lance ensuite sa procédure bilanparcelle
      @Author  CIRAD
      @Version v_3.0
      @param  idSimule  id de la simulation
      @param  numSimule numéro de la simulation
    ---------------------------------------------------------------------------}
    procedure ExecuterSimulation(idSimule: string; numSimule: Integer);

    {*--------------------------------------------------------------------------
      Procédure appelée par ExecuterScenario, pour lancer les simulations
      de type étude de sensibilité.
      Gestion du lancement des simulations de type étude de sensibilité
      Cette procédure est une procédure récursive, les valeurs à incrémenter
      sont placées dans un tableau, la simulation est alors lancée pour
      chaque suite de valeurs possibles.

      @Author  CIRAD
      @Version v_3.0
      @param  indiceTab indice
    ---------------------------------------------------------------------------}
    procedure Sensibilite(indiceTab: Shortint);

    {*--------------------------------------------------------------------------
      Procédure permettant de créer et remplir du tableau des
      valeurs de sensibilité

      @Author  CIRAD
      @Version v_3.0
    ---------------------------------------------------------------------------}
    procedure RemplirTableauSensibilite;

    {*--------------------------------------------------------------------------
      Procédure permettant de mettre à jour la table forçage en
      fonction du tableau des valeurs de sensibilité.

      @Author  CIRAD
      @Version v_3.0
    ---------------------------------------------------------------------------}
    procedure MAJTableForcage;

    {*--------------------------------------------------------------------------
      Procédure permettant de lancer des simulations consecutives.

      @Author  CIRAD
      @Version v_3.0
      @param  idSimule  id de la simulation
      @param  numSimule numéro de la simulation
    ---------------------------------------------------------------------------}
    procedure LanceSimulationsConsecutives(idSimule: string; numSimule:
      Integer);

    {*--------------------------------------------------------------------------
      Procédure execSimule

      @Author  CIRAD
      @Version v_3.0
      @param  idSimule  id de la simulation
      @param  numSimule numéro de la simulation
      @param  NumAnnSimule numéro de l'année simulée
    ---------------------------------------------------------------------------}
    procedure execSimul(const idSimule: string; numSimule: Integer;
      NumAnnSimule: Integer);
      
  end;

var
  
  
  aSimulation: TSimule; /// Une Simulation
  finLastSimul: TDateTime; /// Date de fin de simulation

implementation

uses ModelsManage, DBModule, GestionDesTables, lectdonnee, main,
  GestionDesErreurs,Parser10, Forms, Classes, DateUtils, requetes,
  DeriveClimato;


{--------------------------------------------------------------------------
  Procédure permettant de lancer des simulations consecutives.

  @Author  CIRAD
  @Version v_3.0
  @param  idSimule  id de la simulation
  @param  numSimule numéro de la simulation
---------------------------------------------------------------------------}
procedure TScenario.LanceSimulationsConsecutives(idSimule: string; numSimule:
  Integer);
var
  i, j: Integer;   /// variable de boucle
  aClass: TEntityClass;  /// une TEntityClass
  
begin
  try
{$IFDEF AffichageComplet}
    MainForm.affiche(TimeToStr(Time) + #9 +
      'TScenario.LanceSimulationsConsecutives codeSimule : ' +
      FloatToStr(codeSimule) + ' numSimule : ' + FloatToStr(numSimule),0);
{$ENDIF}
    MainForm.affiche(TimeToStr(Time) + #9 + 'Simulation ' +
      (idSimule),0);

    tabProc.ChargeDll;
    //Chargement des dll
    tabClass.ajouter('Simulation');
    aClass := tabClass.Trouver('Simulation');
    aClass.AttributeFromTable('Simulation');
    //Création de la classe Simulation

    tabClass.CreationClasses;
    //création des autres classes

    SetTabAppels(idSimule, 0);
    //Crée les tabAppel de chaque simulation consécutive
    currentTabAppel := 0;

    j := Trunc(DBModule1.Simule.findfield('NbAnSim').Value) - 1;

    for i := 0 to j do // tant qu'on a pas atteint la derniere année
    begin
      Application.ProcessMessages;
      if mainForm.butStopSimulation.Caption <> 'Arrêt en cours' then
      begin
        //Modif CB 15/12/06   Attention verifier effet sur l'optimisation Numsimulation utilisée
        Numsimulation := i + 1;
        //Modif CB 15/12/06
        finLastSimul := 0;
        execSimul(idSimule, numSimule, i);
        //Execution de la suite de simulation pour la première année ?? Nil n'est pas une destruction
        aSimulation := nil;

      end;
    end;

    //MainForm.memDeroulement.Lines.Add('Simulation '+(idSimule)+' achevée à '+TimeToStr(Time));
  except
    AfficheMessageErreur('TScenario.LanceSimulationConsecutives | Simulation ' +
      aSimulation.name, UClasseScenario);
  end;
end;

{--------------------------------------------------------------------------
  Procédure execSimule

  @Author  CIRAD
  @Version v_3.0
  @param  idSimule  id de la simulation
  @param  numSimule numéro de la simulation
  @param  NumAnnSimule numéro de l'année simulée
---------------------------------------------------------------------------}
procedure TScenario.execSimul(const idSimule: string; numSimule: Integer;
  NumAnnSimule: Integer);
var
  i: Integer; /// entier
  an: Word; ///année
  mois: Word; /// mois
  jour: Word; /// jour
  dateDebRel: TDateTime;  /// Date
begin
  // Ré appel de excsimul pour les cultures consécutives
  try

    if finLastSimul <> 0 then
    begin
      dBModule1.Simule.Locate('Id', IdSimule, [loCaseInsensitive]);
      DecodeDate(finLastSimul, an, mois, jour);
      dBModule1.Simule.Edit;
      // Modif CB 23/11/06 attention années consécutives! et on écrase la définition de debutsimul!!
      // placer cela plus bas avec DateDebRel
      dBModule1.Simule.FieldByName('DebutSimul').Value :=
        EncodeDate(yearof(dBModule1.Simule.FieldByName('DebutSimul').AsDateTime),
        mois, jour);
      dBModule1.Simule.Post;
    end;

    if DBLocateSimule(idSimule, numSimule, NumAnnSimule) then
      //se positionne sur les table et Ecrase dans les tables Resjour et Rescum les anciens resultats de la simulation
      //en cours pour la première simulation consécutive de la première année seulement
      //(ex: lors d'un scenario d'optimisation, +ieurs simulations consécutives peuvent être executées sur plusieurs années,
      //seule la 1ere fait le nettoyage préalable)
    begin

      DecodeDate(dBModule1.Simule.FindField('DebutSimul').Value, an, mois,
        jour);  
       
      dateDebRel := encodedate(an + numAnnSimule, mois, jour);

      //on prend le tabAppel correspondant
      tabAppel := tabAppelConsecutif[currentTabAppel];

      if not ((finLastSimul <> 0) or (NumAnnSimule = 0)) then
        contextObjet.ResetContextInstance(dateDebRel, i);

      //Si la simulation courante est différente de la simulation qui était la avant on la remplace
      //(effectif lors de simulations consécutives)
      if (aSimulation = nil) then
      begin
        //Si il n'y avait pas de simulation (premiere execution) on crée l'instance Simulation
        aSimulation := TSimule.Create('Simulation', dateDebRel, idSimule);
        contextObjet.SetCurrentInstance(aSimulation);
      end
      else if (aSimulation.name <> idSimule) then
      begin
        //Si la simulation qui existait est diférente de celle en cours alors on la remplce
        //(cas de simulations consécutives)
        aSimulation := TSimule.Create('Simulation', dateDebRel, idSimule);
        contextObjet.GetCurrentInstance('Simulation').Free;
        contextObjet.SetCurrentInstance(aSimulation);
      end;

      aSimulation.InitializeCurrentInstance;
      //Lance les procedures d'initialisation de la simulation (module 0 entre autres)

      aSimulation.InitDatesSimule(NumAnnSimule);
      //Maj des dates de la simulation fonction du nb d'années déjà simulées.

      aSimulation.Pars := Parser10.Tparser.Create(nil);
      aSimulation.ContextSave := TStringList.Create;
      aSimulation.TabAppelSave := TStringList.Create;

      //ShowMessage(' 0 '+DBModule1.Resjour.FieldByName('jour').AsString);
      if (finLastSimul <> 0) or (NumAnnSimule = 0) then
        contextObjet.GereCreationInstances(dateDebRel);
      //ShowMessage(' 3 '+DBModule1.Resjour.FieldByName('jour').AsString);
    //Crée les instance si elles n'existent pas, les reset si elles sont différentes, ne fait rien si ce sont les mêmes
    //Et pour les simulation consécutives,
    //Utilise l'enregistrement sauvegardé sous l'id "$$idtmp$$" pour les tables en écritures comme enregistrement de départ

      aSimulation.InitDatesSemis(NumAnnSimule);
      //Maj dates de Semis en fonction du nb d'années déjà simulées.

      aSimulation.Aller(NumAnnSimule);
      //execution de la simulation unique

      finLastSimul := aSimulation.thisDate - 2;
      //DBModule1.Simule.Edit;
      //DBModule1.Simule.FieldByName('FinSimul').Value:=aSimulation.thisDate;
      //DBModule1.Simule.post;
      //ShowMessage(DateToStr(aSimulation.thisDate));

      CleanTable(DBModule1.Resjour, aSimulation.thisDate);
      //CleanTable(DBModule1.ResCum,aSimulation.thisDate);

      currentTabAppel := (currentTabAppel + 1) mod length(tabAppelConsecutif);
      //on passe au modèle suivant si il y en as et on boucle dans le cas de simulations pluriannuelles

      aSimulation.Clean;
      //on nettoie ContextSave, TabAppelSave, Pars

      //  MainForm.memDeroulement.Lines.Add((idSimule)+' année '+IntToStr(NumAnnSimule+1)+' finie... '+TimeToStr(Time));

      if not DBModule1.Simule.FieldByName('IdNextSimule').IsNull then
        //on execute la simulation consécutive si il y en a une
      begin
        contextObjet.prepareNextSimule(asimulation.thisDate);
        //On recopie chaque enregistrement de toutes les tables en écriture
        //afin de garder les valeurs de ces dernières pour la simulation suivante
        //Cet enregistrement est sauvegardé sous l'id "$$idtmp$$" dans la table correspondante

        execSimul(DBModule1.Simule.FieldByName('IdNextSimule').AsString,
          numSimule, NumAnnSimule);
      end;
    end
    else
    begin
      MainForm.affiche(TimeToStr(Time) + #9 +
        'Plantage DBLocateSimule(codeSimule,numSimule) dans ClasseScenario.TScenario.execSimul',0);
      Main.erreur := true;
    end;
  except
    on e: exception do
      AfficheMessageErreur(e.message + ' TScenario.execSimul | idSimule : ' +
        (idSimule) + ' numSimule : ' + FloatToStr(numSimule), UClasseScenario);
  end;
end;


{--------------------------------------------------------------------------
  Procédure appelée par ExecuterScenario, lancera les simulations pour
  l'optimisation, le forçage et l'exécution standard.
  Execution d'une simulation :
  On créé d'abord la classe simulation puis on créé l'objet simulation,
  on lance ensuite sa procédure bilanparcelle
  @Author  CIRAD
  @Version v_3.0
  @param  idSimule  id de la simulation
  @param  numSimule numéro de la simulation
---------------------------------------------------------------------------}
procedure TScenario.ExecuterSimulation(idSimule: string; numSimule: Integer);
// Execution d'une simulation :
// On créé d'abord la classe simulation puis on créé l'objet simulation,
// on lance ensuite sa procédure bilanparcelle
var

  aClass: TEntityClass; /// une TEntityClass
begin
  try
{$IFDEF AffichageComplet}
    MainForm..affiche((TimeToStr(Time) + #9 +
      'TScenario.ExecuterSimulation codeSimule : ' + FloatToStr(codeSimule) +
      ' numSimule : ' + FloatToStr(numSimule),0);
{$ENDIF}
    MainForm.affiche(TimeToStr(Time) + #9 + 'Simulation ' +
      (idSimule),0);

    //     Duree := Time;

    if DBLocateSimule(idSimule, numSimule, 0) then
    begin
      { TODO -ofred : dans le diag de classe 'ajouter' est une function, cela serait plus simple ? }
      // Création de la classe simulation
      tabClass.ajouter('Simulation');
      aClass := tabClass.Trouver('Simulation');
      aClass.AttributeFromTable('Simulation');

      // création de l'instance simulation
      aSimulation := TSimule.Create('Simulation', date, idSimule);
      //initialisation du parser
      {Ajout VN contraintes}aSimulation.Pars := Parser10.Tparser.Create(nil);
      {Ajout VN contraintes}aSimulation.TabAppelSave := TStringList.Create;
      {Ajout VN contraintes}aSimulation.ContextSave := TStringList.Create;

      // Enregistrement de celui-ci dans le TContext
      contextObjet.SetCurrentInstance(aSimulation);
      tabProc.ChargeDll;
      // lancement de la simulation
      aSimulation.BilanParcelle(idSimule);

      {Ajout VN contraintes}aSimulation.Pars.Destroy;
      {Ajout VN contraintes}aSimulation.TabAppelSave.Destroy;
      {Ajout VN contraintes}aSimulation.ContextSave.Destroy;

      //MainForm.memDeroulement.Lines.Add('Simulation '+(idSimule)+' achevée à '+TimeToStr(Time));
    end
    else
    begin
      MainForm.affiche(TimeToStr(Time) + #9 +
        'Plantage DBLocateSimule(codeSimule,numSimule) dans ClasseScenario.TScenario.ExecuterSimulation',0);
        Main.erreur := true;
    end;
  except
    AfficheMessageErreur('TScenario.ExecuterSimulation | idSimule : ' +
      (idSimule) + ' numSimule : ' + FloatToStr(numSimule), UClasseScenario);
  end;
end;

{*--------------------------------------------------------------------------
  Procédure remplirTableForcage
  @Author  CIRAD
  @Version v_3.0
---------------------------------------------------------------------------}
procedure remplirTableForcage();
var
  descrForcage: string; ///  descrForcage
  param: string;  /// param
  MonIndice: Integer;///indice
  MaPosition: Integer; /// position
  MaPos: Integer;  /// pos
begin
  descrForcage := DBModule1.Simule.FindField('DescriptionForcage').AsString;

  viderTable(dbModule1.Forcage);

  MonIndice := 0;
  MaPosition := Pos(';', descrForcage);
  while MaPosition >= 0 do
  begin
    if (maPosition > 0) then
    begin
      param := LeftStr(descrForcage, MaPosition - 1);
    end
    else
    begin
      param := descrForcage;
    end;

    // récupération du codeVar à partir du nomVar dans modeleVar
    MaPos := Pos('=', descrForcage);
    DBModule1.ModeleVar.Locate('NomVar', LeftStr(param, MaPos - 1), []);

    param := Copy(param, MaPos + 1, length(param));

    //viderTable(dbModule1.Forcage);
    DBModule1.Forcage.Insert;
    //Ad          DBModule1.Forcage.FindField('CodeVar').Value:= DBModule1.ModeleVar.findfield('CodeVar').Value;
    DBModule1.Forcage.FindField('IdVar').Value := DBModule1.ModeleVar.findfield('NomVar').Value;
    DBModule1.Forcage.FindField('Valeur').Value := param;
    DBModule1.Forcage.post;

    descrForcage := Copy(descrForcage, MaPosition + 1, length(descrForcage));
    Inc(MonIndice);
    if (MaPosition = 0) then
    begin
      MaPosition := -1;
    end
    else
    begin
      MaPosition := Pos(';', descrForcage);
    end;
  end;
end;

{--------------------------------------------------------------------------
  Procédure permettant d'exécuter les simulations du scénario suivant un
  type d'exécution (pré-positionne également les pointeurs sur les
  tables utilisées dans les simulations)
  @Author  CIRAD
  @Version v_3.0
  @param  typeExe 0 = une exécution normale, sans forçage 1= correspond à
  une exécution avec forçage 2=correspond à une exécution avec etude de
  sensibilité  3=correspond à une exécution avec optimisation de paramètres
---------------------------------------------------------------------------}
procedure TScenario.ExecuterScenario(typeExe: integer);
// Execution du Scenario :
// Ici on va gérer l'execution de toutes les simulations sélectionnées.
// On fait les tests pour remplir la table ModeleVar et appliquer le
// forçage. la premiere simulation est traitée hors de la boucle
var

  cptSimul: Integer; /// compteur de simulation
  i: Integer; /// indice
  codeModelePrec: string; /// code modele précédent
  NextSim: string;  /// simulation suivante
  monMaitre: TDatasource; /// source de donnée maitre
  monMaitre1: TDatasource; /// source de donnée maitre1
  monMasterSourcePluvio: TDatasource;/// source de donnée pluvio
  monMasterSourceMeteo: TDatasource; /// source de donnée meteo
  monMasterSourcePersonalData: TDatasource; /// source de donnée personnelle

  maitreField: string; ///champ
  maitreField1: string;///champ
  MonMasterFieldPluvio: string; ///champ
  MonMasterFieldMeteo: string;///champ
  MonMasterFieldPersonalData: string;///champ

  MonTexte: string; /// texte

begin
  try
{$IFDEF AffichageComplet}
    //MainForm.memDeroulement.Lines.Add('TScenario.ExecuterScenario typeExe : '+FloatToStr(typeExe));
{$ENDIF}
    //Ajout VN
    aSimulation := nil;


    monMasterSourceMeteo := dBModule1.Meteorologie.MasterSource;
    MonMasterFieldMeteo := dBModule1.Meteorologie.MasterFields;


    monMasterSourcePluvio := dBModule1.Pluviometrie.MasterSource;
    MonMasterFieldPluvio := dBModule1.Pluviometrie.MasterFields;
    monMasterSourcePersonalData := dBModule1.PersonalData.MasterSource;
    MonMasterFieldPersonalData := dBModule1.PersonalData.MasterFields;
    monMaitre := DBModule1.Station.MasterSource;
    maitreField := DBModule1.Station.MasterFields;
    monMaitre1 := DBModule1.Pays.MasterSource;
    maitreField1 := DBModule1.Pays.MasterFields;


    dBModule1.Meteorologie.MasterSource := nil;


    dBModule1.Pluviometrie.MasterSource := nil;
    dBModule1.PersonalData.MasterSource := nil;
    DBModule1.Station.MasterSource := nil;
    DBModule1.Pays.MasterSource := nil;



    DBModule1.Meteorologie.MasterFields := '';
    DBModule1.Meteorologie.Filter := '';
    DBModule1.Meteorologie.Filtered := False;

    DBModule1.Pluviometrie.MasterFields := '';
    DBModule1.Pluviometrie.Filter := '';
    DBModule1.Pluviometrie.Filtered := False;
    
    DBModule1.PersonalData.MasterFields := '';
    DBModule1.PersonalData.Filter := '';
    DBModule1.PersonalData.Filtered := False;
    DBModule1.Pays.MasterFields := '';
    DBModule1.Pays.Filter := '';
    DBModule1.Pays.Filtered := False;
    DBModule1.Station.MasterFields := '';
    DBModule1.Station.Filter := '';
    DBModule1.Station.Filtered := False;

    { TODO : Retirer les filtres ? potentiels sur les tables Meteo et pluvio}
    // classement des simulations suivant le codeModele
    DBModule1.RequeteSimule.Active := true;
    // on s'assure qu'on est bien sur le premier enregistrement
    DBModule1.RequeteSimule.First;
    // on initilise le premier Code module
    CodeModelePrec := 'Init'; //AD premier code module ou modèle ???
    typeSimul := typeExe;

    // le type 0 correspond à une exécution normale, sans forçage
    // le type 1 correspond à une exécution avec forçage
    // le type 2 correspond à une exécution avec etude de sensibilité
    // le type 3 correspond à une exécution avec optimisation de paramètres
    case typeExe of
      0, 1:
        // -------------- Scénario simple ou forçage
        begin
          Gestiondestables.numSimulation := 0;

         initTemp;

          // de la première simulation jusqu'à la derniere
          for cptSimul := 0 to DBModule1.RequeteSimule.RecordCount - 1 do
          begin
            {VN simcons}
            DBModule1.Simule.Locate('Id',
              DBModule1.RequeteSimule.findfield('IdSimule').Value, []);
          ////////////////////////////////////////////////////////////////////
            ViderTable(DBModule1.tempPluviometrie);
            ViderTable(DBModule1.tempMeteorologie);

            DBModule1.Pluviometrie.Filter := 'CodeStation=''' +
              vartostr(DBModule1.Site.Lookup('Id', DBModule1.SimuleIdSite.AsString,
              'CodeStationPluie') + '''');
            DBModule1.Pluviometrie.Filter := DBModule1.Pluviometrie.Filter +
              ' AND Jour >= ' +
              QuotedStr(DateToStr(DBModule1.SimuleDebutsimul.AsDateTime));
            DBModule1.Pluviometrie.Filter := DBModule1.Pluviometrie.Filter +
              ' AND Jour <= ' +
              QuotedStr(DateToStr(encodedate(DBModule1.SimuleAnFinSimul.AsInteger,
              12, 31)));
            DBModule1.Pluviometrie.Filtered := True;
          ////////////////////////////////////////////////////////////////////



            //AlimenteTableClimatoPluviometrie;
         ////////////////////////////////////////////////////////////////////
            AlimenteTableTempPluviometrie;


            DBModule1.Meteorologie.Filter := 'CodeStation=''' +
              vartostr(DBModule1.Site.Lookup('Id', DBModule1.SimuleIdSite.AsString,
              'CodeStationMeteo') + '''');
            DBModule1.Meteorologie.Filter := DBModule1.Meteorologie.Filter +
              ' AND Jour >= ' +
              QuotedStr(DateToStr(DBModule1.SimuleDebutsimul.AsDateTime));
            DBModule1.Meteorologie.Filter := DBModule1.Meteorologie.Filter +
              ' AND Jour <= ' +
              QuotedStr(DateToStr(encodedate(DBModule1.SimuleAnFinSimul.AsInteger,
              12, 31)));
            DBModule1.Meteorologie.Filtered := True;
            DBModule1.PersonalData.Filter := DBModule1.Meteorologie.Filter;


            DBModule1.PersonalData.Filtered := True;


            AlimenteTableTempMeteorologie;
          
            Application.ProcessMessages;

            DBModule1.PersonalData.Filter := '';
            DBModule1.PersonalData.Filtered := False;

            //DeriveMeteoPluvio; // ViB le 14/01/2008 retrait  pour  les deux dérivation Temppluvio et TempMeteo

            DeriveTableTempMeteoTempPluvio;
            
            //Fin de Derivation de la Climato

            DBModule1.Pluviometrie.Filter := '';
            DBModule1.Pluviometrie.Filtered := False;

            DBModule1.Meteorologie.Filter := '';
            DBModule1.Meteorologie.Filtered := False;
           
            // si le codeModele est différent de celui de la simulation precedente, on construit la table des variables
            if DBModule1.RequeteSimuleIdModele.Value <> CodeModelePrec then
            begin
              // appel de la méthode de placement sur toute les tables : DBLocateSimule
              // mise à jour du Code precedent
              CodeModelePrec := DBModule1.RequeteSimuleIdModele.Value;
              // positionnement sur la table Modele et donc ModeleModule pour remplir ModeleVar
              DBModule1.Modele.Locate('Id', CodeModelePrec, [loCaseInsensitive]);
                { TODO : On définit déjà la position du curseur de la table modele dans DBLocateSimule est-ce donc utile ici? }
              // vider la table ModeleVar
              ViderTable(DBModule1.ModeleVar);
              // remplir la table ModeleVar en fonction des variables utilisées

              {VN simcons}//RemplirTableModeleVar(dBModule1.ModeleId.value);
              RemplirTableModeleVarSimule(dBModule1.Simule.findfield('Id').Value);

              RemplirTableModeleVar('Init');

              if typeExe = 1 then
                ForcerTable; // si mode Forçage, alors on subtitue les valeurs de la table Forçage
            end; // fin if DBModule1.RequeteSimuleIdModele.Value <> CodeModelePrec then

            // Au debut de chaque simulation, création des différents objets
            tabAppel := TTabAppels.Create();
            tabAppelIni := TTabAppels.Create();
            tabClass := TTabClasses.Create();
            contextObjet := TContext.Create();

            DBModule1.Simule.Locate('Id',
              DBModule1.RequeteSimule.findfield('IdSimule').Value, []);

            // si y'a une description de forçage, il faut remplir la table forcage
            if (DBModule1.Simule.FindField('DescriptionForcage').AsString <> '')
              then
            begin
              remplirTableForcage;
              ForcerTable;
            end;
            {ajout AD}
            
            //showmessage('avant on supprime les ancien enregistrements');

            // Modif CB 30/01/07 on supprime ici les résultats de simulations
            NextSim := DBModule1.Simule.FindField('Id').Value;
            SupprEnreg(DBModule1.Resjour, 'IdSimule', NextSim);
       

            NextSim := DBModule1.Simule.FindField('IdNextSimule').asString;

            //showmessage('nextsim ok');

            if NextSim <> '' then
            begin
              SupprEnreg(DBModule1.Resjour, 'IdSimule', NextSim);

            end;
            // Modif CB 30/01/07 on supprime ici les résultats de simulations

            

            {modif APas requete}
            DeriveResJour();


            // Execution de la simulation
            {VN simcons}//ExecuterSimulation(DBModule1.RequeteSimuleIdSimule.Value,0);
            LanceSimulationsConsecutives(DBModule1.RequeteSimuleIdSimule.Value,
              0);

            {modif APas requete}
            execRequeteScenar(CptSimul);

            DBmodule1.ResTemp.Close;
            DBModule1.ResTemp.EmptyTable;
            DBmodule1.ResTemp.Open;

            {fin modif APas requete}

             {VN simcons}
            tabAppelIni.Free;

            TabAppel.Free;

            contextObjet.Free;


            //ViB 03/10/2007 DeriveClimato
            ////////////////////////////////////////////////////////////////////
            if(ParamCount = 0) then
            begin
              EnleveDeriveMeteoPluvioMoteurTxt;
            end
            else
            begin
              EnleveDeriveMeteoPluvio;
            end;
             //vb30012008
            ////////////////////////////////////////////////////////////////////
            // passage à la simulation suivante
            DBModule1.RequeteSimule.Next;
            aSimulation := nil; // edit VN
          end;
          DBModule1.RequeteSimule.Active := false;
        end;

      
      2: { TODO -ofred : ce qui suit est un labyrinthe pour stagiaire :-) }
        // -------------- méthodes d'optimisation
        begin
          MainForm.affiche(TimeToStr(Time) + #9 +
            'Scénario en cours: Optimisation de paramètres',0);
          // CONTRAINTES :
           // 1) Numsimulation géré en externe dans ce cas
           // sous unit Communication TMonOle.Bilan incrementation a chaque appel externe
           // ex avec R. A la création du main on initialise à 0. Pb on ne peut pas fermer
           // qd appel externe
           // 2) Dans le cas d'une optimisation il ne peut y avoir qu'un seul Modele
           // Modif cb
           {         if NumSimulation = 0
                      then NumSimulation := 1;
           }
            // A tester on bascule sur Result dans DBAnalyse
            // Modif cb
          DBModule1.Resjour.Active := false;
          DBModule1.Resjour.DatabaseName := 'DBAnalyse';
          DBModule1.Resjour.TableName := 'Resjour';
          DBModule1.Resjour.Active := true;

          if NumSimulation = 0 then
          begin
            // mise à jour du Code precedent
            CodeModelePrec := DBModule1.RequeteSimuleIdModele.Value;
            DBModule1.Modele.Locate('Id', CodeModelePrec, [loCaseInsensitive]);
            ViderTable(DBModule1.ModeleVar);
            RemplirTableModeleVarSimule(dBModule1.Simule.findfield('Id').Value);
            RemplirTableModeleVar('Init');
            NumSimulation := 1;
          end;

          typeSimul := 3;
          // Mise à jour de la table forcage par rapport au fichier de R
          MajForcageFichier(CheminSarraH + '\ValParamOut.txt');
          // Forcage de la table modeleVar
          ForcerTable;

          // execution des simulations : CONTRAINTE elles ont toutes le même code modele.
          for cptSimul := 0 to DBModule1.RequeteSimule.RecordCount - 1 do
          begin
            {VN simcons}DBModule1.Simule.Locate('Id',
              DBModule1.RequeteSimule.findfield('IdSimule').Value, []);

         // Au debut de chaque simulation, création des différents objets
            TabAppel := TTabAppels.Create();
            {VN simcons}tabAppelIni := TTabAppels.Create();
            TabClass := TTabClasses.Create();
            contextObjet := TContext.Create();

            LanceSimulationsConsecutives(DBModule1.RequeteSimuleIdSimule.Value,
              0);
                

             {VN simcons}tabAppelIni.Free;
            TabAppel.Free;
            contextObjet.Free;
            // passage à la simulation suivante
            DBModule1.RequeteSimule.Next;
          end;
          DBModule1.RequeteSimule.Active := false;
          // fin des simulations, requete de selection des valeurs simulées sélectionnées.
          RequeteResultat(2);

          //on genère alors un fichiers avec ces valeurs.
          GenereFichierResult(CheminSarraH + '\ValSim.txt', 2);
          MainForm.affiche(TimeToStr(Time) + #9 +
            'TScenario.ExecuterScenario: GenereFichierResult de ValSim.txt effectué',0);

          ViderTable(DBModule1.Resjour);
          //DBModule1.Resjour.EmptyTable;   //ViB le 15/07/2004
          DBModule1.Resjour.Active := false;
          DBModule1.Resjour.DatabaseName := 'DBResult';
          DBModule1.Resjour.TableName := 'Resjour';
          DBModule1.Resjour.Active := true;
         
          MainForm.affiche('',0);
          dBModule1.queGenerique.active := False;
          dBModule1.queGenerique.SQL.Clear;
          dBModule1.queGenerique.SQL.Add('SELECT DISTINCT DV.NOMCOURS as Nom, F.Valeur as Valeur FROM ":DBModele:Forcage.DB" F, ":DBModele:DictionnaireVar.DB" DV WHERE DV.CODEVAR = F.CodeVar ORDER BY DV.NOMCOURS, F.Valeur');
          dBModule1.queGenerique.active := True;
          dBModule1.queGenerique.First;
          for i := 0 to dBModule1.queGenerique.RecordCount - 1 do
          begin
            if Length(dBModule1.queGenerique.Fields.Fields[0].AsString) >= 9
              then
              MonTexte := dBModule1.queGenerique.Fields.Fields[0].AsString + #9 +
                ' valeur trouvée: ' +
                Floattostr(dBModule1.queGenerique.Fields.Fields[1].AsFloat)
            else
              MonTexte := dBModule1.queGenerique.Fields.Fields[0].AsString + #9
                + #9 + ' valeur trouvée: ' +
                Floattostr(dBModule1.queGenerique.Fields.Fields[1].AsFloat);
            MainForm.affiche(TimeToStr(Time) + #9 + MonTexte,0);
            dBModule1.queGenerique.Next;
          end;
          dBModule1.queGenerique.SQL.Clear;
          dBModule1.queGenerique.Close;
          // fin du test de récup...


        end;

    end;

    DBModule1.Station.MasterSource := MonMaitre;
    DBModule1.Pays.MasterSource := MonMaitre1;
    DBModule1.Pays.MasterFields := MaitreField1;
    DBModule1.Station.MasterFields := MaitreField;

    DBModule1.Meteorologie.MasterSource := monMasterSourceMeteo;

    DBModule1.Pluviometrie.MasterSource := monMasterSourcePluvio;

    DBModule1.Meteorologie.MasterFields := MonMasterFieldMeteo;
  
    DBModule1.Pluviometrie.MasterFields := MonMasterFieldPluvio;
    DBModule1.PersonalData.MasterSource := monMasterSourcePersonalData;
    DBModule1.PersonalData.MasterFields := MonMasterFieldPersonalData;
  except
    AfficheMessageErreur('TScenario.ExecuterScenario | typeExe : ' +
      IntToStr(typeExe), UClasseScenario);
  end;
end;

{--------------------------------------------------------------------------
  Procédure appelée par ExecuterScenario, pour lancer les simulations
  de type étude de sensibilité.
  Gestion du lancement des simulations de type étude de sensibilité
  Cette procédure est une procédure récursive, les valeurs à incrémenter
  sont placées dans un tableau, la simulation est alors lancée pour
  chaque suite de valeurs possibles.

  @Author  CIRAD
  @Version v_3.0
  @param  indiceTab indice
---------------------------------------------------------------------------}

procedure TScenario.Sensibilite(indiceTab: Shortint);
// Gestion du lancement des simulations de type étude de sensibilité
// Cette procédure est une procédure récursive, les valeurs à incrémenter sont
// placées dans un tableau, la simulation est alors lancée pour chaque suite
// de valeurs possibles.
begin
  try
    // si on est à la fin du tableau
    if (indiceTab <> length(tabSensibilite) - 1) and (indiceTab <=
      length(tabSensibilite) - 1) then
    begin
      // tant que la valeur n'est pas égale à la valeur à la valeur maximum
      while tabSensibilite[indiceTab, 0] <= tabSensibilite[indiceTab, 1] do
      begin
        // rappel de la procedure avec l'element suivant
        Sensibilite(indiceTab + 1);
        // incrémentation de la valeur suivant le step
        tabSensibilite[indiceTab, 0] := tabSensibilite[indiceTab, 0] +
          tabSensibilite[indiceTab, 2];
      end;
      // on remet la valeur initiale
      tabSensibilite[indiceTab, 0] := tabSensibilite[indiceTab, 3];
    end

      // on n'est pas a la fin du tableau
    else
    begin
      if (indiceTab <= length(tabSensibilite) - 1) then
      begin
        // tant que la valeur n'est pas égale à la valeur à la valeur maximum
        while tabSensibilite[indiceTab, 0] <= tabSensibilite[indiceTab, 1] do
        begin
          // Au debut de chaque simulation, création des différents objets

          tabAppel := TTabAppels.Create();
          {VN simcons}tabAppelIni := TTabAppels.Create();
          tabClass := TTabClasses.Create();
          contextObjet := TContext.Create();

          // remplacement dans la table des variables
          MAJTableForcage;
          // execution de la simulation
          ForcerTable;

          // Deux modes d'appel d'ExecuterSimulation :
          // O Destruction des enregistrements ayant le meme CodeSimule (meme simulation)
          // 1 on ne ne fait rien
          if numSimulation = 1 then
            LanceSimulationsConsecutives(DBModule1.RequeteSimuleIdSimule.Value,
              0)
              //VN ExecuterSimulation(DBModule1.RequeteSimuleIdSimule.Value,0)
          else //VN ExecuterSimulation(DBModule1.RequeteSimuleIdSimule.Value,1);
            LanceSimulationsConsecutives(DBModule1.RequeteSimuleIdSimule.Value,
              1);
          inc(NumSimulation);
          // incrémentation de la valeur suivant le step
          tabSensibilite[IndiceTab, 0] := tabSensibilite[indiceTab, 0] +
            tabSensibilite[indiceTab, 2];

          {VN simcons}tabAppelIni.Free;
          tabAppel.Free;
          contextObjet.Free;
        end;

        // on remet la valeur initiale
        tabSensibilite[indiceTab, 0] := tabSensibilite[indiceTab, 3];
      end;
    end;
  except
    AfficheMessageErreur('TScenario.Sensibilite | indice : ' +
      IntToStr(indiceTab), UClasseScenario);
  end;
end;

{--------------------------------------------------------------------------
  Procédure permettant de créer et remplir du tableau des
  valeurs de sensibilité

  @Author  CIRAD
  @Version v_3.0
---------------------------------------------------------------------------}
procedure TScenario.RemplirTableauSensibilite;
// Création et remplissage du tableau des valeurs de sensibilité
var
  indice: Integer;  /// indice de boucle
begin
  try
    Setlength(tabSensibilite, DBModule1.Forcage.RecordCount, 4);
    DBModule1.Forcage.First;

    for indice := 0 to DBModule1.Forcage.RecordCount - 1 do
    begin
      // récupération des données dans la table forçage
      tabSensibilite[indice, 0] := DBModule1.ForcageValeurMin.Value;
      tabSensibilite[indice, 1] := DBModule1.ForcageValeurMax.Value;
      tabSensibilite[indice, 2] := DBModule1.ForcageStep.Value;
      tabSensibilite[indice, 3] := DBModule1.ForcageValeurMin.Value;
      DBModule1.Forcage.Next;
    end;
  except
    AfficheMessageErreur('TScenario.RemplirTableauSensibilite',
      UClasseScenario);
  end;
end;

{--------------------------------------------------------------------------
  Procédure permettant de mettre à jour la table forçage en
  fonction du tableau des valeurs de sensibilité.

  @Author  CIRAD
  @Version v_3.0
---------------------------------------------------------------------------}
procedure TScenario.MAJTableForcage;
// Mise à jour de la table forçage en fonction du tableau des valeurs de sensibilité
var
  indice: Integer;  ///indice de boucle
begin
  try
    DBModule1.Forcage.First;
    for indice := 0 to DBModule1.Forcage.RecordCount - 1 do
    begin
      DBModule1.Forcage.Edit;
      DBModule1.ForcageValeur.Value := TabSensibilite[indice, 0];
      DBModule1.Forcage.Post;
      DBModule1.Forcage.Next;
    end;
  except
    AfficheMessageErreur('TScenario.MAJTableForcage', UClasseScenario);
  end;
end;
end.

