unit BhyTypeFAO;

interface
 //------------------------------------------------------------------
implementation

uses Math,ModelsManage,Main,DBModule,GestionDesErreurs;
//--------------------------------------------------------
//Proc�dure d'initialisation de la parcelle
//-----------------------------------------------------------
 Procedure InitParcelle(Const StockIniSurf,StockIniProf,Ru,EpaisseurSurf,EpaisseurProf,HumPF,PEvap,DateSemis: double;
                        var StTot,Ltr,Hum,RuSurf, ProfRU,StRuMax,
                        CapaREvap,CapaRFE,CapaRDE,ValRSurf,ValRDE,ValRFE,StockSurface:double);
Var Stockini2:double;
    Stockini1:double;
begin
try
  //MainForm.memDeroulement.Lines.Add('----Procedure InitParcelle----');
// �paisseurs
  ProfRU := EpaisseurSurf + EpaisseurProf ; // � supprimer ult�rieurement dans les diff�rents modules
// capacit� des r�servoirs
  RuSurf:= Ru * EpaisseurSurf /1000;
  CapaREvap:= 0.5*EpaisseurSurf*HumPF;
  CapaRFE:= PEvap*(CapaREvap+RuSurf);
  CapaRDE:= RuSurf - CapaRFE;
  StRuMax := Ru * ProfRu/1000;
// bornage des stockIni
  Stockini1:= min(StockIniSurf,CapaREvap+RuSurf);
  Stockini2:=min(StockIniProf,Ru * EpaisseurProf/1000);
// Contenu des r�servoirs
  ValRSurf:=min(Stockini1,CapaREvap + CapaRDE);
  ValRDE:= max( 0, ValRSurf - CapaREvap);
  ValRFE:= max(0, Stockini1 -( CapaREvap + CapaRDE));
  StockSurface:= ValRDE + ValRFE ;
  StTot:= StockSurface + Stockini2;//transpirable
  Hum:= StTot ;
// initialisation marquant l'absence de culture
  Ltr:=1;

except
  AfficheMessageErreur('InitParcelle',UBhyTypeFAO);
end;
end;
     {------------------------------------------------}

//##############################################################################
//        Calcul des degr�s jours du jour
//##############################################################################


Procedure EvalDJOpt(Const TMoy,TBase,TOpt: Double;
                     Var DJ : Double);

Begin
//DJ:=max(min(TOpt,TMoy),TBase)-Tbase;
End;

//##############################################################################
//        Calcul de la hauteur du couvert en fonction de DJ et cstr
//##############################################################################
Procedure EvolHauteur_SDJ_cstr(Const Phase:double;
                        Const HautMax, Hautmin, SDJMid,SDJ, SDJIni,DJ,cstr,SeuilTempLevee:Double;
                        Var Hauteur : Double);

Var dayHauteur:Double ;
// Calcule la variation journali�re de hauteur en fonction de DJ (du jour) et de cstr
//(en phase de croissance rapide). Pas d'effet de cstr sur croissance lente de d�but

Begin

If Phase<2 then
  Hauteur:=0

else
  If SDJ<SDJIni+SeuilTempLevee then
        Begin
        DayHauteur:=Hautmin*DJ/SDJini ;
        Hauteur:= Hauteur+dayHauteur;
        end
  else
        Begin
    	DayHauteur:= ((HautMax-Hautmin)/(SDJMid-SDJIni))*DJ*cstr;
       	Hauteur:=Min(HautMax, Hauteur + DayHauteur);
        end;
end;
//##############################################################################
//  Proc�dure alternative de la pr�c�dente sans cstr
//##############################################################################

Procedure EvolHauteur_SDJ(Const Phase:double;
                        Const HautMax, Hautmin, SDJMid,SDJ, SDJIni,DJ,SeuilTempGermi:Double;
                        Var Hauteur : Double);

 Var dayHauteur:Double ;
// Calcule la variation journali�re de hauteur en fonction de DJ (du jour)

Begin
try
If Phase<2 then
  Hauteur:=0
  else
  If SDJ<SDJIni+SeuilTempGermi then
        Begin
        DayHauteur:=Hautmin*DJ/SDJini ;
        Hauteur:= Hauteur+dayHauteur;
        end
  else
        Begin
    	DayHauteur:= ((HautMax-Hautmin)/(SDJMid-SDJIni))*DJ;
       	Hauteur:=Min(HautMax, Hauteur + DayHauteur);
        end;
except
  AfficheMessageErreur('EvolHauteur_SDJ',UBhyTypeFAO);
end;
end;

//##############################################################################
// Calcul de Kcp FAO bas� sur somme de degr�s jours (�tait dans arachide)
//##############################################################################

