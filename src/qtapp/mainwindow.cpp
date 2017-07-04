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
#include <QDebug>

#include <qtapp/view.h>


#include <cmath>


static const int numCol = 2;

QT_CHARTS_USE_NAMESPACE

QStringList fromVector(vector<string> list) {
    QStringList r;
    for (int i = 0; i < list.size(); ++i) {
        r << QString::fromStdString(list[i]);
    }
    return r;
}

MainWindow::MainWindow(PSQLLoader * loader, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), loader(loader)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);
    createChartsTab();
    fillCombos();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::fillCombos() {
    ui->simComboBox->addItems(fromVector(loader->load_simulations_list()));
    ui->varComboBox->addItems(fromVector(loader->load_variety_list()));
    ui->plotComboBox->addItems(fromVector(loader->load_plot_list()));
    ui->stationComboBox->addItems(fromVector(loader->load_station_list()));
    ui->itinComboBox->addItems(fromVector(loader->load_itinerary_list()));
}

void MainWindow::show_trace() {
    //    ui->textEdit->clear();
    //    auto elements = ::Trace::trace().elements();
    //    if (!_date.isEmpty())
    //        elements = elements.filter_time(artis::utils::DateTime::toJulianDayNumber(_date.toStdString()));
    //    if (!_model_name.isEmpty())
    //        elements = elements.filter_model_name(_model_name.toStdString());
    //    if (!_var_name.isEmpty())
    //        elements = elements.filter_variable(_var_name.toStdString());
    //    if (!ui->lineEdit_4->text().isEmpty())
    //        elements = elements.filter_type(static_cast<artis::utils::TraceType>(_type));
    //    ui->textEdit->setText(QString::fromStdString(elements.to_string(artis::utils::DATE_FORMAT_YMD)));
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1) {
    _date = arg1;
    show_trace();
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1) {
    _model_name = arg1;
    show_trace();
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1) {
    _var_name = arg1;
    show_trace();
}

void MainWindow::on_lineEdit_4_textChanged(const QString &arg1) {
    _type = arg1.toInt();
    show_trace();
}

serieCompare MainWindow::compareSeries(QLineSeries * src, QLineSeries * ref) {
    serieCompare comparison;
    comparison.maxVal = -999999999999999;
    comparison.minVal = 999999999999999;
    comparison.sumSrc = 0;
    comparison.sumRef = 0;
    comparison.diffStep = -1;
    comparison.diffPercent = 1;
    comparison.valid = true;

    if(src == NULL || ref == NULL) {
        comparison.valid = false;
        return comparison;
    }

    int serieSize = src->count();

    for (int i = 0; i < serieSize; ++i) {
        double srcVal = src != NULL ? src->at(i).y() : 0;
        double refVal = ref != NULL ? ref->at(i).y() : 0;
        if (srcVal > comparison.maxVal) comparison.maxVal = srcVal;
        if (srcVal < comparison.minVal) comparison.minVal = srcVal;
        comparison.sumSrc += srcVal;
        if (refVal > comparison.maxVal) comparison.maxVal = refVal;
        if (refVal < comparison.minVal) comparison.minVal = refVal;
        comparison.sumRef += refVal;

        if( std::abs((srcVal-refVal)/refVal) > 0.0001 && comparison.diffStep == -1 ) comparison.diffStep = i;
    }
    comparison.diffPercent = ((comparison.sumRef-comparison.sumSrc)/comparison.sumRef) * 100;
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
                          QLineSeries *refSeries,
                          QString name) {

    serieCompare comparison = comparisons[headers.indexOf(name)];
    if(!comparison.valid)
        return false;

    QString titleName = name;
    QChart *chart = new QChart();
    QColor color = getColor(row * numCol + col);

    refSeries->setColor(color);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(color);
    refSeries->setPen(pen);
    titleName += " + ref";
    chart->addSeries(refSeries);

    resultsSeries->setColor(color.darker(250));
    pen.setWidth(1);
    pen.setColor(color.darker(250));
    resultsSeries->setPen(pen);
    chart->addSeries(resultsSeries);

    chart->legend()->hide();
    chart->setTitle(titleName);

    QDateTimeAxis *axisX = new QDateTimeAxis;
//    axisX->setTickCount(10);
    axisX->setFormat("dd MM");
    chart->addAxis(axisX, Qt::AlignBottom);
    resultsSeries->attachAxis(axisX);
    refSeries->attachAxis(axisX);

    if(std::abs(comparison.diffPercent) <= 0.00001 || std::abs(comparison.sumRef) <= 0.00001) {
        return false;
    }
    QDateTime date;
    date.setDate(startDate);
    date = date.addDays(comparison.diffStep);
    qDebug() << name << ":"  << comparison.diffPercent << "% at step: " << comparison.diffStep << ":" << date.toString("dd-MM");

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    chart->addAxis(axisY, Qt::AlignLeft);
    axisY->setMax(comparison.maxVal * 1.1 + 0.001);
    axisY->setMin(qMin<double>(-0.001, comparison.minVal - (comparison.maxVal - comparison.minVal) * 0.1));

    //    if(resultsSeries != NULL)
    resultsSeries->attachAxis(axisY);
    //    if (refSeries != NULL)
    refSeries->attachAxis(axisY);

    ChartView *chartView = new ChartView(chart, resultsSeries, refSeries, this);
    lay->addWidget(chartView, row, col);
    return true;
}


