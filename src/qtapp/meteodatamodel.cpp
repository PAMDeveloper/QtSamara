#include "meteodatamodel.h"
#include <QDate>

MeteoDataModel::MeteoDataModel(SamaraParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{
//    for(auto it = params.getRawParameters().begin(); it != params.getRawParameters().end(); ++it) {
//        keys << QString::fromStdString(it->first);
//    }
//    qDebug() << keys;
}

int MeteoDataModel::rowCount(const QModelIndex &parent) const {
    return  parameters->climatics.size() - 1;
}
int MeteoDataModel::columnCount(const QModelIndex &parent) const {
    return 12;
}

QVariant MeteoDataModel::data(const QModelIndex &index, int role) const{
    int row = index.row()+1;
    if(role == Qt::DisplayRole){
        switch(index.column()){
            case 0: return QDate::fromJulianDay(parameters->climatics.at(row).JulianDay).toString("dd/MM/yyyy"); break;
            case 1: return parameters->climatics.at(row).TMax; break;
            case 2: return parameters->climatics.at(row).TMin; break;
            case 3: return parameters->climatics.at(row).TMoy; break;
            case 4: return parameters->climatics.at(row).HMax; break;
            case 5: return parameters->climatics.at(row).HMin; break;
            case 6: return parameters->climatics.at(row).HMoy; break;
            case 7: return parameters->climatics.at(row).Vt; break;
            case 8: return parameters->climatics.at(row).Ins; break;
            case 9: return parameters->climatics.at(row).Rg; break;
            case 10: return parameters->climatics.at(row).ETP; break;
            case 11: return parameters->climatics.at(row).Rain; break;
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
