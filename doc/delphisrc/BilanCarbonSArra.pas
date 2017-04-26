unit BilanCarbonSarra;

interface

implementation
uses Math, ModelsManage, Main, GestionDesErreurs,SysUtils,Dialogs;
var NbJourCompte, NbjStress:integer;

procedure EvalAssimSarrahV3(const AssimPot, Tr, TrPot : Double; var Assimilats : Double);
begin
  try
    if (TrPot > 0) then
    begin
      Assimilats := AssimPot * Tr / TrPot;
    end
    else
    begin
      Assimilats := 0;
    end;
  except
    AfficheMessageErreur('EvalAssimSarrahV3 | AssimPot: '+FloatToStr(AssimPot)+
                         ' TrPot: '+FloatToStr(TrPot),UBilanCarbonSarra);
  end;
end;

procedure EvolBiomTotSarrahV3(const Assimilats, RespMaint, NumPhase, Densite,
                             KResGrain, BiomasseGrain, ChangePhase : Double;
                             var BiomasseTotale, DeltaBiomasseTotale : Double);
begin
  try
    if ((NumPhase = 2) and (ChangePhase = 1)) then
    begin
      BiomasseTotale := Densite * KResGrain * BiomasseGrain / 1000;
    end
    else
    begin
      BiomasseTotale := BiomasseTotale + (Assimilats - RespMaint);
    end;
    DeltaBiomasseTotale := (Assimilats - RespMaint);
  except
    AfficheMessageErreur('EvolBiomTotSarrahV3 | BiomasseTotale: '+FloatToStr(BiomasseTotale)+
                         ' DeltaBiomasseTotale: '+FloatToStr(DeltaBiomasseTotale),UBilanCarbonSarra);
  end;
end;

procedure EvolBiomAeroSarrahV3(const BiomasseTotale, NumPhase,
                 KPenteAero, KBaseAero, ChEvalSlaRapBiom2angePhase, DeltaBiomasseTotale : Double;
                 var DeltaBiomasseAerienne, BiomasseAerienne : Double);
begin
  try
    DeltaBiomasseAerienne := BiomasseAerienne;
    If ((NumPhase >= 2) and (NumPhase <= 4)) then
    begin
      BiomasseAerienne := min(0.9, kPenteAero * BiomasseTotale + kBaseAero) * BiomasseTotale ;
    end
    else
    begin
      BiomasseAerienne := BiomasseAerienne + DeltaBiomasseTotale;
    end;
    DeltaBiomasseAerienne := BiomasseAerienne - DeltaBiomasseAerienne;
  except
    AfficheMessageErreur('EvolBiomAeroSarrahV3 | DeltaBiomasseAerienne: '+FloatToStr(DeltaBiomasseAerienne)+
                         ' BiomasseAerienne: '+FloatToStr(BiomasseAerienne),UBilanCarbonSarra);
  end;
end;

procedure EvalFeuilleTigeSarrahV3(const NumPhase, kBaseLaiDev, kPenteLaiDev,
                        BiomasseAerienne, PartFeuillesTiges, rdt,
                        DeltaBiomasseAerienne, Reallocation : Double;
                        var BiomasseFeuilles, BiomasseTiges, DeltaBiomasseFeuilles : Double);
var
  bM, CM : Double;
begin
  try
    if (NumPhase > 1) then
    begin
      DeltaBiomasseFeuilles := BiomasseFeuilles;
      if (DeltaBiomasseAerienne < 0) then
      begin
        BiomasseFeuilles := max(0, BiomasseFeuilles - (-DeltaBiomasseAerienne + Reallocation) * PartFeuillesTiges);
      end
      else
      begin
        if (NumPhase <= 4) then
        begin
          bM :=  kBaseLaiDev - 0.1;
          cM := ((kPenteLaiDev * 1000)/ bM + 0.78) / 0.75;
          BiomasseFeuilles := (0.1 + bM * power(cM, BiomasseAerienne / 1000)) * BiomasseAerienne;
        end
        else
        begin
          BiomasseFeuilles := BiomasseFeuilles - Reallocation * PartFeuillesTiges;
        end;
      end;
      BiomasseTiges:= BiomasseAerienne - (BiomasseFeuilles + rdt) ;
      DeltaBiomasseFeuilles := BiomasseFeuilles - DeltaBiomasseFeuilles;
    end;
  except
    AfficheMessageErreur('EvalFeuilleTigeSarrahV3',UBilanCarbonSarra);
  end;
