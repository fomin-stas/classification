#include "controller.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    setHurstPeriod(100);
    setHurstWindow(1000);
}

QString Controller::fileName(){
    return fileName_csv;
}

QStringList Controller::headerData()
{
    return _headerData;
}

void Controller::setFileName(QString fileName){
    fileName_csv = fileName;
    emit fileNameChanged();
    RD = new ReadData(fileName_csv);
    connect(RD,SIGNAL(dataReady()),this,SLOT(setHeader()));

}

void Controller::setHeader(QStringList header_csv)
{
    _headerData = header_csv;
}

void Controller::calculateHurst(int column)
{
    QList<double> columnData =  RD->getColumnData(column);
    hurst = new Hurst(&columnData);
    connect(hurst,SIGNAL(hurstReady(double)),this,SIGNAL(hurstReady(double)));
    hurst->calculate();
}

void Controller::dynamicHurst(int column)
{
    dynamicData =  RD->getColumnData(column);
    topicStart = 0;
    countRow = 0;
    currentPeriod = hurstPeriod;
    dHurst = new Hurst();
    connect(dHurst,SIGNAL(hurstReady(double)),this,SIGNAL(hurstAdd(double)));

    if(withTopic){
        topicData =  RD->getColumnData(topicColumn);

        topic = topicData.at(0);
        dHurst2 = new Hurst();
        connect(dHurst2,SIGNAL(hurstReady(double)),this,SIGNAL(hurstAdd2(double)));
    }
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateHurst()));
    timer->start(10);
}

void Controller::stopDynamicHurst(){
    timer->stop();
}

void Controller::updateHurst(){

    if(countRow < dynamicData.size()){
        if(countRow > hurstWindow) {
            if(currentPeriod == hurstPeriod){
                QList<double> hl = dynamicData.mid(countRow - hurstWindow,countRow);
                dHurst->calculateHurst(hl);
                currentPeriod = 0;
            }
            currentPeriod++;
        }
    }else{
        timer->stop();
        return;
    }

    if(withTopic){
        if(topic != topicData.at(countRow)){
            QList<double> hl = dynamicData.mid(topicStart,countRow - topicStart);
            dHurst2->calculateHurst(hl);
            topicStart = countRow;
            topic = topicData.at(countRow);
        }
    }

    emit newData(dynamicData.at(countRow));
    countRow++;
}

void Controller::setHurstWindow(int HW){
    hurstWindow = HW;
}

void Controller::setHeader()
{
    _headerData = RD->getHeader();
    emit dataReady();
}

QList<double> Controller::getRSn()
{
    return hurst->getRSn();
}

QList<double> Controller::getnList()
{
    return hurst->getnList();
}


int Controller::getTopicColumn() const
{
    return topicColumn;
}

void Controller::setTopicColumn(int value)
{
    topicColumn = value;
}

bool Controller::getWithTopic() const
{
    return withTopic;
}

void Controller::setWithTopic(bool value)
{
    withTopic = value;
}

int Controller::getHurstWindow() const
{
    return hurstWindow;
}

int Controller::getHurstPeriod() const
{
    return hurstPeriod;
}

void Controller::setHurstPeriod(int value)
{
    hurstPeriod = value;
}
