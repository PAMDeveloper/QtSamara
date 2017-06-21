#ifndef RESULTSDATAMODEL_H
#define RESULTSDATAMODEL_H
#include <QAbstractTableModel>
using namespace std;
class ResultsDataModel : public QAbstractTableModel
{
public:
    ResultsDataModel(pair <vector <string>, vector < vector <double> > > results, QObject *parent=0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
   pair <vector <string>, vector < vector <double> > > results;
};

#endif // RESULTSDATAMODEL_H
