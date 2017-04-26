Unit BilEau;

interface
Uses Dialogs,SysUtils,Math;



//ok
    Procedure EvalRunOffScale (Const Rain, Irrig, seuilRuiss, PourcRuiss : Double;
                               Var EauDispo, Lr : Double);
//ok
    Procedure RempliRes(Const Ru, ProfRu, RuSurf, EauDispo, StRurMax : Double;
                        Var stRu, stRUR, Hum, stRuSurf, Dr :Double) ;
Procedure RempliResDr_VN(Const VitDr,Ru, ProfRu, RuSurf, EauDispo, StRurMax : Double;
                          Var stRu, stRUR, Hum, stRuSurf, Dr :Double) ;
//ok
    Procedure EvalFTSW(Const RuRac,StockRac : Double;
                         Var ftsw : Double);
//ok
    Procedure EvalFESW(Const stRuSurf,RuSurf : Double;
                       Var fesw : Double);
//ok
    Procedure EvolRur(Const Vrac, Hum, Ru, stRuSurf, RuSurf:Double;
                         var  stRurMax, stRur : Double);
//ok
    Procedure EvalKce (Const Mulch, Ltr : Double;
                       Var Kce : Double);
//ok
    Procedure EvalKcTot(Const Kce, Kcp : Double;
                        Var KcTot : Double);
//ok
    Procedure DemandePlante(Const Kcp, ETo : Double;
                            Var TrPot : Double);
//ok
    Procedure DemandePlante_V2_1(Const Kcp, ETo , Ca, CO2SlopeTr: Double;
                            Var TrPot, CoeffCO2Tr : Double);
//ok
    Procedure DemandeSol(Const Etp, Kce : Double;
                   Var evapPot : Double);
//ok
    Procedure EvapRuSurf(Const evapPot, fesw, stRuSurf : Double;
                   Var evap : Double);
//ok
    Procedure SeuilCSTR(Const trPot, ParSeuilCSTR, ftsw : Double;
                   Var tr : Double);
//ok
    Procedure ExpCSTR(Const Par1ExpCSTR, Par2ExpCSTR, ftsw : Double;
                   Var cstr : Double);
//ok
    Procedure KHR(Const ParKhr, ftsw : Double;
                   Var cstr : Double);
//ok
    Procedure CstrEagle(Const Etp, ftsw : Double;
                   Var cstr : Double);
//ok
    Procedure CstrIsohydrique(Const Etp, ftsw, Kexponent : Double;
                   Var cstr : Double);
//ok
    Procedure CstrPFactor(Const Kcp, ParPFact, ftsw, Etp : Double;
                   Var cstr : Double);
//ok
    Procedure EvalTranspi(Const TrPot, cstr : Double;
                   Var Tr : Double);
//ok
    Procedure EtrMinMax(Const trPot, tr, evap, evapPot : Double;
                   Var etr, etm : Double);
//ok
    Procedure ConsoRes(Const etr, RuSurf, etm, stRurMax : Double;
                  Var stRur, stRu, stRuSurf, tr, trPot : Double);
//ok
    Procedure ConsoResSep(Const stRurMax, RuSurf, evap, trPot, evapPot : Double;
                   Var tr, stRur, stRuSurf, stRu, Etr, Etm : Double);

//  Procedure InitPlot(Const StockIni,Ru,ProfRacIni,Profondeur,DateSemis: double; var StSurf,StTot,Ltr,Hum,StRurMax,RuSurf, ProfRU:double);
    Procedure InitPlot(Const StockIniSurf,StockIniProf,Ru,ProfRacIni,EpaisseurSurf,EpaisseurProf,DateSemis: double; var StSurf,StTot,Ltr,Hum,StRurMax,RuSurf, ProfRU:double);
implementation

uses UChaines,ModelsManage,Main,GestionDesErreurs,DBmodule;

//##############################################################################
//-------------------------------------------------------------------------//
// Initialisation des variables de plot
//-------------------------------------------------------------------------//
// Correction VB: Auparavent, les sols étient au nombre de deux. Il y a maintenant un unique sol avec deux profondeurs
// Procedure InitPlot(Const StockIni,Ru,ProfRacIni,Profondeur,DateSemis: double; var StSurf,StTot,Ltr,Hum,StRurMax,RuSurf, ProfRU:double);
procedure InitPlot(const StockIniSurf, StockIniProf, Ru, ProfRacIni, EpaisseurSurf, EpaisseurProf, DateSemis : double; var StSurf, StTot, Ltr, Hum, StRurMax, RuSurf, ProfRU : double);
begin
  try
    StSurf := StockIniSurf;
    Ltr := 1;
    StRurMax := Ru * ProfRacIni / 1000;
    RuSurf := EpaisseurSurf / 1000 * Ru;
    StTot := StockIniSurf + StockIniProf;
    ProfRU := EpaisseurSurf + EpaisseurProf;
    Hum := max(StTot, StRurMax);
  except
    AfficheMessageErreur('InitPlot',UBilEau);
  end;