Procedure EvolKcpFaoSDJ(Const Phase:Double;
                        Const SDJlevee,KcMax, KcIni, KcEnd, SDJMid,SDJEnd,
{Procedure 340}          SDJIni:Double;
                        Var SDJMax,SDJ,Kcp : Double);

Begin
try
SDJ:= SDJ - SDJlevee;//JCC 18/04/05 les sommes de DJ pour Kcp d�butent � la lev�e
SDJMax:= SDJMax - SDJlevee;
  If Phase<=1 then
          Kcp:=0
    // jcc avril 2004 0 remplac� par 0.001
  else
    If SDJ<SDJIni then
          Kcp:=KcIni
    else
       begin
       If SDJ<SDJEnd then
       Kcp:=Min(KcMax,KcIni+(KcMax-KcIni)*(SDJ-SDJIni)/(SDJMid-SDJIni))
       else
       Kcp:=Min(KcMax,KcMax-(KcMax-KcEnd)*(SDJ-SDJEnd)/(SDJMax-SDJEnd));
      end;
except
  AfficheMessageErreur('EvolKcpFaoSDJ',UBhyTypeFAO);
end;
 end;

 //#############################################################################
 // Proc�dure alternative avec DJ et cstr
 //##############################################################################
Procedure EvolKcpFaoSDJ_cstr(Const Phase:Double;
                        Const SDJlevee,KcMax, KcIni, KcEnd, SDJMid,SDJEnd,
                        SDJIni,DJ,cstr:Double;
                        Var SDJMax,SDJ,Kcp : Double);

 Var DayKcp:Double ;
// Calcule la variation journali�re de Kcp en fonction de DJ (du jour)
//et de cstr (en phase de croissance rapide)

Begin
SDJ:= SDJ - SDJlevee;//JCC 18/04/05 les sommes de DJ pour Kcp d�butent � la lev�e
SDJMax:= SDJMax - SDJlevee;
DayKcp:=0;
If Phase<=1 then
  Kcp:=0
  // jcc avril 2004 0 remplac� par 0.001
else
  If SDJ<SDJIni then
        Kcp:=KcIni
  else
    begin
     If SDJ<SDJEnd then
	DayKcp:=((KcMax-KcIni)/(SDJMid-SDJIni))*DJ*cstr
     else
     	DayKcp:=(KcEnd-KcMax)*DJ/(SDJMax-SDJEnd);
    end;
    Kcp:=Min(KcMax,Kcp+dayKcp);
    //TO DO :v�rifier si correct
end;

//##############################################################################
//          Calcul de Kce potentiel selon les proc�dures FAO 56
//##############################################################################

Procedure EvalKcePotFAO (Const Mulch,vent, HRmin, Kcp,Hauteur : double;
		Var Kce: double);
Var
Kborne : Double;
// avec cette proc�dure fixer Mulch = 120
Begin
try
if  Hauteur=0 then
  //Kce:=max(0.12,1.2+0.04*(vent-2)-0.004*(HRmin-45))    18/04/05
  Kce:=max(0.12,Mulch/100+0.04*(vent-2)-0.004*(HRmin-45))

else
  begin
  //Kborne:=max(Kcp+0.05,1.2+(0.04*(vent-2)-0.004*(HRmin-45))*power(0.3,Hauteur/3));    18/04/05
  Kborne:=max(Kcp+0.05,Mulch/100+(0.04*(vent-2)-0.004*(HRmin-45))*power(Hauteur/3,0.3));
  Kce:=Kborne-Kcp;
  end;
except
  AfficheMessageErreur('EvalKcePotFAO',UBhyTypeFAO);
end;
End;

//##############################################################################
//          Calcul de l'�vaporation potentielle (si couche de surface satur�e)
//##############################################################################

Procedure EvalEvapPot (Const Etp,Kce : double;
                        Var EvapPot : double);
Begin
try
  EvapPot:= Kce*Etp;
except
  AfficheMessageErreur('EvalEvapPot',UBhyTypeFAO);
end;
end;

//##############################################################################
//        Calcul de l'�vaporation r�elle avec mod�le � remplissage prioritaire
//    de la r�serve facilement �vaporable (RFE).
//
//##############################################################################

//Les capacit�s de r�servoirs sont calcul�s comme dans FAO 56 ainsi que la d�croissance de
//l'�vaporation lorsque RFE est �puis�e

Procedure EvolEvapSurfRFE_RDE(Const EvapPot,CapaREvap,CapaRDE,StRurMax,RuSurf:double;
                        Var Evap,ValRSurf, ValRFE,ValRDE,StRur,StRu,Kr: Double);
