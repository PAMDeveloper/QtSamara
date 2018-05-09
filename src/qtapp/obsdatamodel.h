#ifndef OBSDATAMODEL_H
#define OBSDATAMODEL_H


#include <QAbstractTableModel>
using namespace std;
class ObsDataModel : public QAbstractTableModel
{
public:
    ObsDataModel(QObject *parent=0);

    void setObs(const map<string, vector<double>> & obs);
    void load(QString path, QString sep);
    bool save(QString path, QString sep);

    int rowCount(const QModelIndex &parent= QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const;

private:
   map<string, vector<double>> obs;
   vector<string> keys;
};
#endif // OBSDATAMODEL_H
