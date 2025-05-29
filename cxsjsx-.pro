QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
RC_ICON = favicon.ico
SOURCES += \
    CQlabel.cpp \
    EditDialog.cpp \
    LoginDialog.cpp \
    MainWindow.cpp \
    RegisterDialog.cpp \
    chatmessage.cpp \
    chatroom.cpp \
    global_itempost.cpp \
    globalfunc.cpp \
    globalusers.cpp \
    itempost.cpp \
    main.cpp \
    user.cpp

HEADERS += \
    CQlabel.h \
    EditDialog.h \
    LoginDialog.h \
    MainWindow.h \
    RegisterDialog.h \
    chatmessage.h \
    chatroom.h \
    global_itempost.h \
    globalusers.h \
    itempost.h \
    user.h

FORMS += \
    MainWindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    picture.qrc
