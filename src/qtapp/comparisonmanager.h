#ifndef COMPARISONMANAGER_H
#define COMPARISONMANAGER_H

#include <QObject>

class ComparisonManager : public QObject
{
    Q_OBJECT
public:
    explicit ComparisonManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // COMPARISONMANAGER_H