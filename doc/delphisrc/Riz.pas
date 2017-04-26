unit Riz;

interface

implementation
uses Math, ModelsManage, GestionDesErreurs, SysUtils, Dialogs, Main;
var
  MonCompteur: integer;

procedure CalculeLaMoyenne(const LaValeur: Double; const LeCompteur: Integer; var
  LaMoyenne: Double);
begin
  try
    LaMoyenne := (LaMoyenne * (LeCompteur - 1) + LaValeur) / (LeCompteur);
  except
    AfficheMessageErreur('CalculeLaMoyenne', URiz);
  end;
end;

//################################################################################
//#
//#     Procédures spécifiques
//################################################################################
////////------------------------------------------------------------------------

procedure EvolPhenoStress(const SeuilPP, SommeDegresJour, SeuilTempLevee,
  SeuilTempBVP, SeuilTempRPR, SeuilTempMatu1,
  SeuilTempMatu2, StockSurface, PourcRuSurfGermi, RuSurf,
  DateDuJour, DateSemis, stRu: Double;
  var SumPP, NumPhase, SommeDegresJourPhasePrec,
  SeuilTempPhaseSuivante, ChangePhase,
  SeuilTempSousPhaseSuivante, ChangeSousPhase, NumSousPhase: Double);
// Cette procédure est appelée en début de journée et fait évoluer les phase
// phénologiques. Pour celà, elle incrémente les numéro de phase et change la
// valeur du seuil de la phase suivante. ChangePhase est un booléen permettant
// d'informer le modèle pour connaître si un jour est un jour de changement
// de phase. Celà permets d'initialiser les variables directement dans les
// modules spécifiques.
// --> Stades phénologiques pour le modèle Mil réécrit:
// 0 : du jour de semis au début des conditions favorables pour la germination et de la récolte à la fin de simulation (pas de culture)
// 1 : du début des conditions favorables pour la germination au jour de la levée
// 2 : du jour de la levée au début de la phase photopériodique
// 3 : du début de la phase photopériodiqueau début de la phase reproductive
// 4 : du début de la phase reproductive au début de la maturation
//      sousphase1  de début RPR à RPR/4
//      sousphase2  de RPR/4 à RPR/2
//      sousphase3 de RPR/2 à 3/4 RPR
//      sousphase4 de 3/4 RPR à fin RPR
// 5 : du début de la maturation au début du séchage
// 6 : du début du séchage au jour de récolte
// 7 : le jour de la récolte
var
  ChangementDePhase, ChangementDeSousPhase: Boolean;
    // on passe en variable un pseudo booléen et non directement ce booléen (pb de moteur)
