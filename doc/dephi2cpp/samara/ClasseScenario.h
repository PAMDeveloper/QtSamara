/**   @file  
     @brief  
*/
#pragma once
#ifndef INCLUDED_CLASSESCENARIO_H
#define INCLUDED_CLASSESCENARIO_H
/**------------------------------------------------------------------------------
  Classe Scenario
  Cette unit� permet de construire un objet sc�nario. Cette objet a pour fonction
  de g�rer tout le d�roulement d'une ou plusieurs simulations. Il va permettre de
  g�rer la cr�ation de toutes les variables utilis�es ainsi que le for�age des
  valeurs d'entr�es.
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


  /**Concept : caract�rise un ensemble de simulation.
    Description : Un sc�nario est une suite de simulation.
    Cet objet permet donc de g�rer la fa�on dont doivent �tre
    ex�cut�es les simulations du sc�nario.**/
  
    
struct/*class*/ TScenario: public TObject
{double tabSensibilite[ /*?*/ ][ /*?*/ ]; ///sert � stocker les valeurs des attributs pour l'�tude de sensibilit� ,tableau cr��e par la m�thode RemplirTableauSensibilite.

    /**--------------------------------------------------------------------------
      Proc�dure permettant d'ex�cuter les simulations du sc�nario suivant un
      type d'ex�cution (pr�-positionne �galement les pointeurs sur les
      tables utilis�es dans les simulations)
      @Author  CIRAD
      @Version v_3.0
      @param  typeExe 0 = une ex�cution normale, sans for�age 1= correspond �
      une ex�cution avec for�age 2=correspond � une ex�cution avec etude de
      sensibilit�  3=correspond � une ex�cution avec optimisation de param�tres
    ---------------------------------------------------------------------------*/
    void ExecuterScenario(int typeExe);


    /**--------------------------------------------------------------------------
      Proc�dure appel�e par ExecuterScenario, lancera les simulations pour
      l'optimisation, le for�age et l'ex�cution standard.
      Execution d'une simulation :
      On cr�� d'abord la classe simulation puis on cr�� l'objet simulation,
      on lance ensuite sa proc�dure bilanparcelle
      @Author  CIRAD
      @Version v_3.0
      @param  idSimule  id de la simulation
      @param  numSimule num�ro de la simulation
    ---------------------------------------------------------------------------*/
    void ExecuterSimulation(std::string idSimule,  int numSimule);

    /**--------------------------------------------------------------------------
      Proc�dure appel�e par ExecuterScenario, pour lancer les simulations
      de type �tude de sensibilit�.
      Gestion du lancement des simulations de type �tude de sensibilit�
      Cette proc�dure est une proc�dure r�cursive, les valeurs � incr�menter
      sont plac�es dans un tableau, la simulation est alors lanc�e pour
      chaque suite de valeurs possibles.

      @Author  CIRAD
      @Version v_3.0
      @param  indiceTab indice
    ---------------------------------------------------------------------------*/
    void Sensibilite(signed char indiceTab);

    /**--------------------------------------------------------------------------
      Proc�dure permettant de cr�er et remplir du tableau des
      valeurs de sensibilit�

      @Author  CIRAD
      @Version v_3.0
    ---------------------------------------------------------------------------*/
    void RemplirTableauSensibilite();

    /**--------------------------------------------------------------------------
      Proc�dure permettant de mettre � jour la table for�age en
      fonction du tableau des valeurs de sensibilit�.

      @Author  CIRAD
      @Version v_3.0
    ---------------------------------------------------------------------------*/
    void MAJTableForcage();

    /**--------------------------------------------------------------------------
      Proc�dure permettant de lancer des simulations consecutives.

      @Author  CIRAD
      @Version v_3.0
      @param  idSimule  id de la simulation
      @param  numSimule num�ro de la simulation
    ---------------------------------------------------------------------------*/
    void LanceSimulationsConsecutives(std::string idSimule,  int numSimule);

    /**--------------------------------------------------------------------------
      Proc�dure execSimule

      @Author  CIRAD
      @Version v_3.0
      @param  idSimule  id de la simulation
      @param  numSimule num�ro de la simulation
      @param  NumAnnSimule num�ro de l'ann�e simul�e
    ---------------------------------------------------------------------------*/
    void execSimul( std::string const& idSimule,  int numSimule, 
      int NumAnnSimule);
      
  
};



  
  
  TSimule aSimulation; /// Une Simulation
  TDateTime finLastSimul; /// Date de fin de simulation



#endif//INCLUDED_CLASSESCENARIO_H
//END
