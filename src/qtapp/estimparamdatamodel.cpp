#include "estimparamdatamodel.h"
#include <QHash>
#include <QtCore/qmath.h>
#include "utils/juliancalculator.h"

#include <QFile>
#include <QTextStream>

const QString paramList = "startingdate,endingdate,coefflodging,stemporosity,asscstr,attenmitch,bundheight,ca,co2cp,co2exp,co2slopetr,coeffassimsla,coefficientq10,coeffinternodemass,coeffinternodenum,coeffleafdeath,coeffleafwlratio,coeffpaniclemass,coeffpansinkpop,coeffrescapacityinternode,coeffreservesink,coeffrootmasspervolmax,coefftillerdeath,coefftransplantingshock,densityfield,densitynursery,devcstr,durationnursery,epaisseurprof,epaisseursurf,excessassimtoroot,ftswirrig,hauncrittillering,humcr,humfc,humpf,humsat,ictillering,internodelengthmax,irrigauto,irrigautoresume,irrigautostop,irrigautotarget,kcmax,kcritstercold1,kcritstercold2,kcritsterftsw1,kcritsterftsw2,kcritsterheat1,kcritsterheat2,kcritstresscold1,kcritstresscold2,kdf,krespinternode,krespmaintleaf,krespmaintroot,krespmaintsheath,kresppanicle,ktempmaint,leaflengthmax,lifesavingdrainage,mulch,panstructmassmax,parcritsla,percolationmax,pevap,pfactor,phyllo,plantsperhill,plotdrainagedaf,poidssecgrain,pourcruiss,ppcrit,ppexp,ppsens,prioritypan,profracini,ranklongestleaf,relmobiliinternodemax,relphyllophasestemelong,rollingbase,rollingsens,rootcstr,rootfrontmax,rootpartitmax,ru,sdjbvp,sdjlevee,sdjmatu1,sdjmatu2,sdjrpr,seuilcstrmortality,seuilpp,seuilruiss,slamax,slamin,sowing,stockiniprof,stockinisurf,tbase,tempsla,tilability,tlim,topt1,topt2,transplanting,transplantingdepth,txassimbvp,txassimmatu1,txassimmatu2,txconversion,txresgrain,txrusurfgermi,vracbvp,vraclevee,vracmatu1,vracmatu2,vracpsp,vracrpr,waterloggingsens,wsalt,wslat,wslong,wtratioleafsheath";

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

