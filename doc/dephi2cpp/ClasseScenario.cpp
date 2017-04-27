/**   @file  
     @brief  
*/
#include "ClasseScenario.h"



 
#include "ModelsManage.h" 
#include "DBModule.h" 
#include "GestionDesTables.h" 
#include "lectdonnee.h" 
#include "main.h"
  
#include "GestionDesErreurs.h"
#include "Parser10.h" 
#include "Forms.h" 
#include "Classes.h" 
#include "DateUtils.h" 
#include "requetes.h"
  
#include "DeriveClimato.h"


/*--------------------------------------------------------------------------
  Procédure permettant de lancer des simulations consecutives.

  @Author  CIRAD
  @Version v_3.0
  @param  idSimule  id de la simulation
  @param  numSimule numéro de la simulation
---------------------------------------------------------------------------*/
void TScenario::LanceSimulationsConsecutives(std::string idSimule,  int numSimule)
{
  int i; int j;   /// variable de boucle
  TEntityClass aClass;  /// une TEntityClass
  

  try {
/*?*//*$IFDEF AffichageComplet*/
    MainForm.affiche( TimeToStr( Time ) + (char)9 +
      "TScenario.LanceSimulationsConsecutives codeSimule : " +
      FloatToStr( codeSimule ) + " numSimule : " + FloatToStr( numSimule ), 0 );
/*?*//*$ENDIF*/
    MainForm.affiche( TimeToStr( Time ) + (char)9 + "Simulation " +
      ( idSimule ), 0 );

    tabProc.ChargeDll;
    //Chargement des dll
    tabClass.ajouter( "Simulation" );
    aClass = tabClass.Trouver( "Simulation" );
    aClass.AttributeFromTable( "Simulation" );
    //Création de la classe Simulation

    tabClass.CreationClasses;
    //création des autres classes

    SetTabAppels( idSimule , 0 );
    //Crée les tabAppel de chaque simulation consécutive
    currentTabAppel = 0;

    j = Trunc( DBModule1.Simule.findfield( "NbAnSim" ).Value ) - 1;

    { long i_end = j+1 ; for( i = 0 ; i < i_end ; ++i ) // tant qu'on a pas atteint la derniere année
    {
      Application.ProcessMessages;
      if(  MainForm.butStopSimulation.Caption != "Arrêt en cours" )
      {
        //Modif CB 15/12/06   Attention verifier effet sur l'optimisation Numsimulation utilisée
        Numsimulation = i + 1;
        //Modif CB 15/12/06
        finLastSimul = 0;
        execSimul( idSimule , numSimule , i );
        //Execution de la suite de simulation pour la première année ?? Nil n'est pas une destruction
        aSimulation = 00;

      }
    }}

    //MainForm.memDeroulement.Lines.Add('Simulation '+(idSimule)+' achevée à '+TimeToStr(Time));
  
    } catch(...) {AfficheMessageErreur( "TScenario.LanceSimulationConsecutives | Simulation " +
      aSimulation.name , UClasseScenario );
  }
}

