#include "parametersdatamodel.h"
#include <QHash>
#include <QtCore/qmath.h>
#include "utils/juliancalculator.h"

const QString paramList = "coefflodging,asscstr,attenmitch,bundheight,ca,co2cp,co2exp,co2slopetr,coeffassimsla,coefficientq10,coeffinternodemass,coeffinternodenum,coeffleafdeath,coeffleafwlratio,coeffpaniclemass,coeffpansinkpop,coeffrescapacityinternode,coeffreservesink,coeffrootmasspervolmax,coefftillerdeath,coefftransplantingshock,densityfield,densitynursery,devcstr,durationnursery,epaisseurprof,epaisseursurf,excessassimtoroot,ftswirrig,hauncrittillering,humcr,humfc,humpf,humsat,ictillering,internodelengthmax,irrigauto,irrigautoresume,irrigautostop,irrigautotarget,kcmax,kcritstercold1,kcritstercold2,kcritsterftsw1,kcritsterftsw2,kcritsterheat1,kcritsterheat2,kcritstresscold1,kcritstresscold2,kdf,krespinternode,krespmaintleaf,krespmaintroot,krespmaintsheath,kresppanicle,ktempmaint,leaflengthmax,lifesavingdrainage,mulch,panstructmassmax,parcritsla,percolationmax,pevap,pfactor,phyllo,plantsperhill,plotdrainagedaf,poidssecgrain,pourcruiss,ppcrit,ppexp,ppsens,prioritypan,profracini,ranklongestleaf,relmobiliinternodemax,relphyllophasestemelong,rollingbase,rollingsens,rootcstr,rootfrontmax,rootpartitmax,ru,sdjbvp,sdjlevee,sdjmatu1,sdjmatu2,sdjrpr,seuilcstrmortality,seuilpp,seuilruiss,slamax,slamin,sowing,stockiniprof,stockinisurf,tbase,tempsla,tilability,tlim,topt1,topt2,transplanting,transplantingdepth,txassimbvp,txassimmatu1,txassimmatu2,txconversion,txresgrain,txrusurfgermi,vracbvp,vraclevee,vracmatu1,vracmatu2,vracpsp,vracrpr,waterloggingsens,wsalt,wslat,wslong,wtratioleafsheath";

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
    QStringList paramStrList = paramList.split(",");
    for(auto const& token: params->doubles) {
        if(paramStrList.contains(QString::fromStdString(token.first), Qt::CaseInsensitive))
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
    int i = qHash(s);
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
            QString key = keys[index.row()];
            double r = parameters->getDouble(key.toStdString());
            if(r == -999 && role == Qt::DisplayRole)
                return "Null value";
            else if(key.contains("date") || key == "sowing") {
                return QString::fromStdString(JulianCalculator::toStringDate(r, JulianCalculator::YMD, '-'));
            }
            return QString::number(r, 'g', 10);
            return r;

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
    if(QString::fromStdString(key).contains("date") || key == "sowing") {
        QRegExp dateValidator("[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]");
        if(dateValidator.exactMatch(value.toString())) {
            parameters->doubles[key].first = JulianCalculator::toJulianDay(value.toString().toStdString(), JulianCalculator::YMD, '-');
            return true;
        }
        return false;
    }

    bool ok;
    double d = value.toDouble(&ok);
    if(ok)
        parameters->doubles[key].first = d;
    return ok;
}
