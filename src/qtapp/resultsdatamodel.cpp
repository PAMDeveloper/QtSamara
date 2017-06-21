#include "resultsdatamodel.h"

using namespace std;
ResultsDataModel::ResultsDataModel(pair <vector <string>, vector < vector <double> > > results, QObject *parent)
 : QAbstractTableModel(parent), results(results)
{

}

int ResultsDataModel::rowCount(const QModelIndex &parent) const {
    return results.second[0].size();
}
int ResultsDataModel::columnCount(const QModelIndex &parent) const {
    return results.first.size();
}

QVariant ResultsDataModel::data(const QModelIndex &index, int role) const{

    if(role == Qt::DisplayRole) {
        return results.second[index.column()][index.row()];
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant ResultsDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return QString::fromStdString(results.first[section]);
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}
