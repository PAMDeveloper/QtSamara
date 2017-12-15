#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChartView>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

#include <QtCore/qmath.h>
#include <QMap>
#include <QDesktopWidget>
#include <QVBoxLayout>

#include <QTabWidget>
#include <QDir>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>

#include <qtapp/chartview.h>
#include <samara.h>
#include <utils/juliancalculator.h>

#include <cmath>

#include <QDebug>

static const int numCol = 2;


const QString shortParamList = "BundHeight,Ca,CoeffInternodeMass,CoeffLeafDeath,CoeffLeafWLRatio,CoeffPanicleMass,CoeffTillerDeath,CoeffTransplantingShock,DatesSemis,DensityField,DensityNursery,DurationNursery,EpaisseurProf,EpaisseurSurf,FtswIrrig,HumCR,HumFC,HumPF,HumSat,InternodeLengthMax,IrrigAuto,IrrigAutoResume,IrrigAutoStop,IrrigAutoTarget,KcMax,LeafLengthMax,LifeSavingDarainage,Mulch,PercolationMax,PEvap,PFactor,Phyllo,PlantsPerHill,PlotDrainageDAF,PoidsSecGrain,PourcRuiss,PPSens,ProfRacIni,RootFrontMax,RU,SDJBVP,SeuilRuiss,StockIniProf,StockIniSurf,TilAbility,Transplanting,TransplantingDepth,TxConversion";

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
    setWindowTitle( QCoreApplication::applicationName() );
    ui->modelCombo->addItem("Samara 2.1");
    ui->modelCombo->addItem("Samara 2.1 michael");
    ui->modelCombo->addItem("Samara 2.3");
    ui->modelCombo->setCurrentText(settings->value("Samara_version","Samara 2.3").toString());

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

    createChartsTab();
    chartManager = new ChartManager(chartLayout, chartListLayout);
    connect(ui->selectAllBox, SIGNAL(clicked(bool)), chartManager, SLOT(selectAll(bool)));
    connect(ui->selectDefaultBox, SIGNAL(clicked(bool)), chartManager, SLOT(selectDefault(bool)));
    connect(ui->sowingButton, SIGNAL(clicked(bool)), chartManager, SLOT(fromSowing(bool)));

    comparisonManager = new ComparisonManager();

    int log = settings->value("SamaraSimulation_LOG", 0).toInt();
    if(log == 0) ui->smallRadio->setChecked(true);
    else if(log == 1) ui->completeRadio->setChecked(true);
    else ui->ecotropRadio->setChecked(true);

    //**//
//    ui->samaraTabs->removeTab(2);
    //**//
//    on_launchButton_clicked();
//    loadRefFile("D:/PAMStudio/dev/git/build-Samara-Desktop_Qt_5_9_1_MSVC2015_64bit-Release/Sortie Ecotrop.txt");
//    loadRefFile("D:/PAMStudio/dev/git/build-Samara-Desktop_Qt_5_9_1_MSVC2015_64bit-Release/test_621b.txt");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionLoad_database_triggered()
{
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
    ui->varComboBox->blockSignals(true);
    ui->plotComboBox->blockSignals(true);
    ui->stationComboBox->blockSignals(true);
    ui->itinComboBox->blockSignals(true);
    ui->simComboBox->blockSignals(true);
    ui->varComboBox->addItems(fromVector(loader->load_variety_list()));
    ui->plotComboBox->addItems(fromVector(loader->load_plot_list()));
    ui->stationComboBox->addItems(fromVector(loader->load_station_list()));
    ui->itinComboBox->addItems(fromVector(loader->load_itinerary_list()));
    ui->simComboBox->addItems(fromVector(loader->load_simulation_list()));
    ui->simComboBox->setCurrentText(settings->value("SamaraSimulation_Text", ui->simComboBox->itemText(0)).toString());
    loader->load_complete_simulation(ui->simComboBox->currentText().toStdString());
    ui->varComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("variety")));
    ui->plotComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("fieldcode")));
    ui->itinComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("itkcode")));
    ui->stationComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("wscode")));
    ui->varComboBox->blockSignals(false);
    ui->plotComboBox->blockSignals(false);
    ui->stationComboBox->blockSignals(false);
    ui->itinComboBox->blockSignals(false);
    ui->simComboBox->blockSignals(false);
    showDates();
    showParameters(loader->parameters);
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
    settings->setValue("SamaraSimulation_Text", ui->simComboBox->currentText());
    SamaraLogType log = (SamaraLogType)(ui->smallRadio->isChecked() ? 0 : ui->completeRadio->isChecked() ? 1 : 2);
    settings->setValue("SamaraSimulation_LOG", log);
    QString version = ui->modelCombo->currentText();
    settings->setValue("Samara_version", version);
    if(version == "Samara 2.1")
        results = run_samara_2_1(loader->parameters, log);
    else if(version == "Samara 2.1 michael")
        results = run_samara_2_1_micha(loader->parameters, log);
    else if(version == "Samara 2.3")
        results = run_samara_2_3(loader->parameters, log);

    resultsModel->setResults(results);
    ui->resultsTableView->reset();
    observations = loader->load_obs("");
    chartManager->setResults(results, observations, loader->parameters->getDouble("startingdate"), loader->parameters->getDouble("sowing"));