end;
     {------------------------------------------------}
procedure EvalRunOffScale (const Rain, Irrig, seuilRuiss, PourcRuiss : Double;
                                 var EauDispo, Lr : Double);
var
  EnPlus : Double;
begin
  try
    Lr := 0;
    if (Irrig = nullValue) then
    begin
      Enplus := Rain;
    end
    else
    begin
      Enplus := Rain + Irrig;
    end;
    If (Enplus > SeuilRuiss) then
    begin
      Lr := (Enplus - SeuilRuiss) * PourcRuiss / 100;
    end;
    EauDispo := Enplus - Lr;
  except
    AfficheMessageErreur('EvalRunOffScale',UBilEau);
  end;
end;

     {------------------------------------------------}
procedure EvalRunOffIrrigAuto(const Rain, Irrigation, IrrigAuto, IrrigAutoTarget,
                              MaxIrrig, Precision, Stockrac, Rurac, seuilRuiss,
                              PourcRuiss : Double;
                              var EauDispo, Lr, IrrigTotDay : Double);
var
  EnPlus : Double;
  CorrectedIrrigation : Double;
begin
  try
    IrrigTotDay := 0;
    Lr := 0;
    if (Irrigation = NullValue) then
    begin
      CorrectedIrrigation := 0
    end
    else
    begin
      CorrectedIrrigation := Irrigation;
    end;
    if ((IrrigAuto = 1) and (Rurac > 0)) then
    begin
      //mainForm.memDeroulement.Lines.Add('StockRac : ' + FloatToStr(StockRac));
      //mainForm.memDeroulement.Lines.Add('Rurac : ' + FloatToStr(Rurac));
      //mainForm.memDeroulement.Lines.Add('CorrectedIrrigation : ' + FloatToStr(CorrectedIrrigation));

      if ((Stockrac / Rurac) < IrrigAutoTarget) then
      begin
        IrrigTotDay := (Rurac - Stockrac) * 0.8;
        //IrrigTotDay := Rurac - Stockrac;
        //mainForm.memDeroulement.Lines.Add('IrrigTotDay : ' + FloatToStr(IrrigTotDay));
        IrrigTotDay := Max(0, IrrigTotDay - CorrectedIrrigation);
        //mainForm.memDeroulement.Lines.Add('IrrigTotDay : ' + FloatToStr(IrrigTotDay));
        IrrigTotDay := Min(IrrigTotDay, MaxIrrig);
        //mainForm.memDeroulement.Lines.Add('IrrigTotDay : ' + FloatToStr(IrrigTotDay));
        IrrigTotDay := Round(IrrigTotDay / Precision) * Precision;
        //mainForm.memDeroulement.Lines.Add('IrrigTotDay : ' + FloatToStr(IrrigTotDay));
        //showmessage('stop');
      end;
    end;
    IrrigTotDay := CorrectedIrrigation + IrrigTotDay;
    Enplus := Rain + IrrigTotDay;
    If (Enplus > SeuilRuiss) then
    begin
      Lr := (Enplus - SeuilRuiss) * PourcRuiss / 100;
    end;
    EauDispo := Enplus - Lr;
  except
    AfficheMessageErreur('EvalRunOffScale',UBilEau);
  end;
end;

     {------------------------------------------------}
(*      Procedure CalculRuisold;
{
A modifier
Calcul le ruissellement
1) methode des seuils : le ruissellement est egal a un pourcentage
de la quantite de pluie supérieure au seuil definie
2) methode Orstom(IRD) : calibre au senegal, evalue le ruissellement
sur un sol nu en fonction du type de sol en surface (rugosite...) de la pente
definissant les 3 paramètres. Il tient compte aussi de l'interval de temps
entre 2 evennements pluvieux (jours)
VOIR ajouter une methode reduisant le ruissellement par le biais de
ltr, lors de la croissance de la plante
}
      Var
        EnPlus     : Double;

      Begin
        TRes[Lr]:=0;
        Enplus := TJ[NBJSimul,PP]+TJ[NBJSimul,Irr];
        Case Parcelle.TypeRuiss of
          2 : If Enplus>Parcelle.SeuilRuiss
              then TRes[Lr]:=(Enplus - Parcelle.SeuilRuiss) * Parcelle.PourcRuiss/100;
          3 : Begin
                If Parcelle.CodeRuiss < 3 Then
                Begin
                  TRes[Lr]:= TParamRuiss[Parcelle.CodeRuiss,1]*Enplus+
                             TParamRuiss[Parcelle.CodeRuiss,2]*TRes[Ik]+
                             TParamRuiss[Parcelle.CodeRuiss,3]*TRes[Ik]*Enplus
                             - TParamRuiss[Parcelle.CodeRuiss,4];
                End
                Else
                Begin
                  TRes[Lr]:= Parcelle.RuissPerso[1]*Enplus+
                             Parcelle.RuissPerso[2]*TRes[Ik]+
                             Parcelle.RuissPerso[3]*TRes[Ik]*Enplus
                             - Parcelle.RuissPerso[4];
                End;
                If TRes[Lr] < 0 then TRes[Lr] := 0;
                TRes[ik] := (TRes[ik]+(Enplus - TRes[Lr]))* Exp(-0.5)
              End;
        End;
        TRes[EauDispo]:= Enplus-TRes[Lr];
      End;
