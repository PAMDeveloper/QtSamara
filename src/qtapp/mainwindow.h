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
#include <qtapp/resultsdatamodel.h>
#include <qtapp/DBAccessLoader.h>
#include <qtapp/chartmanager.h>
#include <qtapp/resultsmanager.h>
#include <qtapp/comparisonmanager.h>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}


#include "utils/de.h"


#include <ctime>

//struct PARAMS {
//   static const double min[11];
//   static const double max[11];
//   static const string names[11];
//   static const unsigned int size;
//};

//const unsigned int PARAMS::size = 11;
//const double PARAMS::min[11] = {1, 1, 1, 20, 10, 0.01, -0.5, 0.0, 0.0, 0.01, 0.1};
//const double PARAMS::max[11] = {10, 10, 20, 60, 120, 0.5, 0.5, 2, 0.5, 0.1, 0.5};
//const *double PARAMS::names[11] = {"Epsib", "Ict", "MGR_init", "plasto_init", "SLAp", "leaf_length_to_IN_length", "coef_MGR_PI", "slope_length_IN", "slope_LL_BL_at_PI", "density_IN1", "density_IN2"};

class SamaraFitness : public de::IOptimizable
{

public:
    SamaraParameters * parameters;
    SamaraFitness(SamaraParameters * parameters):
        parameters(parameters) {
    }


    double EvaluteCost(std::vector<double> inputs) const override
    {
        Samara samara;
        samara.run_samara_2_3(parameters, (Samara::SamaraLogType)1);
        return 1;
    }

    unsigned int NumberOfParameters() const override
    {
        return 15;
    }

    std::vector<Constraints> GetConstraints() const override
    {
        std::vector<Constraints> constr(NumberOfParameters());

        for (int i = 0; i < NumberOfParameters(); ++i) {
            constr[i] = Constraints(0, 100, true);
        }

        return constr;
    }
};

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
    MeteoDataModel *meteoModel;

    ResultsDataModel *resultsModel;
    ComparisonDataModel2 *comparisonModel = nullptr;


    pair <vector <string>, vector < vector <double> > > results;
    map<string, vector<double>> observations;

    QGridLayout *chartLayout;
    QVBoxLayout * chartListLayout;

    QList<serieCompare> comparisons;
    QStringList headers;
    QDate startDate;
};

#endif // MAINWINDOW_H
