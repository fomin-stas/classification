#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "readdata.h"
#include "hurst.h"
#include <QStringList>
#include <QTimer>

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = 0);
    QString fileName();
    QStringList headerData();


    int getHurstPeriod() const;
    void setHurstPeriod(int value);

    int getHurstWindow() const;

    bool getWithTopic() const;
    void setWithTopic(bool value);

    int getTopicColumn() const;
    void setTopicColumn(int value);

signals:
    void fileNameChanged();
    void headerChange();
    void dataReady();
    void hurstReady(double);
    void hurstAdd(double);
    void hurstAdd2(double);
    void newData(double);


public slots:
    void setFileName(QString fileName);
    void setHeader(QStringList header_csv);
    void calculateHurst(int column);
    void setHeader();
    QList<double> getRSn();
    QList<double> getnList();
    void dynamicHurst(int column);
    void updateHurst();
    void setHurstWindow(int hurstWindow);

private:
    QString fileName_csv;
    ReadData *RD;
    QStringList _headerData;
    Hurst * hurst;
    Hurst * dHurst;
    Hurst * dHurst2;
    QList<double> dynamicData;
    QList<double> topicData;
    double topic;
    int topicStart;
    int countRow;
    QTimer *timer;
    int hurstWindow;
    int hurstPeriod;
    int currentPeriod;
    bool withTopic;
    int topicColumn;

};

#endif // CONTROLLER_H
