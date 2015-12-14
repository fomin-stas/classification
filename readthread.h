#ifndef READTHREAD_H
#define READTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QList>
#include <QStringList>
#include "csvreader.h"

class ReadThread : public QThread
{
    Q_OBJECT
public:
    ReadThread(QThread *parent = 0);
    void run();
    void setFileName(QString filePath);

    ReadThread(QString filePath, QThread *parent = 0);
private:
    QString CSVFilePath;

signals:
    void finishReadCSV(QList<QStringList>);

//private slots:
//    void slotNewTickGeneration();
};

#endif // READTHREAD_H
