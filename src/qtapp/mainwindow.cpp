#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChartView>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCharts/QValueAxis>

#include <QtCore/qmath.h>
#include <QMap>
#include <QDesktopWidget>
#include <QVBoxLayout>

#include <QTabWidget>
#include <QDir>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>

#include <qtapp/view.h>
#include <samara.h>
#include <utils/juliancalculator.h>

#include <cmath>

#include <QDebug>

static const int numCol = 2;

QT_CHARTS_USE_NAMESPACE

QStringList fromVector(vector<string> list) {
    QStringList r;
    for (int i = 0; i < list.size(); ++i) {
        r << QString::fromStdString(list[i]);
    }
    //    r.sort();
    return r;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    settings = new QSettings("PAM Cirad", "Samara");
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);

    resultsModel = new ResultsDataModel();
    ui->resultsTableView->setModel(resultsModel);

    resultsManager = new ResultsManager(ui->resultsTableView, resultsModel);
    connect(ui->colFilterCheckbox, SIGNAL(toggled(bool)), resultsManager, SLOT(filterColumns(bool)));
    connect(ui->phaseFilterCheckbox, SIGNAL(toggled(bool)), resultsManager, SLOT(filterPhases(bool)));
    connect(ui->filterColLineEdit, SIGNAL(textChanged(QString)), resultsManager, SLOT(filterColHeaders(QString)));

    SamaraParameters * paramsSam = new SamaraParameters();
    loader = new DBAccessLoader(paramsSam);
    loadDB(settings->value("SamaraDB_path").toString());

    chartManager = new ChartManager();


    comparisonManager = new ComparisonManager();

    createChartsTab();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_loadDbButton_clicked() {
    QString dirPath = settings->value("SamaraDB_folder", QDir::currentPath()).toString();
    QString filePath = QFileDialog::getOpenFileName(this, "Open database", dirPath , "Access DB (*.mdb *.accdb)");
    if(filePath.isEmpty()) return;

    settings->setValue("SamaraDB_path", filePath);
    settings->setValue("SamaraDB_folder", QFileInfo(filePath).absoluteDir().absolutePath());
    loadDB(filePath);
}

void MainWindow::loadDB(QString filePath) {
    if(filePath.isEmpty()) return;
    if(!QFileInfo(filePath).exists()) return;

    clearDBContext();
    loader->openDb(filePath);
    fillDBCombos();
}

void MainWindow::fillDBCombos() {
    ui->varComboBox->addItems(fromVector(loader->load_variety_list()));
    ui->plotComboBox->addItems(fromVector(loader->load_plot_list()));
    ui->stationComboBox->addItems(fromVector(loader->load_station_list()));
    ui->itinComboBox->addItems(fromVector(loader->load_itinerary_list()));
    ui->simComboBox->addItems(fromVector(loader->load_simulation_list()));
}


void MainWindow::clearDBContext() {
    delete ui->parametersTableView->model();
    delete ui->meteoTableView->model();

    results.first.clear();
    results.second.clear();
    resultsModel->setResults(results);

    ui->varComboBox->clear();
    ui->plotComboBox->clear();
    ui->stationComboBox->clear();
    ui->itinComboBox->clear();
    ui->simComboBox->clear();
}

void MainWindow::on_launchButton_clicked() {
    results = run_samara_2_1(loader->parameters);
    resultsModel->setResults(results);
    ui->resultsTableView->reset();
    observations = loader->load_obs("");
}


void MainWindow::on_simComboBox_currentTextChanged(const QString &arg1) {
    loader->load_simulation(arg1.toStdString());
    string sStart = JulianCalculator::toStringDate(loader->parameters->getDouble("startingdate"),
                                                   JulianCalculator::YMD, '-');
    string sEnd = JulianCalculator::toStringDate(loader->parameters->getDouble("endingdate"),
                                                   JulianCalculator::YMD, '-');

    QDate start = QDate::fromString(QString::fromStdString(sStart),"yyyy-MM-dd");
    QDate end = QDate::fromString(QString::fromStdString(sEnd),"yyyy-MM-dd");
    ui->startDateEdit->blockSignals(true);
    ui->startDateEdit->setDate(start);
    ui->startDateEdit->blockSignals(false);
    ui->endDateEdit->blockSignals(true);
    ui->endDateEdit->setDate(end);
    ui->endDateEdit->blockSignals(false);

    ui->varComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("variety")));
    ui->plotComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("plotcode")));
    ui->itinComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("itkcode")));
    ui->stationComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("wscode")));
}

