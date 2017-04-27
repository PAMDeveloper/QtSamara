{*------------------------------------------------------------------------------
  GestionDesErreurs
  Gestion Des Erreurs du moteur de Sarrah

  @Author  CIRAD
  @Version v_3.0
-------------------------------------------------------------------------------}
unit GestionDesErreurs;

interface
uses Dialogs,Main;

type
  /// Enum�ration des unit�s du moteur
  TUnite = (UModelsManage, UClasseSimule, UClasseScenario,
    UGestionDesTables, UBilEau, UPalmier, UMilBilanCarbone, UBhyTypeFAO,
    UBiomasse, UMeteo, UEcopalm2, UMathStats, UMain, ULectDonnee, UEcopalm2_2,
    URiz, {VN contraintes} UContrainte, USorghum, UPhenologie,
    UBilanCarbonSarra, UUImportExport,
    UDataModuleRecherche, UDeriveClimato, URisocas);


{*--------------------------------------------------------------------------
  Proc�dure permettant d'afficher un message d'erreur ainsi que l'unit� dans
  laquelle l'erreur s'est produite.
  @Author  CIRAD
  @Version v_3.0
  @param  LeMessage Le message a afficher
  @param  LUnite L'unit� d'o� provient l'erreur
---------------------------------------------------------------------------}
procedure AfficheMessageErreur(LeMessage: string; LUnite: TUnite);

implementation

{uses Main;}

var
  ErreurAffichee: Boolean;  /// Vrai si une erreur a �t� affich�e

{--------------------------------------------------------------------------
  Proc�dure permettant d'afficher un message d'erreur ainsi que l'unit� dans
  laquelle l'erreur s'est produite.
  @Author  CIRAD
  @Version v_3.0
  @param  LeMessage Le message a afficher
  @param  LUnite L'unit� d'o� provient l'erreur
---------------------------------------------------------------------------}
procedure AfficheMessageErreur(LeMessage: string; LUnite: TUnite);
var
  NomUnite: string; /// Nom de l'unit�
begin
  try
    //main.erreur := true;
    if not ErreurAffichee then
    begin
      case LUnite of
        UModelsManage: NomUnite := 'ModelsManage.pas';
        UClasseSimule: NomUnite := 'ClasseSimule.pas';
        UClasseScenario: NomUnite := 'ClasseScenario.pas';
        UGestionDesTables: NomUnite := 'GestionDesTables.pas';
        UBilEau: NomUnite := 'BilEau.pas';
        UPalmier: NomUnite := 'Palmier.pas';
        UMilBilanCarbone: NomUnite := 'MilBilanCarbone.pas';
        UBhyTypeFAO: NomUnite := 'BhyTypeFAO.pas';
        UBiomasse: NomUnite := 'Biomasse.pas';
        UMeteo: NomUnite := 'Meteo.pas';
        UEcopalm2: NomUnite := 'Ecopalm2.pas';
        UMathStats: NomUnite := 'MathStats.pas';
        UMain: NomUnite := 'Main.pas';
        ULectDonnee: NomUnite := 'LectDonnee.pas';
        UEcopalm2_2: NomUnite := 'Ecopalm2_2.pas';
        URiz: NomUnite := 'Riz.pas';
        USorghum: NomUnite := 'Sorghum.pas';
        UPhenologie: NomUnite := 'Phenologie.pas';
        UBilanCarbonSarra: NomUnite := 'BilanCarbonSarra.pas';
        UUImportExport: NomUnite := 'UImportExport.pas';
        UDeriveClimato: NomUnite := 'DeriveClimato.pas';
        {VN contraintes}UContrainte: NomUnite := 'Contrainte.pas';
        UDataModuleRecherche: NomUnite := 'DataModuleRecherche.pas';
        URisocas: NomUnite := 'risocas.pas';
      else
        NomUnite := '-- non renseign�--';
      end;
      ShowMessage('Unit� ' + NomUnite + ' | Erreur: ' + LeMessage);
      ErreurAffichee := True;

    end;
  except
    ShowMessage('Erreur g�n�rale grave... Affichage des erreurs impossible');
    ErreurAffichee := True;
  end;
end;

initialization
  ErreurAffichee := False;
end.