begin
  try
    ChangePhase := 0;
    ChangeSousPhase := 0;
    // l'initialisation quotidienne de cette variable à faux permet de stopper le marquage d'une journée de changement de phase
    //mainForm.memDeroulement.Lines.Add('phase n°'+FloatToStr(NumPhase)+' StockSurface='+FloatToStr(StockSurface));
    if Trunc(NumPhase) = 0 then // la culture a été semée mais n'a pas germé
    begin
      if ((StockSurface >= PourcRuSurfGermi * RuSurf) or (stRu > StockSurface))
        then
      begin // on commence ds les conditions favo aujourd'hui
        NumPhase := 1;
        //mainForm.memDeroulement.Lines.Add('------> phase n°'+FloatToStr(NumPhase)+' (car StockSurface='+FloatToStr(StockSurface)+')');
        ChangePhase := 1;
        SeuilTempPhaseSuivante := SeuilTempLevee;
          { TODO : à vérif par JCC, le déclencheur étant en phase 0 les conditions favorables et non SeuilTempGermi }
      end;
    end // fin du if NumPhase=0
    else
    begin
      // vérification d'un éventuel changement de phase
      if ((Trunc(NumPhase) = 2) and (SommeDegresJour >= SeuilTempPhaseSuivante))
        then //si on change de phase de BVP à PSP aujourd'hui
        ChangementDePhase := True
      else
      begin //sinon
        if Trunc(NumPhase) <> 3 then
          ChangementDePhase := (SommeDegresJour >= SeuilTempPhaseSuivante)
        else
          ChangementDePhase := (sumPP >= seuilPP);
            // true=on quittera la phase photopériodique
      end;

      // on a changé de phase
      if ChangementDePhase then
      begin
        ChangePhase := 1;
        NumPhase := NumPhase + 1;
        SommeDegresJourPhasePrec := SeuilTempPhaseSuivante;
          // utilisé dans EvalConversion
        case Trunc(NumPhase) of
          2: SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempBVP;
            // BVP Developpement vegetatif
          3: SumPP := 0; // PSP Photoperiode
          4:
            begin
              // gestion de l'initialisation des sous-phases
              SeuilTempSousPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempRPR
                / 4; // initialisation de la somme des DJ de la 1ère sous phase
              NumSousPhase := 1; // initialisation du n° de sous phase
              MonCompteur := 0; // on est bien le 1er jour de la 1ere sous phase
              ChangeSousPhase := 1;
                // on est bien un jour de changement de sous phase (en locurence, la première...)
              // gestion du seuil de la phase suivante
              SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempRPR;
                // RPR Stade initiation paniculaire
            end;
          5: SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu1;
            // Matu1 remplissage grains
          6: SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu2;
            // Matu2 dessication
        end; // Case NumPhase
      end; // end change

      // gestion des sous-phases de la phase RPR (4)
      if Trunc(NumPhase) = 4 then
      begin
        ChangementDeSousPhase := (SommeDegresJour >=
          SeuilTempSousPhaseSuivante);
        if ChangementDeSousPhase then
        begin
          SeuilTempSousPhaseSuivante := SeuilTempSousPhaseSuivante + SeuilTempRPR
            / 4;
          NumSousPhase := NumSousPhase + 1;
          MonCompteur := 1;
          ChangeSousPhase := 1;
        end
        else
          Inc(MonCompteur);
      end; // fin du if Trunc(NumPhase)=4 then
    end;

    mainform.memDeroulement.Lines.Add(FloatToStr(SeuilTempPhaseSuivante));

  except
    AfficheMessageErreur('EvolPhenoStress | NumPhase: ' + FloatToStr(NumPhase) +
      ' SommeDegresJour: ' + FloatToStr(SommeDegresJour) +
      ' SeuilTempPhaseSuivante: ' + FloatToStr(SeuilTempPhaseSuivante), URiz);
  end;
end;

////////////////////////////////////////////////////////////////////////////////

procedure EvalTMinCstr(const TMin, Cstr, NumPhase, NumSousPhase: Double; var
  TMinMoy, CstrMoy: double);
begin
  try
    if ((NumPhase = 4) and (NumSousPhase = 3)) then
    begin
      CalculeLaMoyenne(TMin, MonCompteur, TMinMoy);
      CalculeLaMoyenne(Cstr, MonCompteur, CstrMoy);
    end
    else
    begin
      if NumPhase < 4 then
      begin
        TMinMoy := 0;
        CstrMoy := 0;
      end;
    end;
  except
    AfficheMessageErreur('EvalTMinCstr', URiz);
  end;
end;

////////////////////////////////////////////////////////////////////////////////

procedure EvalTMaxMoy(const TMax, NumPhase, NumSousPhase: Double; var TMaxMoy:
  double);
begin
  try
    if ((NumPhase = 4) and (NumSousPhase = 4)) then
      CalculeLaMoyenne(TMax, MonCompteur, TMaxMoy)
    else if NumPhase < 4 then
      TMaxMoy := 0;
  except
    AfficheMessageErreur('EvalTMaxMoy', URiz);
  end;
end;

////////////////////////////////////////////////////////////////////////////////

procedure EvalStressTMin(const TStress, TSterilite, TMinMoy, NumPhase,
  ChangePhase: double;
  var StressTMin: Double);
begin
  try
    if NumPhase <= 4 then
      StressTMin := 1;
    if ((NumPhase = 5) and (ChangePhase = 1)) then
      StressTMin := (max(0, min(TStress, TMinMoy) - TSterilite)) / Max(0.0001,
        TStress - TSterilite);
  except
    AfficheMessageErreur('EvalStressTMin', URiz);
  end;
end;

////////////////////////////////////////////////////////////////////////////////

procedure EvalStressTMax(const TLim, TMaxMoy, NumPhase, ChangePhase: Double; var
  StressTMax: Double);
