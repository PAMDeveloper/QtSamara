/**   @file
     @brief
*/
#include "ClasseSimule.h"

void TSimule::CreationClasses()
// Requete pour création automatique des classes(site, crop ....)
// La requete trie les entités en fonction de leur hierarchie et celles
// dont la hierarchie est = 0 ne sont pas retenues. Les entités sont ensuite
// crées dans cet ordre.
{
  // Recuperation de la liste des entites necessaires pour ce modele
  // Voici un exemple de la requete DbModule1.RequeteEntites :
  //    REPONSE CodeEntity      NomEntite       Hierarchie
  //    1       3               Site          1
  //    2       1               Plot            2
  //    3       4               Soil            2
  //    4       2               Crop            3
  //    5       5               Plante          3
}

//----------------------------------------------------------------------------//

void TSimule::InitSimulation(double &anDebutSimul, double &anFinSimul, double &nbAnSim, double &dateDebutSimul, double &dateFinSimul, double &annee, double &maxNbjSimule, double &dateEnCours, double &nbjSemis, double &dateSemisCalc,
                             double const &nbjas, double const &codeParcelle)
// Initialisation des divers paramètres d'entrée. Cette procédure correspond au module 0.
{
  try {
    // récupération des dates et écriture dans rescrop(ou tabValues) à la sortie de la procedure
    anDebutSimul = DbModule1.Simule.findfield("AnDebutSimul").Value;
    anFinSimul = DbModule1.Simule.findfield("AnFinSimul").Value;
    nbAnSim = DbModule1.Simule.findfield("NbAnSim").Value;
    dateFinSimul = DbModule1.Simule.findfield("FinSimul").Value;
    dateDebutSimul = DbModule1.Simule.findfield("DebutSimul").Value;
    annee = DbModule1.Simule.findfield("AnDebutSimul").Value;
    dateEnCours = DbModule1.Simule.findfield("DebutSimul").Value;
    maxNbjSimule = Trunc(dateFinSimul - dateDebutSimul) + 1;
    nbjSemis = NullValue;
    dateSemisCalc = NullValue;

  } catch (...) {
    AfficheMessageErreur("TSimule.InitSimulation", UClasseSimule);
  }
}

//----------------------------VN simcons-------------------------------------//

void TSimule::InitDatesSimule(int const &numSimul) {
  unsigned short jour; unsigned short mois; unsigned short an;

  try {
    // initialisation des variables temporelles pour la nouvelle année
    SetVal("Annee", GetVal("AnDebutSimul") + numSimul);
    DecodeDate(GetVal("DebutSimul"), an, mois, jour);
    SetVal("DebutSimul", EncodeDate(Trunc(GetVal("AnDebutSimul")) + numSimul
                                    , mois, jour));
    SetVal("FinSimul", GetVal("DebutSimul") + GetVal("MaxNbjSimule"));

    thisDate = GetVal("DebutSimul");
    //ShowMessage(DateToStr(thisDate)+' '+DateToStr(GetVal('FinSimul')));

  } catch (...) {
    AfficheMessageErreur("TSimule.InitDatesSimule | Classe ", UClasseSimule);
  }
}

//----------------------------VN simcons-------------------------------------//

void TSimule::InitDatesSemis(int const &numSimul) {
  unsigned short jour; unsigned short mois; unsigned short an;





  try {
    //Si la date de semis est definie par l'utilisateur 3
    //alors on initialise DateSemisCalc à cette valeur (+le nbre d'années simulées)
    //sinon DateSemisCalc prend la valeur 0
    if (contextObjet.GetCurrentInstance("Plot").GetVal("DateSemis") == NullValue)
      SetVal("DateSemisCalc", 0);
    else {
      DecodeDate(contextObjet.GetCurrentInstance("Plot").GetVal("DateSemis")
                 , an, mois, jour);
      SetVal("DateSemisCalc", EncodeDate(an + numSimul, mois, jour));
    }
    //Si la date de semis existe on l'initialise correctement
    //sinon on la met à nullvalue
    if (contextObjet.GetCurrentInstance("Plot").GetVal("DateSemis") != NullValue)
      SetVal("NbJAS", thisDate - GetVal("DateSemisCalc"));
    else
      SetVal("NbJAS", NullValue);

    //ShowMessage(DateToStr(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis'))+' '+DateToStr(thisdate)+' '+DateToStr(GetVal('DateSemisCalc')));

  } catch (...) {
    AfficheMessageErreur("TSimule.InitDatesSemis | Classe ", UClasseSimule);
  }
}
//--------------------------------------------------------------------------//

