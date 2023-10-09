QT     += core gui charts sql widgets
CONFIG += c++11

win32:RC_ICONS += samara.ico
TEMPLATE = app
NAME = samara

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -Ox

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

INCLUDEPATH +=  ../ext_libs/include \
                ./src/ \
                ../rsamara/src

HEADERS += \
    ../rSamara/src/samara.h \
    ../rSamara/src/samara_defines.h \
    ../rSamara/src/parameters.h \
    ../rSamara/src/processes/processes.h \
    ../rSamara/src/samara_parameters.h \
    ../rSamara/src/samara_variables.h \
#    ../rsamara/src/XSamara/DemandePlante_V2_1.h \
#    ../rsamara/src/XSamara/EvalConversion.h \
#    ../rsamara/src/XSamara/EvalETRETM.h \
#    ../rsamara/src/XSamara/EvalTranspi.h \
#    ../rsamara/src/XSamara/EvolPhenoPSPStress.h \
#    ../rsamara/src/XSamara/EvolSomDegresJour.h \
#    ../rsamara/src/XSamara/RS_AddResToGrowthStructPop_V2_1.h \
#    ../rsamara/src/XSamara/RS_AutomaticIrrigation_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalAssim.h \
#    ../rsamara/src/XSamara/RS_EvalAssimPot_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalClumpAndLightInter_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalColdStress.h \
#    ../rsamara/src/XSamara/RS_EvalCstrAssim.h \
#    ../rsamara/src/XSamara/RS_EvalCstrPFactorFAO_V2.h \
#    ../rsamara/src/XSamara/RS_EvalDAF_V2.h \
#    ../rsamara/src/XSamara/RS_EvalDateGermination.h \
#    ../rsamara/src/XSamara/RS_EvalDegresJourVitMoy_V2.h \
#    ../rsamara/src/XSamara/RS_EvalDemandStructIN_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalDemandStructLeaf_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalDemandStructPanicle_V2.h \
#    ../rsamara/src/XSamara/RS_EvalDemandStructRoot_V2.h \
#    ../rsamara/src/XSamara/RS_EvalDemandStructSheath.h \
#    ../rsamara/src/XSamara/RS_EvalDemandTotAndIcPreFlow_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalEvapPot.h \
#    ../rsamara/src/XSamara/RS_EvalFTSW_V2.h \
#    ../rsamara/src/XSamara/RS_EvalFtswMoy.h \
#    ../rsamara/src/XSamara/RS_EvalLai_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalMaximumLai.h \
#    ../rsamara/src/XSamara/RS_EvalParIntercepte_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalRUE_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalRelPotLeafLength.h \
#    ../rsamara/src/XSamara/RS_EvalRespMaint.h \
#    ../rsamara/src/XSamara/RS_EvalRootFront.h \
#    ../rsamara/src/XSamara/RS_EvalRuiss_FloodDyna_V2.h \
#    ../rsamara/src/XSamara/RS_EvalSDJPhase4.h \
#    ../rsamara/src/XSamara/RS_EvalSimAnthesis50.h \
#    ../rsamara/src/XSamara/RS_EvalSimEmergence.h \
#    ../rsamara/src/XSamara/RS_EvalSimPanIni.h \
#    ../rsamara/src/XSamara/RS_EvalSimStartGermin.h \
#    ../rsamara/src/XSamara/RS_EvalSimStartMatu2.h \
#    ../rsamara/src/XSamara/RS_EvalSimStartPSP.h \
#    ../rsamara/src/XSamara/RS_EvalSlaMitch.h \
#    ../rsamara/src/XSamara/RS_EvalSterility.h \
#    ../rsamara/src/XSamara/RS_EvalSupplyTot_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvalTMaxMoy.h \
#    ../rsamara/src/XSamara/RS_EvalTMinMoy.h \
#    ../rsamara/src/XSamara/RS_EvalVitesseRacinaire.h \
#    ../rsamara/src/XSamara/RS_EvolConsRes_Flood_V2.h \
#    ../rsamara/src/XSamara/RS_EvolDemPanFilPopAndIcPFlow_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolDryMatTot_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolEvapSurfRFE_RDE_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolGrowthReserveInternode_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructINPop_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructLeafPop_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructPanPop.h \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructRootPop.h \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructSheathPop.h \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructTot_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolGrowthTot_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolHauteur_SDJ_cstr_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolKcpKceBilhy.h \
#    ../rsamara/src/XSamara/RS_EvolMobiliLeafDeath_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolMobiliTillerDeath_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolPSPMVMD.h \
#    ../rsamara/src/XSamara/RS_EvolPanicleFilPop_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolPlantLeafNumTot.h \
#    ../rsamara/src/XSamara/RS_EvolPlantTilNumTot_V2.h \
#    ../rsamara/src/XSamara/RS_EvolRempliMacropores_V2.h \
#    ../rsamara/src/XSamara/RS_EvolRempliResRFE_RDE_V2.h \
#    ../rsamara/src/XSamara/RS_EvolRurRFE_RDE_V2_1.h \
#    ../rsamara/src/XSamara/RS_EvolSomDegresJourCor.h \
#    ../rsamara/src/XSamara/RS_EvolWaterLoggingUpland_V2.h \
#    ../rsamara/src/XSamara/RS_ExcessAssimilToRoot_V2.h \
#    ../rsamara/src/XSamara/RS_KeyResults_V2_1.h \
#    ../rsamara/src/XSamara/RS_LeafRolling_V2_1.h \
#    ../rsamara/src/XSamara/RS_Phyllochron.h \
#    ../rsamara/src/XSamara/RS_PlantSubmergence_V2.h \
#    ../rsamara/src/XSamara/RS_Priority2GrowthPanStrctPop_V2_1.h \
#    ../rsamara/src/XSamara/RS_TransplantingShock_V2.h \
#    ../rsamara/src/XSamara/RS_Transplanting_V2.h \
#    ../rsamara/src/XSamara/XSamaraAuxiliary.h \
#    ../rsamara/src/XSamara/XSamaraComponent.h \
#    ../rsamara/src/XSamara/XSamaraExogenous.h \
#    ../rsamara/src/XSamara/XSamaraRate.h \
#    ../rsamara/src/XSamara/XSamaraState.h \
    \
    ../rSamara/src/utils/juliancalculator.h \
    ../rSamara/src/utils/resultparser.h \
    ../rSamara/src/utils/simulationloader.h \
    ../rSamara/src/utils/de.h \
    ../rSamara/src/utils/samarafitness.h \
    \
    src/qtapp/callout.h \
    src/qtapp/DBEcosysloader.h \
    src/qtapp/loaderview.h \
    src/qtapp/mainwindow.h \
    src/qtapp/meteodatamodel.h \
    src/qtapp/parametersdatamodel.h \
    src/qtapp/resultsdatamodel.h \
    src/qtapp/resultview.h \
    src/qtapp/comparisondatamodel.h \
    src/qtapp/chartmanager.h \
    src/qtapp/resultsmanager.h \
    src/qtapp/comparisonmanager.h \
    src/qtapp/DBAccessLoader.h \
    src/qtapp/chartview.h \
    src/qtapp/estimparamdatamodel.h \
    src/qtapp/obsdatamodel.h


