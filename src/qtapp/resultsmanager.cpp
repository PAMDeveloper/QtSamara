#include "resultsmanager.h"

const QString subsetStr = "ApexHeight,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,CstrPhase6,CulmsPerHill,CumDr,CumEt,CumIrrig,CumLr,CumTr,CumWUse,DaysDrySowingToGermination,GerminationStatus,DryMatAboveGroundTotPop,DryMatResInternodePop.,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,GrainFillingStatus,GrainYieldPop,HarvestIndex,HaunIndex,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,Lai,LaiDead,LeafDeathPop,NumPhase,PanStructMass,PlantHeight,RootFront,RUE,Sla,SterilityTot,TrEff,WueTot,TmaxMoy,DaysDrySowingToGermination";
const QString removal = "DemStructLeaf, DemStructTot, GrowthStructLeaf, GrowthStructTot, IncreaseResInternodePot, ResInternodeMobiliDay, ApexHeightGain, AssimSurplus, ChangeNurseryStatus, ChangePhase, ChangeSsPhase, CoeffCO2Assim, CoeffCO2Tr, CoeffStressLogging, Conversion, CounterNursery, CstrCum, CstrMean, CstrPhase2, CstrPhase3, CstrPhase4, CstrPhase5, CstrPhase6, CulmsPerHillFin, CulmsPerPlantFin, CumFTSWPhase2, CumFTSWPhase3, CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumGrowthPop, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, CumIrriFin, CumSupplyTot, CumWUseFin, DAF, Decli, DemLeafAreaPlant, DemPanicleFillPop, DemResInternodePop, DemStructInternodePlant, DemStructInternodePop, DemStructLeafPlant, DemstructLeafPop, DemStructRootPlant, DemStructRootPop, DemStructTotPop, Density, FloodwaterGain, FtswMoy, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase6, FtswPhase6, GainRootSystSoilSurfPop, GainRootSystVolPop, GrainFillingStatusFin, GrainYieldPopFin, Growth… (all variable names starting on Growth), HaunGain, HMin, HMax, HMoy, HMoyCalc, IcCum, IcMean, IcPhase2, IcPhase3, IcPhase4, IcPhase5, IcPhase6, IncreaseResInternodePop, LAIFin, LatRad, LIRkdf, LIRkdfcl, MatuProgress, MatuSDJ, MobiliLeafDeath, NumSsPhase, PhaseStemElongation, PlantLeafNumNew, RayExtra, ReservePopFin, ResInternodeMobiliFin, ResInternodeMobiliDayPot, RespMaintDebt, RgMax, RootFrontOld, RootSystSoilSurfPop, RootSystSoilSurfPopOld, RootSystVolPop, RootSystVolPopOld, SDJCorPhase4, SeuilCstrMortality, SeuilTemp, SeuilTempSsPhase, SimEmergence, SimEndCycle, SimPanIni, SimStartGermin, SimStartMatu2, SimStartPSP, SlaMitch, SlaNew, SommeDegresJourMax, SterilityTotFin, SumDDPhasePrec, SumDegresDay, SumPP, SunDistance, SunPosi, TempLAI, TmaxMoy, TminMoy, TMoyCalc, TMoyPrec";

ResultsManager::ResultsManager(QTableView * resultView, ResultsDataModel * resultModel, QObject *parent)
    : QObject(parent), resultModel(resultModel), resultView(resultView) {
    subsetFilter = false;
    subsetInvFilter = true;
    phaseFilter = false;
    strFilter = "";
    subsetStrList = removal.split(",");

}

bool startWith(QString str, QStringList strList) {
    for(QString l: strList) {
        if(str.startsWith(l, Qt::CaseInsensitive))
            return true;
    }
    return false;
}

void ResultsManager::applyFilters() {
    for(int i = resultModel->columnCount() - 1; i >= 0 ; i--){
        QString header = resultModel->headerData(i).toString();
        QStringList strFilters = strFilter.trimmed().split(QRegExp(",|;"));
        resultView->horizontalHeader()->showSection(i);
        if (!strFilter.isEmpty()) {
            if (!startWith(header, strFilters))
                resultView->horizontalHeader()->hideSection(i);
        } else if(subsetFilter){
            if (!subsetStrList.contains(header, Qt::CaseInsensitive))
                resultView->horizontalHeader()->hideSection(i);
        } else if(subsetInvFilter) {
            if (subsetStrList.contains(header, Qt::CaseInsensitive))
                resultView->horizontalHeader()->hideSection(i);
        }
    }

    int phaseIdx = -1;
    for(int i = resultModel->columnCount() - 1; i >= 0 ; i--){
        if(QString::compare(resultModel->headerData(i, Qt::Horizontal).toString(), "NumPhase", Qt::CaseInsensitive) == 0) {
            phaseIdx = i;
            break;
        }
    }
    resultView->verticalHeader()->showSection(0);
    int lastPhase = resultModel->index(0,phaseIdx).data().toInt();
    for(int i = resultModel->rowCount() - 1; i >= 0 ; i--){
        int numPhase = resultModel->index(i,phaseIdx).data().toInt();
        if(phaseFilter && numPhase == lastPhase) {
            resultView->verticalHeader()->hideSection(i);
        } else {
            resultView->verticalHeader()->showSection(i);
            lastPhase = numPhase;
        }
    }
}
void ResultsManager::filterColumns(QString checked) {
    if (checked == "all"){
        subsetFilter = false;
        subsetInvFilter = false;
    } else if (checked == "mid") {
        subsetStrList = removal.split(", ");
        subsetInvFilter = true;
        subsetFilter = false;
    } else if (checked == "small") {
        subsetStrList = subsetStr.split(",");
        subsetInvFilter = false;
        subsetFilter = true;
    } else {
        subsetInvFilter = false;
        subsetFilter = false;
    }

//    if(subsetFilter != checked) {
//        subsetFilter = checked;
//    }
    applyFilters();
}

void ResultsManager::filterPhases(bool checked) {
    if(phaseFilter != checked) {
        phaseFilter = checked;
        applyFilters();
    }
}

void ResultsManager::filterColHeaders(QString filter) {
    if(strFilter != filter) {
        strFilter = filter;
        applyFilters();
    }
}