void TSimule::InitYear(int const &numSimul)
// Initialisation des variables qui changent pour chaque années et création
// de toutes les instances la première année sauf Crop (ou celle qui apparaissent
// en cours de simulation
{
  unsigned short jour; unsigned short mois; unsigned short an;
  int i;




  try {
    // initialisation des variables temporelles pour la nouvelle année
    SetVal("Annee", GetVal("AnDebutSimul") + numSimul);
    DecodeDate(GetVal("DebutSimul"), an, mois, jour);
    SetVal("DebutSimul", EncodeDate(Trunc(GetVal("AnDebutSimul")) + numSimul
                                    , mois, jour));
    SetVal("FinSimul", GetVal("DebutSimul") + GetVal("MaxNbjSimule"));

    thisDate = GetVal("DebutSimul");

    //ShowMessage(DateToStr(thisDate));
    //contextObjet.ResetContextInstance(thisDate);
    //contextObjet.GereCreationInstances(thisDate);

    if (numSimul == 0) {
      // creation des instances d'entite et Initialiser
      DbModule1.RequeteInstance.Active = true;
      DbModule1.RequeteInstance.First;
      {
        long i_end = DbModule1.RequeteInstance.RecordCount ; for (i = 0 ; i < i_end ; ++i) {
          // on créé une instance puis on la place dans le context
          contextObjet.SetCurrentInstance(TEntityInstance.create(DbModule1.RequeteInstanceNomEntite.Value, thisDate, ""));
          DbModule1.RequeteInstance.Next;
        }
      }
      DbModule1.RequeteInstance.Active = false;
    }

    // suppression de l'entite crop si on n'est pas à l'année 1
    else { // numSimul <> 0
      if (contextObjet.GetCurrentInstance("Crop") != 00)
        contextObjet.FreeEntityInstance("Crop");
      if (DBLocateDate(thisDate)) {

        {
          long i_end = length(contextObjet.currentInstances) ; for (i = 0 ; i < i_end ; ++i) {
            if (contextObjet.currentInstances[ i ] != 00) {
              contextObjet.currentInstances[ i ].MAJBookmark(thisDate);
              contextObjet.currentInstances[ i ].MiseAZero(0);
              contextObjet.currentInstances[ i ].myClass.MAJTappels(thisDate);
            }
          }
        }
      }
    }
    //VN Remplacement
    /*DecodeDate(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis'),An,Mois,Jour);
    SetVal('DateSemisCalc',EncodeDate(trunc(GetVal('AnDebutSimul'))+ NumSimul,Mois,Jour));

    if contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis') = nullValue then
          begin { TODO : Gestion des dates de semis inconnues a finaliser *//* TODO -oVB : à faire oui...
 contextObjet.GetCurrentInstance('Plot').SetVal('DateSemis',GetVal('FinSimul'));
 //contextObjet.GetCurrentInstance('Plot').SetVal('TypeSemis',2);
 end
else
 begin
 //contextObjet.GetCurrentInstance('Plot').SetVal('TypeSemis',1);
 DecodeDate(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis'),An,Mois,Jour);
 contextObjet.GetCurrentInstance('Plot').SetVal('DateSemis',EncodeDate(trunc(GetVal('AnDebutSimul'))+ NumSimul,Mois,Jour));
 end;

SetVal('NbJAS',thisDate - contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis') );*/

    if (contextObjet.GetCurrentInstance("Plot").GetVal("DateSemis") == NullValue)
      SetVal("DateSemisCalc", 0);
    else {
      DecodeDate(contextObjet.GetCurrentInstance("Plot").GetVal("DateSemis")
                 , an, mois, jour);
      SetVal("DateSemisCalc", EncodeDate(an + numSimul, mois, jour));
    }

    if (contextObjet.GetCurrentInstance("Plot").GetVal("DateSemis") != NullValue)
      SetVal("NbJAS", thisDate - GetVal("DateSemisCalc"));
    else
      SetVal("NbJAS", NullValue);

    //AD   SetVal('CodeParcelle',DBModule1.SimuleCodeParcelle.Value);
    //ShowMessage(DateToStr(GetVal('FinSimul'))+' '+IntToStr(numSimul)+' '+DateToStr(thisDate)+' '+DateToStr(contextObjet.GetCurrentInstance('Plot').GetVal('DateSemis')));

    /*?*//*$IFDEF AffichageComplet*/
    contextObjet.AfficheLesTEClasses();
    contextObjet.AfficheLesTEInstances();
    /*?*//*$ENDIF*/

  } catch (...) {
    AfficheMessageErreur("TSimule.InitYear | Numéro de simulation " +
                         inttostr(numSimul), UClasseSimule);
  }
}

