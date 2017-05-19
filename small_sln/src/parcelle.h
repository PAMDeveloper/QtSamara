#ifndef PARCELLE_H
#define PARCELLE_H
#include "delphi_defines.h"
#include "variables.h"
#include "parameters.h"

void init_parcelle() {

    double Stockini2;
    double Stockini1;

    try {
        VolRelMacropores = 100 * (HumSat - HumFC);
        ResUtil = (HumFC - HumPF) * 1000;
        ProfRu = EpaisseurSurf + EpaisseurProf;
        // à supprimer ultérieurement dans les différents modules
        RuSurf = ResUtil * EpaisseurSurf * 1.0 / 1000;
        CapaREvap = 0.5 * EpaisseurSurf * HumPF;
        CapaRFE = PEvap * (CapaREvap + RuSurf);
        CapaRDE = RuSurf - CapaRFE;
        StRuMax = ResUtil * ProfRu * 1.0 / 1000;
        Stockini1 = min(StockIniSurf, CapaREvap + RuSurf);
        Stockini2 = min(StockIniProf, ResUtil * EpaisseurProf * 1.0 / 1000);
        ValRSurf = min(Stockini1, CapaREvap + CapaRDE);
        ValRDE = max(0., ValRSurf - CapaREvap);
        ValRFE = max(0., Stockini1 - (CapaREvap + CapaRDE));
        StockSurface = ValRDE + ValRFE;
        StockTotal = StockSurface + Stockini2; //transpirable
        Hum = StockTotal;
        LTRkdfcl = 1;
        LIRkdf = 0;
        LTRkdf = 0;
        CounterNursery = 0;
        VolMacropores = VolRelMacropores * (EpaisseurSurf + EpaisseurProf) * 1.0 / 100;

    } catch (...) {
        AfficheMessageErreur("RS_InitParcelle_V2", URisocas);
    }
}

#endif // PARCELLE_H
