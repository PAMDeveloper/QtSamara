#include "meteodatamodel.h"
#include <utils/juliancalculator.h>

#include <QFile>
#include <QStringList>
#include <QMap>
#include <QRegExp>
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
    if(role == Qt::DisplayRole || role == Qt::UserRole){
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
        if(val == -999 && role != Qt::UserRole)
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
    for (int row = 0; row <= this->rowCount(); ++row) {
        Climate c;
        for (int column = 0; column < this->columnCount(QModelIndex()); ++column) {
            QModelIndex index = this->index(row, column);
            double val = this->data(index, Qt::UserRole).toDouble();
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


QString detectDateSeparator(const QStringList &dateList) {
    QMap<QString, int> separatorCount;
    QRegExp sepRegExp("(\\-|\\/|\\\\|\\s)");

    int testCount = qMin(5, dateList.size()); // Check up to the first 5 dates
    for (int i = 0; i < testCount; ++i) {
        if (sepRegExp.indexIn(dateList.at(i)) != -1) {
            QString separator = sepRegExp.cap(1);
            separatorCount[separator]++;
        }
    }

    QString mostCommonSeparator;
    int maxCount = 0;
    for (const QString &separator : separatorCount.keys()) {
        if (separatorCount[separator] > maxCount) {
            mostCommonSeparator = separator;
            maxCount = separatorCount[separator];
        }
    }

    if (mostCommonSeparator.isEmpty()) {
        qDebug() << "No common date separator detected.";
    }

    return mostCommonSeparator;
}

QString detectDatePattern(const QStringList &dateList, const QString &separator) {
    QMap<QString, int> patternCount;
    // Define regex patterns for different date formats
    QRegExp dmyRegExp(QString("^\\d{1,2}\\%1\\d{1,2}\\%1\\d{4}$").arg(separator));
    QRegExp ymdRegExp(QString("^\\d{4}\\%1\\d{1,2}\\%1\\d{1,2}$").arg(separator));
    QRegExp mdyRegExp(QString("^\\d{1,2}\\%1\\d{1,2}\\%1\\d{4}$").arg(separator));

    int testCount = qMin(5, dateList.size()); // Check up to the first 5 dates
    for (int i = 0; i < testCount; ++i) {
        QString date = dateList.at(i);
        if (dmyRegExp.exactMatch(date)) {
            patternCount["DMY"]++;
        } else if (ymdRegExp.exactMatch(date)) {
            patternCount["YMD"]++;
        } else if (mdyRegExp.exactMatch(date)) {
            patternCount["MDY"]++;
        }
    }

    QString mostCommonPattern;
    int maxCount = 0;
    for (const QString &pattern : patternCount.keys()) {
        if (patternCount[pattern] > maxCount) {
            mostCommonPattern = pattern;
            maxCount = patternCount[pattern];
        }
    }

    if (mostCommonPattern.isEmpty()) {
        qDebug() << "No common date pattern detected.";
    }

    return mostCommonPattern;
}


// VERSION THAT INFER COLUMNS
//bool MeteoDataModel::load(QString path, QString sep) {
//    QFile file(path);
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream in(&file);
//        QString headerLine = in.readLine();
//        sep = headerLine.contains(";") ? ";" : "\t";
//        QStringList headerList = headerLine.split(sep);

//        // Create a map to hold the column names and their indices
//        QMap<QString, int> columnIndexMap;
//        for (int i = 0; i < headerList.size(); ++i) {
//            columnIndexMap[headerList[i]] = i;
//        }

//        // Define the expected column names
//        QStringList expectedColumns = {"weatherdate", "tmin", "tmax", "tmoy", "rhmin", "rhmax", "rhmoy", "rainfall", "windtot", "radiation", "sunshine", "eto"};

//        // Verify that all expected columns are present
//        for (const QString& col : expectedColumns) {
//            if (!columnIndexMap.contains(col)) {
//                qDebug() << "Missing column in CSV:" << col;
//                file.close();
//                return false;
//            }
//        }

//        // Extract the next 5 lines to detect the date pattern
//        QStringList dateLines;
//        for (int i = 0; i < 5; ++i) {
//            QString line = in.readLine();
//            if (!line.isEmpty()) {
//                dateLines << line.split(sep).at(columnIndexMap["weatherdate"]);
//            }
//        }

//        // Call functions to detect date separator and pattern
//        QString dateSeparator = detectDateSeparator(dateLines);
//        QString datePattern = detectDatePattern(dateLines, dateSeparator);

//        // Seek back to the beginning of the data lines
////        in.seek(headerLine.length() + 1); // +1 for the newline character

//        beginResetModel();
//        QString line;
//        while (!(line = in.readLine()).isEmpty()) {
//            QStringList lstLine = line.split(sep);

//            // Use the detected date pattern and separator here
//            double jday = JulianCalculator::toJulianDay(lstLine[columnIndexMap["weatherdate"]].toStdString(), datePattern.toStdString(), dateSeparator.toStdString());

//            std::vector<double> c;
//            for (const QString& col : expectedColumns) {
//                if(col == "weatherdate")
//                    continue;
//                QString val = lstLine[columnIndexMap[col]];
//                bool ok;
//                double num = val.toDouble(&ok);
//                c.push_back(ok ? num : -999);
//            }
//            climate_data[jday] = c;
//        }
//        file.close();

//        // ... Rest of your code to handle -999 values ...

//        endResetModel();
//        return true;
//    }
//    return false;
//}



// OLD VERSION
bool MeteoDataModel::load(QString path, QString sep) {
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        if(sep.isEmpty())
            sep = line.contains(";") ? ";" : "\t";

        sep = ";";
        line = in.readLine();
        beginResetModel();
        while(!line.isEmpty()){
            QStringList lstLine = line.split(sep);
            int i = 0;
            i++; //wscode
            double jday = JulianCalculator::toJulianDay(lstLine[i].toStdString(), QString("DMY").toStdString(), QString("/").toStdString());
            i++;
            std::vector<double> c;
            for (int j = 0; j < 11; ++j) {
                QString val = lstLine[i];
                bool ok;
                double num = val.toDouble(&ok);
                if (ok) {
                    c.push_back(num);
                } else {
                    c.push_back(-999);
                }

                i++;
            }
            climate_data[jday] = c;
            line= in.readLine();
        }
        file.close();


        int vector_size = climate_data.begin()->second.size(); // Assuming all vectors have the same size
        int num_keys = climate_data.size();

        for (int index = 0; index < vector_size; ++index) {
           int num_negative_999 = 0;

           // Count the number of -999 values at the current index
           for (const auto& entry : climate_data) {
               if (entry.second[index] == -999.0) {
                   num_negative_999++;
               }
           }

           // Replace -999 values with 0 if necessary
           if (num_negative_999 > 0 && num_negative_999 < num_keys) {
               for (auto& entry : climate_data) {
                   if (entry.second[index] == -999.0) {
                       entry.second[index] = 0.0;
                   }
               }
           }
        }
        endResetModel();
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
            parameters->irrigation.push_back(-999);
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
