/**   @file  
     @brief  
*/
#pragma once
#ifndef INCLUDED_GESTIONDESERREURS_H
#define INCLUDED_GESTIONDESERREURS_H
/**------------------------------------------------------------------------------
  GestionDesErreurs
  Gestion Des Erreurs du moteur de Sarrah

  @Author  CIRAD
  @Version v_3.0
-------------------------------------------------------------------------------*/
 


 
#include "Dialogs.h"
#include "Main.h"


  /// Enum�ration des unit�s du moteur
  typedef enum TUnite { UModelsManage, UClasseSimule, UClasseScenario, UGestionDesTables, UBilEau, UPalmier, UMilBilanCarbone, UBhyTypeFAO, UBiomasse, UMeteo, UEcopalm2, UMathStats, UMain, ULectDonnee, UEcopalm2_2, URiz, UContrainte, USorghum, UPhenologie, UBilanCarbonSarra, UUImportExport, UDataModuleRecherche, UDeriveClimato, URisocas };


/**--------------------------------------------------------------------------
  Proc�dure permettant d'afficher un message d'erreur ainsi que l'unit� dans
  laquelle l'erreur s'est produite.
  @Author  CIRAD
  @Version v_3.0
  @param  LeMessage Le message a afficher
  @param  LUnite L'unit� d'o� provient l'erreur
---------------------------------------------------------------------------*/
void AfficheMessageErreur(std::string LeMessage,  TUnite LUnite);



#endif//INCLUDED_GESTIONDESERREURS_H
//END
