#ifndef RESULTSMANAGER_H
#define RESULTSMANAGER_H

#include <QObject>
#include <QTableView>
#include <QHeaderView>
#include <qtapp/resultsdatamodel.h>

class ResultsManager : public QObject
{
    Q_OBJECT
public:
    explicit ResultsManager(QTableView * resultView, ResultsDataModel * resultModel, QObject *parent = nullptr);
    void applyFilters();

private:
    QTableView * resultView;
    ResultsDataModel * resultModel;
    QStringList subsetStrList;
    bool subsetFilter;
    bool subsetInvFilter;
    bool phaseFilter;
    QString strFilter;

signals:

public slots:
    void filterColumns(QString);
    void filterPhases(bool);
    void filterColHeaders(QString);
};

#endif // RESULTSMANAGER_H