begin
  try
    if NumPhase <= 4 then
      StressTMax := 1;
    if ((NumPhase = 5) and (ChangePhase = 1)) then
      StressTMax := Min(1, (TLim - max(TMaxMoy, 35)) / (TLim - 35));
  except
    AfficheMessageErreur('EvalStressTMax', URiz);
  end;
end;

////////////////////////////////////////////////////////////////////////////////

procedure EvalStressCstr(const CstrMoy, NumPhase, ChangePhase: Double; var
  StressCstr: Double);
begin
  try
    if NumPhase <= 4 then
      StressCstr := 1;
    if ((NumPhase = 5) and (ChangePhase = 1)) then
      StressCstr := min(1, CstrMoy / 0.5);
  except
    AfficheMessageErreur('StressCstr', URiz);
  end;
end;

////////////////////////////////////////////////////////////////////////////////

procedure EvalSterilite(const StressTMin, StressTMax, StressCstr, NumPhase,
  ChangePhase: Double;
  var Sterilite: Double);
begin
  try
    if NumPhase < 5 then
      Sterilite := 0;
    if ((NumPhase = 5) and (ChangePhase = 1)) then
    begin
      Sterilite := 100 * (1 - StressTMin * StressTMax * StressCstr);
    end;
  except
    AfficheMessageErreur('EvalSterilite', URiz);
  end;
end;

////////////////////////////////////////////////////////////////////////////////

procedure EvalRdtPotRiz(const KRdt, BiomasseTotale, NumPhase, ChangePhase,
  KrdtB,
  StressTMin, StressTMax, StressCstr: Double;
  var RdtPot, BiomTotStadeFloraison, BiomTotStadeIp: Double);
begin
  try
    if ((NumPhase = 4) and (ChangePhase = 1)) then
      BiomTotStadeIp := BiomasseTotale;
    if NumPhase < 5 then
      RdtPot := 0
    else
    begin
      if ((NumPhase = 5) and (ChangePhase = 1)) then
        BiomTotStadeFloraison := BiomasseTotale;
      RdtPot := (KRdt * (BiomTotStadeFloraison - BiomTotStadeIp) + KrdtB) *
        (StressTMin * StressTMax * StressCstr);
    end;
  except
    AfficheMessageErreur('EvalRdtPotRiz', UMilBilanCarbone);
  end;
end;

////////------------------------------------------------------------------------

procedure EvalAlloAeroFeuilSDJ(const NumPhase, FeuilAeroBase, FeuilAeroPente,
  SeuilTempLevee, SeuilTempBVP, SDJmax, SDJ: Double;
  var RatioFeuilles: Double);
// BiomFeuille=RatioFeuilles * BiomAero

begin
  try
    if NumPhase = 2 then
      RatioFeuilles := FeuilAeroBase
    else if ((NumPhase > 2) and (NumPhase < 5)) then
      // donc compris entre la phase d'emergence et reproductive inclus
      RatioFeuilles := max(0.05, FeuilAeroBase + FeuilAeroPente * (SDJ -
        (SeuilTempLevee + SeuilTempBVP)) / (SDJmax - (SeuilTempLevee +
        SeuilTempBVP)))
    else
      RatioFeuilles := 0;
  except
    AfficheMessageErreur('EvalAlloAeroFeuilSDJ', URiz);
  end;
end;
////////------------------------------------------------------------------------

procedure EvalPartitionTotAer(const KPenteAero, BiomTot, KBaseAero: Double;
  var RatioAero: Double);
begin
  try
    RatioAero := Min(1, KBaseAero + 2 * KPenteAero * BiomTot);
  except
    AfficheMessageErreur('EvalPartitionTotAer | KPenteAero: ' +
      FloatToStr(KPenteAero) +
      ' BiomTot: ' + FloatToStr(BiomTot) +
      ' KBaseAero: ' + FloatToStr(KBaseAero), URiz);
  end;
end;

////////------------------------------------------------------------------------

procedure EvolPartBiomasseAerienne(const DeltaBiomasseTotale, RatioAero,
  BiomasseTotale, NumPhase,
  KPenteAero, KBaseAero, ChangePhase: Double;
  var DeltaBiomasseAerienne, BiomasseAerienne: Double);
var
  BiomasseAeriennePrec: Double;
