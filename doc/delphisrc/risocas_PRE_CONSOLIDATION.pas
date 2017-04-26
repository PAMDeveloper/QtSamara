unit risocas;

interface

uses SysUtils, Dialogs;

implementation

uses Math, GestionDesErreurs, Main, ModelsManage, Riz;

// -----------------------------------------------------------------------------
// On calcule la vitesse moyenne de développement en considérant qu'elle est
// linéaire croissante de 0 à 1 entre Tbase et Topt1, puis constante et égale à
// 1 entre Topt1 et Topt2 puis décroissante de 1 à 0 entre Topt2 et Tlethale.
// Puis on calcule la température équivalente comprise entre Tbase et Topt1
// donnant la même vitesse de développement.
// On calcule les degrés jours à partir de cette température équivalente
//
// In order to get a stress-corrected term for degree-days (DegreDuJourCor),
// multiply the daily DegresDuJour by (cstr ^ DEVcstr); DEVcstr is a new parameter
// that slows down development rate under stress, assuming values between 0
// (no effect) and 1 (proportional effect of cstr).
// -----------------------------------------------------------------------------

procedure RS_EvalDegresJourVitMoy(const TMax, TMin, TBase, TOpt1, TOpt2, TL,
  cstr, DEVcstr: double;
  var DegresDuJour, DegresDuJourCor: Double);
var
  v, v1, v2: Double;
begin
  try
    v1 := ((Max(TMin, TBase) + Min(TOpt1, Min(TL, TMax))) / 2 - TBase) /
      (TOpt1 - TBase);
    v2 := (TL - (max(TMax, TOpt2) + TOpt2) / 2) / (TL - TOpt2);
    v := (v1 * (min(Min(TL, TMax), TOpt1) - TMin) +
      (min(TOpt2, max(TOpt1, Min(TL, TMax))) - TOpt1) + v2 *
      (max(TOpt2, Min(TL, TMax)) - TOpt2)) / (Min(TL, TMax) - Max(TMin, TBase));
    DegresDuJour := v * (TOpt1 - TBase);
    DegresDuJourCor := DegresDuJour / Power(Max(cstr, 0.00000001), DEVcstr);
  except
    AfficheMessageErreur('RS_EvalDegresJourVitMoy | TMax=' + FloatToStr(TMax) +
      ' TMin=' + FloatToStr(TMin) + 'TBase=' + FloatToStr(TBase) + ' TOpt1=' +
        FloatToStr(TOpt1) +
      ' TOpt2=' + FloatToStr(TOpt2) + ' TL=' + FloatToStr(TL) + ' DegresDuJour='
        +
      FloatToStr(DegresDuJour) + ' DegreDuJourCor=' +
        FloatToStr(DegresDuJourCor), URisocas);
  end;
end;

procedure RS_EvalDegresJourVitMoy_V2(const NumPhase, TMax, TMin, TBase, TOpt1,
  TOpt2, TLet, cstr, DEVcstr, StressCold : Double;
  var DegresDuJour, DegresDuJourCor: Double);
var
  v, v1, v3 : Double;
  S1, S2, S3 : Double;
  Tn, Tx : Double;
begin
  try
    if (TMax <> TMin) then
    begin
      if ((TMax <= Tbase) or (TMin >= TLet)) then
      begin
        V := 0;
      end
      else
      begin
        Tn := Max(TMin, Tbase);
        Tx := Min(TMax, TLet);
        V1 := ((Tn + Min(TOpt1, Tx)) / 2 - Tbase) / (TOpt1 - Tbase);
        S1 := V1 * Max(0, min(TOpt1, Tx) - Tn);
        S2 := 1 * Max(0, min(Tx, TOpt2) - Max(Tn, TOpt1));
        V3 := (TLet - (Max(Tx, TOpt2) + Max(TOpt2, Tn)) / 2) / (TLet - TOpt2);
        S3 := V3 * Max(0, Tx - Max(TOpt2, Tn));
        V := (S1 + S2 + S3) / (TMax - TMin);
      end
    end
    else
    begin
      if (TMax < TOpt1) then
      begin
        V := (TMax - Tbase) / (TOpt1 - Tbase);
      end
      else
      begin
        if (TMax < TOpt2) then
        begin
          V := 1
        end
        else
        begin
          V := (TLet - TMax) / (Tlet - TOpt2);
        end;
      end;
    end;
    DegresDuJour := V * (TOpt1 - TBase);
    if (NumPhase > 1) and (NumPhase < 5) then
    begin
      DegresDuJourCor := DegresDuJour * Power(Max(cstr, 0.00000001), DEVcstr);
    end
    else
    begin
      DegresDuJourCor := DegresDuJour;
    end;
    DegresDuJourCor := DegresDuJourCor * StressCold;
  except
    AfficheMessageErreur('RS_EvalDegresJourVitMoy | TMax=' + FloatToStr(TMax) +
      ' TMin=' + FloatToStr(TMin) + 'TBase=' + FloatToStr(TBase) + ' TOpt1=' +
        FloatToStr(TOpt1) +
      ' TOpt2=' + FloatToStr(TOpt2) + ' TL=' + FloatToStr(TLet) +
        ' DegresDuJour=' +
      FloatToStr(DegresDuJour) + ' DegreDuJourCor=' +
        FloatToStr(DegresDuJourCor), URisocas);
  end;
end;

// -----------------------------------------------------------------------------
// This new module serves to simulate the number of leaves produced on the main
// stem, needed to estimate upper limits to LAI and demand for assimilates.
// The basis is cumulation of DegresDuJourCor, with the introduction of a new
// variable called HaunIndex, which indicates the number of leaves already
// produced. The new parameter Phyllo (in degree days, typical varietal values
// between 20 and 60) sets the rhythm of leaf development.
// -----------------------------------------------------------------------------

