/**   @file  
     @brief  
*/
#pragma once
#ifndef INCLUDED_CLASSESIMULE_H
#define INCLUDED_CLASSESIMULE_H
//
// Cette unite permet de gerer une simulation
// La procedure BilanParcelle est la plus importante
//
// Auteur(s)       :
// Unit�(s) � voir : <unit� ayant un lien fort>
//
// Date de commencement          :
// Date de derniere modification : 11/02/04
// Etat : fini, teste
//

//{$DEFINE AffichageComplet}

 



 
#include "ModelsManage.h" 
#include "SysUtils.h" 
#include "DB.h" 
#include "DBTables.h" 
#include "Variants.h" 
#include "DateUtils.h" 
#include "Dialogs.h"
/*ajout VN contraintes*/ 
#include "Contrainte.h" 
#include "Parser10.h" 
#include "Classes.h"


  ///Concept : caract�rise une simulation.
  ///Description : Cet objet repr�sente une simulation. Il permet de g�rer celle-ci. Les m�thodes de TSimule servent � construire le mod�le et l'appel des modules le composant.
  
struct/*class*/ TSimule: public TEntityInstance
    /*Ajout VN save*/
{TstringList ContextSave;
    /*Ajout VN save*/TstringList TabAppelSave; /*Ajout VN save*/
      //ContextSave:array of TContext;
    /*Ajout VN contraintes*/Parser10.TParser Pars;
      /// Parser Math�matique, pour le calcul des expression des contraintes.

    TDateTime thisDate; /// repr�sente la date en cours pendant la simulation
    bool calendaire;
      /// permet de sp�cifier si on calcule les dates en tenant compte du calendrier
    double minorStep;
      /// repr�sente le pas de temps le plus petit de la simulation -toujours la journ�e- .
    /*Ajout VN simcons*/void Clean();
    void InitSimulation( double & anDebutSimul, double & anFinSimul, double & nbAnSim, double & dateDebutSimul, double & dateFinSimul, double & annee, double & maxNbjSimule, double & dateEnCours, double & nbjSemis, double & dateSemisCalc,   double const& nbjas, double const& codeParcelle);
      /// Initialisation des divers param�tres d'entr�e. Cette proc�dure correspond au module 0.
    void InitYear( int const& numSimul);
      /// Initialisation des variables, cr�ation des instances initiales (comme l'instance Simulation, sol, etc... qui n'apparaissent pas en cours de simulation)
    void SetList();
      /// Construction de la liste des appels (construit une fois pour toute, cette liste ne contiendra pas les modules d'initialisation)
    //AD     procedure BilanParcelle(codeSimule : Integer);    ///La m�thode BilanParcelle est la m�thode appel�e par la m�thode ExecuterSimulation de la classe TScenario, permet de g�rer le d�roulement de la simulation
    void BilanParcelle(std::string idSimule);
    void SimulAn(); /// Permet la Gestion de la date de semis
    void CreationClasses();
      /// Requete pour cr�ation automatique des classes(site, crop ....)
    void RunModule(int numModule);
      /// Procedure d'appel d'un module avec son num�ro et le contexte associ� � la simulation
    void Aller( int const& numSimule);
      /// G�re le d�roullement de la simulation :
    void StepExe();
      /// Execute le module si la date d'execution correspond � la date en cours
    void CalculNextExe();
      /// Calcule la date suivante et met � jour le tableau des appels.
    void ConfirmeSemis(); /// Gestion de la date de semis
    void MajBookmarks( int const& numSimule);
      /// Mise � jour des bookmark pour chaque table si la date correspond :
    TEnumTypeParam Conversion(std::string x);
      /// Renvoie le type du parametre
    /*Ajout VN Contraintes*/void MajInVar_ToPars(signed char InIndice[ /*?*/ ], 
      ModelsManage.TParam TabParam[ /*?*/ ],  ModelsManage.TPointeurProcParam ParamVal); // Met a jour les variables du parser
    /*Ajout VN Contraintes*/void MajOutVar_ToProcParam(signed char OutIndice[ /*?*/ ],  ModelsManage.TParam TabParam[ /*?*/ ],   ModelsManage.TPointeurProcParam & ParamVal);
    /*Ajout VN simcons*///procedure InitInstances();
    /*Ajout VN simcons*/void InitDatesSimule( int const& numSimul);
    /*Ajout VN simcons*/void InitDatesSemis( int const& numSimul);
    /*Ajout VN save*/void majNbJas(TDateTime dateSemis, TDateTime dateDebut, TDateTime datefin);

  
};

 /*ajout APas*/
  int numSimule2;
    //represente le num�ro de la simulation en cours pour simulations pluriannuelles



#endif//INCLUDED_CLASSESIMULE_H
//END