begin
  try
    if NumPhase <= 1 then // la levée n'a pas eu lieu encore
    begin
      DeltaBiomasseAerienne := 0;
      BiomasseAerienne := 0;
    end
    else
    begin // BVP et sup
      if ((NumPhase = 2) and (ChangePhase = 1)) then // initialisation
      begin
        //BiomasseAerienne := Min(0.9, KPenteAero * BiomasseTotale + KBaseAero)* BiomasseTotale; JCC 15/11/04
        BiomasseAerienne := KBaseAero * BiomasseTotale;
        DeltaBiomasseAerienne := BiomasseAerienne;
      end
      else
      begin
        BiomasseAeriennePrec := BiomasseAerienne;
        if DeltaBiomasseTotale < 0 then
          BiomasseAerienne := max(0, BiomasseAerienne + DeltaBiomasseTotale)
        else
          BiomasseAerienne := BiomasseAerienne + RatioAero *
            DeltaBiomasseTotale;
        DeltaBiomasseAerienne := BiomasseAerienne - BiomasseAeriennePrec;
      end;
    end;
  except
    AfficheMessageErreur('EvolBiomAero | DeltaBiomasseTotale: ' +
      FloatToStr(DeltaBiomasseTotale) +
      ' RatioAero: ' + FloatToStr(RatioAero) +
      ' BiomasseTotale: ' + FloatToStr(BiomasseTotale) +
      ' BiomasseAerienne: ' + FloatToStr(BiomasseAerienne), URiz);
  end;
end;

////////------------------------------------------------------------------------

procedure EvolSlaSDJ(const DeltaDegresJour, SDJBVP, SDJRPR, SlaBVP, SlaRPR,
  BiomLeaf, dayBiomLeaf, NumPhase, ChangePhase: double;
  var sla: Double);
begin
  try
    if ((NumPhase = 2) and (ChangePhase = 1)) then
      Sla := SlaBVP // soit SlaMax...
    else
    begin
      if NumPhase >= 2 then
      begin
        if NumPhase < 5 then
          //Sla:= max(SlaRPR, SlaBVP + KpenteSla* BiomLeaf/1000 + 0.0031*power(BiomLeaf/1000,2))
          //sla := max(SlaRPR,(sla + KpenteSla*BiomLeaf/1000 ))* (BiomLeaf- dayBiomLeaf)/BiomLeaf + (SlaBVP+sla)/2 * (dayBiomLeaf/BiomLeaf)
          //sla := min(sla,(sla - KpenteSla *(sla - SlaRPR)/(SlaBVP-SlaRPR))*
          sla := max(SlaRPR, (sla - (DeltaDegresJour / (SDJBVP + SDJRPR)) *
            (SlaBVP - SlaRPR)) *
            (BiomLeaf - dayBiomLeaf) / BiomLeaf + (SlaBVP + sla) / 3.2 *
              dayBiomLeaf / BiomLeaf)
        else
          Sla := SlaRPR
      end
      else
        Sla := 0;
    end;
  except
    AfficheMessageErreur('EvolSlaSDJ', URiz);
  end;
end;

////////------------------------------------------------------------------------

procedure EvolPhenoPSPStress(const SumPP, PPsens, SommeDegresJour,
  SeuilTempLevee,
  SeuilTempBVP, SeuilTempRPR, SeuilTempMatu1,
  SeuilTempMatu2, StockSurface, PourcRuSurfGermi, RuSurf,
  DateDuJour, DateSemis, stRu : Double;
  var NumPhase, SommeDegresJourPhasePrec,
  SeuilTempPhaseSuivante, ChangePhase,
  SeuilTempSousPhaseSuivante, ChangeSousPhase, NumSousPhase: Double);
{Procedure speciale pour inclure le module photoperiodique de Vaksman & Dingkuhn
qui fonctionne en degres jours et declanche IP lorsque SumPP est inferieur à PPsens}
//JCC le 21/09/05
// Cette procédure est appelée en début de journée et fait évoluer les phase
// phénologiques. Pour celà, elle incrémente les numéro de phase et change la
// valeur du seuil de la phase suivante. ChangePhase est un booléen permettant
// d'informer le modèle pour connaître si un jour est un jour de changement
// de phase. Celà permets d'initialiser les variables directement dans les
// modules spécifiques.
// --> Stades phénologiques pour le modèle Mil réécrit:
// 0 : du jour de semis au début des conditions favorables pour la germination et de la récolte à la fin de simulation (pas de culture)
// 1 : du début des conditions favorables pour la germination au jour de la levée
// 2 : du jour de la levée au début de la phase photopériodique
// 3 : du début de la phase photopériodique au début de la phase reproductive
// 4 : du début de la phase reproductive au début de la maturation
//      sousphase1  de début RPR à RPR/4
//      sousphase2  de RPR/4 à RPR/2
//      sousphase3 de RPR/2 à 3/4 RPR
//      sousphase4 de 3/4 RPR à fin RPR
// 5 : du début de la maturation au début du séchage
// 6 : du début du séchage au jour de récolte
// 7 : le jour de la récolte
var
  ChangementDePhase,
    ChangementDeSousPhase: Boolean;
      // on passe en variable un pseudo booléen et non directement ce booléen (pb de moteur)

