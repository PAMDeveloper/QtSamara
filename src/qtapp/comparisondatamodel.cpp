#include "ComparisonDataModel.h"
#include <QSet>
using namespace std;

ComparisonDataModel::ComparisonDataModel(QMap < QString, QtCharts::QLineSeries * > resultsSeries, QMap < QString, QtCharts::QLineSeries * > refSeries,
                                         QList<serieCompare> comparisons, QStringList headers, QDate startDate, bool clean, QObject *parent)
 : QAbstractTableModel(parent), refSeries(refSeries), resultsSeries(resultsSeries),
   clean(clean), comparisons(comparisons), headers(headers), startDate(startDate)
{
    for (int i = 0; i < headers.size(); ++i) {
        if(     comparisons[i].valid &&
                std::abs(comparisons[i].sumSrc) > 0.00001 &&
                std::abs(comparisons[i].sumRef) > 0.00001 &&
                std::abs(comparisons[i].diffPercent) > 0.0001)
            cleanSeries.push_back(i);
    }
}


int ComparisonDataModel::rowCount(const QModelIndex &parent) const {
    return refSeries[ headers[ cleanSeries[0] ] ]->count();
}
int ComparisonDataModel::columnCount(const QModelIndex &parent) const {
    return clean ? cleanSeries.size() : headers.size();
}

QVariant ComparisonDataModel::data(const QModelIndex &index, int role) const{

    QString header = clean ? headers[cleanSeries[index.column()]] : headers[index.column()];

    if(role == Qt::DisplayRole) {
        QString result = "";
        if( resultsSeries.contains(header) )
            result += QString::number(resultsSeries[header]->at(index.row()).y());
        else {
            result += ".";
        }

        if( refSeries.contains(header) )
            result += " - " + QString::number(refSeries[header]->at(index.row()).y());
        else {
            result += " - .";
        }
        return result;
    }

    if(role == Qt::BackgroundColorRole || role == Qt::UserRole) {
        if(!resultsSeries.contains(header) || !refSeries.contains(header))
            return QColor(Qt::lightGray);
        double valRes = -1;
        double valRef = -1;
        if( resultsSeries.contains(header) )
            valRes = resultsSeries[header]->at(index.row()).y();

        if( refSeries.contains(header) )
            valRef = refSeries[header]->at(index.row()).y();

        bool different = false;
        if( std::abs((valRes-valRef)/valRef) > 0.0001 && std::abs(valRes) > 1e-12 && std::abs(valRef)>1e-12) different = true;

        if(role == Qt::BackgroundColorRole) {
            return different ? QColor(Qt::red).lighter(150) : QColor(Qt::white);
        }

        if(role == Qt::UserRole) {
            return different ? true : false;
        }
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant ComparisonDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        QString name = clean ? headers[cleanSeries[section]] : headers[section];
        double diff = comparisons[headers.indexOf(name)].diffPercent;
        return name + "\n" + (diff != 1 ? QString::number(diff, 'f', 2) + "%" : "");
    }

    if(orientation == Qt::Vertical && role == Qt::DisplayRole){
        QDateTime date;
        date.setDate(startDate);
        date = date.addDays(section);
        return QString::number(section) + ": " + date.toString("dd-MM");
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}
