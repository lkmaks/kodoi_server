QT += core gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        AbstractBoard.cpp \
        Board.cpp \
        ClientSession.cpp \
        Club.cpp \
        LoginSystem.cpp \
        Message.cpp \
        Room.cpp \
        TCPServer.cpp \
        helpers.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    AbstractBoard.h \
    Board.h \
    ClientSession.h \
    Club.h \
    LoginSystem.h \
    Message.h \
    Room.h \
    TCPServer.h \
    helpers.h \
    serialization.h \
    types.h


///