begin
  try
    ChangePhase := 0;
    ChangeSousPhase := 0;
    // l'initialisation quotidienne de cette variable à faux permet de stopper le marquage d'une journée de changement de phase
    if (Trunc(NumPhase) = 0) then // la culture a été semée mais n'a pas germé
    begin
      if ((StockSurface >= PourcRuSurfGermi * RuSurf) or (stRu > StockSurface))
        then
      begin // on commence ds les conditions favo aujourd'hui
        NumPhase := 1;
        ChangePhase := 1;
        SeuilTempPhaseSuivante := SeuilTempLevee;
      end;
    end // fin du if NumPhase=0
    else
    begin
      // vérification d'un éventuel changement de phase
      if ((Trunc(NumPhase) = 1) and (SommeDegresJour >= SeuilTempPhaseSuivante))
        then //si on change de phase de BVP à PSP aujourd'hui
        ChangementDePhase := True
      else
      begin //sinon
        if (Trunc(NumPhase) <> 3) then
        begin
          ChangementDePhase := (SommeDegresJour >= SeuilTempPhaseSuivante);
        end
        else
        begin
          ChangementDePhase := (sumPP <= PPsens);
            // true=on quittera la phase photopériodique
        end;
      end;
      // on a changé de phase
      if ChangementDePhase then
      begin
        ChangePhase := 1;
        NumPhase := NumPhase + 1;
        SommeDegresJourPhasePrec := SeuilTempPhaseSuivante;
          // utilisé dans EvalConversion
        case Trunc(NumPhase) of
          2: SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempBVP;
            // BVP Developpement vegetatif
          4:
            begin
              // gestion de l'initialisation des sous-phases
              SeuilTempSousPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempRPR
                / 4; // initialisation de la somme des DJ de la 1ère sous phase
              NumSousPhase := 1; // initialisation du n° de sous phase
              MonCompteur := 0; // on est bien le 1er jour de la 1ere sous phase
              ChangeSousPhase := 1;
                // on est bien un jour de changement de sous phase (en locurence, la première...)
              // gestion du seuil de la phase suivante
              SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempRPR;
                // RPR Stade initiation paniculaire
            end;
          5: SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu1;
            // Matu1 remplissage grains
          6: SeuilTempPhaseSuivante := SeuilTempPhaseSuivante + SeuilTempMatu2;
            // Matu2 dessication
        end; // Case NumPhase
      end; // end change
      // gestion des sous-phases de la phase RPR (4)
      if (Trunc(NumPhase) = 4) then
      begin
        ChangementDeSousPhase := (SommeDegresJour >=
          SeuilTempSousPhaseSuivante);
        if ChangementDeSousPhase then
        begin
          SeuilTempSousPhaseSuivante := SeuilTempSousPhaseSuivante + SeuilTempRPR
            / 4;
          NumSousPhase := NumSousPhase + 1;
          MonCompteur := 1;
          ChangeSousPhase := 1;
        end
        else
          Inc(MonCompteur);
      end; // fin du if Trunc(NumPhase)=4 then
    end;
  except
    AfficheMessageErreur('EvolPhenoStress | NumPhase: ' + FloatToStr(NumPhase) +
      ' SommeDegresJour: ' + FloatToStr(SommeDegresJour) +
      ' SeuilTempPhaseSuivante: ' + FloatToStr(SeuilTempPhaseSuivante), URiz);
  end;
end;

////////////////////////////////////////////////////////////////////////////////

procedure EvolPSPMVMD(const Numphase, ChangePhase, SomDegresJour, DegresDuJour,
  SeuilPP, PPCrit, DureeDuJour, PPExp: Double;
  var SumPP, SeuilTempPhasePrec, SeuilTempPhaseSuivante: Double);

