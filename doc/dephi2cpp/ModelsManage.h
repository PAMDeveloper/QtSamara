/**   @file  
     @brief  
*/
#pragma once
#ifndef INCLUDED_MODELSMANAGE_H
#define INCLUDED_MODELSMANAGE_H
 
// Cette unite permet de gerer les objets generiques et leurs methodes
// Auteur(s)       :
// Unit�(s) � voir : <unit� ayant un lien fort>
// Date de commencement          : avant 01/02/04 !
// Date de derniere modification : 16/03/04
// Etat : fini, teste
//{$DEFINE AffichageComplet}
//{$DEFINE Debug} // methode d'affichage dans TEI et TEC
//{$DEFINE AffichageTresComplet}



 
#include "DB.h" 
#include "DBTables.h" 
#include "Windows.h" 
#include "SysUtils.h" 
#include "Variants.h" 
#include "DateUtils.h"  /*VN save*/
#include "Dialogs.h"

#include "classes.h" 
#include "HashMap.h"
#include "DCL_intf.h"
#include "HashMapTable.h"


  //--------------------------------------------------------------------------
  //           Objets li�s aux proc�dures
  //--------------------------------------------------------------------------

  ///Concept : caract�rise les valeurs des arguments d'une proc�dure.
  ///Description : Tableau de r�el qui repr�sente les valeurs des arguments d'une proc�dure (pour le mod�lisateur : les variables d'un module).
  typedef double TPointeurProcParam[ /*?*/ ];

  ///Concept : Pointeur sur une proc�dure.
  ///Description : Pointeur sur une proc�dure ayant comme argument un tableau de TpointeurProcParam (soit un tableau de r�el).
  ///Contrainte : le nbre et type des param pour cette declaration en procedures doit etre identique, d'ou les toutes les procedures ont un tableau dynamique de double
  typedef void (*TPointeurProc)( TPointeurProcParam & tabParam);

  ///Concept : Pointeur sur une proc�dure h�berg�e dans une DLL.
  ///Description : Pointeur sur une proc�dure h�berg�e dans une DLL ayant comme argument un tableau de TpointeurProcParam (soit un tableau de r�el).
  typedef void (*TPointeurProcDll)( TPointeurProcParam & tabParam); 