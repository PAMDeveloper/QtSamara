TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
#QMAKE_CXXFLAGS_RELEASE -= -O0
#QMAKE_CXXFLAGS_RELEASE -= -O
#QMAKE_LFLAGS_RELEASE -= -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE += -Ox


CONFIG += BUILD_FOR_APP32
#CONFIG += BUILD_FOR_LIB32
#CONFIG += BUILD_FOR_R32
#CONFIG += BUILD_FOR_R64

BUILD_FOR_APP32 {
    TEMPLATE = app
    CONFIG += shared
    ARCHI = x86
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
    TEMPLATE = lib
    CONFIG += static
    ARCHI = x86
    COMPILER = mingw-4.9.3
}

BUILD_FOR_R64 {
    TEMPLATE = lib
    CONFIG += static
    ARCHI = x64
    COMPILER = mingw-4.9.3
}

LIBS += -L../../PAMStudio/dev/ext_libs/$$COMPILER/$$ARCHI/static -llibpq
INCLUDEPATH +=  ../../PAMStudio/dev/ext_libs/include \
                ../../PAMStudio/dev/git/samara_2/small_sln/src
SOURCES += \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/Connections.cpp \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/DateTime.cpp \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/main.cpp \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/ParametersReader.cpp

HEADERS += \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/BhyTypeFAO.h \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/Bileau.h \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/Connections.hpp \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/DateTime.hpp \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/delphi_defines.h \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/delphirunner.h \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/Meteo.h \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/MilBilanCarbone.h \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/ModelParameters.hpp \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/ParametersReader.hpp \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/Risocas2cpp.h \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/Riz.h \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/sorghum.h \
    ../../PAMStudio/dev/git/samara_2/small_sln/src/tmp.h