SOURCES += \
    ../rSamara/src/samara.cpp \
#    ../rsamara/src/XSamara/DemandePlante_V2_1.cpp \
#    ../rsamara/src/XSamara/EvalConversion.cpp \
#    ../rsamara/src/XSamara/EvalETRETM.cpp \
#    ../rsamara/src/XSamara/EvalTranspi.cpp \
#    ../rsamara/src/XSamara/EvolPhenoPSPStress.cpp \
#    ../rsamara/src/XSamara/EvolSomDegresJour.cpp \
#    ../rsamara/src/XSamara/RS_AddResToGrowthStructPop_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_AutomaticIrrigation_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalAssim.cpp \
#    ../rsamara/src/XSamara/RS_EvalAssimPot_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalClumpAndLightInter_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalColdStress.cpp \
#    ../rsamara/src/XSamara/RS_EvalCstrAssim.cpp \
#    ../rsamara/src/XSamara/RS_EvalCstrPFactorFAO_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvalDAF_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvalDateGermination.cpp \
#    ../rsamara/src/XSamara/RS_EvalDegresJourVitMoy_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvalDemandStructIN_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalDemandStructLeaf_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalDemandStructPanicle_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvalDemandStructRoot_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvalDemandStructSheath.cpp \
#    ../rsamara/src/XSamara/RS_EvalDemandTotAndIcPreFlow_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalEvapPot.cpp \
#    ../rsamara/src/XSamara/RS_EvalFTSW_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvalFtswMoy.cpp \
#    ../rsamara/src/XSamara/RS_EvalLai_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalMaximumLai.cpp \
#    ../rsamara/src/XSamara/RS_EvalParIntercepte_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalRUE_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalRelPotLeafLength.cpp \
#    ../rsamara/src/XSamara/RS_EvalRespMaint.cpp \
#    ../rsamara/src/XSamara/RS_EvalRootFront.cpp \
#    ../rsamara/src/XSamara/RS_EvalRuiss_FloodDyna_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvalSDJPhase4.cpp \
#    ../rsamara/src/XSamara/RS_EvalSimAnthesis50.cpp \
#    ../rsamara/src/XSamara/RS_EvalSimEmergence.cpp \
#    ../rsamara/src/XSamara/RS_EvalSimPanIni.cpp \
#    ../rsamara/src/XSamara/RS_EvalSimStartGermin.cpp \
#    ../rsamara/src/XSamara/RS_EvalSimStartMatu2.cpp \
#    ../rsamara/src/XSamara/RS_EvalSimStartPSP.cpp \
#    ../rsamara/src/XSamara/RS_EvalSlaMitch.cpp \
#    ../rsamara/src/XSamara/RS_EvalSterility.cpp \
#    ../rsamara/src/XSamara/RS_EvalStressWaterLogging_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvalSupplyTot_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvalTMaxMoy.cpp \
#    ../rsamara/src/XSamara/RS_EvalTMinMoy.cpp \
#    ../rsamara/src/XSamara/RS_EvalVitesseRacinaire.cpp \
#    ../rsamara/src/XSamara/RS_EvolConsRes_Flood_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvolDemPanFilPopAndIcPFlow_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolDryMatTot_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolEvapSurfRFE_RDE_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolGrowthReserveInternode_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructINPop_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructLeafPop_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructPanPop.cpp \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructRootPop.cpp \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructSheathPop.cpp \
#    ../rsamara/src/XSamara/RS_EvolGrowthStructTot_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolGrowthTot_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolHauteur_SDJ_cstr_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolKcpKceBilhy.cpp \
#    ../rsamara/src/XSamara/RS_EvolMobiliLeafDeath_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolMobiliTillerDeath_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolPSPMVMD.cpp \
#    ../rsamara/src/XSamara/RS_EvolPanicleFilPop_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolPlantLeafNumTot.cpp \
#    ../rsamara/src/XSamara/RS_EvolPlantTilNumTot_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvolRempliMacropores_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvolRempliResRFE_RDE_V2.cpp \
#    ../rsamara/src/XSamara/RS_EvolRurRFE_RDE_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_EvolSomDegresJourCor.cpp \
#    ../rsamara/src/XSamara/RS_EvolWaterLoggingUpland_V2.cpp \
#    ../rsamara/src/XSamara/RS_ExcessAssimilToRoot_V2.cpp \
#    ../rsamara/src/XSamara/RS_KeyResults_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_LeafRolling_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_Phyllochron.cpp \
#    ../rsamara/src/XSamara/RS_PlantSubmergence_V2.cpp \
#    ../rsamara/src/XSamara/RS_Priority2GrowthPanStrctPop_V2_1.cpp \
#    ../rsamara/src/XSamara/RS_TransplantingShock_V2.cpp \
#    ../rsamara/src/XSamara/RS_Transplanting_V2.cpp \
#    ../rsamara/src/XSamara/SorghumMortality.cpp \
#    ../rsamara/src/XSamara/XSamaraAuxiliary.cpp \
#    ../rsamara/src/XSamara/XSamaraComponent.cpp \
#    ../rsamara/src/XSamara/XSamaraExogenous.cpp \
#    ../rsamara/src/XSamara/XSamaraRate.cpp \
#    ../rsamara/src/XSamara/XSamaraState.cpp \
    \
    src/qtapp/callout.cpp \
    src/qtapp/loaderview.cpp \
    src/qtapp/mainwindow.cpp \
    src/qtapp/meteodatamodel.cpp \
    src/qtapp/parametersdatamodel.cpp \
    src/qtapp/resultsdatamodel.cpp \
    src/qtapp/resultview.cpp \
    src/qtapp/comparisondatamodel.cpp \
    src/qtapp/chartmanager.cpp \
    src/qtapp/resultsmanager.cpp \
    src/qtapp/comparisonmanager.cpp \
    src/qtapp/chartview.cpp \
    src/main.cpp \
    src/qtapp/estimparamdatamodel.cpp \
    src/qtapp/obsdatamodel.cpp

FORMS += \
    src/qtapp/mainwindow.ui

RESOURCES += \
    resources.qrc
