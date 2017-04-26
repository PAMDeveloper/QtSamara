unit MilBilanCarbone;

////////-----------------------------------------------------////////
// Cette unité regroupe l'ensemble des procédures permettant
// le calcul de l'évolution du bilan carbonné pour la culture
// annuelle du Mil. Ces procédures sont basées sur
// l'unité biomasse.pas (de C. Baron) avec quelques modifications
// pour permettre une meilleur interchangabilité des modules du modèle.
// Toute variable préfixée "Delta" représente l'augmentation ou la diminution
// de la valeur qu'elle représente.
// Exemple: DeltaBiomasseTotale représente la quantité d'assimilat créé dans la
// journée par la culture, ce n'est pas un cumul.
//
// Auteur(s)       : V. BONNAL d'après J.C. Combres
// Unité(s) à voir : BilEau.pas, Biomasse.pas
// Date de commencement          : 21/06/2004
// Date de derniere modification : 25/06/2004
// Etat : en cours d'implémentation
////////-----------------------------------------------------////////

interface

uses Math,SysUtils,Dialogs;

implementation

uses GestionDesErreurs,Main,ModelsManage;

Procedure InitiationCulture(Const SeuilTempLevee,SeuilTempBVP,SeuilTempRPR,
                            SeuilTempMatu1,SeuilTempMatu2:Double;
                            var SommeDegresJourMaximale,NumPhase,BiomasseAerienne,
                            BiomasseVegetative,BiomasseTotale,BiomasseTiges,
                            BiomasseRacinaire,BiomasseFeuilles,SommeDegresJour,
                            DeltaBiomasseTotale,SeuilTempPhaseSuivante,Lai:Double);
begin
try
  NumPhase:=0;
  SommeDegresJourMaximale:=SeuilTempLevee+SeuilTempBVP+SeuilTempRPR+SeuilTempMatu1+SeuilTempMatu2;
  SommeDegresJour:=0;
  BiomasseAerienne := 0;
  BiomasseVegetative := 0;
  BiomasseTotale := 0;
  BiomasseTiges := 0;
  BiomasseRacinaire := 0;
  BiomasseFeuilles := 0;
  DeltaBiomasseTotale := 0;
  SeuilTempPhaseSuivante:=0;
  Lai:=0;
except
  AfficheMessageErreur('InitiationCulture',UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

Procedure EvalDateLevee(const StockRacinaire:Double;
                        var NumPhase,ChangePhase:Double);  { TODO : Jamis utilisé ds le modèle... voir avec JCC) }
// à ce jour, cette procédure ne fait que vérifier la viabilité du semi
begin
try
  if ((NumPhase=1) and (StockRacinaire=0)) then
    begin
    NumPhase:=7; // tuer la culture, fin de simulation { TODO : mettre un avertissement et vérifier qu'on arrête bien...}
    ChangePhase:=0;
    mainForm.memDeroulement.Lines.Add('####### LEVEE IMPOSSIBLE ######')
    end;
