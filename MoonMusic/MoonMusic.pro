QT       += core gui sql
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addplaylistpop.cpp \
    collectbutton.cpp \
    commonbutton.cpp \
    database.cpp \
    main.cpp \
    metadata.cpp \
    musicinfolistitem.cpp \
    mycheckbox.cpp \
    mymediaplayer.cpp \
    playercontrol.cpp \
    playlist.cpp \
    playmodebutton.cpp \
    selectplaylistbutton.cpp \
    volumebutton.cpp \
    volumewidget.cpp \
    warnpop.cpp \
    workdeletemetadata.cpp \
    workreadmetadata.cpp \
    worksavemetadata.cpp

HEADERS += \
    Connection.h \
    addplaylistpop.h \
    collectbutton.h \
    commonbutton.h \
    database.h \
    metadata.h \
    musicinfolistitem.h \
    mycheckbox.h \
    mymediaplayer.h \
    playercontrol.h \
    playlist.h \
    playmodebutton.h \
    selectplaylistbutton.h \
    volumebutton.h \
    volumewidget.h \
    warnpop.h \
    workdeletemetadata.h \
    workreadmetadata.h \
    worksavemetadata.h

FORMS += \
    addplaylistpop.ui \
    playercontrol.ui \
    playlist.ui \
    warnpop.ui

INCLUDEPATH += $$PWD/lib/ffmpeg/include
LIBS += -L$$PWD/lib/ffmpeg/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES +=