//----------------------------------------------------------------------------//
/// Construit la liste des appels de procédures ainsi que la récupération
/// des paramètres nécessaires à chacun des appels de procédure ainsi créée.
/* TODO -oViB : Pourquoi SetList est rattaché à TSimule, il est mieux qu'il soit géré dasn TAppel */

void TSimule::SetList()
// Construction de la liste des appels (construit une fois pour toute). Cette liste ne contiendra pas les modules d'initialisation
{
  TAppel aCallProc;
  int j;



  try {
    // on ajoute l'appel au module 0, le module d'initialisation général
    DbModule1.Module.Locate("CodeModule", 0, [  ]);
    aCallProc = TAppel.create(DbModule1.Module.fieldbyname("CodeModule").Value
                              , DbModule1.Module.fieldbyname("CodeEntity").Value, 0
                              , DbModule1.Module.fieldbyname("NomModule").Value);
    TabAppel.ajouter(aCallProc);

    DbModule1.ModuleVar.First;
    {
      long j_end = DbModule1.ModuleVar.RecordCount ; for (j = 0 ; j < j_end ; ++j) {
        aCallProc.DefArgument(DbModule1.ModuleVarOrdre.Value
                              , TArgument.create(DbModule1.EntiteNomEntite.Value
                                                 , DbModule1.DictionnaireVarNOMCOURS.Value));
        aCallProc.DefParam(j, DbModule1.DictionnaireVarNOMCOURS.Value
                           , Conversion(DbModule1.ModuleVarTypeParam.Value));
        DbModule1.ModuleVar.Next;
      }
    }

    // ajout de tous les modules propres au modèle  { TODO -oViB : Comment être assuré qu'on a déjà positionné le pointeur de la table modèle sur le bon modèle pour que la relation maitre détail filtre les modules? }
    DbModule1.ModeleModule.First;

    // Lecture de toutes les lignes de la table DBModele.Module .
    // Pour chaque ligne :
    // - trouve dans DBModule.Module la ligne ayant le meme CodeModule
    // - cree l'instance TAppel correspondant
    // - recupere les variables necessaires dans DBModuleVar (table dynamique)
    while (!DbModule1.ModeleModule.Eof) {
      DbModule1.Module.Locate("CodeModule"
                              , DbModule1.ModeleModuleCodeModule.Value, [  ]);

      aCallProc =
          TAppel.create(DbModule1.Module.fieldbyname("CodeModule").Value
                        , DbModule1.Module.fieldbyname("CodeEntity").Value
                        , DbModule1.ModeleModule.fieldbyname("Step").Value
                        , DbModule1.Module.fieldbyname("NomModule").Value);

      TabAppel.ajouter(aCallProc);

      // Parcours des variables du module
      // (car action du DBModule1.Module.Locate précédent soit application de la
      // relation maitre détail entre ModuleVar et Module)
      DbModule1.ModuleVar.First;
      {
        long j_end = DbModule1.ModuleVar.RecordCount ; for (j = 0 ; j < j_end ; ++j) {
          aCallProc.DefArgument(DbModule1.ModuleVarOrdre.Value
                                , TArgument.create(DbModule1.EntiteNomEntite.Value
                                                   , DbModule1.DictionnaireVarNOMCOURS.Value));
          aCallProc.DefParam(j, DbModule1.DictionnaireVarNOMCOURS.Value
                             , Conversion(DbModule1.ModuleVarTypeParam.Value));
          DbModule1.ModuleVar.Next;
        }
      }

      DbModule1.ModeleModule.Next;
    }

    //DBModule1.ModeleModule.Next;  { TODO -oViB : ké ké c'est que ça????????? }

  } catch (...) {
    AfficheMessageErreur("TSimule.SetList", UClasseSimule);
  }
}

//----------------------------------------------------------------------------//

//AD procedure TSimule.BilanParcelle(codeSimule : Integer);

