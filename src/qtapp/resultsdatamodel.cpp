#include "resultsdatamodel.h"
#include <numeric>
#include <QFile>
#include <QTextStream>
#include <utils/juliancalculator.h>

using namespace std;

ResultsDataModel::ResultsDataModel(QObject *parent)
    : QAbstractTableModel(parent) {
}

void ResultsDataModel::setResults(const pair <vector <string>, vector < vector <double> > > & results) {
    beginResetModel();
    this->results = results;
    endResetModel();
}

int ResultsDataModel::rowCount(const QModelIndex &/*parent*/) const {
    if(columnCount() == 0)
        return 0;
    return (int)results.second[0].size();
}

int ResultsDataModel::columnCount(const QModelIndex &/*parent*/) const {
    return (int)results.first.size();
}

QVariant ResultsDataModel::data(const QModelIndex &index, int role) const{

    if(role == Qt::DisplayRole) {
        double val = results.second[index.column()][index.row()];
        if(index.column() == 0) {
            return QString::fromStdString(JulianCalculator::toStringDate(val, JulianCalculator::YMD,'-'));
        }
        QString s = QString::number(val, 'f', 10);
        s.remove( QRegExp("\\.?0+$") );
        return s;
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant ResultsDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return QString::fromStdString(results.first[section]);
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

void ResultsDataModel::save(QString path, QString sep) {
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    for (int col = 0; col < columnCount(); ++col) {
            out << QString::fromStdString(results.first[col])
                    << sep ;
    }
    out<<"\n";
    for (int row = 0; row < rowCount(); ++row) {
        for (int col = 0; col < columnCount(); ++col) {
                QString result = QString::number(results.second[col][row], 'f', 10);
                out << result << sep;
        }
        out << "\n";
    }
    file.close();
}
