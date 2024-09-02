#include "parametersdatamodel.h"
#include <QHash>
#include <QtCore/qmath.h>
#include "utils/juliancalculator.h"

#include <QFile>
#include <QTextStream>
#include <QDate>

#include <QDebug>

const QString paramList = "asscstr,attenmitch,bundheight,ca,co2cp,co2exp,co2slopetr,coeffassimsla,coefffixedtillerdeath,coefficientq10,coeffinternodemass,coeffinternodenum,coeffleafdeath,coeffleafwlratio,coefflodging,coeffpaniclemass,coeffpansinkpop,coeffrescapacityinternode,coeffreservesink,coeffrootmasspervolmax,coeffterminalleafdeath,coefftillerdeath,coefftransplantingshock,densityfield,densitynursery,depthsoil,devcstr,durationnursery,endingdate,epaisseurprof,epaisseursurf,excessassimtoroot,ftswirrig,hauncrittillering,hauncritstemelongation,humcr,humfc,humpf,humsat,ictillering,internodelengthmax,irrigauto,irrigautoresume,irrigautostop,irrigautotarget,kcmax,kcritstercold1,kcritstercold2,kcritsterftsw1,kcritsterftsw2,kcritsterheat1,kcritsterheat2,kcritstresscold1,kcritstresscold2,kdf,kpar,krespinternode,krespmaintleaf,krespmaintroot,krespmaintsheath,kresppanicle,ktempmaint,leaflengthmax,lifesavingdrainage,mulch,panstructmassmax,parcritsla,percolationmax,pevap,pfactor,phyllo,plantsperhill,plotdrainagedaf,poidssecgrain,pourcruiss,ppcrit,ppexp,ppsens,prioritypan,profracini,ranklongestleaf,relmobiliinternodemax,relphyllophasestemelong,rollingbase,rollingsens,rootcstr,rootfrontmax,rootlignin,rootpartitmax,ru,sdjbvp,sdjlevee,sdjmatu1,sdjmatu2,sdjrpr,seuilcstrmortality,seuilpp,seuilruiss,slamax,slamin,slaswitch,sowing,startingdate,stemporosity,stockiniprof,stockinisurf,tbase,tempsla,tilability,tlim,topt1,topt2,transplanting,transplantingdepth,txassimbvp,txassimmatu1,txassimmatu2,txconversion,txrealloc,txresgrain,txrusurfgermi,volrelmacropores,vracbvp,vraclevee,vracmatu1,vracmatu2,vracpsp,vracrpr,waterloggingsens,wsalt,wslat,wslong,wtratioleafsheath,dryseedingswitch";
const QMap<QString,QString> categories = {
    {"BiomasseRacinaire","sarrah_observations"},
    {"BiomasseTiges","sarrah_observations"},
    {"BiomasseTotale","sarrah_observations"},
    {"CulmsPerHill","sarrah_observations"},
    {"CulmsPerPlant","sarrah_observations"},
    {"CumWReceived","sarrah_observations"},
    {"CumWUse","sarrah_observations"},
    {"DryMatAboveGroundPop","sarrah_observations"},
    {"DryMatPanicleTotPop","sarrah_observations"},
    {"DryMatResInternodePop","sarrah_observations"},
    {"DryMatStemPop","sarrah_observations"},
    {"DryMatStructLeafPop","sarrah_observations"},
    {"DryMatStructRootPop","sarrah_observations"},
    {"FTSW","sarrah_observations"},
    {"FTSW80","sarrah_observations"},
    {"FTSW100","sarrah_observations"},
    {"GrainYieldPop","sarrah_observations"},
    {"Ic","sarrah_observations"},
    {"Lai","sarrah_observations"},
    {"LIRkdfcl","sarrah_observations"},
    {"HarvestIndex","sarrah_observations"},
    {"HaunIndex","sarrah_observations"},
    {"NbJas","sarrah_observations"},
    {"NbreRegimesMois","sarrah_observations"},
    {"PlantHeight","sarrah_observations"},
    {"Rdt","sarrah_observations"},
    {"RdtRegimeMois","sarrah_observations"},
    {"RendementGrains","sarrah_observations"},
    {"RendementHuile","sarrah_observations"},
    {"RootFront","sarrah_observations"},
    {"Sla","sarrah_observations"},
    {"SpikeNumPanicle","sarrah_observations"},
    {"SterilityTot","sarrah_observations"},
    {"StockRac","sarrah_observations"},
    {"StockSurface","sarrah_observations"},
    {"StockTotal","sarrah_observations"},
    {"1","sarrah_observations"},
    {"2","sarrah_observations"},
    {"3","sarrah_observations"},
    {"Edit","sarrah_observations"},
    {"simcode","simulation"},
    {"itkcode","simulation"},
    {"fieldcode","simulation"},
    {"variety","simulation"},
    {"wscode","simulation"},
    {"startingdate","simulation"},
    {"endingdate","simulation"},
    {"sitecode","site"},
    {"countrycode","site"},
    {"sitename","site"},
    {"sitelat","site"},
    {"sitelong","site"},
    {"sitealt","site"},
    {"trialcode","trial"},
    {"sitecode","trial"},
    {"trial","trial"},
    {"startingdate","trial"},
    {"endingdate","trial"},
    {"soilcode","trial"},
    {"trialat","trial"},
    {"trialong","trial"},
    {"trialalt","trial"},
    {"rootdepth","trial"},
    {"weatherstring","trial"},
    {"commentary","trial"},
    {"weathergradiancode","trial"},
    {"projectcode","trial"},
    {"random","trial"},
    {"variety","variety"},
    {"cropcode","variety"},
    {"sdjlevee","variety"},
    {"sdjbvp","variety"},
    {"sdjrpr","variety"},
    {"sdjmatu1","variety"},
    {"sdjmatu2","variety"},
    {"phyllo","variety"},
    {"devcstr","variety"},
    {"ppexp","variety"},
    {"seuilpp","variety"},
    {"ppsens","variety"},
    {"ppcrit","variety"},
    {"tbase","variety"},
    {"topt1","variety"},
    {"topt2","variety"},
    {"tlim","variety"},
    {"txconversion","variety"},
    {"txassimbvp","variety"},
    {"txassimmatu1","variety"},
    {"txassimmatu2","variety"},
    {"asscstr","variety"},
    {"txrealloc","variety"},
    {"krespmaintleaf","variety"},
    {"krespmaintsheath","variety"},
    {"krespmaintroot","variety"},
    {"krespinternode","variety"},
    {"kresppanicle","variety"},
    {"ktempmaint","variety"},
    {"coefficientq10","variety"},
    {"kcmax","variety"},
    {"txrusurfgermi","variety"},
    {"pfactor","variety"},
    {"seuilcstrmortality","variety"},
    {"kdf","variety"},
    {"poidssecgrain","variety"},
    {"txresgrain","variety"},
    {"vraclevee","variety"},
    {"vracbvp","variety"},
    {"vracpsp","variety"},
    {"vracrpr","variety"},
    {"vracmatu1","variety"},
    {"vracmatu2","variety"},
    {"rootcstr","variety"},
    {"coeffrootmasspervolmax","variety"},
    {"rootpartitmax","variety"},
    {"slamin","variety"},
    {"slamax","variety"},
    {"attenmitch","variety"},
    {"internodelengthmax","variety"},
    {"leaflengthmax","variety"},
    {"coeffleafdeath","variety"},
    {"wtratioleafsheath","variety"},
    {"coeffleafwlratio","variety"},
    {"rollingbase","variety"},
    {"rollingsens","variety"},
    {"ranklongestleaf","variety"},
    {"coeffinternodemass","variety"},
    {"coeffinternodenum","variety"},
    {"relmobiliinternodemax","variety"},
    {"coeffrescapacityinternode","variety"},
    {"tilability","variety"},
    {"coefftillerdeath","variety"},
    {"coefffixedtillerdeath","variety"},
    {"coeffterminalleafdeath","variety"},
    {"coeffpaniclemass","variety"},
    {"panstructmassmax","variety"},
    {"coeffpansinkpop","variety"},
    {"kcritstercold1","variety"},
    {"kcritstercold2","variety"},
    {"kcritsterheat1","variety"},
    {"kcritsterheat2","variety"},
    {"kcritsterftsw1","variety"},
    {"kcritsterftsw2","variety"},
    {"kcritstresscold1","variety"},
    {"kcritstresscold2","variety"},
    {"hauncrittillering","variety"},
    {"hauncritstemelongation", "variety"},
    {"ictillering","variety"},
    {"tempsla","variety"},
    {"relphyllophasestemelong","variety"},
    {"excessassimtoroot","variety"},
    {"prioritypan","variety"},
    {"waterloggingsens","variety"},
    {"rootfrontmax","variety"},
    {"coeffreservesink","variety"},
    {"co2slopetr","variety"},
    {"co2exp","variety"},
    {"co2cp","variety"},
    {"coeffassimsla","variety"},
    {"parcritsla","variety"},
    {"coefflodging","variety"},
    {"stemporosity","variety"},
    {"rootlignin","variety"},
    {"wscode","wdataday"},
    {"weatherdate","wdataday"},
    {"tmin","wdataday"},
    {"tmax","wdataday"},
    {"tmoy","wdataday"},
    {"rhmin","wdataday"},
    {"rhmax","wdataday"},
    {"rhmoy","wdataday"},
    {"rainfall","wdataday"},
    {"windtot","wdataday"},
    {"radiation","wdataday"},
    {"sunshine","wdataday"},
    {"eto","wdataday"},
    {"wscode","ws"},
    {"wsname","ws"},
    {"wstype","ws"},
    {"countrycode","ws"},
    {"wslat","ws"},
    {"wslong","ws"},
    {"wsalt","ws"},
    {"trialcode","wstrial"},
    {"wscode","wstrial"},
    {"weathweight","wstrial"},
    {"rainweight","wstrial"},
    {"windtot","wdataday"},
    {"radiation","wdataday"},
    {"sunshine","wdataday"},
    {"eto","wdataday"},
    {"wscode","ws"},
    {"wsname","ws"},
    {"wstype","ws"},
    {"countrycode","ws"},
    {"wslat","ws"},
    {"wslong","ws"},
    {"wsalt","ws"},
    {"trialcode","wstrial"},
    {"wscode","wstrial"},
    {"weathweight","wstrial"},
    {"rainweight","wstrial"},
    {"dryseedingswitch","trial"}
};


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