*)
     {------------------------------------------------}
procedure RempliRes(const Ru, ProfRu, RuSurf, EauDispo, StRurMax : Double;
                          var stRu, stRUR, Hum, stRuSurf, Dr : Double) ;
{
Hypotheses :
On a une dynamique de l'eau represente par un remplissage par le haut
et une evolution des tailles de reservoirs quand ce remplissage est sup
a la quantite maximum de la taille en cours (front d'humectation).
Quand on a atteind la taille maximum par remplissage on considere
que c'est du drainage.
A l'interieur d'un reservoir l'eau est repartie de maniere homogene
(peu etre considere valable jusqu'a 2m de profondeur, dixit CB
 d'apres d'autres sources).

3 representation des reservoirs permettant de simuler 3 reservoirs:
1)ensemble des reservoirs en eau, evoluant en profondeur en fonction
 du front d'humectation
2) reservoir de surface (taille fixe)ou s'effectue l'evaporation et une part de la
transpiration en presence de racines
Modif : on a ajoute l'evaporation en dessous du seuil Pf4.2 estime a
la moitie de la RU.
3) reservoir contenant les racines, evoluant en fonction du front racinaire
REMARQUE : Ces reservoirs se chevauche
Au lieu de gerer des profondeurs on gere des stocks d'eau
(stRuMax stock d'eau maxi (RU * prof. Max)}
var
 StRuMax : Double;
 StRuSurfPrec, EauTranspi : Double;
begin
  try
    StRuMax := Ru ;
    StRuMax := StRuMax * ProfRu / 1000;
    // Rempli Res surface
    StRuSurfPrec := StRuSurf;
    StRuSurf:= min(StRuSurf+EauDispo , RuSurf + Rusurf / 10);
    // enleve la qte d'eau remplissant la partie uniquement evaporable
    if (StRuSurfPrec < Rusurf / 10) then
    begin
      EauTranspi := Max (0, EauDispo - (Rusurf / 10 - StRuSurfPrec));
    end
    else
    begin
      EauTranspi := EauDispo;
    end;
    // remplissage des autres réservoirs
    // rempli res en eau, estime drainage, et evolution du front d'humectation
    StRu := StRu + EauTranspi;
    if (StRu > StRuMax) then
    begin
      Dr := StRu - StRuMax;
      StRu := StRuMax;
    end
    else
    begin
      Dr := 0;
    end;
    Hum:= max(Hum, StRu);
    // Rempli res racines
    StRur:= min(StRur + EauTranspi, StRurMax);
    StRur:= min(StRur, StRu);
  except
    AfficheMessageErreur('RempliRes',UBilEau);
  end;
end;



                {----------------------VN--------------------------}
Procedure RempliResDr_VN(Const VitDr,Ru, ProfRu, RuSurf, EauDispo, StRurMax : Double;
                          Var stRu, stRUR, Hum, stRuSurf, Dr :Double) ;
Var
 StRuMax : Double;
 StRuSurfPrec, EauTranspi : double;
Begin
try
  StRuMax := Ru ;
  StRuMax := StRuMax * ProfRu/1000;
  // Rempli Res surface
  StRuSurfPrec := StRuSurf;
  StRuSurf:= Min(StRuSurf+EauDispo , RuSurf+Rusurf/10);
  // enleve la qte d'eau remplissant la partie uniquement evaporable
  If StRuSurfPrec < Rusurf/10 then
      EauTranspi := Max (0,EauDispo - (Rusurf/10 - StRuSurfPrec))
  else EauTranspi := EauDispo;

  // remplissage des autres réservoirs
  // rempli res en eau, estime drainage, et evolution du front d'humectation
  StRu:= StRu + EauTranspi;
  If StRu > StRuMax then
  Begin
    Dr :=Min(StRu - StRuMax,VitDr);

    StRu := StRu-Dr;

  end
  Else Dr:=0;
  Hum:= Max(Hum , StRu);
  // Rempli res racines
  StRur:= Min(StRur + EauTranspi , StRurMax);
  StRur:= Min(StRur , StRu);
except
  AfficheMessageErreur('RempliResVitDr',UBilEau);
end;
End;
///////////////////////////////////////////////////////////////////////////////

procedure EvalFTSW(const RuRac,StockRac : Double;
                   var ftsw : Double);
{
Estimation de la fraction d'eau transpirable, rapporte donc au reservoir
contenant les racines
// Parametres
IN:
RuRac : mm
StockRac : mm
OUT:
ftsw : mm
}
begin
  try
    if (RuRac > 0) then
    begin
      ftsw :=  StockRac / RuRac;
    end
    else
    begin
      ftsw := 0;
    end;
  except
    AfficheMessageErreur('EvalFTSW | StRurMax: '+FloatToStr(RuRac)+' StRur: '+FloatToStr(StockRac)+' ftsw: '+FloatToStr(ftsw),UBilEau);
  end;
end;
     {------------------------------------------------}
procedure EvalFESW(const stRuSurf, RuSurf : Double;
                         var fesw : Double);
{
Estimation de la fraction d'eau evaporable, rapporte donc au reservoir
de surface, RuSurf est le stock d'eau maxi disponible pour la plante
sur ce reservoir
Modif : on considere que pour l'évaporation la moitie de cette
valeur doit etre ajoutée.
// Parametres
IN:
StRusurf : mm
RuSurf : mm
OUT:
fesw : mm
}
begin
  try
    fesw := StRuSurf / (RuSurf + Rusurf / 10);
  except
    AfficheMessageErreur('EvalFESW',UBilEau);
  end;
end;
     {------------------------------------------------}
      Procedure EvolRur(Const Vrac, Hum, Ru, stRuSurf, RuSurf:Double;
                         var  stRurMax, stRur : Double);
{
La vitesse d'enracinement potentielle de la plante peut etre bloque
par manque d'eau en profondeur (Hum). La profondeur d'humectation
est convertie en quantite d'eau maximum equivalente
// Parametres
IN:
Vrac : mm
Hum : mm
StRuSurf : mm
RU : mm/m
RuSurf : mm/m
INOUT:
stRurMax : mm
stRur : mm
}
      Var
         DeltaRur      : Double;

      Begin
try
// on prend une partie si blocage ou toute la nouvelle prof
// avec une quantite d'eau au max.
  If (Hum-StRurMax) < (VRac/1000*RU)
       Then DeltaRur := Hum-StRurMax
       Else DeltaRur := VRac/1000*RU;
  StRurMax:= StRurMax+Deltarur;
// si les racines vont au dela du stock de surface
// on ajoute le deltaRUR
// sinon on prend au prorata de la profondeur et du stock de surface
  if StRurMax > RuSurf
  then  StRur:= StRur + Deltarur
  else  StRur:= max((StRuSurf-Rusurf/10)* (StRurMax / RuSurf),0);
except
  AfficheMessageErreur('EvolRur | RuSurf: '+FloatToStr(RuSurf),UBilEau);
end;
End;

     {------------------------------------------------}
procedure EvolRurCstr(const Vrac, Hum, Ru, stRuSurf, RuSurf, cstr : Double;
                         var  stRurMax, stRur : Double);
{
La vitesse d'enracinement potentielle de la plante peut etre bloque
par manque d'eau en profondeur (Hum). La profondeur d'humectation
est convertie en quantite d'eau maximum equivalente
// Parametres
IN:
Vrac : mm
Hum : mm
StRuSurf : mm
RU : mm/m
RuSurf : mm/m
INOUT:
stRurMax : mm
stRur : mm
}
var
  DeltaRur, DayVrac : Double;
begin
  try
    // on prend une partie si blocage ou toute la nouvelle prof
    // avec une quantite d'eau au max.
    DayVrac := (VRac * min(Cstr + 0.3, 1)) / 1000 * RU;
    if ((Hum - StRurMax) < DayVrac) then
    begin
      DeltaRur := Hum - StRurMax;
    end
    else
    begin
      DeltaRur := DayVrac;
    end;
    StRurMax := StRurMax + Deltarur;
    // si les racines vont au dela du stock de surface
    // on ajoute le deltaRUR
    // sinon on prend au prorata de la profondeur et du stock de surface
    if (StRurMax > RuSurf) then
    begin
      StRur := StRur + Deltarur;
    end
    else
    begin
      StRur := max((StRuSurf - Rusurf / 10) * (StRurMax / RuSurf), 0);
    end;
  except
    AfficheMessageErreur('EvolRur | RuSurf: '+FloatToStr(RuSurf),UBilEau);
  end;
end;

//////////////////////////////////////////////////////////////////////////

procedure EvalKce(const Mulch, Ltr : Double;
                   var Kce : Double);
begin
  try
    Kce := Mulch/100 * ltr;
  except
    AfficheMessageErreur('EvalKce',UBilEau);
  end;
end;

//////////////////////////////////////////////////////////////////////////
      {------------------------------------------------}
procedure EvalKcTot(const Kce, Kcp : Double;
                   var KcTot : Double);
begin
  try
    if (Kcp = Nullvalue) then
    begin
      Kctot := Kce;
    end
    else
    begin
      Kctot := Kce+ Kcp;
    end;
  except
    AfficheMessageErreur('EvalKcTot',UBilEau);
  end;
end;

///////////////////////////////////////////////////////////////////////////

procedure DemandePlante(const Kcp, ETo : Double; var TrPot : Double);
begin
  try
    TrPot := Kcp * ETo;
  except
    AfficheMessageErreur('DemandePlante',UBilEau);
  end;
end;
     {------------------------------------------------}

procedure DemandePlante_V2_1(Const Kcp, ETo , Ca, CO2SlopeTr: Double; Var TrPot, CoeffCO2Tr : Double);
begin
  try
    TrPot := Kcp * ETo;
	CoeffCO2Tr := Ca * CO2SlopeTr - 400 * CO2SlopeTr + 1; // Coefficient for TrPot response to ambient CO2 (Ca), set to 1 for Ca=400ppm (ambient 2013)
    TrPot := TrPot * CoeffCO2Tr;
  except
    AfficheMessageErreur('DemandePlante_V2_1',UBilEau);
  end;
end;
     {------------------------------------------------}

procedure DemandeSol(const Etp, Kce : Double;
                   var evapPot : Double);
{
Estimation de l'evaporation potentielle du sol, on ne tient pas
compte d'une variation de l'evaporation en fonction d'une humectation
differente entre le haut et le bas du reservoir, on a un parametre
mulch qui peu traduire le phenomene d'auto mulching (defaut : 0.7)
qui peu aussi traduire un mulch par couverture vegetale ou...
La reduction de l'evaporation par l'evolution de la couverture
du sol par la plante est traduit par ltr.

// Parametres
IN:
ETo : mm
Kce : %
OUT:
evapPot : mm
}
begin
  try
    EvapPot:= ETP* Kce;
  except
    AfficheMessageErreur('DemandeSol',UBilEau);
  end;
end;
     {------------------------------------------------}
      Procedure EvapRuSurf(Const evapPot, fesw, stRuSurf : Double;
                   Var evap : Double);
{
Estimation de l'evaporation relle, rapporte a la fraction d'eau evaporable
// Parametres
IN:
fesw : mm
evapPot : mm
stRuSurf : mm
OUT:
evap : mm
}
Begin
try
   evap:= EvapPot* fesw * fesw;
   evap:= Min(evap,StRuSurf);
except
  AfficheMessageErreur('EvapRuSurf',UBilEau);
end;
End;

     {------------------------------------------------}
procedure EvapRuSurfFesw(const evapPot, fesw, stRuSurf : Double;
                   Var evap : Double);
{
Estimation de l'evaporation relle, rapporte a la fraction d'eau evaporable
// Parametres
IN:
fesw : mm
evapPot : mm
stRuSurf : mm
OUT:
evap : mm
}
begin
  try
    evap := EvapPot * fesw;
    evap := min(evap, StRuSurf);
  except
    AfficheMessageErreur('EvapRuSurfFesw',UBilEau);
  end;
end;
//////////////////////////////////////////////////////////////////////

{
Ensemble de procedure permettant d'estimer l'indice de contrainte
de consommation d'eau par transpiration, fonction du sol (fesw)
et de la plante
}

Procedure SeuilCSTR(Const trPot, ParSeuilCSTR, ftsw : Double;
             Var tr : Double);
Begin
try
   tr:= Min(trPot,trPot * ParSeuilCSTR * ftsw);
except
  AfficheMessageErreur('SeuilCSTR',UBilEau);
end;
End;
     {------------------------------------------------}
      Procedure ExpCSTR(Const Par1ExpCSTR, Par2ExpCSTR, ftsw : Double;
                   Var cstr : Double);
{
Estimation de l'indice de contrainte de consommation d'eau
Sadras et MilRoy 1996
Lacape 1999
Paramètre specifique espece/variete
// Parametres
IN:
ftsw : mm
Par1ExpCSTR :
Par2ExpCSTR :
OUT:
cstr : %
}
Begin
try
   cstr:= 1/(1+exp(-(ftsw-Par1ExpCSTR)/Par2ExpCSTR));
except
  AfficheMessageErreur('ExpCSTR',UBilEau);
end;
End;
{------------------------------------------------}
Procedure KHR(Const ParKhr, ftsw : Double;
           Var cstr : Double);
Begin
try
  cstr:= Min(1, ParKHR * ftsw* ftsw);
except
  AfficheMessageErreur('KHR',UBilEau);
end;
End;
     {------------------------------------------------}
      Procedure CstrEagle(Const Etp, ftsw : Double;
                   Var cstr : Double);
{
Estimation de l'indice de contrainte de consommation d'eau
// Parametres
IN:
ftsw : mm
ETo : mm
OUT:
cstr : %
}
Begin
try
  cstr :=( (-0.05 + 0.732/ETP)
         +( 4.97 - 0.66 * ETP)*ftsw
         +(-8.57 + 1.56 * ETP)*ftsw*ftsw
         +( 4.35 - 0.88 * ETP)
             * power(ftsw,3) );
  // bornage pour les faibles ETP CSTR > 1 vers ETP < 3 mm/j
  If cstr > 1 Then cstr := 1;
except
  AfficheMessageErreur('KHR',UBilEau);
end;
End;
{------------------------------------------------}
Procedure CstrIsohydrique(Const Etp, ftsw, Kexponent : Double;
           Var cstr : Double);
Begin
try
  Cstr := min(1, 2* ftsw) * Power((1/(Etp+1)),Kexponent);
except
  AfficheMessageErreur('CstrIsohydrique',UBilEau);
end;
end;

{------------------------------------------------}
procedure CstrPFactor(const Kcp, ParPFact, ftsw, Etp : Double; var cstr : Double);
var
  pFact: Extended;
begin
  try
    pFact := ParPFact + 0.04 * (5 - max(Kcp,1) * ETP);
    pFact := max(0.1, pFact);
    pFact := min(0.8, pFact);
    cstr := min((ftsw / (1 - pFact)), 1);
    cstr := max(0, cstr);
  except
    AfficheMessageErreur('CstrPFactor | pFact: '+FloatToStr(pFact),UBilEau);
  end;
end;
{------------------------------------------------}
Procedure CstrPFactorTropDo(Const Kcp, ParPFact, ftsw, Etp,SeuilFTSW : Double; Var cstr : Double);
  {
  Estimation de l'indice de contrainte de consommation d'eau
  Modif :
  // Parametres
  IN:
  Kctot :
  parPfact :
  ftsw : mm
  ETo : mm
  OUT:
  cstr : %
  }
Var
  pFact: Extended;
Begin
try
  { Eagle et trPot= Kc*Etp}
  //        pFact := ParPFact + 0.04 * (5 - Kctot * ETP);
  pFact := ParPFact + 0.04 * (5 - max(Kcp,1) * ETP);
  pFact := Max(0.1, pFact);
  pFact := Min(0.8, pFact);
  cstr := Min((ftsw/(1-pFact)),1);
  cstr := Max (0,cstr);
//  If (cstr = 1) and (ftsw > SeuilFTSW) Then cstr := max (1-ftsw, 0);

except
  AfficheMessageErreur('CstrPFactorTropDo | pFact: '+FloatToStr(pFact),UBilEau);
end;
End;
  {------------------------------------------------}
procedure EvalTranspi(const TrPot, cstr : Double; var Tr : Double);
begin
  try
    Tr := TrPot * cstr;
  except
    AfficheMessageErreur('EvalTranspi',UBilEau);
  end;
end;

  {------------------------------------------------}
Procedure EtrMinMax(Const trPot, tr, evap, evapPot : Double;
           Var etr, etm : Double);
  {
  Non separation de tr et evap on prend la plus grande valeur
  estimee (evapotranspiration du reservoir de surface ou
  evapotranspiration de la plante/sol Kc plante et sol)
  pour definir la valeur de l'etr qui sera consomme sur les reservoirs
  les valeurs tr et evap doivent ici etre comprises comme :
  tr = etr plante sol et evap = etr du reservoir de surface considere
  comme un sol nu (ie ltr = 1) pour pouvoir reste compatible avec
  la version de SARRA
  // Parametres
  IN:
  tr : mm
  trPot : mm
  evap : mm
  evapPot : mm
  OUT:
  etr : mm
  etm : mm
  }
Begin
try
  If tr > evap Then
  Begin
    Etr:=tr;
    Etm := trPot;
  end
  else
  begin
    Etr:=evap;
    Etm := evapPot;
  end;
except
  AfficheMessageErreur('EtrMinMax',UBilEau);
end;
End;
  {------------------------------------------------}
Procedure ConsoRes(Const etr, RuSurf, etm, stRurMax : Double;
           Var stRur, stRu, stRuSurf, tr, trPot : Double);
  {
  Non separation de tr et evap, consommation d'eau sur les reservoirs
  // Parametres
  IN:
  etm : mm
  etr : mm
  RuSurf : mm
  stRurMax :mm
  INOUT:
  stRur : mm
  stRu : mm
  stRuSurf : mm
  Out:
  Tr : mm
  trPot : mm
  }
Begin
try
  StRu:=Max(StRu- Etr,0);
  StRuSurf:=Max(StRuSurf - Etr,0);
  If stRurMax >=  RuSurf
  Then StRur:=Max(StRur- Etr,0)
  else stRur := max((StRuSurf-Rusurf/10)* stRurMax/RuSurf,0) ;
  Tr := ETR;
  TrPot := ETM;
except
  AfficheMessageErreur('ConsoRes',UBilEau);
end;
  End;
  {------------------------------------------------}
procedure ConsoResSep(const stRurMax, RuSurf, evap, trPot, evapPot : Double;
           var tr, stRur, stRuSurf, stRu, Etr, Etm : Double);
{
Separation de tr et evap. Consommation de l'eau sur les reservoirs
Hypothese : l'evaporation est le processus le plus rapide, retranche
en premier sur le reservoir de surface. Comme reservoir de surface
et reservoirs racinaires se chevauchent, il nous faut aussi calcule sur
le reservoir ayant des racines la part deja extraite pour l'evaporation.
Quand la profondeur des racines est inferieur au reservoir de surface
on ne consomme en evaporation que la fraction correspondant a cette
profondeur sur celle du reservoir de surface (consoRur).
Les estimations d'evaporation et de transpirations sont effectues
separemment, on peut ainsi avoir une consommation legerement superieure
a l'eau disponible. On diminuera donc la transpiration en consequence.

Modif : Pour les stock d'eau on tient compte de la partie rajoutee au
reservoir de surface qui ne peut etre que evapore (air dry)
// Parametres
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
var
  ConsoRur, TrSurf : double;
begin
  try
    // part transpirable sur le reservoir de surface
    TrSurf := max(0 ,StRuSurf - Rusurf / 10);
    // qte d'eau evapore a consommer sur le reservoir de surface
    StRuSurf := max(StRuSurf - Evap, 0);
    // qte d'eau evapore a retirer sur la part transpirable
    if (Evap > TrSurf) then
    begin
      ConsoRur:= TrSurf;
    end
    else
    begin
      ConsoRur:=Evap;
    end;
    StRu := Max(0, StRu - ConsoRur);
    // fraction d'eau evapore sur la part transpirable qd les racines sont moins
    // profondes que le reservoir de surface, mise a jour des stocks transpirables
    if (stRurMax <  RuSurf) then
    begin
      ConsoRur := Evap * stRur / RuSurf;
    end;
    StRur := Max(0, StRur - ConsoRur);
    // reajustement de la qte transpirable considerant que l'evap a eu lieu avant
    // mise a jour des stocks transpirables
    if (Tr > StRur) then
    begin
      Tr := Max(StRur - Tr, 0);
    end;
    if (StRur > 0) then
    begin
      StRuSurf := max(StRuSurf - (Tr * min(TrSurf / StRur, 1)), 0);
    end;
    StRur := Max(0, StRur - Tr);
    StRu := Max(0, StRu - Tr);
    Etr := tr + Evap;
    Etm := trPot + Evap;
  except
    AfficheMessageErreur('ConsoResSep',UBilEau);
  end;
end;
////////////////////////////////////////////////////////////////////////////////



Procedure EvalRUE(const ParIntercepte, BiomasseAerienne, ChangePhase, NumPhase: Double;
                  var CumPar, RUE: Double);
begin
try
  if (NumPhase<2) or ((NumPhase=2) and (ChangePhase=1))  then
    begin
    CumPar:=0;
    RUE:=0;
    end
  else
    begin
    CumPar:=CumPar+ParIntercepte;
    RUE:= BiomasseAerienne/CumPar;
    end;
except
  AfficheMessageErreur('EvalRUE',UBilEau);
end;
end;                                                                                                
/////////////////////////////////////////////////////////////////////////


//Liste de toutes les procedures redef en dyn de l'unite

Procedure EvalRunOffScaleDyn (Var T : TPointeurProcParam);
Begin
  EvalRunOffScale(T[0], T[1], T[2],T[3], T[4], T[5]);
end;

//VN Lesaffre
Procedure RempliResDr_VNDyn (Var T : TPointeurProcParam);
Begin
  RempliResDr_VN(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10]);
end;

Procedure RempliResDyn (Var T : TPointeurProcParam);
Begin
  RempliRes(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9]);
