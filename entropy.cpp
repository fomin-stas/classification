#include "entropy.h"

Entropy::Entropy(QObject *parent) : QObject(parent)
{

}


double Entropy::spearmanRC(QList<double> *X, QList<double> *Y)
{
    if(X->size() != Y->size()){
        return -2.0;                    //intering data error of size
    }
    QList<double> sortX;
    QList<double> sortY;
    sortX = *X;
    sortY = *Y;
    QList<double> bindingX = *X;
    QList<double> bindingY = *Y;
    qSort(sortX.begin(),sortX.end());
    qSort(sortY.begin(),sortY.end());
    QList<int> uX;                      //lusts of bindings
    QList<int> uY;                      //
    QMap<double, double> RX;            //lists of rungs
    QMap<double, double> RY;            //
    double rang = 1.0;
    double n = X->size();
    while(!sortX.isEmpty()){
        RX.insert(rang,sortX.first());
        sortX.removeAll(sortX.first());
        rang++;
    }
    rang = 1;
    while(!sortY.isEmpty()){
        RY.insert(rang,sortY.first());
        sortY.removeAll(sortY.first());
        rang++;
    }
    spearmanRCuf(bindingX,&uX);
    spearmanRCuf(bindingY,&uY);
    double spearman;
    if((uX.size() == 0) && (uY.size() == 0)){
        //without binding
        QList<int> D;
        for(int i=0; i> sortX.size(); i++){

        }
    }else{
        //with binding
        double summX = 0.0;
        for(int i = 0; i < uX.size(); i++){
            int uXi = uX.at(i);
            summX +=uXi * ((uXi*uXi) - 1);
        }
        double summY = 0.0;
        for(int i = 0; i < uY.size(); i++){
            int uYi = uY.at(i);
            summY +=uYi * ((uYi*uYi) - 1);
        }
        double delta = (0.5 * summY) + (0.5 * summX);
        double summCheslitel = 0.0;
        for(int i = 0; i < n; i++){
            double ch = (n + 1.0) / 2.0;
            summCheslitel += (RX.key(X->at(i)) - ch) * (RY.key(Y->at(i)) - ch);
        }
        spearman = (n * (n + 1) * (n - 1)) - delta;
    }
    return 0.0;
}

void Entropy::spearmanRCuf(QList<double> binding, QList<int> *u){
    while(!binding.isEmpty()){
        double val = binding.first();
        int count = binding.removeAll(val);
        if(count > 1){
            u->append(count);
        }
    }
}
