/**
 * @file samara/model/models/samara/SamaraModel.cpp
 * @author The Samara Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2013-2017 Cirad http://www.cirad.fr
 * Copyright (C) 2013-2017 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <samara/model/models/samara/SamaraModel.hpp>
#include <samara/utils/DateTime.hpp>
#include <samara/utils/Connections.hpp>

namespace samara {
double SamaraModel::NullValue = -999;

void SamaraModel::writeResultInDB(const std::string idsimu,double jour)
{
    std::map < std::string,std::string > occurrenceResult;
    std::map < std::string,std::string >::iterator itOccurrRes;
    std::string requete,requeteInsert,requeteUpdate,requeteSetUpdate,requeteTest,requete_nom_colonne="",requete_valeur_colonne="",nomVar,valeurVar;
    PGconn* connection(utils::Connections::connection(
                        "samara", "host=localhost port=5432 dbname=samara user=user_samara password=samarapassword"));

    std::ostringstream strs;
    PGresult* result;


    occurrenceResult["idsimulation"] = idsimu;
    occurrenceResult["jour"] = utils::DateTime::toJulianDay(jour);

    strs << nbjas();
    occurrenceResult["nbjas"] = strs.str();
    strs.str("");

    strs << numphase();
    occurrenceResult["numphase"] = strs.str();
    strs.str("");

    strs << degresdujour();
    occurrenceResult["degresdujour"] = strs.str();
    strs.str("");

    strs << degresdujourcor();
    occurrenceResult["degresdujourcor"] = strs.str();
    strs.str("");

    strs << lai();
    occurrenceResult["lai"] = strs.str();
    strs.str("");

    strs << ftsw();
    occurrenceResult["ftsw"] = strs.str();
    strs.str("");

    strs << cstr();
    occurrenceResult["cstr"] = strs.str();
    strs.str("");

    strs << rootfront();
    occurrenceResult["rootfront"] = strs.str();
    strs.str("");

    strs << eto();
    occurrenceResult["eto"] = strs.str();
    strs.str("");

    strs << etm();
    occurrenceResult["etm"] = strs.str();
    strs.str("");

    strs << etr();
    occurrenceResult["etr"] = strs.str();
    strs.str("");

    strs << rue();
    occurrenceResult["rue"] = strs.str();
    strs.str("");

    strs << conversioneff();
    occurrenceResult["conversioneff"] = strs.str();
    strs.str("");

    strs << ic();
    occurrenceResult["ic"] = strs.str();
    strs.str("");

    strs << culmsperplant();
    occurrenceResult["culmsperplant"] = strs.str();
    strs.str("");

    strs << culmspop();
    occurrenceResult["culmspop"] = strs.str();
    strs.str("");

    strs << culmsperhill();
    occurrenceResult["culmsperhill"] = strs.str();
    strs.str("");

    strs << grainyieldpop();
    occurrenceResult["grainyieldpop"] = strs.str();
    strs.str("");

    strs << drymatstructleafpop();
    occurrenceResult["drymatstructleafpop"] = strs.str();
    strs.str("");

    strs << drymatstructsheathpop();
    occurrenceResult["drymatstructsheathpop"] = strs.str();
    strs.str("");

    strs << drymatstructrootpop();
    occurrenceResult["drymatstructrootpop"] = strs.str();
    strs.str("");

    strs << drymatstructinternodepop();
    occurrenceResult["drymatstructinternodepop"] = strs.str();
    strs.str("");

    strs << drymatresinternodepop();
    occurrenceResult["drymatresinternodepop"] = strs.str();
    strs.str("");

    strs << drymatstructpaniclepop();
    occurrenceResult["drymatstructpaniclepop"] = strs.str();
    strs.str("");

    strs << drymatstructtotpop();
    occurrenceResult["drymatstructtotpop"] = strs.str();
    strs.str("");

    strs << drymatvegetotpop();
    occurrenceResult["drymatvegetotpop"] = strs.str();
    strs.str("");

    strs << drymatpanicletotpop();
    occurrenceResult["drymatpanicletotpop"] = strs.str();
    strs.str("");

    strs << drymatstempop();
    occurrenceResult["drymatstempop"] = strs.str();
    strs.str("");

    strs << drymatabovegroundpop();
    occurrenceResult["drymatabovegroundpop"] = strs.str();
    strs.str("");

    strs << drymattotpop();
    occurrenceResult["drymattotpop"] = strs.str();
    strs.str("");

    strs << sterilitycold();
    occurrenceResult["sterilitycold"] = strs.str();
    strs.str("");

    strs << sterilityheat();
    occurrenceResult["sterilityheat"] = strs.str();
    strs.str("");

    strs << sterilitydrought();
    occurrenceResult["sterilitydrought"] = strs.str();
    strs.str("");

    strs << sterilitytot();
    occurrenceResult["sterilitytot"] = strs.str();
    strs.str("");

    strs << harvestindex();
    occurrenceResult["harvestindex"] = strs.str();
    strs.str("");

    strs << paniclenumpop();
    occurrenceResult["paniclenumpop"] = strs.str();
    strs.str("");

    strs << paniclenumplant();
    occurrenceResult["paniclenumplant"] = strs.str();
    strs.str("");

    strs << grainyieldpanicle();
    occurrenceResult["grainyieldpanicle"] = strs.str();
    strs.str("");

    strs << spikenumpop();
    occurrenceResult["spikenumpop"] = strs.str();
    strs.str("");

    strs << spikenumpanicle();
    occurrenceResult["spikenumpanicle"] = strs.str();
    strs.str("");

    strs << fertspikenumpop();
    occurrenceResult["fertspikenumpop"] = strs.str();
    strs.str("");

    strs << grainfillingstatus();
    occurrenceResult["grainfillingstatus"] = strs.str();
    strs.str("");

    strs << phasestemelongation();
    occurrenceResult["phasestemelongation"] = strs.str();
    strs.str("");

    strs << sla();
    occurrenceResult["sla"] = strs.str();
    strs.str("");

    strs << haunindex();
    occurrenceResult["haunindex"] = strs.str();
    strs.str("");

    strs << apexheight();
    occurrenceResult["apexheight"] = strs.str();
    strs.str("");

    strs << plantheight();
    occurrenceResult["plantheight"] = strs.str();
    strs.str("");

    strs << plantwidth();
    occurrenceResult["plantwidth"] = strs.str();
    strs.str("");

    strs << krolling();
    occurrenceResult["krolling"] = strs.str();
    strs.str("");

    strs << lirkdfcl();
    occurrenceResult["lirkdfcl"] = strs.str();
    strs.str("");

    strs << ltrkdfcl();
    occurrenceResult["ltrkdfcl"] = strs.str();
    strs.str("");

    strs << assimpot();
    occurrenceResult["assimpot"] = strs.str();
    strs.str("");

    strs << assim();
    occurrenceResult["assim"] = strs.str();
    strs.str("");

    strs << respmainttot();
    occurrenceResult["respmainttot"] = strs.str();
    strs.str("");

    strs << supplytot();
    occurrenceResult["supplytot"] = strs.str();
    strs.str("");

    strs << assimsurplus();
    occurrenceResult["assimsurplus"] = strs.str();
    strs.str("");

    strs << assimnotused();
    occurrenceResult["assimnotused"] = strs.str();
    strs.str("");

    strs << tillerdeathpop();
    occurrenceResult["tillerdeathpop"] = strs.str();
    strs.str("");

    strs << plantleafnumtot();
    occurrenceResult["plantleafnumtot"] = strs.str();
    strs.str("");

    strs << deadleafdrywtpop();
    occurrenceResult["deadleafdrywtpop"] = strs.str();
    strs.str("");

    strs << laidead();
    occurrenceResult["laidead"] = strs.str();
    strs.str("");

    strs << rescapacityinternodepop();
    occurrenceResult["rescapacityinternodepop"] = strs.str();
    strs.str("");

    strs << internoderesstatus();
    occurrenceResult["internoderesstatus"] = strs.str();
    strs.str("");

    strs << daylength();
    occurrenceResult["daylength"] = strs.str();
    strs.str("");

    strs << par();
    occurrenceResult["par"] = strs.str();
    strs.str("");

    strs << rgcalc();
    occurrenceResult["rgcalc"] = strs.str();
    strs.str("");

//    strs << vpdcalc();
//    occurrenceResult["vpdcalc"] = strs.str();
//    strs.str("");

    strs << tmoycalc();
    occurrenceResult["tmoycalc"] = strs.str();
    strs.str("");

    strs << hmoycalc();
    occurrenceResult["hmoycalc"] = strs.str();
    strs.str("");

    strs << kcereal();
    occurrenceResult["kcereal"] = strs.str();
    strs.str("");

    strs << stocktotal();
    occurrenceResult["stocktotal"] = strs.str();
    strs.str("");

    strs << eaudispo();
    occurrenceResult["eaudispo"] = strs.str();
    strs.str("");

    strs << stocksurface();
    occurrenceResult["stocksurface"] = strs.str();
    strs.str("");

    strs << stockrac();
    occurrenceResult["stockrac"] = strs.str();
    strs.str("");

    strs << rurac();
    occurrenceResult["rurac"] = strs.str();
    strs.str("");

    strs << kcp();
    occurrenceResult["kcp"] = strs.str();
    strs.str("");

    strs << kce();
    occurrenceResult["kce"] = strs.str();
    strs.str("");

    strs << evappot();
    occurrenceResult["evappot"] = strs.str();
    strs.str("");

    strs << evap();
    occurrenceResult["evap"] = strs.str();
    strs.str("");

    strs << trpot();
    occurrenceResult["trpot"] = strs.str();
    strs.str("");

    strs << tr();
    occurrenceResult["tr"] = strs.str();
    strs.str("");

    strs << lr();
    occurrenceResult["lr"] = strs.str();
    strs.str("");

    strs << dr();
    occurrenceResult["dr"] = strs.str();
    strs.str("");

    strs << parintercepte();
    occurrenceResult["parintercepte"] = strs.str();
    strs.str("");

    strs << sommedegresjour();
    occurrenceResult["sumdegresday"] = strs.str();
    strs.str("");

    strs << vitesseracinaire();
    occurrenceResult["vitesseracinaire"] = strs.str();
    strs.str("");

    strs << cstrassim();
    occurrenceResult["cstrassim"] = strs.str();
    strs.str("");

    strs << rayextra();
    occurrenceResult["rayextra"] = strs.str();
    strs.str("");

    strs << cumpar();
    occurrenceResult["cumpar"] = strs.str();
    strs.str("");

    strs << sommedegresjourcor();
    occurrenceResult["sumdegredaycor"] = strs.str();
    strs.str("");

    strs << sumpp();
    occurrenceResult["sumpp"] = strs.str();
    strs.str("");

    strs << sommedegresjourphaseprecedente();
    occurrenceResult["sumddphaseprec"] = strs.str();
    strs.str("");

    strs << respmaintdebt();
    occurrenceResult["respmaintdebt"] = strs.str();
    strs.str("");

    strs << tminmoy();
    occurrenceResult["tminmoy"] = strs.str();
    strs.str("");

    strs << tmaxmoy();
    occurrenceResult["tmaxmoy"] = strs.str();
    strs.str("");

    strs << ftswmoy();
    occurrenceResult["ftswmoy"] = strs.str();
    strs.str("");

    strs << rootshootratio();
    occurrenceResult["rootshootratio"] = strs.str();
    strs.str("");

    strs << treffinst();
    occurrenceResult["treffinst"] = strs.str();
    strs.str("");

    strs << treff();
    occurrenceResult["treff"] = strs.str();
    strs.str("");

    strs << wueet();
    occurrenceResult["wueet"] = strs.str();
    strs.str("");

    strs << wuetot();
    occurrenceResult["wuetot"] = strs.str();
    strs.str("");

    strs << floodwaterdepth();
    occurrenceResult["floodwaterdepth"] = strs.str();
    strs.str("");

    strs << irrigautoday();
    occurrenceResult["irrigautoday"] = strs.str();
    strs.str("");

    strs << irrigtotday();
    occurrenceResult["irrigtotday"] = strs.str();
    strs.str("");

    strs << fractionplantheightsubmer();
    occurrenceResult["fractionplantheightsubmer"] = strs.str();
    strs.str("");

    strs << density();
    occurrenceResult["density"] = strs.str();
    strs.str("");

    strs << rootmasspervol();
    occurrenceResult["rootmasspervol"] = strs.str();
    strs.str("");

    strs << volmacropores();
    occurrenceResult["volmacropores"] = strs.str();
    strs.str("");

    strs << stockmacropores();
    occurrenceResult["stockmacropores"] = strs.str();
    strs.str("");

    strs << relpotleaflength();
    occurrenceResult["relpotleaflength"] = strs.str();
    strs.str("");

    strs << apexheightgain();
    occurrenceResult["apexheightgain"] = strs.str();
    strs.str("");

    strs << haungain();
    occurrenceResult["haungain"] = strs.str();
    strs.str("");

    strs << conversion();
    occurrenceResult["conversion"] = strs.str();
    strs.str("");

    strs << stresscold();
    occurrenceResult["stresscold"] = strs.str();
    strs.str("");

    strs << fractionrootslogged();
    occurrenceResult["fractionrootslogged"] = strs.str();
    strs.str("");

    strs << resutil();
    occurrenceResult["resutil"] = strs.str();
    strs.str("");

    strs << kctot();
    occurrenceResult["kctot"] = strs.str();
    strs.str("");


    requeteInsert ="INSERT INTO resultat(";
    requeteUpdate = "UPDATE resultat SET ";
    for(itOccurrRes = occurrenceResult.begin();itOccurrRes != occurrenceResult.end();itOccurrRes++){
        nomVar = itOccurrRes->first;
        valeurVar = itOccurrRes->second;

        if (valeurVar.find("nan") != std::string::npos || valeurVar.find("inf") != std::string::npos)
            valeurVar = "NULL";
        if ((nomVar.compare("idsimulation") ==0) || (nomVar.compare("jour") ==0))
            valeurVar = "'" + valeurVar + "'";

        requeteSetUpdate = requeteSetUpdate + nomVar + "=" + valeurVar + ",";
        requete_nom_colonne = requete_nom_colonne + nomVar + ",";
        requete_valeur_colonne = requete_valeur_colonne + valeurVar + ",";
    }

    requeteTest = "SELECT idsimulation,jour FROM resultat WHERE idsimulation ='" +
                        occurrenceResult["idsimulation"] + "' AND jour='" + occurrenceResult["jour"] + "'";

    result = PQexec(connection,requeteTest.c_str());
    if (PQresultStatus(result) != PGRES_TUPLES_OK){
            std::cout << "Error: " << PQerrorMessage(connection) << std::endl;
    }
    if (PQntuples(result) > 0){
        requeteSetUpdate = requeteSetUpdate.substr(0,requeteSetUpdate.length()-1);
        requeteUpdate = requeteUpdate + requeteSetUpdate + " WHERE idsimulation='" + occurrenceResult["idsimulation"]
                        + "' AND jour='" + occurrenceResult["jour"] + "'";
        requete = requeteUpdate;
    }
    else {
        requete_nom_colonne = requete_nom_colonne.substr(0,requete_nom_colonne.length()-1);
        requete_valeur_colonne = requete_valeur_colonne.substr(0,requete_valeur_colonne.length()-1);
        requeteInsert = requeteInsert + requete_nom_colonne + ") VALUES (" + requete_valeur_colonne + ")";
        requete = requeteInsert;
    }
    PQclear(result);


    //std::cout<<requete<<std::endl;
    result = PQexec(connection,requete.c_str());
    if (PQresultStatus(result) != PGRES_COMMAND_OK){
            std::cout << "Error: " << PQerrorMessage(connection) << std::endl;
    }
    PQclear(result);
}

void SamaraModel::writeResultInFile(const std::string idsimu,double jour){

    std::map < std::string,std::string > occurrenceResult;
    std::map < std::string,std::string >::iterator itOccurrRes;
    std::string ligneDonnees="",ligneEntete="",nomVar,valeurVar;

    std::ostringstream strs;


    occurrenceResult["idsimulation"] = idsimu;
    occurrenceResult["jour"] = utils::DateTime::toJulianDay(jour);
    strs << nbjas();
    occurrenceResult["nbjas"] = strs.str();
    strs.str("");

    strs << numphase();
    occurrenceResult["numphase"] = strs.str();
    strs.str("");

    strs << degresdujour();
    occurrenceResult["degresdujour"] = strs.str();
    strs.str("");

    strs << degresdujourcor();
    occurrenceResult["degresdujourcor"] = strs.str();
    strs.str("");

    strs << lai();
    occurrenceResult["lai"] = strs.str();
    strs.str("");

    strs << ftsw();
    occurrenceResult["ftsw"] = strs.str();
    strs.str("");

    strs << cstr();
    occurrenceResult["cstr"] = strs.str();
    strs.str("");

    strs << rootfront();
    occurrenceResult["rootfront"] = strs.str();
    strs.str("");

    strs << eto();
    occurrenceResult["eto"] = strs.str();
    strs.str("");

    strs << etm();
    occurrenceResult["etm"] = strs.str();
    strs.str("");

    strs << etr();
    occurrenceResult["etr"] = strs.str();
    strs.str("");

    strs << rue();
    occurrenceResult["rue"] = strs.str();
    strs.str("");

    strs << conversioneff();
    occurrenceResult["conversioneff"] = strs.str();
    strs.str("");

    strs << ic();
    occurrenceResult["ic"] = strs.str();
    strs.str("");

    strs << culmsperplant();
    occurrenceResult["culmsperplant"] = strs.str();
    strs.str("");

    strs << culmspop();
    occurrenceResult["culmspop"] = strs.str();
    strs.str("");

    strs << culmsperhill();
    occurrenceResult["culmsperhill"] = strs.str();
    strs.str("");

    strs << grainyieldpop();
    occurrenceResult["grainyieldpop"] = strs.str();
    strs.str("");

    strs << drymatstructleafpop();
    occurrenceResult["drymatstructleafpop"] = strs.str();
    strs.str("");

    strs << drymatstructsheathpop();
    occurrenceResult["drymatstructsheathpop"] = strs.str();
    strs.str("");

    strs << drymatstructrootpop();
    occurrenceResult["drymatstructrootpop"] = strs.str();
    strs.str("");

    strs << drymatstructinternodepop();
    occurrenceResult["drymatstructinternodepop"] = strs.str();
    strs.str("");

    strs << drymatresinternodepop();
    occurrenceResult["drymatresinternodepop"] = strs.str();
    strs.str("");

    strs << drymatstructpaniclepop();
    occurrenceResult["drymatstructpaniclepop"] = strs.str();
    strs.str("");

    strs << drymatstructtotpop();
    occurrenceResult["drymatstructtotpop"] = strs.str();
    strs.str("");

    strs << drymatvegetotpop();
    occurrenceResult["drymatvegetotpop"] = strs.str();
    strs.str("");

    strs << drymatpanicletotpop();
    occurrenceResult["drymatpanicletotpop"] = strs.str();
    strs.str("");

    strs << drymatstempop();
    occurrenceResult["drymatstempop"] = strs.str();
    strs.str("");

    strs << drymatabovegroundpop();
    occurrenceResult["drymatabovegroundpop"] = strs.str();
    strs.str("");

    strs << drymattotpop();
    occurrenceResult["drymattotpop"] = strs.str();
    strs.str("");

    strs << sterilitycold();
    occurrenceResult["sterilitycold"] = strs.str();
    strs.str("");

    strs << sterilityheat();
    occurrenceResult["sterilityheat"] = strs.str();
    strs.str("");

    strs << sterilitydrought();
    occurrenceResult["sterilitydrought"] = strs.str();
    strs.str("");

    strs << sterilitytot();
    occurrenceResult["sterilitytot"] = strs.str();
    strs.str("");

    strs << harvestindex();
    occurrenceResult["harvestindex"] = strs.str();
    strs.str("");

    strs << paniclenumpop();
    occurrenceResult["paniclenumpop"] = strs.str();
    strs.str("");

    strs << paniclenumplant();
    occurrenceResult["paniclenumplant"] = strs.str();
    strs.str("");

    strs << grainyieldpanicle();
    occurrenceResult["grainyieldpanicle"] = strs.str();
    strs.str("");

    strs << spikenumpop();
    occurrenceResult["spikenumpop"] = strs.str();
    strs.str("");

    strs << spikenumpanicle();
    occurrenceResult["spikenumpanicle"] = strs.str();
    strs.str("");

    strs << fertspikenumpop();
    occurrenceResult["fertspikenumpop"] = strs.str();
    strs.str("");

    strs << grainfillingstatus();
    occurrenceResult["grainfillingstatus"] = strs.str();
    strs.str("");

    strs << phasestemelongation();
    occurrenceResult["phasestemelongation"] = strs.str();
    strs.str("");

    strs << sla();
    occurrenceResult["sla"] = strs.str();
    strs.str("");

    strs << haunindex();
    occurrenceResult["haunindex"] = strs.str();
    strs.str("");

    strs << apexheight();
    occurrenceResult["apexheight"] = strs.str();
    strs.str("");

    strs << plantheight();
    occurrenceResult["plantheight"] = strs.str();
    strs.str("");

    strs << plantwidth();
    occurrenceResult["plantwidth"] = strs.str();
    strs.str("");

    strs << krolling();
    occurrenceResult["krolling"] = strs.str();
    strs.str("");

    strs << lirkdfcl();
    occurrenceResult["lirkdfcl"] = strs.str();
    strs.str("");

    strs << ltrkdfcl();
    occurrenceResult["ltrkdfcl"] = strs.str();
    strs.str("");

    strs << assimpot();
    occurrenceResult["assimpot"] = strs.str();
    strs.str("");

    strs << assim();
    occurrenceResult["assim"] = strs.str();
    strs.str("");

    strs << respmainttot();
    occurrenceResult["respmainttot"] = strs.str();
    strs.str("");

    strs << supplytot();
    occurrenceResult["supplytot"] = strs.str();
    strs.str("");

    strs << assimsurplus();
    occurrenceResult["assimsurplus"] = strs.str();
    strs.str("");

    strs << assimnotused();
    occurrenceResult["assimnotused"] = strs.str();
    strs.str("");

    strs << tillerdeathpop();
    occurrenceResult["tillerdeathpop"] = strs.str();
    strs.str("");

    strs << plantleafnumtot();
    occurrenceResult["plantleafnumtot"] = strs.str();
    strs.str("");

    strs << deadleafdrywtpop();
    occurrenceResult["deadleafdrywtpop"] = strs.str();
    strs.str("");

    strs << laidead();
    occurrenceResult["laidead"] = strs.str();
    strs.str("");

    strs << rescapacityinternodepop();
    occurrenceResult["rescapacityinternodepop"] = strs.str();
    strs.str("");

    strs << internoderesstatus();
    occurrenceResult["internoderesstatus"] = strs.str();
    strs.str("");

    strs << daylength();
    occurrenceResult["daylength"] = strs.str();
    strs.str("");

    strs << par();
    occurrenceResult["par"] = strs.str();
    strs.str("");

    strs << rgcalc();
    occurrenceResult["rgcalc"] = strs.str();
    strs.str("");

//    strs << vpdcalc();
//    occurrenceResult["vpdcalc"] = strs.str();
//    strs.str("");

    strs << tmoycalc();
    occurrenceResult["tmoycalc"] = strs.str();
    strs.str("");

    strs << hmoycalc();
    occurrenceResult["hmoycalc"] = strs.str();
    strs.str("");

    strs << kcereal();
    occurrenceResult["kcereal"] = strs.str();
    strs.str("");

    strs << stocktotal();
    occurrenceResult["stocktotal"] = strs.str();
    strs.str("");

    strs << eaudispo();
    occurrenceResult["eaudispo"] = strs.str();
    strs.str("");

    strs << stocksurface();
    occurrenceResult["stocksurface"] = strs.str();
    strs.str("");

    strs << stockrac();
    occurrenceResult["stockrac"] = strs.str();
    strs.str("");

    strs << rurac();
    occurrenceResult["rurac"] = strs.str();
    strs.str("");

    strs << kcp();
    occurrenceResult["kcp"] = strs.str();
    strs.str("");

    strs << kce();
    occurrenceResult["kce"] = strs.str();
    strs.str("");

    strs << evappot();
    occurrenceResult["evappot"] = strs.str();
    strs.str("");

    strs << evap();
    occurrenceResult["evap"] = strs.str();
    strs.str("");

    strs << trpot();
    occurrenceResult["trpot"] = strs.str();
    strs.str("");

    strs << tr();
    occurrenceResult["tr"] = strs.str();
    strs.str("");

    strs << lr();
    occurrenceResult["lr"] = strs.str();
    strs.str("");

    strs << dr();
    occurrenceResult["dr"] = strs.str();
    strs.str("");

    strs << parintercepte();
    occurrenceResult["parintercepte"] = strs.str();
    strs.str("");

    strs << sommedegresjour();
    occurrenceResult["sumdegresday"] = strs.str();
    strs.str("");

    strs << vitesseracinaire();
    occurrenceResult["vitesseracinaire"] = strs.str();
    strs.str("");

    strs << cstrassim();
    occurrenceResult["cstrassim"] = strs.str();
    strs.str("");

    strs << rayextra();
    occurrenceResult["rayextra"] = strs.str();
    strs.str("");

    strs << cumpar();
    occurrenceResult["cumpar"] = strs.str();
    strs.str("");

    strs << sommedegresjourcor();
    occurrenceResult["sumdegredaycor"] = strs.str();
    strs.str("");

    strs << sumpp();
    occurrenceResult["sumpp"] = strs.str();
    strs.str("");

    strs << sommedegresjourphaseprecedente();
    occurrenceResult["sumddphaseprec"] = strs.str();
    strs.str("");

    strs << respmaintdebt();
    occurrenceResult["respmaintdebt"] = strs.str();
    strs.str("");

    strs << tminmoy();
    occurrenceResult["tminmoy"] = strs.str();
    strs.str("");

    strs << tmaxmoy();
    occurrenceResult["tmaxmoy"] = strs.str();
    strs.str("");

    strs << ftswmoy();
    occurrenceResult["ftswmoy"] = strs.str();
    strs.str("");

    strs << rootshootratio();
    occurrenceResult["rootshootratio"] = strs.str();
    strs.str("");

    strs << treffinst();
    occurrenceResult["treffinst"] = strs.str();
    strs.str("");

    strs << treff();
    occurrenceResult["treff"] = strs.str();
    strs.str("");

    strs << wueet();
    occurrenceResult["wueet"] = strs.str();
    strs.str("");

    strs << wuetot();
    occurrenceResult["wuetot"] = strs.str();
    strs.str("");

    strs << floodwaterdepth();
    occurrenceResult["floodwaterdepth"] = strs.str();
    strs.str("");

    strs << irrigautoday();
    occurrenceResult["irrigautoday"] = strs.str();
    strs.str("");

    strs << irrigtotday();
    occurrenceResult["irrigtotday"] = strs.str();
    strs.str("");

    strs << fractionplantheightsubmer();
    occurrenceResult["fractionplantheightsubmer"] = strs.str();
    strs.str("");

    strs << density();
    occurrenceResult["density"] = strs.str();
    strs.str("");

    strs << rootmasspervol();
    occurrenceResult["rootmasspervol"] = strs.str();
    strs.str("");

    strs << volmacropores();
    occurrenceResult["volmacropores"] = strs.str();
    strs.str("");

    strs << stockmacropores();
    occurrenceResult["stockmacropores"] = strs.str();
    strs.str("");

    strs << relpotleaflength();
    occurrenceResult["relpotleaflength"] = strs.str();
    strs.str("");

    strs << apexheightgain();
    occurrenceResult["apexheightgain"] = strs.str();
    strs.str("");

    strs << haungain();
    occurrenceResult["haungain"] = strs.str();
    strs.str("");

    strs << conversion();
    occurrenceResult["conversion"] = strs.str();
    strs.str("");

    strs << stresscold();
    occurrenceResult["stresscold"] = strs.str();
    strs.str("");

    strs << fractionrootslogged();
    occurrenceResult["fractionrootslogged"] = strs.str();
    strs.str("");

    strs << resutil();
    occurrenceResult["resutil"] = strs.str();
    strs.str("");

    strs << kctot();
    occurrenceResult["kctot"] = strs.str();
    strs.str("");


    for(itOccurrRes = occurrenceResult.begin();itOccurrRes != occurrenceResult.end();itOccurrRes++){
        nomVar = itOccurrRes->first;
        valeurVar = itOccurrRes->second;
        if (valeurVar.find("nan") != std::string::npos || valeurVar.find("inf") != std::string::npos)
            valeurVar = "NA";
        ligneEntete = ligneEntete + nomVar + "\t";
        ligneDonnees = ligneDonnees + valeurVar + "\t";
    }
    ligneEntete = ligneEntete.substr(0,ligneEntete.length()-1);
    ligneDonnees = ligneDonnees.substr(0,ligneDonnees.length()-1);

    std::ifstream fichierResultTest("Resultats_journaliers.txt");

    if(fichierResultTest)  // si fichier existe deja
    {
        fichierResultTest.close();
        std::ofstream fichierResultEcrire("Resultats_journaliers.txt", std::ios::out | std::ios::app);  //déclaration du flux et ouverture du fichier
        fichierResultEcrire << ligneDonnees << std::endl;

        fichierResultEcrire.close();

    }
    else {
        std::ofstream fichierResultEcrire("Resultats_journaliers.txt", std::ios::out);
        fichierResultEcrire << ligneEntete << std::endl;
        fichierResultEcrire << ligneDonnees << std::endl;
        fichierResultEcrire.close();
    }
}

void SamaraModel::init(double /* t */,
                 const model::models::ModelParameters& parameters)
{
    Altitude = parameters.get<double>("altitude");
    ASScstr = parameters.get<double>("asscstr");
    AttenMitch = parameters.get<double>("attenmitch");
    BundHeight = parameters.get<double>("bundheight");
    Ca = parameters.get<double>("ca");
    CO2Cp = parameters.get<double>("co2cp");
    CO2Exp = parameters.get<double>("co2exp");
    CO2SlopeTr = parameters.get<double>("co2slopetr");
    CoeffAssimSla = parameters.get<double>("coeffassimsla");
    CoefficientQ10 = parameters.get<double>("coefficientq10");
    CoeffInternodeMass = parameters.get<double>("coeffinternodemass");
    CoeffInternodeNum = parameters.get<double>("coeffinternodenum");
    CoeffLeafDeath = parameters.get<double>("coeffleafdeath");
    CoeffLeafWLRatio = parameters.get<double>("coeffleafwlratio");
    CoeffPanicleMass = parameters.get<double>("coeffpaniclemass");
    CoeffPanSinkPop = parameters.get<double>("coeffpansinkpop");
    CoeffResCapacityInternode = parameters.get<double>("coeffrescapacityinternode");
    CoeffReserveSink = parameters.get<double>("coeffreservesink");
    CoeffRootMassPerVolMax = parameters.get<double>("coeffrootmasspervolmax");
    CoeffTillerDeath = parameters.get<double>("coefftillerdeath");
    CoeffTransplantingShock = parameters.get<double>("coefftransplantingshock");
    DateSemis = utils::DateTime::toJulianDayNumber(parameters.get<std::string>("datesemis"));
    DensityField = parameters.get<double>("densityfield");
    DensityNursery = parameters.get<double>("densitynursery");
    DEVcstr = parameters.get<double>("devcstr");
    DurationNursery = parameters.get<double>("durationnursery");
    EpaisseurProf = parameters.get<double>("epaisseurprof");
    EpaisseurSurf = parameters.get<double>("epaisseursurf");
    ExcessAssimToRoot = parameters.get<double>("excessassimtoroot");
    FTSWIrrig = parameters.get<double>("ftswirrig");
    HaunCritTillering = parameters.get<double>("hauncrittillering");
    HumFC = parameters.get<double>("humfc");
    HumPF = parameters.get<double>("humpf");
    HumSat = parameters.get<double>("humsat");
    IcTillering = parameters.get<double>("ictillering");
    InternodeLengthMax = parameters.get<double>("internodelengthmax");
    IrrigAuto = parameters.get<int>("irrigauto");
    IrrigAutoResume = parameters.get<double>("irrigautoresume");
    IrrigAutoStop = parameters.get<double>("irrigautostop");
    IrrigAutoTarget = parameters.get<double>("irrigautotarget");
    KcMax = parameters.get<double>("kcmax");
    KCritSterCold1 = parameters.get<double>("kcritstercold1");
    KCritSterCold2 = parameters.get<double>("kcritstercold2");
    KCritSterFtsw1 = parameters.get<double>("kcritsterftsw1");
    KCritSterFtsw2 = parameters.get<double>("kcritsterftsw2");
    KCritSterHeat1 = parameters.get<double>("kcritsterheat1");
    KCritSterHeat2 = parameters.get<double>("kcritsterheat2");
    KCritStressCold1 = parameters.get<double>("kcritstresscold1");
    KCritStressCold2 = parameters.get<double>("kcritstresscold2");
    Kdf = parameters.get<double>("kdf");
    KPar = parameters.get<double> ("kpar");
    KRespInternode = parameters.get<double>("krespinternode");
    KRespMaintLeaf = parameters.get<double>("krespmaintleaf");
    KRespMaintRoot = parameters.get<double>("krespmaintroot");
    KRespMaintSheath = parameters.get<double>("krespmaintsheath");
    KRespPanicle = parameters.get<double>("kresppanicle");
    KTempMaint = parameters.get<double>("ktempmaint");
    Latitude = parameters.get<double>("latitude");
    LeafLengthMax = parameters.get<double>("leaflengthmax");
    LifeSavingDrainage = parameters.get<double>("lifesavingdrainage");
    Mulch = parameters.get<double>("mulch");
    PanStructMassMax = parameters.get<double>("panstructmassmax");
    PercolationMax = parameters.get<double>("percolationmax");
    PEvap = parameters.get<double>("pevap");
    PFactor = parameters.get<double>("pfactor");
    Phyllo = parameters.get<double>("phyllo");
    PlantsPerHill = parameters.get<double>("plantsperhill");
    PlotDrainageDAF = parameters.get<double>("plotdrainagedaf");
    PoidsSecGrain = parameters.get<double>("poidssecgrain");
    PourcRuiss = parameters.get<double>("pourcruiss");
    PPCrit = parameters.get<double>("ppcrit");
    PPExp = parameters.get<double>("ppexp");
    PPSens = parameters.get<double>("ppsens");
    PriorityPan = parameters.get<double>("prioritypan");
    ProfRacIni = parameters.get<double>("profracini");
    RankLongestLeaf = parameters.get<double>("ranklongestleaf");
    RelMobiliInternodeMax = parameters.get<double>("relmobiliinternodemax");
    RelPhylloPhaseStemElong = parameters.get<double>("relphyllophasestemelong");
    RollingBase = parameters.get<double>("rollingbase");
    RollingSens = parameters.get<double>("rollingsens");
    RootCstr = parameters.get<double>("rootcstr");
    RootFrontMax = parameters.get<double>("rootfrontmax");
    RootPartitMax = parameters.get<double>("rootpartitmax");
    SDJBVP = parameters.get<double>("sdjbvp");
    SDJLevee = parameters.get<double>("sdjlevee");
    SDJMatu1 = parameters.get<double>("sdjmatu1");
    SDJMatu2 = parameters.get<double>("sdjmatu2");
    SDJRPR = parameters.get<double>("sdjrpr");
    SeuilPP = parameters.get<double>("seuilpp");
    SeuilRuiss = parameters.get<double>("seuilruiss");
    SlaMax = parameters.get<double>("slamax");
    SlaMin = parameters.get<double>("slamin");
    StockIniProf = parameters.get<double>("stockiniprof");
    StockIniSurf = parameters.get<double>("stockinisurf");
    TBase = parameters.get<double>("tbase");
    TempSla = parameters.get<double>("tempsla");
    TilAbility = parameters.get<double>("tilability");
    TLet = parameters.get<double>("tlim");
    TOpt1 = parameters.get<double>("topt1");
    TOpt2 = parameters.get<double>("topt2");
    Transplanting = parameters.get<double>("transplanting");
    TransplantingDepth = parameters.get<double>("transplantingdepth");
    TxAssimBVP = parameters.get<double>("txassimbvp");
    TxAssimMatu1 = parameters.get<double>("txassimmatu1");
    TxAssimMatu2 = parameters.get<double>("txassimmatu2");
    TxConversion = parameters.get<double>("txconversion");
    TxResGrain = parameters.get<double>("txresgrain");
    TxRuSurfGermi = parameters.get<double>("txrusurfgermi");
    VRacBVP = parameters.get<double>("vracbvp");
    VRacLevee = parameters.get<double>("vraclevee");
    VRacMatu1 = parameters.get<double>("vracmatu1");
    VRacMatu2 = parameters.get<double>("vracmatu2");
    VRacPSP = parameters.get<double>("vracpsp");
    VRacRPR = parameters.get<double>("vracrpr");
    WaterLoggingSens = parameters.get<double>("waterloggingsens");
    WtRatioLeafSheath = parameters.get<double>("wtratioleafsheath");
}

