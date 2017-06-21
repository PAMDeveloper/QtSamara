#ifndef METEODATAMODEL_H
#define METEODATAMODEL_H

#include <QAbstractTableModel>
#include <parameters.h>

class MeteoDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MeteoDataModel(SamaraParameters * parameters, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
//    Qt::ItemFlags flags(const QModelIndex &index) const;
//    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
//    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
//    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
//     samara::ModelParameters getParameters();

private:
    SamaraParameters * parameters;
    QStringList keys;
};

#endif // METEODATAMODEL_H
