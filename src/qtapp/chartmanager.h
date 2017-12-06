#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>

class ChartManager : public QObject
{
    Q_OBJECT
public:
    explicit ChartManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CHARTMANAGER_H