void TSimule::BilanParcelle(std::string idSimule)
// Gestion du déroullement de la simulation :
// - Création des différentes classe
// - Création de la liste des appels
// - Initialisation de la simulation (module 0)
// - lancement de la simulation pour chaque année
{
  int i; int j;



  try {
    CreationClasses; // Creation des classes (site, soil ...)
    //tabClass.CreationClasses;
    //SetTabAppels(idSimule,0);

    SetList; // Création de liste des Tappels

    //AssignFile(f,'D:\'+DBModule1.Simule.fieldbyname('Id').AsString+'0.txt');
    //Rewrite(f);
    //close(f);

    RunModule(0);   // Initialisation de l'objet simule

    /* TODO -oViB : ATTENTION: il existe un UpdateData (dans DBModule) de la table
    Simule qui met à 1 le nombre d'année si l'année de la date de fin de
    simulation est égal à l'anné de fin de simulation */
    j = Trunc(GetVal("NbAnSim")) - 1;

    //Assign(f,'D:\Init.txt');
    //Rewrite(f);
    //close(f);

    {
      long i_end = j + 1 ; for (i = 0 ; i < i_end ; ++i) { // tant qu'on a pas atteint la derniere année
        Application.ProcessMessages;
        if (mainForm.butStopSimulation.Caption != "Arrêt en cours") {
          //tabAppel.writeAppels(i);
          InitYear(i);
          //InitDates(i);
          //contextObjet.RunInitInstances;
          //SimulAn;
          numSimule2 = i + 1;
          Aller(i + 1);

          //VN save
          while (ContextSave.Count != 0) { //on nettois les sauvegardes
            TContextSave(ContextSave.Objects[ 0 ]).Destroy;
            ContextSave.Delete(0);
          }
          while (TabAppelSave.Count != 0) {
            TTabAppelsSave(TabAppelSave.Objects[ 0 ]).Destroy;
            TabAppelSave.Delete(0);
          }

          //Append(f);
          //WriteLn(f, '.');
          //WriteLn(f);
          //close(f);
          //contextObjet.EmptyContextInstance;
        }
      }
    }

  } catch (...) {
    AfficheMessageErreur("TSimule.BilanParcelle | Simulation " + (idSimule)
                         , UClasseSimule);
  }
}

//----------------------------------------------------------------------------//

void TSimule::SimulAn()
// Permet la Gestion de la date de semis
{
  try {
    if (GetVal("TypeSemis") == 2) {
      /*if not PlanteTypeArachide then
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
      aSimule.myModele.SetList;*/
    }

  } catch (...) {
    AfficheMessageErreur("TSimule.SimulAn", UClasseSimule);
  }
}

//----------------------------------------------------------------------------//

void TSimule::ConfirmeSemis()
// Gestion de la date de semis
{
  try {
    /*SetVal('Semis',thisDate);
    If (GetVal('Semis')+ aPlante.TabState[cyclePot]
          > aSimule.TabState[DateFinSimul])
    then aSimule.TabState[Semis]:=
     aSimule.TabState[DateFinSimul] -aPlante.TabState[cyclePot]; */

  } catch (...) {
    AfficheMessageErreur("TSimule.ConfirmeSemis", UClasseSimule);
  }
}

//----------------------------------------------------------------------------//

void TSimule::RunModule(int numModule)
// Procedure d'appel d'un module avec son numéro et le contexte associé à la simulation
{
  try {
    TabAppel.Executer(numModule, contextObjet);

  } catch (...) {
    AfficheMessageErreur("TSimule.RunModule | Module " +
                         VarToStr(DbModule1.Module.Lookup("CodeModule", numModule, "NomCours")) + " ("
                         + inttostr(numModule) + ")", UClasseSimule);
    //on E: Exception do AfficheMessageErreur(E.message+'TSimule.RunModule | Module '+VarToStr(dBModule1.Module.Lookup('CodeModule',numModule,'NomCours'))+' ('+inttostr(numModule)+')',UClasseSimule);

  }
}

//----------------------------------------------------------------------//
// Execute le module si la date d'execution correspond à la date en
// cours
//----------------------------------------------------------------------//

void TSimule::StepExe() {
  int i;




  try {
    {
      long i_end = high(TabAppel.listAppel) + 1 ; for (i = low(TabAppel.listAppel) ; i < i_end ; ++i) {
        //ShowMessage(DateToStr(tabAppel.listAppel[i].nextExe)+' '+DateToStr(thisDate));
        if (TabAppel.listAppel[ i ].nextExe == thisDate)
          // si la date correspond à la date en cours alors on appelle le module
        {
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
          RunModule(TabAppel.listAppel[ i ].id);
          //if (tabAppel.listAppel[i].id=33229) and (thisDate=EncodeDate(1997,09,15))
          //if show
          //then ShowMessage(inttostr(tabAppel.listAppel[i].id)+' '+DateToStr(contextObjet.getcurrentInstance('Simulation').GetVal('DATEENCOURS'))+' '+DateToStr(thisdate));

        }
      }
    }

  } catch (...) {
    AfficheMessageErreur("TSimule.StepExe", UClasseSimule);
  }
}

