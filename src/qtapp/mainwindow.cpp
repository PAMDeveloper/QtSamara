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
#include <qtapp/meteodatamodel.h>
#include <qtapp/parametersdatamodel.h>

#include <defines.hpp>

using namespace artis::kernel;

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->splitter->setStretchFactor(0, 0);
  ui->splitter->setStretchFactor(1, 1);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::show_trace() {
  ui->textEdit->clear();
  auto elements = ::Trace::trace().elements();
  if (!_date.isEmpty())
    elements = elements.filter_time(artis::utils::DateTime::toJulianDayNumber(_date.toStdString()));
  if (!_model_name.isEmpty())
    elements = elements.filter_model_name(_model_name.toStdString());
  if (!_var_name.isEmpty())
    elements = elements.filter_variable(_var_name.toStdString());
  if (!ui->lineEdit_4->text().isEmpty())
    elements = elements.filter_type(static_cast<artis::utils::TraceType>(_type));
  ui->textEdit->setText(QString::fromStdString(elements.to_string(artis::utils::DATE_FORMAT_YMD)));
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


void MainWindow::addChart(int row, int col,
                          QLineSeries *series, QLineSeries *refSeries,
                          QGridLayout *lay, QString name) {
  QChart *chart = new QChart();
  chart->addSeries(series);
  if (refSeries != NULL) {
    refSeries->setColor(series->color().darker(200));
    name += " + ref";
    chart->addSeries(refSeries);
  }
  chart->legend()->hide();
  chart->setTitle(name);

  QDateTimeAxis *axisX = new QDateTimeAxis;
  axisX->setTickCount(10);
  axisX->setFormat("dd-MM");
  chart->addAxis(axisX, Qt::AlignBottom);
  if (refSeries != NULL)
    refSeries->attachAxis(axisX);
  series->attachAxis(axisX);


  bool refBigger = false;
  double maxVal = 0;
  double minVal = 999999999999999;
  for (int i = 0; i < series->count(); ++i) {
    if (series->at(i).y() > maxVal) {
      maxVal = series->at(i).y();
      refBigger = false;
    }
    if (series->at(i).y() < minVal) {
      minVal = series->at(i).y();
    }
    if (refSeries != NULL) {
      if (refSeries->at(i).y() > maxVal) {
        maxVal = refSeries->at(i).y();
        refBigger = true;
      }
      if (refSeries->at(i).y() < minVal) {
        minVal = refSeries->at(i).y();
      }
    }
  }

  QValueAxis *axisY = new QValueAxis;
  axisY->setLabelFormat("%i");
  chart->addAxis(axisY, Qt::AlignLeft);
  axisY->setMax(maxVal * 1.1 + 0.001);
  axisY->setMin(qMin<double>(-0.001, minVal - (maxVal - minVal) * 0.1));
  if (refBigger) {
    if (refSeries != NULL)
      refSeries->attachAxis(axisY);
    series->attachAxis(axisY);
  } else {
    series->attachAxis(axisY);
    if (refSeries != NULL)
      refSeries->attachAxis(axisY);
  }


  ChartView *chartView = new ChartView(chart, series, refSeries, this);
  lay->addWidget(chartView, row, col);
}


QColor getColor(int i) {
  double PHI = (1 + qSqrt(5)) / 2;
  double n = i * PHI - floor(i * PHI);
  int h = qFloor(n * 256);
  return QColor::fromHsv(h, 245, 245, 255);
}


QLineSeries *MainWindow::getSeries(QString fileName, QDate endDate) {
  QLineSeries *series = new QLineSeries();
  QList<double> values;
  QFile inputFile(fileName);
  QDate lastDate;
  if (inputFile.open(QIODevice::ReadOnly)) {
    QTextStream in(&inputFile);
    in.readLine();
    in.readLine();
    in.readLine();
    while (!in.atEnd()) {
      QString line = in.readLine();
      if (line.isEmpty())
        continue;
      QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
      QDate date = QDate::fromString(list[1].split(",")[0], "dd/MM/yyyy");
      lastDate = date;
      if (date < startDate || date > endDate)
        continue;
      bool ok = false;
      double value = list[2].toDouble(&ok);
      QDateTime t = QDateTime(date);
      //          qDebug() << t.toMSecsSinceEpoch() << value;
      series->append(t.toMSecsSinceEpoch(), value);
    }
    inputFile.close();
  }
  if (lastDate < endDate) {
    for (int i = 1; i < lastDate.daysTo(endDate); ++i) {
      series->append(QDateTime(lastDate.addDays(i)).toMSecsSinceEpoch(), 0);
    }
  }
  return series;
}

void MainWindow::displayData(observer::GlobalView *view,
                             QString refFileName,
                             samara::ModelParameters *parameters,
                             QString begin, QString end) {

  ParametersDataModel *paramModel = new ParametersDataModel(parameters);
  ui->parametersTableView->setModel(paramModel);

  MeteoDataModel *meteoModel = new MeteoDataModel(parameters);
  ui->meteoTableView->setModel(meteoModel);

  QScrollArea *scrollArea = new QScrollArea;
  QWidget *client = new QWidget();
  scrollArea->setWidget(client);
  scrollArea->setWidgetResizable(true);

  QGridLayout *lay = new QGridLayout();
  client->setLayout(lay);
  lay->setSpacing(0);
  ui->tabWidget->addTab(scrollArea, "Variables");

  startDate = QDate::fromString(begin, "yyyy-MM-dd");
  QDate endDate = QDate::fromString(end, "yyyy-MM-dd");
  currentDate = startDate;
  const int numCol = 2;


  QMap < QString, QLineSeries * > refSeries;

  QList< QPair < QString, QList <double>>> results;
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
    while (!in.atEnd()) {
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


  std::map<std::string, std::vector < std::pair < double, std::string >>> m = view->values();
  int j = 0;
  for (auto it = m.begin(); it != m.end(); ++it) {
    QString param = QString::fromStdString(it->first);
    QLineSeries *series = new QLineSeries();
    series->setColor(getColor(j));

    for (int i = 0; i < startDate.daysTo(endDate); ++i) {
      double value = view->get(startDate.addDays(i).toJulianDay(),
                               param.toLocal8Bit().constData());
      QDateTime momentInTime;
      momentInTime.setDate(startDate.addDays(i));
      series->append(momentInTime.toMSecsSinceEpoch(), value);
    }

    addChart(j / numCol, j % numCol, series, refSeries[param], lay, param);
    j++;
  }
}