procedure RS_Phyllochron(const NumPhase, DegresDuJourCor, Phyllo,
  RelPhylloPhaseStemElong: Double;
  var PhaseStemElongation, HaunGain, HaunIndex: Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      if (((NumPhase > 3) or (HaunIndex > 20)) and (NumPhase < 5)) then
      begin
        PhaseStemElongation := 1;
      end
      else
      begin
        PhaseStemElongation := 0;
      end;
      if (PhaseStemElongation = 0) then
      begin
        HaunGain := DegresDuJourCor / Phyllo;
        if (HaunIndex < 3) then
        begin
          HaunGain := HaunGain * 2;
        end;
      end
      else
      begin
        if (PhaseStemElongation = 1) then
        begin
          HaunGain := RelPhylloPhaseStemElong * (DegresDuJourCor / Phyllo);
        end;
      end;
      HaunIndex := HaunIndex + HaunGain;
    end
    else
    begin
      HaunGain := 0;
      PhaseStemElongation := 0;
    end;
  except
    AfficheMessageErreur('RS_Phyllochron', URisocas);
  end;
end;

// -----------------------------------------------------------------------------
// Calcul de la hauteur du couvert en fonction de DJ et cstr
// We introduce the new state variables ApexHeight and PlantHeight, which are
// also output variables. ApexHeight is the sum of Internode lengths on the main
// culm. PlantHeight is (ApexHeight + fn(HaunIndex)) because the leaves
// contribute to plant height. We introduce as new parameters InternodeLengthMax
// and LeafLengthMax.
// -----------------------------------------------------------------------------

procedure RS_EvolHauteur_SDJ_cstr(const PhaseStemElongation, CoeffInternodeNum,
  HaunGain, cstr, InternodeLengthMax, RelPotLeafLength,
  LeafLengthMax, CulmsPerHill, IcMean, Kdf, Ic, WtRatioLeafSheath, StressCold,
  CstrMean : Double;
  var ApexHeightGain, ApexHeight, PlantHeight, PlantWidth : Double);
var
  CorrectedCstrMean: Double;
begin
  try
    if (PhaseStemElongation = 1) then
    begin
      ApexHeightGain := HaunGain * Min(Power(Min(Ic, 1), 0.5), cstr) * StressCold
        * InternodeLengthMax;
      ApexHeightGain := ApexHeightGain * CoeffInternodeNum;
    end
    else
    begin
      ApexHeightGain := 0;
    end;
    ApexHeight := ApexHeight + ApexHeightGain;
    if (CstrMean <= 0) then
    begin
      CorrectedCstrMean := 1;
    end
    else
    begin
      CorrectedCstrMean := CstrMean;
    end;
    PlantHeight := ApexHeight + (1.5 * (1 - Kdf) * RelPotLeafLength *
      LeafLengthMax * Sqrt(IcMean) * CorrectedCstrMean * (1 + 1 /
      WtRatioLeafSheath));
    PlantWidth := power(Kdf,1.5) * 2 * Sqrt(IcMean) * RelPotLeafLength * LeafLengthMax ;{DELETED LB} {*
      Min(1.4, (1 + 0.1 * (CulmsPerHill - 1)));}
  except
    AfficheMessageErreur('RS_EvolHauteur_SDJ_cstr', URisocas);
  end;
end;

// -----------------------------------------------------------------------------
// PAR intercepté journalier (fonction de LTRkdfcl)
// -----------------------------------------------------------------------------

procedure RS_EvalParIntercepte(const PAR, {SHIFTED LB} {LIRkdfcl  ,}{/SHIFTED LB} LAI , Kdf: Double; var PARIntercepte , LIRkdfcl  :
  Double);
begin
  try
	{NEW LB}
	if (LAI > 0) and (LIRkdfcl = 0) then 
		LIRkdfcl := ( 1 - exp(-kdf * LAI));
       // right after germination there is a problem with module sequence, like chicken and egg; this overcomes it
	{NEW LB}
  
	
    PARIntercepte := PAR * LIRkdfcl;
  except
    AfficheMessageErreur('RS_EvalParIntercepte | PAR: ' + FloatToStr(PAR) +
      ' LIRkdfcl: ' + FloatToStr(LIRkdfcl), URisocas);
  end;
end;

// -----------------------------------------------------------------------------
// Assim is less sensitive than transpiration to cstr, but currently cstrassim =
// assim.
// ASScstr is a new parameter that slows attenuates cstr effect on assimilation
// relative to transpiration, assuming values between 1 (same effect on
// transpiration and assimilation) and ca. 0.5 (assim less sensitive than
// transpiration, leasing to increased transpiration efficiency TE).
// -----------------------------------------------------------------------------

procedure RS_EvalCstrAssim(const cstr, ASScstr : Double; var cstrassim : Double);
begin
  try
    cstrassim := Power(Max(cstr, 0.00000001), ASScstr);
  except
    AfficheMessageErreur('RS_EvalCstrAssim', URisocas);
  end;
end;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------

procedure RS_EvalRespMaint(const kRespMaintLeaf, kRespMaintSheath,
  kRespMaintRoot, kRespInternode, kRespPanicle: Double;
  const DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop,
    DryMatStructInternodePop,
  DryMatStructPaniclePop: Double;
  const TMoyCalc, kTempMaint, CoefficientQ10: Double;
  var RespMaintTot: Double);

var
  RespMaintLeafPop: Double;
  RespMaintSheathPop: Double;
  RespMaintRootPop: Double;
  RespMaintInternodePop: Double;
  RespMaintPaniclePop: Double;
  CoeffQ10: Double;
begin
  try
    CoeffQ10 := Power(CoefficientQ10, (TMoyCalc - kTempMaint) / 10);
    RespMaintLeafPop := kRespMaintLeaf * DryMatStructLeafPop * CoeffQ10;
    RespMaintSheathPop := kRespMaintSheath * DryMatStructSheathPop * CoeffQ10;
    RespMaintRootPop := kRespMaintRoot * DryMatStructRootPop * CoeffQ10;
    RespMaintInternodePop := kRespInternode * DryMatStructInternodePop *
      CoeffQ10;
    RespMaintPaniclePop := kRespPanicle * DryMatStructPaniclePop * CoeffQ10;
    RespMaintTot := RespMaintLeafPop + RespMaintSheathPop + RespMaintRootPop +
      RespMaintInternodePop + RespMaintPaniclePop;
  except
    AfficheMessageErreur('RS_EvalRespMaint', URisocas);
  end;
end;

procedure RS_EvolPlantTilNumTot(const NumPhase, ChangePhase, PlantsPerHill,
  TilAbility, Density, Ic, IcTillering, cstr,
  HaunIndex, HaunCritTillering: Double;
  var CulmsPerHill, CulmsPerPlant, CulmsPop: Double);
var
  TilNewPlant: Double;
begin
  try
    if ((NumPhase = 1) and (ChangePhase = 1)) then
    begin
      CulmsPerHill := PlantsPerHill;
    end
    else
    begin
      if ((NumPhase = 2) and (ChangePhase = 1)) then
      begin
        CulmsPerPlant := 1;
        CulmsPop := CulmsPerPlant * Density * PlantsPerHill;
      end
      else
      begin
        if ((NumPhase > 1) and (NumPhase < 4) and (HaunIndex >
          HaunCritTillering)) then
        begin
          TilNewPlant := cstr * Min(Max(0, (Ic - IcTillering) * TilAbility), 2);
          CulmsPerHill := CulmsPerHill + TilNewPlant;
          CulmsPerPlant := CulmsPerHill / PlantsPerHill;
          CulmsPop := CulmsPerHill * Density;
        end
        else
        begin
          CulmsPerPlant := CulmsPerPlant;
          CulmsPop := CulmsPop;
          CulmsPerHill := CulmsPerHill;
        end;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvolPlantTilNumTot', URisocas);
  end;
end;

procedure RS_EvolPlantTilNumTot_V2(const NumPhase, ChangePhase, PlantsPerHill,
  TilAbility, Density, Ic, IcTillering,
  cstr, HaunIndex, HaunCritTillering, LtrKdfcl: Double;
  var CulmsPerHill, CulmsPerPlant, CulmsPop: Double);
var
  TilNewPlant: Double;
begin
  try
    if ((NumPhase = 1) and (ChangePhase = 1)) then
    begin
      CulmsPerHill := PlantsPerHill;
    end
    else
    begin
      if ((NumPhase = 2) and (ChangePhase = 1)) then
      begin
        CulmsPerPlant := 1;
        CulmsPop := CulmsPerPlant * Density * PlantsPerHill;
      end
      else
      begin
        if ((NumPhase > 1) and (NumPhase < 4) and (HaunIndex >
          HaunCritTillering)) then
        begin
          TilNewPlant := cstr * Min(Max(0, (Ic - IcTillering) * TilAbility) *
            Sqrt(LtrKdfcl), 2);
          CulmsPerPlant := CulmsPerPlant + TilNewPlant;
          CulmsPerHill := CulmsPerPlant * PlantsPerHill;
          CulmsPop := CulmsPerHill * Density;
        end
        else
        begin
          CulmsPerPlant := CulmsPerPlant;
          CulmsPop := CulmsPop;
          CulmsPerHill := CulmsPerHill;
        end;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvolPlantTilNumTot_V2', URisocas);
  end;
end;

procedure RS_EvolPlantLeafNumTot(const NumPhase, CulmsPerHill, HaunGain: Double;
  var PlantLeafNumNew, PlantLeafNumTot: Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      PlantLeafNumNew := HaunGain * CulmsPerHill;
      PlantLeafNumTot := PlantLeafNumTot + PlantLeafNumNew;
    end
    else
    begin
      PlantLeafNumNew := PlantLeafNumNew;
      PlantLeafNumTot := PlantLeafNumTot;
    end;
  except
    AfficheMessageErreur('RS_EvolPlantLeafNumTot', URisocas);
  end;
end;

procedure RS_EvolMobiliTillerDeath(const NumPhase, SDJPhase4, SeuilTempRPR,
  CoeffTillerDeath, Density, Ic, PlantsPerHill: Double;
  var TillerDeathPop, CulmsPop, CulmsPerPlant, CulmsPerHill: Double);
begin
  try
    if ((NumPhase = 3) or ((NumPhase = 4) and (SDJPhase4 <= 0.67 * SeuilTempRPR))
      and (CulmsPerPlant >= 1)) then
    begin
      TillerDeathPop := (1 - (Min(Ic, 1))) * CulmsPop * CoeffTillerDeath;
      CulmsPop := CulmsPop - TillerDeathPop;
      CulmsPerPlant := CulmsPop / (Density * PlantsPerHill);
      CulmsPerHill := CulmsPerPlant * PlantsPerHill;
    end;
  except
    AfficheMessageErreur('RS_EvolMobiliTillerDeath', URisocas);
  end;
end;

procedure RS_EvolMobiliTillerDeath_V2(const NumPhase, SDJPhase4, SeuilTempRPR,
  CoeffTillerDeath, Density, Ic, PlantsPerHill: Double;
  var TillerDeathPop, CulmsPop, CulmsPerPlant, CulmsPerHill,
  DryMatStructPaniclePop: Double);
begin
  try
    if ((NumPhase = 3) or ((NumPhase = 4) and (SDJPhase4 <= {NEW} 0.7 * SeuilTempRPR))
      and (CulmsPerPlant >= 1)) then
    begin
      TillerDeathPop := (1 - (Min(Ic, 1))) * CulmsPop * CoeffTillerDeath;
      CulmsPop := CulmsPop - TillerDeathPop;
      CulmsPerPlant := CulmsPop / (Density * PlantsPerHill);
      CulmsPerHill := CulmsPerPlant * PlantsPerHill;
      DryMatStructPaniclePop := DryMatStructPaniclePop * Max(0, CulmsPop) /
        (CulmsPop + TillerDeathPop);
    end;
  except
    AfficheMessageErreur('RS_EvolMobiliTillerDeath_V2', URisocas);
  end;
end;

procedure RS_EvolMobiliLeafDeath(const NumPhase, Ic, CoeffLeafDeath, sla:
  Double;
  var LeafDeathPop, DryMatStructLeafPop,
  MobiliLeafDeath, DeadLeafDrywtPop, LaiDead: Double);
begin
  try
    if (NumPhase > 1) then
    begin
      LeafDeathPop := (1 - (Min(Ic, 1))) * DryMatStructLeafPop * CoeffLeafDeath;
      DryMatStructLeafPop := DryMatStructLeafPop - LeafDeathPop;
      MobiliLeafDeath := 0.25 {NEW} * LeafDeathPop; 
      DeadLeafDrywtPop := DeadLeafDrywtPop + (0.75 {NEW} * LeafDeathPop);
      LaiDead := DeadLeafDrywtPop * sla;
    end;
  except
    AfficheMessageErreur('RS_EvolMobiliLeafDeath', URisocas);
  end;
end;

procedure RS_EvalSupplyTot(const NumPhase, PhaseStemElongation, Assim,
  MobiliLeafDeath, RespMaintTot: Double;
  var RespMaintDebt, AssimNotUsed, AssimNotUsedCum,
  AssimSurplus, SupplyTot , CumSupplyTot: Double);
begin
  try 
    SupplyTot := Assim + MobiliLeafDeath - RespMaintTot - Max(0, RespMaintDebt);
	{NEW}
	
	if NumPhase < 7 then
		CumSupplyTot := CumSupplyTot + SupplyTot {NEW R} - MobiliLeafDeath //  Output Test variable for source for dry matter production (consider also AssimNotUsed!)
	else
		CumSupplyTot := 0;
		
    if (SupplyTot <= 0) then
    begin
      RespMaintDebt := 0 - SupplyTot;
      SupplyTot := 0;
    end
    else
    begin
      RespMaintDebt := 0;
    end;
	
	{DELETED}
    {
	if ((NumPhase < 5) and (PhaseStemElongation = 0)) then
    begin
      AssimNotUsed := AssimSurplus;
      AssimNotUsedCum := AssimNotUsedCum + AssimNotUsed;
    end
    else
    begin
      AssimNotUsed := 0;
      AssimNotUsedCum := AssimNotUsedCum + AssimNotUsed;
    end;
	}
	// These commands seem redundant and in the wrong place. Denete?
	
	
  except
    AfficheMessageErreur('RS_EvalSupplyTot', URisocas);
  end;
end;

procedure RS_EvalRelPotLeafLength(const NumPhase, HaunIndex, RankLongestLeaf:
  Double;
  var RelPotLeafLength: Double);
begin
  try
    if (NumPhase > 1) then
    begin
      RelPotLeafLength := Min((0.1 + 0.9 * HaunIndex / RankLongestLeaf), 1);
    end;
  except
    AfficheMessageErreur('RS_EvalRelPotLeafLength', URisocas);
  end;
end;

procedure RS_EvalDemandStructLeaf(const NumPhase, PlantLeafNumNew, sla, SlaMax,
  RelPotLeafLength, Density, LeafLengthMax, CoeffLeafWLRatio, cstr: Double;
  var DemLeafAreaPlant, DemStructLeafPlant, DemStructLeafPop: Double);
var
  CorrectedSla: Double;
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      DemLeafAreaPlant := (Power((RelPotLeafLength * LeafLengthMax), 2) *
        CoeffLeafWLRatio * 0.725 * PlantLeafNumNew / 1000000) * cstr;
      if (sla = 0) then
      begin
        CorrectedSla := SlaMax;
      end
      else
      begin
        CorrectedSla := sla;
      end;
      DemStructLeafPlant := DemLeafAreaPlant * 0.1 / CorrectedSla;
      DemStructLeafPop := DemStructLeafPlant * Density / 1000;
    end;
  except
    AfficheMessageErreur('RS_EvalDemandStructLeaf', URisocas);
  end;
end;

procedure RS_EvalDemandStructLeaf_V2(const NumPhase, PlantLeafNumNew, SlaNew,
  SlaMax,
  RelPotLeafLength, Density, LeafLengthMax, CoeffLeafWLRatio, cstr, StressCold:
    Double;
  var DemLeafAreaPlant, DemStructLeafPlant, DemStructLeafPop, A_DemStructLeaf: Double);
var
  CorrectedSla: Double; 
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      DemLeafAreaPlant := (Power((RelPotLeafLength * LeafLengthMax), 2) *
        CoeffLeafWLRatio * 0.725 * PlantLeafNumNew / 1000000) * Min(cstr,
        StressCold);
      if (SlaNew = 0) then
      begin
        CorrectedSla := SlaMax;
      end
      else
      begin
        CorrectedSla := SlaNew;
      end;
      DemStructLeafPlant := DemLeafAreaPlant * 0.1 / CorrectedSla;
      DemStructLeafPop := DemStructLeafPlant * Density / 1000; 
	  A_DemStructLeaf := DemStructLeafPlant * Density / 1000; 
    end;
  except
    AfficheMessageErreur('RS_EvalDemandStructLeaf_V2', URisocas);
  end;
end;

procedure RS_EvalDemandStructSheath(const NumPhase, DemStructLeafPop,
  WtRatioLeafSheath, SlaMin, SlaMax, Sla,
  StressCold: Double;
  var DemStructSheathPop {TEST}{, A_DemStructSheath}: Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      DemStructSheathPop := (1 + ((SlaMax - Sla) / (SlaMax - SlaMin))) * 0.5 *
        DemStructLeafPop / WtRatioLeafSheath * Max(0.00001, StressCold);
	  
	  //A_DemStructSheath := DemStructSheathPop;
    end;
  except
    AfficheMessageErreur('RS_EvalDemandStructSheath', URisocas); 
  end;
end;

procedure RS_EvalDemandStructRoot(const NumPhase, Density: Double;
  CoeffRootMassPerVolMax,
  RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot: Double;
  var RootSystSoilSurfPop, RootSoilSurfPop, RootSystVolPop, GainRootSystVolPop,
  GainRootSoilSurfPop, DemStructRootPop, RootSystSoilSurfPopOld,
  RootFrontOld, DemStructRootPlant: Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      RootSystSoilSurfPop := Min(RootFront * RootFront * Density / 1000000,
        10000);
      RootSystVolPop := RootSystSoilSurfPop * RootFront / 1000;
      GainRootSoilSurfPop := RootSystSoilSurfPop - RootSystSoilSurfPopOld;
      GainRootSystVolPop := (RootSystSoilSurfPop + GainRootSoilSurfPop) *
        (RootFront - RootFrontOld);
      DemStructRootPop := Min(CoeffRootMassPerVolMax * GainRootSystVolPop,
        SupplyTot * RootPartitMax);
      DemStructRootPlant := DemStructRootPop * 1000 / density;
      RootSystSoilSurfPopOld := RootSystSoilSurfPop;
      RootFrontOld := RootFront;
    end;
  except
    AfficheMessageErreur('RS_EvalDemandStructRoot', URisocas);
  end;
end;

procedure RS_EvalDemandStructRoot_V2(const NumPhase, Density: Double;
  CoeffRootMassPerVolMax,
  RootPartitMax, GrowthStructTotPop, RootFront, SupplyTot,
  DemStructLeafPop, DemStructSheathPop, DryMatStructRootPop: Double;
  var RootSystSoilSurfPop, RootSystVolPop, GainRootSystVolPop,
  GainRootSystSoilSurfPop, DemStructRootPop, RootSystSoilSurfPopOld,
  RootFrontOld, RootSystVolPopOld, DemStructRootPlant{TEST}{, GrowthView}: Double);
begin
  try
    RootSystSoilSurfPop := Min(RootFront * RootFront * Density / 1000000,
      10000);
    RootSystVolPop := RootSystSoilSurfPop * RootFront / 1000;
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      GainRootSystSoilSurfPop := RootSystSoilSurfPop - RootSystSoilSurfPopOld;
      GainRootSystVolPop := RootSystVolPop - RootSystVolPopOld;
      DemStructRootPop := Min((DemStructLeafPop + DemStructSheathPop) *
        RootPartitMax, Max(0, CoeffRootMassPerVolMax * RootSystVolPop -
        DryMatStructRootPop));
	 
      //GrowthView := DemStructRootPop;
		
      DemStructRootPlant := DemStructRootPop * 1000 / density;
      RootSystSoilSurfPopOld := RootSystSoilSurfPop;
      RootFrontOld := RootFront;
      RootSystVolPopOld := RootSystVolPop;
    end;
  except
    AfficheMessageErreur('RS_EvalDemandStructRoot_V2', URisocas);
  end;
end;

procedure RS_EvalDemandStructInternode(const PhaseElongation, ApexHeightGain,
  CulmsPerHill,
  CoeffInternodeMass, Density, Ic: Double;
  var DemStructInternodePlant, DemStructInternodePop: Double);
begin
  try
    if (PhaseElongation = 1) then
    begin
      DemStructInternodePlant := Power(Min(Ic, 1), 0.5) * ApexHeightGain *
        CulmsPerHill * CoeffInternodeMass;
      DemStructInternodePop := DemStructInternodePlant * Density / 1000;
    end;
  except
    AfficheMessageErreur('RS_EvalDemandStructInternode', URisocas);
  end;
end;

procedure RS_EvalDemandStructIN_V2(const PhaseElongation, ApexHeightGain,
  CulmsPerHill,
  CoeffInternodeMass, Density, Ic , ResCapacityInternodePop , DryMatResInternodePop,
  {NEW G} CoeffReserveSink , {NEW LB} NumPhase : Double;
  var DemStructInternodePlant, DemStructInternodePop , {NEW G}DemResInternodePop: Double);
begin
  try
    if (PhaseElongation = 1) then
    begin
      DemStructInternodePlant := Power(Min(Ic, 1), 0.5) * ApexHeightGain *
        CulmsPerHill * CoeffInternodeMass;
      DemStructInternodePop := DemStructInternodePlant * Density / 1000;
    end;
	
	{NEW LB}
	if (NumPhase > 1) and (NumPhase < 5) then
		 {NEW G}
	 	  DemResInternodePop := (ResCapacityInternodePop - DryMatResInternodePop) * CoeffReserveSink;
		  // CoeffReserveSink is a crop para 0…1 that sets daily reserve sink as fraction of deficit
		 {/NEW G}
	{/NEW LB}
	
  except
    AfficheMessageErreur('RS_EvalDemandStructIN_V2', URisocas);
  end;
end;

procedure RS_EvalDemandStructPanicle(const NumPhase, CoeffPanicleMass,
  CulmsPerHill,
  Ic, DryMatStructPaniclePop, Density,
  PanStructMassMax: Double;
  var DemStructPaniclePlant, PanStructMass,
  DemStructPaniclePop: Double);
begin
  try
    if (NumPhase = 4) then
    begin
      DemStructPaniclePlant := CoeffPanicleMass * CulmsPerHill * Ic;
      PanStructMass := 1000 * DryMatStructPaniclePop / (Density * CulmsPerHill);
      if (PanStructMass > PanStructMassMax) then
      begin
        DemStructPaniclePlant := 0;
      end;
      DemStructPaniclePop := DemStructPaniclePlant * Density / 1000;
    end;
  except
    AfficheMessageErreur('RS_EvalDemandStructPanicle', URisocas);
  end;
end;

procedure RS_EvalDemandStructPanicle_V2(const NumPhase, CoeffPanicleMass,
  CulmsPerHill,
  Ic, DryMatStructPaniclePop, Density,
  PanStructMassMax, StressCold: Double;
  var DemStructPaniclePlant, PanStructMass,
  DemStructPaniclePop: Double);
begin
  try
    if (NumPhase = 4) then
    begin
      DemStructPaniclePlant := CoeffPanicleMass * CulmsPerHill * Sqrt(Min(Ic, 1))
        * Sqrt(Max(0.00001, StressCold));
      PanStructMass := 1000 * DryMatStructPaniclePop / (Density * CulmsPerHill);
      if (PanStructMass > PanStructMassMax) then
      begin
        DemStructPaniclePlant := 0;
      end;
      DemStructPaniclePop := DemStructPaniclePlant * Density / 1000;
    end;
  except
    AfficheMessageErreur('RS_EvalDemandStructPanicle_V2', URisocas);
  end;
end;

procedure RS_EvalDemandTotAndIcPreFlow(const NumPhase, RespMaintTot,
  DemStructLeafPop,
  DemStructSheathPop, DemStructRootPop,
  DemStructInternodePop, DemStructPaniclePop,
  SupplyTot, NbDaysSinceGermination, PlantHeight, Cstr, DemResInternodePop: Double;
  var DemStructTotPop, Ic, IcCumul, IcMean, CstrCumul, CstrMean , A_DemStructTot: Double);

begin
try

    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin

        DemStructTotPop := DemStructLeafPop + DemStructSheathPop +
							DemStructRootPop + DemStructInternodePop +
							DemStructPaniclePop {NEW G} + DemResInternodePop;
		
		A_DemStructTot := DemStructLeafPop + DemStructSheathPop +
							DemStructRootPop + DemStructInternodePop +
							DemStructPaniclePop {NEW G} + DemResInternodePop;
		
		
      Ic := SupplyTot / DemStructTotPop;


      if (Ic <= 0) then
      begin
        Ic := 0;
      end;


      if (PlantHeight = 0) then
      begin
        Ic := 1;
      end;



      IcCumul := IcCumul + Min(Ic, 1);
      IcMean := IcCumul / NbDaysSinceGermination;
      CstrCumul := CstrCumul + Cstr;
      CstrMean := CstrCumul / NbDaysSinceGermination;


    end;
	
	
    if ((NumPhase = 5) or (NumPhase = 6)) then
    begin
      IcCumul := IcCumul + Min(Ic, 1);
      IcMean := IcCumul / NbDaysSinceGermination;
      CstrCumul := CstrCumul + Cstr;
      CstrMean := CstrCumul / NbDaysSinceGermination;
    end;


  except
    AfficheMessageErreur('RS_EvalDemandTotAndIcPreFlow', URisocas);
  end;
end;

procedure RS_EvolGrowthStructLeafPop(const NumPhase, Ic, SupplyTot,
  DemStructLeafPop, DemStructTotPop: Double;
  var GrowthStructLeafPop {, GrowthView}, A_GrowthStructLeaf : Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      if (Ic < 1) then
		  begin
		  
			{YIELD ERROR 34.5}
			GrowthStructLeafPop := SupplyTot * (DemStructLeafPop / DemStructTotPop);
			A_GrowthStructLeaf := SupplyTot * (DemStructLeafPop / DemStructTotPop);
			
		  end
      else
		  begin
			
			GrowthStructLeafPop := DemStructLeafPop;
			A_GrowthStructLeaf := DemStructLeafPop;
			//showMessage('DEM:_'+ floattostr(DemStructLeafPop)+' A_G: _' +floattostr(A_GrowthStructLeaf));
		  end;
    end;
	
	
	{GrowthView := GrowthStructLeafPop;}
  except
    AfficheMessageErreur('RS_EvolGrowthStructLeafPop', URisocas);
  end;
end;

procedure RS_EvolGrowthStructSheathPop(const NumPhase, Ic, SupplyTot,
  DemStructSheathPop, DemStructTotPop: Double;
  var GrowthStructSheathPop: Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      if (Ic < 1) then
      begin
        GrowthStructSheathPop := SupplyTot * (DemStructSheathPop /
          DemStructTotPop);
      end
      else
      begin
        GrowthStructSheathPop := DemStructSheathPop;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvolGrowthStructSheathPop', URisocas);
  end;
end;

procedure RS_EvolGrowthStructRootPop(const NumPhase, Ic, SupplyTot,
  DemStructRootPop, DemStructTotPop: Double;
  var GrowthStructRootPop: Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      if (Ic < 1) then
      begin
        GrowthStructRootPop := SupplyTot * (DemStructRootPop / DemStructTotPop);
      end
      else
      begin
        GrowthStructRootPop := DemStructRootPop;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvolGrowthStructRootPop', URisocas);
  end;
end;

procedure RS_EvolGrowthStructINPop(const NumPhase, Ic, SupplyTot,
  DemStructInternodePop, DemStructTotPop , DemResInternodePop: Double;
  var GrowthStructInternodePop, {NEW G} GrowthResInternodePop: Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      if (Ic < 1) then
      begin
        GrowthStructInternodePop := SupplyTot * (DemStructInternodePop / DemStructTotPop);
		{NEW G}
		GrowthResInternodePop := SupplyTot * (DemResInternodePop / DemStructTotPop);
		{/NEW G}
	  end
      else
      begin
        GrowthStructInternodePop := DemStructInternodePop;
		
		{NEW G}
		GrowthResInternodePop := DemResInternodePop;
		{/NEW G}
	  
	  end;
    end;
  except
    AfficheMessageErreur('RS_EvolGrowthStructInternodePop', URisocas);
  end;
end;

procedure RS_EvolGrowthStructPanPop(const NumPhase, Ic, SupplyTot,
  DemStructPaniclePop, DemStructTotPop: Double;
  var GrowthStructPaniclePop: Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      if (Ic < 1) then
      begin
        GrowthStructPaniclePop := SupplyTot * (DemStructPaniclePop /
          DemStructTotPop);
      end
      else
      begin
        GrowthStructPaniclePop := DemStructPaniclePop;
      end;
    end; 
  except
    AfficheMessageErreur('RS_EvolGrowthStructPaniclePop', URisocas);
  end;
end;

procedure RS_AddResToGrowthStructPop(const NumPhase, Ic, PhaseStemElongation,
  DryMatResInternodePop, DemStructTotPop, DemStructLeafPop, DemStructSheathPop,
  DemStructRootPop, DemStructInternodePop, DemStructPaniclePop,
  RelMobiliInternodeMax, GrowthResInternodePop: Double;
  var ResInternodeMobiliDayPot, GrowthStructDeficit, GrowthStructLeafPop,
  GrowthStructSheathPop, GrowthStructRootPop, GrowthStructInternodePop,
  GrowthStructPaniclePop, GrowthStructTotPop, ResInternodeMobiliDay , A_GrowthStructLeaf , A_GrowthStructTot, A_ResInternodeMobiliDay : Double);
begin
  try
    if (NumPhase > 1) then
    begin
      //if (PhaseStemElongation = 1) then
      {DELETED may 06}{if (NumPhase > 2) then
      begin 
        ResInternodeMobiliDayPot := RelMobiliInternodeMax * DryMatResInternodePop;
        GrowthStructDeficit := Max((DemStructTotPop - GrowthStructTotPop }{NEW LB}{ - GrowthResInternodePop), 0);}
      {DELETED may 06} {end;}
      if ((Ic < 1) and (DemStructTotPop > 0)) then
      begin
        ResInternodeMobiliDay := Min(ResInternodeMobiliDayPot,
          GrowthStructDeficit);
		  
		A_ResInternodeMobiliDay := Min(ResInternodeMobiliDayPot,
          GrowthStructDeficit);
		{DELETED}
		{
        GrowthStructTotPop := GrowthStructLeafPop + GrowthStructSheathPop
          + GrowthStructRootPop + GrowthStructInternodePop +
            GrowthStructPaniclePop + GrowthResInternodePop;
		}
		{/DELETED}	
        GrowthStructLeafPop := GrowthStructLeafPop + ResInternodeMobiliDay *
          (DemStructLeafPop / DemStructTotPop);
		  
		A_GrowthStructLeaf := GrowthStructLeafPop;
		  
        GrowthStructSheathPop := GrowthStructSheathPop + ResInternodeMobiliDay *
          (DemStructSheathPop / DemStructTotPop);
        GrowthStructRootPop := GrowthStructRootPop + ResInternodeMobiliDay *
          (DemStructRootPop / DemStructTotPop);
        GrowthStructInternodePop := GrowthStructInternodePop +
          ResInternodeMobiliDay * (DemStructInternodePop / DemStructTotPop);
        GrowthStructPaniclePop := GrowthStructPaniclePop + ResInternodeMobiliDay
          * (DemStructPaniclePop / DemStructTotPop);
		
		// The following is an update on total growth including mobilization from reserves. Storage does not benefit from mobilization so GrowthResInternodePop is unaltered since module 65, but is included in total growth  
        GrowthStructTotPop := GrowthStructLeafPop + GrowthStructSheathPop
          + GrowthStructRootPop + GrowthStructInternodePop +
            GrowthStructPaniclePop {NEW P} + GrowthResInternodePop;
		
		A_GrowthStructTot := GrowthStructTotPop;
		
      end;
    end;
  except
    AfficheMessageErreur('RS_AddResToGrowthStructPop' +
      ' GrowthStrucTotPop : ' + floattostr(GrowthStructTotPop), URisocas);
  end;
end;

procedure RS_EvolDemPanFilPopAndIcPFlow(const NumPhase, DryMatStructPaniclePop,
  CoeffPanSinkPop,
  SterilityTot, DegresDuJourCor, DegresNumPhase5,
  SupplyTot, Assim, RespMaintTot, StressCold: Double;
  var PanicleSinkPop, DemPanicleFillPop,
  AssimSurplus , Ic , A_AssimSurplus: Double);
begin
  try
    if (NumPhase = 5) then
    begin
      PanicleSinkPop := DryMatStructPaniclePop * CoeffPanSinkPop * (1 -
        SterilityTot);
      DemPanicleFillPop := (DegresDuJourCor / DegresNumPhase5) * PanicleSinkPop
        * Sqrt(Max(0.00001, StressCold));
      Ic := SupplyTot / Max(DemPanicleFillPop, 0.0000001);
      if (Ic <= 0) then
      begin
        Ic := 0;
      end;
    end;
    if (NumPhase = 6) then
    begin
      Ic := Assim / RespMaintTot;
      if (Ic >= 1) then
      begin
        AssimSurplus := Max(0, Assim - RespMaintTot);
		A_AssimSurplus := Max(0, Assim - RespMaintTot);
      end
      else
      begin
        AssimSurplus := 0;
		A_AssimSurplus := 0;
      end;
      if (Ic < 0) then
      begin
        Ic := 0;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvolDemPanFilPopAndIcPFlow', URisocas);
  end;
end;

procedure RS_EvolPanicleFilPop(const NumPhase, Ic, DryMatResInternodePop,
  DemPanicleFilPop,
  SupplyTot, RelMobiliInternodeMax, RespMaintTot, Assim: Double;
  var ResInternodeMobiliDayPot, AssimSurplus,
  PanicleFilDeficit, ResInternodeMobiliDay, PanicleFilPop,
  GrainYieldPop, A_AssimSurplus , A_ResInternodeMobiliDay: Double);
begin
  try
    if (NumPhase = 5) then
    begin
      ResInternodeMobiliDayPot := RelMobiliInternodeMax * DryMatResInternodePop;
      if (Ic > 1) then
      begin
        PanicleFilPop := Max(DemPanicleFilPop, 0);
        PanicleFilDeficit := 0;
        AssimSurplus := SupplyTot - PanicleFilPop;
		A_AssimSurplus := SupplyTot - PanicleFilPop;
      end
      else
      begin
        if (Ic <= 1) then
        begin
          PanicleFilDeficit := Max((DemPanicleFilPop - Max(SupplyTot, 0)), 0);
          ResInternodeMobiliDay := Max(Min(ResInternodeMobiliDayPot, 0.5 *
            PanicleFilDeficit), 0);
		  A_ResInternodeMobiliDay 	:= Max(Min(ResInternodeMobiliDayPot, 0.5 *
            PanicleFilDeficit), 0);
			
          PanicleFilPop := Max((SupplyTot + ResInternodeMobiliDay), 0);
          AssimSurplus := 0;
		  A_AssimSurplus := 0;
        end;
      end;
      GrainYieldPop := GrainYieldPop + PanicleFilPop;
    end
    else
    begin
      if (NumPhase = 6) then
      begin
        AssimSurplus := Assim - RespMaintTot;
		A_AssimSurplus := Assim - RespMaintTot;
        ResInternodeMobiliDay := Min(Max(0, RespMaintTot - Assim),
          DryMatResInternodePop);
		A_ResInternodeMobiliDay := Min(Max(0, RespMaintTot - Assim),
          DryMatResInternodePop);  
		  
      end
      else
      begin
        if (NumPhase > 6) then
        begin
          ResInternodeMobiliDay := 0;
		  A_ResInternodeMobiliDay := 0;
        end;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvolPanicleFilPop', URisocas);
  end;
end;

procedure RS_EvolGrowthReserveInternode(const NumPhase, PhaseStemElongation,
  DryMatStructInternodePop, DryMatStructSheathPop, CoeffResCapacityInternode,
  AssimSurplus, ResInternodeMobiliDay: Double;
  var ResCapacityInternodePop, IncreaseResInternodePop,
  DryMatResInternodePop, AssimNotUsed, AssimNotUsedCum, GrowthResInternodePop, {NEW LB} DryMatResInternodePopOld , A_IncreaseResInternodePop:
    Double);
begin
  try
    //if ((PhaseStemElongation = 1) or (NumPhase >= 5)) then
    if (NumPhase >= 2) then
    begin
	
	  {NEW LB}
	  DryMatResInternodePopOld := DryMatResInternodePop; // Needed to calculate reservesaccumulation for the day which happens in 2 steps
	  {/NEW LB}
    
	  ResCapacityInternodePop := (DryMatStructInternodePop + DryMatStructSheathPop) *
        CoeffResCapacityInternode;
		
	  //growthView := GrowthResInternodePop;
	  {NEW G}
	  DryMatResInternodePop := DryMatResInternodePop + GrowthResInternodePop;
	  // Demand-driven growth of reserve pool
	  {/NEW G}
	  
      IncreaseResInternodePop := Min(Max(AssimSurplus, 0),
        Max((ResCapacityInternodePop - DryMatResInternodePop), 0));
      
	  A_IncreaseResInternodePop := Min(Max(AssimSurplus, 0),
        Max((ResCapacityInternodePop - DryMatResInternodePop), 0));
	  
	  GrowthResInternodePop := IncreaseResInternodePop - ResInternodeMobiliDay;
	  
	  //showmessage(floatToStr(DryMatResInternodePop)+' '+floatToStr(IncreaseResInternodePop)+' '+floatToStr(GrowthResInternodePop));
	  
	  
      DryMatResInternodePop := DryMatResInternodePop + GrowthResInternodePop;
      // Surplus- and mobilization-driven growth of reserve pool
	  //growthView := GrowthResInternodePop;
	  
	  AssimNotUsed := Max((AssimSurplus - IncreaseResInternodePop), 0);
      AssimNotUsedCum := AssimNotUsedCum + AssimNotUsed;
    end;
  except
    AfficheMessageErreur('RS_EvolGrowthReserveInternode', URisocas);
  end;
end;

procedure RS_EvolGrowthTot(const NumPhase, GrowthStructLeafPop,
  GrowthStructSheathPop,
  GrowthStructRootPop, GrowthStructInternodePop, GrowthStructPaniclePop,
  GrowthResInternodePop, PanicleFilPop , DryMatResInternodePop , DryMatResInternodePopOld: Double;
  var GrowthStructTotPop, GrowthDryMatPop , A_GrowthStructTot : Double);
begin
  try
    if (NumPhase < 5) then
    begin
      GrowthStructTotPop := GrowthStructLeafPop + GrowthStructSheathPop +
        GrowthStructRootPop + GrowthStructInternodePop +
        GrowthStructPaniclePop;
	 
	  A_GrowthStructTot := GrowthStructTotPop;
	  
    end
    else
    begin
      GrowthStructTotPop := 0;
	  
	  A_GrowthStructTot := GrowthStructTotPop;
	  
    end;
    GrowthDryMatPop := GrowthStructTotPop + {NEW LB} (DryMatResInternodePop - DryMatResInternodePopOld) {/NEW LB}{DELETED} {GrowthResInternodePop} +
      PanicleFilPop;
  except
    AfficheMessageErreur('RS_EvolGrowthTot', URisocas);
  end;
end;

procedure RS_EvalLai(const NumPhase, ChangePhase, DryMatStructLeafPop, sla,
  SlaMax, {NEW P} LeafLengthMax, RelPotLeafLength, GrowthStructTotPop, GrowthStructLeafPop {/NEW P} , DemStructLeafPop: Double;
  var Lai , {NEW P} LastLeafLengthPot , LastLeafLength {/NEW P}: Double);
var
  CorrectedSla: Double;
begin
  try
  
	//showMessage(floattostr(NumPhase));
    if ((NumPhase = 2) and (ChangePhase = 1)) then
    begin
      CorrectedSla := SlaMax;
    end
    else
    begin
      CorrectedSla := sla;
	  
	  {NEW}
	  LastLeafLengthPot := RelPotLeafLength * LeafLengthMax;
	  
	  if GrowthStructTotPop > 0 then
		LastLeafLength := LastLeafLengthPot * sqrt(GrowthStructLeafPop / DemStructLeafPop);
	  {/NEW}
	  
    end;
    Lai := DryMatStructLeafPop * CorrectedSla;
  except
    AfficheMessageErreur('RS_EvalLai', URisocas);
  end;
end;

procedure RS_EvalClumpAndLightInter(const NumPhase, KRolling, Density,
  PlantWidth,
  PlantHeight, Kdf, Lai, CoeffClump: Double;
  var Kcl, Kdfcl, LIRkdf, LIRkdfcl, LTRkdf,
  LTRkdfcl: Double);
var
  PlantDistance: Double;
  PlantLeafAreaGreen: Double;
  ClumpDens: Double;
  ClumpIsol: Double;
  CorrectedPlantWidth: Double;
  CorrectedPlantHeight: Double;
  RolledLai: Double;
begin
  try
    if (NumPhase > 1) then
    begin
      RolledLai := Lai * KRolling;
      PlantDistance := Sqrt(10000 / Density);
      PlantLeafAreaGreen := Power(PlantDistance, 2) * RolledLai;
      if (PlantWidth = 0) then
      begin
        CorrectedPlantWidth := 1;
      end
      else
      begin
        CorrectedPlantWidth := PlantWidth;
      end;
      if (PlantHeight = 0) then
      begin
        CorrectedPlantHeight := 1;
      end
      else
      begin
        CorrectedPlantHeight := PlantHeight;
      end;
      ClumpDens := Sqrt(Max(PlantLeafAreaGreen / (Power(0.5 * CorrectedPlantWidth
        / 2000, 2) * PI) - 1, 0));
      ClumpIsol := Sqrt(Max(Power(PlantDistance, 2) / (Power(0.5 *
        CorrectedPlantWidth / 2000, 2) * PI) - 1, 0)) / (CorrectedPlantHeight /
        1000);
      Kcl := ClumpDens * ClumpIsol;
      Kdfcl := Kdf - Min(CoeffClump * Kcl, 0.4 * Kdf);
      LIRkdf := 1 - Exp(-Kdf * RolledLai);
      LIRkdfcl := 1 - Exp(-Kdfcl * RolledLai);
      LTRkdf := Exp(-Kdf * RolledLai);
      LTRkdfcl := Exp(-Kdfcl * RolledLai);
    end;
  except
    AfficheMessageErreur('RS_EvalClumpingAndLightInter', URisocas);
  end;
end;

procedure RS_EvalClumpAndLightInter_V2(const NumPhase, KRolling, Density,
  PlantWidth, PlantHeight, Kdf, Lai,
  FractionPlantHeightSubmer: Double;
  var LIRkdf, LIRkdfcl, LTRkdf, LTRkdfcl: Double);
var
  RolledLai: Double;
begin
  try
    if (NumPhase > 1) and (PlantWidth > 0) then
    begin
      RolledLai := Lai * KRolling * {MODIFIED JUNE 20}{Sqrt}power((1 - FractionPlantHeightSubmer), 0.3);
      LIRkdf := 1 - Exp(-Kdf * RolledLai);
      LIRkdfcl := (1 - Exp(-Kdf * RolledLai * 10000 / Min(10000, Density * pi *
        Power(PlantWidth / 2000, 2)))) * (Min(10000, Density * pi *
        Power(PlantWidth / 2000, 2)) / 10000); 
      LTRkdf := 1 - LIRkdf;
      LTRkdfcl := 1 - LIRkdfcl;
    end;
  except
    AfficheMessageErreur('RS_EvalClumpingAndLightInter_V2', URisocas);
  end;
end;

procedure RS_EvalSlaMitch(const SlaMax, SlaMin, AttenMitch, SDJ, SDJLevee,
  NumPhase,
  DegresDuJour, TOpt1, TBase, TempSla, DryMatStructLeafPop,
  GrowthStructLeafPop: Double;
  var SlaMitch, SlaNew, Sla: Double);
begin
  try
    if (NumPhase > 1) then
    begin
      SlaMitch := SlaMin + (SlaMax - SlaMin) * Power(AttenMitch, (SDJ -
        SDJLevee));
      SlaNew := SlaMin + (SlaMitch - SlaMin) * Power(DegresDuJour / (TOpt1 -
        TBase), TempSla);
      Sla := ((Sla * DryMatStructLeafPop) + (SlaNew * GrowthStructLeafPop)) /
        (DryMatStructLeafPop + GrowthStructLeafPop);
    end
    else
    begin
      SlaMitch := 0;
      SlaNew := 0;
      Sla := SlaMax;
    end;
  except
    AfficheMessageErreur('RS_EvalSlaMitch', URisocas);
  end;
end;

procedure RS_EvalRUE(const NumPhase, ChangePhase, ParIntercepte, DryMatTotPop,
  DeadLeafDrywtPop , DryMatStructRootPop, Tr, Evap, Dr, Lr, SupplyTot, AssimNotUsed,
  Irrigation, IrrigAutoDay, Pluie, Assim, AssimPot,
  Conversion, NbJas , Transplanting , NurseryStatus, 
  //NEW
  Density , DensityNursery, DryMatAboveGroundTotPop : Double;
  var RUE, CumPar, CumTr, CumEt, CumWUse, CumWReceived,
  CumIrrig, CumDr, CumLr, TrEffInst, TrEff, WueEt,
  WueTot, ConversionEff{, growthView}: Double);
var
  CorrectedIrrigation: Double;
begin
  try
    if ((NumPhase < 1) or ((NumPhase = 1) and (ChangePhase = 1))) {NEW G}or (Density = DensityNursery){/NEW G} then
		begin
		  CumPar := 0;
		  RUE := 0;
		  CumTr := 0.00001;
		  CumEt := 0.00001;
		  CumWUse := 0.00001;
		  CumWReceived := 0;
		  CumIrrig := 0;
		  CumDr := 0;
		  CumLr := 0;
		end
		
    else
	
    begin
	  
	  {NEW Y}
      If (Transplanting = 0) or (NurseryStatus = 1) then
      begin
       CumPar := CumPar + ParIntercepte;
       CumTr := CumTr + Tr;
       CumEt := CumEt + Tr + Evap;
       CumWUse := CumWUse + Tr + Evap + Dr + Lr;
      end;
	  {/NEW Y}
	  
      if (Irrigation = NullValue) then
      begin
        CorrectedIrrigation := 0;
      end
      else
	  
      begin
        CorrectedIrrigation := Irrigation;
      end;
	
	  {NEW Y}
      If (Transplanting = 0) or (NurseryStatus = 1) then
      begin
       CumWReceived := CumWReceived + Pluie + CorrectedIrrigation + IrrigAutoDay;
       CumIrrig := CumIrrig + CorrectedIrrigation + IrrigAutoDay;
       CumDr := CumDr + Dr;
       CumLr := CumLr + Lr;
      end;
	  {/NEW Y}
	  
      if (AssimPot <> 0) then
      begin
        ConversionEff := Conversion * Assim / {NEW JUNE} (ParIntercepte * Conversion * 10){AssimPot};
      end;
      if ((Tr > 0) and (NbJas > {NEW G}20{/NEW G}) and (NumPhase > 1)) then
      begin
        TrEffInst := (SupplyTot - AssimNotUsed) / (Tr * 10000);
        TrEff := DryMatTotPop / (CumTr * 10000);
        WueEt := DryMatTotPop / (CumEt * 10000);
        WueTot := DryMatTotPop / (CumWuse * 10000);
		
		{DELETED G}
		//RUE := ((DryMatTotPop + DeadLeafDrywtPop - DryMatStructRootPop) / Max(CumPar, 0.00001)) / 10;
		//(Please delete this grey line and replace with the following one in green)
		{/DELETED G}
		
		{NEW G}
        RUE := ( DryMatAboveGroundTotPop / Max(CumPar, 0.00001)) / 10;
		{/NEW G}
		
	  end;
    end;
	
	//growthView := cumPar ;
	
  except
    AfficheMessageErreur('RS_EvalRUE', URisocas);
  end;
end;

procedure RS_EvolKcpKceBilhy(const LTRkdfcl, KcMax, Mulch: Double;
  var Kcp, Kce, KcTot: Double);
begin
  try
    Kcp := Min((1 - LTRkdfcl) * KcMax, KcMax);
    Kcp := Min(Kcp, KcMax);
    Kce := LTRkdfcl * 1 * (Mulch / 100);
    KcTot := Kcp + Kce;
  except
    AfficheMessageErreur('RS_BilhyEvolKcpLai', URisocas);
  end;
end;

procedure RS_EvolEvapSurfRFE_RDE(const Kce, EvapPot, CapaREvap, CapaRDE,
  StRurMax, RuSurf: Double;
  var Evap, ValRSurf, ValRFE, ValRDE, StRur, StRu, Kr, KceReal: Double);
var
  ValRSurfPrec, EvapRU: Double;
  Evap1, Evap2: Double;
begin
  try
    ValRSurfPrec := ValRSurf;
    // ValRSurf est l'eau contenue dans les réservoirs Revap (non transpirable) et RDE (transpirable et évaporable
    if (ValRFE > 0) then
    begin
      if (ValRFE < EvapPot) then
      begin
        Evap1 := ValRFE;
        Evap2 := Max(0, Min(ValRSurf, ((EvapPot - ValRFE) * ValRSurf) /
          (CapaREvap + CapaRDE))); // borné à 0 et ValRSurf le 27/04/05
      end
      else
      begin
        Evap1 := EvapPot;
        Evap2 := 0;
      end;
    end
    else
    begin
      Evap1 := 0;
      Evap2 := Max(0, Min(ValRSurf, EvapPot * ValRSurf / (CapaREvap + CapaRDE)));
        // borné à 0 et ValRSurf le 27/04/05
    end;
    Evap := Evap1 + Evap2;
    ValRFE := ValRFE - Evap1;
    ValRSurf := ValRSurf - Evap2;
    ValRDE := Max(0, ValRSurf - CapaREvap);
    if (EvapPot = 0) then
    begin
      Kr := 0;
    end
    else
    begin
      Kr := Evap / EvapPot;
    end;
    // part de l'évaporation prélevée dans les réservoirs RFE et RDE
    if (ValRSurf >= CapaREvap) then
    begin
      EvapRU := Evap;
    end
    else
    begin
      if (ValRSurfPrec <= CapaREvap) then
      begin
        EvapRU := Evap1;
      end
      else
      begin
        EvapRU := evap1 + ValRSurfPrec - CapaREvap;
      end;
    end;
    //Evaporation de Ru et Rur, MAJ
    if (StRurMax <= RuSurf) then
    begin
      // quand les racines n'ont pas dépassé la première couche
      StRur := Max(0, StRur - EvapRU * StRurMax / RuSurf);
    end
    else
    begin
      StRur := Max(0, StRur - EvapRU);
    end;
    StRu := StRu - EvapRU;
    // Ajout JCS 29/06/2009
    KceReal := Kce * Kr;
  except
    AfficheMessageErreur('RS_EvolEvapSurfRFE_RDE', URisocas);
  end;
end;

procedure RS_EvolEvapSurfRFE_RDE_V2(const NumPhase, Kce, EvapPot, CapaREvap,
  CapaRDE, CapaRFE, RuRac, RuSurf, {FloodwaterDepth,}
  BundHeight, EpaisseurSurf, EpaisseurProf, {StockMacropores,}
  RootFront, ResUtil: Double;
  var Evap, ValRSurf, ValRFE, ValRDE, StockRac, StockTotal, StockSurface,
  Kr, KceReal{NEW JUNE 19}, FloodwaterDepth, StockMacropores: Double);
var
  ValRSurfPrec, EvapRU: Double;
  Evap1, Evap2: Double;
begin
  try
  
	// Evaporation in absence of surface water
    if ((StockMacropores + FloodwaterDepth) = 0) {DELETED JUNE 19}{or (NumPhase = 0)} then
    begin
      ValRSurfPrec := ValRSurf;
      // ValRSurf est l'eau contenue dans les réservoirs Revap (non transpirable) et RDE (transpirable et évaporable
      if (ValRFE > 0) then
      begin
        if (ValRFE < EvapPot) then
        begin
          Evap1 := ValRFE;
          Evap2 := Max(0, Min(ValRSurf, ((EvapPot - ValRFE) * ValRSurf) /
            (CapaREvap + CapaRDE))); // borné à 0 et ValRSurf le 27/04/05
        end
        else
        begin
          Evap1 := EvapPot;
          Evap2 := 0;
        end;
      end
      else
      begin
        Evap1 := 0;
        Evap2 := Max(0, Min(ValRSurf, EvapPot * ValRSurf / (CapaREvap +
          CapaRDE))); // borné à 0 et ValRSurf le 27/04/05
      end;
      Evap := Evap1 + Evap2;
      ValRFE := ValRFE - Evap1;
      ValRSurf := ValRSurf - Evap2;
      ValRDE := Max(0, ValRSurf - CapaREvap);
      if (EvapPot = 0) then
      begin
        Kr := 0;
      end
      else
      begin
        Kr := Evap / EvapPot;
      end;
      // part de l'évaporation prélevée dans les réservoirs RFE et RDE
      if (ValRSurf >= CapaREvap) then
      begin
        EvapRU := Evap;
      end
      else
      begin
        if (ValRSurfPrec <= CapaREvap) then
        begin
          EvapRU := Evap1;
        end
        else
        begin
          EvapRU := evap1 + ValRSurfPrec - CapaREvap;
        end;
      end;

      //Evaporation de Ru et Rur, MAJ
      if (RuRac <= RuSurf) then
      begin
        // quand les racines n'ont pas dépassé la première couche
        StockRac := Max(0, StockRac - EvapRU * RuRac / RuSurf);
      end
      else
      begin
        StockRac := Max(0, StockRac - EvapRU);
      end;
      StockTotal := StockTotal - EvapRU;
      StockRac := Min(StockRac, StockTotal);
      // Ajout JCS 29/06/2009
      KceReal := Kce * Kr;
    end;
	// Surface water evaporation during crop cycle
    if (StockMacropores + FloodwaterDepth > 0) and (NumPhase > 0) then
    begin
      Evap := EvapPot;
      ValRSurf := CapaREvap + StockMacropores * (EpaisseurSurf / (EpaisseurSurf
        + EpaisseurProf));
      ValRFE := CapaRFE + StockMacropores * (EpaisseurSurf / (EpaisseurSurf +
        EpaisseurProf));
      ValRDE := CapaRDE;
      StockRac := RuRac + StockMacropores * (RootFront / (EpaisseurSurf +
        EpaisseurProf));
      StockSurface := RuSurf + StockMacropores * (EpaisseurSurf / (EpaisseurSurf
        + EpaisseurProf));
      StockTotal := (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
        StockMacropores;
      StockRac := Min(StockRac, StockTotal);
      Kr := 1;
      KceReal := Kce;
    end;
	{NEW JUNE 19}
	// Surface water evaporation before and after crop cycle
    if (StockMacropores + FloodwaterDepth > 0) and (NumPhase = 0) then
    begin
      Evap := EvapPot;
      FloodwaterDepth := FloodwaterDepth - Evap;
      If (FloodwaterDepth < 0) then
      Begin
         StockMacropores := StockMacropores + FloodwaterDepth;
         FloodwaterDepth := 0;
         StockMacropores := Max(StockMacropores, 0);
      End;
      ValRSurf := CapaREvap + StockMacropores * (EpaisseurSurf / (EpaisseurSurf
        + EpaisseurProf));
      ValRFE := CapaRFE + StockMacropores * (EpaisseurSurf / (EpaisseurSurf +
        EpaisseurProf));
      ValRDE := CapaRDE;
      StockSurface := RuSurf + StockMacropores * (EpaisseurSurf / (EpaisseurSurf
        + EpaisseurProf));
      StockTotal := (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
        StockMacropores;
      Kr := 1;
      KceReal := Kce;
    end;

  except
    AfficheMessageErreur('RS_EvolEvapSurfRFE_RDE_V2', URisocas);
  end;
end;

procedure RS_EvolConsRes_Flood_V2(const NumPhase, RuRac, RuSurf, CapaREvap, Tr,
  Evap, CapaRDE, CapaRFE,
  EpaisseurSurf, EpaisseurProf, ResUtil: Double;
  var StockRac, StockSurface, StockTotal, ValRFE, ValRDE,
  ValRSurf, FloodwaterDepth, StockMacropores: Double);
var
  TrSurf: Double;
  WaterDeficit: Double;
begin
  try
    TrSurf := 0;
    StockSurface := ValRFE + ValRDE;
    if (FloodwaterDepth + StockMacropores = 0) or (NumPhase = 0) then
    begin
      // le calcul de cstr et de Tr doit intervenir après l'évaporation
      // calcul de la part de transpiration affectée aux réservoirs de surface
      if (RuRac <> 0) then
      begin
        if (RuRac <= RuSurf) then
          //correction JCC le 21/08/02 de stRurMax<=RuSurf/stRurMax
        begin
          TrSurf := Tr;
        end
        else
        begin
          //TrSurf:=Tr*RuSurf/stRurMax;// on peut pondérer ici pour tenir compte du % racines dans chaque couche
          if (StockRac <> 0) then
          begin
            TrSurf := Tr * StockSurface / StockRac;
              // modif du 15/04/05  pondération par les stocks et non les capacités, sinon on n'extrait pas Tr si stock nul
          end;
        end;
      end
      else
      begin
        TrSurf := 0;
      end;
      // MAJ des réservoirs de surface en répartissant TrSurf entre RFE et RDE
      ValRDE := Max(0, ValRSurf - CapaREvap);
      if (ValRDE + ValRFE < TrSurf) then
      begin
        ValRFE := 0;
        ValRSurf := ValRSurf - ValRDE;
        ValRDE := 0;
      end
      else
      begin
        if (ValRFE > TrSurf) then
        begin
          ValRFE := ValRFE - TrSurf; // priorité à la RFU
        end
        else
        begin
          ValRSurf := ValRSurf - (TrSurf - ValRFE);
          ValRDE := ValRDE - (TrSurf - ValRFE);
          ValRFE := 0;
        end;
      end;
      StockSurface := ValRFE + ValRDE;
      StockRac := Max(0, StockRac - Tr);
        // 18/04/05 déplacé en fin de procédure, car utilisé pour le calcul de Trsurf
      StockTotal := Max(0, StockTotal - Tr);
      StockRac := Min(StockRac, StockTotal);
    end;
    if ((StockMacropores + FloodwaterDepth) > 0) and ((StockMacropores +
      FloodwaterDepth) <= (Tr + Evap)) and (NumPhase > 0) then
    begin
      WaterDeficit := (Tr + Evap) - (StockMacropores + FloodwaterDepth);
      StockMacropores := 0;
      FloodwaterDepth := 0;
      StockTotal := (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 -
        WaterDeficit;
      StockRac := RuRac - WaterDeficit;
      StockRac := Min(StockRac, StockTotal);
      StockSurface := Max(EpaisseurSurf * ResUtil / 1000 - WaterDeficit, 0);
      ValRFE := Max(StockSurface - ValRDE - Waterdeficit, 0);
      ValRDE := ValRDE;
      ValRSurf := ValRFE + ValRDE;
    end
    else
    begin
      if ((StockMacropores + FloodwaterDepth) > (Tr + Evap)) and (NumPhase > 0)
        then
      begin
        FloodwaterDepth := FloodwaterDepth - (Tr + Evap);
        StockMacropores := StockMacropores + Min(0, FloodwaterDepth);
        FloodwaterDepth := Max(FloodwaterDepth, 0);
        StockTotal := (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
          StockMacropores;
        StockRac := RuRac + StockMacropores;
        StockRac := Min(StockRac, StockTotal);
        StockSurface := Max(EpaisseurSurf * ResUtil / 1000 + StockMacropores *
          (EpaisseurSurf / (EpaisseurSurf + EpaisseurProf)), 0);
        ValRFE := Max(StockSurface - ValRDE, 0);
        ValRDE := ValRDE;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvolConsRes_Flood_V2', URisocas);
  end;
end;

procedure RS_EvalConversion(const NumPhase, EpsiB, AssimBVP, SommeDegresJourCor,
  SommeDegresJourPhasePrecedente, AssimMatu1, AssimMatu2,
  SeuilTempPhaseSuivante: Double;
  var Conversion: Double);
var
  KAssim: Double;
begin
  try
    { Modif JCC du 21/04/05 en phase BVP KAssim est toujours = 1, sinon autant modifier Tx Conversion.
    Ensuite Kdf augmente et on ne sait pas simuler cette variation. On va jouer sur AssimBVP,
     que l'on pourra d'ailleurs renommer AssimRPR}
    case trunc(NumPhase) of
      2: KAssim := 1;
      3..4: KAssim := AssimBVP;
      5: KAssim := AssimBVP + (SommeDegresJourCor -
        SommeDegresJourPhasePrecedente) *
        (AssimMatu1 - AssimBVP) / (SeuilTempPhaseSuivante -
          SommeDegresJourPhasePrecedente);
      6: KAssim := AssimMatu1 + (SommeDegresJourCor -
        SommeDegresJourPhasePrecedente) *
        (AssimMatu2 - AssimMatu1) / (SeuilTempPhaseSuivante -
          SommeDegresJourPhasePrecedente);
    else
      KAssim := 0;
    end;
    Conversion := KAssim * EpsiB;
	
	//if Conversion < 0 then
		//Conversion := 0;
	
  except
    AfficheMessageErreur('RS_EvalConversion | NumPhase: ' + FloatToStr(NumPhase)
      +
      ' SommeDegresJourCor: ' + FloatToStr(SommeDegresJourCor), URisocas);
  end;
end;

procedure RS_EvolPSPMVMD(const Numphase, ChangePhase, SomDegresJourCor,
  DegresDuJourCor,
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
  try
    if (NumPhase = 3) then
    begin
      if (ChangePhase = 1) then
      begin
        SumPP := 100; //valeur arbitraire d'initialisation >2.5
        SDJPSP := Max(0.01, DegresDuJourCor);
      end
      else
      begin
        SDJPSP := SomDegresJourCor - SeuilTempPhasePrec + Max(0.01,
          DegresDuJourCor);
      end;
      SumPP := Power((1000 / SDJPSP), PPExp) * Max(0, (DureeDuJour - PPCrit)) /
        (SeuilPP - PPCrit);
      SeuilTempPhaseSuivante := SeuilTempPhasePrec + SDJPSP;
    end;
  except
    AfficheMessageErreur('RS_EvolPSPMVMD', URisocas);
  end;
end;

procedure RS_EvolSomDegresJourCor(const DegresDuJourCor, NumPhase: Double;
  var SommeDegresJourCor: Double);
begin
  try
    if (NumPhase >= 1) then // on ne cumule qu'après la germination
    begin
      SommeDegresJourCor := SommeDegresJourCor + DegresDuJourCor;
    end
    else
    begin
      SommeDegresJourCor := 0;
    end;
  except
    AfficheMessageErreur('RS_EvolSomDegresJourCor', URisocas);
  end;
end;

procedure RS_EvalMaximumLai(const NumPhase, ChangePhase, Lai: Double;
  var TempLai, MaximumLai: Double);
begin
  try
    if (Lai > TempLai) then
    begin
      TempLai := Lai;
    end;
    if (NumPhase <> 7) then
    begin
      MaximumLai := 0;
    end
    else if (NumPhase = 7) and (ChangePhase = 1) then
    begin
      MaximumLai := TempLai;
    end;
  except
    AfficheMessageErreur('RS_EvalMaximumLai', URisocas);
  end;
end;

procedure RS_EvalVitesseRacinaire(const VRacLevee, RootSpeedBVP, RootSpeedRPR,
  RootSpeedPSP,
  RootSpeedMatu1, RootSpeedMatu2, RootCstr, cstr, NumPhase, DegreDuJourCor:
    Double; //DegreDuJour ou DegreDuJourCor?
  var VitesseRacinaire, VitesseRacinaireDay: Double);
//Modif JCC du 15/03/2005 pour inclure VracLevée différente de VRacBVP
begin
  try
    case Trunc(NumPhase) of
      1: VitesseRacinaire := VRacLevee;
      2: VitesseRacinaire := RootSpeedBVP;
      3: VitesseRacinaire := RootSpeedPSP;
      4: VitesseRacinaire := RootSpeedRPR;
      5: VitesseRacinaire := RootSpeedMatu1;
        { TODO : attention en cas de gestion du champ vide... }
      6: VitesseRacinaire := RootSpeedMatu2;
    else
      VitesseRacinaire := 0
    end;
    VitesseRacinaireDay := VitesseRacinaire * DegreDuJourCor * Power(cstr,
      RootCstr);
  except
    AfficheMessageErreur('EvalVitesseRacinaire | NumPhase: ' +
      FloatToStr(NumPhase), URisocas);
  end;
end;

procedure RS_EvolDryMatTot(const NumPhase, ChangePhase, PlantsPerHill,
  TxResGrain, PoidsSecGrain, Densite,
  GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop,
  GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop,
  GrowthResInternodePop, GrainYieldPop, ResCapacityInternodePop,
  CulmsPerPlant, CoeffPanSinkPop, SterilityTot: Double;

  var DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop,
  DryMatStructInternodePop, DryMatStructPaniclePop, DryMatStructStemPop,
    DryMatStructTotPop,
  DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
  DryMatAboveGroundPop, DryMatTotPop, HarvestIndex,
  InternodeResStatus, PanicleNumPop, PanicleNumPlant,
  GrainYieldPanicle, SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop,
  GrainFillingStatus, RootShootRatio: Double);
begin
  try
    if ((NumPhase = 2) and (ChangePhase = 1)) then
    begin
      DryMatTotPop := Densite * PlantsPerHill * TxResGrain * PoidsSecGrain /
        1000;
      DryMatStructLeafPop := DryMatTotPop * 0.5;
    end
    else
    begin
      if (NumPhase > 1) then
      begin
        DryMatStructLeafPop := DryMatStructLeafPop + GrowthStructLeafPop;
        DryMatStructSheathPop := DryMatStructSheathPop + GrowthStructSheathPop;
        DryMatStructRootPop := DryMatStructRootPop + GrowthStructRootPop;
        DryMatStructInternodePop := DryMatStructInternodePop +
          GrowthStructInternodePop;
        DryMatStructPaniclePop := DryMatStructPaniclePop +
          GrowthStructPaniclePop;
        DryMatStructStemPop := DryMatStructSheathPop + DryMatStructInternodePop
          + DryMatResInternodePop;
        DryMatStructTotPop := DryMatStructLeafPop + DryMatStructSheathPop +
          DryMatStructRootPop + DryMatStructInternodePop + DryMatStructPaniclePop;
        DryMatVegeTotPop := DryMatStructTotPop + DryMatResInternodePop -
          DryMatStructPaniclePop;
        DryMatPanicleTotPop := DryMatStructPaniclePop + GrainYieldPop;
        DryMatTotPop := DryMatVegeTotPop + GrainYieldPop;
        DryMatAboveGroundPop := DryMatTotPop - DryMatStructRootPop;
        RootShootRatio := DryMatStructRootPop / DryMatAboveGroundPop;
        if (ResCapacityInternodePop = 0) then
        begin
          InternodeResStatus := 0;
        end
        else
        begin
          InternodeResStatus := DryMatResInternodePop / ResCapacityInternodePop;
        end;
      end;
      if (NumPhase > 4) then
      begin
        HarvestIndex := GrainYieldPop / DryMatAboveGroundPop;
        PanicleNumPlant := CulmsPerPlant;
        PanicleNumPop := CulmsPerPlant * Densite * PlantsPerHill;
        GrainYieldPanicle := 1000 * GrainYieldPop / PanicleNumPop;
        SpikeNumPop := 1000000 * DryMatStructPaniclePop * CoeffPanSinkPop /
          PoidsSecGrain;
        SpikeNumPanicle := SpikeNumPop / PanicleNumPop;
        FertSpikeNumPop := SpikeNumPop * (1 - SterilityTot);
        GrainFillingStatus := 1000000 * (GrainYieldPop / FertSpikeNumPop) /
          PoidsSecGrain;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvolDryMatTot', URisocas);
  end;
end;

procedure RS_EvolDryMatTot_V2(const NumPhase, ChangePhase, PlantsPerHill,
  TxResGrain, PoidsSecGrain, Densite,
  GrowthStructLeafPop, GrowthStructSheathPop, GrowthStructRootPop,
  GrowthStructInternodePop, GrowthStructPaniclePop, GrowthStructTotPop,
  GrowthResInternodePop, GrainYieldPop, ResCapacityInternodePop,
  CulmsPerPlant, CoeffPanSinkPop, SterilityTot, DeadLeafdrywtPop {NEW LB}, DryMatResInternodePopOld, PanicleFilPop , AssimNotUsedCum {NEW R} , MobiliLeafDeath: Double;

  var DryMatStructLeafPop, DryMatStructSheathPop, DryMatStructRootPop,
  DryMatStructInternodePop, DryMatStructPaniclePop, {NEW LB} DryMatStemPop {/NEW LB},
    DryMatStructTotPop,
  DryMatResInternodePop, DryMatVegeTotPop, DryMatPanicleTotPop,
  DryMatAboveGroundPop, DryMatTotPop, HarvestIndex,
  InternodeResStatus, PanicleNumPop, PanicleNumPlant,
  GrainYieldPanicle, SpikeNumPop, SpikeNumPanicle, FertSpikeNumPop,
  GrainFillingStatus, RootShootRatio  , DryMatAboveGroundTotPop , {NEW LB} CumGrowthPop, GrowthPop , CumCarbonUsedPop : Double);
begin
  try
  
	{NEW LB}
	CumGrowthPop := CumGrowthPop + GrowthStructLeafPop + GrowthStructSheathPop + GrowthStructInternodepop + GrowthStructRootPop + GrowthStructPaniclePop + (DryMatResInternodePop - DryMatResInternodePopOld) + PanicleFilPop {NEW R} - MobiliLeafDeath;
	
	GrowthPop := GrowthStructLeafPop + GrowthStructSheathPop + GrowthStructInternodepop + GrowthStructRootPop + GrowthStructPaniclePop + (DryMatResInternodePop - DryMatResInternodePopOld) + PanicleFilPop {NEW R} - MobiliLeafDeath;
	// Output Test variable for carbon balance (consider also AssimNotUsedCum)
	{/NEW LB}
  
    if ((NumPhase = 2) and (ChangePhase = 1)) then
    begin
      DryMatTotPop := Densite * PlantsPerHill * TxResGrain * PoidsSecGrain /
        1000;
      DryMatStructLeafPop := DryMatTotPop * 0.5;
    end
    else 
    begin
      if (NumPhase > 1) then
      begin
        DryMatStructLeafPop := DryMatStructLeafPop + GrowthStructLeafPop;
        DryMatStructSheathPop := DryMatStructSheathPop + GrowthStructSheathPop;
        DryMatStructRootPop := DryMatStructRootPop + GrowthStructRootPop;
        DryMatStructInternodePop := DryMatStructInternodePop +
          GrowthStructInternodePop;
        DryMatStructPaniclePop := DryMatStructPaniclePop +
          GrowthStructPaniclePop;
        DryMatStemPop := DryMatStructSheathPop + DryMatStructInternodePop
          + DryMatResInternodePop;
        DryMatStructTotPop := DryMatStructLeafPop + DryMatStructSheathPop +
          DryMatStructRootPop + DryMatStructInternodePop + DryMatStructPaniclePop;
        DryMatVegeTotPop := DryMatStemPop + DryMatStructLeafPop + DryMatStructRootPop {DELETED LB} {+ DryMatResInternodePop} + DeadLeafDryWtPop;{DELETED LB}{DryMatStructTotPop + DryMatResInternodePop -
          DryMatStructPaniclePop;}
        DryMatPanicleTotPop := DryMatStructPaniclePop + GrainYieldPop;
        DryMatTotPop := DryMatVegeTotPop {NEW LB}+ DrymatPanicleTotPop{/NEW LB};{DELETED LB} {+ GrainYieldPop} {NEW Y}{+ DryMatStructPaniclePop }{/NEW Y}{NEW G}{+ DeadLeafDryWtPop}{/NEW G}{;}
        DryMatAboveGroundPop := DryMatTotPop - DryMatStructRootPop {NEW LB} - DeadLeafDryWtPop;
		{NEW Y}
        DryMatAboveGroundTotPop := DryMatAboveGroundPop + DeadLeafDrywtPop;    
		{/NEW Y}
		{NEW LB}
		CumCarbonUsedPop := DryMatTotPop + AssimNotUsedCum; // This should be equal to CumSupplyTot!
		{/NEW LB}
        RootShootRatio := DryMatStructRootPop / DryMatAboveGroundPop;
        if (ResCapacityInternodePop = 0) then
        begin
          InternodeResStatus := 0;
        end
        else
        begin
          InternodeResStatus := DryMatResInternodePop / ResCapacityInternodePop;
        end;
      end;
      if (NumPhase > 4) then
      begin
        HarvestIndex := GrainYieldPop / {NEW LB}DryMatAboveGroundTotPop; // This includes dead leaves
        PanicleNumPlant := CulmsPerPlant;
        PanicleNumPop := CulmsPerPlant * Densite * PlantsPerHill;
        GrainYieldPanicle := 1000 * GrainYieldPop / PanicleNumPop;
        SpikeNumPop := 1000 * DryMatStructPaniclePop * CoeffPanSinkPop /
          PoidsSecGrain;
        SpikeNumPanicle := SpikeNumPop / PanicleNumPop;
        FertSpikeNumPop := SpikeNumPop * (1 - SterilityTot);
        GrainFillingStatus := 1000 * (GrainYieldPop / FertSpikeNumPop) /
          PoidsSecGrain;
      end;
    end;
  except
  on E : Exception do
    AfficheMessageErreur('RS_EvolDryMatTot_V2 '+E.message, URisocas);
  end;
end;

procedure RS_InitiationCulture(const SeuilTempLevee, SeuilTempBVP, SeuilTempRPR,
  SeuilTempMatu1, SeuilTempMatu2: Double;
  var SommeDegresJourMaximale, NumPhase, SommeDegresJour,
  SeuilTempPhaseSuivante, Lai, IcCumul, FTSW, cstr, DurPhase1, DurPhase2,
  DurPhase3, DurPhase4, DurPhase5, DurPhase6, TempLai, ApexHeightGain,
  ChangeNurseryStatus, ChangePhase, ChangeSsPhase, CstrPhase2, CstrPhase3,
  CstrPhase4, CstrPhase5, CstrPhase6, CumCstrPhase2, CumCstrPhase3,
  CumCstrPhase4, CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3,
  CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3,
  CumIcPhase4, CumIcPhase5, CumIcPhase6, DAF, DemLeafAreaPlant,
  DemPanicleFillPop, DemStructInternodePlant, DemStructInternodePop,
  DemStructLeafPlant, DemStructLeafPop, DemStructPaniclePlant,
  DemStructPaniclePop, DemStructRootPlant, DemStructRootPop,
  DemStructSheathPop, DemStructTotPop, FloodWaterGain, FtswPhase2, FtswPhase3,
  FtswPhase4, FtswPhase5, FtswPhase6, GainRootSystSoilSurfPop,
  GainRootSystVolPop, GrowthDryMatPop, GrowthResInternodePop,
  GrowthStructDeficit, GrowthStructInternodePop, GrowthStructLeafPop,
  GrowthStructPaniclePop, GrowthStructRootPop, GrowthStructSheathPop,
  GrowthStructTotPop, HaunGain, IcPhase2, IcPhase3, IcPhase4, IcPhase5,
  IcPhase6, IncreaseResInternodePop, Kcl, Kr, MobiliLeafDeath,
  NbDaysSinceGermination, NurseryStatus, PanicleFilDeficit, PanicleFilPop,
  PanicleSinkPop, PanStructMass, PlantLeafNumNew, ResInternodeMobiliDay,
  ResInternodeMobiliDayPot, RootFrontOld, RootSystSoilSurfPop,
  RootSystSoilSurfPopOld, RootSystVolPop, RootSysVolPopOld, SDJCorPhase4 : Double);
begin
  try
    NumPhase := 0;
    SommeDegresJourMaximale := SeuilTempLevee + SeuilTempBVP + SeuilTempRPR +
      SeuilTempMatu1 + SeuilTempMatu2;
    SommeDegresJour := 0;
    SeuilTempPhaseSuivante := 0;
    Lai := 0;
    IcCumul := 0;
    FTSW := 1;
    cstr := 1;
    DurPhase1 := 0;
    DurPhase2 := 0;
    DurPhase3 := 0;
    DurPhase4 := 0;
    DurPhase5 := 0;
    DurPhase6 := 0;
    TempLai := 0;
    ApexHeightGain := 0;
    ChangeNurseryStatus := 0;
    ChangePhase := 0;
    ChangeSsPhase := 0;
    CstrPhase2 := 0;
    CstrPhase3 := 0;
    CstrPhase4 := 0;
    CstrPhase5 := 0;
    CstrPhase6 := 0;
    CumCstrPhase2 := 0;
    CumCstrPhase3 := 0;
    CumCstrPhase4 := 0;
    CumCstrPhase5 := 0;
    CumCstrPhase6 := 0;
    CumFTSWPhase2 := 0;
    CumFTSWPhase3 := 0;
    CumFTSWPhase4 := 0;
    CumFTSWPhase5 := 0;
    CumFTSWPhase6 := 0;
    CumIcPhase2 := 0;
    CumIcPhase3 := 0;
    CumIcPhase4 := 0;
    CumIcPhase5 := 0;
    CumIcPhase6 := 0;
    DAF := 0;
    DemLeafAreaPlant := 0;
    DemPanicleFillPop := 0;
    DemStructInternodePlant := 0;
    DemStructInternodePop := 0;
    DemStructLeafPlant := 0;
    DemStructLeafPop := 0;
    DemStructPaniclePlant := 0;
    DemStructPaniclePop := 0;
    DemStructRootPlant := 0;
    DemStructRootPop := 0;
    DemStructSheathPop := 0;
    DemStructTotPop := 0;
    FloodWaterGain := 0;
    FtswPhase2 := 0;
    FtswPhase3 := 0;
    FtswPhase4 := 0;
    FtswPhase5 := 0;
    FtswPhase6 := 0;
    GainRootSystSoilSurfPop := 0;
    GainRootSystVolPop := 0;
    GrowthDryMatPop := 0;
    GrowthResInternodePop := 0;
    GrowthStructDeficit := 0;
    GrowthStructInternodePop := 0;
    GrowthStructLeafPop := 0;
    GrowthStructPaniclePop := 0;
    GrowthStructRootPop := 0;
    GrowthStructSheathPop := 0;
    GrowthStructTotPop := 0;
    HaunGain := 0;
    IcPhase2 := 0;
    IcPhase3 := 0;
    IcPhase4 := 0;
    IcPhase5 := 0;
    IcPhase6 := 0;
    IncreaseResInternodePop := 0;
    Kcl := 0;
    Kr := 0;
    MobiliLeafDeath := 0;
    NbDaysSinceGermination := 0;
    NurseryStatus := 0;
    PanicleFilDeficit := 0;
    PanicleFilPop := 0;
    PanicleSinkPop := 0;
    PanStructMass := 0;
    PlantLeafNumNew := 0;
    ResInternodeMobiliDay := 0;
    ResInternodeMobiliDayPot := 0;
    RootFrontOld := 0;
    RootSystSoilSurfPop := 0;
    RootSystSoilSurfPopOld := 0;
    RootSystVolPop := 0;
    RootSysVolPopOld := 0;
    SDJCorPhase4 := 0;
  except
    AfficheMessageErreur('RS_InitiationCulture', URisocas);
  end;
end;

procedure RS_InitiationCulture_V2(const SeuilTempLevee, SeuilTempBVP,
  SeuilTempRPR,
  SeuilTempMatu1, SeuilTempMatu2, EpaisseurSurf,
  EpaisseurProf, VolRelMacropores: Double;
  var SommeDegresJourMaximale, NumPhase, SommeDegresJour,
  SeuilTempPhaseSuivante, Lai, IcCumul, VolMacropores: Double);
begin
  try
    NumPhase := 0;
    SommeDegresJourMaximale := SeuilTempLevee + SeuilTempBVP + SeuilTempRPR +
      SeuilTempMatu1 + SeuilTempMatu2;
    SommeDegresJour := 0;
    SeuilTempPhaseSuivante := 0;
    Lai := 0;
    IcCumul := 0;
    VolMacropores := VolRelMacropores * (EpaisseurSurf + EpaisseurProf) / 100;
  except
    AfficheMessageErreur('RS_InitiationCulture_V2', URisocas);
  end;
end;
 
procedure RS_EvolGrowthStructTot(const NumPhase {SHIFTED LB}{, GrowthStructLeafPop,
  GrowthStructSheathPop, GrowthStructRootPop,
  GrowthStructInternodePop, GrowthStructPaniclePop } {/SHIFTED LB}, SupplyTot, GrowthResInternodePop: Double;
  var GrowthStructTotPop, AssimSurplus {SHIFTED LB} , GrowthStructLeafPop,
  GrowthStructSheathPop, GrowthStructRootPop,
  GrowthStructInternodePop, GrowthStructPaniclePop {/SHIFTED LB} ,A_GrowthStructLeaf, A_GrowthStructTot, A_AssimSurplus : Double);
begin
  try
    if ((NumPhase > 1) and (NumPhase < 5)) then
    begin
      GrowthStructTotPop := GrowthStructLeafPop + GrowthStructSheathPop +
        GrowthStructRootPop +
        GrowthStructInternodePop + GrowthStructPaniclePop {NEW P}+ GrowthResInternodePop;
	
	  A_GrowthStructTot := GrowthStructTotPop;
	
      AssimSurplus := Max((SupplyTot - GrowthStructTotPop {DELETED}{- GrowthResInternodePop}), 0);
	  A_AssimSurplus := Max((SupplyTot - GrowthStructTotPop {DELETED}{- GrowthResInternodePop}), 0);
    end
	{NEW LB}
	else
	begin
		GrowthStructLeafPop := 0;
		A_GrowthStructLeaf := GrowthStructLeafPop;
		
		GrowthStructSheathPop := 0;
		GrowthStructInternodePop := 0;
		GrowthStructRootPop := 0;
		GrowthStructPaniclePop := 0;
		GrowthStructTotPop := 0;
		A_GrowthStructTot := GrowthStructTotPop;
	end;
	{/NEW LB}
  except
    AfficheMessageErreur('RS_EvolGrowthStructTot', URisocas);
  end;
end;

procedure RS_LeafRolling(const NumPhase, RollingBase, RollingSens, FTSW, Eto:
  Double;
  var KRolling: Double);
begin
  try
    if (NumPhase > 1) then
    begin
      KRolling := RollingBase + (1 - RollingBase) * Power(FTSW, Max(0.0000001,
        Eto * RollingSens));
		
	  if KRolling > 1 then
		KRolling := 1;
	  
    end;
  except
    AfficheMessageErreur('RS_LeafRolling', URisocas);
  end;
end;

procedure RS_EvalRootFront(const NumPhase, RuRac, ResUtil: Double;
  var RootFront: Double);
begin
  try
    if (NumPhase > 0) then
    begin
      RootFront := ((1 / ResUtil) * RuRac) * 1000;
    end;
  except
    AfficheMessageErreur('RS_EvalRootFront', URisocas);
  end;
end;

procedure RS_EvalSDJPhase4(const NumPhase, DegreDuJourCor: Double; var
  SDJPhase4: Double);
begin
  try
    if (NumPhase = 4) then
    begin
      SDJPhase4 := SDJPhase4 + DegreDuJourCor;
    end;
  except
    AfficheMessageErreur('RS_EvalSDJPhase4', URisocas);
  end;
end;

procedure RS_EvalDateGermination(const NumPhase, ChangePhase: Double;
  var NbDaysSinceGermination: double);
begin
  try
    if ((NumPhase < 1) or ((NumPhase = 1) and (ChangePhase = 1))) then
    begin
      NbDaysSinceGermination := 0;
    end
    else
    begin
      NbDaysSinceGermination := NbDaysSinceGermination + 1;
    end;
  except
    AfficheMessageErreur('RS_EvalDateGermination', URisocas);
  end;
end;

procedure RS_EvalSterility(const Numphase, ChangePhase, KCritSterCold1,
  KCritSterCold2, KCritSterHeat1, KCritSterHeat2,
  KCritSterFtsw1, KCritSterFtsw2, TMinMoy, TMaxMoy, FtswMoy: Double;
  var SterilityCold, SterilityHeat, SterilityDrought, SterilityTot: Double);
begin
  try
    if ((NumPhase = 5) and (ChangePhase = 1)) then
    begin
      SterilityCold := Max(0, (Min(1, KCritSterCold1 / (KCritSterCold1 -
        KCritSterCold2) - TMinMoy / (KCritSterCold1 - KCritSterCold2))));
      SterilityHeat := 1 - Max(0, (Min(1, KCritSterHeat2 / (KCritSterHeat2 -
        KCritSterHeat1) - TMaxMoy / (KCritSterHeat2 - KCritSterHeat1))));
      SterilityDrought := Max(0, (Min(1, KCritSterFtsw1 / (KCritSterFtsw1 -
        KCritSterFtsw2) - FtswMoy / (KCritSterFtsw1 - KCritSterFtsw2))));
    end
    else
    begin
      SterilityCold := SterilityCold;
      SterilityHeat := SterilityHeat;
      SterilityDrought := SterilityDrought;
    end;
    SterilityTot := Min(0.999, 1 - ((1 - SterilityCold) * (1 - SterilityHeat) *
      (1 - SterilityDrought)));
  except
    AfficheMessageErreur('RS_EvalSterility', URisocas);
  end;
end;

procedure RS_ResetVariablesToZero(const NumPhase, ChangePhase: Double; var
  CulmsPerPlant, CulmsPerHill, CulmsPop, GrainYieldPop, DryMatStructLeafPop,
  DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
  DryMatResInternodePop, DryMatStructPaniclePop, DryMatStructStemPop,
    DryMatStructTotPop,
  DryMatVegeTotPop, DryMatPanicleTotPop, DryMatAboveGroundPop, DryMatTotPop,
  HarvestIndex, PanicleNumPop, PanicleNumPlant, GrainYieldPanicle,
  SpikeNumPop, SpikeNumPanicle, FertSpikePop, GrainFillingStatus,
  PhaseStemElongation, Sla, HaunIndex, ApexHeight, PlantHeight, PlantWidth,
  VitesseRacinaireDay, Kcl, KRolling, LIRKdfcl, LtrKdfcl,
  AssimPot, Assim, RespMaintTot, SupplyTot, AssimSurplus, AssimNotUsed,
  AssimNotUsedCum, TillerDeathPop, DeadLeafDryWtPop, ResCapacityInternodePop,
  InternodeResStatus, cstr, FTSW , DryMatAboveGroundTotPop: Double);

begin
  try
    if ((NumPhase = 7) and (ChangePhase = 1)) then
    begin
      CulmsPerPlant := 0;
      CulmsPerHill := 0;
      CulmsPop := 0;
      GrainYieldPop := 0;
      DryMatStructLeafPop := 0;
      DryMatStructSheathPop := 0;
      DryMatStructRootPop := 0;
      DryMatStructInternodePop := 0;
      DryMatResInternodePop := 0;
      DryMatStructPaniclePop := 0;
      DryMatStructStemPop := 0;
      DryMatStructTotPop := 0;
      DryMatVegeTotPop := 0;
      DryMatPanicleTotPop := 0;
      DryMatAboveGroundPop := 0;
      DryMatTotPop := 0;
      HarvestIndex := 0;
      PanicleNumPop := 0;
      PanicleNumPlant := 0;
      GrainYieldPanicle := 0;
      SpikeNumPop := 0;
      SpikeNumPanicle := 0;
      FertSpikePop := 0;
      GrainFillingStatus := 0;
      PhaseStemElongation := 0;
      Sla := 0;
      HaunIndex := 0;
      ApexHeight := 0;
      PlantHeight := 0;
      PlantWidth := 0;
      VitesseRacinaireDay := 0;
      Kcl := 0;
      KRolling := 0;
      LIRKdfcl := 0;
      LTRKdfcl := 1;
      AssimPot := 0;
      Assim := 0;
      RespMaintTot := 0;
      SupplyTot := 0;
      AssimSurplus := 0;
      AssimNotUsed := 0;
      AssimNotUsedCum := 0;
      TillerDeathPop := 0;
      DeadLeafDryWtPop := 0;
      ResCapacityInternodePop := 0;
      InternodeResStatus := 0;
      cstr := 0;
      FTSW := 0;
	  DryMatAboveGroundTotPop := 0;
    end;
  except
    AfficheMessageErreur('RS_ResetVariablesToZero', URisocas);
  end;
end;

procedure RS_InitParcelle(const StockIniSurf, StockIniProf, Ru, EpaisseurSurf,
  EpaisseurProf, HumPF, PEvap, DateSemis: Double;
  var StTot, LTRkdfcl, Hum, RuSurf, ProfRU, StRuMax,
  CapaREvap, CapaRFE, CapaRDE, ValRSurf, ValRDE, ValRFE,
  StockSurface: Double);
var
  Stockini2: Double;
  Stockini1: Double;
begin
  try
    ProfRU := EpaisseurSurf + EpaisseurProf;
      // à supprimer ultérieurement dans les différents modules
    RuSurf := Ru * EpaisseurSurf / 1000;
    CapaREvap := 0.5 * EpaisseurSurf * HumPF;
    CapaRFE := PEvap * (CapaREvap + RuSurf);
    CapaRDE := RuSurf - CapaRFE;
    StRuMax := Ru * ProfRu / 1000;
    Stockini1 := Min(StockIniSurf, CapaREvap + RuSurf);
    Stockini2 := Min(StockIniProf, Ru * EpaisseurProf / 1000);
    ValRSurf := Min(Stockini1, CapaREvap + CapaRDE);
    ValRDE := Max(0, ValRSurf - CapaREvap);
    ValRFE := Max(0, Stockini1 - (CapaREvap + CapaRDE));
    StockSurface := ValRDE + ValRFE;
    StTot := StockSurface + Stockini2; //transpirable
    Hum := StTot;
    LTRkdfcl := 1;
  except
    AfficheMessageErreur('RS_InitParcelle', URisocas);
  end;
end;

procedure RS_InitParcelle_V2(const StockIniSurf, StockIniProf, EpaisseurSurf,
  EpaisseurProf, HumPF, HumFC, HumSat, PEvap, DateSemis : Double;
  var ResUtil, StockTotal, LTRkdfcl, Hum, RuSurf, ProfRU, StRuMax,
  CapaREvap, CapaRFE, CapaRDE, ValRSurf, ValRDE, ValRFE,
  StockSurface, CounterNursery, VolRelMacropores, VolMacropores, LIRkdf,
  LTRkdf: Double);
var
  Stockini2: Double;
  Stockini1: Double;
begin
  try
    VolRelMacropores := 100 * (HumSat - HumFC);
    ResUtil := (HumFC - HumPF) * 1000;
    ProfRU := EpaisseurSurf + EpaisseurProf;
      // à supprimer ultérieurement dans les différents modules
    RuSurf := ResUtil * EpaisseurSurf / 1000;
    CapaREvap := 0.5 * EpaisseurSurf * HumPF;
    CapaRFE := PEvap * (CapaREvap + RuSurf);
    CapaRDE := RuSurf - CapaRFE;
    StRuMax := ResUtil * ProfRu / 1000;
    Stockini1 := Min(StockIniSurf, CapaREvap + RuSurf);
    Stockini2 := Min(StockIniProf, ResUtil * EpaisseurProf / 1000);
    ValRSurf := Min(Stockini1, CapaREvap + CapaRDE);
    ValRDE := Max(0, ValRSurf - CapaREvap);
    ValRFE := Max(0, Stockini1 - (CapaREvap + CapaRDE));
    StockSurface := ValRDE + ValRFE;
    StockTotal := StockSurface + Stockini2; //transpirable
    Hum := StockTotal;
    LTRkdfcl := 1;
    LIRkdf := 0;
    LTRkdf := 0;
    CounterNursery := 0;
    VolMacropores := VolRelMacropores * (EpaisseurSurf + EpaisseurProf) / 100;
  except
    AfficheMessageErreur('RS_InitParcelle_V2', URisocas);
  end;
end;

procedure RS_EvalAssimPot(const PARIntercepte, PAR, Conversion, Tmax, Tmin, Tbase,
  Topt1, DayLength, StressCold, CO2Exp, Ca , CO2Cp {NEW Y}, SlaMin , Sla , CoeffAssimSla : Double; var AssimPot, CoeffCO2Assim: Double);

begin
  try
	begin


    if (-CO2Exp <> 0) and (CO2Cp <> 0) then
		CoeffCO2Assim := (1 - exp(-CO2Exp * (Ca - CO2Cp))) / (1 - exp(-CO2Exp * (400 - CO2Cp))); 

	// This coefficient always equals 1 at 400ppm CO2 and describes AssimPot response to Ca

    AssimPot := PARIntercepte * Conversion * 10 * CoeffCO2Assim;
    // Ordinary linear effect on intercepted light on canopy assimulation , multiplied by CO2 effect
	
    AssimPot := AssimPot * Min(((3 * Tmax + Tmin) / 4 - Tbase) / (Topt1 -
      Tbase), 1);
	 
	 
    // Reduction of assimilation at diurnal temperatures < Topt1
    AssimPot := AssimPot * Sqrt(Max(0.00001, StressCold));
    // Cold stress effect on AssimPot (affects also organ demands and grain filling)
	
    if ((PARIntercepte <> 0) and (DayLength <> 0)) then
    begin

	  
	  AssimPot := AssimPot *  Power(  (PAR / DayLength), 0.667) / (PAR / DayLength);
	  // De-linearization of PAR response of AssimPot. At 1 MJ/h (cloudless) effect is zero
	  
	  {NEW Y}
	  AssimPot := AssimPot * Power((SlaMin / Max(Sla, SlaMin)), CoeffAssimSla);
      // Effect of SLA on AssimPot ; AssimPot is reduced if Sla>SlaMin; For no effect set parameter CoeffAssimSla=0, for proportional effect set CoeffAssimSla=1. Intermediate values are OK.

	  
	   
    end;
	end;
  except
  on E : Exception do
  begin
    AfficheMessageErreur('RS_EvalAssimPot ', URisocas);
  end;
  end;
end;

procedure RS_EvalEvapPot(const Etp, Kce: Double; var EvapPot: Double);
begin
  try
    EvapPot := Kce * Etp;
  except
    AfficheMessageErreur('RS_EvalEvapPot', URisocas);
  end;
end;

procedure RS_Transplanting_V2(const NumPhase, DensityNursery, DensityField,
  DurationNursery, PlantsPerHill, Transplanting: Double;
  var NurseryStatus, ChangeNurseryStatus, CounterNursery, Density,
    DryMatStructLeafPop,
  DryMatStructSheathPop, DryMatStructRootPop, DryMatStructInternodePop,
    DryMatStructPaniclePop,
  DryMatResInternodePop: Double);
var
  DensityChange: Double;
begin
  try
    DensityChange := DensityField / (DensityNursery / PlantsPerHill);
    if ((Transplanting = 1) and (NumPhase >= 1)) then
    begin
      CounterNursery := CounterNursery + 1;
    end;
    if ((Transplanting = 1) and (CounterNursery < DurationNursery) and
      (ChangeNurseryStatus = 0)) then
    begin
      NurseryStatus := 0;
      ChangeNurseryStatus := 0;
    end
    else
    begin
      if ((Transplanting = 1) and (CounterNursery >= DurationNursery) and
        (ChangeNurseryStatus = 0) and (NurseryStatus = 0)) then
      begin
        NurseryStatus := 1;
        ChangeNurseryStatus := 1;
      end
      else
      begin
        NurseryStatus := 1;
        ChangeNurseryStatus := 0;
      end;
    end;
    if (NurseryStatus = 1) then
    begin
      Density := DensityField;
    end
    else
    begin
      Density := DensityNursery / PlantsPerHill;
    end;
    if (ChangeNurseryStatus = 1) then
    begin
      DryMatStructLeafPop := DryMatStructLeafPop * DensityChange;
      DryMatStructSheathPop := DryMatStructSheathPop * DensityChange;
      DryMatStructRootPop := DryMatStructRootPop * DensityChange;
      DryMatStructInternodePop := DryMatStructInternodePop * DensityChange;
      DryMatStructPaniclePop := DryMatStructPaniclePop * DensityChange;
      DryMatResInternodePop := DryMatResInternodePop * DensityChange;
    end;
  except
    AfficheMessageErreur('RS_Transplanting_V2', URisocas);
  end;
end;

procedure RS_TransplantingShock_V2(const CounterNursery,
  CoeffTransplantingShock: Double; var Assim: Double);
begin
  try
    if ((CounterNursery > 0) and (CounterNursery < 8)) then
    begin
      Assim := Assim * CoeffTransplantingShock;
    end
    else
    begin
      Assim := Assim;
    end;
  except
    AfficheMessageErreur('RS_TransplantingShock_V2', URisocas);
  end;
end;

procedure RS_EvalRuiss_FloodDyna_V2(const NumPhase, Rain, SeuilRuiss,
  PourcRuiss,
  BundHeight, Irrigation, PlantHeight,
  LifeSavingDrainage, PlotDrainageDAF,
  VolMacropores, SuilRuiss, PercolationMax, DAF: Double;
  var StockMacropores, FloodwaterDepth, EauDispo, Lr: Double);
var
  CorrectedIrrigation: Double;
  CorrectedBundheight: Double;
begin
  try
    Lr := 0;
    CorrectedBundheight := Bundheight;
    // implement lifesaving drainage
    if (LifeSavingDrainage = 1) and
      (FloodwaterDepth > (0.5 * PlantHeight)) and
      (PlantHeight > 0) and
      (NumPhase > 1) and
      (BundHeight > 0) then
    begin
      CorrectedBundheight := 0.5 * PlantHeight;
      Lr := Lr + Max(0, FloodwaterDepth - (0.5 * PlantHeight));
      FloodwaterDepth := Min(FloodwaterDepth, (0.5 * PlantHeight));
      if (FloodwaterDepth + StockMacropores > 0) then
      begin
        EauDispo := FloodwaterDepth + StockMacropores;
      end;
    end;
    // implement terminal drainage
    if (NumPhase > 4) and (NumPhase < 7) and (DAF > PlotDrainageDAF) and
      (BundHeight > 0) then
    begin
      CorrectedBundHeight := 0;
      Lr := Lr + FloodwaterDepth;
      FloodWaterDepth := 0;
      if ((FloodwaterDepth + StockMacropores) > 0) then
      begin
        EauDispo := StockMacropores;
      end
      else
      begin
        EauDispo := Rain;
      end;
    end;
    // define corrected irrigation
    if (Irrigation = NullValue) then
    begin
      CorrectedIrrigation := 0;
    end
    else
    begin
      CorrectedIrrigation := Irrigation;
    end;
    // implement runoff and EauDispo under terminal drainage
    if (CorrectedBundHeight = 0) and (BundHeight <> CorrectedBundHeight) then
    begin
      if ((StockMacropores + FloodwaterDepth) = 0) then
      begin
        EauDispo := Rain + CorrectedIrrigation;
      end
      else
      begin
        StockMacropores := StockMacropores + Rain + CorrectedIrrigation;
        Lr := Lr + Max(0, StockMacropores - VolMacropores);
        StockMacropores := StockMacropores - Max(0, StockMacropores -
          VolMacropores);
        EauDispo := StockMacropores;
      end;
    end;
    // implement classical upland runoff (SARRAH)
    if (BundHeight = 0) then
    begin
      if (Rain > SuilRuiss) then
      begin
        Lr := Lr + (Rain + CorrectedIrrigation - SeuilRuiss) * PourcRuiss / 100;
        EauDispo := Rain + CorrectedIrrigation - Lr;
      end
      else
      begin
        EauDispo := Rain + CorrectedIrrigation;
      end;
    end;
    // implement bunded-plot style water ponding and runoff, regular situation w/o drainage
    if (CorrectedBundHeight > 0) then
    begin
      if ((StockMacropores + FloodwaterDepth) = 0) then
      begin
        Lr := Lr + Max((Rain + CorrectedIrrigation - BundHeight -
          VolMacropores), 0);
        EauDispo := Min(Rain + CorrectedIrrigation, BundHeight + VolMacropores);
      end
      else
      begin
        StockMacropores := StockMacropores + Rain + CorrectedIrrigation;
        FloodwaterDepth := FloodwaterDepth + Max(0, StockMacropores -
          VolMacropores);
        StockMacropores := Min(VolMacropores, StockMacropores);
        Lr := Lr + Max(0, FloodwaterDepth - CorrectedBundHeight);
        FloodwaterDepth := Min(FloodwaterDepth, CorrectedBundHeight);
        EauDispo := StockMacropores + FloodwaterDepth;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvalRuiss_FloodDyna_V2', URisocas);
  end;
end;

procedure RS_EvolRempliResRFE_RDE_V2(const NumPhase, RuSurf, EauDispo, RuRac,
  CapaRFE,
  CapaREvap, CapaRDE, StRuMax, PercolationMax, BundHeight,
  EpaisseurSurf, EpaisseurProf, VolMacropores: Double;
  var FloodwaterDepth, StockTotal, StockRac, Hum, StockSurface, Dr,
  ValRDE, ValRFE, ValRSurf, FloodwaterGain, StockMacropores: Double);
var
  EauReste, ValRSurfPrec, EauTranspi: Double;
begin
  try
    Dr := 0;
    EauTranspi := 0;
    if (StockMacropores + FloodwaterDepth = 0) then
    begin
      EauReste := 0;
      ValRFE := ValRFE + EauDispo;
      if (ValRFE > CapaRFE) then
      begin
        EauReste := ValRFE - CapaRFE;
        ValRFE := CapaRFE;
      end;
      ValRSurfPrec := ValRSurf;
      ValRSurf := ValRSurf + EauReste;
      if (ValRSurfPrec < CapaREvap) then
      begin
        EauTranspi := EauDispo - (Min(CapaREvap, ValRSurf) - ValRSurfPrec);
      end
      else
      begin
        EauTranspi := EauDispo;
      end;
      if (ValRSurf > (CapaREvap + CapaRDE)) then
      begin
        ValRSurf := CapaREvap + CapaRDE;
        ValRDE := CapaRDE;
      end
      else
      begin
        if (ValRSurf <= CapaREvap) then
        begin
          ValRDE := 0;
        end
        else
        begin
          ValRDE := ValRSurf - CapaREvap;
        end;
      end;
      StockSurface := ValRFE + ValRDE;
      StockTotal := StockTotal + EauTranspi;
      if (StockTotal > StRuMax) then
      begin
        Dr := StockTotal - StRuMax;
        StockTotal := StRuMax;
      end
      else
      begin
        Dr := 0;
      end;
      if Hum < (CapaRFE + CapaRDE) then
      begin
        Hum := StockSurface;
      end
      else
      begin
        Hum := Max(Hum, StockTotal);
      end;
    end;
    StockRac := Min(StockRac + EauTranspi, RuRac);
    // Shifting non-percolating Dr back to macropores & floodwater if plot is bunded
    if (BundHeight > 0) then
    begin
      // Shifting non-percolating Dr to Floodwater
      StockMacropores := StockMacropores + Max(0, Dr - PercolationMax);
      Dr := Min(Dr, PercolationMax);
      if (StockMacropores > VolMacropores) then
      begin
        FloodWaterDepth := FloodWaterDepth + (StockMacropores - VolMacropores);
        StockMacropores := VolMacropores;
      end;
      // Implementing Dr
      if (FloodwaterDepth >= PercolationMax) then
      begin
        Dr := PercolationMax;
        FloodwaterDepth := FloodwaterDepth - Dr;
        StockMacropores := VolMacropores;
      end
      else
      begin
        if (FloodwaterDepth < PercolationMax) and ((FloodwaterDepth +
          StockMacropores) >= PercolationMax) then
        begin
          Dr := PercolationMax;
          FloodwaterDepth := FloodwaterDepth - Dr;
          StockMacropores := StockMacropores + FloodwaterDepth;
          FloodwaterDepth := 0;
        end
        else
        begin
          Dr := Min(PercolationMax, (FloodwaterDepth + StockMacropores + Dr));
          FloodwaterDepth := 0;
          StockMacropores := 0;
        end;
      end;
    end;
  except
    AfficheMessageErreur('RS_EvolRempliResRFE_RDE_V2', URisocas);
  end;
end;

procedure RS_AutomaticIrrigation_V2(const NumPhase, IrrigAuto, IrrigAutoTarget,
  BundHeight,
  PlantHeight, Irrigation, PlotDrainageDAF, DAF,
  VolMacropores, VolRelMacropores, Rain
  {NEW Y}
  , FTSWIrrig,
  IrrigAutoStop, IrrigAutoResume, ChangeNurseryStatus,
  PercolationMax, NbJas {, Ftsw }, RuSurf , {Ru}ResUtil, RootFront,
  EpaisseurSurf, EpaisseurProf {NEW JUNE 19}, ProfRacIni: Double;
  var FloodwaterDepth, IrrigAutoDay, IrrigTotDay,
  StockMacropores, EauDispo , RuRac, StockRac, Ftsw {NEW JUNE 18}, Lr : Double);
var
  IrrigAutoTargetCor: Double;
  CorrectedIrrigation: Double;
  CorrectedBundHeight: Double;
  StressPeriod : Double;
begin
  try
    CorrectedBundHeight := BundHeight;
    {StressPeriod := 0;     }

    if (Irrigation = NullValue) then
    begin
      CorrectedIrrigation := 0;
    end
    else
    begin
      CorrectedIrrigation := Irrigation;
    end;
    if (NumPhase > 4) and (NumPhase < 7) and (DAF > PlotDrainageDAF) then
    begin
      CorrectedBundHeight := 0;
    end;

    {NEW Y}
    if (NbJas >= IrrigAutoStop) and (NbJas < IrrigAutoResume) then
        StressPeriod := 1
    else
        StressPeriod := 0;
	{/NEW Y}
	
	{NEW JUNE 18}
    // Enable interruption of irrigation for user defined period
	If (StressPeriod=1) and (FloodwaterDepth>0) then
    begin
       Lr := Lr + FloodwaterDepth;
       FloodwaterDepth := 0;    
       // Drain off floodwater during user-defined stressperiod
    End;

	
	
    if (NumPhase < 7) and (DAF <= PlotDrainageDaf) and (IrrigAuto = 1) and
      (NumPhase > 0) and (CorrectedBundHeight > 0) {NEW Y} and (Ftsw <= FTSWIrrig) and (StressPeriod = 0){/NEW Y}
       then
    begin

      // FtswIrrig is a management parameter making irrigation conditional on Ftsw

      IrrigAutoTargetCor := Min((IrrigAutoTarget * BundHeight), (0.5 *
        PlantHeight));
      // Provide initial water flush for infiltration
      if (NumPhase = 1) then
      begin
        IrrigAutoTargetCor := Max(IrrigAutoTargetCor, BundHeight / 2);
      end;
      // dimension irrigation on day i to replenish the floodwater, macropores and RuRac
	  {DELETED JUNE 18}
      {IrrigAutoDay := Max(0, (IrrigAutoTargetCor - FloodwaterDepth +
        Min((VolMacropores - StockMacropores) / 2, VolRelMacropores * 200 /
        100)));  // The sense of the last part of this equation is not clear}
	  {NEW JUNE 18}
	  IrrigAutoDay := Max(0,(IrrigAutoTargetCor - FloodwaterDepth)) +
        + (VolMacropores - StockMacropores) + RuRac * (1-(min(Ftsw, 1)));

        // Pre-irrigation at transplanting, in mm
       {NEW Y}
        If ChangeNurseryStatus = 1 then
                IrrigAutoDay := (IrrigAutoTarget * BundHeight) + VolMacropores + RuSurf + PercolationMax;
		{/NEW Y}

      if (StockMacropores + FloodwaterDepth) = 0 then
      begin
        EauDispo := Rain + CorrectedIrrigation + IrrigAutoDay;
      end
      else
      begin
        FloodwaterDepth := FloodwaterDepth + IrrigAutoDay;
        // make sure Macropores is fully filled before floodwater can build up!
        if (VolMacropores > 0) and (StockMacropores < VolMacropores) and
          (FloodwaterDepth > 0) then
        begin
          StockMacropores := StockMacropores + FloodwaterDepth;
          FloodwaterDepth := max(0, StockMacropores - VolMacropores);
          StockMacropores := StockMacropores - FloodwaterDepth;
		  
		  {NEW P}
		  {NEW JUNE 18}
		  {Provision is introduced where RootFront is not evaluated yet, we take the value of ProfRacIni}
		  if RootFront = 0 then
			RuRac := {NEW JUNE 18}{Ru}ResUtil * ProfRacIni / 1000
		  else
			RuRac := {NEW JUNE 18}{Ru}ResUtil * RootFront / 1000;
		  
		  //showMessage(floattostr(ResUtil)+' * '+floattostr(ProfRacIni)+' NbJas:'+floattostr(NbJas)+' NumPhase:'+floattostr(NumPhase));
		  if RootFront = 0 then
			StockRac := RuRac + StockMacropores * ProfRacIni / (EpaisseurSurf + EpaisseurProf)
		  else
			StockRac := RuRac + StockMacropores * RootFront / (EpaisseurSurf + EpaisseurProf);
			
		  Ftsw := StockRac / RuRac;
		
		  {NEW P}
		  
        end;
        EauDispo := StockMacropores + FloodwaterDepth;
      end;
    end
    else
    begin
      if (NumPhase < 7) and (DAF <= PlotDrainageDaf) and (IrrigAuto = 1) and
        (NumPhase > 0) and (CorrectedBundHeight = 0) then
      begin
        FloodwaterDepth := 0;
        {DELETED JUNE 18}{StockMacropores := 0;}
      end;
    end;
    IrrigTotDay := CorrectedIrrigation + IrrigAutoDay;
  except
    AfficheMessageErreur('RS_AutomaticIrrigation_V2', URisocas);
  end;
end;

procedure RS_FloodwaterDynamic_V2(const FloodwaterDepth, PlantHeight: Double;
  var FractionPlantHeightSubmer: Double);
begin
  try
    if (PlantHeight > 0) then
    begin
      FractionPlantHeightSubmer := Min(Max(0, FloodwaterDepth / PlantHeight),
        1);
    end;
  except
    AfficheMessageErreur('RS_FloodwaterDynamic_V2', URisocas);
  end;
end;

procedure RS_EvolRurRFE_RDE_V2(const VitesseRacinaire, Hum, ResUtil,
  StockSurface, RuSurf, ProfRacIni,
  EpaisseurSurf, EpaisseurProf, ValRDE, ValRFE, NumPhase, ChangePhase,
  FloodwaterDepth, StockMacropores, {NEW Y}RootFrontMax , ChangeNurseryStatus,
  Transplanting, TransplantingDepth {/NEW Y} : Double;
  var RuRac, StockRac, StockTotal, FloodwaterGain,
  RootFront: Double);
var
  DeltaRur: Double;
begin
  try
    if (NumPhase = 0) then
    begin
      RuRac := 0;
      StockRac := 0;
    end
    else
    begin
      if ((NumPhase = 1) and (ChangePhase = 1)) then
        // les conditions de germination sont atteinte et nous sommes le jour même
      begin
        RuRac := ResUtil * Min(ProfRacIni, (EpaisseurSurf + EpaisseurProf)) /
          1000;
        if (ProfRacIni <= EpaisseurSurf) then
        begin
          StockRac := (ValRDE + ValRFE) * ProfRacIni / EpaisseurSurf;
        end
        else
        begin  
          StockRac := StockTotal * Min(ProfRacIni / (EpaisseurSurf +
            EpaisseurProf), 1);
        end;
      end
      else
      begin
        if (Hum - StockMacropores - RuRac) < (VitesseRacinaire / 1000 * ResUtil)
          then
        begin
          DeltaRur := Max(0, Hum - StockMacropores - RuRac);
		  
		  {NEW P}
		  If (RootFront >= (EpaisseurSurf + EpaisseurProf)) or (RootFront >= RootFrontMax) then 
			DeltaRur := 0; 
		  // limit root front progression to RootFrontMax and soil depth
		  {/NEW P}
		  
        end
        else
        begin
          DeltaRur := VitesseRacinaire / 1000 * ResUtil;
		
		  {NEW Y}
          If {NEW P}(RootFront >= (EpaisseurSurf + EpaisseurProf)) or {/NEW P}(RootFront >= RootFrontMax) then 
			DeltaRur := 0; 
		  // limit root front progression to RootFrontMax and soil depth
		  {/NEW Y}
		  
        end;
        RuRac := RuRac + DeltaRur;
        if (RuRac > RuSurf) then
        begin
          StockRac := StockRac + DeltaRur;
        end
        else
        begin
          StockRac := (ValRDE + ValRFE) * (RuRac / RuSurf);
        end;
      end;
    end;
    // The following is needed to have the correct basis for calculating FTSW under
    // supersaturated soil condition (macropores filled)
    if (NumPhase <> 0) then
    begin
      RootFront := ((1 / ResUtil) * RuRac) * 1000; 
	  
	  {NEW Y}
	  if(ChangeNurseryStatus = 1) and (Transplanting = 1) then
	  begin
		RootFront := TransplantingDepth;
		
		if (RootFront < 40) then
			RootFront := 40
		else if (RootFront > 200) then
			RootFront := 200;
		
		// Security: avoid aberrant values for transplanting depth
	  
		// set new root front to depth of transplanting
		RuRac := RootFront * ResUtil / 1000;
		
	  end
	  {/NEW Y}
	  
	  
    end;
    if ((StockMacropores + FloodwaterDepth) > 0) then
    begin
      StockRac := RootFront * ResUtil / 1000 + (RootFront / (EpaisseurSurf +
        EpaisseurProf)) * StockMacropores;
      StockRac := Min(StockRac, StockTotal);
    end;
  except
    AfficheMessageErreur('RS_EvolRurRFE_RDE_V2', URisocas);
  end;
end;

procedure RS_PlantSubmergence_V2(const PlantHeight, FloodwaterDepth: Double;
  var FractionPlantHeightSubmer: Double);
begin
  try
    FractionPlantHeightSubmer := Min(Max(0, FloodwaterDepth / Max(PlantHeight,
      0.1)), 1);
  except
    AfficheMessageErreur('RS_PlantSubmergence_V2', URisocas);
  end;
end;

procedure RS_ExcessAssimilToRoot_V2(const NumPhase, ExcessAssimToRoot,
  DryMatStructRootPop, RootSystVolPop,
  CoeffRootMassPerVolMax: Double;
  var RootMassPerVol, GrowthStructRootPop, AssimNotUsed: Double);
begin
  try
    if (NumPhase > 1) then
    begin
      RootMassPerVol := DryMatStructRootPop / RootSystVolPop;
    end;
    if (ExcessAssimToRoot = 1) then
    begin
      if (NumPhase < 5) and (NumPhase > 1) and (AssimNotUsed > 0) then
      begin
        if (RootMassPerVol < CoeffRootMassPerVolMax) then
        begin
          GrowthStructRootPop := GrowthStructRootPop + AssimNotUsed;
          AssimNotUsed := 0;
        end;
      end;
    end;
  except
    AfficheMessageErreur('RS_ExcessAssimilToRoot_V2', URisocas);
  end;
end;

procedure RS_EvolRempliMacropores_V2(const NumPhase, EpaisseurSurf,
  EpaisseurProf, ResUtil, StockMacropores,
  RootFront, CapaRDE, CapaRFE, FloodwaterDepth: Double;
  var StockTotal, Hum, StockSurface, StockRac, ValRDE,
  ValRFE, ValRSurf: Double);
begin
  try
    if ((StockMacropores + FloodwaterDepth) > 0) then
    begin
      StockTotal := (EpaisseurSurf + EpaisseurProf) * ResUtil / 1000 +
        StockMacropores;
      Hum := StockTotal;
      StockSurface := EpaisseurSurf * ResUtil / 1000 + (EpaisseurSurf /
        (EpaisseurSurf + EpaisseurProf)) * StockMacropores;
      StockRac := RootFront * ResUtil / 1000 + (RootFront / (EpaisseurSurf +
        EpaisseurProf)) * StockMacropores;
      ValRDE := CapaRDE;
      ValRFE := CapaRFE;
      ValRSurf := EpaisseurSurf * ResUtil / 1000;
    end;
  except
    AfficheMessageErreur('RS_EvolRempliMacropores_V2', URisocas);
  end;
end;

procedure RS_EvalFTSW_V2(const RuRac, StockTotal, StockMacropores,
  StRuMax: Double;
  var StockRac, ftsw: Double);
begin
  try
    StockRac := Min(StockRac, (RuRac + (StockMacropores * RuRac / StRuMax)));
    StockRac := Min(StockRac, StockTotal);
    if (RuRac > 0) then
    begin
      ftsw := StockRac / RuRac;
    end
    else
    begin
      ftsw := 0;
    end;
  except
    AfficheMessageErreur('EvalFTSW | StRurMax: ' + FloatToStr(RuRac) + ' StRur: '
      + FloatToStr(StockRac) + ' ftsw: ' + FloatToStr(ftsw), URisocas);
  end;
end;

procedure RS_EvalDAF_V2(const NumPhase: Double;
  var DAF: Double);
begin
  try
    if (NumPhase > 4) then
    begin
      DAF := DAF + 1;
    end
    else
    begin
      DAF := DAF;
    end;
  except
    AfficheMessageErreur('RS_EvalDAF_V2', URisocas);
  end;
end;

procedure RS_EvalSimStartGermin(const NumPhase, ChangePhase, NbJas: Double;
  var SimStartGermin: Double);
begin
  try
    if (NumPhase = 1) and (ChangePhase = 1) then
    begin
      SimStartGermin := NbJas
    end;
  except
    AfficheMessageErreur('RS_EvalSimStartGermin', URisocas);
  end;
end;

procedure RS_EvalSimEmergence(const NumPhase, ChangePhase, NbJas: Double;
  var SimEmergence: Double);
begin
  try
    if (NumPhase = 2) and (ChangePhase = 1) then
    begin
      SimEmergence := NbJas
    end;
  except
    AfficheMessageErreur('RS_EvalSimEmergence', URisocas);
  end;
end;

procedure RS_EvalSimStartPSP(const NumPhase, ChangePhase, NbJas: Double;
  var SimStartPSP: Double);
begin
  try
    if (NumPhase = 3) and (ChangePhase = 1) then
    begin
      SimStartPSP := NbJas
    end;
  except
    AfficheMessageErreur('RS_EvalSimStartPSP', URisocas);
  end;
end;

procedure RS_EvalSimPanIni(const NumPhase, ChangePhase, NbJas: Double;
  var SimPanIni: Double);
begin
  try
    if (NumPhase = 4) and (ChangePhase = 1) then
    begin
      SimPanIni := NbJas
    end;
  except
    AfficheMessageErreur('RS_EvalSimPanIni', URisocas);
  end;
end;

procedure RS_EvalSimAnthesis50(const NumPhase, ChangePhase, NbJas: Double;
  var SimAnthesis50: Double);
begin
  try
    if (NumPhase = 5) and (ChangePhase = 1) then
    begin
      SimAnthesis50 := NbJas
    end;
  except
    AfficheMessageErreur('RS_EvalSimAnthesis50', URisocas);
  end;
end;

procedure RS_EvalSimStartMatu2(const NumPhase, ChangePhase, NbJas: Double;
  var SimStartMatu2: Double);
begin
  try
    if (NumPhase = 6) and (ChangePhase = 1) then
    begin
      SimStartMatu2 := NbJas
    end;
  except
    AfficheMessageErreur('RS_EvalSimStartMatu2', URisocas);
  end;
end;

procedure RS_EvalSimEndCycle(const NumPhase, ChangePhase, NbJas: Double;
  var SimEndCycle: Double);

begin
  try
    if (NumPhase = 7) and (ChangePhase = 1) then
    begin
      SimEndCycle := NbJas;
	
    end;
  except
    AfficheMessageErreur('RS_EvalSimEndCycle ', URisocas);
  end;
end;

procedure RS_EvalColdStress(const KCritStressCold1, KCritStressCold2, TMin:
  Double;
  var StressCold: Double);
begin
  try
    StressCold := 1 - Max(0, Min(1, KCritStressCold1 / (KCritStressCold1 -
      KCritStressCold2) - TMin / (KCritStressCold1 - KCritStressCold2)));
    StressCold := Max(0.00001, StressCold);
  except
    AfficheMessageErreur('RS_EvalColdStress', URisocas);
  end;
end;

procedure RS_EvalAssim(const AssimPot, CstrAssim: Double; var Assim: Double);
begin
  try
    Assim := Max(AssimPot * CstrAssim, 0);
  except
    AfficheMessageErreur('EvalAssim | AssimPot: ' + FloatToStr(AssimPot) +
      ' CstrAssim: ' + FloatToStr(CstrAssim) + ' StressCold: ', URisocas);
  end;
end;

procedure RS_Priority2GrowthPanStrctPop(const PriorityPan, DemStructPaniclePop , {NEW LB} NumPhase,  GrowthStructTotPop,
  DemStructInternodePop, DemStructTotPop, DemStructLeafPop, DemStructSheathPop, DemStructRootPop, DemResInternodePop  :
  Double;
  var GrowthStructPaniclePop, GrowthStructInternodePop {NEW LB}  , GrowthStructLeafPop, GrowthStructSheathPop,
		GrowthStructRootPop, GrowthResInternodePop: Double);
var
  GrowthPanDeficit: Double;
  GrowthStructPaniclePlus : Double;
begin
  try
    if (GrowthStructPaniclePop < DemStructPaniclePop) {NEW LB} and (NumPhase = 4){NEW LB} then
    begin
      GrowthPanDeficit := DemStructPaniclePop - GrowthStructPaniclePop;
	  {NEW LB}
	  GrowthStructPaniclePlus := Min(PriorityPan * GrowthPanDeficit, GrowthStructTotPop - GrowthStructPaniclePop);
	  {/NEW LB}
      GrowthStructPaniclePop := GrowthStructPaniclePop {NEW LB}+ GrowthStructPaniclePlus;
      GrowthStructInternodePop := GrowthStructInternodePop - GrowthStructPaniclePlus * (DemStructInternodePop / DemStructTotPop); 
      GrowthStructLeafPop := GrowthStructLeafPop - GrowthStructPaniclePlus * (DemStructLeafPop / DemStructTotPop);
      GrowthStructSheathPop := GrowthStructSheathPop - GrowthStructPaniclePlus * (DemStructSheathPop / DemStructTotPop);
      GrowthStructRootPop := GrowthStructRootPop - GrowthStructPaniclePlus * (DemStructRootPop / DemStructTotPop);
      GrowthResInternodePop := GrowthResInternodePop - GrowthStructPaniclePlus * (DemResInternodePop / DemStructTotPop);
		 {/NEW LB}
		
		
      {DELETED LB}
	  {GrowthStructInternodePop := Max(0, GrowthStructInternodePop - PriorityPan
        * GrowthPanDeficit);}
    end;
  except
    AfficheMessageErreur('RS_Priority2GrowthPanStrctPop', URisocas);
  end;
end;

procedure RS_KeyResults_V2(const NumPhase, CulmsPerPlant, CulmsPerHill, Cstr,
  FTSW, Ic, Lai,
  GrainYieldPop, DryMatAboveGroundPop, DryMatResInternodePop ,{NEW LB} DryMatTotPop , GrainFillingStatus , SterilityTot , CumIrrig, CumWUse: Double;
  var CulmsPerPlantMax, CulmsPerHillMax, DurPhase1, DurPhase2, DurPhase3,
  DurPhase4, DurPhase5, DurPhase6, CumCstrPhase2, CumCstrPhase3, CumCstrPhase4,
  CumCstrPhase5, CumCstrPhase6, CumFTSWPhase2, CumFTSWPhase3, CumFTSWPhase4,
  CumFTSWPhase5, CumFTSWPhase6, CumIcPhase2, CumIcPhase3, CumIcPhase4,
    CumIcPhase5,
  CumIcPhase6, IcPhase2, IcPhase3, IcPhase4, IcPhase5, IcPhase6, FtswPhase2,
  FtswPhase3, FtswPhase4, FtswPhase5, FtswPhase6, CstrPhase2, CstrPhase3,
    CstrPhase4,
  CstrPhase5, CstrPhase6, DurGermFlow, DurGermMat, LaiFin, CulmsPerHillFin,
  CulmsPerPlantFin, GrainYieldPopFin, DryMatAboveGroundPopFin, ReservePopFin, 
  {NEW LB}
  DryMatTotPopFin , GrainFillingStatusFin , SterilityTotFin, CumIrrigFin, CumWUseFin
  :
    Double);
begin
  try
    if (NumPhase > 1) and (NumPhase < 7) then
    begin
      CulmsPerPlantMax := Max(CulmsPerPlant, CulmsPerPlantMax);
      CulmsPerHillMax := Max(CulmsPerHill, CulmsPerHillMax);
    end;
    if (NumPhase = 1) then
    begin
      DurPhase1 := DurPhase1 + 1;
    end;
    if (NumPhase = 2) then
    begin
      DurPhase2 := DurPhase2 + 1;
      CumCstrPhase2 := CumCstrPhase2 + Cstr;
      CumFTSWPhase2 := CumFTSWPhase2 + FTSW;
      CumIcPhase2 := CumIcPhase2 + Ic;
    end;
    if (NumPhase = 3) then
    begin
      DurPhase3 := DurPhase3 + 1;
      CumCstrPhase3 := CumCstrPhase3 + Cstr;
      CumFTSWPhase3 := CumFTSWPhase3 + FTSW;
      CumIcPhase3 := CumIcPhase3 + Ic;
    end;
    if (NumPhase = 4) then
    begin
      DurPhase4 := DurPhase4 + 1;
      CumCstrPhase4 := CumCstrPhase4 + Cstr;
      CumFTSWPhase4 := CumFTSWPhase4 + FTSW;
      CumIcPhase4 := CumIcPhase4 + Ic;
    end;
    if (NumPhase = 5) then
    begin
      DurPhase5 := DurPhase5 + 1;
      CumCstrPhase5 := CumCstrPhase5 + Cstr;
      CumFTSWPhase5 := CumFTSWPhase5 + FTSW;
      CumIcPhase5 := CumIcPhase5 + Ic;
    end;
    if (NumPhase = 6) then
    begin
      DurPhase6 := DurPhase6 + 1;
      CumCstrPhase6 := CumCstrPhase6 + Cstr;
      CumFTSWPhase6 := CumFTSWPhase6 + FTSW;
      CumIcPhase6 := CumIcPhase6 + Ic;
    end;
    if (NumPhase = 7) then
    begin
      IcPhase2 := CumIcPhase2 / Max(DurPhase2, 0.1);
      IcPhase3 := CumIcPhase3 / Max(DurPhase3, 0.1);
      IcPhase4 := CumIcPhase4 / Max(DurPhase4, 0.1);
      IcPhase5 := CumIcPhase5 / Max(DurPhase5, 0.1);
      IcPhase6 := CumIcPhase6 / Max(DurPhase6, 0.1);
      FtswPhase2 := CumFtswPhase2 / Max(DurPhase2, 0.1);
      FtswPhase3 := CumFtswPhase3 / Max(DurPhase3, 0.1);
      FtswPhase4 := CumFtswPhase4 / Max(DurPhase4, 0.1);
      FtswPhase5 := CumFtswPhase5 / Max(DurPhase5, 0.1);
      FtswPhase6 := CumFtswPhase6 / Max(DurPhase6, 0.1);
      CstrPhase2 := CumCstrPhase2 / Max(DurPhase2, 0.1);
      CstrPhase3 := CumCstrPhase3 / Max(DurPhase3, 0.1);
      CstrPhase4 := CumCstrPhase4 / Max(DurPhase4, 0.1);
      CstrPhase5 := CumCstrPhase5 / Max(DurPhase5, 0.1);
      CstrPhase6 := CumCstrPhase6 / Max(DurPhase6, 0.1);
      DurGermFlow := DurPhase2 + DurPhase3 + DurPhase4;
      DurGermMat := DurPhase2 + DurPhase3 + DurPhase4 + DurPhase5 + DurPhase6;
      LaiFin := Lai;
      CulmsPerHillFin := CulmsPerHill;
      CulmsPerPlantFin := CulmsPerPlant;
      GrainYieldPopFin := GrainYieldPop;
      DryMatAboveGroundPopFin := DryMatAboveGroundPop;
      ReservePopFin := DryMatResInternodePop;
	  
	  {NEW LB}
		DryMatTotPopFin := DryMatTotPop;
		GrainFillingStatusFin := GrainFillingStatus;
		SterilityTotFin := SterilityTot;
		CumIrrigFin := CumIrrig;
		CumWUseFin := CumWUse;

	  {/NEW LB}
	  
    end;
  except
    AfficheMessageErreur('RS_KeyResults_V2', URisocas);
  end;
end;

procedure RS_EvolWaterLoggingUpland_V2(const PercolationMax, BundHeight,
  VolMacropores: Double;
  var Dr, Lr, StockMacropores: Double);
begin
  try
    if (Dr > PercolationMax) and (BundHeight = 0) then
    begin
      StockMacropores := StockMacropores + (Dr - PercolationMax);
      Lr := Lr + Max(0, StockMacropores - VolMacropores);
      Dr := PercolationMax;
      StockMacropores := Min(StockMacropores, VolMacropores);
    end;
  except
    AfficheMessageErreur('RS_EvolWaterLoggingUpland_V2', URisocas);
  end;
end;

procedure RS_EvalStressWaterLogging_V2(const StockMacropores, VolMacropores,
  RootFront,
  EpaisseurSurf, EpaisseurProf, WaterLoggingSens: Double;
  var FractionRootsLogged, CoeffStressLogging: Double);
begin
  try
    if (StockMacropores > 0) and (RootFront > 0) then
    begin
      FractionRootsLogged := (Max(0, RootFront - ((VolMacropores -
        StockMacropores) / VolMacropores) * (EpaisseurSurf + EpaisseurProf))) /
        RootFront;
      CoeffStressLogging := 1 - (FractionRootsLogged * Min(1,
        WaterLoggingSens));
    end;
  except
    AfficheMessageErreur('RS_EvalStressWaterLogging_V2', URisocas);
  end;
end;

procedure RS_EvalCstrPFactorFAO_V2(const PFactor, FTSW, ETo, KcTot,
  StockMacropores,
  CoeffStressLogging: Double;
  var cstr: Double);
var
  pFact: Extended;
begin
  try
    pFact := PFactor + 0.04 * (5 - KcTot * ETo);
    pFact := Max(0, pFact);
    pFact := Min(0.8, pFact);
    cstr := Min((FTSW / (1 - pFact)), 1);
    cstr := Max(0, cstr);
    if (StockMacropores > 0) then
    begin
      cstr := cstr * CoeffStressLogging;
    end;
  except
    AfficheMessageErreur('RS_EvalCstrPFactorFAO_V2', URisocas);
  end;
end;


procedure RS_BindAgronomicResults(const numphase: double);
var
part_1 : TextFile;
part_2 : TextFile;
complete_file : TextFile;
content_of_1 : string;
content_of_2 : string;

begin
  try
	
	if fileexists('D:\Projets Cirad\SarraH\DBEcosys\DBResult\Requetes\Agronomic results short.txt') then
	   begin
			AssignFile(part_1,'D:\Projets Cirad\SarraH\DBEcosys\DBResult\Requetes\Agronomic results short.txt');
			AssignFile(part_2,'D:\Projets Cirad\SarraH\DBEcosys\DBResult\Requetes\Agronomic results short part 2.txt');
			AssignFile(complete_file ,'D:\Projets Cirad\SarraH\DBEcosys\DBResult\Requetes\Agronomic results short FULL.txt');

			reset(part_1);
			reset(part_2);
			reWrite(complete_file);

			while not eof(part_1) do
			begin
					readln(part_1, content_of_1);
					readln(part_2, content_of_2);
					writeLn(complete_file, 'test');//content_of_1 + content_of_2);
			end;
			
			closeFile(part_1);
			closeFile(part_2);
			closeFile(complete_file);
	   end;
	   
  except
    AfficheMessageErreur('RS_BindAgronomicResults', URisocas);
  end;
end;


// -----------------------------------------------------------------------------
// Méthodes dynamiques
// -----------------------------------------------------------------------------

procedure RS_EvalDegresJourVitMoyDyn(var T: TPointeurProcParam);
begin
  RS_EvalDegresJourVitMoy(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9]);
end;

procedure RS_EvalDegresJourVitMoy_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalDegresJourVitMoy_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11]);
end;

procedure RS_PhyllochronDyn(var T: TPointeurProcParam);
begin
  RS_Phyllochron(T[0], T[1], T[2], T[3], T[4], T[5], T[6]);
end;

procedure RS_EvolHauteur_SDJ_cstrDyn(var T: TPointeurProcParam);
begin
  RS_EvolHauteur_SDJ_cstr(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17]);
end;

procedure RS_EvalParIntercepteDyn(var T: TPointeurProcParam);
begin
  RS_EvalParIntercepte(T[0], T[1], T[2] , T[3] , T[4]);
end;

procedure RS_EvalCstrAssimDyn(var T: TPointeurProcParam);
begin
  RS_EvalCstrAssim(T[0], T[1], T[2]);
end;

procedure RS_EvalRespMaintDyn(var T: TPointeurProcParam);
begin
  RS_EvalRespMaint(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10], T[11], T[12], T[13]);
end;

procedure RS_EvolPlantTilNumTotDyn(var T: TPointeurProcParam);
begin
  RS_EvolPlantTilNumTot(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12]);
end;

procedure RS_EvolPlantTilNumTot_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvolPlantTilNumTot_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13]);
end;

procedure RS_EvolPlantLeafNumTotDyn(var T: TPointeurProcParam);
begin
  RS_EvolPlantLeafNumTot(T[0], T[1], T[2], T[3], T[4]);
end;

procedure RS_EvolMobiliTillerDeathDyn(var T: TPointeurProcParam);
begin
  RS_EvolMobiliTillerDeath(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10]);
end;

procedure RS_EvolMobiliTillerDeath_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvolMobiliTillerDeath_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11]);
end;

procedure RS_EvolMobiliLeafDeathDyn(var T: TPointeurProcParam);
begin
  RS_EvolMobiliLeafDeath(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8]);
end;

procedure RS_EvalSupplyTotDyn(var T: TPointeurProcParam);
begin
  RS_EvalSupplyTot(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9], T[10]);
end;

procedure RS_EvalRelPotLeafLengthDyn(var T: TPointeurProcParam);
begin
  RS_EvalRelPotLeafLength(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalDemandStructLeafDyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandStructLeaf(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11]);
end;

procedure RS_EvalDemandStructLeaf_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandStructLeaf_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13]);
end;

procedure RS_EvalDemandStructSheathDyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandStructSheath(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7]);
end;

procedure RS_EvalDemandStructRootDyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandStructRoot(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13], T[14], T[15]);
end;                                                            

procedure RS_EvalDemandStructRoot_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandStructRoot_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18]);
end;

procedure RS_EvalDemandStructInternodeDyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandStructInternode(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7]);
end;

procedure RS_EvalDemandStructIN_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandStructIN_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
  T[8],T[9],T[10],T[11], T[12]);
end;

procedure RS_EvalDemandStructPanicleDyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandStructPanicle(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9]);
end;

procedure RS_EvalDemandStructPanicle_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandStructPanicle_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10]);
end;

procedure RS_EvalDemandTotAndIcPreFlowDyn(var T: TPointeurProcParam);
begin
  RS_EvalDemandTotAndIcPreFlow(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17] , T[18]);
end;

procedure RS_EvolGrowthStructLeafPopDyn(var T: TPointeurProcParam);
begin
  RS_EvolGrowthStructLeafPop(T[0], T[1], T[2], T[3], T[4], T[5], T[6]);
end;

procedure RS_EvolGrowthStructSheathPopDyn(var T: TPointeurProcParam);
begin
  RS_EvolGrowthStructSheathPop(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure RS_EvolGrowthStructRootPopDyn(var T: TPointeurProcParam);
begin
  RS_EvolGrowthStructRootPop(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure RS_EvolGrowthStructINPopDyn(var T: TPointeurProcParam);
begin
  RS_EvolGrowthStructINPop(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7]);
end;

procedure RS_EvolGrowthStructPanPopDyn(var T: TPointeurProcParam);
begin
  RS_EvolGrowthStructPanPop(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure RS_EvolGrowthStructTotDyn(var T: TPointeurProcParam);
begin
  RS_EvolGrowthStructTot(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9], T[10] , T[11], T[12]);
end;

procedure RS_AddResToGrowthStructPopDyn(var T: TPointeurProcParam);
begin
  RS_AddResToGrowthStructPop(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19], T[20], T[21], T[22], T[23]);
