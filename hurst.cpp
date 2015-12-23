/*****************************************************
 * Расчет показателя Хёрста проводился на основе R/S
 * анализа. Наименования функций и переменных соответствуют
 * http://www.math.spbu.ru/user/gran/soi5/Gatchkov5.pdf
 * и частично http://habrahabr.ru/post/256381/ (для
 * упрощения чтения моего кода).
 * На вход подается временной ряд, далее из конструктора
 * расчеты происходят автоматически. По окончании расчетов
 * генерится сигнал hurstReady(double hurst) в котором и
 * содержиться результирующее значение.
 *******************************************************/

#include "hurst.h"

Hurst::Hurst(QList<double> *timeSeries, QObject *parent) : QObject(parent)
{
    M = timeSeries->size();             //Задаем размер исходного временного ряда
    n = 1.0;                            //начальный размер блока А

    NtCal(timeSeries);                     //Рассчитаем логарифмические отношения
}

Hurst::Hurst(QObject *parent): QObject(parent)
{
    M=0.0;
    n=0.0;
}

void Hurst::calculateHurst(QList<double> timeSeries){
    M = timeSeries.size();             //Задаем размер исходного временного ряда
    n = 1.0;                            //начальный размер блока А
    if(Nt.size() > 0) Nt.clear();
    if(nList.size() > 0) nList.clear();
    if(TS.size() > 0) TS.clear();
    if(RSnList.size() > 0) RSnList.clear();
    QList<double> *tSeries = &timeSeries;
    NtCal(tSeries);
    calculate();
}

void Hurst::calculate()
{
    QList<double> RSn;
    while(genNEXTn()){                  //Разделим ряд Nt на A смежных периодов длиной n
        QList<double> Ea = EaCall();        //Определим для каждого периода среднее значение
        QList<double> Xka = XkaCall(&Ea);   //Рассчитаем отклонения от среднего значения для каждого периода
        QList<double> Ria = RiaCall(&Xka);  //Рассчитаем размах в пределах каждого периода
        QList<double> Si = SiCall(&Ea);     //Рассчитаем стандартное отклонения для каждого периода
        double lnRS = qLn(RSnCall(&Ria,&Si));
        if(!qIsNaN(lnRS) && !qIsInf(lnRS)){
            RSn.append(lnRS);      //Рассчитываем среднее значение R/S
            nList.append(qLn(n));
        }
    }
    RSnList = RSn;
    double H=TSQR(&RSn);
    emit hurstReady(H);
}

QList<double> Hurst::getRSn()
{
    return RSnList;
}

QList<double> Hurst::getnList()
{
    return nList;
}

void Hurst::NtCal(QList<double> *_TS)
{
    //TS->clear();
    for (int t = 1; t < _TS->size(); ++t) {
        double N = 0.0;
        N = qLn(_TS->at(t)/_TS->at(t-1));
        TS<<N;
    }

}

bool Hurst::genNEXTn(){
    for (double i = n+1.0; i < (M/2); i=i+1.0){

        if(fmod((M-1.0), i) == 0.0){
            n=i;

            A=(M-1.0)/i;
            return true;
        }
    }
    return false;
}

QList<double> Hurst::EaCall(){
    QList<double> _Ea;
    int ka=0;
    for (int countN=0; countN < A; countN++){
        double Eai = 0.0;
        for(int k=0; k<n; k++){
            Eai += TS.at(ka);
            ka++;
        }
        Eai = Eai/n;
        _Ea.append(Eai);
    }
    return _Ea;
}

QList<double> Hurst::XkaCall(QList<double> *EA)
{
    QList<double> _Xka;
    int ka=0;
    for (int countN=0; countN < A; countN++){
        for(int k=0; k<n; k++){
            double Xka = 0.0;
            for(int i=0;i<=k;i++){
                Xka += (TS.at(ka+i) - EA->at(countN));
            }
            _Xka.append(Xka);
        }
        ka+=n;
    }
    return _Xka;
}

QList<double> Hurst::SiCall(QList<double> *EA)
{
    QList<double> _Si;
    int ka = 0;
    for (int countN=0; countN < A; countN++){
        double Si = 0.0;
        for(int k=0; k<n; k++){
            Si += pow(TS.at(ka) - EA->at(countN),2);
            ka++;
        }
        Si = sqrt(Si/n);
        _Si.append(Si);
    }
    return _Si;
}

QList<double> Hurst::RiaCall(QList<double> *Xka)
{
    QList<double> _Ria;
    int ka = 0;
    for (int countN=0; countN < A; countN++){
        double Xmin = Xka->at(ka);
        double Xmax = Xka->at(ka);
        ka++;
        for(int k=1; k<n; k++){
            if(Xmin > Xka->at(ka)){
                Xmin=Xka->at(ka);
            }
            if(Xmax < Xka->at(ka)){
                Xmax=Xka->at(ka);
            }
            ka++;
        }
        double Ria = Xmax - Xmin;
        _Ria.append(Ria);
    }
    return _Ria;
}

double Hurst::RSnCall(QList<double> * Ri, QList<double> * Si){
    double _RSn = 0.0;
    int c=0;
    for (int countN = 0; countN < A; countN++){
        if(Si->at(countN) == 0.0){
            c++;
            continue;
        }else{
            _RSn += Ri->at(countN)/Si->at(countN);
        }
    }
    _RSn = _RSn/A-c;
    return _RSn;
}

double Hurst::TSQR(QList<double> *RSn)
{
    double summY = 0.0;
    double summX = 0.0;
    double summXY = 0.0;
    double summX2 = 0.0;
    double summY2 = 0.0;
    QList<double> XY;
    QList<double> X2;
    QList<double> Y2;
    double count = RSn->size();
    for(int i=0; i < count;i++){
        double x = nList.at(i);
        double y = RSn->at(i);
        XY.append(x * y);
        X2.append(x * x);
        Y2.append(y * y);
    }
    for(int i=0; i < count;i++){
        summY += RSn->at(i);
        summX += nList.at(i);
        summXY += XY.at(i);
        summX2 += X2.at(i);
        summY2 += Y2.at(i);

    }
    //double Xe = summX/count;
    //double Ye = summY/count;
    double H = ((count * summXY) - (summY * summX))/((count * summX2) - (summX * summX));
    return H;
}

