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
    src/Connections.cpp \
    src/DateTime.cpp \
    src/main.cpp \
    src/ParametersReader.cpp \
    src/samara.cpp

HEADERS += \
    src/BhyTypeFAO.h \
    src/Bileau.h \
    src/Connections.hpp \
    src/DateTime.hpp \
    src/delphi_defines.h \
    src/delphirunner.h \
    src/MilBilanCarbone.h \
    src/ModelParameters.hpp \
    src/ParametersReader.hpp \
    src/Risocas2cpp.h \
    src/Riz.h \
    src/samara.h \
    src/sorghum.h \
    src/variables.h

