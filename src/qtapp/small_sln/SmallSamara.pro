TEMPLATE = app
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle qt
#QMAKE_CXXFLAGS += -P -E  ##msvc
#QMAKE_CXXFLAGS_RELEASE -= -O0
#QMAKE_CXXFLAGS_RELEASE -= -O
#QMAKE_LFLAGS_RELEASE -= -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
#QMAKE_CXXFLAGS_RELEASE += -Ox
QMAKE_CXXFLAGS_RELEASE += -Ofast

#DEFINES += WITH_TRACE

#CONFIG += BUILD_FOR_APP32
#CONFIG += BUILD_FOR_APP64
#CONFIG += BUILD_FOR_LIB32
CONFIG += BUILD_FOR_R32
#CONFIG += BUILD_FOR_R64
#CONFIG += BUILD_FOR_MGW32
#CONFIG += BUILD_FOR_MGW64

BUILD_FOR_APP32 {
    TEMPLATE = app
    CONFIG += shared
    ARCHI = x86
    COMPILER = msvc14
    LIBS += -lSecur32 -lWs2_32 -lShell32 -lAdvapi32
    LIBS += -LD:/PAMStudio/dev/ext_libs/$$COMPILER/$$ARCHI/static -llibpq
}

BUILD_FOR_APP64 {
    TEMPLATE = app
    CONFIG += shared
    ARCHI = x64
    COMPILER = msvc14
    LIBS += -lSecur32 -lWs2_32 -lShell32 -lAdvapi32
    LIBS += -LD:/PAMStudio/dev/ext_libs/$$COMPILER/$$ARCHI/static -llibpq
}

BUILD_FOR_LIB32 {
    TEMPLATE = lib
    CONFIG += static
    ARCHI = x86
    COMPILER = msvc14
    LIBS += -lSecur32 -lWs2_32 -lShell32 -lAdvapi32
}

BUILD_FOR_R32 {
    TEMPLATE = app
    CONFIG += static
    ARCHI = x86
    ARCHI_R = i386
    COMPILER = mingw-4.9.3
    LIBS += -LD:/PAMStudio/dev/ext_libs/$$COMPILER/$$ARCHI/shared -llibpq
}

BUILD_FOR_R64 {
    TEMPLATE = app
    CONFIG += static
    ARCHI = x64
    ARCHI_R = x64
    COMPILER = mingw-4.9.3
    LIBS += -LD:/PAMStudio/dev/ext_libs/$$COMPILER/$$ARCHI/shared -llibpq
}

BUILD_FOR_MGW32 {
    TEMPLATE = app
    CONFIG += static
    ARCHI = x86
    COMPILER = mingw_6.3.0
    LIBS += -LD:/PAMStudio/dev/ext_libs/$$COMPILER/$$ARCHI/shared -llibpq
}

BUILD_FOR_MGW64 {
    TEMPLATE = app
    CONFIG += static
    ARCHI = x64
    COMPILER = mingw_6.3.0
    LIBS += -LD:/PAMStudio/dev/ext_libs/$$COMPILER/$$ARCHI/shared -llibpq
}


### R CONFIG ###
R_PATH = C:/R
RCPP_PATH = $$R_PATH/library/Rcpp
INCLUDEPATH += $$RCPP_PATH/include $$R_PATH/include
LIBS += -L$$RCPP_PATH/libs/$$ARCHI_R -lRcpp
######


INCLUDEPATH +=  D:/PAMStudio/dev/ext_libs/include \
                D:/PAMStudio/dev/git/small_sln/src \

HEADERS += \
    src/parameters.h \
    src/samara.h \
    src/variables.h \
    src/processes/BhyTypeFAO.h \
    src/processes/Bileau.h \
    src/processes/MilBilanCarbone.h \
    src/processes/Risocas2cpp.h \
    src/processes/Riz.h \
    src/processes/sorghum.h \
    src/utils/DateTime.h \
    src/utils/psqlloader.h \
    src/samara_defines.h \
    src/r/rsamara_types.hpp \
    src/utils/fileloader.h

SOURCES += \
    src/main.cpp \
    src/samara.cpp \
    src/utils/DateTime.cpp \
    src/r/rcpp_samara.cpp