end;

Procedure EvalFTSWDyn (Var T : TPointeurProcParam);
Begin
  EvalFTSW(T[0], T[1], T[2]);
end;

Procedure EvalFESWDyn (Var T : TPointeurProcParam);
Begin
  EvalFESW(T[0], T[1], T[2]);
end;

Procedure EvolRurDyn (Var T : TPointeurProcParam);
Begin
  EvolRur(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;

Procedure EvolRurCstrDyn (Var T : TPointeurProcParam);
Begin
  EvolRurCstr(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7]);
end;

Procedure EvalKceDyn (Var T : TPointeurProcParam);
Begin
  EvalKce(T[0], T[1], T[2]);
end;

Procedure EvalKcTotDyn (Var T : TPointeurProcParam);
Begin
  EvalKcTot(T[0], T[1], T[2]);
end;

Procedure DemandePlanteDyn (Var T : TPointeurProcParam);
Begin
  DemandePlante(T[0], T[1], T[2]);
end;

Procedure DemandePlante_V2_1Dyn (Var T : TPointeurProcParam);
Begin
  DemandePlante_V2_1(T[0], T[1], T[2], T[3], T[4], T[5]);
end;

Procedure DemandeSolDyn (Var T : TPointeurProcParam);
Begin
  DemandeSol(T[0],T[1],T[2]);
