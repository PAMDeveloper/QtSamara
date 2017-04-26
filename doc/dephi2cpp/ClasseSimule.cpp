/**   @file  
     @brief  
*/
#include "ClasseSimule.h"



 
#include "DBModule.h" 
#include "GestionDesTables.h" 
#include "main.h" 
#include "LectDonnee.h" 
#include "GestionDesErreurs.h" 
#include "Forms.h"

#include "ClasseScenario.h"

//----------------------------------------------------------------------------//
//VN simcons

void TSimule::Clean()
{
  try {
    Pars.free;
    while(  ContextSave.Count != 0 ) //on nettoit les sauvegardes
    {
      TContextSave( ContextSave.Objects[ 0 ] ).free;
      ContextSave.Delete( 0 );
    }
    ContextSave.free;

    while(  TabAppelSave.Count != 0 )
    {
      TTabAppelsSave( TabAppelSave.Objects[ 0 ] ).free;
      TabAppelSave.Delete( 0 );
    }
    TabAppelSave.free;
  
    } catch(...) {AfficheMessageErreur( "TSimule.Clean" , UClasseSimule );
  }
}

void TSimule::CreationClasses()
// Requete pour création automatique des classes(site, crop ....)
// La requete trie les entités en fonction de leur hierarchie et celles
// dont la hierarchie est = 0 ne sont pas retenues. Les entités sont ensuite
// crées dans cet ordre.
{
  TEntityClass aClass;

  // Recuperation de la liste des entites necessaires pour ce modele
  // Voici un exemple de la requete DbModule1.RequeteEntites :
  //    REPONSE CodeEntity      NomEntite       Hierarchie
  //    1       3               Site	        1
  //    2       1               Plot            2
  //    3       4               Soil            2
  //    4       2               Crop            3
  //    5       5               Plante          3
  try {
    DbModule1.RequeteEntites.Active = true;

    DbModule1.RequeteEntites.First;
    while(  !DbModule1.RequeteEntites.Eof )
    { /* TODO -ofred : dans le diag de classe 'ajouter' est une function, cela serait plus simple ? */
      tabClass.ajouter( DbModule1.RequeteEntitesNomEntite.Value );
      aClass = tabClass.Trouver( DbModule1.RequeteEntitesNomEntite.Value );
      aClass.AttributeFromTable( DbModule1.RequeteEntitesNomEntite.Value );
      DbModule1.RequeteEntites.Next;
        // 16/03/04 casse ici apres avoir fait un forcage qui fonctionne sur un autre modele FREEED
    }

    DbModule1.RequeteEntites.Active = false;
  
    } catch(...) {AfficheMessageErreur( "TSimule.CreationClasses" , UClasseSimule );
  }
}

//----------------------------------------------------------------------------//

void TSimule::InitSimulation( double & anDebutSimul, double & anFinSimul, double & nbAnSim, double & dateDebutSimul, double & dateFinSimul, double & annee, double & maxNbjSimule, double & dateEnCours, double & nbjSemis, double & dateSemisCalc, 
   double const& nbjas, double const& codeParcelle)
// Initialisation des divers paramètres d'entrée. Cette procédure correspond au module 0.
{
  try {
    // récupération des dates et écriture dans rescrop(ou tabValues) à la sortie de la procedure
    anDebutSimul = DbModule1.Simule.findfield( "AnDebutSimul" ).Value;
    anFinSimul = DbModule1.Simule.findfield( "AnFinSimul" ).Value;
    nbAnSim = DbModule1.Simule.findfield( "NbAnSim" ).Value;
    dateFinSimul = DbModule1.Simule.findfield( "FinSimul" ).Value;
    dateDebutSimul = DbModule1.Simule.findfield( "DebutSimul" ).Value;
    annee = DbModule1.Simule.findfield( "AnDebutSimul" ).Value;
    dateEnCours = DbModule1.Simule.findfield( "DebutSimul" ).Value;
    maxNbjSimule = Trunc( dateFinSimul - dateDebutSimul ) + 1;
    nbjSemis = NullValue;
    dateSemisCalc = NullValue;
  
    } catch(...) {AfficheMessageErreur( "TSimule.InitSimulation" , UClasseSimule );
  }
}

//----------------------------VN simcons-------------------------------------//

void TSimule::InitDatesSimule( int const& numSimul)
{
  unsigned short jour; unsigned short mois; unsigned short an;

  try {
    