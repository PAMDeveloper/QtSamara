#include "chartmanager.h"

#include <QCheckBox>
#include <QDate>
#include <QHash>
#include <QtCore/qmath.h>

#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QDebug>

#include "utils/juliancalculator.h"

const QString chartDefaultList = "Cstr,CulmsPerHill,DeadLeafdrywtPop,DryMatAboveGroundTotPop,DryMatResInternodePop,DryMatStemPop,FTSW,GrainYieldPop,Ic,Lai,NumPhase";
//CumRain,Pluie,
//const QString chartCompleteList = "ApexHeight,Assim,AssimNotUsed,AssimNotUsedCum,AssimPot,AssimSurplus,CapaREvap,CapaRFE,Conversion,ConversionEff,Cstr,CstrAssim,CstrMean,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,CulmsPerHill,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumDr,CumEt,CumIrrig,CumLr,CumPAR,CumTr,CumWReceived,CumWUse,DayLength,DeadLeafdrywtPop,DegresDuJour,DegresDuJourCor,Density,Dr,DryMatAboveGroundPop,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStemPop,DryMatStructTotPop,DryMatTotPop,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETP,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FractionPlantHeightSubmer,FractionRootsLogged,FTSW,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GrainFillingStatus,GrainYieldPanicle,GrainYieldPop,HarvestIndex,HaunIndex,HMax,HMin,HMoy,Hum,Ic,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,Ins,InternodeResStatus,Irrigation,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LastLeafLength,LastLeafLengthPot,LeafDeathPop,LIRkdfcl,Lr,LTRkdfcl,MaxLai,MobiliLeafDeath,NumPhase,NurseryStatus,PanicleNumPlant,PanicleNumPop,PanicleSinkPop,PanStructMass,Par,PARIntercepte,PlantHeight,PlantLeafNumTot,PlantWidth,ProfRu,ResCapacityInternodePop,ReservePopFin,ResInternodeMobiliDay,ResInternodeMobiliDayPot,RespMaintDebt,RespMaintTot,ResUtil,Rg,RgCalc,RgMax,RootFront,RootMassPerVol,RootShootRatio,RUE,RUEGreen,RuRac,RuSurf,Sla,SpikeNumPanicle,SpikeNumPop,SterilityCold,SterilityDrought,SterilityHeat,SterilityTot,StockMacropores,StockRac,StockSurface,StockTotal,StressCold,SumDegreDayCor,SumDegresDay,SupplyTot,TillerDeathPop,TMax,TMin,TMoy,Tr,TrEff,TrEffInst,TrPot,VitesseRacinaire,VolMacropores,VolRelMacropores,VPDCalc,Vt,WueEt,WueTot" \
//                                  "MatuSDJ,MatuProgress,GrainMoisture,FreshMatPanicleTotPop,StemVigor,LodgingIndex,FreshMatAbovegroundPop,LodgingResistance,LodgingDay,Lodging,"
//                                  "StemSurfMean,StemDiaMean,StemDiaBase,LodgingPot,RootLigninPop,TmaxMoy";
const QString chart_to_remove = "DemStructLeaf, DemStructTot, GrowthStructLeaf, GrowthStructTot, IncreaseResInternodePot, ResInternodeMobiliDay, ApexHeightGain, AssimSurplus, ChangeNurseryStatus, ChangePhase, ChangeSsPhase, CoeffCO2Assim, CoeffCO2Tr, CoeffStressLogging, Conversion, CounterNursery, CstrCum, CstrMean, CstrPhase2, CstrPhase3, CstrPhase4, CstrPhase5, CstrPhase6, CulmsPerHillFin, CulmsPerPlantFin, CumFTSWPhase2, CumFTSWPhase3, CumFTSWPhase4, CumFTSWPhase5, CumFTSWPhase6, CumGrowthPop, CumIcPhase2, CumIcPhase3, CumIcPhase4, CumIcPhase5, CumIcPhase6, CumIrriFin, CumSupplyTot, CumWUseFin, DAF, Decli, DemLeafAreaPlant, DemPanicleFillPop, DemResInternodePop, DemStructInternodePlant, DemStructInternodePop, DemStructLeafPlant, DemstructLeafPop, DemStructRootPlant, DemStructRootPop, DemStructTotPop, Density, FloodwaterGain, FtswMoy, FtswPhase2, FtswPhase3, FtswPhase4, FtswPhase6, FtswPhase6, GainRootSystSoilSurfPop, GainRootSystVolPop, GrainFillingStatusFin, GrainYieldPopFin, Growth… (all variable names starting on Growth), HaunGain, HMin, HMax, HMoy, HMoyCalc, IcCum, IcMean, IcPhase2, IcPhase3, IcPhase4, IcPhase5, IcPhase6, IncreaseResInternodePop, LAIFin, LatRad, LIRkdf, LIRkdfcl, MatuProgress, MatuSDJ, MobiliLeafDeath, NumSsPhase, PhaseStemElongation, PlantLeafNumNew, RayExtra, ReservePopFin, ResInternodeMobiliFin, ResInternodeMobiliDayPot, RespMaintDebt, RgMax, RootFrontOld, RootSystSoilSurfPop, RootSystSoilSurfPopOld, RootSystVolPop, RootSystVolPopOld, SDJCorPhase4, SeuilCstrMortality, SeuilTemp, SeuilTempSsPhase, SimEmergence, SimEndCycle, SimPanIni, SimStartGermin, SimStartMatu2, SimStartPSP, SlaMitch, SlaNew, SommeDegresJourMax, SterilityTotFin, SumDDPhasePrec, SumDegresDay, SumPP, SunDistance, SunPosi, TempLAI, TmaxMoy, TminMoy, TMoyCalc, TMoyPrec";
const QString chartCompleteList = "A_AssimSurplus,A_DemStructLeaf,A_DemStructTot,A_GrowthStructLeaf,A_GrowthStructTot,DaysDrySowingToGermination,GerminationStatus,A_IncreaseResInternodePop,A_ResInternodeMobiliDay,ApexHeight,ApexHeightGain,Assim,AssimNotUsed,AssimNotUsedCum,AssimPot,AssimSurplus,CapaRDE,CapaREvap,CapaRFE,ChangeNurseryStatus,ChangePhase,ChangeSsPhase,CoeffCO2Assim,CoeffCO2Tr,CoeffStressLogging,Conversion,ConversionEff,CounterNursery,Cstr,CstrAssim,CstrCum,CstrMean,CstrPhase2,CstrPhase3,CstrPhase4,CstrPhase5,CstrPhase6,CulmsPerHill,CulmsPerHillFin,CulmsPerHillMax,CulmsPerPlant,CulmsPerPlantFin,CulmsPerPlantMax,CulmsPop,CumCarbonUsedPop,CumCstrPhase2,CumCstrPhase3,CumCstrPhase4,CumCstrPhase5,CumCstrPhase6,CumDr,CumDrFin,CumEt,CumFTSWPhase2,CumFTSWPhase3,CumFTSWPhase4,CumFTSWPhase5,CumFTSWPhase6,CumGrowthPop,CumIcPhase2,CumIcPhase3,CumIcPhase4,CumIcPhase5,CumIcPhase6,CumIrrig,CumIrrigFin,CumLr,CumPAR,cumRain,CumSupplyTot,CumTr,CumWReceived,CumWUse,CumWUseFin,DAF,DayLength,DeadLeafdrywtPop,Decli,DegresDuJour,DegresDuJourCor,DemLeafAreaPlant,DemPanicleFillPop,DemResInternodePop,DemStructInternodePlant,DemStructInternodePop,DemStructLeafPlant,DemStructLeafPop,DemStructPaniclePlant,DemStructPaniclePop,DemStructRootPlant,DemStructRootPop,DemStructSheathPop,DemStructTotPop,Density,Dr,DryMatAboveGroundPop,DryMatAboveGroundPopFin,DryMatAboveGroundTotPop,DryMatPanicleTotPop,DryMatResInternodePop,DryMatResInternodePopOld,DryMatStemPop,DryMatStructInternodePop,DryMatStructLeafPop,DryMatStructPaniclePop,DryMatStructRootPop,DryMatStructSheathPop,DryMatStructTotPop,DryMatTotPop,DryMatTotPopFin,DryMatVegeTotPop,DurGermFlow,DurGermMat,DurPhase1,DurPhase2,DurPhase3,DurPhase4,DurPhase5,DurPhase6,EauDispo,ETM,ETo,ETP,ETR,Evap,EvapPot,FertSpikeNumPop,FloodwaterDepth,FloodwaterGain,FractionPlantHeightSubmer,FractionRootsLogged,FreshMatAbovegroundPop,FreshMatPanicleTotPop,FTSW,FtswMoy,FtswPhase2,FtswPhase3,FtswPhase4,FtswPhase5,FtswPhase6,GainRootSystSoilSurfPop,GainRootSystVolPop,GrainFillingStatus,GrainFillingStatusFin,GrainMoisture,GrainYieldPanicle,GrainYieldPop,GrainYieldPopFin,GrowthDryMatPop,GrowthPop,GrowthResInternodePop,GrowthStructDeficit,GrowthStructInternodePop,GrowthStructLeafPop,GrowthStructPaniclePop,GrowthStructRootPop,GrowthStructSheathPop,GrowthStructTotPop,HarvestIndex,HaunGain,HaunIndex,HMax,HMin,HMoy,HMoyCalc,Hum,Ic,IcCum,IcMean,IcPhase2,IcPhase3,IcPhase4,IcPhase5,IcPhase6,IncreaseResInternodePop,Ins,InternodeResStatus,Irrigation,IrrigAutoDay,IrrigTotDay,Kce,KceReal,Kcl,Kcp,KcTot,Kr,KRolling,Lai,LaiDead,LaiFin,LastLeafLength,LastLeafLengthPot,LatRad,LeafDeathPop,LIRkdf,LIRkdfcl,Lodging,LodgingDay,LodgingIndex,LodgingPot,LodgingResistance,LodgingResistance2,Lr,LTRkdf,LTRkdfcl,MatuProgress,MatuSDJ,MaxLai,MobiliLeafDeath,NbJAS,NumPhase,NumSsPhase,NurseryStatus,PanicleFilDeficit,PanicleFilPop,PanicleNumPlant,PanicleNumPop,PanicleSinkPop,PanStructMass,Par,PARIntercepte,PhaseStemElongation,PlantHeight,PlantLeafNumNew,PlantLeafNumTot,PlantWidth,Pluie,ProfRu,RayExtra,RelPotLeafLength,ResCapacityInternodePop,ReservePopFin,ResInternodeMobiliDay,ResInternodeMobiliDayPot,RespMaintDebt,RespMaintTot,ResUtil,Rg,RgCalc,RgMax,RootFront,RootFrontOld,RootLigninPop,RootMassPerVol,RootShootRatio,RootSystSoilSurfPop,RootSystSoilSurfPopOld,RootSystVolPop,RootSystVolPopOld,RUE,RUEGreen,RuRac,RuSurf,SDJCorPhase4,SeuilCstrMortality,SeuilTemp,SeuilTempSsPhase,SimAnthesis50,SimEmergence,SimEndCycle,SimPanIni,SimStartGermin,SimStartMatu2,SimStartPSP,Sla,SlaMitch,SlaNew,SommeDegresJourMax,SpikeNumPanicle,SpikeNumPop,StemDiaBase,StemDiaMean,StemSurfMean,StemVigor,SterilityCold,SterilityDrought,SterilityHeat,SterilityTot,SterilityTotFin,StockMacropores,StockRac,StockSurface,StockTotal,StressCold,StRuMax,SumDDPhasePrec,SumDegreDayCor,SumDegresDay,SumPP,SunDistance,SunPosi,SupplyTot,TempLai,TillerDeathPop,TMax,TmaxMoy,TMin,TminMoy,TMoy,TMoyCalc,TMoyPrec,Tr,TrEff,TrEffInst,TrPot,ValRDE,ValRFE,ValRSurf,VitesseRacinaire,VitesseRacinaireDay,VolMacropores,VolRelMacropores,VPDCalc,Vt,WueEt,WueTot";
ChartManager::ChartManager(QGridLayout *chartLayout, QVBoxLayout *chartListLayout, QObject *parent)
    : QObject(parent), chartLayout(chartLayout), chartListLayout(chartListLayout)
{
    chartList = chartCompleteList.split(",");
    defaultList = chartDefaultList.split(",");
    removeList = chart_to_remove.split(", ");
    for (QString key: removeList) {
        for (QString srcKey: chartList) {
            if (key.toLower() == srcKey.toLower()){
                chartList.removeAll(srcKey);
                defaultList.removeAll(srcKey);
            }
        }
    }
    QStringList toRemove;
    for (QString srcKey: chartList) {
        qDebug() << srcKey << srcKey.startsWith("Growth", Qt::CaseInsensitive);
        if (srcKey.startsWith("Growth", Qt::CaseInsensitive)){
            toRemove.append(srcKey);
        }
        if (srcKey.startsWith("Dem", Qt::CaseInsensitive)) {
            toRemove.append(srcKey);
        }
        if (srcKey.startsWith("A_", Qt::CaseInsensitive)) {
            toRemove.append(srcKey);
        }
    }
    for (QString key: toRemove){
        chartList.removeAll(key);
        defaultList.removeAll(key);
    }
    for (QString srcKey: chartList) {
        qDebug() << srcKey;
    }
    sowingChecked = false;
    fillList();
}