end;

Procedure EvapRuSurfDyn (Var T : TPointeurProcParam);
Begin
  EvapRuSurf(T[0],T[1],T[2],T[3]);
end;

Procedure EvapRuSurfFeswDyn (Var T : TPointeurProcParam);
Begin
  EvapRuSurfFesw(T[0],T[1],T[2],T[3]);
end;

Procedure SeuilCSTRDyn (Var T : TPointeurProcParam);
Begin
  SeuilCSTR(T[0],T[1],T[2],T[3]);
end;

Procedure ExpCSTRDyn (Var T : TPointeurProcParam);
Begin
  ExpCSTR(T[0],T[1],T[2],T[3]);
end;

Procedure KHRDyn (Var T : TPointeurProcParam);
Begin
  KHR(T[0],T[1],T[2]);
end;

Procedure CstrEagleDyn (Var T : TPointeurProcParam);
Begin
  CstrEagle(T[0],T[1],T[2]);
end;

Procedure CstrIsohydriqueDyn (Var T : TPointeurProcParam);
Begin
  CstrIsohydrique(T[0],T[1],T[2],T[3]);
end;

Procedure CstrPFactorTropDoDyn (Var T : TPointeurProcParam);
Begin
  CstrPFactorTropDo(T[0],T[1],T[2],T[3],T[4],T[5]);