var
  ValRSurfPrec,EvapRU:double;
  Evap1,Evap2:double;
begin
try
  ValRSurfPrec:=ValRSurf;
    // ValRSurf est l'eau contenue dans les r�servoirs Revap (non transpirable) et RDE (transpirable et �vaporable
  if ValRFE>0 then
    begin
    if ValRFE<EvapPot then
      begin
      Evap1:=ValRFE;
      Evap2:=max(0,min(ValRSurf,((EvapPot-ValRFE)*ValRSurf)/(CapaREvap+CapaRDE)));    // born� � 0 et ValRSurf le 27/04/05
      end
    else
      begin
      Evap1:=EvapPot;
      Evap2:=0;
      end;
    end
  else
    begin
    Evap1:=0;
    Evap2:=max(0,min(ValRSurf,EvapPot*ValRSurf/(CapaREvap+CapaRDE)));   // born� � 0 et ValRSurf le 27/04/05
    end;

  Evap:=Evap1+Evap2;
  ValRFE:=ValRFE-Evap1;
  ValRSurf:=ValRSurf-Evap2;
  ValRDE:=max(0,ValRSurf-CapaREvap);
  if EvapPot=0    then   Kr:=0
  else
  Kr:= Evap/EvapPot;

  // part de l'�vaporation pr�lev�e dans les r�servoirs RFE et RDE
  if ValRSurf>=CapaREvap then
    EvapRU:=Evap
  else
    begin
    if ValRSurfPrec<=CapaREvap then
      EvapRU:=Evap1
    else
      EvapRU:=evap1+ValRSurfPrec-CapaREvap;
    end;

  //Evaporation de Ru et Rur, MAJ
  if StRurMax<=RuSurf then
  // quand les racines n'ont pas d�pass� la premi�re couche
    StRur:=max(0,StRur-EvapRU*StRurMax/RuSurf)
  else
    StRur:=max(0,StRur-EvapRU);

  StRu:=StRu-EvapRU;
except
  AfficheMessageErreur('EvalEvapPot',UBhyTypeFAO);
end;
END;

//###############################################################################
//             calcul du Kc FAO = Kce*Kr+Kcp
//###############################################################################

Procedure EvalKcFAO (Const Kce,Kr,Kcp : Double;
                     Var Kctot : Double);

Begin
try
    Kctot:=Kce*Kr+Kcp;
except
  AfficheMessageErreur('EvalKcFAO',UBhyTypeFAO);
end;
end;

//##############################################################################
//               calcul de FTSW pour m�moire dans BilEau
//###############################################################################

//##############################################################################
//               calcul de cstrpfactorFAO
//##############################################################################

Procedure EvalCstrPFactorFAO(Const PFactor, FTSW, ETo, Kc : Double;
                             Var cstr : Double);
//calcule cstr en fonction de l'ET culture
{TO DO : inclure la texture si sol tr�s sableux pFact := ParPFact+0.1, sol sableux
pFact := ParPFact+0.05, sol argileux pFact := ParPFact-0.05, tr�s argileux
pFact := ParPFact-0.1 ; A inclure avant correction climatique. N�cessaire si l'on veut
comparer des sols sans modifier le pFactor}
Var
  pFact : Extended;
Begin
try
  pFact := PFactor + 0.04 * (5 - Kc * ETo);
  pFact := Max(0, pFact);
  pFact := Min(0.8, pFact);
  cstr := Min((FTSW / (1 - pFact)), 1);
  cstr := Max (0, cstr);
except
  AfficheMessageErreur('EvalCstrPFactorFAO',UBhyTypeFAO);
end;
End;

//##############################################################################
//                Calcul de la transpiration potentielle selon FAO
//                      pour m�moire = DemandePlante  de Bileau
//##############################################################################

//##############################################################################
//                Calcul de la transpiration r�elle
//                      pour m�moire = EvalTranspi de Bileau
//##############################################################################

//##############################################################################
//                Calcul de l'�vapotranspiration maximale et r�elle
//
//##############################################################################
procedure EvalETRETM(const Evap, Tr, Trpot : Double; var ETM, ETR : Double);
begin
  try
    ETM := Evap + Trpot;
    ETR := Evap + Tr;
  except
    AfficheMessageErreur('EvalETRETM',UBhyTypeFAO);
  end;
end;

//##############################################################################
//  Calcul de la consommation dans les diff�rents r�servoirs par transpiration
//##############################################################################

Procedure EvolConsoResRDE_RFE(Const RuRac, RuSurf,CapaREvap,Tr:double;
                         Var StockRac, StockSurface, StockTotal,ValRFE,ValRDE,ValRSurf:double);
