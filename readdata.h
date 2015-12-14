#ifndef READDATA_H
#define READDATA_H

#include <QObject>
#include <QFile>
#include "tickdata.h"
#include "readthread.h"
#include "csvreader.h"

class ReadData : public QObject
{
    Q_OBJECT
public:
    explicit ReadData(QString fileName, QObject *parent = 0);
    ReadData(QObject *parent=0);
    QString fileName();
    QList<double> getColumnData(int n);
    QStringList getRowData(int n);
    QStringList getHeader();
signals:
    void startRead();
    void stopRead();
    void finished();
    void errorRead(int);
    void dataReady();
    void newTick();

public slots:
    void setFileName(QString fileName);
    void startReadData();
private slots:
    void ticksGenerator(QList<QStringList> allData);
private:
    QString fileName_csv;
    QFile *file_csv;
    QList<QStringList> dataList;
};

#endif // READDATA_H