end;

procedure RS_EvolDemPanFilPopAndIcPFlowDyn(var T: TPointeurProcParam);
begin
  RS_EvolDemPanFilPopAndIcPFlow(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13], T[14]);
end;

procedure RS_EvolPanicleFilPopDyn(var T: TPointeurProcParam);
begin
  RS_EvolPanicleFilPop(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13], T[14], T[15]);
end;

procedure RS_EvolGrowthReserveInternodeDyn(var T: TPointeurProcParam);
begin
  RS_EvolGrowthReserveInternode(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9], T[10], T[11], T[12] , T[13], T[14]);
end;

procedure RS_EvolGrowthTotDyn(var T: TPointeurProcParam);
begin
  RS_EvolGrowthTot(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9], T[10], T[11] , T[12]);
end;

procedure RS_EvalLaiDyn(var T: TPointeurProcParam);
begin
  RS_EvalLai(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9], T[10], T[11], T[12]);
end;

procedure RS_EvalClumpAndLightInterDyn(var T: TPointeurProcParam);
begin
  RS_EvalClumpAndLightInter(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13]);
end;

procedure RS_EvalClumpAndLightInter_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalClumpAndLightInter_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11]);
end;

procedure RS_EvalSlaMitchDyn(var T: TPointeurProcParam);
begin
  RS_EvalSlaMitch(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10],
    T[11], T[12], T[13], T[14]);