except
  AfficheMessageErreur('EvalDateLevee | StockRacinaire: '+FloatToStr(StockRacinaire)+
                       ' NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

Procedure EvalDegresJourTOpt(const TOpt, TMoy, TBase:double; var DegresDuJour:Double);
begin
try
  DegresDuJour:=Max(Min(TOpt,TMoy),TBase)-TBase;
  mainForm.memDeroulement.Lines.Add('## Degrès du jour : '+floattostr(DegresDuJour)+'°');
except
  AfficheMessageErreur('EvalDegresJour',UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

Procedure EvolPhenologieMilv2(const SeuilPP,SommeDegresJour,SeuilTempLevee,
                         SeuilTempBVP,SeuilTempRPR,SeuilTempMatu1,
                         SeuilTempMatu2,StockSurface,PourcRuSurfGermi,RuSurf,
                         SlaMax,BiomasseTotale,DateDuJour,DateSemis:Double;
                         Var SumPP,NumPhase,SommeDegresJourPhasePrec,
                         SeuilTempPhaseSuivante,BiomasseTotaleStadeIp,
                         BiomasseTotaleStadeFlo,CycleReel,ChangePhase,
                         DateMaturite:Double);
// Cette procédure est appelée en début de journée et fait évoluer les phase
// phénologiques. Pour celà, elle incrémente les numéro de phase et change la
// valeur du seuil de la phase suivante. ChangePhase est un booléen permettant
// d'informer le modèle pour connaître si un jour est un jour de changement
// de phase. Celà permets d'initialiser les variables directement dans les
// modules spécifiques.
// --> Stades phénologiques pour le modèle Mil réécrit:
// 0 : du jour de semis au début des conditions favorables pour la germination
// 1 : du début des conditions favorables pour la germination au jour de la levée
// 2 : du jour de la levée au début de la floraison
// 3 : du début de la floraison au début de la photosensibilité
// 4 : du début de la photosensibilité au début de la maturation
// 5 : du début de la maturation au début du séchage
// 6 : du début du séchage au jour de récolte
// 7 : du jour de récolte à la fin de la simulation
var ChangementDePhase:Boolean;  // on passe en variable un pseudo booléen et non directement ce booléen (pb de moteur)
begin
try
  ChangePhase:=0; // l'initialisation quotidiènne de cette variable à faux permet de stopper le marquage d'une journée de changement de phase
  mainForm.memDeroulement.Lines.Add('phase n°'+FloatToStr(NumPhase)+' StockSurface='+FloatToStr(StockSurface));
  if NumPhase=0 then // la culture a été semée mais n'a pas germé
    begin
    if StockSurface>=PourcRuSurfGermi*RuSurf then
      begin // on commence ds les conditions favo aujourd'hui
      NumPhase:=1;
      mainForm.memDeroulement.Lines.Add('------> phase n°'+FloatToStr(NumPhase)+' (car StockSurface='+FloatToStr(StockSurface)+')');
      ChangePhase:=1;
      SeuilTempPhaseSuivante := SeuilTempLevee;  { TODO : à vérif par JCC, le déclencheur étant en phase 0 les conditions favorables et non SeuilTempGermi }
      end;
 //   else    { TODO : Pas logique: inutile donc à revoir... }
 //     begin // on vérifie ne pas être arrivé à la fin de la simulation
 //     if SommeDegresJour>=SommeDegresJourMaximale then
 //       NumPhase:=7; // pas de germination réalisée  { TODO : mettre un avertissement }
 //       ChangePhase:=True;
 //     end;
    end // fin du if NumPhase=0
else
    begin
    // vérification d'un éventuel changement de phase
    If NumPhase <> 3 Then
      ChangementDePhase := (SommeDegresJour >= SeuilTempPhaseSuivante)
    else  // Phase photopériodique
      ChangementDePhase := (sumPP >= seuilPP);

    If ChangementDePhase then // on a changé de phase
      begin
      ChangePhase:=1;
      NumPhase := NumPhase + 1;
      mainForm.memDeroulement.Lines.Add('------> phase n°'+FloatToStr(NumPhase));
      SommeDegresJourPhasePrec := SeuilTempPhaseSuivante; // utilisé dans EvalConversion
      Case Trunc(NumPhase) of
        2 : begin   // BVP Developpement vegetatif
            
            SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempBVP;
            end;
        3 : SumPP := 0;   // PSP Photoperiode
        4 : begin   // RPR Stade initiation paniculaire
            SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempRPR;
            BiomasseTotaleStadeIp := BiomasseTotale;
            end;
        5 : begin   // Matu1 remplissage grains
            SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu1;
            BiomasseTotaleStadeFlo:=BiomasseTotale;
            end;
        6 : SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu2; // Matu2 dessication
        7 : begin // Recolte
            CycleReel := DateDuJour-DateSemis;
            DateMaturite := DateDuJour;
            // sortie de RendementDeLaCulture
            end;
        end;  // Case NumPhase
      end;  // end change
    end;
except
  AfficheMessageErreur('EvolPhenologieTemperature | NumPhase: '+FloatToStr(NumPhase)+
                       ' SommeDegresJour: '+FloatToStr(SommeDegresJour)+
                       ' SeuilTempPhaseSuivante: '+FloatToStr(SeuilTempPhaseSuivante) ,UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalConversion(Const NumPhase,EpsiB,AssimBVP,SommeDegresJour,
                         SommeDegresJourPhasePrecedente,AssimMatu1,AssimMatu2,
                         SeuilTempPhaseSuivante:Double;
                         Var Conversion:Double);
Var
  KAssim   : Double;
begin
try
  Case trunc(NumPhase) of
    2..4 : KAssim := AssimBVP;
    5    : KAssim := AssimBVP +(SommeDegresJour-SommeDegresJourPhasePrecedente)*
                     (AssimMatu1 -AssimBVP)/(SeuilTempPhaseSuivante-SommeDegresJourPhasePrecedente);
    6    : KAssim := AssimMatu1+(SommeDegresJour-SommeDegresJourPhasePrecedente)*
                     (AssimMatu2 -AssimMatu1)/(SeuilTempPhaseSuivante-SommeDegresJourPhasePrecedente);
    else
           KAssim := 0;
    end;
  Conversion:=KAssim*EpsiB;
except
  AfficheMessageErreur('EvalConversion | NumPhase: '+FloatToStr(NumPhase)+
                       ' SommeDegresJour: '+FloatToStr(SommeDegresJour),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalParIntercepte(Const PAR,LTR:Double; var PARIntercepte:Double);
begin
try
  PARIntercepte:=PAR*(1-LTR);
except
  AfficheMessageErreur('EvalParIntercepte | PAR: '+FloatToStr(PAR)+
                       ' LTR: '+FloatToStr(LTR),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalAssimPot(const PARIntercepte,Conversion:Double; Var AssimPot:Double);
begin
try
  AssimPot:=PARIntercepte*Conversion*10;
except
  AfficheMessageErreur('EvalAssimPot | PAR Intercepté: '+FloatToStr(PARIntercepte)+
                       ' Conversion: '+FloatToStr(Conversion),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalCstrAssim(const Cstr:Double; var CstrAssim:Double);
// formule simple permettant un éventuel changement de calcul (pour arachide par ex)
begin
try
  CstrAssim:=Cstr;
except
  AfficheMessageErreur('EvalCstrAssim | Cstr: '+FloatToStr(Cstr),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalVitesseRacinaire(Const RootSpeedBVP,RootSpeedRPR,RootSpeedPSP,
                               RootSpeedMatu1,RootSpeedMatu2,NumPhase:Double;
                               var VitesseRacinaire:Double);
begin
try
  Case Trunc(NumPhase) of
    1..2 : VitesseRacinaire := RootSpeedBVP ;
    3    : VitesseRacinaire := RootSpeedPSP ;
    4    : VitesseRacinaire := RootSpeedRPR ;
    5    : VitesseRacinaire := RootSpeedMatu1; { TODO : attention en cas de gestion du champ vide... }
    6    : VitesseRacinaire := RootSpeedMatu2;
    else
           VitesseRacinaire := 0
    end;
Except
  AfficheMessageErreur('EvalVitesseRacinaire | NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------
procedure EvalAssim(Const AssimPot,CstrAssim:Double; var Assimilats:Double);
begin
try
  Assimilats:=AssimPot*CstrAssim;
except
  AfficheMessageErreur('EvalAssim | AssimPot: '+FloatToStr(AssimPot)+
                       ' CstrAssim: '+FloatToStr(CstrAssim),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalDeltaBiomTot(const Assimilats,RespMaint,NumPhase:Double;
                                   var DeltaBiomasseTotale:Double);
begin
try
  if NumPhase>=2 then
    DeltaBiomasseTotale:=Assimilats-RespMaint
  else
    DeltaBiomasseTotale:=0;
except
  AfficheMessageErreur('EvalBiomasseTotaleDuJour | Assim: '+FloatToStr(Assimilats)+
                       ' RespMaint: '+FloatToStr(RespMaint)+
                       ' NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalRdtPotTotGra(Const KRdt,BiomTotStadeFloraison,BiomTotStadeIp,NumPhase:Double;
                            Var RdtPot:Double);
begin
try
  if NumPhase<5 then
    RdtPot:=0
  else
    RdtPot:=KRdt*(BiomTotStadeFloraison-BiomTotStadeIp);
except
  AfficheMessageErreur('EvalRdtPotTotGra | KRdt: '+FloatToStr(KRdt)+
                       ' BiomTotStadeFloraison: '+FloatToStr(BiomTotStadeFloraison)+
                       ' BiomTotStadeIp: '+FloatToStr(BiomTotStadeIp)+
                       ' NumPhase: '+Floattostr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalRdtPotDuJour(Const NumPhase,RdtPot,DegresDuJour,SeuilTempMatu1:Double;
                           Var RdtPotDuJour:Double);
begin
try
  if NumPhase=5 then
    RdtPotDuJour := RdtPot*((DegresDuJour)/SeuilTempMatu1)
  else
    RdtPotDuJour:=0;
except
  AfficheMessageErreur('EvalRdtPotDuJour | NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalReallocation(Const RdtPotDujour,DeltaBiomasseTotale,KRealloc,NumPhase:Double;
                           Var Reallocation,ManqueAssim:Double);
begin
try
  If NumPhase=5 then
    begin
    ManqueAssim:=Max(0,(RdtPotDuJour - max(0,DeltaBiomasseTotale)));
    if DeltaBiomasseTotale<0 then
      Reallocation:=0
    else
      Reallocation:= ManqueAssim*KRealloc;
    end
  else
    begin
    ManqueAssim:=0;
    Reallocation:=0;
    end;
except
  AfficheMessageErreur('EvalRealloc | RdtPotDujour: '+FloatToStr(RdtPotDujour)+
                       ' DeltaBiomasseTotale: '+FloatToStr(DeltaBiomasseTotale)+
                       ' KRealloc: '+FloatToStr(KRealloc)+
                       ' NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvolBiomasseTotale(Const DeltaBiomasseTotale,NumPhase,Densite,
                             KResGrain,BiomasseGrain,ChangePhase:Double;
                             Var BiomasseTotale:Double);
begin
try
  if ((NumPhase=2) and (ChangePhase=1)) then  // initialisation
    BiomasseTotale := Densite * KResGrain * BiomasseGrain/1000 //Biomasse initiale au jour de la levée
  else
    BiomasseTotale:=BiomasseTotale+DeltaBiomasseTotale; // pas de gestion de phase,car gérée en amont
except
  AfficheMessageErreur('EvolBiomTot | BiomasseTotale: '+FloatToStr(BiomasseTotale)+
                       ' DeltaBiomasseTotale: '+FloatToStr(DeltaBiomasseTotale),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvolBiomasseAerienne(Const DeltaBiomasseTotale,RatioAero,BiomasseTotale,NumPhase,
                               KPenteAero,KBaseAero,ChangePhase:Double;
                               Var {DeltaBiomasseAerienne,}BiomasseAerienne:Double);
var
BiomasseAeriennePrec:Double;
begin
try
  if NumPhase<=1 then // la levée n'a pas eu lieu encore
    begin
    //DeltaBiomasseAerienne:=0;
    BiomasseAerienne:=0;
    end
  else
    begin // BVP et sup
    if ((NumPhase=2) and (ChangePhase=1)) then  // initialisation
      begin
      BiomasseAerienne := Min(0.9, KPenteAero * BiomasseTotale + KBaseAero)* BiomasseTotale;  { TODO -oViB : passer ce 0,9 en paramètre? }
     // DeltaBiomasseAerienne:=BiomasseAerienne;
      end
    else
      begin
      BiomasseAeriennePrec := BiomasseAerienne;
      if DeltaBiomasseTotale < 0 Then
        BiomasseAerienne := max(0,BiomasseAerienne + DeltaBiomasseTotale)
      else
        BiomasseAerienne := RatioAero*BiomasseTotale;
     // DeltaBiomasseAerienne := BiomasseAerienne - BiomasseAeriennePrec;
      end;
    end;
except
  AfficheMessageErreur('EvolBiomAero | DeltaBiomasseTotale: '+FloatToStr(DeltaBiomasseTotale)+
                       ' RatioAero: '+FloatToStr(RatioAero)+
                       ' BiomasseTotale: '+FloatToStr(BiomasseTotale)+
                       ' BiomasseAerienne: '+FloatToStr(BiomasseAerienne),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalBiomasseRacinair(Const BiomasseTotale,BiomasseAerienne:Double; Var BiomasseRacinaire:Double);
begin
try
  BiomasseRacinaire:=BiomasseTotale-BiomasseAerienne;
except
  AfficheMessageErreur('EvolBiomasseRacinair | BiomasseTotale: '+FloatToStr(BiomasseTotale)+
                        ' BiomasseAerienne: '+FloatToStr(BiomasseAerienne)+
                        ' BiomasseRacinaire: '+FloatToStr(BiomasseRacinaire),UMilBilanCarbone);
end;
end;
////////------------------------------------------------------------------------

procedure EvalAllomTotAer(Const KPenteAero,BiomTot,KBaseAero:Double;
                                Var RatioAero:Double);
begin
try
  RatioAero:=Min(0.9,KPenteAero*BiomTot + KBaseAero);
except
  AfficheMessageErreur('EvolAllomTotAer | KPenteAero: '+FloatToStr(KPenteAero)+
                       ' BiomTot: '+FloatToStr(BiomTot)+
                       ' KBaseAero: '+FloatToStr(KBaseAero),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalAllomAeroFeuilV1(Const NumPhase,kBaseLaiDev,kPenteLaiDev,BiomasseAerienne:Double;
                                    Var RatioFeuilles:Double);
// BiomFeuille=RatioFeuilles * BiomAero
Var bM,CM:Double;
begin
try
  if ((NumPhase>1) AND (NumPhase<=4)) then // donc compris entre la phase d'emergence et reproductive inclus
    begin
    bM :=  kBaseLaiDev - 0.1;
    cM := ((kPenteLaiDev*1000)/ bM + 0.78)/0.75;
    RatioFeuilles := (0.1 + bM * power(cM,BiomasseAerienne/1000)); { TODO : qu'est ce  ce / 1000? }
    end
  else
    RatioFeuilles := 0; 
except
  AfficheMessageErreur('EvolAllomAeroFeuilV1',UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalAllomAeroFeuilV2(Const Ma,Mb,Mc,BiomasseAerienne,Densite,NumPhase:Double;
                                     Var RatioFeuilles:Double);
begin
try
  if ((NumPhase>=1) AND (NumPhase <= 4)) then
    begin
    RatioFeuilles := (Ma + Mb * power(Mc,BiomasseAerienne/Densite));
    end
  else
    RatioFeuilles := 0;
except
  AfficheMessageErreur('EvolAllomAeroFeuilV2',UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvolBiomasseFeuilles(Const DeltaBiomasseTotale,PartFeuillesTiges,
                               NumPhase,RatioFeuilles, Reallocation,
                               BiomasseAerienne,ChangePhase,kBaseLaiDev,
                               KPenteLaiDev:Double;
                               Var DeltaBiomFeuilles,BiomFeuilles:Double);
var BiomasseFeuillePrec:Double;
begin
try
  BiomasseFeuillePrec:=BiomFeuilles;
  if NumPhase<=1 then // la levée n'a pas eu lieu encore
    Biomfeuilles:=0
  else
    begin
    if ((NumPhase=2) and (ChangePhase=1)) then
      BiomFeuilles:=(kBaseLaiDev+KPenteLaiDev*BiomasseAerienne)*BiomasseAerienne
    else
      begin
      If NumPhase <= 4 then // de la levée à la phase reproductive
        begin
        if DeltaBiomasseTotale <0 Then
          Biomfeuilles:= max(0,Biomfeuilles + DeltaBiomasseTotale*PartFeuillesTiges)
        else
          BiomFeuilles:= RatioFeuilles *BiomasseAerienne;
        end
      else // de la Maturation à la fin de la simulation { TODO -oViB : que se passe t'il en phase 7 après la récolte? }
        BiomFeuilles:= max(0,BiomFeuilles - (Reallocation-min(0,DeltaBiomasseTotale))*PartFeuillesTiges) ;
      end;
    end;
  DeltaBiomFeuilles:=BiomFeuilles-BiomasseFeuillePrec;
  
except
  AfficheMessageErreur('EvolBiomFeuille | DeltaBiomasseTotale: '+FloatToStr(DeltaBiomasseTotale)+
                       ' BiomasseAerienne: '+FloatToStr(BiomasseAerienne)+
                       ' NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

Procedure EvalSlaRapBiomV2(Const SlaBVP, SlaRPR,KpenteSla, dayBiomLeaf, BiomLeaf,
                           NumPhase,ChangePhase: double;
                           Var sla : Double);
begin
try
  if ((NumPhase=2) and (ChangePhase=1)) then
    Sla := SlaBVP // soit SlaMax...
  else
    begin
    if NumPhase>=2 then
      begin
      If dayBiomLeaf >0 then
        sla := (sla - KpenteSla * (sla- SlaRPR))* (BiomLeaf- dayBiomLeaf)/BiomLeaf+ SlaBVP * (dayBiomLeaf/BiomLeaf);
      sla := min(SlaBVP,max(SlaRPR , sla));
      end
    else
      Sla:=0;
    end;
except
  AfficheMessageErreur('EvalSlaRapBiomV2',UMilBilanCarbone);
end;
End;

////////------------------------------------------------------------------------

procedure EvolBiomasseTiges(Const DeltaBiomasseTotale,NumPhase,PartFeuillesTiges,
                            Reallocation,BiomasseFeuilles,BiomasseAerienne:Double;
                            Var BiomasseTiges:Double);
begin
try
  if NumPhase<=1 then // la levée n'a pas eu lieu encore
    BiomasseTiges:=0
  else
    begin
    if NumPhase <= 4 then // de la phase germi à la phase reproductive
      begin
      if DeltaBiomasseTotale <0 then
        BiomasseTiges:= max(0, BiomasseTiges + DeltaBiomasseTotale*(1-PartFeuillesTiges))
      else
        BiomasseTiges:= BiomasseAerienne - BiomasseFeuilles ;
      end
    else // de la photosensible à la fin de la simulation { TODO -oViB : que se passe t'il en phase 7 après la récolte? }
      BiomasseTiges:= max(0, BiomasseTiges - (Reallocation-min(0,DeltaBiomasseTotale))*(1-PartFeuillesTiges)); // ce qui est réalloué est pris dans les feuilles et ds les tiges
    end;                                          { TODO : KRealloc ou Reallocation ??? }
except
  AfficheMessageErreur('EvolBiomasseTiges',UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalBiomasseVegetati(Const BiomasseTiges,BiomasseFeuilles,NumPhase:Double;
                                 Var BiomasseVegetative:Double);
begin
try      
  BiomasseVegetative:=BiomasseTiges+BiomasseFeuilles; // pas de gestion de phase,car gérée en amont
except
  AfficheMessageErreur('EvolBiomasseVegetati | BiomasseTiges: '+FloatToStr(BiomasseTiges)+
                       ' BiomasseFeuille: '+FloatToStr(BiomasseFeuilles),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalDeltaRdt(Const DeltaBiomasseTotale,Reallocation,NumPhase,
                       RdtPotDuJour:Double;
                       Var DRdt:Double);
begin
try
  if NumPhase=5 then
    DRdt:=Min(RdtPotDuJour,Max(0,DeltaBiomasseTotale)+Reallocation)
  else
    DRdt:=0;
except
  AfficheMessageErreur('EvalRdtDuJour | DeltaBiomasseTotale: '+FloatToStr(DeltaBiomasseTotale)+
                       ' Reallocation: '+FloatToStr(Reallocation)+
                       ' NumPhase: '+Floattostr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvolRdtV2(Const DRdt,NumPhase:Double; Var Rdt:Double);
begin
try
  if NumPhase<5 then
    Rdt := 0
  else  
    Rdt := Rdt+DRdt;
except
  AfficheMessageErreur('EvolRdtV2 | DRdt: '+FloatToStr(DRdt)+
                       ' Rdt: '+FloatToStr(Rdt),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvolSomDegresJour(Const DegresDuJour,NumPhase:Double;
                            Var SommeDegresJour:Double);
begin
//try
  if NumPhase>=1 then // on ne cumule qu'après la germination
    SommeDegresJour := SommeDegresJour+DegresDuJour
  else
    SommeDegresJour:=0;
//except
//  AfficheMessageErreur('EvolSommeDegresJour | DegresDuJour: '+FloatToStr(DegresDuJour)+
//                       ' Phase n°'+FloatToStr(NumPhase)+
 //                      ' SommeDegresJour: '+FloatToStr(SommeDegresJour)+
//                       ' SommeDegresJour: '+FloatToStr(SommeDegresJour),UMilBilanCarbone);
//end;
end;

////////////////////////////////////////////////////////////////////////////////////
// Liste de toutes les procedures redef en dyn de l'unite
// Rajouter stdcall à la fin pour permettre l'utilisation de procédures dans des dll.
/////////////////////////////////////////////////////////////////////////////////////


Procedure InitiationCultureDyn (Var T : TPointeurProcParam);
Begin
InitiationCulture(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
                  T[10],T[11],T[12],T[13],T[14],T[15],T[16]);
end;
Procedure EvalDateLeveeDyn (Var T : TPointeurProcParam);
Begin
EvalDateLevee(T[0],T[1],T[2]);
end;
Procedure EvalDegresJourTOptDyn (Var T : TPointeurProcParam);
Begin
EvalDegresJourTOpt(T[0],T[1],T[2],T[3]);
end;
Procedure EvolPhenologieMilv2Dyn (Var T : TPointeurProcParam);
Begin
EvolPhenologieMilv2(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
              T[10],T[11],T[12],T[13],T[14],T[15],T[16],T[17],T[18],T[19],
              T[20],T[21],T[22]);
end;
Procedure EvalConversionDyn (Var T : TPointeurProcParam);
Begin
EvalConversion(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8]);
end;
Procedure EvalSlaRapBiomV2Dyn (Var T : TPointeurProcParam);
Begin
EvalSlaRapBiomV2(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;
Procedure EvalParIntercepteDyn (Var T : TPointeurProcParam);
Begin
EvalParIntercepte(T[0],T[1],T[2]);
end;
Procedure EvalAssimPotDyn (Var T : TPointeurProcParam);
Begin
EvalAssimPot(T[0],T[1],T[2]);
end;
Procedure EvalCstrAssimDyn (Var T : TPointeurProcParam);
Begin
EvalCstrAssim(T[0],T[1]);
end;
Procedure EvalVitesseRacinaireDyn (Var T : TPointeurProcParam);
Begin
EvalVitesseRacinaire(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
Procedure EvalAssimDyn (Var T : TPointeurProcParam);
Begin
EvalAssim(T[0],T[1],T[2]);
end;
Procedure EvalDeltaBiomTotDyn (Var T : TPointeurProcParam);
Begin
EvalDeltaBiomTot(T[0],T[1],T[2],T[3]);
end;
Procedure EvalRdtPotTotGraDyn (Var T : TPointeurProcParam);
Begin
EvalRdtPotTotGra(T[0],T[1],T[2],T[3],T[4]);
end;
Procedure EvalRdtPotDuJourDyn (Var T : TPointeurProcParam);
Begin
EvalRdtPotDuJour(T[0],T[1],T[2],T[3],T[4]);
end;
Procedure EvalReallocationDyn (Var T : TPointeurProcParam);
Begin
EvalReallocation(T[0],T[1],T[2],T[3],T[4],T[5]);
end;
Procedure EvolBiomasseTotaleDyn (Var T : TPointeurProcParam);
Begin
EvolBiomasseTotale(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
Procedure EvolBiomasseAerienneDyn (Var T : TPointeurProcParam);
Begin
EvolBiomasseAerienne(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;
Procedure EvalBiomasseRacinairDyn (Var T : TPointeurProcParam);
Begin
EvalBiomasseRacinair(T[0],T[1],T[2]);
end;
Procedure EvalAllomTotAerDyn (Var T : TPointeurProcParam);
Begin
EvalAllomTotAer(T[0],T[1],T[2],T[3]);
end;
Procedure EvalAllomAeroFeuilV1Dyn (Var T : TPointeurProcParam);
Begin
EvalAllomAeroFeuilV1(T[0],T[1],T[2],T[3],T[4]);
end;
Procedure EvalAllomAeroFeuilV2Dyn (Var T : TPointeurProcParam);
Begin
EvalAllomAeroFeuilV2(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
Procedure EvolBiomasseFeuillesDyn (Var T : TPointeurProcParam);
Begin
EvolBiomasseFeuilles(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10]);
end;
Procedure EvolBiomasseTigesDyn (Var T : TPointeurProcParam);
Begin
EvolBiomasseTiges(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
Procedure EvalBiomasseVegetatiDyn (Var T : TPointeurProcParam);
Begin
EvalBiomasseVegetati(T[0],T[1],T[2],T[3]);
end;
Procedure EvalDeltaRdtDyn (Var T : TPointeurProcParam);
Begin
EvalDeltaRdt(T[0],T[1],T[2],T[3],T[4]);
end;
Procedure EvolRdtV2Dyn (Var T : TPointeurProcParam);
Begin
EvolRdtV2(T[0],T[1],T[2]);
end;
Procedure EvolSomDegresJourDyn (Var T : TPointeurProcParam);
Begin
EvolSomDegresJour(T[0],T[1],T[2]);
end;

initialization
TabProc.AjoutProc('InitiationCulture', InitiationCultureDyn);
TabProc.AjoutProc('EvalDateLevee', EvalDateLeveeDyn);
TabProc.AjoutProc('EvalDegresJourTOpt', EvalDegresJourTOptDyn);
TabProc.AjoutProc('EvolPhenologieMilv2', EvolPhenologieMilv2Dyn);
TabProc.AjoutProc('EvalConversion', EvalConversionDyn);
TabProc.AjoutProc('EvalParIntercepte', EvalParIntercepteDyn);
TabProc.AjoutProc('EvalAssimPot', EvalAssimPotDyn);
TabProc.AjoutProc('EvalCstrAssim', EvalCstrAssimDyn);
TabProc.AjoutProc('EvalVitesseRacinaire', EvalVitesseRacinaireDyn);
TabProc.AjoutProc('EvalAssim', EvalAssimDyn);
TabProc.AjoutProc('EvalDeltaBiomTot', EvalDeltaBiomTotDyn);
TabProc.AjoutProc('EvalRdtPotTotGra', EvalRdtPotTotGraDyn);
TabProc.AjoutProc('EvalRdtPotDuJour', EvalRdtPotDuJourDyn);
TabProc.AjoutProc('EvalReallocation', EvalReallocationDyn);
TabProc.AjoutProc('EvolBiomasseTotale', EvolBiomasseTotaleDyn);
TabProc.AjoutProc('EvolBiomasseAerienne', EvolBiomasseAerienneDyn);
TabProc.AjoutProc('EvalBiomasseRacinair', EvalBiomasseRacinairDyn);
TabProc.AjoutProc('EvalAllomTotAer', EvalAllomTotAerDyn);
TabProc.AjoutProc('EvalAllomAeroFeuilV1', EvalAllomAeroFeuilV1Dyn);
TabProc.AjoutProc('EvalAllomAeroFeuilV2', EvalAllomAeroFeuilV2Dyn);
TabProc.AjoutProc('EvolBiomasseFeuilles', EvolBiomasseFeuillesDyn);
TabProc.AjoutProc('EvolBiomasseTiges', EvolBiomasseTigesDyn);
TabProc.AjoutProc('EvalBiomasseVegetati', EvalBiomasseVegetatiDyn);
TabProc.AjoutProc('EvalDeltaRdt', EvalDeltaRdtDyn);
TabProc.AjoutProc('EvolRdtV2', EvolRdtV2Dyn);
TabProc.AjoutProc('EvolSomDegresJour', EvolSomDegresJourDyn);
TabProc.AjoutProc('EvalSlaRapBiomV2',EvalSlaRapBiomV2Dyn);
end.