void MainWindow::on_varComboBox_currentTextChanged(const QString &arg1) {
    loader->load_variety(arg1.toStdString());
    showParameters(loader->parameters);
}

void MainWindow::on_stationComboBox_currentTextChanged(const QString &arg1) {
    loader->load_station(arg1.toStdString());
    loader->load_meteo(arg1.toStdString(), loader->parameters->getDouble("startingdate"),
                       loader->parameters->getDouble("endingdate"));
    showParameters(loader->parameters);
}

void MainWindow::on_plotComboBox_currentTextChanged(const QString &arg1) {
    loader->load_plot(arg1.toStdString());
    showParameters(loader->parameters);
}

void MainWindow::on_itinComboBox_currentTextChanged(const QString &arg1) {
    loader->load_itinerary(arg1.toStdString());
    showParameters(loader->parameters);
}

void MainWindow::on_startDateEdit_dateChanged(const QDate &date) {
    loader->parameters->doubles["startingdate"].first = date.toJulianDay();
    loader->parameters->strings["startingdate"].first = date.toString("yyyy-MM-dd").toStdString();
    loader->load_meteo(ui->stationComboBox->currentText().toStdString(),
                       loader->parameters->getDouble("startingdate"),
                       loader->parameters->getDouble("endingdate"));
    showParameters(loader->parameters);
}

void MainWindow::on_endDateEdit_dateChanged(const QDate &date) {
    loader->parameters->doubles["endingdate"].first = date.toJulianDay();
    loader->parameters->strings["endingdate"].first = date.toString("yyyy-MM-dd").toStdString();
    loader->load_meteo(ui->stationComboBox->currentText().toStdString(),
                       loader->parameters->getDouble("startingdate"),
                       loader->parameters->getDouble("endingdate"));
    showParameters(loader->parameters);
}


void MainWindow::showParameters(SamaraParameters *parameters) {
    if(ui->parametersTableView->model() != nullptr)
        delete ui->parametersTableView->model();

    ParametersDataModel *paramModel = new ParametersDataModel(parameters);
    ui->parametersTableView->setModel(paramModel);

    if(ui->meteoTableView->model() != nullptr)
        delete ui->meteoTableView->model();
    MeteoDataModel *meteoModel = new MeteoDataModel(parameters);
    ui->meteoTableView->setModel(meteoModel);
}


void MainWindow::on_saveResultButton_clicked() {
    QString dirPath = settings->value("SamaraResult_folder", QDir::currentPath()).toString();
    QString selectedFilter;
    QString filePath = QFileDialog::getSaveFileName(
                this, "Save results as csv", dirPath , "csv tab separated (*.csv);;csv semicolon separated (*.csv)",&selectedFilter);
    if(filePath.isEmpty()) return;
    settings->setValue("SamaraResult_folder", filePath);
    QString sep = (selectedFilter == "csv tab separated (*.csv)" ? "\t" : ";");
    resultsModel->save(filePath, sep);
}