end;

procedure RS_EvalRUEDyn(var T: TPointeurProcParam);
begin
  RS_EvalRUE(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20], T[21], T[22], T[23], T[24], T[25], T[26], T[27], T[28], T[29],
    T[30], T[31] , T[32], T[33], T[34] , T[35] , T[36] , T[37]);
end;

procedure RS_EvolKcpKceBilhyDyn(var T: TPointeurProcParam);
begin
  RS_EvolKcpKceBilhy(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure RS_EvolEvapSurfRFE_RDEDyn(var T: TPointeurProcParam);
begin
  RS_EvolEvapSurfRFE_RDE(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13]);
end;

procedure RS_EvolEvapSurfRFE_RDE_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvolEvapSurfRFE_RDE_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20], T[21], T[22], T[23]);
end;

procedure RS_EvalConversionDyn(var T: TPointeurProcParam);
begin
  RS_EvalConversion(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8]);
end;

procedure RS_EvolPSPMVMDDyn(var T: TPointeurProcParam);
begin
  RS_EvolPSPMVMD(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10]);
end;

procedure RS_EvolSomDegresJourCorDyn(var T: TPointeurProcParam);
begin
  RS_EvolSomDegresJourCor(T[0], T[1], T[2]);
end;

procedure RS_EvalMaximumLaiDyn(var T: TPointeurProcParam);
begin
  RS_EvalMaximumLai(T[0], T[1], T[2], T[3], T[4]);
