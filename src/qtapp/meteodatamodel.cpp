#include "meteodatamodel.h"
#include <utils/juliancalculator.h>

MeteoDataModel::MeteoDataModel(SamaraParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{
//    for(auto it = params.getRawParameters().begin(); it != params.getRawParameters().end(); ++it) {
//        keys << QString::fromStdString(it->first);
//    }
//    qDebug() << keys;
}

int MeteoDataModel::rowCount(const QModelIndex &/*parent*/) const {
    return  (int)parameters->climatics.size();
}
int MeteoDataModel::columnCount(const QModelIndex &/*parent*/) const {
    return 12;
}

QVariant MeteoDataModel::data(const QModelIndex &index, int role) const{
    int row = index.row();
    if(role == Qt::DisplayRole){
        double val;
        switch(index.column()){
            case 0: return QString::fromStdString(
                        JulianCalculator::toStringDate(
                            parameters->getDouble("startingdate")+row-1,
                            JulianCalculator::YMD, '-'
                            )); break;
            case 1: val = parameters->climatics.at(row).TMax; break;
            case 2: val = parameters->climatics.at(row).TMin; break;
            case 3: val = parameters->climatics.at(row).TMoy; break;
            case 4: val = parameters->climatics.at(row).HMax; break;
            case 5: val = parameters->climatics.at(row).HMin; break;
            case 6: val = parameters->climatics.at(row).HMoy; break;
            case 7: val = parameters->climatics.at(row).Vt; break;
            case 8: val = parameters->climatics.at(row).Ins; break;
            case 9: val = parameters->climatics.at(row).Rg; break;
            case 10: val = parameters->climatics.at(row).ETP; break;
            case 11: val = parameters->climatics.at(row).Rain; break;
        }

        if(val == -999)
            return "No Value";
        return val;
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