var
  TrSurf:double;
begin
  try
  // le calcul de cstr et de Tr doit intervenir apr�s l'�vaporation
  //calcul de la part de transpiration affect�e aux r�servoirs de surface
    if (RuRac <> 0) then
    begin
      if (RuRac <= RuSurf) then      //correction JCC le 21/08/02 de stRurMax<=RuSurf/stRurMax
        TrSurf:=Tr
      else
      //TrSurf:=Tr*RuSurf/stRurMax;// on peut pond�rer ici pour tenir compte du % racines dans chaque couche
        if  (StockRac <> 0) then
          TrSurf := Tr * StockSurface / StockRac;  // modif du 15/04/05  pond�ration par les stocks et non les capacit�s, sinon on n'extrait pas Tr si stock nul
    end
    else
      TrSurf := 0;

    // MAJ des r�servoirs de surface en r�partissant TrSurf entre RFE et RDE
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
        ValRFE := ValRFE - TrSurf  // priorit� � la RFU
      else
      begin
        ValRSurf := ValRSurf - (TrSurf - ValRFE);
        ValRDE := ValRDE - (TrSurf - ValRFE);
        ValRFE := 0;
      end;
    end;

    StockSurface := ValRFE + ValRDE;
    StockRac := max(0, StockRac - Tr);    // 18/04/05 d�plac� en fin de proc�dure, car utilis� pour le calcul de Trsurf
    StockTotal := max(0, StockTotal - Tr);

  except
    AfficheMessageErreur('EvolConsoResRDE_RFE',UBhyTypeFAO);
  end;
end;

//##############################################################################
//  Calcul du ruissellement de la pluie m�thode seuil
//le ruissellement est egal a un pourcentage de la quantite de pluie sup�rieure 
//  au seuil defini
//##############################################################################

Procedure EvalRuiss_Seuil (Const Rain, seuilRuiss, PourcRuiss,Irrig: Double;
                                 Var EauDispo, Lr : Double);
Begin
try
  Lr:=0;
  If Rain>SeuilRuiss then
    Lr:=(Rain - SeuilRuiss) * PourcRuiss/100;
  if Irrig = nullValue then
    EauDispo:= Rain-Lr
  else
    EauDispo:= Rain+Irrig-Lr;
except
  AfficheMessageErreur('EvalRuiss_Seuil',UBhyTypeFAO);
end;
End;

//##############################################################################
//  Calcul du remplissage des diff�rents r�servoirs par EauDispo
//##############################################################################

Procedure EvolRempliResRFE_RDE(Const RuSurf, EauDispo, StRurMax,CapaRFE,
                       CapaREvap,CapaRDE,StRuMax:double;
                       var stRu, stRUR, Hum, stRuSurf, Dr,
                       ValRDE,ValRFE,ValRSurf:double);
// Proc�dure JCC - 19/07/2002
VAR
  EauReste,ValRSurfPrec,EauTranspi:double;