end;

procedure RS_EvalVitesseRacinaireDyn(var T: TPointeurProcParam);
begin
  RS_EvalVitesseRacinaire(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11]);
end;

procedure RS_EvolDryMatTotDyn(var T: TPointeurProcParam);
begin
  RS_EvolDryMatTot(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20], T[21], T[22], T[23], T[24], T[25], T[26], T[27], T[28], T[29],
    T[30], T[31], T[32], T[33], T[34], T[35], T[36], T[37], T[38], T[39]);
end;

procedure RS_EvolDryMatTot_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvolDryMatTot_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20], T[21], T[22], T[23], T[24], T[25], T[26], T[27], T[28], T[29],
    T[30], T[31], T[32], T[33], T[34], T[35], T[36], T[37], T[38], T[39],
    T[40], T[41], T[42], T[43] , T[44] , T[45] , T[46] , T[47] , T[48]);
end;

procedure RS_InitiationCultureDyn(var T: TPointeurProcParam);
begin
  RS_InitiationCulture(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9], T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20], T[21], T[22], T[23], T[24], T[25], T[26], T[27], T[28], T[29], T[30],
    T[31], T[32], T[33], T[34], T[35], T[36], T[37], T[38], T[39], T[40], T[41],
    T[42], T[43], T[44], T[45], T[46], T[47], T[48], T[49], T[50], T[51], T[52],
    T[53], T[54], T[55], T[56], T[57], T[58], T[59], T[60], T[61], T[62], T[63],
    T[64], T[65], T[66], T[67], T[68], T[69], T[70], T[71], T[72], T[73], T[74],
    T[75], T[76], T[77], T[78], T[79], T[80], T[81], T[82], T[83], T[84], T[85],
    T[86], T[87], T[88], T[89], T[90], T[91], T[92], T[93], T[94], T[95], T[96],
    T[97], T[98]);
