#include "readdata.h"


ReadData::ReadData(QString fileName, QObject *parent) : QObject(parent)
{
    fileName_csv = fileName;
    startReadData();
}

ReadData::ReadData(QObject *parent) : QObject(parent)
{

}

QString ReadData::fileName()
{
    return fileName_csv;
}

QList<double> ReadData::getColumnData(int n)
{
    QList<double> _data;
    for(int i=1 ; i < dataList.size(); i++){
        QStringList SL = dataList.at(i);
        QString cellString = SL.at(n);
        cellString = cellString.replace(",",".");
        double cellData = cellString.toDouble();
        if (cellData == 0.0) continue;
        _data.append(cellData);
    }
    MathOperations MO;
   // MO.linearNormalization(&_data);
    return _data;
}

QStringList ReadData::getRowData(int n)
{
     return dataList.size()>n ? dataList.at(n) : QStringList();
}

QStringList ReadData::getHeader()
{
    return dataList.at(0);
}

void ReadData::setFileName(QString fileName){
    fileName_csv = fileName;
}

void ReadData::startReadData(){
    if(fileName_csv.isEmpty() || fileName_csv.isNull()){
        emit errorRead(-1);
        return;
    }
    ReadThread * RT = new ReadThread(fileName_csv);
    qRegisterMetaType<QList<QStringList> >("QList<QStringList>");
    connect(RT,SIGNAL(finishReadCSV(QList<QStringList>)),this,SLOT(ticksGenerator(QList<QStringList>)));
    RT->start();
}

void ReadData::ticksGenerator(QList<QStringList> allData)
{
    dataList = allData;
    emit dataReady();
}


