#include "readthread.h"

ReadThread::ReadThread(QThread *parent) : QThread(parent)
{

}

ReadThread::ReadThread(QString filePath,QThread *parent) : QThread(parent)
{
    CSVFilePath = filePath;
}

void ReadThread::run()
{
    CsvReader *csv = new CsvReader(0,CSVFilePath); //
    if (csv->Open()){
        QList<QStringList> str = csv->CSVRead();
        emit finishReadCSV(str);
    }
}

void ReadThread::setFileName(QString filePath)
{
    CSVFilePath = filePath;
}
