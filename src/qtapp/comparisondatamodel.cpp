#include "comparisondatamodel.h"
#include <QSet>
#include <numeric>
#include <algorithm>
#include <string>
using namespace std;

#include <QDebug>
ComparisonDataModel2::ComparisonDataModel2(const pair <vector <string>, vector < vector <double> > > & results,
                                         const pair <vector <string>, vector < vector <double> > > & refs,
                                         QObject *parent)
 : QAbstractTableModel(parent), results(results), refs(refs)
{
    for (int j = 0; j < refs.first.size(); ++j) {
        bool paired = false;
        string refName = refs.first[j];
        for (int i = 0; i < results.first.size(); ++i) {
            string resName = results.first[i];
            std::transform(refName.begin(), refName.end(), refName.begin(), ::tolower);
            std::transform(resName.begin(), resName.end(), resName.begin(), ::tolower);
//            qDebug() << QString::fromStdString(refName) << QString::fromStdString(resName);
            if( resName == refName ){
                paired = true;
//                qDebug() << "*****************" << QString::fromStdString(refName) << QString::fromStdString(refs.first[j]);
                double sumRes = accumulate(results.second[i].begin(), results.second[i].end(), 0.0000);
                double sumRef = accumulate(refs.second[j].begin(), refs.second[j].end(), 0.0000);
                double diff = qAbs((sumRes - sumRef)/sumRes);
//                if(sumRef != 0 && sumRes != 0
////                        && diff > 0.0001
////                        || refs.first[j] == "degresdujour"
////                        || refs.first[j] == "daylength"
//                        )
                if(sumRef != 0)
                    visibleHeaders.push_back(QPair<int,int>(i,j));
                else {
                    qDebug() << QString::fromStdString(refName) << " equals 0";
                }
            }
        }
        if(!paired) {
            qDebug() << QString::fromStdString(refName) << " not paired";
        }
    }
//    for (int i = 0; i < headers.size(); ++i) {
//        if(     comparisons[i].valid &&
//                std::abs(comparisons[i].sumSrc) > 0.00001 &&
//                std::abs(comparisons[i].sumRef) > 0.00001 &&
//                std::abs(comparisons[i].diffPercent) > 0.0001)
//            cleanSeries.push_back(i);
//    }
}


int ComparisonDataModel2::rowCount(const QModelIndex &parent) const {
    return refs.second[ visibleHeaders[0].first].size();
}
int ComparisonDataModel2::columnCount(const QModelIndex &parent) const {
    return /*clean ? cleanSeries.size() : */visibleHeaders.size();
}

QVariant ComparisonDataModel2::data(const QModelIndex &index, int role) const{

    if(index.column() > visibleHeaders.count() - 1)
        return QVariant();
//    QString header = clean ? visibleHeaders[cleanSeries[index.column()]] : visibleHeaders[index.column()];

    if(role == Qt::DisplayRole) {
        QString result = QString::number(results.second[ visibleHeaders[index.column()].first][index.row()]);
        QString ref = QString::number(refs.second[visibleHeaders[index.column()].second][index.row()]);
        return result + " - " + ref;
    }

//    if(role == Qt::BackgroundColorRole || role == Qt::UserRole) {
//        if(!resultsSeries.contains(header) || !refSeries.contains(header))
//            return QColor(Qt::lightGray);
//        double valRes = -1;
//        double valRef = -1;
//        if( resultsSeries.contains(header) )
//            valRes = resultsSeries[header]->at(index.row()).y();

//        if( refSeries.contains(header) )
//            valRef = refSeries[header]->at(index.row()).y();

//        bool different = false;
//        if( std::abs((valRes-valRef)/valRef) > 0.0001 && std::abs(valRes) > 1e-12 && std::abs(valRef)>1e-12) different = true;

        if(role == Qt::BackgroundColorRole) {
            double ref = refs.second[visibleHeaders[index.column()].second][index.row()];
            double res = results.second[ visibleHeaders[index.column()].first][index.row()];
            if(ref==res) return QColor(Qt::white);
            double variation = qAbs(res-ref)/qAbs((ref+res)/3);

            return QColor(/*variation < 2 ? 255 : (int)(255-qMin(variation*50.,255.))*/255,(int)(255-qMin(variation*100.,255.)),(int)(255-qMin(variation*100.,255.)));
        }

        if(role == Qt::UserRole) {
            double ref = refs.second[visibleHeaders[index.column()].second][index.row()];
            double res = results.second[ visibleHeaders[index.column()].first][index.row()];
//            if( ref!=ref || res != res )
//                return false;
            return qAbs(ref-res) < 0.0000001;
        }
//    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return QVariant();
}

QVariant ComparisonDataModel2::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
//        QString name = /*clean ? visibleHeaders[cleanSeries[section]] : */results.first[visibleHeaders[section].first];
        QString name = QString::fromStdString(results.first[visibleHeaders[section].first]);
//        double diff = comparisons[visibleHeaders.indexOf(name)].diffPercent;
//        return name + "\n" + (diff != 1 ? QString::number(diff, 'f', 2) + "%" : "");
        return name;
    }

//    if(orientation == Qt::Vertical && role == Qt::DisplayRole){
//        QDateTime date;
//        date.setDate(startDate);
//        date = date.addDays(section);
//        return QString::number(section) + ": " + date.toString("dd-MM");
//    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

#include <QFile>
#include <QTextStream>
void ComparisonDataModel2::save() {
    QFile file("exportComparison.csv");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QString sep = ";";
    for (int col = 0; col < columnCount(); ++col) {
//        if(!index(95,col).data(Qt::UserRole).toBool()) {
            out << QString::fromStdString(results.first[visibleHeaders[col].first])
                    << sep << QString::fromStdString(results.first[visibleHeaders[col].first])
                    << sep;
//        }
    }
    out<<"\n";
    for (int col = 0; col < columnCount(); ++col) {
//        if(!index(95,col).data(Qt::UserRole).toBool()) {
            out << "C++" << sep << "Ecotrop" << sep;
//        }
    }
    out<<"\n";
    for (int row = 0; row < rowCount(); ++row) {
        for (int col = 0; col < columnCount(); ++col) {
//            if(!index(95,col).data(Qt::UserRole).toBool()) {
                QString result = QString::number(results.second[ visibleHeaders[col].first][row]);
                QString ref = QString::number(refs.second[visibleHeaders[col].second][row]);
                out << result << sep << ref << sep;
//            }
        }
        out << "\n";
    }
    file.close();
}