//----------------------------------------------------------------------------//

void TSimule::CalculNextExe()
// Calcule la date suivante et met à jour le tableau des appels.
// Pour chaque module dont la date d'execution correspond à la date en
// cours, on fait passer la date d'execution au prochain pas de temps
// Ex: pour des modules mensuels, le 31/03 donne 30/04
{
  int i;
  double step;
  TDateTime DateExe;
  unsigned short Year; unsigned short Month; unsigned short Day;


  try {
    {
      long i_end = length(TabAppel.listAppel) ; for (i = 0 ; i < i_end ; ++i) {
        // si la date correspond à la date en cours alors on calcule la prochaine date d'execution en fonction du step
        if (TabAppel.listAppel[ i ].nextExe == thisDate) {
          DateExe = thisDate; //Edit VN debug
          step = TabAppel.listAppel[ i ].StepExe;
          if ((step == 1))     //  DateExe := IncDay(DateExe,trunc(Step));
            DateExe = IncrementeDate(TabAppel.listAppel[ i ].nextExe, step
                                     , calendaire);
          //if (step >1) and (step <> 0) then  //
          if (step > 1) { // ViB le 27/02/2004
            /* TODO : Fait péter toute gestion d'un pas différent de mensuel, revoir la condition pour le 5, 10 et 15 jours) */
            //if (DayOfTheMonth(IncDay(thisDate,Trunc(Step))) > 27) then  // ViB le 01/03/2004
            {
              if (ModeDebug)
                mainForm.affiche(TimeToStr(Time) + (char)9 +
                                 "####  ---> Date: " + datetostr(thisDate) + " pas: " +
                                 floattostr(step), 0);
              //DecodeDate(EndOfTheMonth(IncDay(thisDate,Trunc(Step))),Year,Month,Day); // ViB 26/02/2004
              DecodeDate(EndOfTheMonth(thisDate), Year, Month, Day);
              ++Month; // ViB le 01/03/2004
              if (Month == 13) { // ViB le 01/03/2004
                // ViB le 01/03/2004
                Month = 1; // ViB le 01/03/2004
                ++Year; // ViB le 01/03/2004
              } // ViB le 01/03/2004
              Day = DayOfTheMonth(EndOfAMonth(Year, Month));      // ViB le 01/03/2004
              DateExe = EncodeDate(Year, Month, Day);
              //if modeDebugPrecis then mainForm.memDeroulement.Lines.add(tabAppel.listAppel[i].theProc.name+ '--> prochaine exécution: '+datetostr(DateExe)); // ViB le 01/03/2004
            }
            //          else
            /*             DateExe := IncDay(DateExe,trunc(Step));
            DecodeDate(DateExe,Year,Month,Day);
            Day := Trunc(DayOfTheMonth(DateExe)+step-1);
            If Day > 27 then Day := DayOfTheMonth(EndOfAMonth(Year,Month));
            DateExe := EncodeDate(Year,Month,Day);    */
          } // fin if (step >1) and (step <> 0) then
          TabAppel.listAppel[ i ].nextExe = DateExe;

          /*if step=0 then   //VN debug simcons
          begin
          Assign(f,'D:\init next exe.txt');
          Append(f);
          WriteLn(f, inttostr(tabAppel.listAppel[i].id)+#9+datetostr(tabAppel.listAppel[i].nextExe)+#9+datetostr(thisDate)+#9+tabAppel.listAppel[i].classe.name+#9+dbmodule1.module.lookup('CodeModule',tabAppel.listAppel[i].id,'NomCours'));
          close(f);
          end;*/

        } // fin if tabAppel.listAppel[i].nextExe = thisDate then
        //else
        //if tabAppel.listAppel[i].id=33226 then ShowMessage(dateToStr(tabAppel.listAppel[i].nextExe)+' '+dateToStr(thisDate));
      }
    } // fin for i:= 0 to length(tabAppel.listAppel)-1 do

  } catch (...) {
    AfficheMessageErreur("TSimule.CalculNextExe", UClasseSimule);
  }
}

