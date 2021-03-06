#include "mathoperations.h"

MathOperations::MathOperations(QObject *parent) : QObject(parent)
{

}

void MathOperations::linearNormalization(QList<double> *list)
{
    int listSize = list->size();
    double min = list->at(0);
    double  max = list->at(0);
    for(int i=1; i<listSize; i++){
        if (list->at(i) < min)
            min = list->at(i);
        if (list->at(i) > max)
            max = list->at(i);
    }
    for(int i=0; i<listSize; i++){
        double xNorm = (list->at(i) - min)/(max-min);
        list->replace(i,xNorm);
    }
}
