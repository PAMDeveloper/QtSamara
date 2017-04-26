unit Phenologie;

interface

implementation
uses Math, ModelsManage, Main, GestionDesErreurs,SysUtils,Dialogs;


////////------------------------------------------------------------------------

procedure EvolPhenoSarrahV3(const SommeDegresJour, SeuilTempLevee,
                         SeuilTempBVP, SeuilTempRPR, SeuilTempMatu1,
                         SeuilTempMatu2, SeuilPluie,
                         StockSurface : Double;
                         Var NumPhase, SommeDegresJourPhasePrec,
                         SeuilTempPhaseSuivante, ChangePhase, PhasePhotoper : Double);
{Méthode générique pour le test de fin de la phase photopériodique.
PhasePhotoper = 0 en fin de la phase photoper et = 1 en debut de la phase
Cette procédure est appelée en début de journée et fait évoluer les phase
phénologiques. Pour celà, elle incrémente les numéro de phase et change la
valeur du seuil de somme de degré jours de la phase suivante.
ChangePhase est un booléen permettant
d'informer le modèle pour connaître si un jour est un jour de changement
de phase. Celà permets d'initialiser les variables directement dans les
modules spécifiques.
--> Stades phénologiques pour les céréales:
// 0 : du jour de semis au début des conditions favorables pour la germination et de la récolte à la fin de simulation (pas de culture)
// 1 : du début des conditions favorables pour la germination au jour de la levée
// 2 : du jour de la levée au début de la phase photopériodique
// 3 : du début de la phase photopériodique au début de la phase reproductive
// 4 : du début de la phase reproductive au début de la maturation (seulement pour le mais et riz) Pas pris en compte ici!
//      sousphase1  de début RPR à RPR/4
//      sousphase2  de RPR/4 à RPR/2
//      sousphase3 de RPR/2 à 3/4 RPR
//      sousphase4 de 3/4 RPR à fin RPR
// 5 : du début de la maturation au stade grain laiteux
// 6 : du début du stade grain laiteux au jour de récolte
// 7 : le jour de la récolte
}
var
  ChangementDePhase, ChangementDeSousPhase : Boolean;
begin
  try
  ChangePhase := 0;
  if (Trunc(NumPhase) = 0) then
  begin
    if (StockSurface >= SeuilPluie) then
    begin
      NumPhase := 1;
      ChangePhase := 1;
      SeuilTempPhaseSuivante := SeuilTempLevee;
    end;
  end
  else
  begin
    if ((Trunc(NumPhase) = 2) and (SommeDegresJour >= SeuilTempPhaseSuivante)) then
    begin
      ChangementDePhase := True
    end
    else
    begin
      If (Trunc(NumPhase) <> 3) Then
      begin
        ChangementDePhase := (SommeDegresJour >= SeuilTempPhaseSuivante)
      end
      else
      begin
        ChangementDePhase := (PhasePhotoper = 0);
      end;
    end;
    if ChangementDePhase then
    begin
      ChangePhase := 1;
      NumPhase := NumPhase + 1;
      SommeDegresJourPhasePrec := SeuilTempPhaseSuivante;
      case Trunc(NumPhase) of
        1 : SeuilTempPhaseSuivante:= SeuilTempLevee;
        2 : SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempBVP;
        3 : PhasePhotoper := 1;
        4 : SeuilTempPhaseSuivante := SommeDegresJour + SeuilTempRPR;
        5 : SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu1;
        6 : SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu2;
      end;
    end;
  end;
  except
    AfficheMessageErreur('EvolPhenoSarrahV3 | NumPhase: '+FloatToStr(NumPhase)+
                       ' SommeDegresJour: '+FloatToStr(SommeDegresJour)+
                       ' SeuilTempPhaseSuivante: '+FloatToStr(SeuilTempPhaseSuivante) ,UPhenologie);
  end;
end;

////////////////////////////////////////////////////////////////////////////////
procedure EvalVitesseRacSarraV3(const VRacLevee, RootSpeedBVP, RootSpeedRPR, RootSpeedPSP,
                               RootSpeedMatu1, RootSpeedMatu2, NumPhase : Double;
                              var VitesseRacinaire : Double);
begin
  try
    case Trunc(NumPhase) of
      1    :  VitesseRacinaire := VRacLevee;
      2    :  VitesseRacinaire := RootSpeedBVP;
      3    :  VitesseRacinaire := RootSpeedPSP;
      4    :  VitesseRacinaire := RootSpeedRPR;
      5    :  begin
                if (RootSpeedMatu1 = NullValue) then
                begin
                  VitesseRacinaire := 0;
                end
                else
                begin
                  VitesseRacinaire := RootSpeedMatu1;
                end;
              end;
      6    :  begin
                if (RootSpeedMatu2 = NullValue) then
                begin
                  VitesseRacinaire := 0;
                end
                else
                begin
                  VitesseRacinaire := RootSpeedMatu2;
                end;
              end;
    else
      VitesseRacinaire := 0
    end;
  except
    AfficheMessageErreur('EvalVitesseRacSarraV3 | NumPhase: '+FloatToStr(NumPhase),UPhenologie);
  end;
