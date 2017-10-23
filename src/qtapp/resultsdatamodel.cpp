#include "resultsdatamodel.h"
#include <numeric>
using namespace std;
ResultsDataModel::ResultsDataModel(pair <vector <string>, vector < vector <double> > > results, QObject *parent)
    : QAbstractTableModel(parent), results(results)
{
    for (int i = 0; i < results.first.size(); ++i) {
        double sumRes = accumulate(results.second[i].begin(), results.second[i].end(), 0);
        if(sumRes != 0){
            visibleHeaders.push_back(i);
        }
    }
}

int ResultsDataModel::rowCount(const QModelIndex &parent) const {
    return results.second[0].size();
}

int ResultsDataModel::columnCount(const QModelIndex &parent) const {
    //    return results.first.size();
    return visibleHeaders.count();
}

QVariant ResultsDataModel::data(const QModelIndex &index, int role) const{

    if(role == Qt::DisplayRole) {
        return results.second[visibleHeaders[index.column()]][index.row()];
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant ResultsDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return QString::fromStdString(results.first[visibleHeaders[section]]);
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}
