unit ModelsManage;
// Cette unite permet de gerer les objets generiques et leurs methodes
// Auteur(s)       :
// Unit�(s) � voir : <unit� ayant un lien fort>
// Date de commencement          : avant 01/02/04 !
// Date de derniere modification : 16/03/04
// Etat : fini, teste
//{$DEFINE AffichageComplet}
//{$DEFINE Debug} // methode d'affichage dans TEI et TEC
//{$DEFINE AffichageTresComplet}

interface

uses DB, DBTables, Windows, SysUtils, Variants, DateUtils, Dialogs {VN save},
classes, HashMap,DCL_intf,HashMapTable;

type
  //--------------------------------------------------------------------------
  //           Objets li�s aux proc�dures
  //--------------------------------------------------------------------------

  ///Concept : caract�rise les valeurs des arguments d'une proc�dure.
  ///Description : Tableau de r�el qui repr�sente les valeurs des arguments d'une proc�dure (pour le mod�lisateur : les variables d'un module).
  TPointeurProcParam = array of Double;

  ///Concept : Pointeur sur une proc�dure.
  ///Description : Pointeur sur une proc�dure ayant comme argument un tableau de TpointeurProcParam (soit un tableau de r�el).
  ///Contrainte : le nbre et type des param pour cette declaration en procedures doit etre identique, d'ou les toutes les procedures ont un tableau dynamique de double
  TPointeurProc = procedure(var tabParam: TPointeurProcParam);

  ///Concept : Pointeur sur une proc�dure h�berg�e dans une DLL.
  ///Description : Pointeur sur une proc�dure h�berg�e dans une DLL ayant comme argument un tableau de TpointeurProcParam (soit un tableau de r�el).
  TPointeurProcDll = procedure(var tabParam: TPointeurProcParam); stdcall;

  ///Concept : caract�rise une proc�dure.
  ///Description : Classe repr�sentant une proc�dure.
  TProcedure = class
  private
    name: string[50];
      /// nom de la procedure: nom court du module repr�sent� de la table Module.DB.
    constructor Create(n: string); ///Constructeur
  end;

  ///Concept : caract�rise une proc�dure inclue dans le moteur.
  TProc = class(TProcedure)
  private
    pointeur: TPointeurProc;
      ///pointeur de la proc�dure repr�sent�e par un type TPointeurProc
    constructor Create(n: string; p: TPointeurProc); ///Constructeur
  end;

  ///Concept : caract�rise une proc�dure h�berg�e dans une Dll
  TProcDll = class(TProcedure)
  private
    pointeur: TPointeurProcDll;
      ///pointeur de la proc�dure repr�sent�e par un type TPointeurProcDll
    constructor Create(n: string; p: TPointeurProcDll); ///Constructeur
  end;

  //Ajout VN contraintes
  TProcContrainte = class(TProcedure)
  private
    Corps: string;
    constructor Create(n: string; CorpsContrainte: string); ///Constructeur
  end;
  //Fin Ajout VN

 ///Concept : caract�rise l'ensemble des proc�dures charg�es et disponibles pour le moteur de simulation.
 ///Description : Classe d�finissant le tableau listProc(Dll) de toutes les proc�dures charg�es lors de l'initialisation des unit�s de mod�le.
  TTabProc = class
  private
    {ToDo VN: Utiliser le polymorphisme et 1 seule liste}
    //listProcContrainte: array of TProcContrainte;
    listProcContrainte : TStrHashMap;
        /// liste des contraintes (modules) disponibles dans la plateforme et impl�ment�s � partir d'un script.

    //{$IFDEF HashmapListProc}
    listProc : TStrHashMap;
    //{$ELSE}
    //listProc: array of TProc;
   // {$ENDIF}
    
      /// liste des procedures (modules) disponibles dans la plateforme et impl�ment�s au sein m�me du moteur (interne).

     listProcDll : TStrHashMap;
    //listProcDll: array of TProcDll;
      /// liste des procedures (modules) disponibles dans la plateforme et impl�ment�s dans une Dll.
    listDll: array of THandle;
      /// liste les librairies (dll) devant �tre charg�es pour utiliser les proc�dures qu'elles contiennent...
    constructor Create(); ///Constructeur
  public
    {Ajout VN contraintes}procedure AjoutProcContrainte(nameProc: string;
      CorpsContrainte: string);
      ///  ajoute une contrainte interne � listProcContrainte
    procedure AjoutProc(nameProc: string; pointeur: TPointeurProc);
      ///  ajoute une procedure interne � listProc
    procedure AjoutProcDll(nameProc: string; pointeur: TPointeurProcDll);
      ///  ajoute une procedure Dll � listProcDll
    function Trouver(nameProc: string): TProcedure;
      /// recherche une proc�dure en fct de son nom dans listProc puis dans listProcDll si elle n'est pas trouv� dans listProc { TODO -oViB : A corriger: le cas d'une proc�dure interne et dll de m�me nom }
    procedure ChargeDll;
      /// Pour le mod�le utilis�, charge toutes les proc�dures Dll utiles
  end;

  //--------------------------------------------------------------------------
  //             Objets li�s aux param�tres de proc�dures
  //--------------------------------------------------------------------------
  ///Concept : �num�re la liste des possibilit�s de passage d'un param�tre de proc�dure (par r�f�rence ou valeur).
  ///Description : �num�re les types de passage d'un param�tre de proc�dure ce qui repr�sente la mani�re de traiter la valeur de la variable d'un module (in, inout et out).
  TEnumTypeParam = (kIn, kInOut, kOut);

  ///Concept : Caract�rise un param�tre de proc�dure.
  ///Description : Classe d�finissant un param�tre de proc�dure impl�ment�e.
  TParam = class
  private
    name: string[25];
      /// nom du parametre: correspond au nom court du param�tre dans la table DictionnaireVar
    typeParam: TEnumTypeParam;
      /// type de parametre: type de passage � la proc�dure -de type TenumTypeParam passage en valeur ou r�f�rence-.
    constructor Create(n: string; t: TEnumTypeParam); ///Constructeur
  public
    {Ajout VN contraintes}function getName(): string;
  end;

  //--------------------------------------------------------------------------
  //             Objets li�s aux attributs des objets
  //--------------------------------------------------------------------------

  ///Concept : �num�re les types possibles d'un attribut utilis�s par le moteur.
  ///Description : Liste �num�r�e des types possibles d'un attribut (seul aDouble est impl�ment� -soit le type r�el-, les autres n'�tant jamais utilis�s comme date -en fait TdateTime soit aussi r�el-, etc�).
  TAttributeType = (aDouble);

  ///Concept : caract�rise un attribut d'un objet.
  ///Description : D�finit les attributs des classes d'entit�s (cela repr�sente aussi des "param�tres" ou " variables ").
  TAttribute = class
  private
    codeVar: Integer;
      /// repr�sentant le code de l'attribut (en fait le code de la variable dans la table DictionnaireVar) exemple : 30 = Site.RG
    myType: TAttributeType;
      /// type de l'attribut (qui est un TattributeType limit� � ce jour aux r�els).
    constructor Create(LeTypeDeLAttribut: TAttributeType; LeCodeVar: Integer);
      ///Constructeur
  public //Des m�thodes permettant de , de lire sa valeur et de retrouver un attribut par son nom sont aussi cr��es mais en virtuelles abstraites et sont surcharg�es dans les objets descendants.
    function GetName: string; virtual; abstract;
      /// abstrait, r�cup�re le nom de l'attribut (surcharg�e dans les objets descendants)
    function GetVal: Double; virtual; abstract;
      /// abstrait, r�cup�re la valeur de l'attribut (surcharg�e dans les objets descendants)
    procedure SetVal(val: Double); virtual; abstract;
      /// abstrait, �crit la valeur de l'attribut (surcharg�e dans les objets descendants)
  end;

  ///Concept : caract�rise un attribut orient� base de donn�es ayant un champ unique comme flux de donn�es.
  ///Description : D�finit un attribut d'instance d'entit� qui acc�de directement (par pointeur) au champ de la base de donn�es lui correspondant. La lecture/�criture est ainsi simplifi�e.
  TAttributeField = class(TAttribute)
    // decrit un attribut non temporaire, pointant en entree vers une table herite de la classe TAttribute
  private
    myField: TField;
      ///L'attribut " myField " de type TField (Pascal Objet) permet cet acc�s direct aux donn�es.
  public
    constructor Create(LeChamp: TField; LeTypeDAttribut: TAttributeType;
      LeCodeVar: Integer); ///Constructeur
    function GetName: string; override; ///r�cup�re le nom du champ TField
    function GetVal: Double; override; ///r�cup�re la valeur du champ courant
    procedure SetVal(Val: Double); override;
      ///qui �dite la valeur du champ d�finit dans TField
  end;

  ///Concept : caract�rise un attribut orient� base de donn�es ayant deux champs distinct en flux de donn�es entrant et sortant.
  ///Description : Classe h�ritant de TattributeField pr�c�dente mais qui a comme attribut suppl�mentaire " myFieldOut "
  TAttributeFieldForce = class(TAttributeField)
    myFieldOut: TField;
      ///Ce champ repr�sente donc le champ destination des attributs que l'on souhaitera forcer�
    constructor Create(fIn, fOut: TField; t: TAttributeType; i: Integer);
      ///Constructeur
    procedure SetVal(val: Double); override;
      ///m�thode �crivant en sortie -donc dans myFieldOut- la valeur r�cup�r�e par la m�thode h�rit�e GetVal de l'objet parent TAttributeField
  end;

  ///Concept : caract�rise un attribut temporaire dont le flux de donn�es reste charg� en m�moire.
  ///Description : D�finit un attribut temporaire qui n'a pas de relation avec une base de donn�es (contrairement � TAttributField).
  TAttributeTemp = class(TAttribute)
    name: string; /// d�finit le nom de l'attribut
    index: Integer;
      /// d�finit un index (mais il est aussi identifi� par le code de la variable h�rit� par l'objet parent TAttribut). Les valeurs de ces attributs temporaires sont h�berg�es dans le tableau TabValues de l'objet TEntityInstance et non au sein m�me de l'objet TAttributTemp. Ainsi, cet index repr�sente la position de la valeur lui correspondant dans le tableau TabValues de l'objet TEntityInstance.
    // Value : Double;
    constructor Create(n: string; t: TAttributeType; i, indice: Integer);
      ///Constructeur
    function GetName: string; override; ///lit le nom de l'attribut.
    //function GetVal: Double; Override;
    //procedure SetVal(val : Double); Override;
  end;

  ///Concept: Caract�rise un flux de donn�es orient� base de donn�es. { TODO : devrait h�riter de TAttributeField? }
  ///Description: Cet objet permet de g�rer l'acc�s � une table d'un attribut de type TField (utilis� par TAttributeField ou TAttributeFieldForce).
  TAccessRecord = class
    myBookMark: TBookMark;
      /// repr�sente le bookmark actif sur la table myTable de l'objet, caract�riser l'acc�s aux flux de donn�es grace � son type TBookMark (qui identifie un enregistrement dans un ensemble de donn�es pour une navigation ult�rieure)
    myTable: TTable;
      /// la table source repr�sentant la table de l'enregistrement
    readOnly: Boolean;
      /// true quand la valeur de BDD est en lecture seule (lui-m�me d�finit par la valeur ReadOnly de la table dans la table TableEntity.DB)
    myStep: Double;
      /// le pas de temps entre deux executions d'un module (d�finit par la valeur Step de la table " myTable " de la table TableEntity.DB)
    nextDate: TDateTime;
      /// repr�sentant la date d'acc�s � l'enregistrement (date du prochain acc�s � l'enregistrement?)
    constructor Create(table: TTable; step: Double; rO: Boolean; laDate:
      TDateTime); ///Constructeur
    procedure MAJNextDate(laDate: TDateTime; recopier: boolean);
      ///Cette m�thode peut dupliquer l'enregistrement courant pour en cr�er un nouveau index� suivant le pas Step (mais que pour les tables qui ne sont pas en lecture seule, soit Resjour et ResCum)

    {VN simcons}procedure CopynewEnreg(thisdate: TdateTime);
    //Recopie l'enregistrements de la table de la simulation courante
    //puis met son num�ro de simulation � nil pour ne pas qu'il soit effac� par le dblocateSimule
    //!!!Verifier que la table coit ouverte en �criture!!!
    //(bug quand le test est fait dans cette proc�dure, je ne sais pas d'ou ca viens)

  end;

  //--------------------------------------------------------------------------
  //             Objets utilis�s par le moteur
  //--------------------------------------------------------------------------

  ///Concept : caract�rise les classes (les structures) des objets utilis�s par le moteur.
  ///Description : D�finit la structure des entit�s (ex: plante, sol) utilis�es par le mod�le. A ces types sont associ� des param�tres ou attributs d'entr�e et de sortie.
  TEntityClass = class
    // classe contenant la liste des attributs pour une entite, herite de la classe TEntity
    name: string[20];
      /// nom de la classe correspond � la valeur du champ " NomEntite " de la table Entite -ou de la table ModeleVar-),
    code: shortint;
      /// identifiant: qui correspond � la valeur du champ " CodeEntite " de la table Entite
    nbEntites: shortint; ///r epr�sente le nombre d'entit� pour la classe d�finie
     

     tabAttributes: array of TAttribute;

      ///regroupe les attributs de la classe d'entit� (tableau de TAttribute)
    TESTtabAttributes: array of TAttribute;
      // Tableau de test, � substituer par tabAttributes:
    constructor Create(n: string; c: shortint); ///Constructeur
    destructor Destroy; override; ///Destructeur
    procedure DefAttributTFieldEntree(f: TField; t: TAttributeType; i: Integer);
      overload; ///Cr�ation d'un attribut du type TAttributeField en l'ajoutant dans le tableau " tabAttributes ".
    procedure DefAttributTFieldEntSortie(f1, f2: TField; t: TAttributeType; i:
      Integer); overload;
      ///Cr�ation d'un attribut du type TAttributeFieldForce (avec entr�e et sortie diff�rentes) en l'ajoutant dans le tableau " tabAttributes ".
    procedure DefAttributTemp(LeNom: string; LeTypeDAttribut: TAttributeType;
      LeCodeVariable, IndiceListeAttributsTemp: Integer); overload;
      ///Cr�ation d'un attribut temporaire du type TAttributeTemp  (d�connect� de l'objet  TField) en l'ajoutant dans le tableau " tabAttributes ".
    {procedure DefAttributCharge(LeChamp : TField; LeTypeDeLAttribut: TAttributeType; LeCodeVar : Integer); ///Cr�ation d'un attribut du type TAttributeFieldCharge en l'ajoutant dans le tableau " tabAttributes".}
    procedure AttributeFromTable(nomEntite: string);
      ///pour l'entit� pass�e en param�tre, cr�� son tableau des attributs " tabAttributes " en se basant sur le contenu de ModeleVar,
    procedure MAJTappels(date: TDateTime);
      ///Mise � jour de la date des instances de TAppel relative a l'instance courante de TEntityClass
    procedure DefaultInitInstance(const defaultValue: Double);
      ///Mise � jour de la valeur du champ de tous les attributs de type TAttributField  de tabAttributes s'ils sont en en lecture/�criture.
    function Trouver(nameAttr: string): TAttribute; overload;
      ///renvoie le TAttribute correspondant au nom  de la variable (donc de l'attribut)
    function Trouver(indexAttr: Integer): TAttribute; overload;
      ///renvoie le TAttribute correspondant au code de la variable (donc de l'attribut)
  end;

  ///Concept : caract�rise l'instance d'une classe (appel�s " entit�s ") d'un objet utilis� par le moteur.
  ///Description : caract�rise les entit�s utilis�es par le mod�le (plante1, plante2, Sol1,�), contient la liste des valeurs des attributs pour une entite herite de la classe TEntity
  TEntityInstance = class
    {VN simcons}
    IsInitialized: boolean;
      name: string;

    myClass: TEntityClass;
      ///identifie une classe d'entit� (de type TentityClass) et donc une liste de variables
    tabValues: array of Double;
      ///permet quand � lui de stocker directement la valeur des attributs de type TattributeTemp (donc tableau de r�el).
    inOutTables: array of TAccessRecord;
      ///permet d'acc�der aux valeurs des attributs de type TAttributField et TattributeForceField: ctt les valeurs des TAttributField
    {TabAttributsChargees: array of TAttributChampCharge; // contient les valeurs des TAttributChampCharge=}
    //VN simcons constructor Create(NomC : String;date : TDateTime);   ///Constructeur
    constructor Create(NomC: string; date: TDateTime; n: string); ///Constructeur
    destructor Destroy; override; ///Destructeur
    function GetVal(nameAttr: string): Double; overload;
      ///retourne la valeur de l'attribut pass� en param�tre (lu dans " inOutTable " pour les TattributeField ou TAttributFieldForce et lu dans " tabValues " pour les TAttributeTemp)
    procedure SetVal(nameAttr: string; Val: Double); overload;
      ///�crit la valeur de l'attribut pass� en param�tre (�crit dans " inOutTable " pour les TattributeField ou TAttributFieldForce et �crit dans " tabValues " pour les TAttributeTemp)
    function GetVal(indexAttr: Integer): Double; overload;
    procedure SetVal(indexAttr: Integer; Val: Double); overload;
    function GetVal(theAttr: TAttribute): Double; overload;
    procedure SetVal(theAttr: TAttribute; Val: Double); overload;
    procedure CreerBookmark(laDate: TDateTime);
      ///� l'initialisation des entit�s (dans InitYears par exemple), pour chacune des entit�s de la table " TableEntity ", il est cr�� un item dans " inOutTables ". Cet item est donc un TAccessReccord (d�signant une table, un bookmark, etc...) propre � une entit�
    procedure MAJBookmark(laDate: TDateTime);
      ///pour tous les items de " inOutTables ", on ex�cute la m�thode " MAJNextDate " h�rit�e du TAccessReccord.
    {procedure RempliTabAttributsChargees();// � documenter apr�s impl�mentation }
    procedure MiseAZero(mode: Integer);
      ///initialise toutes les valeurs de tabValues des variables de type TattributeTemp � la valeur indiqu�e dans ModeleVar.
    {VN SimCons}procedure InitializeCurrentInstance();
    {VN SimCons}procedure SetLastSimuleEnreg(idSimule: string);
  end;

  //--------------------------------------------------------------------------
  //            TContext
  //--------------------------------------------------------------------------

  ///Concept : m�morise l'ensemble des objets utilis�s par le moteur. Il contient les classes d'entit� et les instances d'entit� actives du moteur.
  ///Description : Cet objet stocke toutes la liste des entit�s et les classes d'entit�s, ce qui permet de retrouver les bonnes entit�s pour effectuer les calculs.
  TContext = class
    currentInstances: array of TEntityInstance;
      ///contient les instances courantes des entit�s (de type tableau de TentityInstance)
    classesIndexes: array of TEntityClass; ///contient un tableau TentityClass.
    // Au rang i, on trouve :
    // * dans classesIndexes[i] l'instance de la classe TEntityClass
    // * dans currentInstances[i] l'unique instance de TEntityInstance correspondante
    constructor Create(); ///Constructeur
    destructor Destroy; override; ///Destructeur
    function GetCurrentInstance(theClass: TEntityClass): TEntityInstance;
      overload; virtual; ///renvoie l'instance courante
    function GetCurrentInstance(nameClass: string): TEntityInstance; overload;
      virtual;
    procedure SetCurrentInstance(anInstance: TEntityInstance); virtual;
      ///met � jour les instances dans la liste des instances
    procedure FreeEntityInstance(nameClass: string); overload;
      ///supprime une instance courante � partir du nom d'une instance
    procedure FreeEntityInstance(anInstance: TEntityInstance); overload;
      ///supprime une instance courante � partir de l'instance de la classe d'instance
    {VN SimCons}procedure ResetContextInstance(ThisDate: TDateTime; nbAn:
      integer);
    {VN SimCons}procedure GereCreationInstances(thisDate: TdateTime);
    {VN SimCons}procedure RunInitInstances();
    {VN SimCons}procedure prepareNextSimule(thisdate: TDateTime);
    // {VN SimCons}Procedure SetlastSimuleEnreg(idSimule:String);
//Sauvegarde le dernier enregistrement dans un enregistrement temporaire pour ne pas qu'il soit �cras�
//sous l'idSimule $$idtmp$$
//Cet enregistrement servira par la suite de premier enregistrement pour la simulations simulation suivante

  protected
    function findClassIndex(theClass: TEntityClass): Integer;
      ///retourne le num�ro dans le tableau classesIndexes de l'instance de TentityClass

  end;

  //VN save
  TContextSave = class
    bookmarks: array of array of pointer;
      //les bookmarks des tables de chaque instance
    NextExes: array of array of TDateTime;
      //les nextexe des tables de chaque instance
    Filters: array of array of string;
      //les nextexe des tables de chaque instance

    tempValues: array of array of double;
      //les valeurs des variables temporaires des tables de chaque instance

    Results: array of array of double;
      //Etat des enregistrements des tables en sortie

    Instances: array of shortint; //les instances (ou nil)

    constructor Create(contexte: TContext); ///Constructeur
    destructor Destroy; override;
    procedure UseSave(contexte: Tcontext); ///Destructeur
  end;
  //fin VN save

    ///Concept : m�morise l'attribut d'une classe d'objet (ou classe d'entit�) utilis� par le moteur en l'associant � cet objet.
    ///Description : Objet d�finissant classe d'entit� (TEntityClass) et un attribut (TAttribute). Sert de lien entre l'attribut et la classe d'entit�. Repr�sente donc les arguments des entit�s. Cela peut donc �tre un attribut simple d'une classe ou d'une entit� comme un attribut repr�sentant un param�tre de proc�dure.
  TArgument = class // lien entre l'attribut et la classe d'entite
    theClass: TEntityClass; /// la classe d'entite
    theAttribute: TAttribute; /// l'attribut de l'entite
    constructor Create(nameClass, nameAttr: string); ///Constructeur
  end;

  ///Concept : m�morise les classes d'entit� cr�es par le moteur.
  ///Description : Objet contenant la liste de toutes les classes d'entit�s d�j� cr�es. Compos� de l'attribut " listClass " de type tableau de TEntityClass (le tableau liste des classes d'entit�s) et des m�thodes Ajouter (qui ajoute un type de classe dans la liste listClass) et Trouver qui renvoie une TEntityClass de la liste " listClass " suivant le code ou le nom de la classe. Ces m�thodes sont notamment utilis�es lors de la cr�ation des appels de proc�dures afin de retrouver, par exemple, la classe d'une entit� de la proc�dure du TAppel.
  TTabClasses = class
    // Contient la liste de toutes les classes d'entites deja creees
    listClass: array of TEntityClass;
      ///le tableau liste des classes d'entit�s cr�es
    constructor Create; ///Constructeur
    destructor Destroy; override; ///Destructeur
    //VN simcons procedure Ajouter(nameClass : string);  ///ajoute un type de classe dans la liste listClass
    function Ajouter(nameClass: string): TEntityClass;
    function Trouver(nameClass: string): TEntityClass; overload;
      ///retourne la classe d'entit� suivant son nom issue de listClass
    function Trouver(codeClass: shortInt): TEntityClass; overload;
      //////retourne la classe d'entit� suivant son code issue de listClass
    procedure CreationClasses;
  end;

  //----------------------------------------------------------------------------
  //            Objets li�s aux appels de proc�dure
  //----------------------------------------------------------------------------

  ///Concept : caract�rise l'appel d'une proc�dure en d�crivant ses param�tres et son rattachement � telle ou telle classe d'entit�.
  ///Description : Objet repr�sentant une proc�dure (au sens stricte de programmation) lors de la construction de la classe
  TAppel = class
    /// Permet l'appel de la meme procedure, plusieurs fois, qui varie en fonction des liens entre ses parametres et les attributs des entites qui sont lie
    id: Integer;
      /// identifiant de l'appel (en fait le ode du module dans la table Module.DB)
    stepExe: Shortint; /// pas de temps de l'execution
    // (-1 fct de date, 0 initialisation, 1 journalier,
    // 0,1 horaire..., 30 mensuel)
    nextExe: TDateTime;
      /// date et heure de la prochaine execution de la proc�dure repr�sent�e
    theProc: TProcedure; // nom et pointeur de la procedure repr�sent�e
    classe: TEntityClass; /// classe de l'entit� g�rant cette proc�dure
    tabArg: array of TArgument;
      /// tableau des arguments de la classe d'entit� li�s � la proc�dure
    tabParam: array of TParam;
      /// tableau des param�tres lors de la construction les liens. param argument est traduit par un m�me indice de tableau
    tabIndiceParamInX: array of Shortint;
      /// tableau des indices des param en en in et Inout
    tabIndiceParamOutX: array of Shortint;
      /// tableau des indices des param en en Out et Inout
    constructor Create(idA, code, leStep: Integer; nameProc: string); overload;
      ///Constructeur
    destructor Destroy; override; ///Destructeur
    procedure DefArgument(rangArg: Integer; A: TArgument);
      ///ajoute un argument � la liste
    procedure DefParam(rangParam: Integer; nomParam: string; typeParam:
      TEnumTypeParam);
    {VN save}constructor Create(sample: TAppel); overload;
      ///ajoute un param�tre � la liste tabIndiceParamInX ou tabIndiceParamOutX    // construit le tableau ordonn� de def des param
  end;

  ///Concept : m�morise les objets TAppel utilis�s par le moteur.
  ///Description : Objet repr�sentant le tableau des appels et permettant la gestion des appels d'ex�cution des proc�dures.
  TTabAppels = class
    listAppel: array of TAppel; /// liste des appels (tableau de Tappel)
    constructor Create(); ///Constructeur
    destructor Destroy; override; ///Destructeur
    procedure Ajouter(aCall: TAppel); ///ajoute un TAppel � la liste listAppel
    procedure Executer(idAppel: Integer; context: TContext);
      ///ex�cute un appel en connaissant son rang dans la liste
    function Trouver(idAppel: Integer): TAppel;

    //procedure Executer(aCall : Tappel; context : TContext);overload;
  end;

  TTabAppelsSave = class
    nextExes: array of TDateTime;
    constructor Create(tab: TTabAppels); ///Constructeur
    destructor Destroy; override; ///Destructeur
    procedure UseSave(tab: TTabAppels);
  end;

procedure SetTabAppels(codeSimule: string; IndexTable: integer);
var
  { TODO -ofred : trouver un moyen pour d�placer ces d�clarations dans
  ClasseScenario. Ensuite, permettre que les classes TContext et TEntityClass
  puisse connaitre les instances (passage par param�tre) }
  // Classes des appels de procedures dynamique
  tabProc: TTabProc;
  tabAppelConsecutif: array of TTabAppels; //Modif VN simcons
  currentTabAppel: integer; //Modif VN simcons
  tabAppel: TTabAppels;
  tabAppelIni: TTabAppels; //Modif VN simcons
  tabClass: TTabClasses;
  contextObjet: TContext;

  // Classes des appels de procedures dynamique

  { TODO :

  Voir a mettre toutes les declarations et methodes ici.
  Avec modif init en virtuelle et ?abstract... et partie en override eventuelle
  pour acces specifique a BDD ou.. de ECOSYS }

implementation // IMPLEMENTATION ---------------------------------------------

uses GestionDesTables, Main, GestionDesErreurs, DbModule, {Ajout VN simcons}
ClasseScenario, Contrainte, ClasseSimule;

// Classes des appels de procedures dynamiques
var
  sauvDernierAttribut : string;
constructor TParam.Create(n: string; t: TEnumTypeParam);
begin
  name := n;
  typeParam := t;
end;

//Ajout VN contraintes

function TParam.getName(): string;
begin
  result := name;
end;
//Fin Ajout VN

constructor TProcedure.Create(n: string); // y'avait pas avant
begin
  name := n;
end;

constructor TProc.Create(n: string; p: TPointeurProc);
begin
  inherited Create(n); // y'avait pas avant
  //name     := n;
  pointeur := p;
end;

constructor TProcDll.Create(n: string; p: TPointeurProcDll);
begin
  inherited Create(n); // y'avait pas avant
  pointeur := p;
end;

//Ajout VN contraintes

constructor TProcContrainte.Create(n: string; CorpsContrainte: string);
begin
  inherited Create(n);
  Corps := CorpsContrainte;
end;
//Fin Ajout VN

////////------------------------------------------------------/////////
////////                Classe TTabProc
////////------------------------------------------------------/////////

constructor TTabProc.Create();
begin

  listProc := TStrHashMap.Create();
  listProcDll := TStrHashMap.Create();
  listProcContrainte := TStrHashMap.Create();

end;

// Retrouve un processus a partir de son nom
{ TODO -ofred : si le processus n'existe pas, on retourne quand meme un resultat... bug possible, non ?  voir TTabClasses.Trouver}
//function TTabProc.Trouver(nameProc : String): TProc;

function TTabProc.Trouver(nameProc: string): TProcedure;
var
  I: Integer;

begin
  try
      if(listProc.ContainsKey(nameProc))then
      begin

        Result := TProc(listProc.GetValue(nameProc));
        Exit;
      end;

    if(listProcDll.ContainsKey(nameProc))then
      begin

        Result := TProcDll(listProcDll.GetValue(nameProc));
         Exit;
      end;

    if(listProcContrainte.ContainsKey(nameProc))then
      begin

        Result := TProcContrainte(listProcContrainte.GetValue(nameProc));
        Exit;
      end;

      result := nil;
    
  except
    AfficheMessageErreur('TTabProc.Trouver | Processus ' + nameProc,
      UModelsManage);
  end;
end;

// Ajoute un processus dans la liste apres l'avoir cree

procedure TTabProc.AjoutProc(nameProc: string; pointeur: TPointeurProc);
begin
  try
     //{$IFDEF HashmapListProc}
       listProc.PutValue(nameProc,TProc.Create(nameProc, pointeur));
     //{$ELSE}
     // SetLength(listProc, High(listProc) + 2);
     // listProc[High(listProc)] := TProc.Create(nameProc, pointeur);
     //{$ENDIF}

  except
    AfficheMessageErreur('TTabProc.AjoutProc | Processus ' + nameProc,
      UModelsManage);
  end;
end;

// Ajoute un processus dans la liste apres l'avoir cree

procedure TTabProc.AjoutProcDll(nameProc: string; pointeur: TPointeurProcDll);
begin
  try
    //SetLength(listProcDll, Length(listProcDll) + 1);
   // listProcDll[High(listProcDll)] := TProcDll.Create(nameProc, pointeur);
     listProcDll.PutValue(nameProc,TProcDll.Create(nameProc, pointeur));
  except
    AfficheMessageErreur('TTabProc.AjoutProcDll | Processus ' + nameProc,
      UModelsManage);
  end;
end;
//Ajout VN contraintes
// Ajoute un processus dans la liste apres l'avoir cree

procedure TTabProc.AjoutProcContrainte(nameProc: string; CorpsContrainte:
  string);
begin
  try
    //SetLength(listProcContrainte, length(listProcContrainte) + 1);
   // listProcContrainte[High(listProcContrainte)] :=
   //   TProcContrainte.Create(nameProc, CorpsContrainte);
   listProcContrainte.PutValue(nameProc,TProcContrainte.Create(nameProc, CorpsContrainte));
  except
    AfficheMessageErreur('TTabProc.AjoutProcContrainte | Processus ' + nameProc,
      UModelsManage);
  end;
end;
//AjoutVN

//  Permet de charger les diff�rente procedures depuis les Dll

procedure TTabProc.ChargeDll;
var
  i, j: Integer;
  stmp: string;
  AdrTmp: procedure(var tabParam: TPointeurProcParam); stdcall;
  MaDll: THandle;
begin
  try
    DBModule1.Requete1.Params[0].AsString := DBModule1.ModeleId.Value;
    DBModule1.Requete1.Active := true;
    DBModule1.Requete1.First;
    Setlength(ListDll, DBModule1.Requete1.RecordCount);
    for i := 0 to DBModule1.Requete1.RecordCount - 1 do
    begin
      MaDll := LoadLibrary(PChar(CheminSarraH + '\Librairie\' +
        DBModule1.Requete1.FieldByName('Librairie').AsString));
      if MaDll = 0 then
      begin
        mainForm.affiche(TimeToStr(Time) + #9 +
          'Erreur au chargement de la DLL [' + CheminSarraH + '/' +
          DBModule1.Requete1.FieldByName('Librairie').AsString + ']',0) ;
          Main.erreur := true;
        end
      else
      begin
        ListDll[i] := MaDll;
        DBModule1.Requete2.Params[0].AsString :=
          DBModule1.Requete1.Params[0].AsString;
        DBModule1.Requete2.Active := true;
        DBModule1.Requete2.Filter := 'Librairie = ''' +
          DBModule1.Requete1.FieldByName('Librairie').Value + '''';
        DBModule1.Requete2.Filtered := true;
        DBModule1.Requete2.First;
        for j := 0 to DBModule1.Requete2.RecordCount - 1 do
        begin
          // charge les proc�dures.
          stmp := DBModule1.Requete2.FieldByName('NomModule').AsString + 'Dyn';
          @AdrTmp := GetProcAddress(ListDll[i], PChar(stmp));
          if @AdrTmp = nil then
          begin
            mainForm.affiche(TimeToStr(Time) + #9 +
              'Proc�dure [' + stmp + '] non charg�e' + #13#10 +
              'Cette proc�dure doit �tre pr�sente dans la DLL [' + CheminSarraH +
              '/' + DBModule1.Requete1.FieldByName('Librairie').AsString + ']',0) ;
              Main.erreur := true;
            end
          else
            //AjoutProc(DBModule1.Requete2.FieldByName('NomModule').AsString,@AdrTmp); //avant y'avait �a
            AjoutProcDll(DBModule1.Requete2.FieldByName('NomModule').AsString,
              @AdrTmp);
          //FreeLibrary(MaDll);
          DBModule1.Requete2.Next;
        end;
        DBModule1.Requete1.Next;
        DBModule1.Requete2.Active := false;
      end;
    end;
    DBModule1.Requete1.Active := false;
  except
    AfficheMessageErreur('TTabProc.ChargeDll', UModelsManage);
  end;
end;



//------------------------------------------------------------------------------
//                        Classe TContext
//------------------------------------------------------------------------------

constructor TContext.Create();
begin
  currentInstances := nil;
  classesIndexes := nil;
end;

//------------------------------------------------------------------------------

destructor TContext.Destroy;
var
  i: integer;
begin
  for i := 0 to length(currentInstances) - 1 do
    currentInstances[i].Free;

  for i := 0 to length(classesIndexes) - 1 do
    classesIndexes[i].Free;
  inherited destroy;
end;

//------------------------------------------------------------------------------

function TContext.GetCurrentInstance(nameClass: string): TEntityInstance;
// Retourne l'instance courante a partir du nom de la classe de l'entite
{ TODO -ofred : retourne une seule TEntityInstance, alors qu'il peut y en avoir plusieurs ? }
begin
  try
    result := GetCurrentInstance(tabClass.Trouver(nameClass));
  except
    AfficheMessageErreur('TContext.GetCurrentInstance(surcharg�e:String) | Classe '
      + nameClass, UModelsManage);
  end;
end;

//------------------------------------------------------------------------------

function TContext.GetCurrentInstance(theClass: TEntityClass): TEntityInstance;
// Retourne l'instance courante a partir de l'instance de la classe de l'entite
var
  tmp: TEntityInstance;
begin
  try
    tmp := currentInstances[FindClassIndex(theClass)];
    if tmp <> nil then
      result := tmp
    else
      result := nil;
  except
    AfficheMessageErreur('TContext.GetCurrentInstance (surcharg�e:TEntityClass) | Classe '
      + theClass.name, UModelsManage);
  end;
end;

//------------------------------------------------------------------------------

procedure TContext.SetCurrentInstance(anInstance: TEntityInstance);
// Ajoute une instance courante a partir de l'instance de la classe de l'entite
// AJOUTE OU MISE A JOUR ???
{ TODO -ofred : vu que FindClassIndex peut retourner nil => possibilite de bug }
begin
  currentInstances[FindClassIndex(anInstance.myClass)] := anInstance;
end;

//------------------------------------------------------------------------------

procedure TContext.FreeEntityInstance(anInstance: TEntityInstance);
// Supprimer UNE instance courante a partir de l'instance de la classe d'instance
var
  currentClasseIndex: TEntityInstance;
begin
  //  if currentInstances[findClassIndex(anInstance.myClass)].myClass.NbEntites > 0

    { TODO -ofred : est ce vraiment utile, on recupere quelque chose que l'on a en parametre ! }
  currentClasseIndex := currentInstances[findClassIndex(anInstance.myClass)];

  // S'il y a d'autre instance de la classe TEntityInstance
  // la variable TEntityClass.nbEntites > 0 => decremente ce nombre

  if currentClasseIndex.myClass.nbEntites > 0 then
    dec(currentClasseIndex.myClass.nbEntites);

  currentClasseIndex.myClass.DefaultInitInstance(NullValue);
  currentClasseIndex.MiseAZero(0);

  // Apres avoir supprimer l'instance de TEntityInstance et avoir mis a jour
  // TEntityClass, maj de TEntity si besoin
  if currentClasseIndex.myClass.NbEntites = 0 then
    currentClasseIndex.myClass.MAJTappels(0);

  currentInstances[findClassIndex(anInstance.myClass)] := nil;
  anInstance.free;
end;

//------------------------------------------------------------------------------

procedure TContext.FreeEntityInstance(nameClass: string);
// Supprimer UNE instance courante a partir du nom de l'instance de la classe d'entite
begin
  // tant qu'on trouve des instances associ�es � la classe nameClass
  { TODO -ofred : ou est la boucle qui supprime toute les instances ? }
  FreeEntityInstance(GetCurrentInstance(tabClass.Trouver(nameClass)));
end;

//------------------------------------------------------------------------------

// Recherche le numero dans le tableau classesIndexes de l'instance de TEntityClass
// Si ce dernier n'existe pas, on le cree
{ TODO -ofred : est ce normal d'ajouter quelque chose dans une methode de recherche? }

function TContext.findClassIndex(theClass: TEntityClass): Integer;
var
  i: Integer;
begin
  i := Low(classesIndexes);

  while (i <= High(classesIndexes)) and (classesIndexes[i].Name <> theClass.Name)
    do
    Inc(i);
  if i <= High(classesIndexes) then
    result := i
  else
  begin
    SetLength(currentInstances, length(currentInstances) + 1);
      // modif fred high+2 => length+1
    SetLength(classesIndexes, length(classesIndexes) + 1);
      // modif fred high+2 => length+1
    classesIndexes[High(classesIndexes)] := theClass;
    result := High(classesIndexes);
  end;
end;
//----------------------------VN simcons-------------------------------------//

procedure TContext.RunInitInstances();
//Lance les proc�dures d'initialisation pour l'entit� courante
var
  i: integer;
begin
  try
    for i := 0 to length(currentInstances) - 1 do
    begin
      if (currentInstances[i] <> nil) and (not currentInstances[i].IsInitialized)
        then
      begin
        currentInstances[i].InitializeCurrentInstance;
      end;
    end;

  except
    AfficheMessageErreur('TContext.RunInitInstances | Classe ' +
      currentInstances[i].myClass.name, UModelsManage);
  end;
end;
//----------------------------VN SimCons ---------------------------------------

procedure TContext.GereCreationInstances(thisDate: TdateTime);
//Cree les instances ou les remplaces
var
  homeTable: TTable;
  i, j: integer;
  curInstance: TEntityInstance;
begin
  try

    DbModule1.RequeteInstance.Active := true;
    DbModule1.RequeteInstance.First;

    for i := 0 to DbModule1.RequeteInstance.RecordCount - 1 do
      //on parcours toutes les Instances de la simulation
    begin
      HomeTable :=
        TTable(DBModule1.FindComponent(dBModule1.RequeteInstanceHomeTable.Value));
      //on recupere la table m�re de l'entit� afin de pouvoir nommer les instances
      curInstance :=
        GetCurrentInstance(DBModule1.RequeteInstanceNomEntite.Value);

      if (curInstance = nil) then //si l'instance n'existe pas on la cr�e
      begin
        curInstance :=
          TEntityInstance.create(DBModule1.RequeteInstanceNomEntite.Value,
          thisDate, HomeTable.FindField('Id').AsString);

        SetCurrentInstance(curInstance)
          //on ajoute l'instance au contexte
      end
      else
      begin
        if {(DBLocateDate(thisDate)) and}(curInstance.name <>
          HomeTable.FindField('Id').AsString) then
          //Si l'instance existe d�ja et est diff�rente on la met � jour, si c'est la m�me on ne change rien
        begin
          curInstance.MiseAZero(0);
          //Mise a zero de tout les champs des attributs en lecture ecriture (Resjour, Rescum...etc)

          curInstance.IsInitialized := false;
        end;

        curInstance.myClass.MAJTappels(thisDate);
        //Mise a jour de TabAppels

        curInstance.SetLastSimuleEnreg(GetCurrentInstance('Simulation').name);
        //Pour toutes les tables en �critures des instances existantes, si on a pr�alablement sauvegard� un enregistrement
        //avec un champs 'idSimule' � $$idTmp$$ pour qu'il ne soit pas �cras� alors on lui donne la valeur de l'id de la
        //simulation courante

              //curInstance.MAJBookmark(thisDate);
        for j := 0 to High(curInstance.inOutTables) do
          curInstance.inOutTables[j].MAJNextDate(thisdate, false);
        //Mise a jour des bookmarks des tables pour l'acc�s aux donn�es (et copie de l'enreg pour les table ouverte en �criture)
      end;
      DbModule1.RequeteInstance.Next;
    end;
    DbModule1.RequeteInstance.Active := false;

    if GetCurrentInstance('Crop') <> nil then
      contextObjet.FreeEntityInstance('Crop');

{$IFDEF AffichageComplet}
    contextObjet.AfficheLesTEClasses();
    contextObjet.AfficheLesTEInstances();
{$ENDIF}
  except
    AfficheMessageErreur('TContext.InitInstances', UModelsManage);
  end;
end;

procedure TContext.ResetContextInstance(ThisDate: TDateTime; nbAn: integer);
//Reinitialise les instances
var
  i: integer;
begin
  try
    if GetCurrentInstance('Crop') <> nil then
      contextObjet.FreeEntityInstance('Crop');
    //ShowMessage(DateToStr(ThisDate));
    for i := 0 to length(currentInstances) - 1 do
    begin
      if (currentInstances[i] <> nil)
        {and ((nbAn<>0) or (currentInstances[i].myclass.name<>'Simulation'))} then
      begin
        //ShowMessage(currentInstances[i].name);
        currentInstances[i].MAJBookmark(thisDate);
        currentInstances[i].MiseAZero(0);
        currentInstances[i].myClass.MAJTappels(thisDate);
        currentInstances[i].IsInitialized := false;
      end;
    end;
  except
    AfficheMessageErreur('TContext.EmptyContextInstance', UModelsManage);
  end;
end;

procedure TContext.prepareNextSimule(thisdate: TDateTime);
//Sauvegarde le dernier enregistrement dans un enregistrement temporaire pour ne pas qu'il soit �cras�
//sous l'idSimule $$idtmp$$
//Cet enregistrement servira par la suite de premier enregistrement pour la simulations simulation suivante
var
  i, j: integer;
begin
  try

    for i := 0 to length(currentinstances) - 1 do
      if currentinstances[i] <> nil then
        for j := 0 to length(currentinstances[i].inouttables) - 1 do
          currentinstances[i].inOutTables[j].CopynewEnreg(thisdate - 1);

  except
    AfficheMessageErreur('TContext.prepareNextSimule', UModelsManage);
  end;
end;

{Procedure TContext.SetlastSimuleEnreg(idSimule:String);
var i,j:integer;
masterS:TDataSource;
begin
try
  for i:=0 to length(currentinstances)-1 do
    if currentinstances[i]<>nil then
      for j:=0 to length(currentinstances[i].inouttables)-1 do
        if Not currentinstances[i].inOutTables[j].readOnly then
          with currentinstances[i].inOutTables[j] do
            if not readOnly then
            begin
            //ShowMessage(myTable.FieldByName('idSimule').AsString+' '+myTable.FieldByName('jour').AsString);
              masterS:=myTable.MasterSource;
              //ShowMessage(myTable.FieldByName('idSimule').AsString+' '+myTable.FieldByName('jour').AsString);
              myTable.MasterSource:=nil;
              myTable.gotobookmark(mybookmark);
              ShowMessage(myTable.FieldByName('idSimule').AsString+' '+myTable.FieldByName('jour').AsString);
              if myTable.FieldByName('idSimule').AsString='$$idTmp$$' then
                myTable.FieldByName('idSimule').AsString:=idSimule;

              myTable.MasterSource:=masterS;
            end;
except
  AfficheMessageErreur('TContext.SetlastSimuleEnreg',UModelsManage);
end;
end;     }

//----------------------------fin VN SimCons ---------------------------------------

//------------------------------------------------------------------------------
//                          Classe TArgument
//------------------------------------------------------------------------------

constructor TArgument.Create(nameClass, nameAttr: string);
// Contient une instance de la classe TEntityClass et une instance de TAttribute (qui peut etre un attribut ou plusieurs)
{ TODO -ofred : TAttributType permet de quoi faire ? }
begin
  theClass := tabClass.Trouver(nameClass);
  theAttribute := theClass.Trouver(nameAttr);
end;

//------------------------------------------------------------------------------
//                          GESTION DES ATTRIBUTS
//------------------------------------------------------------------------------

constructor TAttribute.Create(LeTypeDeLAttribut: TAttributeType; LeCodeVar:
  Integer);
begin
  myType := LeTypeDeLAttribut;
  codeVar := LeCodeVar;
end;

//------------------------------------------------------------------------------

constructor TAttributeTemp.Create(n: string; t: TAttributeType; i, indice:
  Integer);
begin
  inherited Create(t, i);
  name := n;
  index := indice;
end;

//------------------------------------------------------------------------------

function TAttributeTemp.GetName: string;
begin
  result := LowerCase(name);
end;

//------------------------------------------------------------------------------

constructor TAttributeField.Create(LeChamp: TField; LeTypeDAttribut:
  TAttributeType; LeCodeVar: Integer);
begin
  inherited Create(LeTypeDAttribut, LeCodeVar);
  myField := LeChamp;
end;

function TAttributeField.GetName: string;
begin
  result := LowerCase(myField.FieldName);
end;

function TAttributeField.GetVal: Double;
var
  valeur : string;
begin
 if myField.IsNull then
 begin

      result := nullValue;

 end
 else
 begin

    //if ((TDataSet(myField.GetParentComponent).Name='Meteorologie'))//or (myField.FieldName='ETP'))
    //then
    ///begin

       //  ShowMessage(myField.AsString);
     //    ShowMessage(FloatToStr(Ttype(TStrHashMap(Test.MapMeteo.GetValue(TDataSet(myField.GetParentComponent).FieldByName('Jour').AsString)).GetValue(myField.DisplayName)).val));

   //     Result:= Ttype(TStrHashMap(Test.MapMeteo.GetValue(TDataSet(myField.GetParentComponent).FieldByName('Jour').AsString)).GetValue(myField.DisplayName)).val;

   // end
    //else
   // begin
        


      result := myField.AsFloat;

   // end;

 end;
  //


  // test ViB le 10/02/2005 pour voir comment interf�rer directement au niveau de la lecture des TFields -- a retire rapidement.
  //if ((TDataSet(myField.GetParentComponent).Name='Meteorologie') and (myField.FieldName='TMax')) then Result:= 50;

end;

//------------------------------------------------------------------------------

{ TODO -ofred : y a t il besoin de tester val, ceci n'etant pas fait pour
 la meme methode dans la sous-classe TAttributeFieldForce ? }

procedure TAttributeField.SetVal(val: Double);
begin
  try
    myField.DataSet.Edit;
    if val = nullValue then
      myField.Value := null
    else
      myField.Value := val;
    myField.DataSet.Post;
  except
    AfficheMessageErreur('TAttributeField.SetVal | Champs ' + GetName +
      ', Valeur ' + FloatToStr(val), UModelsManage);
  end;
end;

//------------------------------------------------------------------------------

constructor TAttributeFieldForce.Create(fin, fout: TField; t: TAttributeType; i:
  Integer);
begin
  inherited Create(fin, t, i);
  myFieldOut := fOut;
end;

procedure TAttributeFieldForce.SetVal(val: Double);
begin
  try
    if myFieldOut <> nil then { TODO -ofred : pourquoi? }
    begin
      myFieldOut.DataSet.Edit;
      myFieldOut.Value := GetVal;
      myFieldOut.DataSet.Post;
    end;
  except
    AfficheMessageErreur('TAttributeFieldForce.SetVal | Champs ' + GetName +
      ', Valeur ' + FloatToStr(val), UModelsManage);
  end;
end;

{constructor TAttributChampCharge.Create(LeChamp : TField; LeTypeDAttribut : TAttributeType; LeCodeVar : Integer);
begin
  inherited Create(LeChamp,LeTypeDAttribut,LeCodeVar);
  MonCurseur:=0;
end;

//------------------------------------------------------------------------------

function TAttributChampCharge.GetVal : Double;
begin
  result := TabValeursChargees[MonCurseur];
end;

//------------------------------------------------------------------------------

procedure TAttributChampCharge.SetVal(Val : Double);
begin
  TabValeursChargees[MonCurseur]:=Val;
end;

//------------------------------------------------------------------------------

procedure TAttributChampCharge.RempliTabValeursAttributsChargees();
begin
// l�, on rigole moins....
// l'id�e est qu'on rempli le tableau uniquement des valeurs n�c�ssaires...
// ainsi, on ne charge que les donn�es au pas de temps voulu et donc,
// la mise � jour du curseur sera une simple incr�mentation de MonCurseur.
// il faut le pas, le champs, la table et la date du premieret dernier  jour...
// le pas est trouv� dans DBModule1.TableEntityStep.Value
// le champ (donc la table) dans le MyField h�rit�
// la date du 1er jour.... dans aSimulation.getval('dateDebutSimul')
// la date du dernier jour.... dans aSimulation.getval('dateFinSimul')
// utiliser TDataSet(myField.GetParentComponent).Name='Meteorologie'
// et myField.FieldName='TMax'

mainForm.memDeroulement.Lines.Add('Attribut '+GetName+' charg� en m�moire');
//if TDataSet(myField.GetParentComponent).Locate('Jour',LaDateDeDebut,[]) then // on suppose que la relation maitre/d�tail nous filtre les autres stations
//  begin
//  TDataSet(myField.GetParentComponent).First;
//  //while not TDataSet(myField.GetParentComponent). do
//    begin
//    SetVal(myField.AsFloat);
//    TDataSet(myField.GetParentComponent).Next;   // suivant le pas g�rer le next...
//    MAJIndice;
//    end;
//  end
//else
//  Beep; // pas bon !
//end;

//------------------------------------------------------------------------------

Procedure TAttributChampCharge.MAJIndice();
begin
if Length(TabValeursChargees)=MonCurseur then // si le tabValeurChargees = l'indice MonCurseur, c'est qu'on veut acc�der � une valeur qui n'existe pas encore...
  SetLength(TabValeursChargees,Length(TabValeursChargees)+1); // on dimensionne le tableau en cons�quence...
Inc(MonCurseur);

end;
}
{
////////---------------------------------------------------------------/////////
////////                        Classe TAttributeFieldInOut
////////---------------------------------------------------------------/////////
constructor TAttributeFieldInOut.Create(Fin,Fout : TField; T : TAttributeType; I : Integer);
begin
  Inherited Create(Fin,T,I);
  MyFieldOut := FOut;
end;

procedure TAttributeFieldInOut.SetVal(Val : Double);
begin
  myFieldOut.DataSet.Edit;
  myFieldOut.Value := Val;
  myFieldOut.DataSet.Post;
end;}

/////-----------------------------------------------------------------------////
/////                       Classe TEntityClasse                            ////
/////-----------------------------------------------------------------------////

constructor TEntityClass.Create(n: string; c: Shortint);
// constructeur renseigne seulement le nom et le code
begin
  code := c;
  name := n;
  nbEntites := 0;

  //setlength(TESTtabAttributes,0);
end;

//------------------------------------------------------------------------------

destructor TEntityClass.Destroy;
var
  i: integer;
  //F: TextFile;
  //S: string;
begin


    for i := 0 to length(TabAttributes) - 1 do
  begin
    TabAttributes[i].Free;
  end;
  TabAttributes := nil;


  inherited Destroy;
end;

//------------------------------------------------------------------------------

procedure TEntityClass.DefAttributTFieldEntree(f: TField; t: TAttributeType; i:
  Integer);
// Creation d'un attribut du type TField
begin
  try


    SetLength(tabAttributes, High(tabAttributes) + 2);
    // length(tabAttributes)+1 non?
    tabAttributes[High(tabAttributes)] := TAttributeField.Create(f, t, i);


  except
    AfficheMessageErreur('TEntityClass.DefAttributTFieldEntree', UModelsManage);
  end;
end;

//------------------------------------------------------------------------------

procedure TEntityClass.DefAttributTFieldEntSortie(f1, f2: TField; t:
  TAttributeType; i: Integer);
// Creation d'un attribut du type TField avec entree et sortie diff�rentes
begin
  try



     SetLength(tabAttributes, High(tabAttributes) + 2);
      // length(tabAttributes)+1 non?
    tabAttributes[High(tabAttributes)] := TAttributeFieldForce.Create(f1, f2, t,
      i);
      


  except
    AfficheMessageErreur('TEntityClass.DefAttributTFieldEntSortie',
      UModelsManage);
  end;
end;

//------------------------------------------------------------------------------

procedure TEntityClass.DefAttributTemp(LeNom: string; LeTypeDAttribut:
  TAttributeType; LeCodeVariable, IndiceListeAttributsTemp: Integer);
// Creation d'un attribut temporaire
begin
  try

    SetLength(tabAttributes, High(tabAttributes) + 2);
     
    tabAttributes[High(tabAttributes)] := TAttributeTemp.Create(LeNom,
      LeTypeDAttribut, LeCodeVariable, IndiceListeAttributsTemp);

  except
    AfficheMessageErreur('TEntityClass.DefAttributTemp', UModelsManage);
  end;
end;

//------------------------------------------------------------------------------
{
procedure TEntityClass.DefAttributCharge(LeChamp : TField; LeTypeDeLAttribut: TAttributeType; LeCodeVar : Integer);
// Creation d'un attribut du type champ charg�...
begin
try
  SetLength(TESTtabAttributes, length(TESTtabAttributes)+1);
  TESTtabAttributes[High(TESTtabAttributes)] := TAttributChampCharge.Create(LeChamp,LeTypeDeLAttribut,LeCodeVar);
except
  AfficheMessageErreur('TEntityClass.DefAttributCharge',UModelsManage);
end;
end; }

//------------------------------------------------------------------------------

procedure TEntityClass.AttributeFromTable(nomEntite: string);
// Remplir le tableau des attributs tabAttributes pour l'entite passee en parametre
// en utilisant ce qui est indiqu� dans la table ModeleVar.DB
var
  ChampEnEntree, ChampEnSortie: TField;
  indTmp: Integer;
    // sert de compteur d'indice dans le tableau des attributs temporaires propre � l'entit�.
begin
  try
    indTmp := 0;

    DBModule1.ModeleVar.Filter := 'NomEntite = ''' + nomEntite + '''';
      // contrainte verifier qu'il n'y a pas de relation maitre d�tail filtre qui teste le champ NomEntite pour savoir s'il existe
    DBModule1.ModeleVar.Filtered := True;
      // la valeur passee en parametre de cette procedure

    // Pour chaque variable definie dans ModeleVar.DB pour l'entite pass�e en parametre
    // on cree le champ necessaire (Field) de bon type :  TIntegerField, TFloatField, etc.
    while not DBModule1.ModeleVar.Eof do
    begin
      if DBModule1.ModeleVarSource.Value = 'Temp' then
        // si c'est une variable temporaire
      begin
        DefAttributTemp(DBModule1.ModeleVarNomVar.Value, aDouble,
          DBModule1.ModeleVarCodeVar.Value, indTmp);
        inc(indTmp);
      end
      else // le champ Source dans ModeleVar est <> de 'Temp'
      begin
        // Permet de cr�er un pointeur sur un champ dont on renseigne le num�ro de la table et le nom du champ.
        if DBModule1.ModeleVarSource.Value = 'Tableau' then // For�age
        begin
          ChampEnEntree :=
            CreerChamp(DBModule1.DynamicTables[DBModule1.ModeleVarIndiceTableSource.AsInteger].Fields[DBModule1.ModeleVarIndiceChampSource.AsInteger]);
          if DBModule1.ModeleVarIndiceChampDest.Value <> -1 then
            // test si il y a une sortie valide dans la table ModeleVar
            ChampEnSortie :=
              CreerChamp(TTable(DBModule1.Components[DBModule1.ModeleVarIndiceTableDest.AsInteger]).Fields[DBModule1.ModeleVarIndiceChampDest.AsInteger])
          else
            ChampEnSortie := nil;
          DefAttributTFieldEntSortie(ChampEnEntree, ChampEnSortie, aDouble,
            DBModule1.ModeleVarCodeVar.Value);
        end
        else // le champ Source de ModeleVar est diff�rent de 'Temp' et de 'Tableau'
        begin

          ChampEnEntree :=
            CreerChamp(TTable(DBModule1.Components[DBModule1.ModeleVarIndiceTableSource.AsInteger]).Fields[DBModule1.ModeleVarIndiceChampSource.AsInteger]);
          // if DBModule1.ModeleVarSource.Value = 'Meteorologie' then     { TODO -oViB : Th�oriquement voil� ce qu'il faut mettre en place... }
          //   DefAttributCharge(ChampEnEntree,aDouble,DBModule1.ModeleVarCodeVar.Value)
          // else // il s'agit du attribut classique
          { if DBModule1.ModeleVarSource.Value = 'Meteorologie' then     //Th�oriquement voil� ce qu'il faut mettre en place...
             DefAttributCharge(ChampEnEntree,aDouble,DBModule1.ModeleVarCodeVar.Value);      }
          DefAttributTFieldEntree(ChampEnEntree, aDouble,
            DBModule1.ModeleVarCodeVar.Value);
            // sauvegarde de ce pointeur dans le tableau de TAttribute
        end
      end;
      DBModule1.ModeleVar.Next;
    end;

    DBModule1.ModeleVar.Filtered := False;
    DBModule1.ModeleVar.Filter := '';
  except
    showmessage(DBModule1.ModeleVarNomVar.Value);
    showmessage(inttostr(DBModule1.ModeleVarIndiceTableSource.AsInteger));
    AfficheMessageErreur('TEntityClass.AttributeFromTable | Entit� ' +
      nomEntite, UModelsManage);
  end;
end;

//------------------------------------------------------------------------------

procedure TEntityClass.MAJTappels(date: TDateTime);
  { TODO -oViB: Pourquoi rattacher cette m�thode � TEntityClass et non tout simplement � TAppel? }
// Met a jour de la date des instances de TAppel relative a l'instance courante de TEntityClass
var
  i: integer;
  step: real;
  Year, Month, Day: Word;
  DateTemp: TDateTime;
begin
  try
    for i := 0 to length(tabAppel.listAppel) - 1 do
    begin
      if tabAppel.listAppel[i].classe.name = name then
      begin
        step := tabAppel.listAppel[i].stepExe;
        DateTemp := date;
        if (date > 0) and (step > 1) then
        begin
          DecodeDate(date, Year, Month, Day);
          Day := Trunc(Trunc(DayOfTheMonth(date) / step) * step + step);
          if Day > 27 then
            Day := DayOfTheMonth(EndOfAMonth(Year, Month));
          DateTemp := EncodeDate(Year, Month, Day);
        end;
        tabAppel.listAppel[i].nextExe := DateTemp;
      end;
    end;
  except
    AfficheMessageErreur('TEntityClass.MAJTappels | Date ' + datetostr(date),
      UModelsManage);
  end;
end;

//------------------------------------------------------------------------------

procedure TEntityClass.DefaultInitInstance(const defaultValue: Double);
// Mise � jour � la valeur "defaultValue" (toujours 0 car en appel en dur)
// pour tous les champs des attributs de type TAttributField  de tabAttributes
// s'ils sont en en lecture/�criture
var
  i: Integer;
  liste : ICollection;
  iterator : IIterator;
  ob: TAttributeField;
  temp : Tobject;
begin                      
  try



    for i := 0 to high(tabAttributes) do
    begin
      if ((tabAttributes[i] is TAttributeField) and
        (DBModule1.TableEntity.Locate('Source',
        TAttributeField(TabAttributes[i]).myField.DataSet.Name,
        [loCaseInsensitive])) and (not DBModule1.TableEntityReadOnly.value)) then
        TAttributeField(tabAttributes[i]).Setval(defaultValue);
    end;


  except
    AfficheMessageErreur('TEntityClass.DefaultInitInstance | Valeur par d�faut '
      + FloatToStr(defaultValue), UModelsManage);
      ShowMessage('default');
  end;
end;

//------------------------------------------------------------------------------

function TEntityClass.Trouver(nameAttr: string): TAttribute;
// Retrouve un TAttribute a partir d'un nom
var
  i: Integer;
  obj: TAttribute;
begin
  try


    i := Low(tabAttributes);
    while (i <= High(tabAttributes)) and (tabAttributes[i].GetName <>
      LowerCase(nameAttr)) do
      Inc(i);
    Result := tabAttributes[i];

  except
    AfficheMessageErreur('TEntityClass.Trouver (surcharg� String) | Nom de l''attribut '
      + nameAttr, UModelsManage);
  end;
end;

//------------------------------------------------------------------------------

function TEntityClass.Trouver(indexAttr: Integer): TAttribute;
// Retrouve un TAttribute a partir d'un Codevar
var
  i: Integer;
begin
  try


    i := Low(tabAttributes);
    while (i <= High(tabAttributes)) and (tabAttributes[i].CodeVar <> indexAttr)
      do
      Inc(i);
    Result := tabAttributes[i];


  except
    AfficheMessageErreur('TEntityClass.Trouver (surcharg� Integer) | Index de l''attribut '
      + IntToStr(indexAttr), UModelsManage);
  end;
end;


/////-----------------------------------------------------------------------////
/////                          Classe TAccessRecord                         ////
/////-----------------------------------------------------------------------////

constructor TAccessRecord.Create(table: TTable; step: Double; RO: Boolean;
  laDate: TDateTime);


begin
  try
    myStep := step;
    readOnly := RO;
    myTable := Table;

    if (RO = false) then
      CreerPremierEnreg(Table, myStep);
    MAJNextDate(LaDate, False);

  except
    AfficheMessageErreur('TAccessRecord.Create | Table ' + table.Name +
      ' pas de temps ' + FloatToStr(step) + ' Lecture seule ' + BoolToStr(RO, True)
      + ' Date ' + DateToStr(ladate), UModelsManage);
  end;
end;

//------------------------------------------------------------------------------

procedure TAccessRecord.MAJNextDate(laDate: TDateTime; recopier: Boolean);
//VN met � jour la date de prochain changement de valeur de la table
var
  jour, mois, anprec, an: Word;
  DateRel: TDateTime;
begin
  try
    //ShowMessage(DateToStr(laDate));
    if ((not readOnly) and (recopier)) then
      RecopieEnreg(myTable, laDate, mystep, NumSimulation);
        // Modif APas  changer le 0 en numsimule !

    //VN Table   --------------------------------------------------
    if myStep = -2 then
    begin
      DecodeDate(MyTable.FieldByName('Jour').AsDateTime, an, mois, jour);
      DateRel := EncodeDate(yearof(laDate), mois, jour);
      //on r�cupere l'ann�e relative

      if DateRel <
        contextObjet.GetCurrentInstance('Simulation').GetVal('DebutSimul') then
        DateRel := EncodeDate(yearof(laDate) + 1, mois, jour)
      else if DateRel >
        contextObjet.GetCurrentInstance('Simulation').GetVal('FinSimul') then
        DateRel := EncodeDate(yearof(laDate) - 1, mois, jour);
      //on r�ajuste la premi�re date suivant sa position par rapport aux limites de la simulation.

      anprec := an;
      //on garde en m�moire l'ann�e de l'enregistrement

      if DateRel > laDate then
        //Si la date en cours est sup�rieure � celle du premier enregistrement, alors on a pas de valeurs,
        //on prend alors la premi�re qui suis comme date de prochaine mise a jour.
      begin
        myTable.Next;
        DecodeDate(MyTable.FieldByName('Jour').AsDateTime, an, mois, jour);
        DateRel := EncodeDate(yearof(laDate) + an - anprec, mois, jour);
      end;

      while (DateRel <= laDate) and not MyTable.Eof do
        //on se positionne sur l'enregistrement qui correspond
      begin
        anprec := an;
        myTable.Next;
        DecodeDate(MyTable.FieldByName('Jour').AsDateTime, an, mois, jour);
        DateRel := EncodeDate(yearof(laDate) + an - anprec, mois, jour);
        //pour chaque enregistrement on fait la comparaison avec la date relative
      end;
      //on est sorti, ca veut dire qu'on � d�pass� la date courante

      nextDate := DateRel;
      //on met � jour la date de prochaine execution

      myTable.Prior;
      //on reviens sur la date valable

          //ShowMessage('date de la table: '+DateToStr(MyTable.FieldByName('Jour').AsDateTime)+'   nextexe: '+DateToStr(DateRel)+' date en cours: '+DateToStr(ladate)+' ');
          //ShowMessage(DateToStr(nextDate));
    end;
    //finVN Table  --------------------------------------------------

    //  myTable.FreeBookmark(myBookMark);
    myBookMark := myTable.GetBookmark; { TODO -oViB : ??? }
    myTable.GotoBookmark(myBookMark); { TODO -oViB : ??? }

    //VN gestionEau
     //if myStep < 0 then
    if myStep = -1 then
    begin // alors on est dans le cas de table � acc�s journalier mais dont les donn�es ne sont pas continues (Irrigation, ObsParcelle)

      nextDate := MyTable.FieldByName('Jour').AsDateTime;
      //decodedate(MyTable.FieldByName('Jour').AsDateTimeprec,an,mois,jour);
      //nextDate :=encodedate(YearOf(laDate),mois,jour);
      if nextDate = laDate then //VN Table
        //Si l'enregistrement correspond � la date en cours on calcule nextexe, sinon nextexe ne sera pas remis � jour.
      begin
        myTable.Next;
        nextDate := MyTable.FieldByName('Jour').AsDateTime;
        myTable.Prior;
      end;

      myTable.Filtered := false;
      if myTable.Name = 'Irrigation' then
        myTable.Filter := 'Id=''' +
          dBModule1.ItineraireTechnique.fieldByName('IdIrrigation').AsString +
          ''' and Jour=' + QuotedStr(DateToStr(LaDate))
      else {//VN v2.0} if myTable.Name = 'DonneesObs' then
          myTable.Filter := 'Id=''' +
            dBModule1.Simule.fieldByName('IdDonneesObs').AsString + ''' and Jour='
            + QuotedStr(DateToStr(LaDate));

      //AD myTable.Filter   := 'Jour = ' + QuotedStr(DateToStr(LaDate));

      myTable.Filtered := True;
    end
    else if myStep > 0 then //VN Table
      nextDate := IncrementeDate(laDate, myStep, true);
        { TODO : Revoir le cas calendaire car ici tjs forc� � vrai! }

    //if myStep=-2 then ShowMessage(DateToStr(nextDate));
  except
    AfficheMessageErreur('TAccessRecord.MAJNextDate | Date ' + DateToStr(laDate)
      + ' recopier ' + BoolToStr(recopier, True), UModelsManage);
  end;
end;

//VN simcons
//Recopie l'enregistrements de la table de la simulation courante
//puis met son id de simulation � '$$idTmp$$' pour ne pas qu'il soit effac� par le dblocateSimule

procedure TAccessRecord.CopynewEnreg(thisdate: TdateTime);
var
  masterS: TDataSource;
begin
  try
    if not readOnly then
    begin
      masterS := myTable.MasterSource;
      myTable.MasterSource := nil;

      while myTable.Locate('idSimule', '$$idTmp$$', []) do
        myTable.Delete;

      //ShowMessage(DateToStr(myTable.fieldbyname('jour').AsDateTime));
      RecopieEnreg(myTable, thisdate, myStep,
        myTable.FieldByName('NumeroSimule').Value);
      //ShowMessage(DateToStr(myTable.fieldbyname('jour').AsDateTime));

      myTable.Edit;
      myTable.FieldByName('idSimule').Value := '$$idTmp$$';
      // Modif CB 23/11/06   attention si jour inexistant?
      myTable.FieldByName('jour').Value := thisdate - myStep;
      // Modif CB 23/11/06
      myTable.Post;

      //    myTable.FreeBookmark(myBookMark);
      myBookMark := myTable.GetBookmark;

      myTable.MasterSource := masterS;

    end;
  except
    on e: exception do
      AfficheMessageErreur(e.message + ' TAccessRecord.CopynewEnreg ',
        UModelsManage);
  end;
end;

////////------------------------------------------------------/////////
////////                Classe TEntityInstance
////////------------------------------------------------------/////////

// Constructeur, rempli le tableau tabValues avec les variables temporaires
//VN simcons constructor TEntityInstance.Create(nomC : String;date :TDateTime);

constructor TEntityInstance.Create(nomC: string; date: TDateTime; n: string);

begin
  try
    {VN simcons}IsInitialized := False;
    name := n;
    myClass := tabClass.Trouver(nomC);
    myClass.nbEntites := myClass.NbEntites + 1;
    myClass.MAJTappels(date);

    CreerBookmark(date);

    MiseAZero(-1);
      // on initialise toutes les variables attribute Field des tables resultat a la valeur dans modelevar (for�age)

  except
    AfficheMessageErreur('TEntityInstance.Create | Nom ' + NomC + ' Date ' +
      DateToStr(Date), UModelsManage);
  end;
end;

destructor TEntityInstance.Destroy;
var
  i: Integer;
begin
  try
    tabValues := nil;

    for i := 0 to length(inOutTables) - 1 do
    begin
      inOutTables[i].Free;
    end;
    inOutTables := nil; // fin suppression inOutTables


    inherited Destroy;  

  except
    AfficheMessageErreur(' TEntityInstance.Destroy', UModelsManage);
  end;
end;

// on initialise toutes les valeurs de tabValues des variables de type TAttributeTemp
// a la valeur indiqu�e dans ModeleVar.

procedure TEntityInstance.MiseAZero(mode: Integer);
var
  i: Integer;
  liste : ICollection;
  iterator : IIterator;
  ob: TAttributeField;
  obj: TObject;
begin
  try
    case mode of
      0:
        begin // � la destruction d'entit�
        end;
      -1:
        begin // a la creation d'entite

          for i := 0 to Length(myClass.tabAttributes) - 1 do
          begin
            if myClass.tabAttributes[i] is TAttributeTemp then
            begin
              SetLength(tabValues, High(tabValues) + 2);
                { TODO -oViB: length +1? }
              if DBModule1.ModeleVar.Locate('CodeVar',
                myClass.tabAttributes[i].codeVar, [loCaseInsensitive]) then
                tabValues[High(tabValues)] := DBModule1.ModeleVarValeur.value
              else
                tabValues[High(tabValues)] := 0;
            end;
          end;

         
        end;
    end;
   // ShowMessage('ici');
    if myClass.name <> 'Simulation' then
      myClass.DefaultInitInstance(0);

    //ShowMessage('fin');
  except
    AfficheMessageErreur('TEntityInstance.MiseAZero | Mode ' + IntToStr(mode),
      UModelsManage);
  end;
end;

procedure TEntityInstance.CreerBookmark(laDate: TDateTime);
var
  i: integer;
begin
  try
    DBModule1.TableEntity.Filter := 'CodeEntity = ' + IntToStr(myClass.code);
    {TODO -oViB : l'appel CreerBookmark se fait sur le r�sultat d'une requete ce qui implique le risque d'une erreur de saisie dans la base de donn�e. }
    DBModule1.TableEntity.Filtered := True;
    setlength(inOutTables, DBModule1.TableEntity.RecordCount);
    for i := 0 to DBModule1.TableEntity.RecordCount - 1 do
    begin
      //ici, faire un elseif en impl�mentation finale...
      {if DBModule1.TableEntitysource.asstring='Meteorologie' then RempliTabAttributsChargees(TTable(DBModule1.Components[RetourneIndiceTable(DBModule1.TableEntitySource.Value)]),DBModule1.TableEntityStep.Value,DBModule1.TableEntityReadOnly.Value,LaDate);}
      inOutTables[i] :=
        TAccessRecord.Create(TTable(DBModule1.Components[RetourneIndiceTable(DBModule1.TableEntitySource.Value)]), DBModule1.TableEntityStep.Value,
        DBModule1.TableEntityReadOnly.Value, LaDate);
      DBModule1.TableEntity.Next;
    end;
    DBModule1.TableEntity.Filtered := False;
    DBModule1.TableEntity.Filter := '';
  except
    AfficheMessageErreur('TEntityInstance.CreerBookmark | Date ' +
      DateToStr(laDate), UModelsManage);
  end;
end;

procedure TEntityInstance.MAJBookmark(LaDate: TDateTime);
//Utilis� � chaque d�but de chaque ann�es de Simulation (dans inityear)

var
  i: integer;
begin
  try
    for i := 0 to High(inOutTables) do
      inOutTables[i].MAJNextDate(laDate, true);
  except
    AfficheMessageErreur('TEntityInstance.MAJBookmark | Date ' +
      DateToStr(laDate), UModelsManage);
  end;
end;

//------------------------------------------------------------------------------
//Ajout VN simcons

procedure TEntityInstance.InitializeCurrentInstance();
//Lance les proc�dures d'initialisation de l'instance
var
  i: integer;
begin
  try
    for i := 0 to length(tabAppelIni.listAppel) - 1 do
    begin
      if tabAppelIni.listAppel[i].classe.name = myClass.name then
      begin
        tabAppelIni.Executer(tabAppelIni.listAppel[i].id, contextObjet);
      end
    end;
    IsInitialized := true;
  except
    AfficheMessageErreur('TEntityInstance.InitializeCurrentInstance | Module ' +
      DBModule1.Module.Lookup('Id', tabAppelIni.listAppel[i].id, 'NomCours'),
      UModelsManage);
  end;
end;

procedure TEntityInstance.SetLastSimuleEnreg(idSimule: string);
//Utilise l'enregistrement $$Temp$$ comme premier enregistrement si il y a lieu
//utilis� pour sauvegarder les variables des tables en �criture lors de simulations cons"cutives
var
  j: integer;
  masterS: TDataSource;

begin
  try
    for j := 0 to length(inouttables) - 1 do
      if not inOutTables[j].readOnly then
        with inOutTables[j] do
          if not readOnly then
          begin
            //pour chaque table en �criture
            masterS := myTable.MasterSource;
            myTable.MasterSource := nil;
            myTable.gotobookmark(mybookmark);
            myTable.Edit;
            if myTable.FieldByName('idSimule').AsString = '$$idTmp$$' then
              //si le champs temporaire existe alors on l'utilise pour la simulation en cours
              myTable.FieldByName('idSimule').AsString := idSimule;
            myTable.Post;

            //            myTable.FreeBookmark(myBookMark);
            mybookmark := myTable.GetBookmark;
            myTable.MasterSource := masterS;
            myTable.gotobookmark(mybookmark);
          end;

  except
    AfficheMessageErreur('TEntityInstance.SetLastSimuleEnreg ', UModelsManage);
  end;
end;
//Fin Ajout VN
//------------------------------------------------------------------------------
{
procedure TEntityInstance.RempliTabAttributsChargees();
var i:integer;
begin
try
  DBModule1.TableEntity.Filter := 'CodeEntity = '+IntToStr(myClass.code);
  DBModule1.TableEntity.Filtered := True;
  setlength(inOutTables,DBModule1.TableEntity.RecordCount);
  for i :=0 to  DBModule1.TableEntity.RecordCount-1 do
    begin
    TabAttributsChargees[i]:= TAccessRecord.Create(TTable(DBModule1.Components[RetourneIndiceTable(DBModule1.TableEntitySource.Value)]),DBModule1.TableEntityStep.Value,DBModule1.TableEntityReadOnly.Value,LaDate);
    DBModule1.TableEntity.Next;
    end;
  DBModule1.TableEntity.Filtered := False;
  DBModule1.TableEntity.Filter := '';
except
  AfficheMessageErreur('TEntityInstance.CreerBookmark | Date '+DateToStr(laDate),UModelsManage);
end;
Try

except
  AfficheMessageErreur('TEntityInstance.RempliTabAttributsChargees',UModelsManage);
end;
end;

procedure TEntityInstance.MAJTabAttributsChargees();
var I:integer;
begin
Try
  if Length(TabAttributsChargees)=0 then mainForm.memDeroulement.Lines.Add('Le tableau TabAttributsChargees est vide');
  for I :=0 to High(TabAttributsChargees)-1 do TabAttributsChargees[I].MAJTabValeursAttributsChargees();
except
  AfficheMessageErreur('TEntityInstance.RempliTabAttributsChargees',UModelsManage);
end;
end; }

//------------------------------------------------------------------------------

// Recuperer la valeur en fonction du nom de l'attribut

function TEntityInstance.GetVal(nameAttr: string): Double;
begin
  try
    result := GetVal(myClass.Trouver(nameAttr));
  except
    AfficheMessageErreur('TEntityInstance.GetVal (surcharg� String)| Nom de l''attribut '
      + nameAttr, UModelsManage);
  end;
end;

// Recuperer la valeur en fonction du codeVar

function TEntityInstance.GetVal(indexAttr: Integer): Double;
begin
  try
    result := GetVal(myClass.Trouver(indexAttr));
  except
    AfficheMessageErreur('TEntityInstance.GetVal (surcharg� Integer)| Index de l''attribut '
      + intToStr(indexAttr), UModelsManage);
  end;
end;

// Recuperer la valeur en fonction du Tattribute

function TEntityInstance.GetVal(theAttr: TAttribute): Double;
begin
  try
    //if ((theAttr is TAttributeField) or (theAttr is TAttributChampCharge)) then
    if theAttr is TAttributeField then
      result := theAttr.GetVal
    else
      result := tabValues[TAttributeTemp(theAttr).index];
  except
    AfficheMessageErreur('TEntityInstance.GetVal (surcharg� TAttribute)| Nom de l''attribut: '
      + theAttr.GetName, UModelsManage);
  end;
end;

// Changer la valeur en fonction du nom

procedure TEntityInstance.SetVal(nameAttr: string; val: Double);
begin
  try
    SetVal(myClass.Trouver(nameAttr), val);
  except
    AfficheMessageErreur('TEntityInstance.SetVal (surcharg� String)| Nom de l''attribut '
      + nameAttr + 'Valeur ' + FloatToStr(val), UModelsManage);
  end;
end;

// Changer la valeur en fonction du codeVar

procedure TEntityInstance.SetVal(indexAttr: Integer; val: Double);
begin
  try
    SetVal(myClass.Trouver(indexAttr), val);
  except
    AfficheMessageErreur('TEntityInstance.SetVal (surcharg� Integer) | Index de l''attribut '
      + IntToStr(indexAttr) + 'Valeur ' + FloatToStr(val), UModelsManage);
  end;
end;

// Changer la valeur en fonction du Tattribute

procedure TEntityInstance.SetVal(theAttr: TAttribute; val: Double);
begin
  try
    //if ((theAttr is TAttributeField) or (theAttr is TAttributChampCharge)) then
    if theAttr is TAttributeField then
      theAttr.SetVal(val)
    else
    begin
      //ShowMessage(IntToStr(length(tabValues))+' '+IntToStr(TAttributeTemp(theAttr).index)+' '+FloatToStr(val));
      tabValues[TAttributeTemp(theAttr).index] := val;
    end;
  except
    AfficheMessageErreur('TEntityInstance.SetVal (surcharg� TAttribute) | Nom de l''attribut '
      + theAttr.GetName + 'Valeur ' + FloatToStr(val), UModelsManage);
  end;
end;

////////------------------------------------------------------/////////
////////                Classe TTabClasses
////////------------------------------------------------------/////////

constructor TTabClasses.Create;
begin
  listClass := nil;
end;

destructor TTabClasses.Destroy;
var
  i: Integer;
begin

  for i := 0 to length(listClass) - 1 do
  begin
    tabClass.ListClass[i].Free;
  end;
  listClass := nil;
  inherited Destroy;
end;

// Ajoute un type de classe dans la liste
//VN simcons
//procedure TTabClasses.Ajouter(nameClass : String);

function TTabClasses.Ajouter(nameClass: string): TEntityClass;
begin
  try
{$IFDEF AffichageComplet}
    MainForm.affiche((TimeToStr(Time) + #9 +
      'TTabClasses.Ajouter ( ' + nameClass + ' )',0);
{$ENDIF}

    DBModule1.Entite.MasterSource := nil;
    if DBModule1.Entite.Locate('NomEntite', nameClass, []) then
    begin
      SetLength(listClass, High(listClass) + 2);
        { TODO -oViB : � remplacer par length(listClass)+1 non? }
      result := TEntityClass.Create(nameClass,
        DBModule1.EntiteCodeEntite.value);
      listClass[High(listClass)] := result;
      //VN simcons listClass[High(listClass)] :=TEntityClass.Create(nameClass,DBModule1.EntiteCodeEntite.value);
    end;
    DBModule1.Entite.MasterSource := DBModule1.DSDictionnaireVar;
      { TODO -oViB : et le master field??? }
  except
    AfficheMessageErreur('TTabClasses.Ajouter | Nom de la classe ' + nameClass,
      UModelsManage);
  end;
end;

//-----------------------------------VN Simcons--------------------------------------

procedure TTabClasses.CreationClasses;
// Requete pour cr�ation automatique des classes(site, crop ....)
// La requete trie les entit�s en fonction de leur hierarchie et celles
// dont la hierarchie est = 0 ne sont pas retenues. Les entit�s sont ensuite
// cr�es dans cet ordre.
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
      aClass := ajouter(DbModule1.RequeteEntitesNomEntite.Value);
      aClass.AttributeFromTable(DbModule1.RequeteEntitesNomEntite.Value);
      DbModule1.RequeteEntites.Next;
        // 16/03/04 casse ici apres avoir fait un forcage qui fonctionne sur un autre modele FREEED
    end;

    DbModule1.RequeteEntites.Active := false;
  except
    AfficheMessageErreur('TSimule.CreationClasses', UClasseSimule);
  end;
end;
//------------------------------------------------------------------------------------------

// Retrouve une classe a partir de son nom

function TTabClasses.Trouver(nameClass: string): TEntityClass;
var
  i: Integer;
begin
  try
    i := 0;
    while (i <= High(listClass)) and (listClass[I].name <> nameClass) do
      inc(I);
    if i > High(listClass) then
      result := nil
    else
      result := listClass[I];
  except
    AfficheMessageErreur('TTabClasses.Trouver (surcharg� String) | Nom de la classe '
      + nameClass, UModelsManage);
  end;
end;

// Retrouve une classe a partir de son code

function TTabClasses.Trouver(codeClass: ShortInt): TEntityClass;
var
  i: Integer;
begin
  try
    i := 0;
    while (i <= High(listClass)) and (listClass[I].code <> codeClass) do
      Inc(i);
    if i > High(listClass) then
      result := nil
    else
      result := listClass[I];
  except
    AfficheMessageErreur('TTabClasses.Trouver (surcharg� ShortInt) | Num�ro de la classe '
      + IntToStr(codeClass), UModelsManage);
  end;
end;

////////------------------------------------------------------/////////
////////                Classe TAppel
////////------------------------------------------------------/////////

constructor TAppel.Create(idA, code, leStep: Integer; nameProc: string);
begin
  id := IdA;
  theProc := tabProc.Trouver(nameProc);
  classe := tabClass.Trouver(code);
  stepExe := leStep;
  nextExe := 0;
  tabArg := nil;
end;
//-------------------------------VN Save--------------------------------------

constructor TAppel.Create(sample: TAppel);
var
  
  i: integer;
begin
  try
    id := sample.id;
    stepExe := sample.stepExe;
    nextExe := sample.nextExe;
    theProc := sample.theProc;
    classe := sample.classe;

    SetLength(tabArg, length(sample.tabArg));
    for i := 0 to length(sample.tabArg) - 1 do
      tabArg[i] := sample.tabArg[i];

    SetLength(tabParam, length(sample.tabParam));
    for i := 0 to length(sample.tabParam) - 1 do
      tabParam[i] := sample.tabParam[i];

    SetLength(tabIndiceParamOutX, length(sample.tabIndiceParamOutX));
    for i := 0 to length(sample.tabIndiceParamOutX) - 1 do
      tabIndiceParamOutX[i] := sample.tabIndiceParamOutX[i];

    SetLength(tabIndiceParamInX, length(sample.tabIndiceParamInX));
    for i := 0 to length(sample.tabIndiceParamInX) - 1 do
      tabIndiceParamInX[i] := sample.tabIndiceParamInX[i];

  except
    AfficheMessageErreur('TAppel.CloneAppel ', UModelsManage);
  end;
end;
//-------------------------------Fin VN --------------------------------------

destructor TAppel.Destroy;
var
  i: Integer;
begin
  for i := 0 to length(tabArg) - 1 do
    tabArg[i].Free;
  inherited Destroy;
end;

{ TODO -ofred :
si rangArg = 10 et high(tabArg) = 5      => le tableau grandi de 5
seul la derniere case est remplie ?
voir la methode suivante qui n'est pas coh�rente au niveau des indices}
// Ajoute un argument a la liste

procedure TAppel.DefArgument(rangArg: Integer; a: TArgument);
begin
  try
    if rangArg - 1 > High(tabArg) then
      SetLength(tabArg, rangArg);
    tabArg[rangArg - 1] := A;
  except
    AfficheMessageErreur('TAppel.DefArgument | Rang de l''argument' +
      IntToStr(rangArg) + ' Argument ' + a.ClassName, UModelsManage);
  end;
end;

// Ajoute un parametre a la liste

procedure TAppel.DefParam(rangParam: Integer; nomParam: string; typeParam:
  TEnumTypeParam);
begin
  try

    if rangParam > High(tabParam) then
      SetLength(tabParam, rangParam + 1);
    tabParam[rangParam] := TParam.Create(nomParam, typeParam);

    case typeParam of
      kIn, kInOut:
        begin
          SetLength(TabIndiceParamInX, length(TabIndiceParamInX) + 1);
            // modif fred high+2 => length+1
          TabIndiceParamInX[High(TabIndiceParamInX)] := RangParam;
        end;
    end; //Case
    case typeParam of
      kOut, kInOut:
        begin
          SetLength(TabIndiceParamOutX, length(TabIndiceParamOutX) + 1);
            // modif fred high+2 => length+1
          TabIndiceParamOutX[High(TabIndiceParamOutX)] := RangParam;
        end;
    end; //Case
  except
    AfficheMessageErreur('TAppel.DefParam | Rang du param�tre' +
      IntToStr(rangParam) + ' nom du param�tre ' + nomParam, UModelsManage);
  end;
end;

////////---------------------------------------------------------------/////////
////////                      Classe TTabAppel                         /////////
////////---------------------------------------------------------------/////////

constructor TTabAppels.Create();
begin
  listAppel := nil;
end;

destructor TTabAppels.Destroy;
var
  i: Integer;
begin
  for i := length(listAppel) - 1 downto 0 do
    listAppel[i].Free;
  listAppel := nil;
  inherited Destroy;
end;

//-----------------------------VN simcons--------------------------------

procedure TTabAppels.Executer(idAppel: Integer; context: TContext);
// Execute un appel en connaissant son rang dans la liste
// Proc�dure: 1 r�cup�ration des valeurs des param�tres In etInOut
//            2 ex�cution de la proc�dure identifi�e par idAppel
//            3 restitution des valeurs des param�tres InOut et Out
var
  aCall: TAppel;
  args: TPointeurProcParam;
  i: Integer;


begin
  try
    aCall := Trouver(idAppel);
    SetLength(args, high(aCall.tabParam) + 1);
    { TODO : BUG: l'appel de high(aCall.tabParam) dans le cas d'un module dont la
     d�claration des param�tres se situent � la fin de la table ModuleVar retourne
     n+1 param�tre...  soit avec le +1 qui suit, on a un tableau
     de n+2 param�tres... le param�tre en trop est une copie du dernier param�tre.
     Le Pb vient certainement de la valeur retourn�e par high() quand aCall.tabParam est vide?}

    // 1- pr�paration de tous les param�tres IN et INOUT
    // dans aProc.TabIndiceParamInX on a sp�cifi� les indices
    // des param en entree defini dans le tableau aProc.TabParam

    //showmessage('Etape 1');

    //if (idAppel=33276) then
      //showmessage('Etape 1');

    for i := 0 to high(aCall.tabIndiceParamInX) do
    begin
      with TArgument(aCall.TabArg[aCall.TabIndiceParamInX[i]]) do
      begin
        if context.GetCurrentInstance(theClass) <> nil then
          // si l'entit�e existe
          args[aCall.TabIndiceParamInX[i]] :=
            context.GetCurrentInstance(theClass).GetVal(theAttribute)
        else // si elle n'existe pas, on met la valeur nulle
          args[aCall.TabIndiceParamInX[i]] := 0;
      end;
    end;


    if aCall.theProc is TProc then //avant, y'avait pas
    begin
      TProc(aCall.theProc).Pointeur(args); //avant, y'avait un ;
    end
    else if aCall.theProc is TProcdll then
    begin
      TProcDll(aCall.theProc).Pointeur(args) //avant, y'avait pas
    end
    else {//AjoutVN} if aCall.theProc is TProcContrainte then
      begin

        aSimulation.MajInVar_ToPars(aCall.TabIndiceParamInX, aCall.tabParam,
          args); //Mise a jour des variables d'entr�e du parser
        StartExeConstraint(TProcContrainte(aCall.theProc).Corps,
          aSimulation.Pars, 1);
        aSimulation.MajOutVar_ToProcParam(aCall.tabIndiceParamOutX,
          aCall.tabParam, args);
          //Mise a jour du tableau des variables de sorties du parser
        //ShowMessage(floattostr(contextObjet.GetCurrentInstance('Soil').GetVal('ArriveeEauJour'))+' '+floattostr(contextObjet.GetCurrentInstance('Plot').GetVal('ArriveeEauJour_out')));
        //if idAppel<>33236 then    ShowMessage(DBModule1.Resjour.FieldByName('ArriveeEauJour_out').AsString);
      end;

    //if (idAppel=33229) then ShowMessage('AFTER '+IntToStr(length(listappel)));
    //aCall := Trouver(idAppel);// on re-r�cup�re le Tappel qui � pu chang� si on a sauvegard�/charg�.
    //Fin Ajout VN
    //context:=contextObjet;

    //showmessage('Etape 3');

      //if (idAppel=33276) then
      //showmessage('Etape 3');

    // 3- traitement de tous les param�tres OUT et INOUT, on y restitu les valeurs modifi�es par la proc�dure
    for i := 0 to high(aCall.TabIndiceParamOutX) do
    begin
      with TArgument(aCall.TabArg[aCall.TabIndiceParamOutX[i]]) do
        context.GetCurrentInstance(theClass).SetVal(theAttribute,
          args[aCall.TabIndiceParamOutX[i]]);
    end;
  except
    on E: Exception do
      AfficheMessageErreur(E.message +
        ' TTabAppels.Executer | Identifiant de l''appel ' + IntToStr(acall.id) +
        ' Contexte ' + context.ClassName, UModelsManage);
    // AfficheMessageErreur(' TTabAppels.Executer | Identifiant de l''appel '+IntToStr(idAppel)+ ' Contexte '+context.ClassName,UModelsManage);
  end;
end;

// Ajoute un TAppel a la liste

procedure TTabAppels.Ajouter(aCall: TAppel);
begin
  try
    SetLength(listAppel, High(listAppel) + 2);
    listAppel[High(listAppel)] := aCall;
  except
    AfficheMessageErreur('TTabAppels.Ajouter | Appel ' + aCall.ClassName,
      UModelsManage);
  end;
end;

// Trouve un TAppel a partir de son identifiant

function TTabAppels.Trouver(idAppel: Integer): TAppel;
var
  i: Integer;
begin
  try
    i := 0;
    while (i <= High(listAppel)) and (listAppel[i].id <> idAppel) do
      Inc(i);
    if i > High(listAppel) then
      result := nil
    else
      result := listAppel[I];
  except
    AfficheMessageErreur(' TTabAppels.Trouver | Identifiant de l''appel ' +
      IntToStr(idAppel), UModelsManage);
  end;
end;

//--------------------------VN Simcons--------------------------------------

function conversion(x: string): TEnumTypeParam;
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
    AfficheMessageErreur('TSimule.conversion | Type de param�tre ' + x,
      UClasseSimule);
  end;
end;

//--------------------------VN Simcons--------------------------------------

{ TContextSave }

//Sauvegarde les valeur des �l�ments du TContext sujets � modification au cours de l'execution d'une simulation

constructor TContextSave.Create(contexte: TContext);
var
  i, j: integer;
  tableOut: TTable;
begin
  try
    SetLength(bookmarks, length(contexte.currentInstances));
    SetLength(tempValues, length(contexte.currentInstances));
    SetLength(instances, length(contexte.currentInstances));
    SetLength(NextExes, length(contexte.currentInstances));
    SetLength(Filters, length(contexte.currentInstances));
    //On dimensionne les tableaux

    for i := 0 to length(contexte.currentInstances) - 1 do
      //Pour chaque instance non nulle
    begin
      if contexte.currentInstances[i] <> nil then
      begin
        SetLength(bookmarks[i],
          length(contexte.currentInstances[i].inouttables));
        SetLength(NextExes[i],
          length(contexte.currentInstances[i].inouttables));
        SetLength(Filters[i], length(contexte.currentInstances[i].inouttables));

        for j := 0 to length(contexte.currentInstances[i].inouttables) - 1 do
        begin
          bookmarks[i, j] :=
            contexte.currentInstances[i].inouttables[j].myBookMark;
          Filters[i, j] :=
            contexte.currentInstances[i].inouttables[j].myTable.Filter;
          NextExes[i, j] :=
            contexte.currentInstances[i].inouttables[j].nextDate;
        end;
        //Sauvegarde des bookmarks de chaques tables de l'instance
        //Sauvegarde de la date de prochaine mise � jour de chaque table pour chaque instance
        //Sauvegarde des filtres des tables (en particulier les table avec un pas de -1)

        SetLength(tempValues[i],
          length(contexte.currentInstances[i].TabValues));
        for j := 0 to length(contexte.currentInstances[i].TabValues) - 1 do
          tempValues[i, j] := contexte.currentInstances[i].TabValues[j];
        //Sauvegarde des valeurs des variables temporaire de chaque instance

        instances[i] := 1;
        //On sauvegarde la pr�sence de l'instance

      end
      else
        instances[i] := 0;
      //On sauvegarde l'absence de l'instance
    end;

    //......................................................................
    //On sauvegarde l'�tat courant des tables en �criture,
    //permet de prendre en compte les sauvegarde en plein milieu de pas de temps

    DBModule1.queGenerique.Active := false;
    DBModule1.queGenerique.SQL.Clear;
    DBModule1.queGenerique.SQL.add('Select Source from '':DBModele:TableEntity.db'' where ReadOnly=''false'';');
    DBModule1.queGenerique.Active := true;
    //On recupere toutes les tables en �criture

    setlength(Results, DBModule1.queGenerique.RecordCount);
    //on redimensionne le tableau en fonction du nombre de tables en �criture

    DBModule1.queGenerique.First;
    for i := 0 to DBModule1.queGenerique.RecordCount - 1 do
      //Pour chaque table on sauvegarde le contenu de l'enregistrement courant dans le tableau Results.
    begin
      tableOut :=
        TTable(DBModule1.findcomponent(DBModule1.queGenerique.Fields[0].Value));

      SetLength(Results[i], tableOut.FieldCount);
      for j := tableOut.IndexFieldCount to tableOut.FieldCount - 1 do
        Results[i, j] := tableOut.Fields[j].AsFloat;

      DBModule1.queGenerique.Next;
    end;
  except
    AfficheMessageErreur('TContextSave.Create ', UClasseSimule);
  end;
end;

//Destruction des �l�ments

destructor TContextSave.Destroy;
begin
  bookmarks := nil;
  NextExes := nil;
  Filters := nil;
  tempValues := nil;
  instances := nil;
  Results := nil;

  inherited;
end;

//Utilise la sauvegarde pour �craser les valeurs du Tcontext.
//

procedure TContextSave.UseSave(contexte: Tcontext);
var
  i, j: integer;
  homeTable: TTable;
  tableOut: TTable;

begin
  try
    //......................................................................
    //On parcours toutes les instances pour y �craser les valeurs sauvegard�es
    for i := 0 to length(Instances) - 1 do
    begin
      if Instances[i] = 1 then
      begin
        if contexte.currentInstances[i] = nil then
          //si on a sauvegard� une instance qui n'est plus, on la recr�e avant d'ecraser les valeurs
        begin
          HomeTable :=
            TTable(DBModule1.FindComponent(dBModule1.RequeteInstanceHomeTable.Value));
          //on recupere la table m�re de l'entit� afin de pouvoir nommer l'instance

          contexte.SetCurrentInstance(TEntityInstance.Create(contexte.classesIndexes[i].name, 0, HomeTable.FindField('Id').value));
          //Cr�ation de l'instance perdue
        end;

        //on �crase les valeurs qui ont pu changer au cours de la simulation:
        for j := 0 to length(bookmarks[i]) - 1 do
        begin
          if Filters[i, j] <> '' then
            contexte.currentInstances[i].inouttables[j].myTable.Filtered :=
              false;
          contexte.currentInstances[i].inouttables[j].myBookMark := bookmarks[i,
            j];
          contexte.currentInstances[i].inouttables[j].nextDate := NextExes[i,
            j];
          contexte.currentInstances[i].inouttables[j].myTable.GotoBookmark(bookmarks[i, j]);
          if Filters[i, j] <> '' then
          begin
            contexte.currentInstances[i].inouttables[j].myTable.Filter :=
              Filters[i, j];
            contexte.currentInstances[i].inouttables[j].myTable.Filtered :=
              true;
          end;
        end;
        //bookmarks
        //Date de prochaine mise � jour des valeurs des tables
        //mise a jour de l'enregistrement actif
        //chargement du filtre sauvegard�

        for j := 0 to length(tempValues[i]) - 1 do
          contexte.currentInstances[i].TabValues[j] := tempValues[i, j];
        //Valeurs temporaires

      end
      else //if Instances[i]=1
      begin
        if contexte.currentInstances[i] <> nil then
          contexte.FreeEntityInstance(contexte.currentInstances[i]);
        //si lors de la sauvegarde l'instance �tait a nil mais qu'elle est pr�sente dans le contexte en cours, on la supprime

      end; //else  du  if Instances[i]=1
    end; //for

    while i < length(contexte.currentInstances) - 1 do
      //Tant qu'il reste des instance qui n'�tait pas pr�sente lors de la sauvegarde on les supprime
    begin
      contexte.FreeEntityInstance(contexte.currentInstances[i]);
      inc(i);
    end;

    //......................................................................
    //On met les valeur des tables en �criture � celles sauvegard�e:

    DBModule1.queGenerique.Active := false;
    DBModule1.queGenerique.SQL.Clear;
    DBModule1.queGenerique.SQL.add('Select Source from '':DBModele:TableEntity.db'' where ReadOnly=''false'';');
    DBModule1.queGenerique.Active := true;

    DBModule1.queGenerique.First;
    for i := 0 to DBModule1.queGenerique.RecordCount - 1 do
    begin
      tableOut :=
        TTable(DBModule1.findcomponent(DBModule1.queGenerique.Fields[0].Value));
      tableOut.Edit;
      for j := tableOut.IndexFieldCount to tableOut.FieldCount - 1 do
        tableOut.Fields[j].value := Results[i, j];
      tableOut.Post;
      DBModule1.queGenerique.Next;
    end;

  except
    AfficheMessageErreur('TContextSave.UseSave ', UClasseSimule);
  end;
end;

{ TTabAppelsSave }
//Sauvegarde les dates de prochaine execution de tous les tAppels

constructor TTabAppelsSave.Create(tab: TTabAppels);
var
  i: integer;
begin
  try
    SetLength(nextExes, length(tab.listappel));
    for i := 0 to length(tab.listappel) - 1 do
    begin
      nextExes[i] := tab.listappel[i].nextExe;
    end;
  except
    AfficheMessageErreur('TTabAppelsSave.Create ', UClasseSimule);
  end;
end;

destructor TTabAppelsSave.Destroy;
begin
  nextExes := nil;
  inherited;
end;

//Ecrase les date de prochaine execution des tappel de tab fourni en param�tre par ceux de la sauvegarde

procedure TTabAppelsSave.UseSave(tab: TTabAppels);
var
  i: integer;
begin
  try
    for i := 0 to length(tab.listappel) - 1 do
    begin
      tab.listappel[i].nextExe := nextExes[i];
    end;
  except
    AfficheMessageErreur('TTabAppelsSave.UseSave ', UClasseSimule);
  end;
end;

procedure SetTabAppels(codeSimule: string; IndexTable: integer);
// Construction de la liste des Tappels (construit une fois pour toute).
//Il en resulte le tableau tabAppelConsecutif qui contiendra 1 TtabAppel par simulation devant �tre executer cons�cutivement.
//Cette liste ne contiendra pas les modules d'initialisation qui eux sont contenus dans tabAppelIni

//Construit TabAppelConsecutif dans l'ordre d'execution des simulations execut� cons�cutivement
//Construit tabAppelIni avec une recursivit� invers� sur les mod�les
//Modele:         A     B     C
//Module d'init:  1     4     7
//Module d'init:  2     5     8
//Module d'init:  3     6

//TabAppelIni: {7,8, 4,5,6, 1,2,3}
var
  aCallProc: TAppel;
  j: Integer;

  TabAppelCorrespondant: TtabAppels;
  nextSim: string;
begin
  try
    nextSim := VarToStr((DBModule1.Simule.Lookup('id', codeSimule,
      'IdNextSimule')));
    //on recupere l'id de la simulation cons�cutive � celle la

    //......................................................................
    //Appel recursif en d�but pour inverser l'ordre d'execution des proc�dures d'initialisations:
    if nextSim <> '' then
      SetTabAppels(nextSim, IndexTable + 1)
        //appel recursif sur la simulation suivante
    else
      SetLength(tabAppelConsecutif, IndexTable + 1);
    //sinon on connait alors le nombre de simulations (donc de modeles)

    //......................................................................
    tabAppelConsecutif[IndexTable] := TTabAppels.Create;
    tabAppel := tabAppelConsecutif[IndexTable];
    //La premi�re est executer en dernier (cf recursivit�) au sortir de cette methode tabappel pointe
    //sur la premiere case de tabAppelConsecutif

    DBModule1.Simule.Locate('Id', CodeSimule, []);
    //La premi�re est executer en dernier (cf recursivit�) au sortir de cette methode
    //on est toujours sur la premi�re simulation
    DBModule1.Modele.Locate('Id', DBModule1.Simule.Fieldbyname('IdModele').Value,
      []);

    //......................................................................
    // ajout de tous les modules propres au mod�le

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

      if
        TTable(DBModule1.FindComponent('ModeleModule')).FieldByName('CodeExecution').AsInteger > 1 then
      begin
        TabAppelCorrespondant := tabAppelConsecutif[IndexTable];
      end
      else
        TabAppelCorrespondant := tabAppelIni;

      aCallProc :=
        TAppel.Create(DBModule1.Module.fieldbyname('CodeModule').Value,
        DBModule1.Module.fieldbyname('CodeEntity').Value,
        DBModule1.ModeleModuleStep.Value,
        DBModule1.Module.fieldbyname('NomModule').Value);

      TabAppelCorrespondant.ajouter(aCallProc);

      // Parcours des variables du module
      // (car action du DBModule1.Module.Locate pr�c�dent soit application de la
      // relation maitre d�tail entre ModuleVar et Module)
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

    //......................................................................
    //Pour �viter d'avoir le module 0 en doublons, on ne l'ajoute qu'une seule fois,
    // si c'est la derni�re simulation, on ajoute l'appel au module 0, le module d'initialisation g�n�ral

    if nextSim = '' then
    begin
      DBModule1.Module.Locate('CodeModule', 0, []);
      aCallProc :=
        TAppel.Create(DBModule1.Module.fieldbyname('CodeModule').Value,
        DBModule1.Module.fieldbyname('CodeEntity').Value,
        DBModule1.ModeleModuleStep.Value,
        DBModule1.Module.fieldbyname('NomModule').Value);
      tabAppelIni.ajouter(aCallProc);

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
    end

  except
    AfficheMessageErreur('TSimule.SetList', UClasseSimule);
  end;
end;

initialization
  tabProc := TTabProc.Create();
end.