serieCompare MainWindow::compareSeries(QLineSeries * src, QScatterSeries * ref) {
    serieCompare comparison;
    comparison.maxVal = -999999999999999;
    comparison.minVal = 999999999999999;
    comparison.sumSrc = 0;
    comparison.sumRef = 0;
    comparison.diffStep = -1;
    comparison.diffPercent = 1;
    comparison.valid = true;

    //    if(src == NULL || ref == NULL) {
    //        comparison.valid = false;
    //        return comparison;
    //    }

    int serieSize = src->count();

    for (int i = 0; i < serieSize; ++i) {
        double srcVal = src != NULL ? src->at(i).y() : 0;
        //        double refVal = ref != NULL ? ref->at(i).y() : 0;
        if (srcVal > comparison.maxVal) comparison.maxVal = srcVal;
        if (srcVal < comparison.minVal) comparison.minVal = srcVal;
        comparison.sumSrc += srcVal;
        //        if (refVal > comparison.maxVal) comparison.maxVal = refVal;
        //        if (refVal < comparison.minVal) comparison.minVal = refVal;
        //        comparison.sumRef += refVal;

        //        if( std::abs((srcVal-refVal)/refVal) > 0.0001 && comparison.diffStep == -1 ) comparison.diffStep = i;
    }
    //    comparison.diffPercent = ((comparison.sumRef-comparison.sumSrc)/comparison.sumRef) * 100;
    return comparison;
}


QColor getColor(int i) {
    double PHI = (1 + qSqrt(5)) / 2;
    double n = i * PHI - floor(i * PHI);
    int h = qFloor(n * 256);
    return QColor::fromHsv(h, 245, 245, 255);
}

bool MainWindow::addChart(int row, int col,
                          QLineSeries *resultsSeries,
                          QScatterSeries *refSeries,
                          QString name) {

    serieCompare comparison = comparisons[headers.indexOf(name)];
    if(!comparison.valid)
        return false;

    QString titleName = name;
    QChart *chart = new QChart();
    QColor color = getColor(row * numCol + col);
    QPen pen;
    if(refSeries != NULL) {
        refSeries->setColor(color.darker(150));
//        pen.setWidth(1);
        pen.setColor(color);
        refSeries->setMarkerSize(5);
//        refSeries->setMarkerSize(1);
        refSeries->setPen(pen);
        titleName += " + ref";
        chart->addSeries(refSeries);
    }

    resultsSeries->setColor(color/*.darker(250)*/);
    pen.setWidth(1);
    pen.setColor(color/*.darker(250)*/);
    resultsSeries->setPen(pen);
    chart->addSeries(resultsSeries);

    chart->legend()->hide();
    chart->setTitle(titleName);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    //    axisX->setTickCount(10);
    axisX->setFormat("dd MM");
    chart->addAxis(axisX, Qt::AlignBottom);
    resultsSeries->attachAxis(axisX);
    if(refSeries != NULL)
        refSeries->attachAxis(axisX);

    //    if(/*std::abs(comparison.diffPercent) <= 0.00001 ||*/ std::abs(comparison.sumSrc/*sumRef*/) <= 0.00001) {
    //        return false;
    //    }
    //    QDateTime date;
    //    date.setDate(startDate);
    //    date = date.addDays(comparison.diffStep);
    //    qDebug() << name << ":"  << comparison.diffPercent << "% at step: " << comparison.diffStep << ":" << date.toString("dd-MM");

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setMax(comparison.maxVal * 1.1 + 0.001);
//    axisY->setMin(qMin<double>(-0.001, comparison.minVal - (comparison.maxVal - comparison.minVal) * 0.1));
    axisY->setMin(comparison.minVal);
    //    if(resultsSeries != NULL)
    resultsSeries->attachAxis(axisY);

    if (refSeries != NULL)
        refSeries->attachAxis(axisY);

    ChartView *chartView = new ChartView(chart, resultsSeries, refSeries, this);
    lay->addWidget(chartView, row, col);
    return true;
}

void MainWindow::chartClicked(bool clicked) {
    QCheckBox * chartBox = (QCheckBox *)sender();
    if(clicked)
        chartVisibles.append(chartBox->text().toLower());
    else
        chartVisibles.removeAll(chartBox->text().toLower());

    displayData(results);

}