/*--------------------------------------------------------------------------
  Procédure execSimule

  @Author  CIRAD
  @Version v_3.0
  @param  idSimule  id de la simulation
  @param  numSimule numéro de la simulation
  @param  NumAnnSimule numéro de l'année simulée
---------------------------------------------------------------------------*/
void TScenario::execSimul( std::string const& idSimule,  int numSimule, 
  int NumAnnSimule)
{
  int i; /// entier
  unsigned short an; ///année
  unsigned short mois; /// mois
  unsigned short jour; /// jour
  TDateTime dateDebRel;  /// Date

  // Ré appel de excsimul pour les cultures consécutives
  try {

    if(  finLastSimul != 0 )
    {
      DBModule1.Simule.Locate( "Id" , idSimule , [ loCaseInsensitive ] );
      DecodeDate( finLastSimul , an , mois , jour );
      DBModule1.Simule.Edit;
      // Modif CB 23/11/06 attention années consécutives! et on écrase la définition de debutsimul!!
      // placer cela plus bas avec DateDebRel
      DBModule1.Simule.FieldByName( "DebutSimul" ).Value =
        EncodeDate( yearof( DBModule1.Simule.FieldByName( "DebutSimul" ).AsDateTime )
        , mois , jour );
      DBModule1.Simule.Post;
    }

    if(  DBLocateSimule( idSimule , numSimule , NumAnnSimule ) )
      //se positionne sur les table et Ecrase dans les tables Resjour et Rescum les anciens resultats de la simulation
      //en cours pour la première simulation consécutive de la première année seulement
      //(ex: lors d'un scenario d'optimisation, +ieurs simulations consécutives peuvent être executées sur plusieurs années,
      //seule la 1ere fait le nettoyage préalable)
    {

      DecodeDate( DBModule1.Simule.findfield( "DebutSimul" ).Value , an , mois
        , jour );  
       
      dateDebRel = EncodeDate( an + NumAnnSimule , mois , jour );

      //on prend le tabAppel correspondant
      tabAppel = tabAppelConsecutif[ currentTabAppel ];

      if(  !( ( finLastSimul != 0 ) || ( NumAnnSimule == 0 ) ) )
        contextObjet.ResetContextInstance( dateDebRel , i );

      //Si la simulation courante est différente de la simulation qui était la avant on la remplace
      //(effectif lors de simulations consécutives)
      if(  ( aSimulation == 00 ) )
      {
        //Si il n'y avait pas de simulation (premiere execution) on crée l'instance Simulation
        aSimulation = TSimule.Create( "Simulation" , dateDebRel , idSimule );
        contextObjet.SetCurrentInstance( aSimulation );
      }
      else if(  ( aSimulation.name != idSimule ) )
      {
        //Si la simulation qui existait est diférente de celle en cours alors on la remplce
        //(cas de simulations consécutives)
        aSimulation = TSimule.Create( "Simulation" , dateDebRel , idSimule );
        contextObjet.GetCurrentInstance( "Simulation" ).Free;
        contextObjet.SetCurrentInstance( aSimulation );
      }

      aSimulation.InitializeCurrentInstance;
      //Lance les procedures d'initialisation de la simulation (module 0 entre autres)

      aSimulation.InitDatesSimule( NumAnnSimule );
      //Maj des dates de la simulation fonction du nb d'années déjà simulées.

      aSimulation.Pars = Parser10.Tparser.Create( 00 );
      aSimulation.ContextSave = TStringList.Create;
      aSimulation.TabAppelSave = TStringList.Create;

      //ShowMessage(' 0 '+DBModule1.Resjour.FieldByName('jour').AsString);
      if(  ( finLastSimul != 0 ) || ( NumAnnSimule == 0 ) )
        contextObjet.GereCreationInstances( dateDebRel );
      //ShowMessage(' 3 '+DBModule1.Resjour.FieldByName('jour').AsString);
    //Crée les instance si elles n'existent pas, les reset si elles sont différentes, ne fait rien si ce sont les mêmes
    //Et pour les simulation consécutives,
    //Utilise l'enregistrement sauvegardé sous l'id "$$idtmp$$" pour les tables en écritures comme enregistrement de départ

      aSimulation.InitDatesSemis( NumAnnSimule );
      //Maj dates de Semis en fonction du nb d'années déjà simulées.

      aSimulation.Aller( NumAnnSimule );
      //execution de la simulation unique

      finLastSimul = aSimulation.thisDate - 2;
      //DBModule1.Simule.Edit;
      //DBModule1.Simule.FieldByName('FinSimul').Value:=aSimulation.thisDate;
      //DBModule1.Simule.post;
      //ShowMessage(DateToStr(aSimulation.thisDate));

      CleanTable( DBModule1.Resjour , aSimulation.thisDate );
      //CleanTable(DBModule1.ResCum,aSimulation.thisDate);

      currentTabAppel = ( currentTabAppel + 1 ) % length( tabAppelConsecutif );
      //on passe au modèle suivant si il y en as et on boucle dans le cas de simulations pluriannuelles

      aSimulation.Clean;
      //on nettoie ContextSave, TabAppelSave, Pars

      //  MainForm.memDeroulement.Lines.Add((idSimule)+' année '+IntToStr(NumAnnSimule+1)+' finie... '+TimeToStr(Time));

      if(  !DBModule1.Simule.FieldByName( "IdNextSimule" ).IsNull )
        //on execute la simulation consécutive si il y en a une
      {
        contextObjet.prepareNextSimule( aSimulation.thisDate );
        //On recopie chaque enregistrement de toutes les tables en écriture
        //afin de garder les valeurs de ces dernières pour la simulation suivante
        //Cet enregistrement est sauvegardé sous l'id "$$idtmp$$" dans la table correspondante

        execSimul( DBModule1.Simule.FieldByName( "IdNextSimule" ).AsString
          , numSimule , NumAnnSimule );
      }
    }
    else
    {
      MainForm.affiche( TimeToStr( Time ) + (char)9 +
        "Plantage DBLocateSimule(codeSimule,numSimule) dans ClasseScenario.TScenario.execSimul", 0 );
      main.erreur = true;
    }
  
    }
   catch( exception const& e )
       {AfficheMessageErreur( e.message + " TScenario.execSimul | idSimule : " +
        ( idSimule ) + " numSimule : " + FloatToStr( numSimule ) , UClasseScenario );}
  
}