//----------------------------------------------------------------------------//

void TSimule::Aller(int const &numSimule)
// Gère le déroullement de la simulation :
// On récupère d'abord le plus petit pas de temps de tous les modules
// Ensuite tant qu'on a pas atteint la date de fin de simulation :
//  - on execute les modules
//  - on change la date de la prochaine execution
//  - on incremente la date en cours avec le plus petit pas de temps
//  - on fait passer les bookmarks sur l'enregistrement suivant

{
  TDateTime datefin; TDateTime semis;
  int i;

  double test;
  unsigned short jour; unsigned short mois; unsigned short an; unsigned short anPrec;



  try {
    // recuperation de la date de fin de la simulation
    datefin = GetVal("FinSimul");
    //VN déplacé dans la boucle
    //semis   := GetVal('DateSemisCalc');
    anPrec = 0;

    // requete qui renvoie le plus petit pas de temps
    DbModule1.RequeteStep.Active = true;
    DbModule1.RequeteStep.First;
    minorStep = DbModule1.RequeteStepMINOFstep.Value;
    DbModule1.RequeteStep.Active = false;

    //ShowMessage(floattostr(contextObjet.GetCurrentInstance('Soil').GetVal('ArriveeEauJour')));
    //ShowMessage('###'+IntToStr(tabAppel.listappel[0].id)+' '+IntToStr(tabAppel.listappel[0].stepExe)+' '+dateToStr(tabAppel.listappel[0].nextExe)+' '+DateToStr(thisDate));

    // boucle tant que la date courante thisDate n'a pas atteind la date de fin
    while (thisDate <= datefin) {
      DecodeDate(thisDate, an, mois, jour);
      //VN contraintes
      Pars.SetVariable("Dateencours", thisDate);
      Pars.SetVariable("DebutSimul", thisDate);
      Pars.SetVariable("finsimul", datefin);
      semis = GetVal("DateSemisCalc");
      //VN contraintes

      //if contextObjet.GetCurrentInstance('Soil').GetVal('ArriveeEauJour') = 0 then ShowMessage('zero');
      if (anPrec != an)
        // si l'annee precedente est differente de l'annee courante on l'initialise comme etant égal a l'annee courante
      {
        anPrec = an;
        mainForm.Label2.Caption = FormatDateTime("hh:mm:ss", Time - Duree) +
                                  " - Num. simulation: " + inttostr(NumSimulation) + " - Simulation: " +
                                  DbModule1.Simule.findfield("Id").AsString + (char)9 + " - Année: " +
                                  inttostr(an);
      }
      //VN saveSemis
      //if  (thisDate = semis) then
      if ((contextObjet.GetCurrentInstance("Crop") == 00) && (thisDate == semis)) {       // Création de crop si on est arrivé à la date de semis on place l'instance de Crop dans le context
        //ShowMessage(DateToStr(GetVal('DateSemisCalc')));
        contextObjet.SetCurrentInstance(TEntityInstance.create("Crop", thisDate
                                                               , DbModule1.Variete.findfield("id").AsVariant));
      }

      // S'il existe une instance de type Crop <=> a partir du moment ou il y a eu le semis
      if (!(contextObjet.GetCurrentInstance("Crop") == 00)) {
        test = contextObjet.GetCurrentInstance("Crop").GetVal("NumPhase");
        //dateEnStr := FormatDateTime('dd:mm',contextObjet.GetCurrentInstance('Crop').GetVal('DateMaturite'));

        // Si le numero de la phase de l'instance de Crop est 7
        // Destruction de l'entité Crop car elle est a Maturité on place l'instance de Crop dans le context
        if (contextObjet.GetCurrentInstance("Crop").GetVal("NumPhase") == 7)
          contextObjet.FreeEntityInstance("Crop");
      } // fin : test sur existence d'une instance de Crop

      //Ajout VN simcons
      contextObjet.RunInitInstances;
      //Lance les modules d'initialisation si il faut le faire.

      //tabAppel.writeAppels(0);
      /*Assign(f,'D:\Suivi.txt');
      Append(f);
      acall:=tabAppel.listappel[5];
      WriteLn(f, inttostr(acall.stepExe)+#9+inttostr(acall.id)+#9+datetostr(acall.nextExe)+#9+datetostr(TSimule(contextObjet.GetCurrentInstance('Simulation')).thisDate)+#9+acall.classe.name+#9+dbmodule1.module.lookup('CodeModule',acall.id,'NomCours'));
      close(f);*/

      //ShowMessage(DBModule1.Resjour.FieldByName('ArriveeEauJour_out').AsString+' '+floattostr(contextObjet.GetCurrentInstance('Soil').GetVal('ArriveeEauJour'))+' '+floattostr(contextObjet.GetCurrentInstance('Plot').GetVal('ArriveeEauJour_out')));

      StepExe; // parcours du tableau et execution des modules selon leur pas de temps
      CalculNextExe; // parcours du tableau et mise à jours de la prochaine date d'execution des modules

      // Passage à l'enregistrement suivant A modifier pour adapter au pas de temps
      if (DbModule1.Forcage.RecordCount != 0) {
        {
          long i_end = length(DbModule1.DynamicTables) ; for (i = 0 ; i < i_end ; ++i)
            DbModule1.DynamicTables[ i ].Next;
        }
      }

      thisDate = thisDate + minorStep; // passage au pas de temps suivant

      //VN simcons (permet la manipulation de la fin de simul dans les modules)
      datefin = GetVal("FinSimul");

      if (thisDate != datefin) {
        //if (thisDate - Semis>0) and (thisDate - Semis<10) then ShowMessage(DateToStr(thisDate)+' '+DateToStr(semis));
        SetVal("DateEnCours", thisDate);
        //ShowMessage(DateToStr(thisDate));
        MajBookmarks(numSimule);   // Mise à jour des TAccessRecords
        SetVal("NbJAS", thisDate - semis);
      }
      //ShowMessage(DateToStr(thisDate)+' '+DateToStr(GetVal('DateEnCours')));
      Application.ProcessMessages;
      if (mainForm.butStopSimulation.Caption == "Arrêt en cours")
        thisDate = datefin + 1;
    } // fin du while thisDate <= dateFin do

    //VN save: Dans le cas de semis calculé on remet à jour la variable nbjas dans resjour
    // Finaliser le choix forcer ou calculer une date de semis....
    // Modif CB 02/06 ne fiare MajNbjAs que si date semis calculee
    if (contextObjet.GetCurrentInstance("Plot").GetVal("DateSemis") == NullValue)
      // Modif CB 02/06
      majNbJas(GetVal("DateSemisCalc"), GetVal("DebutSimul"), datefin - 1);

  } catch (Exception const &e)
  {AfficheMessageErreur(e.message + "TSimule.Aller", UClasseSimule);}

}