int ParametersDataModel::rowCount(const QModelIndex &/*parent*/) const {
    return keys.size();
}
int ParametersDataModel::columnCount(const QModelIndex &/*parent*/) const {
    return 2;
}

QColor ParametersDataModel::getColor(QString s) {
    int i = qHash(s);
    double PHI = (1 + qSqrt(5)) / 2;
    double n = i * PHI - floor(i * PHI);
    int h = qFloor(n * 256);
    return QColor::fromHsv(h, 245, 245, 255);
}


bool compare(const std::pair<std::string, std::pair<double, std::string>>& pair1,
             const std::pair<std::string, std::pair<double, std::string>>& pair2) {
    // First, compare based on the string in the pair
    if (pair1.second.second < pair2.second.second)
        return true;
    if (pair1.second.second > pair2.second.second)
        return false;

    // If the string in the pair is equal, compare based on the key string
    return pair1.first < pair2.first;
}

//std::multimap<std::string, std::pair<double, std::string>> get_sorted_map(std::map<std::string, std::pair<double, std::string>> map) {
//    std::multimap<std::string, std::pair<double, std::string>> sortedMap;

//    // Copy the map elements to the multimap
//    for (const auto& pair : myMap) {
//        sortedMap.insert(pair);
//    }

//    // Sort the multimap using the custom comparator
//    std::multimap<std::string, std::pair<double, std::string>,
//                  bool(*)(const std::pair<std::string, std::pair<double, std::string>>&, const std::pair<std::string, std::pair<double, std::string>>&)>
//        finalMap(compare);
//    finalMap.insert(sortedMap.begin(), sortedMap.end());
//    return finalMap;
//}

