#ifndef COMPARISON_DATA_MODEL_H
#define COMPARISON_DATA_MODEL_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QtCharts/QLineSeries>

using namespace std;

struct serieCompare {
    double minVal;
    double maxVal;
    double sumSrc;
    double sumRef;
    double diffPercent;
    int diffStep;
    bool valid;
};

class ComparisonDataModel : public QAbstractTableModel
{
     Q_OBJECT
public:
    ComparisonDataModel(QMap < QString, QtCharts::QLineSeries * > resultsSeries, QMap < QString, QtCharts::QLineSeries * > refSeries,
                        QList<serieCompare> comparisons, QStringList headers, QDate startDate,
                        bool clean = false, QObject *parent=0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QMap < QString, QtCharts::QLineSeries * > refSeries;
    QMap < QString, QtCharts::QLineSeries * > resultsSeries;
    QStringList headers;
    QList<int> cleanSeries;
    QList<serieCompare> comparisons;
    bool clean;
    QDate startDate;
};

#endif // COMPARISON_DATA_MODEL_H