void ChartManager::fillList() {
    defaultChecked = false;
    for(QString name: chartList) {
        QCheckBox * chartBox = new QCheckBox(name);
        chartBox->setObjectName(name + "box");
        charts.insert(name, createChart(name));
        chartListLayout->addWidget(chartBox);
        connect(chartBox, SIGNAL(clicked(bool)), this, SLOT(check(bool)));
    }
}

void ChartManager::check(bool checked) {
    if(resultSeries.empty()) {
        static_cast<QCheckBox *>(sender())->setChecked(false);
        return;
    }

    QString name = static_cast<QCheckBox *>(sender())->text();
    if(checked && !checkedList.contains(name))
        checkedList.append(name);

    if(!checked && checkedList.contains(name))
        checkedList.removeAll(name);

    displayCharts();
}

ChartView * ChartManager::createChart(QString name) {
    ChartView * chartView = new ChartView(name);
    chartView->setObjectName(name);
    return chartView;
}

void ChartManager::displayCharts() {
    for(QString name: chartList) {
        charts[name]->setVisible(false);
        chartLayout->removeWidget(chartLayout->parent()->findChild<QWidget*>(name));
    }

    for(QString name: checkedList) {
        if(resultSeries.find(name) == resultSeries.end()) {
            chartListLayout->parent()->findChild<QCheckBox*>(name+"box")->setChecked(false);
            continue;
        }
        if(charts[name]->series == nullptr)
            charts[name]->setSeries(resultSeries[name], obsSeries[name]);
        chartLayout->addWidget(charts[name], chartLayout->count() / 2, chartLayout->count() % 2);
        charts[name]->setSowing(sowingChecked);
        charts[name]->setVisible(true);
    }
    chartLayout->setColumnStretch(0,1);
    if(checkedList.count() > 1) {
        chartLayout->setColumnStretch(1,1);
    }
}