/*--------------------------------------------------------------------------
  Procédure appelée par ExecuterScenario, lancera les simulations pour
  l'optimisation, le forçage et l'exécution standard.
  Execution d'une simulation :
  On créé d'abord la classe simulation puis on créé l'objet simulation,
  on lance ensuite sa procédure bilanparcelle
  @Author  CIRAD
  @Version v_3.0
  @param  idSimule  id de la simulation
  @param  numSimule numéro de la simulation
---------------------------------------------------------------------------*/
void TScenario::ExecuterSimulation(std::string idSimule,  int numSimule)
// Execution d'une simulation :
// On créé d'abord la classe simulation puis on créé l'objet simulation,
// on lance ensuite sa procédure bilanparcelle
{

  TEntityClass aClass; /// une TEntityClass

  try {
    MainForm.affiche( TimeToStr( Time ) + (char)9 + "Simulation " +
      ( idSimule ), 0 );

    //     Duree := Time;

    if(  DBLocateSimule( idSimule , numSimule , 0 ) )
    {
      /* TODO -ofred : dans le diag de classe 'ajouter' est une function, cela serait plus simple ? */
      // Création de la classe simulation
      tabClass.ajouter( "Simulation" );
      aClass = tabClass.Trouver( "Simulation" );
      aClass.AttributeFromTable( "Simulation" );

      // création de l'instance simulation
      aSimulation = TSimule.Create( "Simulation" , date , idSimule );
      //initialisation du parser
      /*Ajout VN contraintes*/aSimulation.Pars = Parser10.Tparser.Create( 00 );
      /*Ajout VN contraintes*/aSimulation.TabAppelSave = TStringList.Create;
      /*Ajout VN contraintes*/aSimulation.ContextSave = TStringList.Create;

      // Enregistrement de celui-ci dans le TContext
      contextObjet.SetCurrentInstance( aSimulation );
      tabProc.ChargeDll;
      // lancement de la simulation
      aSimulation.BilanParcelle( idSimule );

      /*Ajout VN contraintes*/aSimulation.Pars.Destroy;
      /*Ajout VN contraintes*/aSimulation.TabAppelSave.Destroy;
      /*Ajout VN contraintes*/aSimulation.ContextSave.Destroy;

      //MainForm.memDeroulement.Lines.Add('Simulation '+(idSimule)+' achevée à '+TimeToStr(Time));
    }
    else
    {
      MainForm.affiche( TimeToStr( Time ) + (char)9 +
        "Plantage DBLocateSimule(codeSimule,numSimule) dans ClasseScenario.TScenario.ExecuterSimulation", 0 );
        main.erreur = true;
    }
  
    } catch(...) {AfficheMessageErreur( "TScenario.ExecuterSimulation | idSimule : " +
      ( idSimule ) + " numSimule : " + FloatToStr( numSimule ) , UClasseScenario );
  }
}

/**--------------------------------------------------------------------------
  Procédure remplirTableForcage
  @Author  CIRAD
  @Version v_3.0
---------------------------------------------------------------------------*/
void remplirTableForcage()
{
  std::string descrForcage; ///  descrForcage
  std::string param;  /// param
  int MonIndice;///indice
  int MaPosition; /// position
  int MaPos;  /// pos

  descrForcage = DBModule1.Simule.findfield( "DescriptionForcage" ).AsString;

  viderTable( DBModule1.Forcage );

  MonIndice = 0;
  MaPosition = Pos( ";" , descrForcage );
  while(  MaPosition >= 0 )
  {
    if(  ( MaPosition > 0 ) )
    {
      param = LeftStr( descrForcage , MaPosition - 1 );
    }
    else
    {
      param = descrForcage;
    }

    // récupération du codeVar à partir du nomVar dans modeleVar
    MaPos = Pos( "=" , descrForcage );
    DBModule1.ModeleVar.Locate( "NomVar" , LeftStr( param , MaPos - 1 ) , [  ] );

    param = Copy( param , MaPos + 1 , length( param ) );

    //viderTable(dbModule1.Forcage);
    DBModule1.Forcage.Insert;
    //Ad          DBModule1.Forcage.FindField('CodeVar').Value:= DBModule1.ModeleVar.findfield('CodeVar').Value;
    DBModule1.Forcage.findfield( "IdVar" ).Value = DBModule1.ModeleVar.findfield( "NomVar" ).Value;
    DBModule1.Forcage.findfield( "Valeur" ).Value = param;
    DBModule1.Forcage.Post;

    descrForcage = Copy( descrForcage , MaPosition + 1 , length( descrForcage ) );
    ++MonIndice;
    if(  ( MaPosition == 0 ) )
    {
      MaPosition = -1;
    }
    else
    {
      MaPosition = Pos( ";" , descrForcage );
    }
  }
}