end;

procedure RS_InitiationCulture_V2Dyn(var T: TPointeurProcParam);
begin
  RS_InitiationCulture_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13], T[14]);
end;

procedure RS_LeafRollingDyn(var T: TPointeurProcParam);
begin
  RS_LeafRolling(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure RS_EvalRootFrontDyn(var T: TPointeurProcParam);
begin
  RS_EvalRootFront(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalSDJPhase4Dyn(var T: TPointeurProcParam);
begin
  RS_EvalSDJPhase4(T[0], T[1], T[2]);
end;

procedure RS_EvalDateGerminationDyn(var T: TPointeurProcParam);
begin
  RS_EvalDateGermination(T[0], T[1], T[2]);
end;

procedure RS_EvalSterilityDyn(var T: TPointeurProcParam);
begin
  RS_EvalSterility(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10], T[11], T[12], T[13], T[14]);
end;

procedure RS_ResetVariablesToZeroDyn(var T: TPointeurProcParam);
begin
  RS_ResetVariablesToZero(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20], T[21], T[22], T[23], T[24], T[25], T[26], T[27], T[28], T[29],
    T[30], T[31], T[32], T[33], T[34], T[35], T[36], T[37], T[38], T[39],
    T[40], T[41], T[42], T[43], T[44], T[45], T[46], T[47], T[48], T[49], T[50]);
