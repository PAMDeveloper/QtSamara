#include "meteodatamodel.h"
#include <utils/juliancalculator.h>

#include <QFile>
#include <QDebug>
#include <QDate>
#include <QTextStream>

MeteoDataModel::MeteoDataModel(SamaraParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{

    starting_date = JulianCalculator::toJulianDay("01/01/2010", "YMD", "/");
    ending_date = JulianCalculator::toJulianDay("01/12/2010", "YMD", "/");
    qDebug() << starting_date << ending_date;
}

int MeteoDataModel::rowCount(const QModelIndex &/*parent*/ = QModelIndex()) const {
    int rowCount = qMax<double>(0, ending_date - starting_date);
//    qDebug() << rowCount;
    return rowCount;
}
int MeteoDataModel::columnCount(const QModelIndex &/*parent*/) const {
    return 12;
}

QVariant MeteoDataModel::data(const QModelIndex &index, int role) const{
    int row = index.row();
    double current_jday = starting_date + row;
//    qDebug() << fixed << current_jday << starting_date << ending_date << starting_climate << ending_climate;
    if(role == Qt::DisplayRole){
        double val;
        if (climate_data.find( current_jday ) == climate_data.end())
            return "Out";
        else {
            std::vector<double> c = climate_data.at(current_jday);
            switch(index.column()){
//                case 0: return QString::fromStdString(JulianCalculator::toStringDate(c[0], JulianCalculator::YMD, '-')); break;
                case 0: return QString::fromStdString(JulianCalculator::toStringDate(current_jday, JulianCalculator::YMD, '-')); break;
                case 1: val = c[1]; break;
                case 2: val = c[0]; break;
                case 3: val = c[2]; break;
                case 4: val = c[4]; break;
                case 5: val = c[3]; break;
                case 6: val = c[5]; break;
                case 7: val = c[7]; break;
                case 8: val = c[9]; break;
                case 9: val = c[8]; break;
                case 10: val = c[10]; break;
                case 11: val = c[6]; break;
//                case 12: val = -1; break;
//                case 12: val = parameters->getIrrigation(row); break;
    //            case 0: return QString::fromStdString(
    //                        JulianCalculator::toStringDate(
    //                            parameters->getDouble("startingdate")+row-1,
    //                            JulianCalculator::YMD, '-'
    //                            )); break;
    //            case 1: val = parameters->climatics.at(row).TMax; break;
    //            case 2: val = parameters->climatics.at(row).TMin; break;
    //            case 3: val = parameters->climatics.at(row).TMoy; break;
    //            case 4: val = parameters->climatics.at(row).HMax; break;
    //            case 5: val = parameters->climatics.at(row).HMin; break;
    //            case 6: val = parameters->climatics.at(row).HMoy; break;
    //            case 7: val = parameters->climatics.at(row).Vt; break;
    //            case 8: val = parameters->climatics.at(row).Ins; break;
    //            case 9: val = parameters->climatics.at(row).Rg; break;
    //            case 10: val = parameters->climatics.at(row).ETP; break;
    //            case 11: val = parameters->climatics.at(row).Rain; break;
    ////			case 12: val = parameters->getIrrigation(row); break;
            }
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
//			case 12: return "Irrigation"; break;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool MeteoDataModel::save(QString path, QString sep) {
    QFile file(path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto const& token : parameters->climatics) {
            out << token.TMax << sep << token.TMin << sep << token.TMoy << sep << token.HMax
                << sep << token.HMin<< sep << token.HMoy << sep << token.Vt << sep <<
                   token.Ins << sep << token.Rg << sep << token.Rain << sep << token.ETP << "\n";
        }
        file.close();
        return true;
    }
    return false;
}

void MeteoDataModel::set_starting_date(QDate date){
    beginResetModel();
    double jday = date.toJulianDay();
    starting_date = jday;
//    qDebug() << starting_date;
    endResetModel();
}

void MeteoDataModel::set_ending_date(QDate date) {
    beginResetModel();
    double jday = date.toJulianDay();
    ending_date = jday;
//    qDebug() << ending_date;
    endResetModel();
}

void MeteoDataModel::populate(SamaraParameters * params) {
    for (int row = 0; row < this->rowCount(); ++row) {
        Climate c;
        for (int column = 0; column < this->columnCount(QModelIndex()); ++column) {
            QModelIndex index = this->index(row, column);
            double val = this->data(index, Qt::DisplayRole).toDouble();
            switch (column) {
                case 1: c.TMax = val; break;
                case 2: c.TMin = val; break;
                case 3: c.TMoy = val; break;
                case 4: c.HMax = val; break;
                case 5: c.HMin = val; break;
                case 6: c.HMoy = val; break;
                case 7: c.Vt = val; break;
                case 8: c.Ins = val; break;
                case 9: c.Rg = val; break;
                case 10: c.ETP = val; break;
                case 11: c.Rain = val; break;
            }
        }
        params->climatics.push_back(c);
    }
}

bool compareFirstCell(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    return vec1[0] < vec2[0];  // Compare the first cell of the second dimension
}

bool MeteoDataModel::load(QString path, QString sep) {
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        if(sep.isEmpty())
            sep = line.contains(";") ? ";" : "\t";
//        parameters->climatics.clear();
        sep = ";";
        line = in.readLine();
        QStringList lstLine = line.split(sep);
        qDebug() << lstLine[1];
//        starting_climate = JulianCalculator::toJulianDay(lstLine[1].toStdString(), QString("DMY").toStdString(), QString("/").toStdString());
//                JulianCalculator::toJulianDay(lstLine[1].toStdString(), QString("DMY").toStdString(), QString("/").toStdString());
//        qDebug() << starting_climate;
        beginResetModel();
        while(!line.isEmpty()){
            QStringList lstLine = line.split(sep);
            int i = 0;
            i++; //wscode
            double jday = JulianCalculator::toJulianDay(lstLine[i].toStdString(), QString("DMY").toStdString(), QString("/").toStdString());
//            qDebug()<<fixed<<lstLine[i]<<"JDAY"<<jday;
            i++;
            std::vector<double> c;
            c.push_back(lstLine[i].toDouble()); i++; //TMin
            c.push_back(lstLine[i].toDouble()); i++; //TMax
            c.push_back(lstLine[i].toDouble()); i++; //TMoy
            c.push_back(lstLine[i].toDouble()); i++; //HMin
            c.push_back(lstLine[i].toDouble()); i++; //HMax
            c.push_back(lstLine[i].toDouble()); i++; //HMoy
            c.push_back(lstLine[i].toDouble()); i++; //Rain
            c.push_back(lstLine[i].toDouble()); i++; //Vt
            c.push_back(lstLine[i].toDouble()); i++; //Rg
            c.push_back(lstLine[i].toDouble()); i++; //Ins
            c.push_back(lstLine[i].toDouble()); i++; //ETP
            climate_data[jday] = c;


//            for (const auto& element : c) {
//                qDebug() << element;
//            }

//                Climate c;
//            c.TMin = lstLine[++i].toDouble();
//            c.TMax = lstLine[++i].toDouble();
//            c.TMoy = lstLine[++i].toDouble();
//            c.HMin = lstLine[++i].toDouble();
//            c.HMax = lstLine[++i].toDouble();
//            c.HMoy = lstLine[++i].toDouble();
//            c.Rain = lstLine[++i].toDouble();
//            c.Vt = lstLine[++i].toDouble();
//            c.Rg = lstLine[++i].toDouble();
//            c.Ins = lstLine[++i].toDouble();
//            c.ETP  = lstLine[++i].toDouble();
//            parameters->climatics.push_back(c);
//            QString nextline = in.readLine();
//            if (nextline.isEmpty()) {
//                ending_climate = JulianCalculator::toJulianDay(lstLine[1].toStdString(), QString("DMY").toStdString(), QString("/").toStdString());
//                qDebug() << ending_climate;
//                break;
//            }
            line= in.readLine();
        }
        file.close();
//        std::sort(climate_data.begin(), climate_data.end(), compareFirstCell);
//        dataChanged(index(0,0),index(this->rowCount()-1,1));
        endResetModel();
        qDebug() << "Meteo loaded";
        return true;
    }
    return false;
}

bool MeteoDataModel::loadIrrigation(QString path, QString sep) {
	QFile file(path);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        //if (sep.isEmpty())
            sep = line.contains(";") ? ";" : "\t";
        parameters->irrigation.clear();
        for (size_t i = 0; i < parameters->climatics.size(); i++) {
            parameters->irrigation.push_back(-999.);
        }

        beginResetModel();
        while (!line.isEmpty()) {
            QStringList lstLine = line.split(sep);
            double value = lstLine[1].toDouble();
            double date = JulianCalculator::toJulianDay( lstLine[0].toStdString(),
                JulianCalculator::YMD, '-');
            double row = date - parameters->getDouble("startingdate") + 2;
            if(row >=0 && row < parameters->irrigation.size())
                parameters->irrigation[row] = value;
            line = in.readLine();
        }
        file.close();
		endResetModel();
		return true;
	}
	return false;
}
