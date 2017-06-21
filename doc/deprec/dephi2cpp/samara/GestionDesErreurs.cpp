/**   @file  
     @brief  
*/
#include "GestionDesErreurs.h"



/*uses Main;*/


  bool ErreurAffichee;  /// Vrai si une erreur a été affichée

/*--------------------------------------------------------------------------
  Procédure permettant d'afficher un message d'erreur ainsi que l'unité dans
  laquelle l'erreur s'est produite.
  @Author  CIRAD
  @Version v_3.0
  @param  LeMessage Le message a afficher
  @param  LUnite L'unité d'où provient l'erreur
---------------------------------------------------------------------------*/
void AfficheMessageErreur(std::string LeMessage,  TUnite LUnite)
{
  std::string NomUnite; /// Nom de l'unité

  try {
    //main.erreur := true;
    if(  !ErreurAffichee )
    {
       switch( LUnite ) {
        case UModelsManage:  NomUnite = "ModelsManage.pas";
        case UClasseSimule:  NomUnite = "ClasseSimule.pas";
        case UClasseScenario:  NomUnite = "ClasseScenario.pas";
        case UGestionDesTables:  NomUnite = "GestionDesTables.pas";
        case UBilEau:  NomUnite = "BilEau.pas";
        case UPalmier:  NomUnite = "Palmier.pas";
        case UMilBilanCarbone:  NomUnite = "MilBilanCarbone.pas";
        case UBhyTypeFAO:  NomUnite = "BhyTypeFAO.pas";
        case UBiomasse:  NomUnite = "Biomasse.pas";
        case UMeteo:  NomUnite = "Meteo.pas";
        case UEcopalm2:  NomUnite = "Ecopalm2.pas";
        case UMathStats:  NomUnite = "MathStats.pas";
        case UMain:  NomUnite = "Main.pas";
        case ULectDonnee:  NomUnite = "LectDonnee.pas";
        case UEcopalm2_2:  NomUnite = "Ecopalm2_2.pas";
        case URiz:  NomUnite = "Riz.pas";
        case USorghum:  NomUnite = "Sorghum.pas";
        case UPhenologie:  NomUnite = "Phenologie.pas";
        case UBilanCarbonSarra:  NomUnite = "BilanCarbonSarra.pas";
        case UUImportExport:  NomUnite = "UImportExport.pas";
        case UDeriveClimato:  NomUnite = "DeriveClimato.pas";
        /*VN contraintes*/case UContrainte:  NomUnite = "Contrainte.pas";
        case UDataModuleRecherche:  NomUnite = "DataModuleRecherche.pas";
        case URisocas:  NomUnite = "risocas.pas";
      default:
        NomUnite = "-- non renseigné--";
      }
      ShowMessage( "Unité " + NomUnite + " | Erreur: " + LeMessage );
      ErreurAffichee = true;

    }
  
    } catch(...) {ShowMessage( "Erreur générale grave... Affichage des erreurs impossible" );
    ErreurAffichee = true;
  }
}


#if 0 //INITIALIZATION

  ErreurAffichee = false;

#endif//INITIALIZATION


//END