EstimParamDataModel::EstimParamDataModel(SamaraParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{
    settings = new QSettings("PAM Cirad", "Samara");
    QStringList paramStrList = paramList.split(",");
    for(auto const& token: params->doubles) {
        if(paramStrList.contains(QString::fromStdString(token.first), Qt::CaseInsensitive))
            keys << QString::fromStdString(token.first);
    }
    qSort(keys.begin(), keys.end(), KeyLessThan(params));
}


bool EstimParamDataModel::isValid(int row) const{
    return index(row,1).data().toDouble() < index(row,2).data().toDouble();
}

int EstimParamDataModel::rowCount(const QModelIndex &/*parent*/) const {
    return keys.size();
}
int EstimParamDataModel::columnCount(const QModelIndex &/*parent*/) const {
    return 3;
}

QColor EstimParamDataModel::getColor(QString s) {
    int i = qHash(s);
    double PHI = (1 + qSqrt(5)) / 2;
    double n = i * PHI - floor(i * PHI);
    int h = qFloor(n * 256);
    return QColor::fromHsv(h, 245, 245, 255);
}

QVariant EstimParamDataModel::data(const QModelIndex &index, int role) const{
    if(index.column() == 0 && role == Qt::DisplayRole)
        return keys[index.row()];

    if(index.column() == 0 && role == Qt::CheckStateRole)
        return checked.contains(keys[index.row()]);

    if(index.column() == 1 && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        bool ok;
        double val = settings->value(keys[index.row()] + "_Min", -999).toDouble(&ok);
        if (val == -999 || !ok) {
            return parameters->getDouble(keys[index.row()].toStdString()) * 0.5;
        }
        else return val;
    }

    if(index.column() == 2 && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        bool ok;
        double val = settings->value(keys[index.row()] + "_Max", -999).toDouble(&ok);
        if (val == -999 || !ok) {
            return parameters->getDouble(keys[index.row()].toStdString()) * 2;
        }
        else return val;
    }

    if(role == Qt::BackgroundColorRole) {
        if(!isValid(index.row()))
            return QColor::fromRgb(255,100,100);
        return getColor(QString::fromStdString(parameters->doubles[keys[index.row()].toStdString()].second)).lighter();
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}
QVariant EstimParamDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
        case 0:return "Estimation"; break;
        case 1: return "Min"; break;
        case 2: return "Max"; break;
        }


    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags EstimParamDataModel::flags(const QModelIndex &index) const {
    if(index.column() == 0)
        return Qt::ItemIsUserCheckable |  QAbstractTableModel::flags(index);

    else
        return Qt::ItemIsEditable |  QAbstractTableModel::flags(index);

    return QAbstractTableModel::flags(index);
}

bool EstimParamDataModel::setData(const QModelIndex &idx, const QVariant &value, int role) {
    if(idx.column() == 0 && role == Qt::CheckStateRole) {
        if(!checked.contains(keys[idx.row()])) {
            if(index(idx.row(),1).data().toString() != "" && index(idx.row(),2).data().toString() != "" && isValid(idx.row()))
                checked.append(keys[idx.row()]);
        } else {
            checked.removeAll(keys[idx.row()]);
        }
        emit dataChanged(idx,idx);
    }

    if(role != Qt::EditRole)
        return false;

    if(idx.column() > 0) {
        bool ok;
        double d = value.toDouble(&ok);
        if(ok) {
            settings->setValue(keys[idx.row()] + (idx.column() == 1 ? "_Min" : "_Max"), d);
        }
        return ok;
    }
    return true;
}


bool EstimParamDataModel::save(QString path, QString sep) {
//    QFile file(path);
//    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//        QTextStream out(&file);
//        out << parameters->strings.size() << "\n";
//        for (auto const& token : parameters->strings) {
//            out << QString::fromStdString(token.first) << sep << QString::fromStdString(token.second.first) << "\n";
//        }

//        out << parameters->doubles.size() << "\n";
//        for (auto const& token : parameters->doubles) {
//            out << QString::fromStdString(token.first) << sep << QString::number(token.second.first) << "\n";
//        }

////        for (int row = 0; row < rowCount(); ++row) {
////            for (int col = 0; col < columnCount(); ++col) {
////                    out << index(row,col).data().toString() << sep;
////            }
////            out << "\n";
////        }

//        file.close();
//        return true;
//    }
    return false;
}


bool EstimParamDataModel::load(QString path, QString sep) {
//    QFile file(path);
//    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream in(&file);
//        int rowCt = in.readLine().toInt();
//        for (int row = 0; row < rowCt; ++row) {
//            QString line = in.readLine();
//            QStringList lstLine = line.split(sep);
//            parameters->strings[ lstLine[0].toStdString() ].first = lstLine[1].toStdString();
//        }

//        rowCt = in.readLine().toInt();
//        for (int row = 0; row < rowCt; ++row) {
//            QString line = in.readLine();
//            QStringList lstLine = line.split(sep);
//            parameters->doubles[ lstLine[0].toStdString() ].first = lstLine[1].toDouble();
//        }

////        int row = 0;
////        while (!in.atEnd()) {
////            QString line = in.readLine();
////            QStringList lstLine = line.split(sep);
////            setData(index(row,0), lstLine[0], Qt::EditRole);
////            setData(index(row,1), lstLine[1], Qt::EditRole);
////            row++;
////        }
//        file.close();
//        dataChanged(index(0,0),index(rowCount()-1,1));
//        return true;
//    }
    return false;
}

vector<pair<double, double> > EstimParamDataModel::bounds() {
    vector<pair<double, double> > b;
    for(int row = 0; row < rowCount(); row++) {
        if(checked.contains(keys[row])) {
            b.push_back(pair<double,double>(index(row,1).data().toDouble(), index(row,2).data().toDouble()));
        }
    }
    return b;
}

vector<string> EstimParamDataModel::params() {
    vector<string> params;
    for (int i = 0; i < checked.size(); ++i) {
        params.push_back(checked[i].toStdString());
    }
    return params;
}