//    QMessageBox::information(this, "Simulation", "Simulation and charting done.");
}

void MainWindow::showDates() {
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
}

void MainWindow::on_simComboBox_currentTextChanged(const QString &arg1) {
    loader->load_simulation(arg1.toStdString());
    showDates();
    ui->varComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("variety")));
    ui->plotComboBox->setCurrentText(QString::fromStdString(loader->parameters->getString("fieldcode")));
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


void MainWindow::createChartsTab() {
    QWidget *client = new QWidget();
    ui->scrollArea_2->setWidget(client);
    ui->scrollArea_2->setWidgetResizable(true);
    chartLayout = new QGridLayout();
    client->setLayout(chartLayout);
    chartLayout->setSpacing(0);
    QWidget *client2 = new QWidget();
    ui->scrollArea->setWidget(client2);
    ui->scrollArea->setWidgetResizable(true);
    chartListLayout = new QVBoxLayout();
    client2->setLayout(chartListLayout);
    chartListLayout->setSpacing(0);
}



/************* COMPARISON ***************/



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

void MainWindow::sectionClicked(int row) {
    int count = 0;
    QString arg1 = ui->lineEdit->text();
    for(int i = comparisonModel->columnCount() - 1; i >= 0 ; i--){
        bool isInStrFilter = !arg1.isEmpty() &&
                comparisonModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().startsWith(arg1, Qt::CaseInsensitive);
        if(
//                comparisonModel->index(row, i).data(Qt::UserRole).toDouble() < 10/comparisonModel->precision
                comparisonModel->index(row, i).data(Qt::UserRole).toBool()
                && !isInStrFilter) {
            ui->comparisonTableView->hideColumn(i);
        } else {
            ui->comparisonTableView->showColumn(i);
            count++;
        }
    }
    ui->comparisonTableView->resizeColumnsToContents();
    ui->diffCountLabel->setText(QString::number(count) + " diffs.");
}

void MainWindow::on_pushButton_2_clicked()
{
    for(int i = comparisonModel->columnCount() - 1; i >= 0 ; i--){
        ui->comparisonTableView->showColumn(i);
    }
    ui->diffCountLabel->setText("");
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
            names.push_back(header.toLower().toStdString());
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
//    ui->comparisonTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    ui->comparisonTableView->
    connect(ui->comparisonTableView->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionClicked(int)));
}

void MainWindow::on_oldResButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open results");
    loadRefFile(fileName);
}


bool startWithFrom(QString str, QStringList l) {
    for(QString s:l)
        if(str.startsWith(s, Qt::CaseInsensitive))
            return true;
    return false;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QStringList filters = arg1.split(QRegExp(",|;"));
    for(int i = comparisonModel->columnCount() - 1; i >= 0 ; i--){
        if(!startWithFrom(comparisonModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString(),filters)) {
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


void MainWindow::on_filterParamButton_clicked(bool checked)
{
    QAbstractItemModel * model = ui->parametersTableView->model();
    for(int i = model->rowCount() - 1; i >= 0 ; i--){
        if(checked && !shortParamList.contains(model->index(i,0).data().toString(), Qt::CaseInsensitive)) {
            ui->parametersTableView->verticalHeader()->hideSection(i);
        } else {
            ui->parametersTableView->verticalHeader()->showSection(i);
        }
    }
}

void MainWindow::on_precisionSpinBox_valueChanged(int arg1)
{
    comparisonModel->precision = arg1;
    ui->comparisonTableView->reset();
}