/*--------------------------------------------------------------------------
  Procédure permettant d'exécuter les simulations du scénario suivant un
  type d'exécution (pré-positionne également les pointeurs sur les
  tables utilisées dans les simulations)
  @Author  CIRAD
  @Version v_3.0
  @param  typeExe 0 = une exécution normale, sans forçage 1= correspond à
  une exécution avec forçage 2=correspond à une exécution avec etude de
  sensibilité  3=correspond à une exécution avec optimisation de paramètres
---------------------------------------------------------------------------*/
void TScenario::ExecuterScenario(int typeExe)
// Execution du Scenario :
// Ici on va gérer l'execution de toutes les simulations sélectionnées.
// On fait les tests pour remplir la table ModeleVar et appliquer le
// forçage. la premiere simulation est traitée hors de la boucle
{

  int cptSimul; /// compteur de simulation
  int i; /// indice
  std::string codeModelePrec; /// code modele précédent
  std::string NextSim;  /// simulation suivante
  TDatasource monMaitre; /// source de donnée maitre
  TDatasource monMaitre1; /// source de donnée maitre1
  TDatasource monMasterSourcePluvio;/// source de donnée pluvio
  TDatasource monMasterSourceMeteo; /// source de donnée meteo
  TDatasource monMasterSourcePersonalData; /// source de donnée personnelle

  std::string maitreField; ///champ
  std::string maitreField1;///champ
  std::string MonMasterFieldPluvio; ///champ
  std::string MonMasterFieldMeteo;///champ
  std::string MonMasterFieldPersonalData;///champ

  std::string MonTexte; /// texte


  try {
/*?*//*$IFDEF AffichageComplet*/
    //MainForm.memDeroulement.Lines.Add('TScenario.ExecuterScenario typeExe : '+FloatToStr(typeExe));
/*?*//*$ENDIF*/
    //Ajout VN
    aSimulation = 00;


    monMasterSourceMeteo = DBModule1.Meteorologie.MasterSource;
    MonMasterFieldMeteo = DBModule1.Meteorologie.MasterFields;


    monMasterSourcePluvio = DBModule1.Pluviometrie.MasterSource;
    MonMasterFieldPluvio = DBModule1.Pluviometrie.MasterFields;
    monMasterSourcePersonalData = DBModule1.PersonalData.MasterSource;
    MonMasterFieldPersonalData = DBModule1.PersonalData.MasterFields;
    monMaitre = DBModule1.Station.MasterSource;
    maitreField = DBModule1.Station.MasterFields;
    monMaitre1 = DBModule1.Pays.MasterSource;
    maitreField1 = DBModule1.Pays.MasterFields;


    DBModule1.Meteorologie.MasterSource = 00;


    DBModule1.Pluviometrie.MasterSource = 00;
    DBModule1.PersonalData.MasterSource = 00;
    DBModule1.Station.MasterSource = 00;
    DBModule1.Pays.MasterSource = 00;



    DBModule1.Meteorologie.MasterFields = "";
    DBModule1.Meteorologie.Filter = "";
    DBModule1.Meteorologie.Filtered = false;

    DBModule1.Pluviometrie.MasterFields = "";
    DBModule1.Pluviometrie.Filter = "";
    DBModule1.Pluviometrie.Filtered = false;
    
    DBModule1.PersonalData.MasterFields = "";
    DBModule1.PersonalData.Filter = "";
    DBModule1.PersonalData.Filtered = false;
    DBModule1.Pays.MasterFields = "";
    DBModule1.Pays.Filter = "";
    DBModule1.Pays.Filtered = false;
    DBModule1.Station.MasterFields = "";
    DBModule1.Station.Filter = "";
    DBModule1.Station.Filtered = false;

    /* TODO : Retirer les filtres ? potentiels sur les tables Meteo et pluvio*/
    // classement des simulations suivant le codeModele
    DBModule1.RequeteSimule.Active = true;
    // on s'assure qu'on est bien sur le premier enregistrement
    DBModule1.RequeteSimule.First;
    // on initilise le premier Code module
    codeModelePrec = "Init"; //AD premier code module ou modèle ???
    typeSimul = typeExe;

    // le type 0 correspond à une exécution normale, sans forçage
    // le type 1 correspond à une exécution avec forçage
    // le type 2 correspond à une exécution avec etude de sensibilité
    // le type 3 correspond à une exécution avec optimisation de paramètres
     switch( typeExe ) {
      case 0:  case 1: 
        // -------------- Scénario simple ou forçage
        {
          GestionDesTables.Numsimulation = 0;

         initTemp;

          // de la première simulation jusqu'à la derniere
          { long cptSimul_end = DBModule1.RequeteSimule.RecordCount ; for( cptSimul = 0 ; cptSimul < cptSimul_end ; ++cptSimul )
          {
            /*VN simcons*/
            DBModule1.Simule.Locate( "Id"
              , DBModule1.RequeteSimule.findfield( "IdSimule" ).Value , [  ] );
          ////////////////////////////////////////////////////////////////////
            viderTable( DBModule1.tempPluviometrie );
            viderTable( DBModule1.tempMeteorologie );

            DBModule1.Pluviometrie.Filter = "CodeStation=\'" +
              vartostr( DBModule1.Site.Lookup( "Id" , DBModule1.SimuleIdSite.AsString
              , "CodeStationPluie" ) + "\'" );
            DBModule1.Pluviometrie.Filter = DBModule1.Pluviometrie.Filter +
              " AND Jour >= " +
              QuotedStr( DateToStr( DBModule1.SimuleDebutsimul.AsDateTime ) );
            DBModule1.Pluviometrie.Filter = DBModule1.Pluviometrie.Filter +
              " AND Jour <= " +
              QuotedStr( DateToStr( EncodeDate( DBModule1.SimuleAnFinSimul.AsInteger
              , 12 , 31 ) ) );
            DBModule1.Pluviometrie.Filtered = true;
          ////////////////////////////////////////////////////////////////////



            //AlimenteTableClimatoPluviometrie;
         ////////////////////////////////////////////////////////////////////
            AlimenteTableTempPluviometrie;


            DBModule1.Meteorologie.Filter = "CodeStation=\'" +
              vartostr( DBModule1.Site.Lookup( "Id" , DBModule1.SimuleIdSite.AsString
              , "CodeStationMeteo" ) + "\'" );
            DBModule1.Meteorologie.Filter = DBModule1.Meteorologie.Filter +
              " AND Jour >= " +
              QuotedStr( DateToStr( DBModule1.SimuleDebutsimul.AsDateTime ) );
            DBModule1.Meteorologie.Filter = DBModule1.Meteorologie.Filter +
              " AND Jour <= " +
              QuotedStr( DateToStr( EncodeDate( DBModule1.SimuleAnFinSimul.AsInteger
              , 12 , 31 ) ) );
            DBModule1.Meteorologie.Filtered = true;
            DBModule1.PersonalData.Filter = DBModule1.Meteorologie.Filter;


            DBModule1.PersonalData.Filtered = true;


            AlimenteTableTempMeteorologie;
          
            Application.ProcessMessages;

            DBModule1.PersonalData.Filter = "";
            DBModule1.PersonalData.Filtered = false;

            //DeriveMeteoPluvio; // ViB le 14/01/2008 retrait  pour  les deux dérivation Temppluvio et TempMeteo

            DeriveTableTempMeteoTempPluvio;
            
            //Fin de Derivation de la Climato

            DBModule1.Pluviometrie.Filter = "";
            DBModule1.Pluviometrie.Filtered = false;

            DBModule1.Meteorologie.Filter = "";
            DBModule1.Meteorologie.Filtered = false;
           
            // si le codeModele est différent de celui de la simulation precedente, on construit la table des variables
            if(  DBModule1.RequeteSimuleIdModele.Value != codeModelePrec )
            {
              // appel de la méthode de placement sur toute les tables : DBLocateSimule
              // mise à jour du Code precedent
              codeModelePrec = DBModule1.RequeteSimuleIdModele.Value;
              // positionnement sur la table Modele et donc ModeleModule pour remplir ModeleVar
              DBModule1.Modele.Locate( "Id" , codeModelePrec , [ loCaseInsensitive ] );
                /* TODO : On définit déjà la position du curseur de la table modele dans DBLocateSimule est-ce donc utile ici? */
              // vider la table ModeleVar
              viderTable( DBModule1.ModeleVar );
              // remplir la table ModeleVar en fonction des variables utilisées

              /*VN simcons*///RemplirTableModeleVar(dBModule1.ModeleId.value);
              RemplirTableModeleVarSimule( DBModule1.Simule.findfield( "Id" ).Value );

              RemplirTableModeleVar( "Init" );

              if(  typeExe == 1 )
                ForcerTable; // si mode Forçage, alors on subtitue les valeurs de la table Forçage
            } // fin if DBModule1.RequeteSimuleIdModele.Value <> CodeModelePrec then

            // Au debut de chaque simulation, création des différents objets
            tabAppel = TTabAppels.Create(  );
            tabAppelIni = TTabAppels.Create(  );
            tabClass = TTabClasses.Create(  );
            contextObjet = TContext.Create(  );

            DBModule1.Simule.Locate( "Id"
              , DBModule1.RequeteSimule.findfield( "IdSimule" ).Value , [  ] );

            // si y'a une description de forçage, il faut remplir la table forcage
            if(  ( DBModule1.Simule.findfield( "DescriptionForcage" ).AsString != "" ) )
            {
              remplirTableForcage;
              ForcerTable;
            }
            /*ajout AD*/
            
            //showmessage('avant on supprime les ancien enregistrements');

            // Modif CB 30/01/07 on supprime ici les résultats de simulations
            NextSim = DBModule1.Simule.findfield( "Id" ).Value;
            SupprEnreg( DBModule1.Resjour , "IdSimule" , NextSim );
       

            NextSim = DBModule1.Simule.findfield( "IdNextSimule" ).AsString;

            //showmessage('nextsim ok');

            if(  NextSim != "" )
            {
              SupprEnreg( DBModule1.Resjour , "IdSimule" , NextSim );

            }
            // Modif CB 30/01/07 on supprime ici les résultats de simulations

            

            /*modif APas requete*/
            DeriveResJour(  );


            // Execution de la simulation
            /*VN simcons*///ExecuterSimulation(DBModule1.RequeteSimuleIdSimule.Value,0);
            LanceSimulationsConsecutives( DBModule1.RequeteSimuleIdSimule.Value
              , 0 );

            /*modif APas requete*/
            execRequeteScenar( cptSimul );

            DBModule1.ResTemp.Close;
            DBModule1.ResTemp.EmptyTable;
            DBModule1.ResTemp.Open;

            /*fin modif APas requete*/

             /*VN simcons*/
            tabAppelIni.Free;

            tabAppel.Free;

            contextObjet.Free;


            //ViB 03/10/2007 DeriveClimato
            ////////////////////////////////////////////////////////////////////
            if( ( ParamCount == 0 ) )
            {
              EnleveDeriveMeteoPluvioMoteurTxt;
            }
            else
            {
              EnleveDeriveMeteoPluvio;
            }
             //vb30012008
            ////////////////////////////////////////////////////////////////////
            // passage à la simulation suivante
            DBModule1.RequeteSimule.Next;
            aSimulation = 00; // edit VN
          }}
          DBModule1.RequeteSimule.Active = false;
        }

      
      case 2:  /* TODO -ofred : ce qui suit est un labyrinthe pour stagiaire :-) */
        // -------------- méthodes d'optimisation
        {
          MainForm.affiche( TimeToStr( Time ) + (char)9 +
            "Scénario en cours: Optimisation de paramètres", 0 );
          // CONTRAINTES :
           // 1) Numsimulation géré en externe dans ce cas
           // sous unit Communication TMonOle.Bilan incrementation a chaque appel externe
           // ex avec R. A la création du main on initialise à 0. Pb on ne peut pas fermer
           // qd appel externe
           // 2) Dans le cas d'une optimisation il ne peut y avoir qu'un seul Modele
           // Modif cb
           /*         if NumSimulation = 0
                      then NumSimulation := 1;
           */
            // A tester on bascule sur Result dans DBAnalyse
            // Modif cb
          DBModule1.Resjour.Active = false;
          DBModule1.Resjour.DatabaseName = "DBAnalyse";
          DBModule1.Resjour.TableName = "Resjour";
          DBModule1.Resjour.Active = true;

          if(  Numsimulation == 0 )
          {
            // mise à jour du Code precedent
            codeModelePrec = DBModule1.RequeteSimuleIdModele.Value;
            DBModule1.Modele.Locate( "Id" , codeModelePrec , [ loCaseInsensitive ] );
            viderTable( DBModule1.ModeleVar );
            RemplirTableModeleVarSimule( DBModule1.Simule.findfield( "Id" ).Value );
            RemplirTableModeleVar( "Init" );
            Numsimulation = 1;
          }

          typeSimul = 3;
          // Mise à jour de la table forcage par rapport au fichier de R
          MajForcageFichier( CheminSarraH + "\\ValParamOut.txt" );
          // Forcage de la table modeleVar
          ForcerTable;

          // execution des simulations : CONTRAINTE elles ont toutes le même code modele.
          { long cptSimul_end = DBModule1.RequeteSimule.RecordCount ; for( cptSimul = 0 ; cptSimul < cptSimul_end ; ++cptSimul )
          {
            /*VN simcons*/DBModule1.Simule.Locate( "Id"
              , DBModule1.RequeteSimule.findfield( "IdSimule" ).Value , [  ] );

         // Au debut de chaque simulation, création des différents objets
            tabAppel = TTabAppels.Create(  );
            /*VN simcons*/tabAppelIni = TTabAppels.Create(  );
            tabClass = TTabClasses.Create(  );
            contextObjet = TContext.Create(  );

            LanceSimulationsConsecutives( DBModule1.RequeteSimuleIdSimule.Value
              , 0 );
                

             /*VN simcons*/tabAppelIni.Free;
            tabAppel.Free;
            contextObjet.Free;
            // passage à la simulation suivante
            DBModule1.RequeteSimule.Next;
          }}
          DBModule1.RequeteSimule.Active = false;
          // fin des simulations, requete de selection des valeurs simulées sélectionnées.
          RequeteResultat( 2 );

          //on genère alors un fichiers avec ces valeurs.
          GenereFichierResult( CheminSarraH + "\\ValSim.txt" , 2 );
          MainForm.affiche( TimeToStr( Time ) + (char)9 +
            "TScenario.ExecuterScenario: GenereFichierResult de ValSim.txt effectué", 0 );

          viderTable( DBModule1.Resjour );
          //DBModule1.Resjour.EmptyTable;   //ViB le 15/07/2004
          DBModule1.Resjour.Active = false;
          DBModule1.Resjour.DatabaseName = "DBResult";
          DBModule1.Resjour.TableName = "Resjour";
          DBModule1.Resjour.Active = true;
         
          MainForm.affiche( "", 0 );
          DBModule1.queGenerique.Active = false;
          DBModule1.queGenerique.SQL.Clear;
          DBModule1.queGenerique.SQL.Add( "SELECT DISTINCT DV.NOMCOURS as Nom, F.Valeur as Valeur FROM \":DBModele:Forcage.DB\" F, \":DBModele:DictionnaireVar.DB\" DV WHERE DV.CODEVAR = F.CodeVar ORDER BY DV.NOMCOURS, F.Valeur" );
          DBModule1.queGenerique.Active = true;
          DBModule1.queGenerique.First;
          { long i_end = DBModule1.queGenerique.RecordCount ; for( i = 0 ; i < i_end ; ++i )
          {
            if(  length( DBModule1.queGenerique.Fields.Fields[ 0 ].AsString ) >= 9 )
              MonTexte = DBModule1.queGenerique.Fields.Fields[ 0 ].AsString + (char)9 +
                " valeur trouvée: " +
                FloatToStr( DBModule1.queGenerique.Fields.Fields[ 1 ].AsFloat );else
              MonTexte = DBModule1.queGenerique.Fields.Fields[ 0 ].AsString + (char)9
                + (char)9 + " valeur trouvée: " +
                FloatToStr( DBModule1.queGenerique.Fields.Fields[ 1 ].AsFloat );
            MainForm.affiche( TimeToStr( Time ) + (char)9 + MonTexte, 0 );
            DBModule1.queGenerique.Next;
          }}
          DBModule1.queGenerique.SQL.Clear;
          DBModule1.queGenerique.Close;
          // fin du test de récup...


        }

    }

    DBModule1.Station.MasterSource = monMaitre;
    DBModule1.Pays.MasterSource = monMaitre1;
    DBModule1.Pays.MasterFields = maitreField1;
    DBModule1.Station.MasterFields = maitreField;

    DBModule1.Meteorologie.MasterSource = monMasterSourceMeteo;

    DBModule1.Pluviometrie.MasterSource = monMasterSourcePluvio;

    DBModule1.Meteorologie.MasterFields = MonMasterFieldMeteo;
  
    DBModule1.Pluviometrie.MasterFields = MonMasterFieldPluvio;
    DBModule1.PersonalData.MasterSource = monMasterSourcePersonalData;
    DBModule1.PersonalData.MasterFields = MonMasterFieldPersonalData;
  
    } catch(...) {AfficheMessageErreur( "TScenario.ExecuterScenario | typeExe : " +
      IntToStr( typeExe ) , UClasseScenario );
  }
}