BEGIN
try
  // Remplissage de la RFE:
  EauReste:=0;
  ValRFE:=ValRFE+EauDispo;    //non born�
  if ValRFE>CapaRFE then
    begin                      // bornage
    EauReste:=ValRFE-CapaRFE;
    ValRFE:=CapaRFE;
    end;

  // Remplissage des autres r�servoirs de surface :
  //Il faut dissocier l'eau qui va dans le r�servoir Revap (non transpirable)
  //et l'eau qui va dans lesr�servoirs de transpiration
  ValRSurfPrec:=ValRSurf;
  ValRSurf:=ValRSurf+EauReste;      //non born�
  { On calcule la quantit� d'eau qui ira dans les r�servoirs racinaire, total et hum
  apr�s d�duction de l'eau qui va dans le r�servoir uniquement �vaporable}
  if ValRSurfPrec<CapaREvap then
    EauTranspi:=EauDispo-(min(CapaREvap,ValRSurf)-ValRSurfPrec)
  else
    EauTranspi:=EauDispo;

  if ValRSurf>(CapaREvap+CapaRDE) then        //bornage ValRSurf
    begin
    ValRSurf:=CapaREvap+CapaRDE;
    ValRDE:=CapaRDE;          //JCC 20/9/02
    end
  else                          //JCC 20/9/02
    begin
    if ValRSurf <= CapaREvap then    //JCC 20/9/02
      ValRDE:=0                    //JCC 20/9/02
    else                             //JCC 20/9/02
      ValRDE:= ValRSurf-CapaREvap;  //JCC 20/9/02
    end;

  stRuSurf:= ValRFE+ValRDE;                   //JCC 20/9/02
  //stRuSurf:= min(stRuSurf+EauTranspi,RuSurf);  //JCC 20/9/02

  // remplissage des autres r�servoirs par CB    { TODO : stRuSurf n'est plus modifi� par la fonction: attention ! }
  // rempli res en eau, estime drainage, et evolution du front d'humectation
  StRu:= StRu + EauTranspi;
  If StRu > StRuMax then
    begin
    Dr := StRu - StRuMax;
    StRu := StRuMax;
    end
  else
    begin
    Dr:=0;
    end;
  { Modif JCC du 21/04/05 pour que Hum se r�duise par �vaporation tant qu'il est < RuSurf}
  if Hum <  (CapaRFE+CapaRDE) then
    Hum:= stRuSurf
  else
    Hum:= Max(Hum , StRu);
  // Rempli reservoir racines
  StRur:= Min(StRur + EauTranspi , StRurMax);
  //StRur:= Min(StRur , StRu);       JCC 27/04/05
except
  AfficheMessageErreur('EvolRempliResRFE_RDE',UBhyTypeFAO);
end;
END;

//##############################################################################
// Calcul de l'�volution de la RU Racinaire en fonction des r�servoirs de surface
//##############################################################################



Procedure EvolRurRFE_RDE(Const Vrac, Hum, Ru, stRuSurf, RuSurf,ProfRacIni,
                      Ze,Z2,ValRDE,ValRFE,NumPhase,ChangePhase:Double;
                     var  stRurMax, stRur,stRu : Double);
var
  DeltaRur      : Double;
begin
try
if NumPhase=0 then
  begin
  StRurMax:=0;
  stRur:=0;
  end
else
  begin
  if ((NumPhase=1) and (ChangePhase=1)) then  // les conditions de germination sont atteinte et nous sommes le jour m�me
    begin
    stRurMax:=Ru * min(ProfRacIni,(Ze+Z2))/1000;
    if ProfRacIni<= Ze then
      stRur:=(ValRDE+ValRFE)*ProfRacIni/Ze
    else
      stRur:=StRu*Min(ProfRacIni/(Ze+Z2),1);
    end
  else
    begin
    if (Hum-StRurMax) < (VRac/1000*RU) Then
      DeltaRur := Hum-StRurMax
    else
      DeltaRur := VRac/1000*RU;
    StRurMax:= StRurMax+Deltarur;
    if StRurMax > RuSurf then
      StRur:= StRur+Deltarur
    else
      StRur:= (ValRDE+ValRFE)* (StRurMax / RuSurf);
    end;
  end;
except
  AfficheMessageErreur('EvolRurRFE_RDE',UBhyTypeFAO);
end;
end;
////////////////////////////////////////////////////////////////////////////////

//##############################################################################
//#  Procedures specifiques pour comparer aux autres modeles                   #
//##############################################################################

////////////////////////////////////////////////////////////////////////////////


Procedure EvolRurPalmier(Const stRuMax,stRu :Double;
                     var  stRurMax, stRur: Double);
{pas de croissance racinaire}
begin
try
  //stRurMax:=Ru * (Ze+Z2)/1000;
  stRurMax:=stRuMax ;
  stRur:=min(StRu,stRurMax);
except
  AfficheMessageErreur('EvolRurPalmier',UBhyTypeFAO);
end;
end;
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
Procedure EvalFESW_RFE_RDE(Const CapaREvap,CapaRFE,CapaRDE,ValRSurf,ValRFE:double;
                        Var FESW: Double);
{Pour utiliser ce Bhy avec module FESW� de Christian}
begin
try
  FESW:= (ValRSurf + ValRFE)/(CapaREvap+CapaRFE+CapaRDE);
except
  AfficheMessageErreur('EvalFESW_RFE_RDE',UBhyTypeFAO);
end;
END;
//##############################################################################

Procedure EvolEvapSurfFESW(Const EvapPot,FESW,CapaREvap,CapaRDE,StRurMax,RuSurf:double;
                        Var Evap,ValRSurf, ValRFE,ValRDE,StRur,StRu: Double);
var
  ValRSurfPrec,EvapRU:double;
  Evap1,Evap2:double;

{Les capacit�s de r�servoirs sont calcul�s comme dans FAO 56 mais l'�vaporation
est fonction de FESW� (mod�le CB) }

begin
try
  ValRSurfPrec:=ValRSurf ;
    // ValRSurf est l'eau contenue dans les r�servoirs Revap (non transpirable) et RDE (transpirable et �vaporable
  Evap:= EvapPot*FESW*FESW;

  if ValRFE>0 then
    begin
    if ValRFE<Evap then
      begin
      Evap1:=ValRFE;
      Evap2:=Evap-ValRFE;
      end
    else
      begin
      Evap1:=Evap;
      Evap2:=0;
      end;
    end
  else
    begin
    Evap1:=0;
    Evap2:=Evap;
    end;

  ValRFE:=ValRFE-Evap1;
  ValRSurf:=ValRSurf-Evap2;
  ValRDE:=max(0,ValRSurf-CapaREvap);


  // part de l'�vaporation pr�lev�e dans les r�servoirs RFE et RDE
  if ValRSurf>=CapaREvap then
    EvapRU:=Evap
  else
    begin
    if ValRSurfPrec<=CapaREvap then
      EvapRU:=Evap1
    else
      EvapRU:=evap1+ValRSurfPrec-CapaREvap;
    end;

  //Evaporation de Ru et Rur, MAJ
  if StRurMax<=RuSurf then
  // quand les racines n'ont pas d�pass� la premi�re couche
    StRur:=max(0,StRur-EvapRU*StRurMax/RuSurf)
  else
    StRur:=max(0,StRur-EvapRU);

  StRu:=StRu-EvapRU;
except
  AfficheMessageErreur('EvalEvapPot',UBhyTypeFAO);
end;
END;
/////////////////////////////////////////////////////////////////////////////////
Procedure EvolRurRFE_RDEcstr(Const Vrac, Hum, Ru, stRuSurf, RuSurf,ProfRacIni,
                      Ze,Z2,ValRDE,ValRFE,NumPhase,ChangePhase,Cstr:Double;
                     var  stRurMax, stRur,stRu : Double);
{fait �voluer RUR en fonction de cstr (CB)}
var
  DeltaRur      : Double;
begin
try
if NumPhase=0 then
  begin
  StRurMax:=0;
  stRur:=0;
  end
else
  begin
  if ((NumPhase=1) and (ChangePhase=1)) then  // les conditions de germination sont atteinte et nous sommes le jour m�me
    begin
    stRurMax:=Ru * min(ProfRacIni,(Ze+Z2))/1000;
    if ProfRacIni<= Ze then
      stRur:=(ValRDE+ValRFE)*ProfRacIni/Ze
    else
      stRur:=StRu*Min(ProfRacIni/(Ze+Z2),1);
    end
  else
    begin
    {if (Hum-StRurMax) < (VRac/1000*RU) Then
      DeltaRur := Hum-StRurMax
    else
      DeltaRur := VRac/1000*RU; Partie modifi�e par CB}
    If (Hum-StRurMax) < (max(VRac*Cstr,Vrac*0.7)/1000*RU)
       Then DeltaRur := Hum-StRurMax
       Else DeltaRur := max(VRac*Cstr,Vrac*0.7)/1000*RU;
    StRurMax:= StRurMax+Deltarur;
    if StRurMax > RuSurf then
      StRur:= StRur+Deltarur
    else
      StRur:= (ValRDE+ValRFE)* (StRurMax / RuSurf);
    end;
  end;
except
  AfficheMessageErreur('EvolRurRFE_RDEcstr',UBhyTypeFAO);
end;
end;
   {---------------------------------------------------------}
Procedure EvalCstrExpon(Const FExpCstrTr, GExpCstrTr, ftsw : Double;
                   Var cstr : Double);
{
Estimation de l'indice de contrainte de consommation d'eau
Sadras et MilRoy 1996
Lacape 1999
Param�tre specifique espece/variete
// Parametres
IN:
ftsw : mm
Par1ExpCSTR :
Par2ExpCSTR :
OUT:
cstr : %
}
      Begin
         cstr:= 1/(1+exp(-(ftsw-FExpCstrTr)/GExpCstrTr));
      End;
     {------------------------------------------------}


////////////////////////////////////////////////////////////////////////////////
//Modules Kce et Kcp alternatifs
////////////////////////////////////////////////////////////////////////////////

//##############################################################################
//          Calcul de Kce potentiel fonction du Rn au sol et borne FAO
//##############################################################################

Procedure EvalKcePotLTRRn (Const Mulch,vent, HRmin,Kcp,Hauteur,Lai : double;
		Var Kce: double);
Var
Kborne : Double;
// avec cette proc�dure fixer Mulch = 120
{ JCC le 21/09/2005 : la borne est identique � FAO mais on introduit le Rn
transmis au sol au lieu de Kcp}
Begin
try
if  ((Hauteur=0)or(Lai=0)) then
   Kce:=max(0.12,Mulch/100+0.04*(vent-2)-0.004*(HRmin-45))
else
  begin
  Kborne:=max(Kcp+0.05,Mulch/100+(0.04*(vent-2)-0.004*(HRmin-45))*power(Hauteur/3,0.3));
  Kce:=Kborne*exp(-0.7*Lai);
  end;
except
  AfficheMessageErreur('EvalKcePotLTRRn',UBhyTypeFAO);
end;
End;

//##############################################################################
// Calcul de Kcp FAO bas� sur somme de degr�s jours (�tait dans arachide)
//##############################################################################

{Procedure EvolKcpLai2(Const NumPhase:Double;
                        Const KcMax, Lai:Double;
                        Var Kcp, MaxLai : Double);

Begin
try
  If Lai > MaxLai then MaxLai:= Lai;
  If ((NumPhase<=1) or (Lai = 0)) then
        begin
          Kcp:=0 ;
          MaxLai:=0 ;
        end
  else
   { If NumPhase< 5    then }
      //begin
        //Kcp:= min(0.62*power(Lai,0.3),(0.7+(KcMax-0.7)*max(0,Lai-1.5)/1.5));
        //Kcp:=KcMax*min(1,power(Lai/3,0.5));
        //Kcp:= min (Kcp,KcMax);
      //end
    {else
      Kcp:= min(KcMax,power(Lai/max(0.01,MaxLai),2));   }
{except
  AfficheMessageErreur('EvolKcpLai2',UBhyTypeFAO);
end;
end;  }
/////////////////////////////////////////////////////////////////////////////////
Procedure EvolKcpLai2(Const NumPhase:Double;
                        Const KcMax, Lai:Double;
                        Var Kcp, MaxLai : Double);

Begin
try
  If Lai > MaxLai then MaxLai:= Lai;
  If ((NumPhase<=1) or (Lai = 0)) then
        begin
          Kcp:=0 ;
          MaxLai:=0 ;
        end
  else
    If  NumPhase< 5    then
       Kcp:=KcMax*power(min(1,(Lai/3)),0.5)
    else
      begin
          //Kcp:=KcMax*power(min(1,(Lai/3)),0.5);
          Kcp:= KcMax*min(power(min(1,Lai/3),0.5),power(Lai/max(0.01,MaxLai),1.5));
          Kcp:= min (Kcp,KcMax);
      
      end;
  
    {else
      Kcp:= min(KcMax,power(Lai/max(0.01,MaxLai),2));   }
except
  AfficheMessageErreur('EvolKcpLai2',UBhyTypeFAO);
end;
end;



//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
// LISTE DES PROCEDURES DYNAMIQUES
//
//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
//1
Procedure InitParcelleDyn (Var T : TPointeurProcParam);
Begin
  InitParcelle(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11],T[12],T[13],T[14],T[15],T[16],T[17],T[18],T[19],T[20]);
end;
//1 bis
Procedure EvalDJOptDyn (Var T : TPointeurProcParam);
Begin
  EvalDJOpt(T[0],T[1],T[2],T[3]);
end;
//2
Procedure EvolHauteur_SDJ_cstrDyn (Var T : TPointeurProcParam);
Begin
  EvolHauteur_SDJ_cstr(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9]);
end;
//3
Procedure EvolHauteur_SDJDyn (Var T : TPointeurProcParam);
Begin
  EvolHauteur_SDJ(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8]);
