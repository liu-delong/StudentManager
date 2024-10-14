QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backup.cpp \
    backupthread.cpp \
    classschedule.cpp \
    datetimemanager.cpp \
    dbmanager.cpp \
    dbmapper.cpp \
    leaveandmakeup.cpp \
    leaveandmakeuptabledelegate.cpp \
    main.cpp \
    mainwindow.cpp \
    modifystuinfo.cpp \
    mylistview.cpp \
    newstudent.cpp \
    report.cpp \
    signinmanager.cpp \
    signinrecord.cpp \
    signinrecordtabledelegate.cpp \
    signintabledelegate.cpp \
    sqlworker.cpp \
    stuinfo.cpp \
    stuinfotabledelegate.cpp \
    temp.cpp \
    timeadjmanager.cpp \
    utils.cpp

HEADERS += \
    backup.h \
    backupthread.h \
    classschedule.h \
    datetimemanager.h \
    dbmanager.h \
    dbmapper.h \
    leaveandmakeup.h \
    leaveandmakeuptabledelegate.h \
    mainwindow.h \
    modifystuinfo.h \
    mylistview.h \
    newstudent.h \
    report.h \
    signinmanager.h \
    signinrecord.h \
    signinrecordtabledelegate.h \
    signintabledelegate.h \
    sqlworker.h \
    stuinfo.h \
    stuinfotabledelegate.h \
    temp.h \
    timeadjmanager.h \
    utils.h

FORMS += \
    backup.ui \
    classschedule.ui \
    leaveandmakeup.ui \
    mainwindow.ui \
    modifystuinfo.ui \
    newstudent.ui \
    report.ui \
    signinmanager.ui \
    signinrecord.ui \
    stuinfo.ui \
    temp.ui \
    timeadjmanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