void ChartManager::fromSowing(bool checked) {
    sowingChecked = checked;
    displayCharts();
}

void ChartManager::setResults(pair<vector<string>, vector<vector<double> > > results, map<string, vector<double> > observations,
                              double /*startDate*/, double sowingDate) {
    clearSeries();
    generateResultSeries(results, sowingDate);
    generateObsSeries(observations, sowingDate);
    displayCharts();
}

void ChartManager::clearSeries() {
    for (auto token: resultSeries.toStdMap()) {
        delete token.second;
    }

    for (auto token: obsSeries.toStdMap()) {
        delete token.second;
    }

    resultSeries.clear();
    obsSeries.clear();

    for(QString name: chartList)
        charts[name]->clear();
}


QColor getColor(QString s) {
    int i = qHash(s) % 256;
    double PHI = (1 + qSqrt(5)) / 2;
    double n = i * PHI - floor(i * PHI);
    int h = qFloor(n * 256);
    return QColor::fromHsv(h, 245, 245, 255);
}

//#include <QDebug>
void ChartManager::generateResultSeries(pair <vector <string>, vector < vector <double> > > results, double sowing) {
    QDateTime date;
    for (int i = 0; i < results.first.size(); ++i) {
        QString name = QString::fromStdString(results.first[i]);
        QLineSeries * serie = new QLineSeries();
        serie->setColor(getColor(name));
        for (int d = 0; d < results.second[i].size(); ++d) {
            serie->append(results.second[0][d] - sowing, results.second[i][d]);
//            string dateStr = JulianCalculator::toStringDate(results.second[0][d], JulianCalculator::YMD, '-');
//            date.setDate(QDate::fromString(QString::fromStdString(dateStr),"yyyy-MM-dd"));
//            serie->append(date.toMSecsSinceEpoch(), results.second[i][d]);
        }
        resultSeries.insert(name, serie);
    }
}