/*--------------------------------------------------------------------------
  Procédure appelée par ExecuterScenario, pour lancer les simulations
  de type étude de sensibilité.
  Gestion du lancement des simulations de type étude de sensibilité
  Cette procédure est une procédure récursive, les valeurs à incrémenter
  sont placées dans un tableau, la simulation est alors lancée pour
  chaque suite de valeurs possibles.

  @Author  CIRAD
  @Version v_3.0
  @param  indiceTab indice
---------------------------------------------------------------------------*/

void TScenario::Sensibilite(signed char indiceTab)
// Gestion du lancement des simulations de type étude de sensibilité
// Cette procédure est une procédure récursive, les valeurs à incrémenter sont
// placées dans un tableau, la simulation est alors lancée pour chaque suite
// de valeurs possibles.
{
  try {
    // si on est à la fin du tableau
    if(  ( indiceTab != length( tabSensibilite ) - 1 ) && ( indiceTab <=
      length( tabSensibilite ) - 1 ) )
    {
      // tant que la valeur n'est pas égale à la valeur à la valeur maximum
      while(  tabSensibilite[ indiceTab , 0 ] <= tabSensibilite[ indiceTab , 1 ] )
      {
        // rappel de la procedure avec l'element suivant
        Sensibilite( indiceTab + 1 );
        // incrémentation de la valeur suivant le step
        tabSensibilite[ indiceTab , 0 ] = tabSensibilite[ indiceTab , 0 ] +
          tabSensibilite[ indiceTab , 2 ];
      }
      // on remet la valeur initiale
      tabSensibilite[ indiceTab , 0 ] = tabSensibilite[ indiceTab , 3 ];
    }

      // on n'est pas a la fin du tableau
    else
    {
      if(  ( indiceTab <= length( tabSensibilite ) - 1 ) )
      {
        // tant que la valeur n'est pas égale à la valeur à la valeur maximum
        while(  tabSensibilite[ indiceTab , 0 ] <= tabSensibilite[ indiceTab , 1 ] )
        {
          // Au debut de chaque simulation, création des différents objets

          tabAppel = TTabAppels.Create(  );
          /*VN simcons*/tabAppelIni = TTabAppels.Create(  );
          tabClass = TTabClasses.Create(  );
          contextObjet = TContext.Create(  );

          // remplacement dans la table des variables
          MAJTableForcage;
          // execution de la simulation
          ForcerTable;

          // Deux modes d'appel d'ExecuterSimulation :
          // O Destruction des enregistrements ayant le meme CodeSimule (meme simulation)
          // 1 on ne ne fait rien
          if(  Numsimulation == 1 )
            LanceSimulationsConsecutives( DBModule1.RequeteSimuleIdSimule.Value
              , 0 )
              //VN ExecuterSimulation(DBModule1.RequeteSimuleIdSimule.Value,0);else //VN ExecuterSimulation(DBModule1.RequeteSimuleIdSimule.Value,1);
            LanceSimulationsConsecutives( DBModule1.RequeteSimuleIdSimule.Value
              , 1 );
          ++Numsimulation;
          // incrémentation de la valeur suivant le step
          tabSensibilite[ indiceTab , 0 ] = tabSensibilite[ indiceTab , 0 ] +
            tabSensibilite[ indiceTab , 2 ];

          /*VN simcons*/tabAppelIni.Free;
          tabAppel.Free;
          contextObjet.Free;
        }

        // on remet la valeur initiale
        tabSensibilite[ indiceTab , 0 ] = tabSensibilite[ indiceTab , 3 ];
      }
    }
  
    } catch(...) {AfficheMessageErreur( "TScenario.Sensibilite | indice : " +
      IntToStr( indiceTab ) , UClasseScenario );
  }
}