end;

Procedure CstrPFactorDyn (Var T : TPointeurProcParam);
Begin
  CstrPFactor(T[0],T[1],T[2],T[3],T[4]);
end;

Procedure EvalTranspiDyn (Var T : TPointeurProcParam);
Begin
  EvalTranspi(T[0],T[1],T[2]);
end;

Procedure EtrMinMaxDyn (Var T : TPointeurProcParam);
Begin
  EtrMinMax(T[0],T[1],T[2],T[3],T[4],T[5]);
end;

Procedure ConsoResDyn (Var T : TPointeurProcParam);
Begin
  ConsoRes(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8]);
end;

Procedure ConsoResSepDyn (Var T : TPointeurProcParam);
Begin
  ConsoResSep(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10]);
end;


procedure EvalRUEDyn (Var T : TPointeurProcParam) ;

begin

  EvalRUE(T[0],T[1],T[2],T[3],T[4],T[5]);

end;

procedure EvalRunOffIrrigAutoDyn(Var T : TPointeurProcParam) ;
begin
  EvalRunOffIrrigAuto(T[0], T[1], T[2], T[3], T[4], T[5], T[6], T[7], T[8], T[9], T[10], T[11], T[12]);
end;

//-------------------------------------------------------------------------//
// Cette procédure va permettre d'appeler toute la chaîne d'initialisation
//-------------------------------------------------------------------------//
Procedure InitPlotDyn (Var T : TPointeurProcParam);
begin
  InitPlot(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11],T[12],T[13]);
