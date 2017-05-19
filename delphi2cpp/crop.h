#ifndef CROP_H
#define CROP_H
#include "variables.h"
namespace crop {
    void kill_crop(){
        NumPhase = 0;
        RelPotLeafLength = 0;
        RootFront = 0;
        SumDegreDayCor = 0;
        RootMassPerVol = 0;
        TrPot = 0;
        Lai = 0;
        LIRkdfcl = 0;
        RespMaintDebt = 0;
        WueEt = 0;
        WueTot = 0;
        A_DemStructLeaf = 0;
        A_DemStructTot = 0;
        CoeffCO2Tr = 0;
        CoeffCO2Assim = 0;
        CstrMean = 0;
        DemResInternodePop = 0;
        GrowthDryMatPop = 0;
        Tr = 0;
        TrEff = 0;
        ConversionEff = 0;
        GrowthPop = 0;
        IcMean = 0;
        DryMatResInternodePopOld = 0;
        CumGrowthPop = 0;
        CumCarbonUsedPop = 0;
        SterilityTot = 0;
        SterilityDrought = 0;
        A_AssimSurplus = 0;
        ETM = 0;
    }
}

#endif // CROP_H