/*--------------------------------------------------------------------------
  Procédure permettant de créer et remplir du tableau des
  valeurs de sensibilité

  @Author  CIRAD
  @Version v_3.0
---------------------------------------------------------------------------*/
void TScenario::RemplirTableauSensibilite()
// Création et remplissage du tableau des valeurs de sensibilité
{
  int indice;  /// indice de boucle

  try {
    Setlength( tabSensibilite , DBModule1.Forcage.RecordCount , 4 );
    DBModule1.Forcage.First;

    { long indice_end = DBModule1.Forcage.RecordCount ; for( indice = 0 ; indice < indice_end ; ++indice )
    {
      // récupération des données dans la table forçage
      tabSensibilite[ indice , 0 ] = DBModule1.ForcageValeurMin.Value;
      tabSensibilite[ indice , 1 ] = DBModule1.ForcageValeurMax.Value;
      tabSensibilite[ indice , 2 ] = DBModule1.ForcageStep.Value;
      tabSensibilite[ indice , 3 ] = DBModule1.ForcageValeurMin.Value;
      DBModule1.Forcage.Next;
    }}
  
    } catch(...) {AfficheMessageErreur( "TScenario.RemplirTableauSensibilite"
      , UClasseScenario );
  }
}

/*--------------------------------------------------------------------------
  Procédure permettant de mettre à jour la table forçage en
  fonction du tableau des valeurs de sensibilité.

  @Author  CIRAD
  @Version v_3.0
---------------------------------------------------------------------------*/
void TScenario::MAJTableForcage()
// Mise à jour de la table forçage en fonction du tableau des valeurs de sensibilité
{
  int indice;  ///indice de boucle

  try {
    DBModule1.Forcage.First;
    { long indice_end = DBModule1.Forcage.RecordCount ; for( indice = 0 ; indice < indice_end ; ++indice )
    {
      DBModule1.Forcage.Edit;
      DBModule1.ForcageValeur.Value = tabSensibilite[ indice , 0 ];
      DBModule1.Forcage.Post;
      DBModule1.Forcage.Next;
    }}
  
    } catch(...) {AfficheMessageErreur( "TScenario.MAJTableForcage" , UClasseScenario );
  }
}


//END
