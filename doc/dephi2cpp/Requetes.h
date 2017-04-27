/**   @file  
     @brief  
*/
#pragma once
#ifndef INCLUDED_REQUETES_H
#define INCLUDED_REQUETES_H
/**------------------------------------------------------------------------------
  Requetes
  Requetes du moteur de Sarrah

  @Author  CIRAD
  @Version v_3.0
-------------------------------------------------------------------------------*/
 



  
  
#include "DBModule.h" 
#include "Windows.h" 
#include "Messages.h" 
#include "SysUtils.h" 
#include "Classes.h" 
#include "Graphics.h" 
#include "Controls.h" 
#include "Forms.h"
    
#include "Dialogs.h"
  
#include "Db.h" 
#include "DBTables.h"

void initTemp(); //AP requete
void execRequeteScenar( int const& CptSimul); // A Pas Requete
void requete(std::string r); //AP requete
void BatchResJour(); //AP requete
void deriveResJour(); //AP requete
void EnlevederiveResJour(); //AP requete
std::string FormeReqTemp(std::string ReqSel2, std::string ReqWhere2);
//modif CB 30/01/07 copie proc dans UimportExport dans gestionbdd
void SupprEnreg( TTable const& maTable,  std::string nomCol, std::string valChamps);
// suppression d'un enregistrement dans la table maTable.
// Condition : la valeur du champs correspondant à la colonne nomCol est égale à valChamps


  bool tabFrom[16];
    //liste les tables qu'on a besoin de mettre dans le FROM
  std::string cheminBdd; std::string mode;



#endif//INCLUDED_REQUETES_H
//END
