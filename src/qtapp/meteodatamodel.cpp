#include "meteodatamodel.h"
#include <QDebug>

MeteoDataModel::MeteoDataModel(samara::ModelParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{
//    for(auto it = params.getRawParameters().begin(); it != params.getRawParameters().end(); ++it) {
//        keys << QString::fromStdString(it->first);
//    }
//    qDebug() << keys;
}

int MeteoDataModel::rowCount(const QModelIndex &parent) const {
    return  parameters->getMeteoValues()->size();
}
int MeteoDataModel::columnCount(const QModelIndex &parent) const {
    return 12;
}

QVariant MeteoDataModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole){
        switch(index.column()){
            case 0: return index.row(); break;
            case 1: return parameters->getMeteoValues()->at(index.row()).TMax; break;
            case 2: return parameters->getMeteoValues()->at(index.row()).TMin; break;
            case 3: return parameters->getMeteoValues()->at(index.row()).TMoy; break;
            case 4: return parameters->getMeteoValues()->at(index.row()).HMax; break;
            case 5: return parameters->getMeteoValues()->at(index.row()).HMin; break;
            case 6: return parameters->getMeteoValues()->at(index.row()).HMoy; break;
            case 7: return parameters->getMeteoValues()->at(index.row()).Vt; break;
            case 8: return parameters->getMeteoValues()->at(index.row()).Ins; break;
            case 9: return parameters->getMeteoValues()->at(index.row()).Rg; break;
            case 10: return parameters->getMeteoValues()->at(index.row()).ETP; break;
            case 11: return parameters->getMeteoValues()->at(index.row()).Rain; break;
        }
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant MeteoDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
            case 0: return "Date"; break;
            case 1: return "TMax"; break;
            case 2: return "TMin"; break;
            case 3: return "TMoy"; break;
            case 4: return "HMax"; break;
            case 5: return "HMin"; break;
            case 6: return "HMoy"; break;
            case 7: return "Vt"; break;
            case 8: return "Ins"; break;
            case 9: return "Rg"; break;
            case 10: return "ETP"; break;
            case 11: return "Rain"; break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}
