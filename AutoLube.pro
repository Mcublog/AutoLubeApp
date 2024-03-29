#-------------------------------------------------
#
# Project created by QtCreator 2019-06-12T16:45:05
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoLube
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# bug workaround https://bugreports.qt.io/browse/QTBUG-76293
android {
    QMAKE_LFLAGS += -nostdlib++
}

win32-g++{
    message("Desktop")
    MY_DEF = "Desktop"
    DEFINES += "DESKTOP"
}

SOURCES += \
    src/bledevice.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/scanpage.cpp \
    src/workpage.cpp

HEADERS += \
    inc/bledevice.h \
    inc/mainwindow.h \
    inc/scanpage.h \
    inc/workpage.h

FORMS += \
    forms/mainwindow.ui \
    forms/scanpage.ui \
    forms/workpage.ui

CONFIG += mobility
MOBILITY = 

INCLUDEPATH += $$quote("$$PWD/inc")

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