void TSimule::majNbJas(TDateTime dateSemis, TDateTime dateDebut, TDateTime datefin)

//Met à jour la variable nbJas pour l'années simulée
//Utile surtout aprés un calcul automatique de la date de semis
{
  try {
    DbModule1.queGenerique.Close;
    DbModule1.queGenerique.SQL.Clear;
    DbModule1.queGenerique.SQL.Add("Update \':DBResult:Resjour.db\' Set nbJas = jour-\'" + FormatDateTime("mm/dd/yyyy", dateSemis) + "\' where jour>=\'" +
                                   FormatDateTime("mm/dd/yyyy", dateDebut) + "\'and jour<=\'" +
                                   FormatDateTime("mm/dd/yyyy", datefin) + "\';");
    DbModule1.queGenerique.ExecSQL;

  } catch (Exception const &e)
  {AfficheMessageErreur("setNbJas " + e.Message, UClasseSimule);}

}



void SaveSimulation(double &DoSave,   double const &CodeSave) {
  int index;



  try {
    if (DoSave == 1) {
      //ShowMessage('save '+DateToStr(CodeSave)+' '+DateToStr(aSimulation.thisDate));
      DoSave = 0;
      aSimulation.SetVal("DoSave", 0);
      //Avant de sauvegarder on remet la demande de sauvegarde  à 0

      index = aSimulation.ContextSave.IndexOf(floattostr(CodeSave));
      if (index >= 0) {
        TContextSave(aSimulation.ContextSave.Objects[ index ]).free;
        aSimulation.ContextSave.Delete(index);
        TTabAppelsSave(aSimulation.TabAppelSave.Objects[ index ]).free;
        aSimulation.TabAppelSave.Delete(index);
      }
      //On efface une éventuelle sauvegarde qui aurait le même code

      aSimulation.ContextSave.AddObject(floattostr(CodeSave)
                                        , TContextSave.create(contextObjet));
      aSimulation.TabAppelSave.AddObject(floattostr(CodeSave)
                                         , TTabAppelsSave.create(TabAppel));
      //Création des sauvegardes à partir des objets à sauvegarder
    }

  } catch (Exception const &e)
  {AfficheMessageErreur("SaveSimulation " + e.Message, UClasseSimule);}

}