void ChartManager::generateObsSeries(map<string, vector<double> > observations, double sowing) {
    QDateTime date;
    for (auto token: observations) {
        QString name = QString::fromStdString(token.first);
        QScatterSeries * serie = new QScatterSeries();
        serie->setColor(getColor(name).darker(200));
        serie->setMarkerSize(7);
        for (int d = 0; d < token.second.size(); ++d) {
//            string dateStr = JulianCalculator::toStringDate(observations["Jour"][d], JulianCalculator::YMD, '-');
//            date.setDate(QDate::fromString(QString::fromStdString(dateStr),"yyyy-MM-dd"));
            if(token.second[d] != -999)
                serie->append(observations["ObsPlantDate"][d] - sowing, token.second[d]);
        }
        obsSeries.insert(name, serie);
    }
}

void ChartManager::selectAll(bool checked) {
    if(resultSeries.empty()) {
        static_cast<QCheckBox *>(sender())->setChecked(false);
        return;
    }
    if(!checked) {
        for(QString name: chartList) {
            if(!defaultChecked || !defaultList.contains(name)) {
                checkedList.removeAll(name);
                chartListLayout->parent()->findChild<QCheckBox*>(name + "box")->setChecked(false);
            }
        }
    } else {
        checkedList.clear();
        for(QString name: chartList) {
            if(!checkedList.contains(name)) {
                checkedList.append(name);
                chartListLayout->parent()->findChild<QCheckBox*>(name + "box")->setChecked(true);
            }
        }
    }

    displayCharts();
}

void ChartManager::selectDefault(bool checked) {
    if(resultSeries.empty()) {
        static_cast<QCheckBox *>(sender())->setChecked(false);
        return;
    }
    defaultChecked = checked;
    if(checked) {
        for(QString name: defaultList) {
            if(!checkedList.contains(name)) {
                checkedList.append(name);
                chartListLayout->parent()->findChild<QCheckBox*>(name + "box")->setChecked(true);
            }
        }
    } else {
        for(QString name: defaultList) {
            if(checkedList.contains(name)) {
                checkedList.removeAll(name);
                chartListLayout->parent()->findChild<QCheckBox*>(name + "box")->setChecked(false);
            }
        }
    }

    displayCharts();
}


