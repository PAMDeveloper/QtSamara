TEMPLATE = app
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
#QMAKE_CXXFLAGS_RELEASE -= -O0
#QMAKE_CXXFLAGS_RELEASE -= -O
#QMAKE_LFLAGS_RELEASE -= -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE += -Ox
#QMAKE_CXXFLAGS_RELEASE += -Ofast


CONFIG += BUILD_FOR_APP32
#CONFIG += BUILD_FOR_APP64
#CONFIG += BUILD_FOR_LIB32
#CONFIG += BUILD_FOR_R32
#CONFIG += BUILD_FOR_R64
#CONFIG += BUILD_FOR_MGW32
#CONFIG += BUILD_FOR_MGW64

BUILD_FOR_APP32 {
    TEMPLATE = app
    CONFIG += shared
    ARCHI = x86
    COMPILER = msvc14
    LIBS += -lSecur32 -lWs2_32 -lShell32 -lAdvapi32
}

BUILD_FOR_APP64 {
    TEMPLATE = app
    CONFIG += shared
    ARCHI = x64
    COMPILER = msvc14
    LIBS += -lSecur32 -lWs2_32 -lShell32 -lAdvapi32
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
    COMPILER = mingw-4.9.3
}

BUILD_FOR_R64 {
    TEMPLATE = app
    CONFIG += static
    ARCHI = x64
    COMPILER = mingw-4.9.3
}

BUILD_FOR_MGW32 {
    TEMPLATE = app
    CONFIG += static
    ARCHI = x86
    COMPILER = mingw_6.3.0
}

BUILD_FOR_MGW64 {
    TEMPLATE = app
    CONFIG += static
    ARCHI = x64
    COMPILER = mingw_6.3.0
}

LIBS += -LD:/PAMStudio/dev/ext_libs/$$COMPILER/$$ARCHI/shared -llibpq
INCLUDEPATH +=  D:/PAMStudio/dev/ext_libs/include \
                D:/PAMStudio/dev/git/samara_2/small_sln/src
SOURCES += \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/Connections.cpp \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/DateTime.cpp \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/main.cpp \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/ParametersReader.cpp \
    src/variables.cpp

HEADERS += \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/BhyTypeFAO.h \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/Bileau.h \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/Connections.hpp \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/DateTime.hpp \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/delphi_defines.h \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/delphirunner.h \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/Meteo.h \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/MilBilanCarbone.h \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/ModelParameters.hpp \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/ParametersReader.hpp \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/Risocas2cpp.h \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/Riz.h \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/sorghum.h \
    D:/PAMStudio/dev/git/samara_2/small_sln/src/tmp.h \
    src/variables.h \
    src/parameters.h \
    src/samara.h \
    src/parcelle.h