end;


initialization
  TabProc.AjoutProc('EvalRunOffScale', EvalRunOffScaleDyn);
  TabProc.AjoutProc('RempliRes', RempliResDyn);
  TabProc.AjoutProc('EvalFTSW', EvalFTSWDyn);
  TabProc.AjoutProc('EvalFESW', EvalFESWDyn);
  TabProc.AjoutProc('EvolRurCstr', EvolRurCstrDyn);
  TabProc.AjoutProc('EvalKce', EvalKceDyn);
  TabProc.AjoutProc('EvolRur', EvolRurDyn);
  TabProc.AjoutProc('EvalKcTot', EvalKcTotDyn);
  TabProc.AjoutProc('DemandePlante', DemandePlanteDyn);
  TabProc.AjoutProc('DemandePlante_V2_1', DemandePlante_V2_1Dyn);
  TabProc.AjoutProc('DemandeSol', DemandeSolDyn);
  TabProc.AjoutProc('EvapRuSurf', EvapRuSurfDyn);
  TabProc.AjoutProc('EvapRuSurfFesw', EvapRuSurfFeswDyn);
  TabProc.AjoutProc('SeuilCSTR', SeuilCSTRDyn);
  TabProc.AjoutProc('ExpCSTR', ExpCSTRDyn);
  TabProc.AjoutProc('KHR', KHRDyn);
  TabProc.AjoutProc('CstrEagle', CstrEagleDyn);
  TabProc.AjoutProc('CstrIsohydrique', CstrIsohydriqueDyn);
  TabProc.AjoutProc('CstrPFactor', CstrPFactorDyn);
  TabProc.AjoutProc('CstrPFactorTropDo', CstrPFactorTropDoDyn);
  TabProc.AjoutProc('EvalTranspi', EvalTranspiDyn);
  TabProc.AjoutProc('EtrMinMax', EtrMinMaxDyn);
  TabProc.AjoutProc('ConsoRes', ConsoResDyn);
  TabProc.AjoutProc('ConsoResSep', ConsoResSepDyn);
  TabProc.AjoutProc('InitPlot', InitPlotDyn);
  TabProc.AjoutProc('RempliResDr_VN', RempliResDr_VNDyn);
  TabProc.AjoutProc('EvalRUE', EvalRUEDyn);
  TabProc.AjoutProc('EvalRunOffIrrigAuto', EvalRunOffIrrigAutoDyn);



End.

