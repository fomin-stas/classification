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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"
#include "renderarea.h"

#ifdef wint_t
#include <c:\qwt-6.1.2\src\qwt_plot.h>
#include <c:\qwt-6.1.2\src\qwt_plot_grid.h>
#include <c:\qwt-6.1.2\src\qwt_legend.h>
#include <c:\qwt-6.1.2\src\qwt_plot_curve.h>
#include <c:\qwt-6.1.2\src\qwt_symbol.h>
#include <c:\qwt-6.1.2\src\qwt_plot_magnifier.h>
#include <c:\qwt-6.1.2\src\qwt_plot_panner.h>
#include <c:\qwt-6.1.2\src\qwt_plot_picker.h>
#include <c:\qwt-6.1.2\src\qwt_picker_machine.h>
#else
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#endif
QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QLabel;
class QSlider;
class QSpinBox;
class QComboBox;
class QCheckBox;

QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    int getTopicColumn() const;
    void setTopicColumn(QMap<int, QString> *value);

public slots:
    void newHurst(double data);
    void newHurst2(double data);
private slots:
    void newLetter();
    void save();
    void print();
    void undo();
    void about();
    void addHeaderData();
    void runH();
    void dynamicH();
    void dataUpdate(double);
    void dataHurstUpdate(double);
    void newData(double);


    void stopDynamicHurstTriggered();
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void createPoligon(QPolygonF points, QString title);
    void createCentralPlot();
    void createDockPlot();
    void createDockPlot2();
    Controller *control;
    QwtPlot *d_plot;
    QwtPlot *h_plot;
    QwtPlot *h_plot2;

    QwtPlotCurve *hurstCurve;
    QwtPlotCurve *dataCurve;
    QwtPlotCurve *hurstCurve2;

    QTextEdit *textEdit;
    RenderArea *renderArea;
    QListWidget *customerList;
    QListWidget *paragraphsList;
    QLabel *lableHurst;
    QSpinBox *hurstWindow;
    QSpinBox *hurstPeriod;
    QComboBox *topicColumn;
    QCheckBox *withTopic;



    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newLetterAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *aboutAct;
    QAction *quitAct;
    QAction *runHurst;
    QAction *dynamicHurst;
    QAction *stopDynamicHurst;

    QPolygonF dataPoints;
    QPolygonF hurstPoints;
    QPolygonF hurstPoints2;

    double timer;
    int topic;
    QColor *curvColor;

};
//! [0]

#endif
