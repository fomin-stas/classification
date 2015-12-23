QT += widgets
CONFIG += qwt
qtHaveModule(printsupport): QT += printsupport

linux-g++: INCLUDEPATH += /usr/include/qwt
linux-g++: LIBS += -L/usr/lib -lqwt-qt5

win32: include ( c:/qwt-6.1.2/qwt.prf )





RC_FILE = classification.rc

HEADERS         = mainwindow.h \
    controller.h \
    csvreader.h \
    hurst.h \
    readdata.h \
    readthread.h \
    tickdata.h \
    renderarea.h \
    mathoperations.h

SOURCES         = main.cpp \
    mainwindow.cpp \
    controller.cpp \
    csvreader.cpp \
    hurst.cpp \
    readdata.cpp \
    readthread.cpp \
    tickdata.cpp \
    renderarea.cpp \
    mathoperations.cpp

RESOURCES       = classification.qrc

# install
target.path = ./build

INSTALLS += target
