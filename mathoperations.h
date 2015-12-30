#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H

#include <QObject>
#include <QtAlgorithms>
#include <QMap>


class MathOperations : public QObject
{
    Q_OBJECT

public:
    explicit MathOperations(QObject *parent = 0);
    void linearNormalization(QList<double> *list);

signals:

public slots:

private:

};

#endif // MATHOPERATIONS_H
