unit MilBilanCarbone;

////////-----------------------------------------------------////////
// Cette unit� regroupe l'ensemble des proc�dures permettant
// le calcul de l'�volution du bilan carbonn� pour la culture
// annuelle du Mil. Ces proc�dures sont bas�es sur
// l'unit� biomasse.pas (de C. Baron) avec quelques modifications
// pour permettre une meilleur interchangabilit� des modules du mod�le.
// Toute variable pr�fix�e "Delta" repr�sente l'augmentation ou la diminution
// de la valeur qu'elle repr�sente.
// Exemple: DeltaBiomasseTotale repr�sente la quantit� d'assimilat cr�� dans la
// journ�e par la culture, ce n'est pas un cumul.
//
// Auteur(s)       : V. BONNAL d'apr�s J.C. Combres
// Unit�(s) � voir : BilEau.pas, Biomasse.pas
// Date de commencement          : 21/06/2004
// Date de derniere modification : 25/06/2004
// Etat : en cours d'impl�mentation
////////-----------------------------------------------------////////

interface

uses Math,SysUtils,Dialogs;

implementation

uses GestionDesErreurs,Main,ModelsManage;

procedure InitiationCulture(const SeuilTempLevee, SeuilTempBVP, SeuilTempRPR,
                            SeuilTempMatu1, SeuilTempMatu2 : double;
                            var SommeDegresJourMaximale, NumPhase, BiomasseAerienne,
                            BiomasseVegetative, BiomasseTotale, BiomasseTiges,
                            BiomasseRacinaire, BiomasseFeuilles, SommeDegresJour,
                            DeltaBiomasseTotale, SeuilTempPhaseSuivante, Lai : Double);
begin
  try
    NumPhase := 0;
    SommeDegresJourMaximale := SeuilTempLevee + SeuilTempBVP + SeuilTempRPR + SeuilTempMatu1 + SeuilTempMatu2;
    SommeDegresJour := 0;
    BiomasseAerienne := 0;
    BiomasseVegetative := 0;
    BiomasseTotale := 0;
    BiomasseTiges := 0;
    BiomasseRacinaire := 0;
    BiomasseFeuilles := 0;
    DeltaBiomasseTotale := 0;
    SeuilTempPhaseSuivante:=0;
    Lai := 0;
  except
    AfficheMessageErreur('InitiationCulture',UMilBilanCarbone);
  end;
end;

////////------------------------------------------------------------------------

Procedure EvalDateLevee(const StockRacinaire:Double;
                        var NumPhase,ChangePhase:Double);  { TODO : Jamis utilis� ds le mod�le... voir avec JCC) }