void MainWindow::createChartsTab() {
    //    QScrollArea *scrollArea = new QScrollArea;
    QWidget *client = new QWidget();
    ui->scrollArea_2->setWidget(client);
    ui->scrollArea_2->setWidgetResizable(true);

    lay = new QGridLayout();
    client->setLayout(lay);
    lay->setSpacing(0);
    //    ui->tabWidget->addTab(scrollArea, "Variables");
    //    ui->tabWidget->setCurrentIndex(2);


    QWidget *client2 = new QWidget();
    ui->scrollArea->setWidget(client2);
    ui->scrollArea->setWidgetResizable(true);

    QVBoxLayout * lay2 = new QVBoxLayout();
    client2->setLayout(lay2);
    lay2->setSpacing(0);
    vector <string> names {"ObsPlantDate","ApexHeight","ApexHeightGain","Assim","AssimNotUsed","AssimNotUsedCum","AssimPot","AssimSurplus","A_AssimSurplus","A_DemStructLeaf","A_DemStructTot","A_GrowthStructLeaf","A_GrowthStructTot","A_IncreaseResInternodePop","A_ResInternodeMobiliDay","CapaRDE","CapaREvap","CapaRFE","ChangeNurseryStatus","ChangePhase","ChangeSsPhase","CoeffCO2Assim","CoeffCO2Tr","CoeffStressLogging","Conversion","ConversionEff","CounterNursery","Cstr","CstrAssim","CstrCum","CstrMean","CstrPhase2","CstrPhase3","CstrPhase4","CstrPhase5","CstrPhase6","CulmsPerHill","CulmsPerHillFin","CulmsPerHillMax","CulmsPerPlant","CulmsPerPlantFin","CulmsPerPlantMax","CulmsPop","CumCarbonUsedPop","CumCstrPhase2","CumCstrPhase3","CumCstrPhase4","CumCstrPhase5","CumCstrPhase6","CumDr","CumEt","CumFTSWPhase2","CumFTSWPhase3","CumFTSWPhase4","CumFTSWPhase5","CumFTSWPhase6","CumGrowthPop","CumIcPhase2","CumIcPhase3","CumIcPhase4","CumIcPhase5","CumIcPhase6","CumIrrig","CumIrrigFin","CumLr","CumPAR","CumSupplyTot","CumTr","CumWReceived","CumWUse","CumWUseFin","DAF","DayLength","DeadLeafdrywtPop","Decli","DegresDuJour","DegresDuJourCor","DemLeafAreaPlant","DemPanicleFillPop","DemResInternodePop","DemStructInternodePlant","DemStructInternodePop","DemStructLeafPlant","DemStructLeafPop","DemStructPaniclePlant","DemStructPaniclePop","DemStructRootPlant","DemStructRootPop","DemStructSheathPop","DemStructTotPop","Density","Dr","DryMatAboveGroundPop","DryMatAboveGroundPopFin","DryMatAboveGroundTotPop","DryMatPanicleTotPop","DryMatResInternodePop","DryMatResInternodePopOld","DryMatStructInternodePop","DryMatStructLeafPop","DryMatStructPaniclePop","DryMatStructRootPop","DryMatStructSheathPop","DryMatStructStemPop","DryMatStructTotPop","DryMatTotPop","DryMatTotPopFin","DryMatVegeTotPop","DurGermFlow","DurGermMat","DurPhase1","DurPhase2","DurPhase3","DurPhase4","DurPhase5","DurPhase6","EauDispo","ETM","ETo","ETR","Evap","EvapPot","FertSpikeNumPop","FloodwaterDepth","FloodwaterGain","FractionPlantHeightSubmer","FractionRootsLogged","FTSW","FtswMoy","FtswPhase2","FtswPhase3","FtswPhase4","FtswPhase5","FtswPhase6","GainRootSystSoilSurfPop","GainRootSystVolPop","GrainFillingStatus","GrainFillingStatusFin","GrainYieldPanicle","GrainYieldPop","GrainYieldPopFin","GrowthDryMatPop","GrowthPop","GrowthResInternodePop","GrowthStructDeficit","GrowthStructInternodePop","GrowthStructLeafPop","GrowthStructPaniclePop","GrowthStructRootPop","GrowthStructSheathPop","GrowthStructTotPop","HarvestIndex","HaunGain","HaunIndex","HMoyCalc","Hum","Ic","IcCum","IcMean","IcPhase2","IcPhase3","IcPhase4","IcPhase5","IcPhase6","IncreaseResInternodePop","InternodeResStatus","Irrigation","IrrigAutoDay","IrrigTotDay","Kce","KceReal","Kcl","Kcp","KcTot","Kr","KRolling","Lai","LaiDead","LaiFin","LastLeafLength","LastLeafLengthPot","LatRad","LeafDeathPop","LIRkdf","LIRkdfcl","Lr","LTRkdf","LTRkdfcl","MaxLai","MobiliLeafDeath","NumPhase","NumSsPhase","NurseryStatus","PanicleFilDeficit","PanicleFilPop","PanicleNumPlant","PanicleNumPop","PanicleSinkPop","PanStructMass","Par","PARIntercepte","PhaseStemElongation","PlantHeight","PlantLeafNumNew","PlantLeafNumTot","PlantWidth","ProfRu","RayExtra","RelPotLeafLength","ResCapacityInternodePop","ReservePopFin","ResInternodeMobiliDay","ResInternodeMobiliDayPot","RespMaintDebt","RespMaintTot","ResUtil","RgCalc","RgMax","RootFront","RootFrontOld","RootMassPerVol","RootShootRatio","RootSystSoilSurfPop","RootSystSoilSurfPopOld","RootSystVolPop","RootSystVolPopOld","RUE","RuRac","RuSurf","SDJCorPhase4","SeuilCstrMortality","SeuilTemp","SeuilTempSsPhase","SimAnthesis50","SimEmergence","SimEndCycle","SimPanIni","SimStartGermin","SimStartMatu2","SimStartPSP","Sla","SlaMitch","SlaNew","SommeDegresJourMax","SpikeNumPanicle","SpikeNumPop","SterilityCold","SterilityDrought","SterilityHeat","SterilityTot","SterilityTotFin","StockMacropores","StockRac","StockSurface","StockTotal","StressCold","StRuMax","SumDDPhasePrec","SumDegreDayCor","SumDegresDay","SumPP","SunDistance","SunPosi","SupplyTot","TempLai","TillerDeathPop","TmaxMoy","TminMoy","TMoyCalc","TMoyPrec","Tr","TrEff","TrEffInst","TrPot","ValRDE","ValRFE","ValRSurf","VPDCalc","VitesseRacinaire","VitesseRacinaireDay","VolMacropores","VolRelMacropores","WueEt","WueTot"};
//    for (int i = 0; i < names.size(); ++i) {
//        std::transform(names[i].begin(), names[i].end(), names[i].begin(), ::tolower);
//    }
    //    vector <string> names {"Jour","NbJas","NumPhase","DegresDuJour","DegresDuJourCor","Lai","MaxLai","FTSW","Cstr","RootFront","Eto","ETM","ETR","RUE","ConversionEff","Ic","CulmsPerPlant","CulmsPop","CulmsPerHill","GrainYieldPop","DryMatStructLeafPop","DryMatStructSheathPop","DryMatStructRootPop","DryMatStructInternodePop","DryMatResInternodePop","DryMatStructPaniclePop","DryMatStructTotPop","DryMatVegeTotPop","DryMatPanicleTotPop","DryMatStemPop","DryMatAboveGroundPop","DryMatTotPop","SterilityCold","SterilityHeat","SterilityDrought","SterilityTot","HarvestIndex","PanicleNumPop","PanicleNumPlant","GrainYieldPanicle","SpikeNumPop","SpikeNumPanicle","FertSpikeNumPop","GrainFillingStatus","PhaseStemElongation","Sla","HaunIndex","ApexHeight","PlantHeight","PlantWidth","VitesseRacinaireDay","Kcl","KRolling","LIRkdfcl","LTRkdfcl","AssimPot","Assim","RespMaintTot","SupplyTot","AssimSurplus","AssimNotUsed","AssimNotUsedCum","IcMean","CstrMean","TillerDeathPop","PlantLeafNumTot","DeadLeafdrywtPop","LaiDead","ResCapacityInternodePop","InternodeResStatus","DayLength","Par","RgCalc","VDPCalc","TMoyCalc","HMoyCalc","KceReal","StockTotal","EauDispo","StockSurface","StockRac","RURac","Kcp","Kce","EvapPot","Evap","TrPot","Tr","Lr","Dr","ParIntercepte","SumDegresDay","VitesseRacinaire","CstrAssim","RayExtra","CumPAR","SumDegreDayCor","SumPP","SumDDPhasePrec","SeuilTemp","RespMaintDebt","TMinMoy","TMaxMoy","FtswMoy","ResInternodeMobiliDay","RootShootRatio","TrEffInst","TrEff","WueEt","WueTot","SlaMitch","FloodwaterDepth","IrrigAutoDay","IrrigTotDay","FractionPlantHeightSubmer","Density","RootMassPerVol","CumTr","CumEt","CumWUse","CumWReceived","CumIrrig","CumDr","CumLr","VolMacropores","StockMacropores","IcCum","CstrCum","SlaNew","LTRkdf","LIRkdf","RelPotLeafLength","ApexHeightGain","Haungain","Conversion","RootFrontOld","SimStartGermin","SimEmergence","SimStartPSP","SimPanIni","SimAnthesis50","SimStartMatu2","SimEndCycle","StressCold","CulmsPerPlantMax","CulmsPerHillMax","DurGermFlow","DurGermMat","LaiFin","CulmsPerHillFin","CulmsPerPlantFin","GrainYieldPopFin","DryMatAboveGroundPopFin","ReservePopFin","FractionRootsLogged","ResUtil","KcTot","DryMatAboveGroundTotPop","DemResInternodePop","LastLeafLengthPot","LastLeafLength","CumSupplyTot","DryMatResInternodePopOld","CumGrowthPop","GrowthPop","A_DemStructLeaf","A_GrowthStructLeaf","A_DemStructTot","A_GrowthStructTot","CoeffCO2Tr","CoeffCO2Assim","A_AssimSurplus","A_IncreaseResInternodePop","A_ResInternodeMobiliDay","CumCarbonUsedPop","DryMatTotPopFin","GrainFillingStatusFin","SterilityTotFin","CumIrrigFin","CumWUseFin","GrowthDryMatPop_V2_1","RUEgreen"};
    for (int i = 0; i < names.size(); ++i) {
        QCheckBox * chartBox = new QCheckBox(QString::fromStdString(names[i]));
        lay2->addWidget(chartBox);
        connect(chartBox, SIGNAL(clicked(bool)), this, SLOT(chartClicked(bool)));
    }
}

