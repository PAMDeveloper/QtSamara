#ifndef COMPARISON_DATA_MODEL2_H
#define COMPARISON_DATA_MODEL2_H

#include <QAbstractTableModel>
#include <QDateTime>
#include <QtCharts/QLineSeries>


using namespace std;

//struct serieCompare {
//    double minVal;
//    double maxVal;
//    double sumSrc;
//    double sumRef;
//    double diffPercent;
//    int diffStep;
//    bool valid;
//};

class ComparisonDataModel2 : public QAbstractTableModel
{
     Q_OBJECT
public:
    ComparisonDataModel2(const pair <vector <string>, vector < vector <double> > > & results,
                         const pair <vector <string>, vector < vector <double> > > & refs,
                         QObject *parent=0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    pair <vector <string>, vector < vector <double> > > results;
    pair <vector <string>, vector < vector <double> > > refs;
    QList<QPair<int,int>> visibleHeaders;
//    QList<serieCompare> comparisons;
//    bool clean;
//    QDate startDate;
};

#endif // COMPARISON_DATA_MODEL2_H
