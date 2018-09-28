#include "resultsmanager.h"

const QString subsetStr = "ApexHeight,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,CstrPhase6,CulmsPerHill,CumDr,CumEt,CumIrrig,CumLr,CumTr,CumWUse,DryMatAboveGroundTotPop,DryMatResInternodePop.,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,GrainFillingStatus,GrainYieldPop,HarvestIndex,HaunIndex,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,Lai,LaiDead,LeafDeathPop,NumPhase,PanStructMass,PlantHeight,RootFront,RUE,Sla,SterilityTot,TrEff,WueTot,TmaxMoy";

ResultsManager::ResultsManager(QTableView * resultView, ResultsDataModel * resultModel, QObject *parent)
    : QObject(parent), resultModel(resultModel), resultView(resultView) {
    subsetFilter = false;
    phaseFilter = false;
    strFilter = "";
    subsetStrList = subsetStr.split(",");

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
        if( (!strFilter.isEmpty() && !startWith(header, strFilters))
            || (subsetFilter && !subsetStrList.contains(header, Qt::CaseInsensitive))) {
            resultView->horizontalHeader()->hideSection(i);
        } else {
            resultView->horizontalHeader()->showSection(i);
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
void ResultsManager::filterColumns(bool checked) {
    if(subsetFilter != checked) {
        subsetFilter = checked;
        applyFilters();
    }
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