end;
{Module n°39 - EvalSlaCereales	Procédure Delphi: EvalSlaCereales, s'exécute tous les jours sur les entités concernées :
	1 - SlaMax -IN- (en ha/kg) : Surface massique maximum des feuilles
	2 - SlaMin -IN- (en ha/kg) : Surface massique minimum des feuilles
	3 - SlaPente -IN- : Dépend du module utilisé
	4 - DayBiomLeaf -IN- (en kg/ha) : Augmentation journalière de la biomasse foliaire
	5 - BiomasseFeuilles -IN- (en kg/ha) : Biomasse des feuilles
	6 - NumPhase -IN- : Numéro de la phase phénologique
	7 - Sla -INOUT- (en ha/kg) : Surface massique des feuilles
}
procedure EvalSlaSarrahV3(const SlaBVP, SlaRPR,
                             KpenteSla, DeltaBiomasseFeuilles, BiomasseFeuilles, NumPhase, ChangePhase : Double;
                       var sla : Double);
{
On suppose que les jeunes feuilles on un SLA supérieur aux vieilles feuilles.
La fraction de jeunes (nouvelles) feuilles fait donc monter le SLA global
du couvert. Le paramètre penteSLA provoque une chute générale du SLA
(penteSLA = chute relative par jour = fraction de différence entre SLAmax
et SLAmin). Fonctionnement conçu surtout pour les légumineuses, mais
peut être aussi adapté aux autres espèces.
Paramètres :
SLAmax (0.001 … 0.01), ex : 0.007
SLAmin (0.001 … 0.01), ex : 0.002
penteSLA (0 … 0.2), ex : 0.1
Avec : SLAini = SLAmax
}
begin
  try
    if (BiomasseFeuilles > 0) then
    begin
      if ((NumPhase = 2) and (ChangePhase = 1)) then
      begin
        sla := SlaBvp;
      end;
      sla := (sla - KpenteSla * (sla - SlaRPR)) * (BiomasseFeuilles - DeltaBiomasseFeuilles) / BiomasseFeuilles + (SlaBVP + sla)/2 * (DeltaBiomasseFeuilles / BiomasseFeuilles);
      sla := min(SlaBVP, max(SlaRPR, sla));
    end;
  except
    AfficheMessageErreur('EvalSlaSarrahV3',UBilanCarbonSarra);
  end;
end;


procedure EvalRdtPotSarrahV3(Const KRdt,BiomasseTotale,NumPhase,ChangePhase,KrdtB,
                        KRdtBiom,DegresDuJour,SeuilTempMatu1, tr, trPot, BiomasseFeuilles : Double;
                        Var RdtPot,BiomTotStadeFloraison,BiomTotStadeIp,RdtPotDuJour:Double);
{ voir a tester une prise en compte de la photoper en fn du nbre de degre jours sur la période de photoper}
begin
try
  if ((NumPhase=4) and (ChangePhase=1)) then
      BiomTotStadeIp := BiomasseTotale;
  if (NumPhase=5) then
    begin
      If (ChangePhase=1) then
      begin
        BiomTotStadeFloraison:=BiomasseTotale;
        RdtPot:=(KRdt*(BiomTotStadeFloraison-BiomTotStadeIp)+ KrdtB)+ KRdtBiom*BiomTotStadeFloraison;
        if (RdtPot > BiomasseTotale) then
		    begin
          RdtPot := BiomasseTotale;
		    end;
      end;
      If trpot > 0 Then RdtPotDuJour := RdtPot * (DegresDuJour/SeuilTempMatu1) * (tr/trPot)
      Else RdtPotDuJour := 0;
    end ;