end;

////////////////////////////////////////////////////////////////////////////////
////////------------------------------------------------------------------------
{ On calcule la vitesse moyenne de développement en considérant qu'elle est linéaire
croissante de 0 à 1 entre Tbase et Topt1, puis constante et égale à 1 entre Topt1 et Topt2
puis décroissante de 1 à 0 entre Topt2 et Tlethale.
Puis on calcule la température équivalente comprise entre Tbase et Topt1 donnant la même
vitesse de développement.
On calcule les degrés jours à partir de cette température équivalente
A SIMPLIFIER!!! Clarté...
Relation avec ancienne formule?
sumDD:=sumDD+max(min(TOpt,TMoy),TBase)-Tbase;
Pb bornage sur la temp moyenne ou sur les extrèmes???? Pas pareil!!! 
Jusqu'à maintenant on le faisait sur TMoy = (Tmin+ Tmax)/2!!! assez différent!!!
si Tmoy <= Topt2 
    DegresDuJour:= max(min(TOpt1,TMoy),TBase)-Tbase
sinon 
    DegresDuJour= (Topt1-Tbase) * (1 - ( (min(TL, Tmoy) - Topt2 )/(TL -Topt2)) 
      
     }
Procedure EvalDegresJourSarrahV3(const TMoy, TBase, TOpt1, TOpt2, TL :double;
                                var DegresDuJour,SomDegresJour:Double);

begin
try
{ Pb de méthode !?
  v1:= ((Max(TMin,TBase)+Min(TOpt1,TMax))/2 -TBase )/( TOpt1 - TBase);
  v2:= (TL - max(TMax,TOpt2)) / (TL - TOpt2);
  v:= (v1 * (min(TMax,TOpt1) - TMin)+(min(TOpt2,max(TOpt1,TMax)) - TOpt1) + v2 * (max(TOpt2,TMax)-TOpt2))/( TMax- TMin);
  DegresDuJour:= v * (TOpt1-TBase);
}
  If Tmoy <= Topt2 then
     DegresDuJour:= max(min(TOpt1,TMoy),TBase)-Tbase
  else  
     DegresDuJour := (TOpt1-TBase) * (1 - ( (min(TL, TMoy) - TOpt2 )/(TL -TOpt2)));
  SomDegresJour := SomDegresJour + DegresDuJour;
except
  AfficheMessageErreur('EvalDegresJourSarrahV3 | TMoy='+FloatToStr(TMoy)+
  'TBase='+FloatToStr(TBase)+' TOpt1='+FloatToStr(TOpt1)+
  ' TOpt2='+FloatToStr(TOpt2)+' TL='+FloatToStr(TL)+' DegresDuJour='+FloatToStr(DegresDuJour),UPhenologie);
end;
end;

////////------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////

procedure PhotoperSarrahV3(const Numphase, ChangePhase, SomDegresJour, DegresDuJour,
                SeuilPP, PPCrit, DureeDuJour, PPExp, PPSens : Double;
                var SumPP, SeuilTempPhasePrec, PhasePhotoper : Double);

{Procedure speciale Vaksman Dingkuhn valable pour tous types de sensibilite
photoperiodique et pour les varietes non photoperiodique.
PPsens varie de 0,4 a 1,2. Pour PPsens > 2,5 = variété non photoperiodique.
SeuilPP = 13.5
PPcrit = 12
SumPP est dans ce cas une variable quotidienne (et non un cumul)}

begin
  try
    if (NumPhase = 3) then
    begin
      if (ChangePhase = 1) then
      begin
        SumPP := 100;
      end;
      SumPP := power((1000 / max(0.01, SomDegresJour - SeuilTempPhasePrec)), PPExp) * max(0, (DureeDuJour - PPCrit)) / (SeuilPP - PPCrit);
      if (sumPP <= PPsens) then
      begin
        PhasePhotoper :=0;
      end;
    end;
  except
    AfficheMessageErreur('PhotoperSarrahV3',UPhenologie);
  end;
end;

////////////////////////////////////////////////////////////////////////////////
Procedure EvolPhenoSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
EvolPhenoSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
               T[10],T[11],T[12]);
end;

Procedure PhotoperSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
PhotoperSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11]);
end;

Procedure EvalDegresJourSarrahV3Dyn (Var T : TPointeurProcParam);
Begin
EvalDegresJourSarrahV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;
Procedure EvalVitesseRacSarraV3Dyn (Var T : TPointeurProcParam);
Begin
EvalVitesseRacSarraV3(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;
////////////////////////////////////////////////////////////////////////////////////
initialization
 TabProc.AjoutProc('EvolPhenoSarrahV3', EvolPhenoSarrahV3Dyn);
 TabProc.AjoutProc('PhotoperSarrahV3', PhotoperSarrahV3Dyn);
 TabProc.AjoutProc('EvalDegresJourSarrahV3', EvalDegresJourSarrahV3Dyn);
 TabProc.AjoutProc('EvalVitesseRacSarraV3', EvalVitesseRacSarraV3Dyn);
                                                           
end.

