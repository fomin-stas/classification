/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//! [0]
#include <QtWidgets>
#ifndef QT_NO_PRINTDIALOG
#include <QtPrintSupport>
#endif

#include "mainwindow.h"
//! [0]

//! [1]
MainWindow::MainWindow()
{
    createCentralPlot();
    this->setWindowIcon(QIcon(":/images/icon.ico"));
    setCentralWidget(d_plot);
    control = new Controller();
    connect(control,SIGNAL(dataReady()),this,SLOT(addHeaderData()));
    connect(control,SIGNAL(hurstReady(double)),this,SLOT(dataUpdate(double)));
    connect(control,SIGNAL(hurstAdd(double)),this,SLOT(newHurst(double)));
    connect(control,SIGNAL(hurstAdd2(double)),this,SLOT(newHurst2(double)));
    connect(control,SIGNAL(newData(double)),this,SLOT(newData(double)));
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();
    setWindowTitle(tr("Classification by Hurst"));
}

void MainWindow::newLetter()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open CSV data file"), QDir::currentPath(), tr("Image Files (*.csv)"));
    QProgressDialog progress("Data loading...", "Abort loading",0,0, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    connect(control,SIGNAL(dataReady()),&progress,SLOT(close()));
    control->setFileName(fileName);
}

void MainWindow::print()
{
#ifndef QT_NO_PRINTDIALOG
    QTextDocument *document = textEdit->document();
    QPrinter printer;

    QPrintDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    document->print(&printer);
    statusBar()->showMessage(tr("Ready"), 2000);
#endif
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Choose a file name"), ".",
                                                    tr("HTML (*.html *.htm)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Classification"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toHtml();
    QApplication::restoreOverrideCursor();
    statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);
}

void MainWindow::undo()
{

}


void MainWindow::addHeaderData()
{
    QStringList headerData = control->headerData();
    for(int i = 0; i< headerData.size(); i++){
        QString str = headerData.at(i);
        headerData.replace(i,str.toLocal8Bit());
    }
    paragraphsList->addItems(headerData);
    topicColumn->addItems(headerData);
    paragraphsList->setCurrentRow(0);
    dynamicHurst->setEnabled(true);
    runHurst->setEnabled(true);
}

void MainWindow::runH()
{
    QList<QListWidgetItem*> columns = paragraphsList->selectedItems();
    QList<int> rows;
    for(int i=0; i< columns.size(); i++){
        rows.append(paragraphsList->row(columns.at(i)));
    }
    control->calculateHurst(rows);
}

void MainWindow::dynamicH()
{
    dynamicHurst->setEnabled(false);
    timer = 0.0;
    topic = 0;
    dataPoints.clear();
    hurstPoints.clear();
    hurstPoints2.clear();
    hurstCurve = new QwtPlotCurve();

    int c = 255 / (paragraphsList->count()+1);
    int del = paragraphsList->currentRow()%2 == 0?2:1;
    int del2 = paragraphsList->currentRow()%3 == 0?2:1;
    int del3 = paragraphsList->currentRow()%4 == 0?2:1;
    int r = (c * paragraphsList->currentRow())/(del * del2) ;
    int g = (255 - (c * paragraphsList->currentRow()))/del3;
    int b = qAbs((c * paragraphsList->currentRow() * 2) - 255);
    curvColor = new QColor(r,g,b);
    hurstCurve->setPen( *curvColor, 0 ); // цвет и толщина кривой
    hurstCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // сглаживание
    hurstCurve->attach( h_plot ); // отобразить кривую на графике

    hurstCurve2 = new QwtPlotCurve();
    hurstCurve2->setPen( *curvColor , 0 ); // цвет и толщина кривой
    hurstCurve2->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // сглаживание
    hurstCurve2->attach( h_plot2 ); // отобразить кривую на графике

    dataCurve = new QwtPlotCurve();
    dataCurve->setTitle( paragraphsList->currentItem()->text() );
    dataCurve->setPen( *curvColor, 0 ); // цвет и толщина кривой
    dataCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // сглаживание

    dataCurve->attach( d_plot ); // отобразить кривую на графике

    int column = paragraphsList->currentRow();
    control->setWithTopic(withTopic->isChecked());
    if(withTopic->isChecked()){
        control->setTopicColumn(topicColumn->currentIndex());
    }
    control->setHurstPeriod(hurstPeriod->text().toInt());
    control->setHurstWindow(hurstWindow->text().toInt());
    control->dynamicHurst(column);
    stopDynamicHurst->setEnabled(true);

}

void MainWindow::stopDynamicHurstTriggered(){
    stopDynamicHurst->setEnabled(false);
    dynamicHurst->setEnabled(true);

}