void SamaraModel::initCrop()
{
    NumPhase = 1;
    SommeDegresJourMaximale = SDJLevee + SDJBVP + SDJRPR + SDJMatu1 + SDJMatu2;
    SommeDegresJour = 0;
    SeuilTempPhaseSuivante = 0;
    Lai = 0;
    IcCumul = 0;
    FTSW = 1;
    Cstr = 1;
    DurPhase1 = 0;
    DurPhase2 = 0;
    DurPhase3 = 0;
    DurPhase4 = 0;
    DurPhase5 = 0;
    DurPhase6 = 0;
    TempLai = 0;
    ApexHeightGain = 0;
    ChangeNurseryStatus = 0;
    ChangePhase = 0;
    ChangeSsPhase = 0;
    CstrPhase2 = 0;
    CstrPhase3 = 0;
    CstrPhase4 = 0;
    CstrPhase5 = 0;
    CstrPhase6 = 0;
    CumCstrPhase2 = 0;
    CumCstrPhase3 = 0;
    CumCstrPhase4 = 0;
    CumCstrPhase5 = 0;
    CumCstrPhase6 = 0;
    CumFtswPhase2 = 0;
    CumFtswPhase3 = 0;
    CumFtswPhase4 = 0;
    CumFtswPhase5 = 0;
    CumFtswPhase6 = 0;
    CumIcPhase2 = 0;
    CumIcPhase3 = 0;
    CumIcPhase4 = 0;
    CumIcPhase5 = 0;
    CumIcPhase6 = 0;
    DAF = 0;
    DemLeafAreaPlant = 0;
    DemPanicleFillPop = 0;
    DemStructInternodePlant = 0;
    DemStructInternodePop = 0;
    DemStructLeafPlant = 0;
    DemStructLeafPop = 0;
    DemStructPaniclePlant = 0;
    DemStructPaniclePop = 0;
    DemStructRootPlant = 0;
    DemStructRootPop = 0;
    DemStructSheathPop = 0;
    DemStructTotPop = 0;
    FloodWaterGain = 0;
    FtswPhase2 = 0;
    FtswPhase3 = 0;
    FtswPhase4 = 0;
    FtswPhase5 = 0;
    FtswPhase6 = 0;
    GainRootSystSoilSurfPop = 0;
    GainRootSystVolPop = 0;
    GrowthDryMatPop = 0;
    GrowthResInternodePop = 0;
    GrowthStructDeficit = 0;
    GrowthStructInternodePop = 0;
    GrowthStructLeafPop = 0;
    GrowthStructPaniclePop = 0;
    GrowthStructRootPop = 0;
    GrowthStructSheathPop = 0;
    GrowthStructTotPop = 0;
    HaunGain = 0;
    IcPhase2 = 0;
    IcPhase3 = 0;
    IcPhase4 = 0;
    IcPhase5 = 0;
    IcPhase6 = 0;
    IncreaseResInternodePop = 0;
    Kcl = 0;
    Kr = 0;
    MobiliLeafDeath = 0;
    NbDaysSinceGermination = 0;
    NurseryStatus = 0;
    PanicleFilDeficit = 0;
    PanicleFilPop = 0;
    PanicleSinkPop = 0;
    PanStructMass = 0;
    PlantLeafNumNew = 0;
    ResInternodeMobiliDay = 0;
    ResInternodeMobiliDayPot = 0;
    RootFrontOld = 0;
    RootSystSoilSurfPop = 0;
    RootSystSoilSurfPopOld = 0;
    RootSystVolPop = 0;
    RootSystVolPopOld = 0;
    SDJCorPhase4 = 0;
}

} // namespace samara