except
  AfficheMessageErreur('EvalRdtPotSarrahV3',UBilanCarbonSarra);
end;
end;

procedure EvalRdtPotRespSarrahV3(const KRdt, BiomasseTotale, NumPhase, ChangePhase, KrdtB,
                        KRdtBiom, DegresDuJour, SeuilTempMatu1, tr, trPot, RespMaint, BiomasseFeuilles : Double;
                        var RdtPot, BiomTotStadeFloraison, BiomTotStadeIp, RdtPotDuJour : Double);
begin
  try
    if ((NumPhase = 4) and (ChangePhase = 1)) then
    begin
      BiomTotStadeIp := BiomasseTotale;
    end;
    if (NumPhase = 5) then
    begin
      if (ChangePhase = 1) then
      begin
        BiomTotStadeFloraison := BiomasseTotale;
        RdtPot := (KRdt * (BiomTotStadeFloraison - BiomTotStadeIp) + KrdtB) + KRdtBiom * BiomTotStadeFloraison;
        if (RdtPot > BiomasseTotale) then
		    begin
          RdtPot := BiomasseTotale;
		    end;
      end;
      if (trpot > 0) Then
      begin
        RdtPotDuJour := RdtPot * (DegresDuJour / SeuilTempMatu1) * (tr / trPot);
        RdtPotDuJour := max(RdtPotDuJour, RespMaint * 0.15);
      end
      else
      begin
        RdtPotDuJour := 0;
      end;
    end ;
  except
    AfficheMessageErreur('EvalRdtPotRespSarrahV3',UBilanCarbonSarra);
  end;
end;

procedure EvalRespMaintSarrahV3(const biomTot, tempMoy, KtempMaint, kRespMaint, BiomFeuilles, Numphase : Double;
                        var respMaint : Double);
{
RespMaint Kg/ha/j  en équivalent matière sèche
KRespMaint     (0.015)
KTempMaint °C  (25 )
}
begin
  try
    respMaint := kRespMaint * biomTot * power(2, (tempMoy - kTempMaint) / 10);
    respMaint := respmaint+kRespMaint * Biomfeuilles * power(2, (tempMoy - kTempMaint) / 10);
	if (Numphase > 4) then
	begin
    if (Biomfeuilles = 0) then
    begin
      respMaint := 0;
    end;
  end;
  except
    AfficheMessageErreur('EvalRespMaint',UBiomasse);
  end;
end;

procedure EvolDayRdtSarraV3(const RdtPotDuJour, DeltaBiomasseAerienne,
                  Reallocation, NumPhase : Double;
                  var Rdt : Double);
{
On tend vers le potentiel en fn du rapport des degresJours/sumDegresJours
pour la phase de remplissage
Frein sup fn du flux de sève estimé par le rapport Tr/TrPot
}
begin
  try
    if (NumPhase = 5) then
    begin
      Rdt := Rdt + min(RdtPotDuJour, Max(0, DeltaBiomasseAerienne) + Reallocation);
    end;
  except
    AfficheMessageErreur('EvolDayRdtSarraV3',UBilanCarbonSarra);
  end;
end;

procedure EvalReallocationSarrahV3(const RdtPotDuJour, DeltaBiomasseAerienne,
                     TxRealloc, NumPhase, Biomleaf : Double;
                     var Reallocation, ManqueAssim : Double);
{
La reallocation est à 0 quand la biomasse foliaire verte est inférieure à 30 kh/ha
}
begin
  try
    if (NumPhase = 5) then
    begin
      ManqueAssim := max(0, (RdtPotDuJour - max(0, DeltaBiomasseAerienne)));
      Reallocation := min(ManqueAssim * TxRealloc, max (0, biomleaf - 30));
    end
    else
    Begin
      ManqueAssim := 0;
      Reallocation := 0;
    end;
  except
    AfficheMessageErreur('EvalReallocationSarrahV3 | RdtPotDujour: '+FloatToStr(RdtPotDujour)+
                         ' DeltaBiomasseAerienne: '+FloatToStr(DeltaBiomasseAerienne)+
                         ' TxRealloc: '+FloatToStr(TxRealloc)+
                         ' NumPhase: '+FloatToStr(NumPhase),UBilanCarbonSarra);
  end;
