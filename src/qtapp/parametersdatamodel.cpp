#include "parametersdatamodel.h"
#include <QHash>
#include <QtCore/qmath.h>


class KeyLessThan
{
public:
    KeyLessThan( SamaraParameters * params):params(params){}
    bool operator()(const QString &v1, const QString &v2) const{
        QString v1cat = QString::fromStdString(params->doubles[v1.toStdString()].second);
        QString v2cat = QString::fromStdString(params->doubles[v2.toStdString()].second);

        if(v1cat == v2cat)
            return v1 < v2;
        return v1cat < v2cat;
    }

private:
SamaraParameters * params;
};

ParametersDataModel::ParametersDataModel(SamaraParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{
    for(auto const& token: params->doubles) {
        keys << QString::fromStdString(token.first);
    }
    qSort(keys.begin(), keys.end(), KeyLessThan(params));
}


int ParametersDataModel::rowCount(const QModelIndex &parent) const {
    return keys.size();
}
int ParametersDataModel::columnCount(const QModelIndex &parent) const {
    return 2;
}

QColor ParametersDataModel::getColor(QString s) {
    int i = qHash(s) % 350;
    double PHI = (1 + qSqrt(5)) / 2;
    double n = i * PHI - floor(i * PHI);
    int h = qFloor(n * 256);
    return QColor::fromHsv(h, 245, 245, 255);
}

QVariant ParametersDataModel::data(const QModelIndex &index, int role) const{
    if(index.column() == 0 && role == Qt::DisplayRole)
        return keys[index.row()];


    if(index.column() == 1 && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        try
        {
            double r = parameters->getDouble(keys[index.row()].toStdString());
            if(r == -999 && role == Qt::DisplayRole)
                return "Null value";
            return QString::number(r, 'f', 6);

        }
        catch(...) {
            return QString::fromStdString(parameters->getString(keys[index.row()].toStdString()));
        }
    }

    if(role == Qt::BackgroundColorRole)
        return getColor(QString::fromStdString(parameters->doubles[keys[index.row()].toStdString()].second)).lighter();

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

Qt::ItemFlags ParametersDataModel::flags(const QModelIndex &index) const {
    if(index.column() == 1)
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);

    return QAbstractTableModel::flags(index);
}

bool ParametersDataModel::setData(const QModelIndex &idx, const QVariant &value, int role) {
    if(role != Qt::EditRole)
        return false;
    string key = data(index(idx.row(),0), Qt::DisplayRole).toString().toStdString();
    bool ok;
    double d = value.toDouble(&ok);
    if(ok)
        parameters->doubles[key].first = d;
    return ok;
}
