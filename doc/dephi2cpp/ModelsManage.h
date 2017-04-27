/**   @file  
     @brief  
*/
#pragma once
#ifndef INCLUDED_MODELSMANAGE_H
#define INCLUDED_MODELSMANAGE_H
 
// Cette unite permet de gerer les objets generiques et leurs methodes
// Auteur(s)       :
// Unité(s) à voir : <unité ayant un lien fort>
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
  //           Objets liés aux procédures
  //--------------------------------------------------------------------------

  ///Concept : caractérise les valeurs des arguments d'une procédure.
  ///Description : Tableau de réel qui représente les valeurs des arguments d'une procédure (pour le modélisateur : les variables d'un module).
  typedef double TPointeurProcParam[ /*?*/ ];

  ///Concept : Pointeur sur une procédure.
  ///Description : Pointeur sur une procédure ayant comme argument un tableau de TpointeurProcParam (soit un tableau de réel).
  ///Contrainte : le nbre et type des param pour cette declaration en procedures doit etre identique, d'ou les toutes les procedures ont un tableau dynamique de double
  typedef void (*TPointeurProc)( TPointeurProcParam & tabParam);

  ///Concept : Pointeur sur une procédure hébergée dans une DLL.
  ///Description : Pointeur sur une procédure hébergée dans une DLL ayant comme argument un tableau de TpointeurProcParam (soit un tableau de réel).
  typedef void (*TPointeurProcDll)( TPointeurProcParam & tabParam); 