#include "obsdatamodel.h"

#include "ObsDataModel.h"
#include <numeric>
#include <QFile>
#include <QTextStream>
#include <utils/juliancalculator.h>

using namespace std;

ObsDataModel::ObsDataModel(QObject *parent)
    : QAbstractTableModel(parent) {
}

void ObsDataModel::setObs(const map<string, vector<double>> & obs) {
    beginResetModel();
    keys.clear();
    this->obs = obs;
    for(auto it = obs.begin(); it != obs.end(); ++it) {
        keys.push_back(it->first);
    }
    endResetModel();
}

int ObsDataModel::rowCount(const QModelIndex &/*parent*/) const {
    if(columnCount() == 0)
        return 0;
    return (int) obs.at(keys[0]).size();
}

int ObsDataModel::columnCount(const QModelIndex &/*parent*/) const {
    return (int)keys.size();
}

QVariant ObsDataModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole) {
        double val = obs.at(keys[index.column()])[index.row()];
        if(keys[index.column()] == "obsplantdate") {
            return QString::fromStdString(JulianCalculator::toStringDate(val, JulianCalculator::YMD,'-'));
        }
        if(val == -999)
            return "";
        QString s = QString::number(val, 'f', 10);
        s.remove( QRegExp("\\.?0+$") );
        return s;
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant ObsDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return QString::fromStdString(keys[section]);
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

void ObsDataModel::load(QString fileName, QString sep) {
    QFile inputFile(fileName);
    keys.clear();
    obs.clear();
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        QStringList list = in.readLine().split(sep);
        for(QString header: list) {
            keys.push_back(header.toLower().toStdString());
            obs.insert(pair< string,vector<double> >(header.toLower().toStdString(),vector<double>()));
        }

        while (!in.atEnd())
        {
            QStringList list = in.readLine().split('\t');
            for (int i = 0; i < list.size(); ++i) {
                obs[keys[i]].push_back(list[i].replace(',','.').toDouble());
            }
        }
        inputFile.close();
    }

    dataChanged(index(0,0),index(rowCount()-1,1));
}

bool ObsDataModel::save(QString path, QString sep) {
    QFile file(path);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (auto const& token : obs) {
            out << QString::fromStdString(token.first) << sep;
        }
        out << "\n";

        for (int i = 0; i < obs.begin()->second.size(); ++i) {
            for (auto const& token : obs) {
                out << fixed << QString::number(token.second[i]) << sep;
            }
            out << "\n";
        }

        file.close();
        return true;
    }
    return false;
}