void LoadSimulation(double &DoLoad, double &nbLoaded,   double const &CodeSave) {
  int index;
  TContextSave saveCtxte;
  TTabAppelsSave saveTabappel;

  try {
    if (DoLoad == 1) {
      //ShowMessage('load '+DateToStr(CodeSave)+' '+DateToStr(aSimulation.thisDate));
      index = aSimulation.ContextSave.IndexOf(floattostr(CodeSave));
      if (index >= 0) {
        //ShowMessage(DateToStr(aSimulation.thisdate));
        saveCtxte = TContextSave(aSimulation.ContextSave.Objects[ index ]);
        //on récupere la sauvegarde du Tcontext qui correspond au code de sauvegarde

        saveCtxte.UseSave(contextObjet);
        //on utilise cette sauvegarde pour écrase les valeurs du contexte en cours

        saveTabappel = TTabAppelsSave(aSimulation.TabAppelSave.Objects[ index ]);
        //on récupere la sauvegarde du TtabAppel qui correspond au code de sauvegarde

        saveTabappel.UseSave(TabAppel);
        //on utilise cette sauvegarde pour écrase les valeurs du contexte en cours

        aSimulation.thisDate = aSimulation.GetVal("DATEENCOURS");
        //on met à jour la variable indépendant des TentityInstance sujette à modification

        aSimulation.Pars.SetVariable("DATEENCOURS"
                                     , aSimulation.GetVal("DATEENCOURS"));
        //maj de la date en cours du parser

        nbLoaded = nbLoaded + 1;
        //on incrémente le nombre de chargement effectué
        //ShowMessage(DateToStr(aSimulation.thisdate));
      }
      //else AfficheMessageErreur('LoadSimulation | sauvegarde '+ FloatToStr(CodeSave) +'inexistante',UClasseSimule);

      DoLoad = 0;
      //on efface la demande de chargement pour ne pas recharger

    } else
      nbLoaded = 0;

  } catch (Exception const &e)
  {AfficheMessageErreur("LoadSimulation " + e.Message, UClasseSimule);}

}

//calcule la date de semis et déclanche la sauvegarde ou le chargement

void CalcSemisAuto(double &nbJStress, double &DoLoad, double &DoSave, double &dateSemisCalc,
                   double const &nbJTestSemis, double const &StressResistance, double const &seuilStress, double const &Cstr, double const &dateEnCours, double const &SeuilEauSemis, double const &eauDispo) {
  try {
    //Si la plante n'existe pas et que le seuil de pluie est atteint
    //alors on lance la sauvegarde et on initialise la date de semis
    // Nota permet
    // des semis consécutifs sur une longue saison pluvieuse cas Benin cote d'ivoire

    if ((contextObjet.GetCurrentInstance("Crop") == 00)) {

      if ((eauDispo > SeuilEauSemis)) {
        DoSave = 1;
        dateSemisCalc = dateEnCours;
        nbJStress = 0;
      }
    } else {
      if ((dateEnCours < dateSemisCalc + nbJTestSemis)) {
        if ((Cstr < seuilStress))
          nbJStress = nbJStress + 1;
        if (nbJStress > StressResistance) {
          DoLoad = 1;
        }
      }
      // A resoudre MAjNbJAS  si test Ok... Fait dans proc TSimule.Aller
      //    if (DateEnCours=DateSemisCalc+nbJTestSemis) then majNbJas(DateEnCours);
      //Si on a trouvé la bonne date de semis on met à jour les nbjas
    }


  } catch (Exception const &e)
  {AfficheMessageErreur("CalcSemisAuto " + e.Message, UClasseSimule);}

}

//seme si la date de Semis est égale à la date courante

void Seme(double const &dateSemisCalc, double const &dateEnCours) {
  try {
    if (dateSemisCalc == dateEnCours)
      //si on est à une date de semis alors on seme
    {
      contextObjet.SetCurrentInstance(TEntityInstance.create("Crop"
                                                             , dateEnCours, DbModule1.Variete.findfield("id").AsVariant));
      //showMessage(DateToStr(DateSemisCalc));
    }

  } catch (Exception const &e)
  {AfficheMessageErreur("Seme " + e.Message, UClasseSimule);}

}



//END