QMap < QString, QLineSeries * > MainWindow::getRefSeries(QString refFileName) {
    QMap < QString, QLineSeries * > refSeries;
    if (!refFileName.isNull() && !refFileName.isEmpty() && QFileInfo(refFileName).exists()) {
        QFile file(refFileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        QStringList headers = in.readLine().split('\t');
        foreach (QString header, headers) {
            refSeries.insert(header, new QLineSeries());
        }
        QDateTime date;
        date.setDate(startDate);
        int l = 0;
        while (!in.atEnd()) {
            l++;
            QStringList vals = in.readLine().split('\t');
            for (int i = 0; i < vals.size(); ++i) {
                refSeries[headers[i]]
                        ->append(date.toMSecsSinceEpoch(),
                                 vals[i].replace(",", ".")
                                 .toDouble());
            }
            date = date.addDays(1);
        }
        file.close();
    }
    return refSeries;
}

QMap < QString, QLineSeries * > MainWindow::getResultSeries(pair <vector <string>, vector < vector <double> > > results) {
    QMap < QString, QLineSeries * > resultsSeries;
    for (int i = 0; i < results.first.size(); ++i) {
        QLineSeries * serie = new QLineSeries();
        QDateTime date;
        date.setDate(startDate);
        for (int d = 0; d < results.second[i].size(); ++d) {
            serie->append(date.toMSecsSinceEpoch(), results.second[i][d]);
            date = date.addDays(1);
        }
        resultsSeries.insert(QString::fromStdString(results.first[i]), serie);
    }
    return resultsSeries;
}

QMap < QString, QScatterSeries * > MainWindow::getObsSeries(map<string, vector<double>> observations){
    QMap < QString, QScatterSeries * > resultsSeries;

    for (auto token: observations) {
        QScatterSeries * serie = new QScatterSeries();
        QDateTime date;
        for (int d = 0; d < token.second.size(); ++d) {
            date.setDate(QDate::fromJulianDay(observations["Jour"][d]));
//            qDebug() << date.toString("MM-dd") << token.second[d];
            if(token.second[d] != -999)
                serie->append(date.toMSecsSinceEpoch(), token.second[d]);
        }
//        qDebug() << "APPEND " << QString::fromStdString(token.first);
        resultsSeries.insert(QString::fromStdString(token.first).toLower(), serie);
    }
    return resultsSeries;
}

void MainWindow::displayData(pair <vector <string>, vector < vector <double> > > results) {
    startDate = QDate::fromString(
                QString::fromStdString(loader->parameters->getString("startingdate"/*"datedebut"*/)).split("T")[0],
            "yyyy-MM-dd");

    QMap < QString, QScatterSeries * > obsSeries = getObsSeries(observations);
    QMap < QString, QLineSeries * > resultsSeries = getResultSeries(results);

    headers << /*obsSeries.keys() << */resultsSeries.keys();
    headers = headers.toSet().toList();
    headers.sort(Qt::CaseInsensitive);

    QLayoutItem *item;
    while((item = lay->takeAt(0))) {
        if (item->layout()) {
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    int j = 0;
    foreach (QString key, headers) {
        comparisons.push_back(compareSeries(resultsSeries[key], obsSeries[key]));
        //        if(showCharts)
        if(chartVisibles.contains(key))
            if( addChart(j / numCol, j % numCol, resultsSeries[key], obsSeries[key], key) )
                j++;
    }
}

void MainWindow::sectionClicked(int row) {
    //    QMessageBox::information(this, "Clicked", QString::number(row));
    bool all_good = true;
    QString arg1 = ui->lineEdit->text();
    for(int i = comparisonModel->columnCount() - 1; i >= 0 ; i--){
        if(comparisonModel->index(row, i).data(Qt::UserRole).toBool()) {
            ui->comparisonTableView->hideColumn(i);
        } else {
            ui->comparisonTableView->showColumn(i);
            all_good = false;
        }
        if(comparisonModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().startsWith(arg1) && !arg1.isEmpty()) {
            ui->comparisonTableView->showColumn(i);
        }
    }

    if(all_good) {
        for(int i = comparisonModel->columnCount() - 1; i >= 0 ; i--){
            ui->comparisonTableView->showColumn(i);
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    for(int i = comparisonModel->columnCount() - 1; i >= 0 ; i--){
        ui->comparisonTableView->showColumn(i);
    }
}


void MainWindow::loadRefFile(QString fileName){
    QFile inputFile(fileName);
    vector < vector <double> > values;
    vector <string> names;
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        QStringList list = in.readLine().split('\t');
        for(QString header: list) {
            names.push_back(header./*toLower().*/toStdString());
            values.push_back(vector<double>());
        }

        while (!in.atEnd())
        {
            list = in.readLine().split('\t');
            for (int i = 0; i < list.size(); ++i) {
                values[i].push_back(list[i].replace(',','.').toDouble());
            }
        }
        inputFile.close();
    }
    comparisonModel = new ComparisonDataModel2(
                results,
                pair <vector <string>, vector < vector <double> > > (names, values)
                );
    ui->comparisonTableView->setModel(comparisonModel);
//    ui->comparisonTableView->
    connect(ui->comparisonTableView->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionClicked(int)));
}

void MainWindow::on_oldResButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open results");
    loadRefFile(fileName);
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    for(int i = comparisonModel->columnCount() - 1; i >= 0 ; i--){
        if(!comparisonModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().startsWith(arg1)) {
            ui->comparisonTableView->hideColumn(i);
        } else {
            ui->comparisonTableView->showColumn(i);
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    comparisonModel->save();
}


