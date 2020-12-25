QT += quick widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chartmodel.cpp \
    datamanager.cpp \
    formcontroller.cpp \
    formlist.cpp \
    main.cpp \
    messagedata.cpp \
    network/roommanager.cpp \
    questionformlist.cpp \
    questionformmodel.cpp \
    network/requester.cpp \
    receiveeventmessage.cpp \
    sendeventmessage.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    network/requester.pri

HEADERS += \
    chartmodel.h \
    datamanager.h \
    formcontroller.h \
    formlist.h \
    messagedata.h \
    network/roommanager.h \
    questionformlist.h \
    questionformmodel.h \
    network/requester.h \
    receiveeventmessage.h \
    sendeventmessage.h \
    structures.h
