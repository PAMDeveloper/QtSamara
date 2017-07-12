#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QGridLayout>
#include <QMouseEvent>
#include <QDate>
#include <qtapp/comparisondatamodel.h>

#include <parameters.h>
#include <qtapp/meteodatamodel.h>
#include <qtapp/parametersdatamodel.h>
#include <qtapp/resultsdatamodel.h>
#include <utils/psqlloader.h>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(PSQLLoader * loader, QWidget *parent = 0);
    ~MainWindow();

    void displayData( pair <vector <string>, vector < vector <double> > > results );

    bool addChart(int row, int col,
                  QLineSeries *series,
                  QLineSeries *refseries,
                  QString name);

    serieCompare compareSeries(QLineSeries * src, QLineSeries * ref);

    QMap < QString, QLineSeries * > getRefSeries(QString refFileName);
    QMap < QString, QLineSeries * > getResultSeries(pair <vector <string>, vector < vector <double> > > results);


//    QLineSeries *getSeries(QString fileName, QDate endDate);

    void createChartsTab();
    void show_trace();
    void fillCombos();
    void showParameters(SamaraParameters * parameters);

private slots:
    void sectionClicked(int);
    void on_simComboBox_currentTextChanged(const QString &arg1);
    void on_varComboBox_currentTextChanged(const QString &arg1);
    void on_stationComboBox_currentTextChanged(const QString &arg1);
    void on_plotComboBox_currentTextChanged(const QString &arg1);
    void on_itinComboBox_currentTextChanged(const QString &arg1);

    void on_startDateEdit_dateChanged(const QDate &date);

    void on_endDateEdit_dateChanged(const QDate &date);

    void on_launchButton_clicked();

    void chartClicked(bool);

private:
    Ui::MainWindow *ui;
    QStringList chartVisibles;
    QString _date;
    QString _model_name;
    QString _var_name;
    int _type;

private:
    PSQLLoader * loader;
    ComparisonDataModel *comparisonModel;
    SamaraParameters * parameters;
    QList<serieCompare> comparisons;
    QStringList headers;
    std::string simulation;
    QDate startDate;
    QGridLayout *lay;
};

#endif // MAINWINDOW_H