// � ce jour, cette proc�dure ne fait que v�rifier la viabilit� du semi
begin
try
  if ((NumPhase=1) and (StockRacinaire=0)) then
    begin
    NumPhase:=7; // tuer la culture, fin de simulation { TODO : mettre un avertissement et v�rifier qu'on arr�te bien...}
    ChangePhase:=0;
    mainForm.memDeroulement.Lines.Add(TimeToStr(Time)+#9+'####### LEVEE IMPOSSIBLE ######')
    end;
except
  AfficheMessageErreur('EvalDateLevee | StockRacinaire: '+FloatToStr(StockRacinaire)+
                       ' NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------
// La temp�rature moyenne calcul�e est born�e � Topt
Procedure EvalDegresJourTOpt(const TOpt, TMoy, TBase:double; var DegresDuJour:Double);
begin
try
  DegresDuJour:=Max(Min(TOpt,TMoy),TBase)-TBase;
except
  AfficheMessageErreur('EvalDegresJour',UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------
// Proc�dure classique en bornant la temp�rature maxi � Topt et la mini � Tbase
// avec cette proc. saisir Topt1 = Topt2
Procedure EvalDegresJourTborne(const TMax, TMin, TBase,TOpt:double; var DegresDuJour:Double);
begin
try
  DegresDuJour:=(Max(TMin,TBase)+Min(TOpt,TMax))/2 -TBase;
except
  AfficheMessageErreur('EvalDegresJourTborne',UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------
{ On calcule la vitesse moyenne de d�veloppement en consid�rant qu'elle est lin�aire
croissante de 0 � 1 entre Tbase et Topt1, puis constante et �gale � 1 entre Topt1 et Topt2
puis d�croissante de 1 � 0 entre Topt2 et Tlethale.
Puis on calcule la temp�rature �quivalente comprise entre Tbase et Topt1 donnant la m�me
vitesse de d�veloppement.
On calcule les degr�s jours � partir de cette temp�rature �quivalente
 }
procedure EvalDegresJourVitMoy(const TMax, TMin, TBase, TOpt1, TOpt2, TL : Double;
                                var DegresDuJour : Double);
var
  v, v1, v2 : Double;
begin
  try
    v1 := ((max(TMin, TBase) + Min(TOpt1, min(TL, TMax)))/2 - TBase) / (TOpt1 - TBase);
    v2 := (TL - (max(TMax, TOpt2) + TOpt2) / 2) / (TL - TOpt2);
    v := (v1 * (min(min(TL, TMax), TOpt1) - TMin) + (min(TOpt2, max(TOpt1, min(TL, TMax))) - TOpt1) + v2 * (max(TOpt2, min(TL, TMax)) - TOpt2)) / (min(TL, TMax)- max(TMin, TBase));
    DegresDuJour := v * (TOpt1 - TBase);
  except
    AfficheMessageErreur('EvalDegresJourVitMoy | TMax='+FloatToStr(TMax)+
                         ' TMin='+FloatToStr(TMin)+ 'TBase='+FloatToStr(TBase)+' TOpt1='+FloatToStr(TOpt1)+
                         ' TOpt2='+FloatToStr(TOpt2)+' TL='+FloatToStr(TL)+' DegresDuJour='+FloatToStr(DegresDuJour),UMilBilanCarbone);
  end;
end;

////////------------------------------------------------------------------------

Procedure EvolPhenologieMilv2(const SeuilPP,SommeDegresJour,SeuilTempLevee,
                         SeuilTempBVP,SeuilTempRPR,SeuilTempMatu1,
                         SeuilTempMatu2,StockSurface,PourcRuSurfGermi,RuSurf,
                         SlaMax,BiomasseTotale,DateDuJour,DateSemis,stRu:Double;
                         Var SumPP,NumPhase,SommeDegresJourPhasePrec,
                         SeuilTempPhaseSuivante,BiomasseTotaleStadeIp,
                         BiomasseTotaleStadeFlo,CycleReel,ChangePhase,
                         DateMaturite:Double);
// Cette proc�dure est appel�e en d�but de journ�e et fait �voluer les phase
// ph�nologiques. Pour cel�, elle incr�mente les num�ro de phase et change la
// valeur du seuil de la phase suivante. ChangePhase est un bool�en permettant
// d'informer le mod�le pour conna�tre si un jour est un jour de changement
// de phase. Cel� permets d'initialiser les variables directement dans les
// modules sp�cifiques.
// --> Stades ph�nologiques pour le mod�le Mil r��crit:
// 0 : du jour de semis au d�but des conditions favorables pour la germination
// 1 : du d�but des conditions favorables pour la germination au jour de la lev�e
// 2 : du jour de la lev�e au d�but de la floraison
// 3 : du d�but de la floraison au d�but de la photosensibilit�
// 4 : du d�but de la photosensibilit� au d�but de la maturation
// 5 : du d�but de la maturation au d�but du s�chage
// 6 : du d�but du s�chage au jour de r�colte
// 7 : du jour de r�colte � la fin de la simulation
var ChangementDePhase:Boolean;  // on passe en variable un pseudo bool�en et non directement ce bool�en (pb de moteur)
begin
try
  ChangePhase:=0; // l'initialisation quotidi�nne de cette variable � faux permet de stopper le marquage d'une journ�e de changement de phase
  //mainForm.memDeroulement.Lines.Add('phase n�'+FloatToStr(NumPhase)+' StockSurface='+FloatToStr(StockSurface));
  if NumPhase=0 then // la culture a �t� sem�e mais n'a pas germ�
    begin
    if ((StockSurface>=PourcRuSurfGermi*RuSurf) or (stRu>StockSurface)) then
      begin // on commence ds les conditions favo aujourd'hui
      NumPhase:=1;
      //mainForm.memDeroulement.Lines.Add('------> phase n�'+FloatToStr(NumPhase)+' (car StockSurface='+FloatToStr(StockSurface)+')');
      ChangePhase:=1;
      SeuilTempPhaseSuivante := SeuilTempLevee;  { TODO : � v�rif par JCC, le d�clencheur �tant en phase 0 les conditions favorables et non SeuilTempGermi }
      end;
 //   else    { TODO : Pas logique: inutile donc � revoir... }
 //     begin // on v�rifie ne pas �tre arriv� � la fin de la simulation
 //     if SommeDegresJour>=SommeDegresJourMaximale then
 //       NumPhase:=7; // pas de germination r�alis�e  { TODO : mettre un avertissement }
 //       ChangePhase:=True;
 //     end;
    end // fin du if NumPhase=0
else
    begin
    // v�rification d'un �ventuel changement de phase
    If NumPhase <> 3 Then
      ChangementDePhase := (SommeDegresJour >= SeuilTempPhaseSuivante)
    else  // Phase photop�riodique
      ChangementDePhase := (sumPP >= seuilPP);

    If ChangementDePhase then // on a chang� de phase
      begin
      ChangePhase:=1;
      NumPhase := NumPhase + 1;
      //mainForm.memDeroulement.Lines.Add('------> phase n�'+FloatToStr(NumPhase));
      SommeDegresJourPhasePrec := SeuilTempPhaseSuivante; // utilis� dans EvalConversion
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

procedure EvalConversion(const NumPhase, EpsiB, AssimBVP, SommeDegresJour,
                         SommeDegresJourPhasePrecedente, AssimMatu1, AssimMatu2,
                         SeuilTempPhaseSuivante : Double;
                         var Conversion : Double);
var
  KAssim : Double;
begin
  try
    case Trunc(NumPhase) of
      2    : KAssim := 1;
      3..4 : KAssim := AssimBVP;
      5    : KAssim := AssimBVP + (SommeDegresJour - SommeDegresJourPhasePrecedente) *
                       (AssimMatu1 - AssimBVP) / (SeuilTempPhaseSuivante - SommeDegresJourPhasePrecedente);
      6    : KAssim := AssimMatu1 + (SommeDegresJour - SommeDegresJourPhasePrecedente) *
                       (AssimMatu2 - AssimMatu1) / (SeuilTempPhaseSuivante - SommeDegresJourPhasePrecedente);
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

procedure EvalParIntercepte(const PAR, LTR : Double; var PARIntercepte : Double);
begin
  try
    PARIntercepte := PAR * (1 - LTR);
  except
    AfficheMessageErreur('EvalParIntercepte | PAR: '+FloatToStr(PAR)+
                         ' LTR: '+FloatToStr(LTR),UMilBilanCarbone);
  end;
end;

////////------------------------------------------------------------------------

procedure EvalAssimPot(const PARIntercepte, Conversion : Double; var AssimPot : Double);
begin
  try
    AssimPot := PARIntercepte* Conversion * 10;
  except
    AfficheMessageErreur('EvalAssimPot | PAR Intercept�: '+FloatToStr(PARIntercepte)+
                         ' Conversion: '+FloatToStr(Conversion),UMilBilanCarbone);
  end;
end;

////////------------------------------------------------------------------------

procedure EvalCstrAssim(const Cstr:Double; var CstrAssim:Double);
// formule simple permettant un �ventuel changement de calcul (pour arachide par ex)
begin
try
  CstrAssim:=Cstr;
except
  AfficheMessageErreur('EvalCstrAssim | Cstr: '+FloatToStr(Cstr),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalVitesseRacinaire(Const VRacLevee,RootSpeedBVP,RootSpeedRPR,RootSpeedPSP,
                               RootSpeedMatu1,RootSpeedMatu2,NumPhase:Double;
                              var VitesseRacinaire:Double);
 //Modif JCC du 15/03/2005 pour inclure VracLev�e diff�rente de VRacBVP

begin
try
  Case Trunc(NumPhase) of
    1    : VitesseRacinaire := VRacLevee ;
    2    : VitesseRacinaire := RootSpeedBVP ;
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
procedure EvalAssim(Const AssimPot,CstrAssim:Double; var Assim:Double);
begin
try
  Assim:=AssimPot*CstrAssim;
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

procedure EvalRdtPotTotGra(Const KRdt,BiomTotStadeFloraison,BiomTotStadeIp,NumPhase,KrdtB:Double;
                            Var RdtPot:Double);
begin
try
  if NumPhase<5 then
    RdtPot:=0
  else
    RdtPot:=KRdt*(BiomTotStadeFloraison-BiomTotStadeIp)+ KrdtB;
except
  AfficheMessageErreur('EvalRdtPotTotGra | KRdt: '+FloatToStr(KRdt)+
                       ' BiomTotStadeFloraison: '+FloatToStr(BiomTotStadeFloraison)+
                       ' BiomTotStadeIp: '+FloatToStr(BiomTotStadeIp)+
                       ' NumPhase: '+Floattostr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalDRdtPot(Const NumPhase,RdtPot,DegresDuJour,SeuilTempMatu1:Double;
                           Var RdtPotDuJour:Double);
begin
try
  if NumPhase=5 then
    RdtPotDuJour := RdtPot*(DegresDuJour/SeuilTempMatu1)
  else
    RdtPotDuJour:=0;
except
  AfficheMessageErreur('EvalDRdtPot | NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalReallocation(Const RdtPotDujour,DeltaBiomasseAerienne,KRealloc,NumPhase:Double;
                           Var Reallocation,ManqueAssim:Double);
begin
try
  If NumPhase=5 then
    begin
    ManqueAssim:=Max(0,(RdtPotDuJour - max(0,DeltaBiomasseAerienne)));
    if DeltaBiomasseAerienne<0 then
     Reallocation:=0       //JCC 15/11/04
      //Reallocation:= ManqueAssim*KRealloc*0.5
    else
      Reallocation:= ManqueAssim*KRealloc;      {il faudra borner la r�allocation � BiomFeuilles-10}
    end
  else
    begin
    ManqueAssim:=0;
    Reallocation:=0;
    end;
except
  AfficheMessageErreur('EvalRealloc | RdtPotDujour: '+FloatToStr(RdtPotDujour)+
                       ' DeltaBiomasseAerienne: '+FloatToStr(DeltaBiomasseAerienne)+
                       ' KRealloc: '+FloatToStr(KRealloc)+
                       ' NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalReallocation2(Const RdtPotDujour,DeltaBiomasseAerienne,KRealloc,NumPhase,PenalAlloc:Double;
                           Var Reallocation,ManqueAssim:Double);
begin
try
  If NumPhase=5 then
    begin
    ManqueAssim:=Max(0,(RdtPotDuJour - max(0,DeltaBiomasseAerienne)));
    if DeltaBiomasseAerienne<0 then
     //Reallocation:=0   //JCC 15/11/04
      Reallocation:= ManqueAssim*KRealloc*PenalAlloc
    else
      Reallocation:= ManqueAssim*KRealloc;      {il faudra borner la r�allocation � BiomFeuilles-10}
    end
  else
    begin
    ManqueAssim:=0;
    Reallocation:=0;
    end;
except
  AfficheMessageErreur('EvalRealloc2 | RdtPotDujour: '+FloatToStr(RdtPotDujour)+
                       ' DeltaBiomasseAerienne: '+FloatToStr(DeltaBiomasseAerienne)+
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
    BiomasseTotale := Densite * KResGrain * BiomasseGrain/1000 //Biomasse initiale au jour de la lev�e
  else
    BiomasseTotale:=BiomasseTotale+DeltaBiomasseTotale; // pas de gestion de phase,car g�r�e en amont
except
  AfficheMessageErreur('EvolBiomTot | BiomasseTotale: '+FloatToStr(BiomasseTotale)+
                       ' DeltaBiomasseTotale: '+FloatToStr(DeltaBiomasseTotale),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvolBiomasseAerienne(Const DeltaBiomasseTotale,RatioAero,BiomasseTotale,NumPhase,
                               KPenteAero,KBaseAero,ChangePhase:Double;
                               Var DeltaBiomasseAerienne,BiomasseAerienne:Double);
var
BiomasseAeriennePrec:Double;
begin
try
  if NumPhase<=1 then // la lev�e n'a pas eu lieu encore
    begin
    DeltaBiomasseAerienne:=0;
    BiomasseAerienne:=0;
    end
  else
    begin // BVP et sup
    if ((NumPhase=2) and (ChangePhase=1)) then  // initialisation
      begin
      //BiomasseAerienne := Min(0.9, KPenteAero * BiomasseTotale + KBaseAero)* BiomasseTotale; JCC 15/11/04
      BiomasseAerienne := Min(0.9, KBaseAero)* BiomasseTotale; { TODO -oViB : passer ce 0,9 en param�tre? }
      DeltaBiomasseAerienne:=BiomasseAerienne;
      end
    else
      begin
      BiomasseAeriennePrec := BiomasseAerienne;
      if DeltaBiomasseTotale < 0 Then
        BiomasseAerienne := max(0,BiomasseAerienne + DeltaBiomasseTotale)
      else
        BiomasseAerienne := RatioAero*BiomasseTotale;
      DeltaBiomasseAerienne := BiomasseAerienne - BiomasseAeriennePrec;
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

procedure EvalBiomasseRacinair(const BiomasseTotale, BiomasseAerienne : Double; var BiomasseRacinaire : Double);
begin
  try
    BiomasseRacinaire := BiomasseTotale - BiomasseAerienne;
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
    //RatioFeuilles := (Ma + Mb * power(Mc,BiomasseAerienne*12.346/Densite));
    RatioFeuilles := (Ma + Mb * power(Mc,BiomasseAerienne/1000));
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
  if NumPhase<=1 then // la lev�e n'a pas eu lieu encore
    Biomfeuilles:=0
  else
    begin
    if ((NumPhase=2) and (ChangePhase=1)) then
      //BiomFeuilles:=(kBaseLaiDev+KPenteLaiDev*BiomasseAerienne)*BiomasseAerienne   JCC 15/11/04
      BiomFeuilles:=kBaseLaiDev*BiomasseAerienne
    else
      begin
      If NumPhase <= 4 then // de la lev�e � la phase reproductive
        begin
        if DeltaBiomasseTotale <0 Then
          Biomfeuilles:= max(0,Biomfeuilles + DeltaBiomasseTotale*PartFeuillesTiges)
        else
          BiomFeuilles:= RatioFeuilles *BiomasseAerienne;
        end
      else // de la Maturation � la fin de la simulation { TODO -oViB : que se passe t'il en phase 7 apr�s la r�colte? }
        BiomFeuilles:= max(10,BiomFeuilles - (Reallocation-min(0,DeltaBiomasseTotale))*PartFeuillesTiges) ;
        //JCC 18/10/05 max (0,..) remplac� par max(10,..) pour �viter division par 0 dans Sla
      end;
    end;
  DeltaBiomFeuilles:=BiomFeuilles-BiomasseFeuillePrec;
  
except
  AfficheMessageErreur('EvolBiomFeuille | DeltaBiomasseTotale: '+FloatToStr(DeltaBiomasseTotale)+
                       ' BiomasseAerienne: '+FloatToStr(BiomasseAerienne)+
                       ' NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;


////////////////////////////////////////////////////////////////////////////////
procedure EvolBiomasseFeuilles2(Const DeltaBiomasseTotale,PartFeuillesTiges,
                               NumPhase,RatioFeuilles, Reallocation,
                               BiomasseAerienne,ChangePhase,kBaseLaiDev,
                               KPenteLaiDev:Double;
                               Var DeltaBiomFeuilles,BiomFeuilles:Double);
var BiomasseFeuillePrec:Double;
begin
try
  BiomasseFeuillePrec:=BiomFeuilles;
  if NumPhase<=1 then // la lev�e n'a pas eu lieu encore
    Biomfeuilles:=0
  else
    begin
    if ((NumPhase=2) and (ChangePhase=1)) then
      //BiomFeuilles:=(kBaseLaiDev+KPenteLaiDev*BiomasseAerienne)*BiomasseAerienne   JCC 15/11/04
      BiomFeuilles:=kBaseLaiDev*BiomasseAerienne
    else
      begin
      If NumPhase <= 4 then // de la lev�e � la phase reproductive
        begin
        if DeltaBiomasseTotale <0 Then
          Biomfeuilles:= max(10,Biomfeuilles + DeltaBiomasseTotale*PartFeuillesTiges)
          //JCC 08/06/06 max (10.. oubli� ici correction ce jour
        else
          BiomFeuilles:= max(10,RatioFeuilles *BiomasseAerienne);
          //JCC 08/06/06 max (10.. oubli� ici correction ce jour
        end
      else // de la Maturation � la fin de la simulation { TODO -oViB : que se passe t'il en phase 7 apr�s la r�colte? }
        BiomFeuilles:= max(10,BiomFeuilles - (Reallocation-min(0,DeltaBiomasseTotale))*PartFeuillesTiges) ;
        //JCC 18/10/05 max (0,..) remplac� par max(10,..) pour �viter division par 0 dans Sla
      end;
    end;
  DeltaBiomFeuilles:=BiomFeuilles-BiomasseFeuillePrec;
  
except
  AfficheMessageErreur('EvolBiomFeuille2 | DeltaBiomasseTotale: '+FloatToStr(DeltaBiomasseTotale)+
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
      //If dayBiomLeaf >0 then        Modif JCC du 07/04/05 suite � modif CB
        //sla := (sla - KpenteSla * (sla- SlaRPR))* (BiomLeaf- dayBiomLeaf)/BiomLeaf+ SlaBVP * (dayBiomLeaf/BiomLeaf);
        //JCC 05/11/04 nouvelle formule de MD et CB
        sla := (sla - KpenteSla * (sla - SlaRPR))* (BiomLeaf- dayBiomLeaf)/BiomLeaf + (SlaBVP+sla)/2 * (dayBiomLeaf/BiomLeaf);
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
  if NumPhase<=1 then // la lev�e n'a pas eu lieu encore
    BiomasseTiges:=0
  else
    begin
    if NumPhase <= 4 then // de la phase germi � la phase reproductive
      begin
      if DeltaBiomasseTotale <0 then
        BiomasseTiges:= max(0, BiomasseTiges + DeltaBiomasseTotale*(1-PartFeuillesTiges))
      else
        BiomasseTiges:= BiomasseAerienne - BiomasseFeuilles ;
      end
    else // de la photosensible � la fin de la simulation { TODO -oViB : que se passe t'il en phase 7 apr�s la r�colte? }
      BiomasseTiges:= max(0, BiomasseTiges - (Reallocation-min(0,DeltaBiomasseTotale))*(1-PartFeuillesTiges)); // ce qui est r�allou� est pris dans les feuilles et ds les tiges
    end;                                          { TODO : KRealloc ou Reallocation ??? }
except
  AfficheMessageErreur('EvolBiomasseTiges',UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvalBiomasseVegetati(const BiomasseTiges, BiomasseFeuilles, NumPhase : Double;
                                 var BiomasseVegetative : Double);
begin
  try
    BiomasseVegetative := BiomasseTiges  +BiomasseFeuilles;
  except
    AfficheMessageErreur('EvolBiomasseVegetati | BiomasseTiges: '+FloatToStr(BiomasseTiges)+
                         ' BiomasseFeuille: '+FloatToStr(BiomasseFeuilles),UMilBilanCarbone);
  end;
end;

////////------------------------------------------------------------------------

procedure EvalDeltaRdt(Const DeltaBiomasseAerienne,Reallocation,NumPhase,
                       RdtPotDuJour:Double;
                       Var DRdt:Double);
begin
try
  if NumPhase=5 then
    DRdt:=Min(RdtPotDuJour,Max(0,DeltaBiomasseAerienne)+Reallocation)
  else
    DRdt:=0;
except
  AfficheMessageErreur('EvalRdtDuJour | DeltaBiomasseAerienne: '+FloatToStr(DeltaBiomasseAerienne)+
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

procedure EvolSomDegresJour(const DegresDuJour, NumPhase : Double;
                            var SommeDegresJour : Double);
begin
  try
    if (NumPhase >= 1) then
    begin
      SommeDegresJour := SommeDegresJour + DegresDuJour;
    end
    else
    begin
      SommeDegresJour := 0;
    end;
  except
    AfficheMessageErreur('EvolSommeDegresJour | DegresDuJour: '+FloatToStr(DegresDuJour)+
                         ' Phase n�'+FloatToStr(NumPhase)+
                         ' SommeDegresJour: '+FloatToStr(SommeDegresJour)+
                         ' SommeDegresJour: '+FloatToStr(SommeDegresJour),UMilBilanCarbone);
  end;
end;
////////------------------------------------------------------------------------

Procedure EvolPhenoGraminees(const PPSens,SommeDegresJour,SeuilTempLevee,
                         SeuilTempBVP,SeuilTempRPR,SeuilTempMatu1,
                         SeuilTempMatu2,StockSurface,PourcRuSurfGermi,RuSurf,
                         DateDuJour,DateSemis,stRu:Double;
                         Var SumPP,NumPhase,SommeDegresJourPhasePrec,
                         SeuilTempPhaseSuivante,ChangePhase:Double);
// Cette proc�dure est appel�e en d�but de journ�e et fait �voluer les phase
// ph�nologiques. Pour cel�, elle incr�mente les num�ro de phase et change la
// valeur du seuil de la phase suivante. ChangePhase est un bool�en permettant
// d'informer le mod�le pour conna�tre si un jour est un jour de changement
// de phase. Cel� permets d'initialiser les variables directement dans les
// modules sp�cifiques.
// --> Stades ph�nologiques pour le mod�le Mil r��crit:
// 0 : du jour de semis au d�but des conditions favorables pour la germination et de la r�colte � la fin de simulation (pas de culture)
// 1 : du d�but des conditions favorables pour la germination au jour de la lev�e
// 2 : du jour de la lev�e au d�but de la phase photop�riodique
// 3 : du d�but de la phase photop�riodiqueau d�but de la phase reproductive
// 4 : du d�but de la phase reproductive au d�but de la maturation
// 5 : du d�but de la maturation au d�but du s�chage
// 6 : du d�but du s�chage au jour de r�colte
// 7 : le jour de la r�colte
var ChangementDePhase:Boolean;  // on passe en variable un pseudo bool�en et non directement ce bool�en (pb de moteur)
begin
try
  ChangePhase:=0; // l'initialisation quotidi�nne de cette variable � faux permet de stopper le marquage d'une journ�e de changement de phase
  //mainForm.memDeroulement.Lines.Add('phase n�'+FloatToStr(NumPhase)+' StockSurface='+FloatToStr(StockSurface));
  if NumPhase=0 then // la culture a �t� sem�e mais n'a pas germ�
    begin
    if ((StockSurface>=PourcRuSurfGermi*RuSurf) or (stRu>StockSurface)) then
      begin // on commence ds les conditions favo aujourd'hui
      NumPhase:=1;
      //mainForm.memDeroulement.Lines.Add('------> phase n�'+FloatToStr(NumPhase)+' (car StockSurface='+FloatToStr(StockSurface)+')');
      ChangePhase:=1;
      SeuilTempPhaseSuivante := SeuilTempLevee;  { TODO : � v�rif par JCC, le d�clencheur �tant en phase 0 les conditions favorables et non SeuilTempGermi }
      end;
 //   else    { TODO : Pas logique: inutile donc � revoir... }
 //     begin // on v�rifie ne pas �tre arriv� � la fin de la simulation
 //     if SommeDegresJour>=SommeDegresJourMaximale then
 //       NumPhase:=7; // pas de germination r�alis�e  { TODO : mettre un avertissement }
 //       ChangePhase:=True;
 //     end;
    end // fin du if NumPhase=0
else
    begin
    // v�rification d'un �ventuel changement de phase
    If NumPhase <> 3 Then
      ChangementDePhase := (SommeDegresJour >= SeuilTempPhaseSuivante)
    else  // Phase photop�riodique
      ChangementDePhase := (sumPP <= PPSens);

    If ChangementDePhase then // on a chang� de phase
      begin
      ChangePhase:=1;
      NumPhase := NumPhase + 1;
      //mainForm.memDeroulement.Lines.Add('------> phase n�'+FloatToStr(NumPhase));
      SommeDegresJourPhasePrec := SeuilTempPhaseSuivante; // utilis� dans EvalConversion
      Case Trunc(NumPhase) of
        2 : begin   // BVP Developpement vegetatif

            SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempBVP;
            end;
        3 : SumPP := 0;   // PSP Photoperiode
        4 : begin   // RPR Stade initiation paniculaire
            SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempRPR;
            //BiomasseTotaleStadeIp := BiomasseTotale;
            end;
        5 : begin   // Matu1 remplissage grains
            SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu1;
            //BiomasseTotaleStadeFlo:=BiomasseTotale;
            end;
        6 : SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu2; // Matu2 dessication

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
   { JCC le14/03/2005 on va faire calculer BiomTotStadeFloraison et BiomTotStadeIp dans ce module sp�cifique gramin�es
   au lieu de le calculer dans EvonPhenoTemp   }
procedure EvalRdtPotGramin(Const KRdt,BiomasseTotale,NumPhase,ChangePhase,KrdtB:Double;
                            Var RdtPot,BiomTotStadeFloraison,BiomTotStadeIp:Double);
begin
try
  if ((NumPhase=4) and (ChangePhase=1)) then
      BiomTotStadeIp := BiomasseTotale;
  if NumPhase<5 then
    RdtPot:=0
  else
    begin
      if ((NumPhase=5) and (ChangePhase=1)) then
         BiomTotStadeFloraison:=BiomasseTotale;
      RdtPot:=KRdt*(BiomTotStadeFloraison-BiomTotStadeIp)+ KrdtB;

    end ;
except
  AfficheMessageErreur('EvalRdtPotGraminees | KRdt: '+FloatToStr(KRdt)+
                       ' BiomasseTotale: '+FloatToStr(BiomasseTotale)+
                       ' NumPhase: '+FloatToStr(NumPhase)+
                       ' ChangePhase: '+Floattostr(ChangePhase) +
                       ' KrdtB: '+Floattostr(KrdtB),UMilBilanCarbone);
end;
end;
//---------------------------------------------------------------------------------------
procedure EvolLAIPhases(const NumPhase, BiomasseFeuilles, sla : Double;
                               var LAI : Double);

begin
  try
    if (NumPhase <= 1) then
    begin
      LAI := 0
    end
    else
    begin
      if (NumPhase <= 6) then
      begin
        LAI:= BiomasseFeuilles * sla;
      end
      else
      begin
        LAI:=0;
      end;
    end;
  except
    AfficheMessageErreur('EvolLAIPhases: ',UMilBilanCarbone);
  end;
end;

////////------------------------------------------------------------------------

procedure EvalDRdtPotcstr (Const NumPhase,RdtPot,DegresDuJour,SeuilTempMatu1,cstr:Double;
                          Var RdtPotDuJour:Double);
{On pond�re la demande journali�re par cstr comme Christian Baron}
begin
try
  if NumPhase=5 then
    RdtPotDuJour := RdtPot*(DegresDuJour/SeuilTempMatu1)*cstr
  else
    RdtPotDuJour:=0;
except
  AfficheMessageErreur('EvalDRdtPotcstr | NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------

procedure EvolSomDegresJourcstr(Const stRuSurf,RuSurf,DegresDuJour,NumPhase:Double;
                            Var SommeDegresJour:Double);
  { on va multiplier DJ par cstr pour tenter de traduire le retard � la lev�e en cas de s�cheresse}
begin
//try
  if NumPhase<1 then // on ne cumule qu'apr�s la germination
    SommeDegresJour:=0
  else
     if NumPhase=1 then
       SommeDegresJour := SommeDegresJour+ DegresDuJour*max(0.3,min(1,stRuSurf/(0.7*RuSurf)))
     else
       SommeDegresJour := SommeDegresJour+DegresDuJour ;
//except
//  AfficheMessageErreur('EvolSommeDegresJour | DegresDuJour: '+FloatToStr(DegresDuJour)+
//                       ' Phase n�'+FloatToStr(NumPhase)+
 //                      ' SommeDegresJour: '+FloatToStr(SommeDegresJour)+
//                       ' SommeDegresJour: '+FloatToStr(SommeDegresJour),UMilBilanCarbone);
//end;
end;

////////------------------------------------------------------------------------

Procedure EvalSlaRapBiom2(Const SlaBVP, SlaRPR,KpenteSla, BiomLeaf,dayBiomLeaf,
                           NumPhase,ChangePhase: double;
                           Var sla : Double);
begin
try
  //if ((NumPhase=2) and (ChangePhase=1)) then                    //  JCC 18/10/05
  if NumPhase=2 then
       Sla := SlaBVP // soit SlaMax...
  else
    begin
    //if NumPhase>=2 then
    if NumPhase>2 then
     {Begin
      if NumPhase<5 then}
       begin
       sla := min(sla,(sla - KpenteSla *min((SlaBVP-SlaRPR)/3.5,(sla - SlaRPR)))*
              (BiomLeaf- max(0,dayBiomLeaf))/BiomLeaf + (SlaBVP+sla)/3 * max(0,dayBiomLeaf)/BiomLeaf);
       sla := min(SlaBVP,max(SlaRPR , sla));
       end
     { else
       Sla:= SlaRPR
     end }
    else
      Sla:=0;
    end;
except
  AfficheMessageErreur('EvalSlaRapBiom2',UMilBilanCarbone);
end;
End;

////////------------------------------------------------------------------------

procedure EvalAlloAeroFeuilLin(Const NumPhase,FeuilAeroBase,FeuilAeroPente,BiomasseAerienne:Double;
                                    Var RatioFeuilles:Double);
// BiomFeuille=RatioFeuilles * BiomAero

begin
try
  if ((NumPhase>1) AND (NumPhase<=4)) then // donc compris entre la phase d'emergence et reproductive inclus
     RatioFeuilles := max(0.1,FeuilAeroBase + FeuilAeroPente * BiomasseAerienne)  
  else
    RatioFeuilles := 0;
except
  AfficheMessageErreur('EvalAlloAeroFeuilLin',UMilBilanCarbone);
end;
end;

////////------------------------------------------------------------------------



procedure EvalReallocationSorgho(Const RdtPotDujour,DeltaBiomasseAerienne,TxRealloc,NumPhase,PenalAlloc:Double;
                           Var Reallocation,ManqueAssim,ReallocationMax:Double);
begin
try
  If NumPhase=5 then
    begin
    ManqueAssim:=Max(0,(RdtPotDuJour - max(0,DeltaBiomasseAerienne)));
    if DeltaBiomasseAerienne<0 then
      begin
      If ReallocationMax>0 then // il y a encore des r�serves Carbonn�es � mobiliser
        begin
        Reallocation:= ManqueAssim*TxRealloc*PenalAlloc;
        ReallocationMax:=ReallocationMax-Reallocation; // on diminue les r�serves
        end
      else Reallocation:=0; // on a consomm� toute la  r�serve on ne peut r�alouer
      //BiomTotStadeFloraison:=BiomTotStadeFloraison-Reallocation  ;
      end
    else
      Reallocation:= ManqueAssim*TxRealloc;      {il faudra borner la r�allocation � BiomFeuilles-10}
    end
  else
    begin
    ManqueAssim:=0;
    Reallocation:=0;
    end;
except
  AfficheMessageErreur('EvalReallocationSorgho | RdtPotDujour: '+FloatToStr(RdtPotDujour)+
                       ' DeltaBiomasseAerienne: '+FloatToStr(DeltaBiomasseAerienne)+
                       ' TxRealloc: '+FloatToStr(TxRealloc)+
                       ' NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;

procedure EvalReallocationSorgho2(Const RdtPotDujour,DeltaBiomasseAerienne,TxRealloc,NumPhase,PenalAlloc:Double;
                           Var Reallocation,ManqueAssim,ReallocationMax:Double);
begin
try
  If NumPhase=5 then
    begin
    ManqueAssim:=Max(0,(RdtPotDuJour - max(0,DeltaBiomasseAerienne)));
    if DeltaBiomasseAerienne<0 then
      begin
      If ReallocationMax>0 then // il y a encore des r�serves Carbonn�es � mobiliser
        begin
        Reallocation:= min(ManqueAssim*TxRealloc*PenalAlloc,ReallocationMax);
        end
      else Reallocation:=0; // on a consomm� toute la  r�serve on ne peut r�alouer
      //BiomTotStadeFloraison:=BiomTotStadeFloraison-Reallocation  ;
      end
    else
      Reallocation:= min(ManqueAssim*TxRealloc,ReallocationMax);
    end
  else
    begin
    ManqueAssim:=0;
    Reallocation:=0;
    end;
  ReallocationMax:=max(0,ReallocationMax-Reallocation); // on diminue les r�serves
except
  AfficheMessageErreur('EvalReallocationSorgho2 | RdtPotDujour: '+FloatToStr(RdtPotDujour)+
                       ' DeltaBiomasseAerienne: '+FloatToStr(DeltaBiomasseAerienne)+
                       ' TxRealloc: '+FloatToStr(TxRealloc)+
                       ' NumPhase: '+FloatToStr(NumPhase),UMilBilanCarbone);
end;
end;
////////////////////////////////////////////////////////////////////////////////



procedure EvalRdtPotSorgho(Const KRdt,BiomasseTotale,NumPhase,ChangePhase,KrdtB,
                        StressTMin,StressTMax,StressCstr,KRdtBiom,KReallocMax:Double;
                        Var RdtPot,BiomTotStadeFloraison,BiomTotStadeIp,ReallocationMax:Double);
begin
try
  if ((NumPhase=4) and (ChangePhase=1)) then
      BiomTotStadeIp := BiomasseTotale;
  if NumPhase<5 then
    begin
    RdtPot:=0;
    ReallocationMax:=0;
    end
  else
    begin
    if ((NumPhase=5) and (ChangePhase=1)) then
      begin
      BiomTotStadeFloraison:=BiomasseTotale;
      ReallocationMax := BiomTotStadeFloraison* KReallocMax; // permet de d�finir le r�servoir disponible et mobilisable
      end;
      RdtPot:=(KRdt*(BiomTotStadeFloraison-BiomTotStadeIp)+ KrdtB)*(StressTMin*StressTMax*StressCstr)+KRdtBiom*BiomTotStadeFloraison;
    end ;
except
  AfficheMessageErreur('EvalRdtPotSorgho',UMilBilanCarbone);
end;
end;

procedure EvalRdtPotSorgho2(Const BiomasseTotale,NumPhase,ChangePhase,
                        KRdtBiom,KReallocMax:Double;
                        Var RdtPot,BiomTotStadeFloraison,BiomTotStadeIp,ReallocationMax:Double);
begin
try
  if ((NumPhase=4) and (ChangePhase=1)) then
    BiomTotStadeIp := BiomasseTotale;
  if NumPhase<5 then
    begin
    RdtPot:=0;
    ReallocationMax:=0;
    end
  else
    begin
    if ((NumPhase=5) and (ChangePhase=1)) then
      begin
      BiomTotStadeFloraison:=BiomasseTotale;
      ReallocationMax := BiomTotStadeFloraison* KReallocMax; // permet de d�finir le r�servoir disponible et mobilisable
      end;
     RdtPot:=KRdtBiom*BiomTotStadeFloraison;
    end ;
except
  AfficheMessageErreur('EvalRdtPotSorgho2',UMilBilanCarbone);
end;
end;


////////------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////
// Liste de toutes les procedures redef en dyn de l'unite
// Rajouter stdcall � la fin pour permettre l'utilisation de proc�dures dans des dll.
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
Procedure EvalDegresJourTborneDyn (Var T : TPointeurProcParam);
Begin
EvalDegresJourTborne(T[0],T[1],T[2],T[3],T[4]);
end;
Procedure EvalDegresJourVitMoyDyn (Var T : TPointeurProcParam);
Begin
EvalDegresJourVitMoy(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
Procedure EvolPhenologieMilv2Dyn (Var T : TPointeurProcParam);
Begin
EvolPhenologieMilv2(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
              T[10],T[11],T[12],T[13],T[14],T[15],T[16],T[17],T[18],T[19],
              T[20],T[21],T[22],T[23]);
end;
Procedure EvolPhenoGramineesDyn (Var T : TPointeurProcParam);
Begin
EvolPhenoGraminees(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
               T[10],T[11],T[12],T[13],T[14],T[15],T[16],T[17]);
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
EvalVitesseRacinaire(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
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
EvalRdtPotTotGra(T[0],T[1],T[2],T[3],T[4],T[5]);
end;
Procedure EvalRdtPotGraminDyn (Var T : TPointeurProcParam);
Begin
EvalRdtPotGramin(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;
Procedure EvalDRdtPotDyn (Var T : TPointeurProcParam);
Begin
EvalDRdtPot(T[0],T[1],T[2],T[3],T[4]);
end;
Procedure EvalReallocationDyn (Var T : TPointeurProcParam);
Begin
EvalReallocation(T[0],T[1],T[2],T[3],T[4],T[5]);
end;
Procedure EvalReallocation2Dyn (Var T : TPointeurProcParam);
Begin
EvalReallocation2(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
Procedure EvolBiomasseTotaleDyn (Var T : TPointeurProcParam);
Begin
EvolBiomasseTotale(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
Procedure EvolBiomasseAerienneDyn (Var T : TPointeurProcParam);
Begin
EvolBiomasseAerienne(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8]);
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
Procedure EvolLAIPhasesDyn (Var T : TPointeurProcParam);
Begin
EvolLAIPhases(T[0],T[1],T[2],T[3]);
end;
Procedure EvalDRdtPotcstrDyn (Var T : TPointeurProcParam);
Begin
EvalDRdtPotcstr(T[0],T[1],T[2],T[3],T[4],T[5]);
end;
Procedure EvolSomDegresJourcstrDyn (Var T : TPointeurProcParam);
Begin
EvolSomDegresJourcstr(T[0],T[1],T[2],T[3],T[4]);
end;
Procedure EvalSlaRapBiom2Dyn (Var T : TPointeurProcParam);
Begin
EvalSlaRapBiom2(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;
Procedure EvalAlloAeroFeuilLinDyn (Var T : TPointeurProcParam);
Begin
EvalAlloAeroFeuilLin(T[0],T[1],T[2],T[3],T[4]);
end;
Procedure EvalRdtPotSorghoDyn (Var T : TPointeurProcParam);
Begin

EvalRdtPotSorgho(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11],T[12],T[13]);
end;
Procedure EvalRdtPotSorgho2Dyn (Var T : TPointeurProcParam);
Begin
EvalRdtPotSorgho2(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8]);
end;
Procedure EvalReallocationSorghoDyn (Var T : TPointeurProcParam);
Begin
EvalReallocationSorgho(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;
Procedure EvalReallocationSorgho2Dyn (Var T : TPointeurProcParam);
Begin
EvalReallocationSorgho2(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;
Procedure EvolBiomasseFeuilles2Dyn (Var T : TPointeurProcParam);
Begin
EvolBiomasseFeuilles2(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10]);
end;


initialization
TabProc.AjoutProc('InitiationCulture', InitiationCultureDyn);
TabProc.AjoutProc('EvalDateLevee', EvalDateLeveeDyn);
TabProc.AjoutProc('EvalDegresJourTOpt', EvalDegresJourTOptDyn);
TabProc.AjoutProc('EvalDegresJourTborne', EvalDegresJourTborneDyn);
TabProc.AjoutProc('EvalDegresJourVitMoy', EvalDegresJourVitMoyDyn);
TabProc.AjoutProc('EvolPhenologieMilv2', EvolPhenologieMilv2Dyn);
TabProc.AjoutProc('EvolPhenoGraminees', EvolPhenoGramineesDyn);
TabProc.AjoutProc('EvalConversion', EvalConversionDyn);
TabProc.AjoutProc('EvalParIntercepte', EvalParIntercepteDyn);
TabProc.AjoutProc('EvalAssimPot', EvalAssimPotDyn);
TabProc.AjoutProc('EvalCstrAssim', EvalCstrAssimDyn);
TabProc.AjoutProc('EvalVitesseRacinaire', EvalVitesseRacinaireDyn);
TabProc.AjoutProc('EvalAssim', EvalAssimDyn);
TabProc.AjoutProc('EvalDeltaBiomTot', EvalDeltaBiomTotDyn);
TabProc.AjoutProc('EvalRdtPotTotGra', EvalRdtPotTotGraDyn);
TabProc.AjoutProc('EvalRdtPotGramin', EvalRdtPotGraminDyn);
TabProc.AjoutProc('EvalDRdtPot', EvalDRdtPotDyn);
TabProc.AjoutProc('EvalReallocation', EvalReallocationDyn);
TabProc.AjoutProc('EvalReallocation2', EvalReallocation2Dyn);
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
TabProc.AjoutProc('EvolLAIPhases',EvolLAIPhasesDyn);
TabProc.AjoutProc('EvalDRdtPotcstr',EvalDRdtPotcstrDyn);
TabProc.AjoutProc('EvolSomDegresJourcstr', EvolSomDegresJourcstrDyn);
TabProc.AjoutProc('EvalSlaRapBiom2', EvalSlaRapBiom2Dyn);
TabProc.AjoutProc('EvalAlloAeroFeuilLin', EvalAlloAeroFeuilLinDyn);
TabProc.AjoutProc('EvalRdtPotSorgho', EvalRdtPotSorghoDyn);
TabProc.AjoutProc('EvalRdtPotSorgho2', EvalRdtPotSorgho2Dyn);
TabProc.AjoutProc('EvalReallocationSorgho', EvalReallocationSorghoDyn);
TabProc.AjoutProc('EvalReallocationSorgho2', EvalReallocationSorgho2Dyn);
TabProc.AjoutProc('EvolBiomasseFeuilles2', EvolBiomasseFeuilles2Dyn);
end.


