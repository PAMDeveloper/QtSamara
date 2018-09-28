#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QGridLayout>
#include <QMouseEvent>
#include <QDate>
#include <qtapp/comparisondatamodel.h>
#include <qtapp/comparisondatamodel.h>

#include <parameters.h>
#include <qtapp/meteodatamodel.h>
#include <qtapp/parametersdatamodel.h>
#include <qtapp/estimparamdatamodel.h>
#include <qtapp/resultsdatamodel.h>
#include <qtapp/DBAccessLoader.h>
#include <qtapp/chartmanager.h>
#include <qtapp/resultsmanager.h>
#include <qtapp/comparisonmanager.h>
#include <qtapp/obsdatamodel.h>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayData( pair <vector <string>, vector < vector <double> > > results );

    bool addChart(int row, int col,
                  QLineSeries *series,
                  QScatterSeries *refseries,
                  QString name);

    serieCompare compareSeries(QLineSeries * src, QScatterSeries * ref);

    void loadDB(QString filePath);
    void clearDBContext();
    void createChartsTab();
    void fillDBCombos();
    void showParameters(SamaraParameters * parameters);
    void showDates();
    void loadRefFile(QString path);

private slots:
    void on_actionLoad_database_triggered();
    void on_launchButton_clicked();
    void on_saveResultButton_clicked();

    void sectionClicked(int);
    void on_simComboBox_currentTextChanged(const QString &arg1);
    void on_varComboBox_currentTextChanged(const QString &arg1);
    void on_stationComboBox_currentTextChanged(const QString &arg1);
    void on_plotComboBox_currentTextChanged(const QString &arg1);
    void on_itinComboBox_currentTextChanged(const QString &arg1);
    void on_startDateEdit_dateChanged(const QDate &date);
    void on_endDateEdit_dateChanged(const QDate &date);

    void on_oldResButton_clicked();
    void on_pushButton_2_clicked();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_pushButton_3_clicked();

    void on_filterParamButton_clicked(bool checked);

    void on_precisionSpinBox_valueChanged(int arg1);

    void on_actionSave_Parameters_triggered();

    void on_actionLoad_Parameters_triggered();

    void on_actionSave_Meteo_triggered();

    void on_estimButton_clicked();

    void on_loadObsFromDB_clicked();

    void on_actionLoad_Meteo_triggered();

    void on_loadEstimContext_clicked();

    void on_actionSave_Observations_triggered();

    void on_actionLoad_batch_triggered();

    void on_actionLoad_Irrigation_triggered();

private:
    Ui::MainWindow *ui;
    QSettings * settings;

    QStringList chartVisibles;
    QString _date;
    QString _model_name;
    QString _var_name;
    int _type;

    DBAccessLoader * loader;
    ChartManager * chartManager;
    ResultsManager * resultsManager;
    ComparisonManager * comparisonManager;
    ParametersDataModel *paramModel;
    EstimParamDataModel *estimModel;
    MeteoDataModel *meteoModel;

    ResultsDataModel *resultsModel;
    ComparisonDataModel2 *comparisonModel = nullptr;
    ObsDataModel *obsmodel;

    pair <vector <string>, vector < vector <double> > > results;
    map<string, vector<double>> observations;

    QGridLayout *chartLayout;
    QVBoxLayout * chartListLayout;

    QList<serieCompare> comparisons;
    QStringList headers;
    QDate startDate;
};

#endif // MAINWINDOW_H
