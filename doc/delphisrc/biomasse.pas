unit Biomasse;
{- peuplement :
- mySite : Site
- initialisation de plante (parcelle.dateSemis), densité … (Bdd)
- paramètre variétaux : kEpsiB, kRdt, tabSla(numPhase), penteBiom, kRepBiomLeaf, kResGrain, biomGrain, kdf (Bdd)
-
- Ltr : Lai, kdf
- Assimilation : mySite.Par, KepsiB, ltr
- Lai :
- LaiIni : tabSla(numPhaseIni), biomTotIni
- Lai :
- Phase BVP… PSP LaiDev : tabSla(numPhase), penteBiom, kRepBiomLeaf, biomTot
- Phase RPR…matu2 LaiScenescence : LaiFlo (chgt phase PSP->RPR), penteScen, Plante.cstr ou tr/trMax
- Kcp : kcMax, ltr
- Biomasse Totale :
- BiomTotIni : densite, kResGrain, biomGrain
- RespMaintenance : kRespMaint, biomTot, mySite.temp,
- BiomTot : assimilation, respMaintenance, Plante.cstr
- RdtPot : biomTotIp(phase psp->Rpr), biomTotFlo(phase Rpr->MatuRemplissage), kRdt (calculé lors du chgt de phase RPR->Matu
- PuitRdt : rdtPot, biomTotFlo(phase Rpr->MatuRemplissage) (calculé lors du chgt de phase RPR-Matu
- rendement : rdtPot, seuilTemp(matu remplissage), cstr ou tr/trMax

- plante :
- initialise Racine
- paramètre variétaux : tabSeuilTemp(phase), tabSeuilRoot(Vrac, phase), kPhotoper   (Bdd)
- cstr : indice de contrainte hydrique Racine.DisponibleTrWater, Racine.MaxDisponibleTrWater
- FAO
- Eagleman
- trMax : kcp, Site.Eto
- tr : trMax, cstr
- phase phéno : Peuplement.degDay, numphase, tabSeuilTemp(numPhase), sumDegDay ou datePhotoper
- photopériode : cas phase phéno
- déduit de la date de fin de saison des pluies (site.FinPluie), donne une date de fin de photopériode datePhotoper
- calculé : kPhotoper
- Racine :
- paramètre variétaux : tabSeuilRoot(Vrac, phase)   (Bdd)
- mySol : Sol
- myLastSousStrate : SousStrate
- rootSpeed : tabSeuilRoot(Vrac, phase), Plante.phase
- rootDepth : myLastSousStrate.depth
- MaxDisponibleWater : mySol.MaxTrWater(myLastSousStrate)
- DisponibleWater : mySol.DisponibleTrWater(myLastSousStrate)
- ConsommeTr : mySol.ConsommeTrWater(myLastSousStrate), Plante.tr
- UpDateTr : consommeTr, plante.tr (si tout n'as pu être consommer)
}
interface

{Proc. d'initialisation du peuplement (Plante)}

Procedure EvalBiomIni (Const SlaBVP,KResGrain, kPenteAero, kBaseAero,densite,
                             biomGrain,SeuilTempGermi : Double;
                     Var biomTot, BiomAero, sla, numphase,VRac,SeuilTemp :Double);
// ok
Procedure SumTemp(Const tempMoy, kTempPhase : Double;
                  Var sumDD : Double);
//ok
Procedure EvolSDJOpt(Const TMoy,TBase,TOpt: Double;
                     Var sumDD : Double);
//ok
Procedure EvalSumPhotoPer (Const DayLength, KPPCrit, kPPSens, kPPForme: Double;
                           Var sumPP, SeuilPP : Double);

Procedure EvolPhenoTemp(Const SeuilPP,
                        TempGermi, TempBvp, TempRPR, TempMatu1, TempMatu2,
                        RootSpeedBVP, RootSpeedPSP, RootSpeedRPR,
                        RootSpeedMatu1,RootSpeedMatu2,
                        KRdt, ThisDate, DateSemis,BiomTot : Double;
                        Var NoPhase, Vrac, SumDD, sumPP, SeuilTemp,
                        SumDDPhasePrec, biomtotStadeIp, cycleReel, RdtPot,DateMaturite : Double);
Procedure EvolPhenoTempC(Const SeuilPP,
                        TempGermi, TempBvp, TempRPR, TempMatu1, TempMatu2,
                        RootSpeedBVP, RootSpeedPSP, RootSpeedRPR,
                        RootSpeedMatu1,RootSpeedMatu2,
                        KRdt, ThisDate, DateSemis,BiomTot, ConstRdt : Double;
                        Var NoPhase, Vrac, SumDD, sumPP, SeuilTemp,
                        SumDDPhasePrec, biomtotStadeIp, cycleReel, RdtPot,DateMaturite : Double);


Procedure EvalLtr(Const Kdf, Lai : Double;
                  Var ltr : Double);

Procedure FreinEpsiBTemp(Const Tbase,TOpt, TMin, Tmax : Double;
                         Var IndTemp : Double );
//ok
Procedure AssimilatPotRapLeaf(Const KEpsiB, Par, Ltr, MaxBiomLeaf, BiomLeaf, NumPhase: double;
                       Var  assimPot :Double);
// pas retenue
Procedure AssimilatPotTemp(Const TabKassim : array of double;
                            KEpsiB, Par, sumDD, SeuilTemp, Ltr, IndTemp,sumDDPhasePrec: double;
                            numPhase : Integer;
                       Var  assimPot :Double);

Procedure AssimilatPot(Const KAssimBVP, KAssimMatu1, KAssimMatu2,
                            KEpsiB, Par, sumDD, SeuilTemp, Ltr, sumDDPhasePrec, NoPhase: double;
                       Var  assimPot :Double);

Procedure AssimWUE(Const KAssimBVP, KAssimMatu1, KAssimMatu2,Par,KEpsiB,
                            KWP, Tr, sumDD, SeuilTemp, Ltr, sumDDPhasePrec,NoPhase: double;
                       Var  assimPot :Double);

Procedure AssimRUExWUE(Const KAssimBVP, KAssimMatu1, KAssimMatu2,
                            KEpsiB, Par, sumDD, SeuilTemp, Ltr, sumDDPhasePrec,
                            Tr, TrPot,NoPhase: double;
                       Var  assimPot :Double);

Procedure AssimRUESwitchWUE(Const KAssimBVP, KAssimMatu1, KAssimMatu2,
                            KEpsiB, Par, sumDD, SeuilTemp, Ltr, sumDDPhasePrec,
                            KWP, Tr, TrPot,NoPhase: double;
                       Var  assimPot :Double);
//ok
Procedure Assimilats(Const KEpsiB, Par, Ltr, Cstr: double;
                       Var  assim :Double);
//ok
Procedure EvalRespMaint(Const biomTot, tempMoy, KtempMaint, kRespMaint, BiomFeuilles, Numphase : double;
                        Var respMaint : Double);
//ok
Procedure EvolBiomasse(Const assimPot, tr, trPot, respMaint, KpenteAero,KbaseAero : double;
                        Var dayBiomAero, dayBiomTot, biomTot, biomAero : Double) ;
//ok
Procedure EvolBiomTot(Const assimPot, respMaint, KpenteAero, KbaseAero : double;
                        Var dayBiomAero, dayBiomTot, biomTot, biomAero : Double) ;


Procedure EvalSlaRapBiom(Const SlaBVP, SlaRPR,
                             KpenteSla, dayBiomLeaf, BiomLeaf : Double;
                       Var sla : Double);


Procedure EvolBiomLeaf(Const KpenteLaiDev, biomAero, KbaseLaiDev, dayRdt,
                              dayBiomTot, Krealloc, RespMaint, AssimPot,
                              Nophase : Double;
                       Var biomLeaf, dayBiomLeaf, rdt, ManqueAssim, MaxBiomLeaf : Double);
//ok
Procedure EvolLAI(Const sla, biomLeaf : Double;
                  Var lai : Double);
//ok
Procedure EvolKcp(Const KcMax, Ltr : Double;
                  Var Kcp : Double);

Procedure EvolRdt(Const SeuilTempMatu1,rdtPot, tempMoy, KTempPhase, tr, trPot, Nophase : Double;
                  Var dayRdt : Double);
// A supprimer plus tard
Procedure EvalRdtPot(Const biomTotStadeFlor, biomTotStadeIp, Krdt : Double;
                     Var rdtPot : Double);

implementation
uses UChaines, Math, ModelsManage, Main, GestionDesErreurs,SysUtils;
{Proc. d'evolution du peuplement (Plante)}
//-------------------------------------------------------------------------//
// Initialisation des variables de la culture (Crop)
//-------------------------------------------------------------------------//
Procedure InitCrop(Const SeuilTempBVP,SeuilTempGermi,SeuilTempMatu1,SeuilTempMatu2,SeuilTempRPR: double; var numPhase,SumDDMax,SeuilTemp,DegreDay:double);
begin
   numPhase:=1;
   SumDDMax:= SeuilTempBVP+SeuilTempGermi+SeuilTempMatu1+SeuilTempMatu2+SeuilTempRPR;
   SeuilTemp:=SeuilTempGermi;
   //DegreDay:=0;
end;


Procedure EvalRdtPot(Const biomTotStadeFlor, biomTotStadeIp, Krdt : Double;
                     Var rdtPot : Double);
{
Evaluation du rendement potentiel en fonction de l'état de la plante
définie par la différence de biomasse totale du stade Ip a Floraison
Ie durant la phase RPR (reproductive)
Nota : pas de prise ne compte d'un effet photoP sur la longueur du cycle et
donc du rendement.
}
Begin
  rdtPot := max(0,kRdt * (biomTotStadeFlor - biomTotStadeIp));
End;

Procedure EvalSumPhotoPer (Const DayLength, KPPCrit, kPPSens, kPPForme: Double;
                           Var sumPP, SeuilPP : Double);
{ TODO : Modif jours court ou jours long implique une nouvelle variable type
KPPShortDay, algo ecrit non fonctionnel }
{
KPPCrit : duree du jour pour la mise en place de la floraison
permet une definition d'un jour donnee en ayant kForme=3
kPPSens : amplifie l'effet de la formulation de la photoperiode
(Ex : si representation lineaire modifie la pente 0,2..1..5)
kForme : forme de la courbe d'evolution de la photoperiode
  1 : lineaire
  2 : parabole
  3 : exponentielle
Quand SumPP est >= a seuilPP alors déclenchement de la phase Matu1 gere dans
le module de gestion des phases
IN:
stRurMax : mm
RuSurf : mm
evap : mm
trPot : mm
evaPot : mm
INOUT :
stRuSurf : mm
tr : mm
stRur : mm
stRu : mm
OUT:
etr : mm
etm : mm
}
Var
   PPActual   : Double;
Begin
try
{cas jours court ou jours long :
If KPPShortDay then PPactual := max(0.01, DayLength - KPPCrit);
else PPactual := max(0.01, KPPCrit - DayLength );
}
  PPactual := max(0.01, DayLength - KPPCrit);
  seuilPP := power(10, kPPForme);
  sumPP := sumPP + kPPSens/power(PPActual,kPPForme);
  MainForm.memDeroulement.Lines.Add(TimeToStr(Time)+#9+'DayLength '+ FloatToStr(DayLength)+'sEUILPP '+ FloatToStr(SeuilPP)+' SummPP '+FloatToStr(Sumpp));

except
  AfficheMessageErreur('EvalSumPhotoPer',UBiomasse);
end;
End;

Procedure SumTemp(Const tempMoy, kTempPhase : Double;
                  Var sumDD : Double);
{
Simple cumul de degré/jour
}
Begin
   sumDD := sumDD + tempMoy -kTempPhase;
End;

////////////////////////////////////////////////////////////////////////////////

Procedure EvolSDJOpt(Const TMoy,TBase,TOpt: Double;
                     Var sumDD : Double);
{ On ne somme que les valeurs sup a Tbase, et on borne a Topt les degres
jours a ajouter. }
Begin
sumDD:=sumDD+max(min(TOpt,TMoy),TBase)-Tbase;
End;

////////////////////////////////////////////////////////////////////////////////
Procedure EvolPhenoTemp(Const SeuilPP,
                        TempGermi, TempBvp, TempRPR, TempMatu1, TempMatu2,
                        RootSpeedBVP, RootSpeedPSP, RootSpeedRPR,
                        RootSpeedMatu1,RootSpeedMatu2,
                        KRdt, ThisDate, DateSemis,BiomTot : Double;
                        Var NoPhase, Vrac, SumDD, sumPP, SeuilTemp,
                        SumDDPhasePrec, biomtotStadeIp, cycleReel, RdtPot,DateMaturite : Double);
{
InOut : SumDD, SeuilPP, SumPP, NoPhase

}
Var Change : Boolean;
    NumPhase : Integer;

Begin

  Numphase := trunc(NoPhase);
  If NumPhase = 3 Then  // Photoper PSP
      change := (sumPP >= seuilPP)
  else  change := (SumDD >= SeuilTemp);
  begin
    If change then
      begin
      inc(numPhase);
      NoPhase := NumPhase;
      SumDDPhasePrec := sumDD;
        Case NumPhase Of
         2 : Begin   // BVP Developpement vegetatif
             seuilTemp := seuilTemp + TempBVP;
             Vrac := RootSpeedBVP;
            end;
         3 : Begin   // PSP Photoper
             SumPP := 0;
             Vrac := RootSpeedPSP;
            end;
         4 : Begin   // RPR Stade initiation paniculaire
             seuilTemp := SumDD + TempRPR;
             Vrac := RootSpeedRPR;
             biomtotStadeIp := BiomTot;
            end;
         5 : Begin   // Matu1 remplissage grains
             seuilTemp := seuilTemp + TempMatu1;
             if RootSpeedMatu1 = NullValue
               then Vrac := 0
               else Vrac := RootSpeedMatu1;
             rdtPot := max(0,kRdt * (biomTot - biomTotStadeIp));
            end;
         6 : Begin   // Matu2 dessication
             seuilTemp := seuilTemp + TempMatu2;
             if RootSpeedMatu2 = NullValue
               then Vrac := 0
               else Vrac := RootSpeedMatu2;
            end;
       7 :begin // Recolte
            CycleReel := ThisDate-Datesemis;
            DateMaturite := ThisDate;
          end;
        End;  // Csae NumPhase
      end;  // End change
    end;
End;

////////////////////////////////////////////////////////////////////////////////

Procedure EvolPhenoTempC(Const SeuilPP,
                        TempGermi, TempBvp, TempRPR, TempMatu1, TempMatu2,
                        RootSpeedBVP, RootSpeedPSP, RootSpeedRPR,
                        RootSpeedMatu1,RootSpeedMatu2,
                        KRdt, ThisDate, DateSemis,BiomTot, ConstRdt : Double;
                        Var NoPhase, Vrac, SumDD, sumPP, SeuilTemp,
                        SumDDPhasePrec, biomtotStadeIp, cycleReel, RdtPot,DateMaturite : Double);
{
InOut : SumDD, SeuilPP, SumPP, NoPhase

}
Var Change : Boolean;
    NumPhase : Integer;

Begin
try
  Numphase := trunc(NoPhase);
  If NumPhase = 3 Then  // Photoper PSP
  Begin
  MainForm.memDeroulement.Lines.Add(TimeToStr(Time)+#9+'EvolPheno sEUILPP '+ FloatToStr(SeuilPP)+' SummPP '+FloatToStr(Sumpp));
      change := (sumPP >= seuilPP);
  end
  else  change := (SumDD >= SeuilTemp);
  begin
    If change then
      begin
      inc(numPhase);
      NoPhase := NumPhase;
      SumDDPhasePrec := sumDD;
        Case NumPhase Of
         2 : Begin   // BVP Developpement vegetatif
             seuilTemp := seuilTemp + TempBVP;
             Vrac := RootSpeedBVP;
            end;
         3 : Begin   // PSP Photoper
             SumPP := 0;
             Vrac := RootSpeedPSP;
            end;
         4 : Begin   // RPR Stade initiation paniculaire
             seuilTemp := SumDD + TempRPR;
             Vrac := RootSpeedRPR;
             biomtotStadeIp := BiomTot;
            end;
         5 : Begin   // Matu1 remplissage grains
             seuilTemp := seuilTemp + TempMatu1;
             if RootSpeedMatu1 = NullValue
               then Vrac := 0
               else Vrac := RootSpeedMatu1;
             rdtPot := max(0,ConstRdt + kRdt * (biomTot - biomTotStadeIp));
            end;
         6 : Begin   // Matu2 dessication
             seuilTemp := seuilTemp + TempMatu2;
             if RootSpeedMatu2 = NullValue
               then Vrac := 0
               else Vrac := RootSpeedMatu2;
            end;
       7 :begin // Recolte
            CycleReel := ThisDate-Datesemis;
            DateMaturite := ThisDate;
          end;
        End;  // Csae NumPhase
      end;  // End change
    end;
except
  AfficheMessageErreur('EvolPhenoTempCereales',UBiomasse);
end;
End;

////////////////////////////////////////////////////////////////////////////////


procedure EvalLtr(const Kdf, Lai : Double;
                  var ltr : Double);
begin
  try
    ltr := exp(-KDf * lai) ;
  except
    AfficheMessageErreur('EvalLtr',UBiomasse);
  end;
end;


Procedure FreinEpsiBTemp(Const Tbase,TOpt, TMin, Tmax : Double;
                         Var IndTemp : Double );
Var
 TempBorne,Topt1 : Double;
Begin
Topt1 :=25;
// TempBorne := 0.25* TMin + 0.75 * Tmax;
TempBorne := (TMin+TMax)/2;
 TempBorne := Min (TempBorne, Topt1);
 TempBorne := Max (TempBorne, TBase);
 IndTemp := (TempBorne - TBase)/Topt1-TBase;
End;
// Pas retenue
// validation avec cstreagle 22/08/2002
Procedure AssimilatPotTemp(Const TabKassim: array of double;
                            KEpsiB, Par, sumDD, SeuilTemp, Ltr, IndTemp,sumDDPhasePrec: double;
                            numPhase : Integer;
                       Var  assimPot :Double);
{
AssimPot de matiere seche Kg/ha/j
rg Mj/m²/j (* 10000 = 1 ha)
KEpsiB en g/Mj (/1000 = 1 kg)
kpar = 0.5 ou 0.48
}
Var
  KAssim : Double;
  PhasePred    : Integer;

Begin
   PhasePred := numPhase-1;
// On rapporte SumDD et SeuilTemp au rapport de degré jour
// sur la seule phase en cours, avant floraison  Kassim tjs a 1
// => second membre = 0 pour phases avant floraison
   KAssim := TabKAssim[PhasePred] +
             (sumDD-sumDDPhasePrec)*(TabKAssim[numPhase]-TabKAssim[PhasePred])
                   / (SeuilTemp-sumDDPhasePrec);
   assimPot := Par * KEpsiB * KAssim
                     * 10 * (1-LTR)* IndTemp;
End;


// validation avec cstreagle 22/08/2002
Procedure AssimilatPot(Const KAssimBVP, KAssimMatu1, KAssimMatu2,
                            KEpsiB, Par, sumDD, SeuilTemp, Ltr, sumDDPhasePrec, NoPhase: double;
                       Var  assimPot :Double);
{
AssimPot de matiere seche Kg/ha/j
rg Mj/m²/j (* 10000 = 1 ha)
KEpsiB en g/Mj (/1000 = 1 kg)
kpar = 0.5 ou 0.48
}
Var
  KAssim : Double;
  NumPhase    : Integer;
Begin
try
   NumPhase := trunc(NoPhase);
 // Pas d'assimilation durant la germination
 // Pour les phases de maturations,On reduit progressivement KAssim :
 // en fonction de l'évolution des degres jours sur le delta de la phase en cours
   Case NumPhase of
   1    : assimPot := 0;
   2..4 : assimPot := Par * KEpsiB * KAssimBVP * 10 * (1-LTR);
   5 : begin
      KAssim := KAssimBVP +
             (sumDD-sumDDPhasePrec)*(KAssimMatu1 -KAssimBVP)
                   / (SeuilTemp-sumDDPhasePrec);
      assimPot := Par * KEpsiB * KAssim * 10 * (1-LTR);
     end;
   6 : begin
      KAssim := KAssimMatu1 +
             (sumDD-sumDDPhasePrec)*(KAssimMatu2 -KAssimMatu1)
                   / (SeuilTemp-sumDDPhasePrec);
      assimPot := Par * KEpsiB * KAssim * 10 * (1-LTR);
      end;
   end; // Case
except
  AfficheMessageErreur('AssimilatPot',UBiomasse);
end;
End;

Procedure AssimWUE(Const KAssimBVP, KAssimMatu1, KAssimMatu2,Par,KEpsiB,
                            KWP, Tr, sumDD, SeuilTemp, Ltr, sumDDPhasePrec,NoPhase: double;
                       Var  assimPot :Double);
{
AssimPot de matiere seche Kg/ha/j
rg Mj/m²/j (* 10000 = 1 ha)
KEpsiB en g/Mj (/1000 = 1 kg)
kpar = 0.5 ou 0.48
KWP * 10 0000 pour rapporter en Kg/Ha de conv eau/sucre
}
Var
  KAssim : Double;
  NumPhase    : Integer;
Begin
try
   NumPhase := trunc(NoPhase);
 // Pas d'assimilation durant la germination
 // Pour les phases de maturations,On reduit progressivement KAssim :
 // en fonction de l'évolution des degres jours sur le delta de la phase en cours
   Case NumPhase of
   1    : assimPot := 0;
   2..4 : assimPot := Par * KEpsiB * KAssimBVP * 10 * (1-LTR);
   5 : begin
      KAssim := KAssimBVP +
             (sumDD-sumDDPhasePrec)*(KAssimMatu1 -KAssimBVP)
                   / (SeuilTemp-sumDDPhasePrec);
      assimPot := KWP * 10000 * Tr * Kassim;
     end;
   6 : begin
      KAssim := KAssimMatu1 +
             (sumDD-sumDDPhasePrec)*(KAssimMatu2 -KAssimMatu1)
                   / (SeuilTemp-sumDDPhasePrec);
      assimPot := KWP * 10000 * Tr * Kassim;
      end;
   end; // Case
except
  AfficheMessageErreur('AssimWUE',UBiomasse);
end;
End;

{ TODO : // AssimilatRUExWUEremplacera a terme AssimilatPot et EvolBiomasse avec EvolBiomTot }
Procedure AssimRUExWUE(Const KAssimBVP, KAssimMatu1, KAssimMatu2,
                            KEpsiB, Par, sumDD, SeuilTemp, Ltr, sumDDPhasePrec,
                            Tr, TrPot,NoPhase: double;
                       Var  assimPot :Double);
{
AssimPot de matiere seche Kg/ha/j
rg Mj/m²/j (* 10000 = 1 ha)
KEpsiB en g/Mj (/1000 = 1 kg)
kpar = 0.5 ou 0.48
}
Var
  KAssim : Double;
  NumPhase    : Integer;
Begin
try
   NumPhase := trunc(NoPhase);
 // Pas d'assimilation durant la germination
 // Pour les phases de maturations,On reduit progressivement KAssim :
 // en fonction de l'évolution des degres jours sur le delta de la phase en cours
   Case NumPhase of
   1    : assimPot := 0;
   2..4 : assimPot := Par * KEpsiB * KAssimBVP * 10 * (1-LTR);
   5 : begin
      KAssim := KAssimBVP +
             (sumDD-sumDDPhasePrec)*(KAssimMatu1 -KAssimBVP)
                   / (SeuilTemp-sumDDPhasePrec);
      assimPot := Par * KEpsiB * KAssim * 10 * (1-LTR) * Tr/TrPot;
     end;
   6 : begin
      KAssim := KAssimMatu1 +
             (sumDD-sumDDPhasePrec)*(KAssimMatu2 -KAssimMatu1)
                   / (SeuilTemp-sumDDPhasePrec);
      assimPot := Par * KEpsiB * KAssim * 10 * (1-LTR) * Tr/TrPot;
      end;
   end; // Case
except
  AfficheMessageErreur('AssimRUExWUE',UBiomasse);
end;
End;

Procedure AssimRUESwitchWUE(Const KAssimBVP, KAssimMatu1, KAssimMatu2,
                            KEpsiB, Par, sumDD, SeuilTemp, Ltr, sumDDPhasePrec,
                            KWP, Tr, TrPot,NoPhase: double;
                       Var  assimPot :Double);
{
AssimPot de matiere seche Kg/ha/j
rg Mj/m²/j (* 10000 = 1 ha)
KEpsiB en g/Mj (/1000 = 1 kg)
kpar = 0.5 ou 0.48
KWP * 10 0000 pour rapporter en Kg/Ha de conv eau/sucre}
Var
  KAssim, assimWUE : Double;
  NumPhase    : Integer;
Begin
try
   NumPhase := trunc(NoPhase);
 // Pas d'assimilation durant la germination
 // Pour les phases de maturations,On reduit progressivement KAssim :
 // en fonction de l'évolution des degres jours sur le delta de la phase en cours
   Case NumPhase of
   1    : Begin
            assimPot := 0;
            assimWUE := 0;
          end;
   2..4 : Begin
           assimPot := Par * KEpsiB * KAssimBVP * 10 * (1-LTR);
           assimWue := KWP * 10000 * Tr * KassimBVP;
         end;
   5 : begin
      KAssim := KAssimBVP +
             (sumDD-sumDDPhasePrec)*(KAssimMatu1 -KAssimBVP)
                   / (SeuilTemp-sumDDPhasePrec);
      assimPot := Par * KEpsiB * KAssim * 10 * (1-LTR) * Tr/TrPot;
   assimWue := KWP * 10000 * Tr * Kassim;
     end;
   6 : begin
      KAssim := KAssimMatu1 +
             (sumDD-sumDDPhasePrec)*(KAssimMatu2 -KAssimMatu1)
                   / (SeuilTemp-sumDDPhasePrec);
      assimPot := Par * KEpsiB * KAssim * 10 * (1-LTR) * Tr/TrPot;
      assimWue := KWP * 10000 * Tr * Kassim;
      end;
   end; // Case
   assimpot := min(assimpot, assimWUE);
except
  AfficheMessageErreur('AssimRUESwitchWUE',UBiomasse);
end;
End;

// Pas retenue
// validation avec cstreagle 22/08/2002
Procedure AssimilatPotRapLeaf(Const KEpsiB, Par, Ltr, MaxBiomLeaf, BiomLeaf, NumPhase: double;
                       Var  assimPot :Double);
{
AssimPot de matiere seche Kg/ha/j
rg Mj/m²/j (* 10000 = 1 ha)
KEpsiB en g/Mj (/1000 = 1 kg)
kpar = 0.5 ou 0.48
}
Var
  KAssim : Double;
Begin
try
   KAssim := min (1,1-max(0,(BiomLeaf-MaxBiomLeaf)/(0.3*BiomLeaf)));
   assimPot := Par * KEpsiB * KAssim
                     * 10 * (1-LTR);
// prise en compte de la non assimilation en periode de germination
  if numphase < 2 then
   assimpot := 0;
except
  AfficheMessageErreur('AssimilatPotRapLeaf',UBiomasse);
end;
End;

(*
jeu d'essai avec reduction d'assimilation et TMin
Procedure AssimilatPot(Const TabKassim : array of double;
                            KEpsiB, Par, sumDD, SeuilTemp, Ltr, TMin: double;
                            numPhase : Integer;
                       Var  assimPot :Double);
{
AssimPot de matiere seche Kg/ha/j
rg Mj/m²/j (* 10000 = 1 ha)
KEpsiB en g/Mj (/1000 = 1 kg)
kpar = 0.5 ou 0.48
}
Var
  KAssim, Ktemp : Double;
  PhasePred    : Integer;
Begin
   PhasePred := numPhase-1;
{ TODO : Revoir rapport sumdd et SeuilTemp : rapport sur phase seule. }
   KAssim := TabKAssim[PhasePred] +
             sumDD*(TabKAssim[numPhase]-TabKAssim[PhasePred])
                   / SeuilTemp;
{   If Tmoy < 20 then KTemp := Max( 0,Tmoy-10)/10
      else if Tmoy < 40 then Ktemp := 1
         else if Tmoy < 60 then Ktemp := 1 - ((TMoy-40)/ 20)
            else Ktemp := 0;
}
   If TMin < 22 then KTemp := Max( 0,TMin-10)/12
      else Ktemp := 1 ;
   assimPot := Par * KEpsiB * KAssim * Ktemp
                     * 10 * (1-LTR);
End;
*)

Procedure Assimilats(Const KEpsiB, Par, Ltr, Cstr: double;
{Proc 76}            Var  assim :Double);
// ViB: à supprimer, spé palmier
// proc reprise par Palmier.EvolOffreBruteJour Proc 205
Begin
try
   assim := Par * KEpsiB * Cstr * 10 * (1-LTR);
except
  AfficheMessageErreur('Assimilats',UBiomasse);
end;
End;

procedure EvalRespMaint(const biomTot, tempMoy, KtempMaint, kRespMaint, BiomFeuilles, Numphase : Double;
                        var respMaint : Double);
{
RespMaint Kg/ha/j  en équivalent matière sèche
KRespMaint     (0.015)
KTempMaint °C  (25 )
}
begin
  try
    respMaint := kRespMaint * biomTot * power(2, (tempMoy - kTempMaint) / 10);
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


Procedure EvolBiomasse(Const assimPot, tr, trPot, respMaint, KpenteAero,
                             KbaseAero : double;
                        Var dayBiomAero, dayBiomTot, biomTot, biomAero : Double) ;
{
Biomtot en Kg/ha/j
Basé sur des relations allométriques bornées aux extrèmes
}
Begin
try
  dayBiomTot := assimPot* tr/trPot - RespMaint;
  biomTot := biomTot + dayBiomTot;
  dayBiomAero := BiomAero;
  BiomAero := Min(0.9, kPenteAero * biomTot + kBaseAero)* biomTot ;
  dayBiomAero := BiomAero - dayBiomAero;
except
  AfficheMessageErreur('EvolBiomasse' + FloatToStr(trpot-RespMaint),UBiomasse);
end;
End;


Procedure EvolBiomTot(Const assimPot, respMaint, KpenteAero, KbaseAero : double;
                        Var dayBiomAero, dayBiomTot, biomTot, biomAero : Double) ;
{
Biomtot en Kg/ha/j
Basé sur des relations allométriques bornées aux extrèmes
}
Begin
try
  dayBiomTot := assimPot - RespMaint;
  biomTot := biomTot + dayBiomTot;
  dayBiomAero := BiomAero;
  BiomAero := Min(0.9, kPenteAero * biomTot + kBaseAero)* biomTot ;
  dayBiomAero := BiomAero - dayBiomAero;
except
  AfficheMessageErreur('EvolBiomTot',UBiomasse);
end;
End;

Procedure EvalSlaRapBiom(Const SlaBVP, SlaRPR,
                             KpenteSla, dayBiomLeaf, BiomLeaf : Double;
                       Var sla : Double);
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

Equation :

SLA(j) = min (SLAmax , max (SLAmin ,
        [(SLA(j-1) - dSLA*(SLAmax-SLAmin)) *  (BMfeuille(j-1)) / BMfeuille(j))
+   SLAmax * (BMfeuille(j)-BMfeuille(j-1)) / BMfeuille(j)]))
}
Begin
try
/// Modif cb 03/01/29
{ If dayBiomLeaf >0 then
     sla := (sla - KpenteSla * (sla- TabSla[ord(phRPR)]))
                 * (BiomLeaf- dayBiomLeaf)/BiomLeaf
                 + TabSla[ord(phBVP)] * (dayBiomLeaf/BiomLeaf)
 else sla := sla - KpenteSla * (sla- TabSla[ord(phRPR)]);
 }
 If dayBiomLeaf >0 then
     sla := (sla - KpenteSla * (sla- SlaRPR))
                 * (BiomLeaf- dayBiomLeaf)/BiomLeaf
                 + SlaBVP * (dayBiomLeaf/BiomLeaf);
/// Modif cb 03/01/29
   sla := min(SlaBVP,
             max(SlaRPR , sla));
except
  AfficheMessageErreur('EvalSlaRapBiom',UBiomasse);
end;
End;

////////////////////////////////////////////////////////////////////////////////

Procedure EvalSlaCereales(Const SlaBVP, SlaRPR,
                             KpenteSla, dayBiomLeaf, BiomLeaf, NumPhase : Double;
                       Var sla : Double);
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

Equation :

SLA(j) = min (SLAmax , max (SLAmin ,
        [(SLA(j-1) - dSLA*(SLAmax-SLAmin)) *  (BMfeuille(j-1)) / BMfeuille(j))
+   SLAmax * (BMfeuille(j)-BMfeuille(j-1)) / BMfeuille(j)]))
}
Begin
try
/// Modif cb 03/01/29
{ If dayBiomLeaf >0 then
     sla := (sla - KpenteSla * (sla- TabSla[ord(phRPR)]))
                 * (BiomLeaf- dayBiomLeaf)/BiomLeaf
                 + TabSla[ord(phBVP)] * (dayBiomLeaf/BiomLeaf)
 else sla := sla - KpenteSla * (sla- TabSla[ord(phRPR)]);
 }
// Modif 19/08/04
{ If dayBiomLeaf >0 then
     sla := (sla - KpenteSla * (sla - SlaRPR))
                 * (BiomLeaf- dayBiomLeaf)/BiomLeaf
                 + SlaBVP * (dayBiomLeaf/BiomLeaf);
}
    If Numphase = 1 then sla := SlaBvp;
     sla := (sla - KpenteSla * (sla - SlaRPR))
                 * (BiomLeaf- dayBiomLeaf)/BiomLeaf
                 + (SlaBVP+sla)/2 * (dayBiomLeaf/BiomLeaf);
/// Modif cb 03/01/29
   sla := min(SlaBVP,
             max(SlaRPR , sla));
except
  AfficheMessageErreur('EvalSlaCereales',UBiomasse);
end;
End;

////////////////////////////////////////////////////////////////////////////////



Procedure EvolBiomLeaf(Const KpenteLaiDev, biomAero, KbaseLaiDev, dayRdt,
                              dayBiomTot, Krealloc, RespMaint, AssimPot,
                              Nophase : Double;
                       Var biomLeaf, dayBiomLeaf, rdt, ManqueAssim, MaxBiomLeaf : Double);
{
Evolution du LAI en 2 étapes :
Avant Floraison :
Relation allométrique biomasse Aérienne-Foliaire, bornée aux extrèmes
Pb Apparent : la biomasse foliaire chute pendant la phase RPR en cycle Long!
pb de calage des la pente et du bornage ?
Après floraison :
concept de compétition (répartition assimilats), une scénescence peut
apparaitre si l'apport en assimilat (dayBiomTot) est inf à la demande
(dayRdt)pour la croissance des grains, on retranche alors cette différence
à la biomasse aérienne.
Remarque : avec les réserve (palmier a huile) on instituait une réallocation
moindre que la différence des demandes
On borne cette diminution à 10 kg/ha pour des problèmes de simulation
(division/0) dans les calculs de transpiration
}
//Krealloc := 0.5;
Var NewBLeaf, cM, bM : Double;
    NumPhase : Integer;

Begin
try
//!!! Kchimique dans rdt...mil sorgho = 1 donc sans effet mais ?
  If NoPhase <= 4 then begin  // PhRPR phase reproductive
//!!! Test scenescence et dayBiomTot!!!
//    if daybiomTot < 0 then TRes[biomLeaf] := max (TRes[biomLeaf]+daybiomTot, 1)
//    else begin
// equation de Misterlich : y = aM + bM * cM puiss x
// Bornes des valeurs : Am -0.12..0.12, bM 0..1, x 0..50, cM
// lissage de la relation linéaire au point de changement de pente
// (bornage à 0.1 ou 0.2)
// données d'entrées pente relation allometrique, seuil asymptotique (bornage)
// base de la pente
// relation lineaire pour faire le lien avec la pente de Misterlich
// aM = seuil et bM = base - seuil)
// cM = (penteAllom / bM + 0.78)/0.75
//Attention valeurs rapporte en T/Ha pour ne pas partir dans les choux
    bM :=  kBaseLaiDev - 0.1;
    cM := ((kPenteLaiDev*1000)/ bM + 0.78)/0.75;
    NewBLeaf := (0.1 + bM * power(cM,BiomAero/1000))* BiomAero;
//    NewBLeaf := max(0.1, kPenteLaiDev * BiomAero + kBaseLaiDev) * BiomAero;
    NewBLeaf := min(NewBLeaf,BiomAero);
    //   end;
  end
  else
  Begin
// phase de scenescence ie il ne peut y avoir que decroissance de biomleaf
    ManqueAssim := 0;
    if NoPhase = 5 then  // Phase Matu1
    begin
//dans ManqueAssim on ne rajoute pas le manque en assimilat
// (dayBiomTot < 0) pour l'evaluation du puit du rendement
//? et pour les feuilles quelle est alors la part a retrancher,
//  faut il ajouter ce manque de daybiomtot?
       ManqueAssim := max(0,(dayRdt - max(0,dayBiomTot)));
// en attendant diminution de l'augmentation de rdt
// Krealloc : pourcentage d'assimilat realloue par les feuilles/?Tiges
// modif du 11/01/05 cb
//  avant     rdt:= rdt + max(0, dayRdt - ManqueAssim * KRealloc);
// bornage de la croissance de rendement par la qte restante de feuilles
       rdt:= rdt + min((biomleaf-10)* KRealloc*0.5,max(0, dayRdt - ManqueAssim * KRealloc));
// Fin modif du 11/01/05 cb
    end;
// on ajoute au ManqueAssim ce qui est pris pour la respMaint,
// ie dayBiomtot en val negative
    ManqueAssim := ManqueAssim - min(0,dayBiomTot);
//    else ManqueAssim := max(0,(RespMaint) - AssimPot);
    NewBLeaf := max (biomLeaf- (ManqueAssim * KRealloc)*0.5, 10);
//    NewBLeaf := max (biomLeaf- ManqueAssim, 10);
//    NewBLeaf := max (biomLeaf- (ManqueAssim * KRealloc), 10);
  End;
  dayBiomLeaf := NewBLeaf - biomLeaf;
  biomLeaf := NewBLeaf;
// Test reduction KAssim avec des rapports de biomasse feuilles
  MaxBiomLeaf := Max(MaxBiomLeaf, BiomLeaf) ;
except
  AfficheMessageErreur('EvolBiomLeaf',UBiomasse);
end;
End;

////////////////////////////////////////////////////////////////////////////////

/// ATTENTION faire une nouveau module pour tester la diff.... 

Procedure EvolBiomLeaf2(Const KpenteLaiDev, biomAero, KbaseLaiDev, dayRdt,
                              dayBiomTot, Krealloc, RespMaint, AssimPot,
                              Nophase : Double;
                       Var biomLeaf, dayBiomLeaf, rdt, ManqueAssim, MaxBiomLeaf : Double);
{
Evolution du LAI en 2 étapes :
Avant Floraison :
Relation allométrique biomasse Aérienne-Foliaire, bornée aux extrèmes
Pb Apparent : la biomasse foliaire chute pendant la phase RPR en cycle Long!
pb de calage des la pente et du bornage ?
Après floraison :
concept de compétition (répartition assimilats), une scénescence peut
apparaitre si l'apport en assimilat (dayBiomTot) est inf à la demande
(dayRdt)pour la croissance des grains, on retranche alors cette différence
à la biomasse aérienne.
Remarque : avec les réserve (palmier a huile) on instituait une réallocation
moindre que la différence des demandes
On borne cette diminution à 10 kg/ha pour des problèmes de simulation
(division/0) dans les calculs de transpiration
}
//Krealloc := 0.5;
Var NewBLeaf, cM, bM : Double;
    NumPhase : Integer;

Begin
try
//!!! Kchimique dans rdt...mil sorgho = 1 donc sans effet mais ?
  If NoPhase <= 4 then begin  // PhRPR phase reproductive
//!!! Test scenescence et dayBiomTot!!!
//    if daybiomTot < 0 then TRes[biomLeaf] := max (TRes[biomLeaf]+daybiomTot, 1)
//    else begin
// equation de Misterlich : y = aM + bM * cM puiss x
// Bornes des valeurs : Am -0.12..0.12, bM 0..1, x 0..50, cM
// lissage de la relation linéaire au point de changement de pente
// (bornage à 0.1 ou 0.2)
// données d'entrées pente relation allometrique, seuil asymptotique (bornage)
// base de la pente
// relation lineaire pour faire le lien avec la pente de Misterlich
// aM = seuil et bM = base - seuil)
// cM = (penteAllom / bM + 0.78)/0.75
//Attention valeurs rapporte en T/Ha pour ne pas partir dans les choux
    bM :=  kBaseLaiDev - 0.1;
    cM := ((kPenteLaiDev*1000)/ bM + 0.78)/0.75;
    NewBLeaf := (0.1 + bM * power(cM,BiomAero/1000))* BiomAero;
//    NewBLeaf := max(0.1, kPenteLaiDev * BiomAero + kBaseLaiDev) * BiomAero;
    NewBLeaf := min(NewBLeaf,BiomAero);
    //   end;
  end
  else
  Begin
// phase de scenescence ie il ne peut y avoir que decroissance de biomleaf
    ManqueAssim := 0;
    if NoPhase = 5 then  // Phase Matu1
    begin
//dans ManqueAssim on ne rajoute pas le manque en assimilat
// (dayBiomTot < 0) pour l'evaluation du puit du rendement
//? et pour les feuilles quelle est alors la part a retrancher,
//  faut il ajouter ce manque de daybiomtot?
       ManqueAssim := max(0,(dayRdt - max(0,dayBiomTot)));
// en attendant diminution de l'augmentation de rdt
// Krealloc : pourcentage d'assimilat realloue par les feuilles/?Tiges
// modif du 11/01/05 cb
//  avant     rdt:= rdt + max(0, dayRdt - ManqueAssim * KRealloc);
// bornage de la croissance de rendement par la qte restante de feuilles
       rdt:= rdt + min((biomleaf-10)* KRealloc*0.5,max(0, dayRdt - ManqueAssim *(1- KRealloc)));
// Fin modif du 11/01/05 cb
    end;
// on ajoute au ManqueAssim ce qui est pris pour la respMaint,
// ie dayBiomtot en val negative
    ManqueAssim := ManqueAssim - min(0,dayBiomTot);
//    else ManqueAssim := max(0,(RespMaint) - AssimPot);
    NewBLeaf := max (biomLeaf- (ManqueAssim * KRealloc)*0.5, 10);
//    NewBLeaf := max (biomLeaf- ManqueAssim, 10);
//    NewBLeaf := max (biomLeaf- (ManqueAssim * KRealloc), 10);
  End;
  dayBiomLeaf := NewBLeaf - biomLeaf;
  biomLeaf := NewBLeaf;
// Test reduction KAssim avec des rapports de biomasse feuilles
  MaxBiomLeaf := Max(MaxBiomLeaf, BiomLeaf) ;
except
  AfficheMessageErreur('EvolBiomLeaf2',UBiomasse);
end;
end;

////////////////////////////////////////////////////////////////////////////////

Procedure EvolLAI(Const sla, biomLeaf : Double;
                  Var lai : Double);
{
Evolution du LAI en fonction de la surface massique et de la biomasse
foliaire
}

Begin
try
  lai := sla * biomLeaf;
except
  AfficheMessageErreur('EvolLAI',UBiomasse);
end;
End;

procedure EvolKcp(const KcMax, Ltr : Double;
                  var Kcp : Double);
begin
  try
    kcp := KcMax * (1 - Ltr);
  except
    AfficheMessageErreur('EvolKcp',UBiomasse);
  end;
end;


Procedure EvolRdt(Const SeuilTempMatu1,
                       rdtPot, tempMoy, KTempPhase, tr, trPot, Nophase : Double;
                  Var dayRdt : Double);
{

}
Begin
try
  If (NoPhase = 5)Then  // PhMatu1
      dayRdt := RdtPot * ((tempMoy -kTempPhase)/SeuilTempMatu1) * tr/trPot
  else dayRdt := 0;
except
  AfficheMessageErreur('EvolRdt',UBiomasse);
end;
End;

Procedure EvalBiomIni (Const SlaBVP,KResGrain, kPenteAero, kBaseAero,densite,
                             biomGrain,SeuilTempGermi : Double;
                     Var biomTot, BiomAero, sla, numphase,VRac,SeuilTemp :Double);
Begin
try
  BiomTot := densite * kResGrain * BiomGrain/1000;
  BiomAero := Min(0.9, kPenteAero * biomTot + kBaseAero)* biomTot;
  sla := SlaBVP;
  numphase := 1;
  //Vrac := 0;
  SeuilTemp := SeuilTempGermi;
except
  AfficheMessageErreur('EvalBiomIni',UBiomasse);
end;
End;

Procedure SortiesResCrop (Const ThisDate, rdt : double;
                          Var DateMaturite, RdtCrop : double);
Begin
try
  DateMaturite := ThisDate;
  RdtCrop := rdt;
except
  AfficheMessageErreur('SortiesResCrop',UBiomasse);
end;
End;

////////////////////////////////////////////////////////////////////////////////////
//Liste de toutes les procedures redef en dyn de l'unite
/////////////////////////////////////////////////////////////////////////////////////
 // Rajouter stdcall à la fin pour permettre l'utilisation de procédures dans des dll.
Procedure EvalLtrDyn (Var T : TPointeurProcParam);
Begin
  EvalLtr(T[0], T[1], T[2]);
end;

Procedure EvalRespMaintDyn (Var T : TPointeurProcParam);
Begin
  EvalRespMaint(T[0], T[1], T[2],T[3], T[4],T[5],T[6]);
end;

Procedure SumTempDyn(Var T : TPointeurProcParam);
Begin
  SumTemp(T[0], T[1], T[2]);
end;

Procedure EvolSDJOptDyn(Var T : TPointeurProcParam);
Begin
  EvolSDJOpt(T[0], T[1], T[2], T[3]);
end;

Procedure EvalSumPhotoPerDyn (Var T : TPointeurProcParam);
Begin
  EvalSumPhotoPer(T[0],T[1],T[2],T[3],T[4],T[5]);
end;

Procedure EvalRdtPotDyn (Var T : TPointeurProcParam);
Begin
  EvalRdtPot(T[0],T[1],T[2],T[3]);
end;

Procedure FreinEpsiBTempDyn (Var T : TPointeurProcParam);
Begin
  FreinEpsiBTemp(T[0],T[1],T[2],T[3],T[4]);
end;

Procedure AssimilatPotRapLeafDyn (Var T : TPointeurProcParam);
Begin
  AssimilatPotRapLeaf(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;

Procedure AssimilatsDyn (Var T : TPointeurProcParam);
Begin
  Assimilats(T[0],T[1],T[2],T[3],T[4]);
end;

Procedure EvolBiomasseDyn (Var T : TPointeurProcParam);
Begin
  EvolBiomasse(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9]);
end;

Procedure EvolBiomTotDyn (Var T : TPointeurProcParam);
Begin
  EvolBiomTot(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;

Procedure EvolLAIDyn (Var T : TPointeurProcParam);
Begin
  EvolLAI(T[0],T[1],T[2]);
end;

Procedure EvolKcpDyn (Var T : TPointeurProcParam);
Begin
  EvolKcp(T[0],T[1],T[2]);
end;

Procedure EvolPhenoTempDyn (Var T : TPointeurProcParam);
Begin
  EvolPhenoTemp(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
                T[10],T[11],T[12],T[13],T[14],T[15],T[16],T[17],T[18],T[19],
                T[20],T[21],T[22],T[23],T[24]);
end;

Procedure EvolPhenoTempCDyn (Var T : TPointeurProcParam);
Begin
  EvolPhenoTempC(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
                T[10],T[11],T[12],T[13],T[14],T[15],T[16],T[17],T[18],T[19],
                T[20],T[21],T[22],T[23],T[24],T[25]);
end;

Procedure AssimilatPotDyn (Var T : TPointeurProcParam);
Begin
  AssimilatPot(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
                T[10]);
end;

Procedure AssimRUESwitchWUEDyn (Var T : TPointeurProcParam);
Begin
  AssimRUESwitchWUE(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
                T[10],T[11],T[12],T[13]);
end;

Procedure AssimRUExWUEDyn (Var T : TPointeurProcParam);
Begin
  AssimRUExWUE(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
                T[10],T[11],T[12]);
end;

Procedure AssimWUEDyn (Var T : TPointeurProcParam);
Begin
  AssimWUE(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
                T[10],T[11],T[12]);
end;

Procedure EvolBiomLeafDyn (Var T : TPointeurProcParam);
Begin
  EvolBiomLeaf(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
                T[10],T[11],T[12],T[13]);
end;

Procedure EvolBiomLeaf2Dyn (Var T : TPointeurProcParam);
Begin
  EvolBiomLeaf2(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
                T[10],T[11],T[12],T[13]);
end;

Procedure EvalSlaRapBiomDyn (Var T : TPointeurProcParam);
Begin
  EvalSlaRapBiom(T[0],T[1],T[2],T[3],T[4],T[5]);
end;

Procedure EvalSlaCerealesDyn (Var T : TPointeurProcParam);
Begin
  EvalSlaCereales(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;

Procedure EvolRdtDyn (Var T : TPointeurProcParam);
Begin
  EvolRdt(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;

Procedure EvalBiomIniDyn (Var T : TPointeurProcParam);
Begin
  EvalBiomIni(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],
              T[10],T[11],T[12]);
end;

Procedure InitCropDyn (Var T : TPointeurProcParam);
begin
   InitCrop(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8]);
end;


initialization
  TabProc.AjoutProc('EvalLtr', EvalLtrDyn);
  TabProc.AjoutProc('EvalRespMaint', EvalRespMaintDyn);
  TabProc.AjoutProc('SumTemp', SumTempDyn);
  TabProc.AjoutProc('EvolSDJOpt', EvolSDJOptDyn);
  TabProc.AjoutProc('EvalSumPhotoPer', EvalSumPhotoPerDyn);
  TabProc.AjoutProc('EvalRdtPot', EvalRdtPotDyn);
  TabProc.AjoutProc('FreinEpsiBTemp', FreinEpsiBTempDyn);
  TabProc.AjoutProc('AssimilatPotRapLeaf', AssimilatPotRapLeafDyn);
  TabProc.AjoutProc('Assimilats', AssimilatsDyn);
  TabProc.AjoutProc('EvolBiomasse', EvolBiomasseDyn);
  TabProc.AjoutProc('EvolBiomTot', EvolBiomTotDyn);
  TabProc.AjoutProc('EvolBiomLeaf', EvolBiomLeafDyn);
  TabProc.AjoutProc('EvolBiomLeaf2', EvolBiomLeaf2Dyn);
  TabProc.AjoutProc('EvolLai', EvolLAIDyn);
  TabProc.AjoutProc('EvolKcp', EvolKcpDyn);
  TabProc.AjoutProc('InitCrop', InitCropDyn);
  TabProc.AjoutProc('EvolPhenoTemp', EvolPhenoTempDyn);
  TabProc.AjoutProc('EvolPhenoTempC', EvolPhenoTempCDyn);
  TabProc.AjoutProc('AssimilatPot', AssimilatPotDyn);
  TabProc.AjoutProc('AssimRUESwitchWUE', AssimRUESwitchWUEDyn);
  TabProc.AjoutProc('AssimRUExWUE', AssimRUExWUEDyn);
  TabProc.AjoutProc('AssimWUE', AssimWUEDyn);
  TabProc.AjoutProc('EvalSlaRapBiom', EvalSlaRapBiomDyn);
  TabProc.AjoutProc('EvalSlaCereales', EvalSlaCerealesDyn);
  TabProc.AjoutProc('EvolRdt', EvolRdtDyn);
  TabProc.AjoutProc('EvalBiomIni', EvalBiomIniDyn)
end.