end;

procedure MortaliteSarraV3(const DeltaBiomasseAerienne, NbjTestSemis,SeuilCstrMortality, ChangePhase : Double;
                  var NumPhase, RuRac : Double);
{
Test sur 20 jours
Dès que le delta est négatif sur 10 jours
}
begin
  try
    if (NumPhase >= 2) then
    begin
      if (NumPhase = 2) and (ChangePhase = 1) then
      begin
        NbjourCompte := 0;
        NbjStress := 0;
      end;
      inc(Nbjourcompte);
      if (Nbjourcompte < NbjTestSemis) then
      begin
        if (DeltaBiomasseAerienne < 0) then
        begin
          inc(NbjStress);
        end;
      end;
      if (NbjStress > SeuilCstrMortality) then
      begin
        Numphase := 0;
        RuRac := 0;
        NbJStress := 0;
      end;
    end;
  except
    AfficheMessageErreur('MortaliteSarraV3',UBilanCarbonSarra);
  end;
end;
////////////////////////////////////////////////////////////////////////////////

Procedure MortaliteSarraV3Dyn (Var T : TPointeurProcParam);
Begin
  MortaliteSarraV3(T[0],T[1],T[2],T[3],T[4],T[5]);
end;
Procedure EvolDayRdtSarraV3Dyn (Var T : TPointeurProcParam);
Begin
  EvolDayRdtSarraV3(T[0],T[1],T[2],T[3],T[4]);
end;
Procedure EvalRdtPotSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
  EvalRdtPotSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11],T[12],T[13],T[14]);
end;

Procedure EvalRdtPotRespSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
  EvalRdtPotRespSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11],T[12],T[13],T[14],T[15]);
end;
Procedure EvalReallocationSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
  EvalReallocationSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
Procedure EvalAssimSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
  EvalAssimSarrahV3(T[0],T[1],T[2],T[3]);
end;
Procedure EvolBiomTotSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
  EvolBiomTotSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8]);
end;
Procedure EvolBiomAeroSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
  EvolBiomAeroSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;
Procedure EvalFeuilleTigeSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
  EvalFeuilleTigeSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10]);
end;
Procedure EvalSlaSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
  EvalSlaSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;
procedure EvalRespMaintSarrahV3Dyn(Var T : TPointeurProcParam);
begin
EvalRespMaintSarrahV3( T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
////////////////////////////////////////////////////////////////////////////////////
initialization
 TabProc.AjoutProc('EvolDayRdtSarraV3', EvolDayRdtSarraV3Dyn);
 TabProc.AjoutProc('EvalRdtPotSarrahV3', EvalRdtPotSarrahV3Dyn);
 TabProc.AjoutProc('EvalRdtPotRespSarrahV3', EvalRdtPotRespSarrahV3Dyn);
 TabProc.AjoutProc('EvalReallocationSarrahV3', EvalReallocationSarrahV3Dyn);
 TabProc.AjoutProc('EvalAssimSarrahV3', EvalAssimSarrahV3Dyn);
 TabProc.AjoutProc('EvolBiomTotSarrahV3', EvolBiomTotSarrahV3Dyn);
 TabProc.AjoutProc('EvolBiomAeroSarrahV3', EvolBiomAeroSarrahV3Dyn);
 TabProc.AjoutProc('EvalFeuilleTigeSarrahV3', EvalFeuilleTigeSarrahV3Dyn);
 TabProc.AjoutProc('EvalSlaSarrahV3', EvalSlaSarrahV3Dyn);
 TabProc.AjoutProc('MortaliteSarraV3', MortaliteSarraV3Dyn);
 TabProc.AjoutProc('EvalRespMaintSarrahV3',EvalRespMaintSarrahV3Dyn);
end.