void MainWindow::dataUpdate(double h)
{
    QString strH = QString::number(h);
    lableHurst->setText("H=" + strH);
    QPolygonF points;
    QList<double> RSn = control->getRSn();
    QList<double> nList = control->getnList();
    for(int i=0; i<RSn.size();i++){
        points << QPointF(nList.at(i), RSn.at(i));
    }
    //createPoligon(points, "ln(R/S) от (n)");
}

void MainWindow::dataHurstUpdate(double)
{

}

void MainWindow::newData(double data)
{
    timer = timer + 0.1;
    dataPoints.append(QPointF(timer,data));
    dataCurve->setSamples(dataPoints);
    //dataCurve->attach( d_plot );
    d_plot->replot();
}

void MainWindow::newHurst(double data)
{
    if(isnan(data) || isinf(data)) return;
    hurstPoints.append(QPointF(timer,data));
    hurstCurve->setSamples(hurstPoints);

    h_plot->replot();
}

void MainWindow::newHurst2(double data)
{
    if(isnan(data) || isinf(data)) return;
    hurstPoints2.append(QPointF(topic++,data));
    hurstCurve2->setSamples(hurstPoints2);

    h_plot2->replot();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Classification"),
                       tr("The <b>Classification</b> example demonstrates how to "
                          "use Hurst. "));
}