var
  SDJPSP: Double;
  {Procedure speciale Vaksman Dingkuhn valable pour tous types de sensibilite
  photoperiodique et pour les varietes non photoperiodique. PPsens varie de 0,4
  a 1,2. Pour PPsens > 2,5 = variété non photoperiodique. SeuilPP = 13.5 PPcrit = 12
  SumPP est dans ce cas une variable quotidienne (et non un cumul) testee dans
  EvolPhenoPhotoperStress}
begin
  if NumPhase = 3 then
  begin
    if ChangePhase = 1 then
    begin
      SumPP := 100; //valeur arbitraire d'initialisation >2.5
      //SeuilTempPhasePrec:= SeuilTempPhaseSuivante;   a déjà changé dans evolpheno
      SDJPSP := max(0.01, DegresDuJour);
    end
    else
      SDJPSP := SomDegresJour - SeuilTempPhasePrec + max(0.01, DegresDuJour);

    SumPP := power((1000 / SDJPSP), PPExp) * max(0, (DureeDuJour - PPCrit)) /
      (SeuilPP - PPCrit);
    SeuilTempPhaseSuivante := SeuilTempPhasePrec + SDJPSP;
  end;
end;

procedure RS_EvalTMinMoy(const TMin, NumPhase, NumSousPhase: Double; var
  TMinMoy: double);
begin
  try
    if ((NumPhase = 4) and (NumSousPhase = 3)) then
    begin
      CalculeLaMoyenne(TMin, MonCompteur, TMinMoy);
    end
    else
    begin
      if NumPhase < 4 then
      begin
        TMinMoy := 0;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvalTMinMoy', URiz);
  end;
end;

procedure RS_EvalFtswMoy(const Ftsw, NumPhase, NumSousPhase: Double; var
  FtswMoy: double);
begin
  try
    if ((NumPhase = 4) and (NumSousPhase = 4)) then
    begin
      CalculeLaMoyenne(Ftsw, MonCompteur, FtswMoy);
    end
    else
    begin
      if NumPhase < 4 then
      begin
        FtswMoy := 0;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvalFtswMoy', URiz);
  end;
end;

procedure RS_EvalTMaxMoy(const TMax, NumPhase, NumSousPhase: Double; var
  TMaxMoy: double);
begin
  try
    if ((NumPhase = 4) and (NumSousPhase = 4)) then
      CalculeLaMoyenne(TMax, MonCompteur, TMaxMoy)
    else if NumPhase < 4 then
      TMaxMoy := 0;
  except
    AfficheMessageErreur('RS_EvalTMaxMoy', URiz);
  end;
end;


procedure RS_EvalTMaxMoy_V2_3(const TMax, TMin, HMax, HMin, NumPhase, NumSousPhase: Double; var
  TMaxMoy: double);
var
	TPanToa : double;
begin
  try
    if ((NumPhase = 4) and (NumSousPhase = 4)) then
	begin
                // 0.888 reduced to 0.8 09-02-2016
		TPanToa := (0.8 * (TMax - TMin) + TMin) - (8.32 - 0.118 * ((1-0.8)* (HMax-HMin) + HMin));
		
		CalculeLaMoyenne(TPanToa, MonCompteur, TMaxMoy);
		//  TPanToa is panicle T at time of anthesis (TOA); TOA is estimated at 10:30h and Tair at this time is estimated to be at 88.8% of the diurnal T amplitude; RH effect on air-panicle T difference (TD) is TD=8.32-0.118RH, and this term is subtracted from the air temperature; RH is thereby estimated to be at (100-88.8)% of the diurnal RH amplitude; data from Julia.
	end
    else if NumPhase < 4 then
      TMaxMoy := 0;
  except
    AfficheMessageErreur('RS_EvalTMaxMoy_V2_3', URiz);
  end;
end;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Liste de toutes les procedures redef en dyn de l'unite
// Rajouter stdcall à la fin pour permettre l'utilisation de procédures dans des dll.
/////////////////////////////////////////////////////////////////////////////////////

