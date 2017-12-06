#ifndef RESULTSDATAMODEL_H
#define RESULTSDATAMODEL_H
#include <QAbstractTableModel>
using namespace std;
class ResultsDataModel : public QAbstractTableModel
{
public:
    ResultsDataModel(QObject *parent=0);

    void setResults(const pair <vector <string>, vector < vector <double> > > & results);
    void save(QString path, QString sep);

    int rowCount(const QModelIndex &parent= QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const;


private:
   pair <vector <string>, vector < vector <double> > > results;
};

#endif // RESULTSDATAMODEL_H