end;
//4
Procedure EvolKcpFaoSDJDyn (Var T : TPointeurProcParam);
Begin
  EvolKcpFaoSDJ(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10]);
end;
//5
Procedure EvolKcpFaoSDJ_cstrDyn (Var T : TPointeurProcParam);
Begin
  EvolKcpFaoSDJ_cstr(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11],T[12]);
end;
//6
Procedure EvalKcePotFAODyn (Var T : TPointeurProcParam);
Begin
  EvalKcePotFAO(T[0],T[1],T[2],T[3],T[4],T[5]);
end;
//7
Procedure EvalEvapPotDyn (Var T : TPointeurProcParam);
Begin
  EvalEvapPot(T[0],T[1],T[2]);
end;
//8
Procedure EvolEvapSurfRFE_RDEDyn (Var T : TPointeurProcParam);
Begin
  EvolEvapSurfRFE_RDE(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11]);
end;
//9
Procedure EvalKcFAODyn (Var T : TPointeurProcParam);
Begin
  EvalKcFAO(T[0],T[1],T[2],T[3]);
end;
//10
Procedure EvalCstrPFactorFAODyn (Var T : TPointeurProcParam);
Begin
  EvalCstrPFactorFAO(T[0],T[1],T[2],T[3],T[4]);
end;
//11
Procedure EvalETRETMDyn (Var T : TPointeurProcParam);
Begin
  EvalETRETM(T[0],T[1],T[2],T[3],T[4]);
