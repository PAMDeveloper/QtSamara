#include "parametersdatamodel.h"
#include <QDebug>
ParametersDataModel::ParametersDataModel(SamaraParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{
    for(auto const& token: params->doubles) {
        keys << QString::fromStdString(token.first);
    }
}

int ParametersDataModel::rowCount(const QModelIndex &parent) const {
    return keys.size();
}
int ParametersDataModel::columnCount(const QModelIndex &parent) const {
    return 2;
}

QVariant ParametersDataModel::data(const QModelIndex &index, int role) const{
    if(index.column() == 0 && role == Qt::DisplayRole)
        return keys[index.row()];


    if(index.column() == 1 && role == Qt::DisplayRole) {
        try
        {
          return parameters->getDouble(keys[index.row()].toStdString());

        }
        catch(...) {
            return QString::fromStdString(parameters->getString(keys[index.row()].toStdString()));
        }

    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}
QVariant ParametersDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
        case 0:return "Name"; break;
        case 1: return "Value"; break;
        }


    }

    return QAbstractTableModel::headerData(section, orientation, role);
}
