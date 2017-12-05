#ifndef VISUMANAGER_H
#define VISUMANAGER_H

#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>

#include <QDateTime>

class VisuManager {
    pair <vector <string>, vector < vector <double> > >  data;
    QList <int> visiblesCharts;

    VisuManager(pair <vector <string>, vector < vector <double> > > data): data(data) {}

    QColor getColor(int i) {
        double PHI = (1 + qSqrt(5)) / 2;
        double n = i * PHI - floor(i * PHI);
        int h = qFloor(n * 256);
        return QColor::fromHsv(h, 245, 245, 255);
    }

    QLineSe

    QChart * getChart(QString name) {

    }
};


#endif // VISUMANAGER_H
