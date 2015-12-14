#ifndef TICKDATA_H
#define TICKDATA_H

#include <QObject>

class TickData : public QObject
{
    Q_OBJECT
public:
    explicit TickData(QObject *parent = 0);

signals:

public slots:
};

#endif // TICKDATA_H