end;
//12
Procedure EvolConsoResRDE_RFEDyn (Var T : TPointeurProcParam);
Begin
  EvolConsoResRDE_RFE(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9]);
end;
//13
Procedure EvalRuiss_SeuilDyn (Var T : TPointeurProcParam);
Begin
  EvalRuiss_Seuil(T[0],T[1],T[2],T[3],T[4],T[5]);
end;
//14
Procedure EvolRempliResRFE_RDEDyn (Var T : TPointeurProcParam);
Begin
  EvolRempliResRFE_RDE(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11],T[12],T[13],T[14]);
end;
//15
Procedure EvolRurRFE_RDEDyn (Var T : TPointeurProcParam);
Begin
  EvolRurRFE_RDE(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11],T[12],T[13],T[14]);
end;

Procedure EvolRurPalmierDyn (Var T : TPointeurProcParam);
Begin
  EvolRurPalmier(T[0],T[1],T[2],T[3]);
end;

Procedure EvalFESW_RFE_RDEDyn (Var T : TPointeurProcParam);
Begin
  EvalFESW_RFE_RDE(T[0],T[1],T[2],T[3],T[4],T[5]);
end;

Procedure EvolEvapSurfFESWDyn (Var T : TPointeurProcParam);
Begin
  EvolEvapSurfFESW(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11]);
