#ifndef ENTROPY_H
#define ENTROPY_H

#include <QObject>
#include <QMap>

class Entropy : public QObject
{
    Q_OBJECT

public:
    explicit Entropy(QObject *parent = 0);

signals:

public slots:

private:
    double spearmanRC(QList<double> *X, QList<double> *Y);
    void spearmanRCuf(QList<double> binding, QList<int> *u);
};

#endif // ENTROPY_H
