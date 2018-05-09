#ifndef ESTIMPARAMDATAMODEL_H
#define ESTIMPARAMDATAMODEL_H

#include <QAbstractTableModel>
#include <parameters.h>
#include <QColor>
#include <QSettings>

class EstimParamDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    EstimParamDataModel(SamaraParameters * parameters, QObject *parent=0);
    bool isValid(int row) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

    bool save(QString path, QString sep = ";");
    bool load(QString path, QString sep = ";");

    vector<pair<double, double> > bounds();
    vector<string> params();

    static QColor getColor(QString s);

    QStringList keys;
private:
    QSettings * settings;
    QStringList checked;
    SamaraParameters * parameters;
};

#endif // ESTIMPARAMDATAMODEL_H
