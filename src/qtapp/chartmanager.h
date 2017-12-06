#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMap>

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>

#include <samara.h>

#include <qtapp/chartview.h>

using namespace std;
using namespace QtCharts;

class ChartManager : public QObject
{
    Q_OBJECT
public:
    explicit ChartManager(QGridLayout *chartLayout, QVBoxLayout *chartListLayout, QObject *parent = nullptr);

    void fillList();
    void setResults(pair <vector <string>, vector < vector <double> > > results, map<string, vector<double>> observations);
    void generateObsSeries(map<string, vector<double>> observations);
    void generateResultSeries(pair <vector <string>, vector < vector <double> > > results);
    void clearSeries();
    ChartView * createChart(QString name);

    void displayCharts();

    QMap <QString, QAbstractSeries*> resultSeries;
    QMap <QString, QAbstractSeries*> obsSeries;
    QMap <QString, ChartView *> charts;

    QGridLayout *chartLayout;
    QVBoxLayout *chartListLayout;
    QStringList chartList;
    QStringList defaultList;
    QStringList checkedList;

signals:

public slots:
};

#endif // CHARTMANAGER_H