void ParametersDataModel::addKey(QString key){
    beginResetModel();
    keys.append(key);
    qSort(keys.begin(), keys.end(), KeyLessThan(parameters));
    endResetModel();
}
QVariant ParametersDataModel::data(const QModelIndex &index, int role) const{
    if(index.column() == 0 && role == Qt::DisplayRole)
        return keys[index.row()];

    if(index.column() == 1 && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        try
        {
            QString key = keys[index.row()];
            double r = parameters->getDouble(key.toStdString());
            if (key == "sowing" || key == "endingdate" || key == "startingdate")
                return QString::fromStdString(JulianCalculator::toStringDate(r, JulianCalculator::YMD, '-'));
//            if(r == -999 && role == Qt::DisplayRole)
//                return "Null value";
//            else if(key.contains("date") || key == "sowing") {
//                return QString::fromStdString(JulianCalculator::toStringDate(r, JulianCalculator::YMD, '-'));
//            }
            return QString::number(r, 'g', 10);
//            return r;

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
        return Qt::ItemIsEditable |  QAbstractTableModel::flags(index);

    return QAbstractTableModel::flags(index);
}

bool ParametersDataModel::setData(const QModelIndex &idx, const QVariant &value, int role) {
    if(role != Qt::EditRole)
        return false;
    string key = data(index(idx.row(),0), Qt::DisplayRole).toString().toStdString();
    if (key == "sowing" || key == "endingdate" || key == "startingdate") {
        QRegExp dateValidator("[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]");
        if(dateValidator.exactMatch(value.toString())) {
            parameters->doubles[key].first = JulianCalculator::toJulianDay(value.toString().toStdString(), JulianCalculator::YMD, '-');
            parameters->strings[key].first = value.toString().toStdString();
            emit date_changed(QString::fromStdString(key), parameters->doubles[key].first);
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


bool ParametersDataModel::save(QString path, QString sep) {
    QFile file(path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << parameters->strings.size() << "\n";
        for (auto const& token : parameters->strings) {
            out << QString::fromStdString(token.first) << sep << QString::fromStdString(token.second.first) << "\n";
        }

        out << parameters->doubles.size() << "\n";
        for (auto const& token : parameters->doubles) {
            out << fixed << QString::fromStdString(token.first) << sep << QString::number(token.second.first, 'f') << "\n";
        }

//        for (int row = 0; row < rowCount(); ++row) {
//            for (int col = 0; col < columnCount(); ++col) {
//                    out << index(row,col).data().toString() << sep;
//            }
//            out << "\n";
//        }

        file.close();
        return true;
    }
    return false;
}


bool ParametersDataModel::load(QString path, QString sep) {
    beginResetModel();
    keys.clear();
    parameters->doubles.clear();
    parameters->strings.clear();
    QStringList paramStrList = paramList.split(",");
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        int rowCt = in.readLine().toInt();
        for (int row = 0; row < rowCt; ++row) {
            QString line = in.readLine().remove("\\");
            QStringList lstLine = line.split(sep);

            std::string param_name = lstLine[0].toStdString();
            if(parameters->doubles.find(param_name) == parameters->doubles.end() && paramStrList.contains(QString::fromStdString(param_name), Qt::CaseInsensitive)) {
                QString category = categories[lstLine[0]];
                parameters->strings[param_name] = pair <string, string> ( lstLine[1].toStdString(), category.toStdString() );
//                keys << QString::fromStdString(param_name);
            } else {
                parameters->strings[ param_name ].first = lstLine[1].toStdString();
            }

        }

        rowCt = in.readLine().toInt();
//        for (int row = 0; row < rowCt; ++row) {
        while (!in.atEnd()){
            QString line = in.readLine().remove("\\").remove('"');
            QStringList lstLine = line.split(sep);
//            qDebug() << line;
//            qDebug() << lstLine;
//            qDebug() << lstLine[0];
//            qDebug() << lstLine[1];

            std::string param_name = lstLine[0].toStdString();
            if(parameters->doubles.find(param_name) == parameters->doubles.end() && paramStrList.contains(QString::fromStdString(param_name), Qt::CaseInsensitive)) {
                QString category = categories[lstLine[0]];
                parameters->doubles[param_name] =
                        pair <double, string> (
                            lstLine[1].toDouble(), category.toStdString());
                keys << QString::fromStdString(param_name);
            } else {
                parameters->doubles[ lstLine[0].toStdString() ].first = lstLine[1].toDouble();
            }
//            line = in.readLine().strip();
        }

        for (QString param_name: paramStrList){
            if(parameters->doubles.find(param_name.toStdString()) == parameters->doubles.end()) {
                double default_value = 0.0;
                if(param_name == "hauncritstemelongation")
                    default_value = 20.0;
                parameters->doubles[param_name.toStdString()] =
                        pair <double, string> (
                            default_value, categories[param_name].toStdString());
                keys << param_name;
            }
        }

        qSort(keys.begin(), keys.end(), KeyLessThan(parameters));

        dataChanged(index(0,0),index(rowCount()-1,1));
        endResetModel();
        emit date_changed(QString::fromStdString("startingdate"), parameters->doubles["startingdate"].first);
        emit date_changed(QString::fromStdString("endingdate"), parameters->doubles["endingdate"].first);
        file.close();
        return true;
    }
    return false;
}

void ParametersDataModel::changeDate(QString key, QDate jdate) {
    beginResetModel();
    if (key == "startingdate") {
        parameters->doubles["startingdate"].first = jdate.toJulianDay();
        parameters->strings["startingdate"].first = jdate.toString("yyyy-MM-dd").toStdString();

    } else if (key == "endingdate") {
        parameters->doubles["endingdate"].first = jdate.toJulianDay();
        parameters->strings["endingdate"].first = jdate.toString("yyyy-MM-dd").toStdString();
    }
    endResetModel();
}