end;

procedure RS_EvalAssimPotDyn(var T: TPointeurProcParam);
begin
  RS_EvalAssimPot(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8] , T[9], T[10], T[11], T[12] , T[13] , T[14] , T[15], T[16]);
end;

procedure RS_InitParcelleDyn(var T: TPointeurProcParam);
begin
  RS_InitParcelle(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20]);
end;

procedure RS_InitParcelle_V2Dyn(var T: TPointeurProcParam);
begin
  RS_InitParcelle_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20], T[21], T[22], T[23], T[24], T[25], T[26], T[27]);
end;

procedure RS_EvalEvapPotDyn(var T: TPointeurProcParam);
begin
  RS_EvalEvapPot(T[0], T[1], T[2]);
end;

procedure RS_Transplanting_V2Dyn(var T: TPointeurProcParam);
begin
  RS_Transplanting_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13], T[14], T[15]);
end;

procedure RS_TransplantingShock_V2Dyn(var T: TPointeurProcParam);
begin
  RS_TransplantingShock_V2(T[0], T[1], T[2]);
end;

procedure RS_EvalRuiss_FloodDyna_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalRuiss_FloodDyna_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16]);
end;

procedure RS_EvolRempliResRFE_RDE_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvolRempliResRFE_RDE_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20], T[21], T[22], T[23]);
end;

procedure RS_AutomaticIrrigation_V2Dyn(var T: TPointeurProcParam);
begin
  RS_AutomaticIrrigation_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15]
    //NEW
    , T[16],T[17],T[18],T[19],
    T[20],T[21],T[22],T[23],
	T[24],T[25],T[26],T[27],
	T[28],T[29],T[30], T[31]);
end;

procedure RS_FloodwaterDynamic_V2Dyn(var T: TPointeurProcParam);
begin
  RS_FloodwaterDynamic_V2(T[0], T[1], T[2]);
end;

procedure RS_EvolRurRFE_RDE_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvolRurRFE_RDE_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
	T[20], T[21] , T[22]);
end;

procedure RS_EvolConsRes_Flood_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvolConsRes_Flood_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8],
    T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18]);
end;

procedure RS_PlantSubmergence_V2Dyn(var T: TPointeurProcParam);
begin
  RS_PlantSubmergence_V2(T[0], T[1], T[2]);
end;

procedure RS_ExcessAssimilToRoot_V2Dyn(var T: TPointeurProcParam);
begin
  RS_ExcessAssimilToRoot_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7]);
end;

procedure RS_EvolRempliMacropores_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvolRempliMacropores_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7],
    T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15]);
end;

procedure RS_EvalFTSW_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalFTSW_V2(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure RS_EvalDAF_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalDAF_V2(T[0], T[1]);
end;

procedure RS_EvalSimStartGerminDyn(var T: TPointeurProcParam);
begin
  RS_EvalSimStartGermin(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalSimEmergenceDyn(var T: TPointeurProcParam);
begin
  RS_EvalSimEmergence(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalSimStartPSPDyn(var T: TPointeurProcParam);
begin
  RS_EvalSimStartPSP(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalSimPanIniDyn(var T: TPointeurProcParam);
begin
  RS_EvalSimPanIni(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalSimAnthesis50Dyn(var T: TPointeurProcParam);
begin
  RS_EvalSimAnthesis50(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalSimStartMatu2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalSimStartMatu2(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalSimEndCycleDyn(var T: TPointeurProcParam);
begin
  RS_EvalSimEndCycle(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalColdStressDyn(var T: TPointeurProcParam);
begin
  RS_EvalColdStress(T[0], T[1], T[2], T[3]);
end;

procedure RS_EvalAssimDyn(var T: TPointeurProcParam);
begin
  RS_EvalAssim(T[0], T[1], T[2]);
end;

procedure RS_Priority2GrowthPanStrctPopDyn(var T: TPointeurProcParam);
begin
  RS_Priority2GrowthPanStrctPop(T[0], T[1], T[2], T[3] , T[4], T[5], T[6], T[7] ,T[8], T[9], T[10], T[11],T[12], T[13], T[14], T[15]);
end;

procedure RS_KeyResults_V2Dyn(var T: TPointeurProcParam);
begin 
  RS_KeyResults_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9],
    T[10], T[11], T[12], T[13], T[14], T[15], T[16], T[17], T[18], T[19],
    T[20], T[21], T[22], T[23], T[24], T[25], T[26], T[27], T[28], T[29],
    T[30], T[31], T[32], T[33], T[34], T[35], T[36], T[37], T[38], T[39],
    T[40], T[41], T[42], T[43], T[44], T[45], T[46], T[47], T[48], T[49],
    T[50], T[51], T[52], T[53], T[54], T[55], T[56], T[57], T[58], T[59],
	T[60], T[61], T[62], T[63], T[64], T[65]);
end;

procedure RS_EvolWaterLoggingUpland_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvolWaterLoggingUpland_V2(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

procedure RS_EvalStressWaterLogging_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalStressWaterLogging_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7]);
end;

procedure RS_EvalCstrPFactorFAO_V2Dyn(var T: TPointeurProcParam);
begin
  RS_EvalCstrPFactorFAO_V2(T[0], T[1], T[2], T[3], T[4], T[5], T[6]);
end;

procedure RS_BindAgronomicResultsDyn(var T: TPointeurProcParam);
begin
  RS_BindAgronomicResults(T[0]);
end;
 


initialization
  TabProc.AjoutProc('RS_EvalDegresJourVitMoy', RS_EvalDegresJourVitMoyDyn);
  TabProc.AjoutProc('RS_EvalDegresJourVitMoy_V2',
    RS_EvalDegresJourVitMoy_V2Dyn);
  TabProc.AjoutProc('RS_Phyllochron', RS_PhyllochronDyn);
  TabProc.AjoutProc('RS_EvolHauteur_SDJ_cstr', RS_EvolHauteur_SDJ_cstrDyn);
  TabProc.AjoutProc('RS_EvalParIntercepte', RS_EvalParIntercepteDyn);
  TabProc.AjoutProc('RS_EvalCstrAssim', RS_EvalCstrAssimDyn);
  TabProc.AjoutProc('RS_EvalRespMaint', RS_EvalRespMaintDyn);
  TabProc.AjoutProc('RS_EvolPlantTilNumTot', RS_EvolPlantTilNumTotDyn);
  TabProc.AjoutProc('RS_EvolPlantTilNumTot_V2', RS_EvolPlantTilNumTot_V2Dyn);
  TabProc.AjoutProc('RS_EvolPlantLeafNumTot', RS_EvolPlantLeafNumTotDyn);
  TabProc.AjoutProc('RS_EvolMobiliTillerDeath', RS_EvolMobiliTillerDeathDyn);
  TabProc.AjoutProc('RS_EvolMobiliTillerDeath_V2',
    RS_EvolMobiliTillerDeath_V2Dyn);
  TabProc.AjoutProc('RS_EvolMobiliLeafDeath', RS_EvolMobiliLeafDeathDyn);
  TabProc.AjoutProc('RS_EvalSupplyTot', RS_EvalSupplyTotDyn);
  TabProc.AjoutProc('RS_EvalRelPotLeafLength', RS_EvalRelPotLeafLengthDyn);
  TabProc.AjoutProc('RS_EvalDemandStructLeaf', RS_EvalDemandStructLeafDyn);
  TabProc.AjoutProc('RS_EvalDemandStructLeaf_V2',
    RS_EvalDemandStructLeaf_V2Dyn);
  TabProc.AjoutProc('RS_EvalDemandStructSheath', RS_EvalDemandStructSheathDyn);
  TabProc.AjoutProc('RS_EvalDemandStructRoot', RS_EvalDemandStructRootDyn);
  TabProc.AjoutProc('RS_EvalDemandStructRoot_V2',
    RS_EvalDemandStructRoot_V2Dyn);
  TabProc.AjoutProc('RS_EvalDemandStructInternode',
    RS_EvalDemandStructInternodeDyn);
  TabProc.AjoutProc('RS_EvalDemandStructIN_V2', RS_EvalDemandStructIN_V2Dyn);
  TabProc.AjoutProc('RS_EvalDemandStructPanicle',
    RS_EvalDemandStructPanicleDyn);
  TabProc.AjoutProc('RS_EvalDemandStructPanicle_V2',
    RS_EvalDemandStructPanicle_V2Dyn);
  TabProc.AjoutProc('RS_EvalDemandTotAndIcPreFlow',
    RS_EvalDemandTotAndIcPreFlowDyn);
  TabProc.AjoutProc('RS_EvolGrowthStructLeafPop',
    RS_EvolGrowthStructLeafPopDyn);
  TabProc.AjoutProc('RS_EvolGrowthStructSheathPop',
    RS_EvolGrowthStructSheathPopDyn);
  TabProc.AjoutProc('RS_EvolGrowthStructRootPop',
    RS_EvolGrowthStructRootPopDyn);
  TabProc.AjoutProc('RS_EvolGrowthStructINPop', RS_EvolGrowthStructINPopDyn);
  TabProc.AjoutProc('RS_EvolGrowthStructPanPop', RS_EvolGrowthStructPanPopDyn);
  TabProc.AjoutProc('RS_EvolGrowthStructTot', RS_EvolGrowthStructTotDyn);
  TabProc.AjoutProc('RS_AddResToGrowthStructPop',
    RS_AddResToGrowthStructPopDyn);
  TabProc.AjoutProc('RS_EvolDemPanFilPopAndIcPFlow',
    RS_EvolDemPanFilPopAndIcPFlowDyn);
  TabProc.AjoutProc('RS_EvolPanicleFilPop', RS_EvolPanicleFilPopDyn);
  TabProc.AjoutProc('RS_EvolGrowthReserveInternode',
    RS_EvolGrowthReserveInternodeDyn);
  TabProc.AjoutProc('RS_EvolGrowthTot', RS_EvolGrowthTotDyn);
  TabProc.AjoutProc('RS_EvalLai', RS_EvalLaiDyn);
  TabProc.AjoutProc('RS_EvalClumpAndLightInter', RS_EvalClumpAndLightInterDyn);
  TabProc.AjoutProc('RS_EvalClumpAndLightInter_V2',
    RS_EvalClumpAndLightInter_V2Dyn);
  TabProc.AjoutProc('RS_EvalSlaMitch', RS_EvalSlaMitchDyn);
  TabProc.AjoutProc('RS_EvalRUE', RS_EvalRUEDyn);
  TabProc.AjoutProc('RS_EvolKcpKceBilhy', RS_EvolKcpKceBilhyDyn);
  TabProc.AjoutProc('RS_EvolEvapSurfRFE_RDE', RS_EvolEvapSurfRFE_RDEDyn);
  TabProc.AjoutProc('RS_EvolEvapSurfRFE_RDE_V2', RS_EvolEvapSurfRFE_RDE_V2Dyn);
  TabProc.AjoutProc('RS_EvalConversion', RS_EvalConversionDyn);
  TabProc.AjoutProc('RS_EvolPSPMVMD', RS_EvolPSPMVMDDyn);
  TabProc.AjoutProc('RS_EvolSomDegresJourCor', RS_EvolSomDegresJourCorDyn);
  TabProc.AjoutProc('RS_EvalMaximumLai', RS_EvalMaximumLaiDyn);
  TabProc.AjoutProc('RS_EvalVitesseRacinaire', RS_EvalVitesseRacinaireDyn);
  TabProc.AjoutProc('RS_EvolDryMatTot', RS_EvolDryMatTotDyn);
  TabProc.AjoutProc('RS_EvolDryMatTot_V2', RS_EvolDryMatTot_V2Dyn);
  TabProc.AjoutProc('RS_InitiationCulture', RS_InitiationCultureDyn);
  TabProc.AjoutProc('RS_LeafRolling', RS_LeafRollingDyn);
  TabProc.AjoutProc('RS_EvalRootFront', RS_EvalRootFrontDyn);
  TabProc.AjoutProc('RS_EvalSDJPhase4', RS_EvalSDJPhase4Dyn);
  TabProc.AjoutProc('RS_EvalDateGermination', RS_EvalDateGerminationDyn);
  TabProc.AjoutProc('RS_EvalSterility', RS_EvalSterilityDyn);
  TabProc.AjoutProc('RS_ResetVariablesToZero', RS_ResetVariablesToZeroDyn);
  TabProc.AjoutProc('RS_EvalAssimPot', RS_EvalAssimPotDyn);
  TabProc.AjoutProc('RS_InitParcelle', RS_InitParcelleDyn);
  TabProc.AjoutProc('RS_InitParcelle_V2', RS_InitParcelle_V2Dyn);
  TabProc.AjoutProc('RS_EvalEvapPot', RS_EvalEvapPotDyn);
  TabProc.AjoutProc('RS_Transplanting_V2', RS_Transplanting_V2Dyn);
  TabProc.AjoutProc('RS_TransplantingShock_V2', RS_TransplantingShock_V2Dyn);
  TabProc.AjoutProc('RS_EvalRuiss_FloodDyna_V2', RS_EvalRuiss_FloodDyna_V2Dyn);
  TabProc.AjoutProc('RS_EvolRempliResRFE_RDE_V2',
    RS_EvolRempliResRFE_RDE_V2Dyn);
  TabProc.AjoutProc('RS_AutomaticIrrigation_V2', RS_AutomaticIrrigation_V2Dyn);
  TabProc.AjoutProc('RS_FloodwaterDynamic_V2', RS_FloodwaterDynamic_V2Dyn);
  TabProc.AjoutProc('RS_EvolRurRFE_RDE_V2', RS_EvolRurRFE_RDE_V2Dyn);
  TabProc.AjoutProc('RS_EvolConsRes_Flood_V2', RS_EvolConsRes_Flood_V2Dyn);
  TabProc.AjoutProc('RS_PlantSubmergence_V2', RS_PlantSubmergence_V2Dyn);
  TabProc.AjoutProc('RS_ExcessAssimilToRoot_V2', RS_ExcessAssimilToRoot_V2Dyn);
  TabProc.AjoutProc('RS_EvolRempliMacropores_V2',
    RS_EvolRempliMacropores_V2Dyn);
  TabProc.AjoutProc('RS_EvalFTSW_V2', RS_EvalFTSW_V2Dyn);
  TabProc.AjoutProc('RS_InitiationCulture_V2', RS_InitiationCulture_V2Dyn);
  TabProc.AjoutProc('RS_EvalDAF_V2', RS_EvalDAF_V2Dyn);
  TabProc.AjoutProc('RS_EvalSimStartGermin', RS_EvalSimStartGerminDyn);
  TabProc.AjoutProc('RS_EvalSimEmergence', RS_EvalSimEmergenceDyn);
  TabProc.AjoutProc('RS_EvalSimStartPSP', RS_EvalSimStartPSPDyn);
  TabProc.AjoutProc('RS_EvalSimPanIni', RS_EvalSimPanIniDyn);
  TabProc.AjoutProc('RS_EvalSimAnthesis50', RS_EvalSimAnthesis50Dyn);
  TabProc.AjoutProc('RS_EvalSimStartMatu2', RS_EvalSimStartMatu2Dyn);
  TabProc.AjoutProc('RS_EvalSimEndCycle', RS_EvalSimEndCycleDyn);
  TabProc.AjoutProc('RS_EvalColdStress', RS_EvalColdStressDyn);
  TabProc.AjoutProc('RS_EvalAssim', RS_EvalAssimDyn);
  TabProc.AjoutProc('RS_Priority2GrowthPanStrctPop',
    RS_Priority2GrowthPanStrctPopDyn);
  TabProc.AjoutProc('RS_KeyResults_V2', RS_KeyResults_V2Dyn);
  TabProc.AjoutProc('RS_EvolWaterLoggingUpland_V2',
    RS_EvolWaterLoggingUpland_V2Dyn);
  TabProc.AjoutProc('RS_EvalStressWaterLogging_V2',
    RS_EvalStressWaterLogging_V2Dyn);
  TabProc.AjoutProc('RS_EvalCstrPFactorFAO_V2', RS_EvalCstrPFactorFAO_V2Dyn);
  TabProc.AjoutProc('RS_BindAgronomicResults',  RS_BindAgronomicResultsDyn );

  
  
end.