void MainWindow::createActions()
{
    newLetterAct = new QAction(QIcon(":/images/new.png"), tr("&New data"), this);
    newLetterAct->setShortcuts(QKeySequence::New);
    newLetterAct->setStatusTip(tr("Create a new form letter"));
    connect(newLetterAct, SIGNAL(triggered()), this, SLOT(newLetter()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current graph"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    printAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current graph"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    runHurst = new QAction(QIcon(":/images/calc.png"),tr("&Run Hurst"), this);
    runHurst->setStatusTip(tr("Run Hurst calculating."));
    runHurst->setEnabled(false);
    connect(runHurst, SIGNAL(triggered()), this, SLOT(runH()));

    dynamicHurst = new QAction(QIcon(":/images/play.png"),tr("Run &dinamic Hurst"), this);
    dynamicHurst->setStatusTip(tr("Run dynamic Hurst."));
    dynamicHurst->setEnabled(false);
    //dynamicHurst->setCheckable(true);
    connect(dynamicHurst, SIGNAL(triggered()), this, SLOT(dynamicH()));

    stopDynamicHurst = new QAction(QIcon(":/images/stopbutton.png"),tr("&Stop dinamic Hurst"), this);
    stopDynamicHurst->setStatusTip(tr("Run dynamic Hurst."));
    //stopDynamicHurst->setCheckable(true);
    stopDynamicHurst->setEnabled(false);
    connect(stopDynamicHurst, SIGNAL(triggered()), control, SLOT(stopDynamicHurst()));
    connect(stopDynamicHurst, SIGNAL(triggered()),this, SLOT(stopDynamicHurstTriggered()));
}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newLetterAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Calculating"));
    editMenu->addAction(runHurst);
    editMenu->addAction(dynamicHurst);
    editMenu->addAction(stopDynamicHurst);
    viewMenu = menuBar()->addMenu(tr("&View"));
    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newLetterAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Hurst"));
    editToolBar->setFloatable(true);
    editToolBar->addAction(dynamicHurst);
    editToolBar->addAction(stopDynamicHurst);
    editToolBar->addAction(runHurst);

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Data ready"));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Hurst"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QVBoxLayout *gl = new QVBoxLayout();
    QFormLayout *fl = new QFormLayout();
    QWidget * dWidget = new QWidget();
    withTopic = new QCheckBox();
    withTopic->setChecked(false);
    topicColumn = new QComboBox();
    topicColumn->setEnabled(false);
    connect(withTopic,SIGNAL(toggled(bool)),topicColumn,SLOT(setEnabled(bool)));
    dWidget->setMinimumHeight(100);
    lableHurst = new QLabel("Hurst=");
    hurstWindow = new QSpinBox();
    hurstWindow->setRange(50,15000);
    hurstPeriod = new QSpinBox();
    hurstPeriod->setRange(10,1000);

    gl->addWidget(lableHurst);
    fl->addRow("H window size:",hurstWindow);
    fl->addRow("H period:",hurstPeriod);
    fl->addRow("With topic", withTopic);
    fl->addRow("Topic column", topicColumn);

    gl->addLayout(fl);
    dWidget->setLayout(gl);
    dock->setWidget(dWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Header data"), this);
    paragraphsList = new QListWidget(dock);
    paragraphsList->setSelectionMode(QAbstractItemView::MultiSelection);
    dock->setWidget(paragraphsList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());

    dock = new QDockWidget(tr("Dynamic Hurst"), this);
    createDockPlot();
    QVBoxLayout *vl = new QVBoxLayout();

    QWidget * wt = new QWidget();
    vl->addWidget(h_plot);
    wt->setLayout(vl);
    dock->setWidget(wt);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Header data"), this);
    createDockPlot2();
    dock->setWidget(h_plot2);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
    viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::createPoligon(QPolygonF points, QString title)
{
    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setTitle( title );
    curve->setPen( Qt::blue, 0 );
    curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                       QBrush( Qt::yellow ), QPen( Qt::red, 0 ), QSize( 4, 4 ) );
    curve->setSymbol( symbol );
    curve->setSamples( points );
    curve->attach( d_plot );
    d_plot->replot();
}

void MainWindow::createCentralPlot()
{
    d_plot = new QwtPlot( this );
    d_plot->setTitle( "Curent data" ); // заголовок
    d_plot->setCanvasBackground( Qt::white ); // цвет фона
    d_plot->setAxisScale(QwtPlot::xBottom, 0, 20);
    d_plot->setAxisScale(QwtPlot::yLeft, 0, 10);
    d_plot->setAxisAutoScale(0,true);
    d_plot->setAxisAutoScale(1,true);
    d_plot->setAutoReplot(true);
    // Параметры осей координат
    d_plot->setAxisTitle(QwtPlot::yLeft, "Data");
    d_plot->setAxisTitle(QwtPlot::xBottom, "Time");
    d_plot->insertLegend( new QwtLegend() );
    // Включить сетку
    // #include <qwt_plot_grid.h>
    QwtPlotGrid *grid = new QwtPlotGrid(); //
    grid->setMajorPen( Qt::green, 0 , Qt::DashLine ); // цвет линий и толщина
    grid->attach( d_plot ); // добавить сетку к полю графика
    // Включить возможность приближения/удаления графика
    // #include <qwt_plot_magnifier.h>
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(d_plot->canvas());
    // клавиша, активирующая приближение/удаление
    magnifier->setMouseButton(Qt::MidButton);
    // Включить возможность перемещения по графику
    // #include <qwt_plot_panner.h>
    QwtPlotPanner *d_panner = new QwtPlotPanner( d_plot->canvas() );
    // клавиша, активирующая перемещение
    d_panner->setMouseButton( Qt::RightButton );
}

void MainWindow::createDockPlot()
{
    h_plot = new QwtPlot();
    h_plot->setTitle( "Dynamic Hurst" );
    h_plot->setCanvasBackground( Qt::white );
    h_plot->setAxisScale(QwtPlot::xBottom, 100, 200);
    h_plot->setAxisScale(QwtPlot::yLeft, 0, 1);
    h_plot->setAxisTitle(QwtPlot::yLeft, "Hurst");
    h_plot->setAxisTitle(QwtPlot::xBottom, "t");
    h_plot->insertLegend( new QwtLegend() );
    h_plot->setAxisAutoScale(0,true);
    h_plot->setAxisAutoScale(1,true);
    h_plot->setAutoReplot(true);
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen( Qt::gray, 0, Qt::DashLine );
    grid->attach( h_plot );
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(h_plot->canvas());
    magnifier->setMouseButton(Qt::MidButton);
    QwtPlotPanner *d_panner = new QwtPlotPanner( h_plot->canvas() );
    d_panner->setMouseButton( Qt::RightButton );
}

void MainWindow::createDockPlot2()
{
    h_plot2 = new QwtPlot();
    h_plot2->setTitle( "Hurst by topic" );
    h_plot2->setCanvasBackground( Qt::white );
    h_plot2->setAxisScale(QwtPlot::xBottom, 0, 25);
    h_plot2->setAxisScale(QwtPlot::yLeft, 0, 1);
    h_plot2->setAxisTitle(QwtPlot::yLeft, "Hurst");
    h_plot2->setAxisTitle(QwtPlot::xBottom, "Number of topic");
    h_plot2->insertLegend( new QwtLegend() );
    h_plot2->setAxisAutoScale(0,true);
    h_plot2->setAxisAutoScale(1,true);
    h_plot2->setAutoReplot(true);
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(Qt::gray, 0, Qt::DashLine);
    grid->attach( h_plot2 );
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(h_plot2->canvas());
    magnifier->setMouseButton(Qt::MidButton);
    QwtPlotPanner *d_panner = new QwtPlotPanner( h_plot2->canvas() );
    d_panner->setMouseButton(Qt::RightButton);
}

int MainWindow::getTopicColumn() const
{
    return topicColumn->currentData().toInt();
}

void MainWindow::setTopicColumn(QMap<int,QString> *value)
{
    QMap<int,QString>::const_iterator i = value->constBegin();
    while (i != value->constEnd()) {
        topicColumn->addItem(i.value(), i.key());
        i++;
    }
}