procedure RS_EvalTMinMoyDyn(var T: TPointeurProcParam);
begin
  RS_EvalTMinMoy(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalFtswMoyDyn(var T: TPointeurProcParam);
begin
  RS_EvalFtswMoy(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalTMaxMoyDyn(var T: TPointeurProcParam);
begin
  RS_EvalTMaxMoy(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalTMaxMoy_V2_3Dyn(var T: TPointeurProcParam);
begin
  RS_EvalTMaxMoy_V2_3(T[0], T[1], T[2], T[3], T[4], T[5], T[6]);
end;

procedure EvolPhenoStressDyn(var T: TPointeurProcParam);
begin
  EvolPhenoStress(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
      T[20]);
end;

procedure EvalTMinCstrDyn(var T: TPointeurProcParam);
begin
  EvalTMinCstr(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure EvalTMaxMoyDyn(var T: TPointeurProcParam);
begin
  EvalTMaxMoy(T[0], T[1], T[2], T[3]);
end;

procedure EvalStressTMinDyn(var T: TPointeurProcParam);
begin
  EvalStressTMin(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure EvalStressTMaxDyn(var T: TPointeurProcParam);
begin
  EvalStressTMax(T[0], T[1], T[2], T[3], T[4]);
end;

procedure EvalStressCstrDyn(var T: TPointeurProcParam);
begin
  EvalStressCstr(T[0], T[1], T[2], T[3]);
end;

procedure EvalSteriliteDyn(var T: TPointeurProcParam);
begin
  EvalSterilite(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure EvalRdtPotRizDyn(var T: TPointeurProcParam);
begin
  EvalRdtPotRiz(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10]);
end;

procedure EvalAlloAeroFeuilSDJDyn(var T: TPointeurProcParam);
begin
  EvalAlloAeroFeuilSDJ(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7]);
end;

procedure EvalPartitionTotAerDyn(var T: TPointeurProcParam);
begin
  EvalPartitionTotAer(T[0], T[1], T[2], T[3]);
end;

procedure EvolPartBiomasseAerienneDyn(var T: TPointeurProcParam);
begin
  EvolPartBiomasseAerienne(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8]);
end;

procedure EvolSlaSDJDyn(var T: TPointeurProcParam);
begin
  EvolSlaSDJ(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9]);
end;

procedure EvolPhenoPSPStressDyn(var T: TPointeurProcParam);
begin
  EvolPhenoPSPStress(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
      T[20]);
end;

procedure EvolPSPMVMDDyn(var T: TPointeurProcParam);
begin
  EvolPSPMVMD(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10]);
end;

////////////////////////////////////////////////////////////////////////////////////
initialization
  TabProc.AjoutProc('EvolPhenoStress', EvolPhenoStressDyn);
  TabProc.AjoutProc('EvalTMinCstr', EvalTMinCstrDyn);
  TabProc.AjoutProc('EvalTMaxMoy', EvalTMaxMoyDyn);
  TabProc.AjoutProc('EvalStressTMin', EvalStressTMinDyn);
  TabProc.AjoutProc('EvalStressTMax', EvalStressTMaxDyn);
  TabProc.AjoutProc('EvalStressCstr', EvalStressCstrDyn);
  TabProc.AjoutProc('EvalSterilite', EvalSteriliteDyn);
  TabProc.AjoutProc('EvalRdtPotRiz', EvalRdtPotRizDyn);
  TabProc.AjoutProc('EvalAlloAeroFeuilSDJ', EvalAlloAeroFeuilSDJDyn);
  TabProc.AjoutProc('EvalPartitionTotAer', EvalPartitionTotAerDyn);
  TabProc.AjoutProc('EvolPartBiomasseAerienne', EvolPartBiomasseAerienneDyn);
  TabProc.AjoutProc('EvolSlaSDJ', EvolSlaSDJDyn);
  TabProc.AjoutProc('EvolPhenoPSPStress', EvolPhenoPSPStressDyn);
  TabProc.AjoutProc('EvolPSPMVMD', EvolPSPMVMDDyn);
  TabProc.AjoutProc('RS_EvalTMinMoy', RS_EvalTMinMoyDyn);
  TabProc.AjoutProc('RS_EvalFtswMoy', RS_EvalFtswMoyDyn);
  TabProc.AjoutProc('RS_EvalTMaxMoy', RS_EvalTMaxMoyDyn);
  TabProc.AjoutProc('RS_EvalTMaxMoy_V2_3', RS_EvalTMaxMoy_V2_3Dyn);
  
end.