void MainWindow::createChartsTab() {
    QScrollArea *scrollArea = new QScrollArea;
    QWidget *client = new QWidget();
    scrollArea->setWidget(client);
    scrollArea->setWidgetResizable(true);

    lay = new QGridLayout();
    client->setLayout(lay);
    lay->setSpacing(0);
    ui->tabWidget->addTab(scrollArea, "Variables");
    ui->tabWidget->setCurrentIndex(2);
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
            serie->append(date.toMSecsSinceEpoch(),
                          results.second[i][d]);
            date = date.addDays(1);
        }
        resultsSeries.insert(QString::fromStdString(results.first[i]), serie);
    }
    return resultsSeries;
}

void MainWindow::displayData(
        SamaraParameters * parameters,
        pair <vector <string>, vector < vector <double> > > results,
        QString refFileName,
        bool showCharts) {


    ParametersDataModel *paramModel = new ParametersDataModel(parameters);
    ui->parametersTableView->setModel(paramModel);

    MeteoDataModel *meteoModel = new MeteoDataModel(parameters);
    ui->meteoTableView->setModel(meteoModel);

    ResultsDataModel *resultsModel = new ResultsDataModel(results);
    ui->resultsTableView->setModel(resultsModel);

    startDate = QDate::fromString(
                QString::fromStdString(parameters->getString("datedebut")),
                "yyyy-MM-dd");

    QMap < QString, QLineSeries * > refSeries = getRefSeries(refFileName);
    QMap < QString, QLineSeries * > resultsSeries = getResultSeries(results);

    headers << refSeries.keys() << resultsSeries.keys();
    headers = headers.toSet().toList();
    headers.sort(Qt::CaseInsensitive);

    int j = 0;
    foreach (QString key, headers) {
        comparisons.push_back(compareSeries(resultsSeries[key], refSeries[key]));
        if(showCharts)
            if( addChart(j / numCol, j % numCol, resultsSeries[key], refSeries[key], key) )
                j++;
    }

    ui->comparisonTableView->verticalHeader()->setSectionsClickable(true);
    connect(ui->comparisonTableView->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionClicked(int)));

    comparisonModel = new ComparisonDataModel(resultsSeries, refSeries, comparisons, headers, startDate, true);
    ui->comparisonTableView->setModel(comparisonModel);

}

void MainWindow::sectionClicked(int row) {
    bool all_good = true;
    for(int i = comparisonModel->columnCount() - 1; i >= 0 ; i--){
        if(!comparisonModel->index(row, i).data(Qt::UserRole).toBool()) {
            ui->comparisonTableView->hideColumn(i);
        } else {
            ui->comparisonTableView->showColumn(i);
            all_good = false;
        }
    }

    if(all_good) {
        for(int i = comparisonModel->columnCount() - 1; i >= 0 ; i--){
            ui->comparisonTableView->showColumn(i);
        }
    }
}
