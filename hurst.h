#ifndef HURST_H
#define HURST_H

#include <QObject>
#include <QtGlobal>
#include <QtMath>

class Hurst : public QObject
{
    Q_OBJECT
public:
    explicit Hurst(QList<double> *timeSeries, QObject *parent = 0);
    explicit Hurst(QObject *parent = 0);
    void calculate();

    void calculateHurst(QList<double> timeSeries);
signals:
    void hurstReady(double hurst);

public slots:
    QList<double> getRSn();
    QList<double> getnList();

private:
    void NtCal(QList<double> *_TS); //Рассчитаем логарифмические отношения
    bool genNEXTn(); //Сформируем очередное значение n, если его нет возвращает false
    QList<double> EaCall();
    QList<double> XkaCall(QList<double> *EA);
    QList<double> SiCall(QList<double> *EA);
    QList<double> RiaCall(QList<double> *Xka);
    double RSnCall(QList<double> *Ri, QList<double> *Si);
  //  double RSn();
    double TSQR(QList<double> *RSn);

    QList<double> Nt;
    double n; //current count of moments
    QList<double> nList; //список всех генерируемых n
    double A; //количество смежных периодов размерности n
    double M; //размер временного ряда
    QList<double> TS; //time series for hurst
    QList<double> RSnList;


};

#endif // HURST_H
