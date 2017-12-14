QT     += core gui charts sql widgets
CONFIG += c++11

win32:RC_ICONS += samara.ico
TEMPLATE = app
NAME = samara


SAMARA_GIT_COMMIT = $$system(git log -n 1 --pretty="%h")
SAMARA_GIT_VERSION = $$system(git describe --abbrev=0 --tags)
RSAMARA_GIT_COMMIT = $$system(cd ..\\rSamara && git log -n 1 --pretty="%h")
RSAMARA_GIT_VERSION = $$system(cd ..\\rSamara && git describe --abbrev=0 --tags)
message(SAMARA_GIT_COMMIT: $$SAMARA_GIT_COMMIT)
message(SAMARA_GIT_VERSION: $$SAMARA_GIT_VERSION)
message(RSAMARA_GIT_COMMIT: $$RSAMARA_GIT_COMMIT)
message(RSAMARA_GIT_VERSION: $$RSAMARA_GIT_VERSION)

QMAKE_TARGET = SAMARA
QMAKE_TARGET_COMPANY = "CIRAD - UMR AGAP - PAM TEAM"
QMAKE_TARGET_DESCRIPTION = UI:$$SAMARA_GIT_VERSION-$$SAMARA_GIT_COMMIT Core:$$RSAMARA_GIT_VERSION-$$RSAMARA_GIT_COMMIT Qt:$$QT_VERSION
QMAKE_TARGET_COPYRIGHT = "Copyright 2017 by CIRAD"
QMAKE_TARGET_PRODUCT = "Samara simulation engine"
VERSION = $$RSAMARA_GIT_VERSION
#SHORT_VERSION = $$split(VERSION, ".")
#VERSION_PE_HEADER = $$member(SHORT_VERSION, 0).$$member(SHORT_VERSION, 1)

CONFIG(debug, debug|release) {
    TARGET = $${NAME}d
} else {
    TARGET = $${NAME}
}

win32 {
    *-msvc* {
        COMPILER = msvc14
        LIBS += -lSecur32 -lWs2_32 -lShell32 -lAdvapi32
    }
}

INCLUDEPATH +=  ../../ext_libs/include \
                ./src/ \
                ../rsamara/src

HEADERS += \
    src/qtapp/callout.h \
    src/qtapp/DBEcosysloader.h \
    src/qtapp/loaderview.h \
    src/qtapp/mainwindow.h \
    src/qtapp/meteodatamodel.h \
    src/qtapp/parametersdatamodel.h \
    src/qtapp/resultsdatamodel.h \
    src/qtapp/resultview.h \
    ../rSamara/src/utils/juliancalculator.h \
    ../rSamara/src/utils/resultparser.h \
    ../rSamara/src/utils/simulationloader.h \
    ../rSamara/src/parameters.h \
    ../rSamara/src/samara.h \
    ../rSamara/src/samara_defines.h \
    ../rSamara/src/variables.h \
    src/qtapp/comparisondatamodel.h \
    ../rSamara/src/processes/processes.h \
    src/qtapp/chartmanager.h \
    src/qtapp/resultsmanager.h \
    src/qtapp/comparisonmanager.h \
    src/qtapp/DBAccessLoader.h \
    src/qtapp/chartview.h

SOURCES += \
    src/qtapp/callout.cpp \
    src/qtapp/loaderview.cpp \
    src/qtapp/mainwindow.cpp \
    src/qtapp/meteodatamodel.cpp \
    src/qtapp/parametersdatamodel.cpp \
    src/qtapp/resultsdatamodel.cpp \
    src/qtapp/resultview.cpp \
    src/main.cpp \
    ../rSamara/src/samara.cpp \
    src/qtapp/comparisondatamodel.cpp \
    src/qtapp/chartmanager.cpp \
    src/qtapp/resultsmanager.cpp \
    src/qtapp/comparisonmanager.cpp \
    src/qtapp/chartview.cpp

FORMS += \
    src/qtapp/mainwindow.ui

RESOURCES += \
    resources.qrc
