#ifndef PARAMETERSDATAMODEL_H
#define PARAMETERSDATAMODEL_H

#include <QAbstractTableModel>
#include <parameters.h>
#include <QColor>

class ParametersDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ParametersDataModel(SamaraParameters * parameters, QObject *parent=0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

    bool save(QString path, QString sep = ";");
    bool load(QString path, QString sep = ";");

    static QColor getColor(QString s);
    void changeDate(QString, QDate);
    void addKey(QString);

    SamaraParameters * parameters;
    QStringList keys;
private:
    std::multimap<std::string, std::pair<double, std::string>> local_map;

signals:
    void date_changed(QString, double);
};

#endif // PARAMETERSDATAMODEL_H
