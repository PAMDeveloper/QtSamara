/**   @file  
     @brief  
*/
#pragma once
#ifndef INCLUDED_CLASSESCENARIO_H
#define INCLUDED_CLASSESCENARIO_H
/**------------------------------------------------------------------------------
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
-------------------------------------------------------------------------------*/

//{$DEFINE AffichageComplet}

 



 
#include "Windows.h" 
#include "SysUtils.h" 
#include "StrUtils.h" 
#include "DB.h"  
#include "Dialogs.h" 
#include "ClasseSimule.h"

#include "Variants.h"
#include "HashMapTable.h"


  /**Concept : caractérise un ensemble de simulation.
    Description : Un scénario est une suite de simulation.
    Cet objet permet donc de gérer la façon dont doivent être
    exécutées les simulations du scénario.**/
  
    
struct/*class*/ TScenario: public TObject
{double tabSensibilite[ /*?*/ ][ /*?*/ ]; ///sert à stocker les valeurs des attributs pour l'étude de sensibilité ,tableau créée par la méthode RemplirTableauSensibilite.

    /**--------------------------------------------------------------------------
      Procédure permettant d'exécuter les simulations du scénario suivant un
      type d'exécution (pré-positionne également les pointeurs sur les
      tables utilisées dans les simulations)
      @Author  CIRAD
      @Version v_3.0
      @param  typeExe 0 = une exécution normale, sans forçage 1= correspond à
      une exécution avec forçage 2=correspond à une exécution avec etude de
      sensibilité  3=correspond à une exécution avec optimisation de paramètres
    ---------------------------------------------------------------------------*/
    void ExecuterScenario(int typeExe);


    /**--------------------------------------------------------------------------
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
    void ExecuterSimulation(std::string idSimule,  int numSimule);

    /**--------------------------------------------------------------------------
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
    void Sensibilite(signed char indiceTab);

    /**--------------------------------------------------------------------------
      Procédure permettant de créer et remplir du tableau des
      valeurs de sensibilité

      @Author  CIRAD
      @Version v_3.0
    ---------------------------------------------------------------------------*/
    void RemplirTableauSensibilite();

    /**--------------------------------------------------------------------------
      Procédure permettant de mettre à jour la table forçage en
      fonction du tableau des valeurs de sensibilité.

      @Author  CIRAD
      @Version v_3.0
    ---------------------------------------------------------------------------*/
    void MAJTableForcage();

    /**--------------------------------------------------------------------------
      Procédure permettant de lancer des simulations consecutives.

      @Author  CIRAD
      @Version v_3.0
      @param  idSimule  id de la simulation
      @param  numSimule numéro de la simulation
    ---------------------------------------------------------------------------*/
    void LanceSimulationsConsecutives(std::string idSimule,  int numSimule);

    /**--------------------------------------------------------------------------
      Procédure execSimule

      @Author  CIRAD
      @Version v_3.0
      @param  idSimule  id de la simulation
      @param  numSimule numéro de la simulation
      @param  NumAnnSimule numéro de l'année simulée
    ---------------------------------------------------------------------------*/
    void execSimul( std::string const& idSimule,  int numSimule, 
      int NumAnnSimule);
      
  
};



  
  
  TSimule aSimulation; /// Une Simulation
  TDateTime finLastSimul; /// Date de fin de simulation



#endif//INCLUDED_CLASSESCENARIO_H
//END