end;

Procedure EvolRurRFE_RDEcstrDyn (Var T : TPointeurProcParam);
Begin
  EvolRurRFE_RDEcstr(T[0],T[1],T[2],T[3],T[4],T[5],T[6],T[7],T[8],T[9],T[10],T[11],T[12],T[13],T[14],T[15]);
end;

Procedure EvalCstrExponDyn (Var T : TPointeurProcParam);
Begin
  EvalCstrExpon(T[0],T[1],T[2],T[3]);
end;

Procedure EvalKcePotLTRRnDyn (Var T : TPointeurProcParam);
Begin
  EvalKcePotLTRRn(T[0],T[1],T[2],T[3],T[4],T[5],T[6]);
end;

Procedure EvolKcpLai2Dyn (Var T : TPointeurProcParam);
Begin
  EvolKcpLai2(T[0],T[1],T[2],T[3],T[4]);
end;

initialization
        TabProc.AjoutProc('InitParcelle', InitParcelleDyn);
        TabProc.AjoutProc('EvalDJOpt', EvalDJOptDyn);
        TabProc.AjoutProc('EvolHauteur_SDJ_cstr', EvolHauteur_SDJ_cstrDyn);
        TabProc.AjoutProc('EvolHauteur_SDJ', EvolHauteur_SDJDyn);
        TabProc.AjoutProc('EvolKcpFaoSDJ', EvolKcpFaoSDJDyn);
        TabProc.AjoutProc('EvolKcpFaoSDJ_cstr', EvolKcpFaoSDJ_cstrDyn);
        TabProc.AjoutProc('EvalKcePotFAO', EvalKcePotFAODyn);
        TabProc.AjoutProc('EvalEvapPot', EvalEvapPotDyn);
        TabProc.AjoutProc('EvolEvapSurfRFE_RDE', EvolEvapSurfRFE_RDEDyn);
        TabProc.AjoutProc('EvalKcFAO', EvalKcFAODyn);
        TabProc.AjoutProc('EvalCstrPFactorFAO', EvalCstrPFactorFAODyn);
        TabProc.AjoutProc('EvalETRETM', EvalETRETMDyn);
        TabProc.AjoutProc('EvolConsoResRDE_RFE', EvolConsoResRDE_RFEDyn);
        TabProc.AjoutProc('EvalRuiss_Seuil', EvalRuiss_SeuilDyn);
        TabProc.AjoutProc('EvolRempliResRFE_RDE', EvolRempliResRFE_RDEDyn);
        TabProc.AjoutProc('EvolRurRFE_RDE', EvolRurRFE_RDEDyn);
        TabProc.AjoutProc('EvolRurPalmier', EvolRurPalmierDyn);
        TabProc.AjoutProc('EvalFESW_RFE_RDE', EvalFESW_RFE_RDEDyn);
        TabProc.AjoutProc('EvolEvapSurfFESW', EvolEvapSurfFESWDyn);
        TabProc.AjoutProc('EvolRurRFE_RDEcstr', EvolRurRFE_RDEcstrDyn);
        TabProc.AjoutProc('EvalCstrExpon', EvalCstrExponDyn);
        TabProc.AjoutProc('EvalKcePotLTRRn', EvalKcePotLTRRnDyn);
        TabProc.AjoutProc('EvolKcpLai2', EvolKcpLai2Dyn);


end.
