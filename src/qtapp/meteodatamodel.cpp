#include "meteodatamodel.h"
#include <utils/juliancalculator.h>

#include <QFile>
#include <QTextStream>

MeteoDataModel::MeteoDataModel(SamaraParameters * params, QObject *parent)
    : QAbstractTableModel(parent), parameters(params)
{}

int MeteoDataModel::rowCount(const QModelIndex &/*parent*/ = QModelIndex()) const {
    return  (int)parameters->climatics.size();
}
int MeteoDataModel::columnCount(const QModelIndex &/*parent*/) const {
    return 13;
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
			case 12: val = parameters->getIrrigation(row); break;
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
			case 12: return "Irrigation"; break;
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

bool MeteoDataModel::load(QString path, QString sep) {
    QFile file(path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString line = in.readLine();
        if(sep.isEmpty())
            sep = line.contains(";") ? ";" : "\t";
        parameters->climatics.clear();
        beginResetModel();
        while(!line.isEmpty()) {
            QStringList lstLine = line.split(sep);
            Climate c;
            int i = -1;
            c.TMax = lstLine[++i].toDouble();
            c.TMin = lstLine[++i].toDouble();
            c.TMoy = lstLine[++i].toDouble();
            c.HMax = lstLine[++i].toDouble();
            c.HMin = lstLine[++i].toDouble();
            c.HMoy = lstLine[++i].toDouble();
            c.Vt = lstLine[++i].toDouble();
            c.Ins = lstLine[++i].toDouble();
            c.Rg = lstLine[++i].toDouble();
            c.Rain = lstLine[++i].toDouble();
            c.ETP  = lstLine[++i].toDouble();
            parameters->climatics.push_back(c);
            line = in.readLine();
        }
        file.close();
//        dataChanged(index(0,0),index(this->rowCount()-1,1));
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