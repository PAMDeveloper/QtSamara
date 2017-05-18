/**
 * @file utils/ParametersReader.cpp
 * @author The Samara Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2013-2014 ULCO http://www.univ-littoral.fr
 * Copyright (C) 2013-2014 INRA http://www.inra.fr
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

#include "DateTime.hpp"
//#include <iostream>
#include <algorithm>
#include "ParametersReader.hpp"
#include <iso646.h>

namespace utils {

void ParametersReader::loadFromDatabase(const std::string& id,
                                        samara::ModelParameters& parameters)
{
    PGconn* connection(
                utils::Connections::connection(
                    "samara", "host=localhost port=5432 dbname=samara user=user_samara password=samarapassword"));
    if (PQstatus(connection) != CONNECTION_OK)
    {
        std::cout << "Connection to database failed: %s" << PQerrorMessage(connection);
        PQfinish(connection);
        exit(1);
    } else {
        std::cout << "DB connection initialized." << std::endl;
    }

    load_simulation(id, connection, parameters);
    load_variety(parameters.getString("idvariete"), connection, parameters);
    load_itineraire_technique( parameters.getString("iditinerairetechnique"), connection, parameters);
    load_plot(parameters.getString("idparcelle"), connection, parameters);
    load_station(parameters.getString("codestationmeteo"), connection, parameters);
    load_meteo(connection, parameters);
    std::cout << "Simulation " << id << ":" << std::endl;
    std::cout << " - begin = " << parameters.getString("datedebut")
              << std::endl;
    std::cout << " - end = " << parameters.getString("datefin")
              << std::endl;
    std::cout << " - variety = " << parameters.getString("idvariete")
              << std::endl;
    std::cout << " - itineraire technique = "
              << parameters.getString("iditinerairetechnique")
              << std::endl;
    std::cout << " - date semis = "
              << parameters.getString("datesemis")
              << std::endl;
    std::cout << " - plot = " << parameters.getString("idparcelle")
              << std::endl;
    std::cout << " - site = " << parameters.getString("idsite")
              << std::endl;
    std::cout << " - station = "
              << parameters.getString("codestationmeteo")
              << std::endl;
    std::cout << " - type_soil = "
              << parameters.getString("codetypesol")
              << std::endl;
    std::cout << " - model = "
              << parameters.getString("idmodele")
              << std::endl;
    std::cout << " - meteo = "
              << parameters.meteoValues.size() << " values"
              << std::endl;

}

void ParametersReader::load_meteo(PQConnector connection, samara::ModelParameters &parameters){
    std::string beginDate;
    std::string endDate;
    double begin;
    double end;
    unsigned int beginYear;
    unsigned int endYear;
    int nbLigneResultMeteo,nbLigneResultPluie,compteurLigneResult;

    begin = artis::utils::DateTime::toJulianDayNumber(parameters.getString("datedebut")); //beginDate
    end = artis::utils::DateTime::toJulianDayNumber(parameters.getString("datefin")); //endDate
    beginYear = artis::utils::DateTime::year(begin);
    endYear = artis::utils::DateTime::year(end);

    std::ostringstream convertisseurAnnee;
    std::string yearString;

    for (unsigned int year = beginYear; year <= endYear; year++) {
        convertisseurAnnee<<year;
        yearString = convertisseurAnnee.str();
        std::string requestMeteorology = "SELECT * FROM meteo WHERE codestation='" +
                parameters.getString("codestationmeteo") + "' AND jour >='" + yearString
                + "-01-01' AND jour <='" + yearString + "-12-31' ORDER BY jour";

        PGresult *resultMeteorology = PQexec(connection,requestMeteorology.c_str());
        if (PQresultStatus(resultMeteorology) != PGRES_TUPLES_OK) {
            std::cout << "Error: " << PQerrorMessage(connection) << std::endl;
        }

        std::string requestRainfall = "SELECT * FROM precipitation WHERE codestation='" +
                parameters.getString("codestationpluie") +"' AND jour >='" + yearString
                + "-01-01' AND jour <='" + yearString + "-12-31' ORDER BY jour";

        PGresult *resultRainfall = PQexec(connection,requestRainfall.c_str());
        if (PQresultStatus(resultRainfall) != PGRES_TUPLES_OK){
            std::cout << "Error: " << PQerrorMessage(connection) << std::endl;
        }

        nbLigneResultMeteo = PQntuples(resultMeteorology);
        nbLigneResultPluie = PQntuples(resultRainfall);

        if (nbLigneResultMeteo>0 && nbLigneResultPluie>0){
            bool finished = false;
            compteurLigneResult = 0;
            while (not finished) {
                std::string day;
                double t;
				day = PQgetvalue(resultMeteorology,compteurLigneResult,1);
                t = artis::utils::DateTime::toJulianDayNumber(day);
                if (t >= begin && t <= end) {
                   
                        parameters.meteoValues.push_back(
                                    samara::Climate(
                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,2)),
                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,3)),
                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,4)),
                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,5)),
                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,6)),
                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,7)),
                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,8)),
                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,9)),
                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,10)),
//                                        std::stod(PQgetvalue(resultMeteorology,compteurLigneResult,11)),
                                        std::stod(PQgetvalue(resultRainfall,compteurLigneResult,2))
                                        ));

                    //}
                }
                compteurLigneResult++;
                if(compteurLigneResult == nbLigneResultMeteo || compteurLigneResult == nbLigneResultPluie){
                    finished = true;
                }
            }
        }
    }
}

void ParametersReader::load_data(//pqxx::connection& connection,
                                 PGconn* connection,
                                 const std::string& table,
                                 const std::string& key,
                                 const std::string& value,
                                 const std::vector < std::string >& names,
                                 samara::ModelParameters& parameters)
{
    //    try {
    //pqxx::work action(connection);
    //pqxx::result result = action.exec(

	std::string query = "SELECT * FROM " + table + " WHERE " + key + "='" + value + "'";
    PGresult* result = PQexec(connection,
                              query.c_str());
    if (PQresultStatus(result) != PGRES_TUPLES_OK){
        std::cout << "Error: " << PQerrorMessage(connection) << std::endl;
    }
    if (PQntuples(result)>0) {
        //            for (pqxx::result::tuple::const_iterator it = result[0].begin();
        //                 it != result[0].end(); ++it) { //it correspond a un champ field
        //boucle sur les colonnes
        for(int compteur = 0;compteur<PQnfields(result);compteur++){
            //                std::vector < std::string >::const_iterator itv =
            //                    std::find(names.begin(), names.end(), it->name()); //recupere le nom de la colonne du curseur

            std::vector < std::string >::const_iterator itv = //iterateur de vecteur de string
                    std::find(names.begin(), names.end(), PQfname(result,compteur)); //indique l indice ou se trouve le champ
            //dans le vecteur des noms de champ
            if (itv != names.end()) { //si le nom du champ a ete trouve
                //if (it->type() == 1043) { //test sur le type de la colonne, ici si c est une chaine de caract.
                if((PQftype(result,compteur) == 1043) || (PQftype(result,compteur) == 1082)){
                    parameters.setString(*itv,  std::string(PQgetvalue(result,0,compteur)));
                }
                else {
                    parameters.setDouble(*itv, atof(PQgetvalue(result,0,compteur)));
                }
            } else {
                //parameters.set < double >(it->name(), 0.);

                // parameters.set < double >(PQfname(result,compteur), 0.);

            }
        }
    }
    //   } catch (pqxx::sql_error e) {
    //       std::cout << "Error: " << e.query() << std::endl;
    //   }
}

void ParametersReader::load_variety(
        const std::string& id,
        //pqxx::connection& connection,
        PGconn* connection,
        samara::ModelParameters& parameters)
{
    std::vector < std::string > names;
    std::string nameParameterArray[] = {
        "asscstr",
                "attenmitch",
                "ca",
                "co2cp",
                "co2exp",
                "co2slopetr",
                "coeffassimsla",
                "coefficientq10",
                "coeffinternodemass",
                "coeffinternodenum",
                "coeffleafdeath",
                "coeffleafwlratio",
                "coeffpaniclemass",
                "coeffpansinkpop",
                "coeffrescapacityinternode",
                "coeffreservesink",
                "coeffrootmasspervolmax",
                "coefftillerdeath",
                "devcstr",
                "excessassimtoroot",
                "ftswirrig",
                "hauncrittillering",
                "ictillering",
                "internodelengthmax",
                "irrigautoresume",
                "irrigautostop",
                "kcmax",
                "kcritstercold1",
                "kcritstercold2",
                "kcritsterftsw1",
                "kcritsterftsw2",
                "kcritsterheat1",
                "kcritsterheat2",
                "kcritstresscold1",
                "kcritstresscold2",
                "kdf",
                "krespinternode",
                "krespmaintleaf",
                "krespmaintroot",
                "krespmaintsheath",
                "kresppanicle",
                "ktempmaint",
                "leaflengthmax",
                "panstructmassmax",
                "pfactor",
                "phyllo",
                "poidssecgrain",
                "ppcrit",
                "ppexp",
                "ppsens",
                "prioritypan",
                "ranklongestleaf",
                "relmobiliinternodemax",
                "relphyllophasestemelong",
                "rollingbase",
                "rollingsens",
                "rootcstr",
                "rootfrontmax",
                "rootpartitmax",
                "sdjbvp",
                "sdjlevee",
                "sdjmatu1",
                "sdjmatu2",
                "sdjrpr",
                "seuilpp",
                "slamax",
                "slamin",
                "tbase",
                "tempsla",
                "tilability",
                "tlim",
                "topt1",
                "topt2",
                "transplantingdepth",
                "txassimbvp",
                "txassimmatu1",
                "txassimmatu2",
                "txconversion",
                "txresgrain",
                "txrusurfgermi",
                "vracbvp",
                "vraclevee",
                "vracmatu1",
                "vracmatu2",
                "vracpsp",
                "vracrpr",
                "waterloggingsens",
                "wtratioleafsheath"
    };
    size_t arraySize = sizeof(nameParameterArray)/sizeof(std::string);
    for (size_t i =0;i<arraySize;i++)
        names.push_back(nameParameterArray[i]);

    //load_data(connection, "variety", "Id", id, names, parameters);
    load_data(connection, "variete", "idvariete", id, names, parameters);
}

void ParametersReader::load_itineraire_technique(
        const std::string& id,
        //pqxx::connection& connection,
        PGconn* connection,
        samara::ModelParameters& parameters)
{
    std::vector < std::string > names;
    std::string nameParameterArray[] = {
        //        "bundheight",
        //        "coefftransplantingshock",
        //        "datesemis",
        //        "densityfield",
        //        "densitynursery",
        //        "durationnursery",
        //        "ftswirrig",
        //        "irrigauto",
        //        "irrigautoresume",
        //        "irrigautostop",
        //        "irrigautotarget",
        //        "lifesavingdrainage",
        //        "mulch",
        //        "plantsperhill",
        //        "plotdrainagedaf",
        //        "profracini",
        //        "transplanting",
        //        "transplantingdepth"
        "datesemis",
        "profracini",
        "densite",
        "laiini",
        "mulch",
        "degreesjourini",
        "biomassini",
        "seuileausemis",
        "nbjtestsemis",
        "seuilstresssemis",
        "plantsperhill",
        "bundheight",
        "lifesavingdrainage",
        "plotdrainagedaf",
        "irrigauto",
        "irrigautotarget",
        "transplanting",
        "durationnursery",
        "densitynursery",
        "densityfield",
        "coefftransplantingshock",
        "irrigautostop",
        "irrigautoresume",
        "ftswirrig",
        "transplantingdepth",
    };
    size_t arraySize = sizeof(nameParameterArray)/sizeof(std::string);
    for (size_t i =0;i<arraySize;i++)
        names.push_back(nameParameterArray[i]);
    load_data(connection, "itinerairetechnique", "iditinerairetechnique", id, names, parameters);
}

void ParametersReader::load_plot(
        const std::string& id,
        //pqxx::connection& connection,
        PGconn* connection,
        samara::ModelParameters& parameters)
{
    std::vector < std::string > names;
    std::string nameParameterArray[] = {
        //        "Ca", "EpaisseurProf", "EpaisseurSurf", "StockIniProf", "StockIniSurf",
        //        "IdTypeSol"
        "nom","stockiniprof", "stockinisurf","epaisseurprof", "epaisseursurf",
        "tankminirrig","tankmaxirrig","tankcapacity","volrelmacropores","ca",
        "codetypesol","idsite"

    };
    size_t arraySize = sizeof(nameParameterArray)/sizeof(std::string);
    for (size_t i =0;i<arraySize;i++)
        names.push_back(nameParameterArray[i]);
    //load_data(connection, "plot", "Id", id, names, parameters);

    load_data(connection, "parcelle", "idparcelle", id, names, parameters);

    load_site(parameters.getString("idsite"), connection, parameters);

    load_type_soil(parameters.getString("codetypesol"), connection,parameters);
}

void ParametersReader::load_simulation(
        const std::string& id,
        //pqxx::connection& connection,
        PGconn* connection,
        samara::ModelParameters& parameters)
{
    std::vector < std::string > names;
    //std::vector < std::string > names = { "IdSite","IdPlot", "IdVariety",
    //                                            "IdItineraireTechnique", "IdModele",
    //                                            "BeginDate", "EndDate" };
    std::string nameParameterArray[] = { "codestationmeteo","codestationpluie", "datedebut",
                                         "datefin","anneedebut","anneefin","simulationyearnumber",
                                         "descriptionforcage","idtypeet","iditinerairetechnique",
                                         "idparcelle","iddossier","idvariete","idmodele" };

    size_t arraySize = sizeof(nameParameterArray)/sizeof(std::string);
    for (size_t i =0;i<arraySize;i++)
        names.push_back(nameParameterArray[i]);
    //load_data(connection, "simulation", "Id", id, names, parameters);
    load_data(connection, "simulation", "idsimulation", id, names, parameters);
}

void ParametersReader::load_site(
        const std::string& id,
        //pqxx::connection& connection,
        PGconn* connection,
        samara::ModelParameters& parameters)
{
    std::vector < std::string > names;
    //    std::vector < std::string > names = { "SeuilPluie", "Kpar",
    //                                          "CodeStationMeteo",
    //                                          "CodeStationPluie" };
    std::string nameParameterArray[] = { "seuilpluie", "kpar",
                                         "codepays" };

    size_t arraySize = sizeof(nameParameterArray)/sizeof(std::string);
    for (size_t i =0;i<arraySize;i++)
        names.push_back(nameParameterArray[i]);
    //load_data(connection, "site", "Id", id, names, parameters);
    load_data(connection, "site", "idsite", id, names, parameters);
}

void ParametersReader::load_station(
        const std::string& id,
        //pqxx::connection& connection,
        PGconn* connection,
        samara::ModelParameters& parameters)
{
    std::vector < std::string > names;
    //std::vector < std::string > names = { "Altitude", "Latitude" };
    std::string nameParameterArray[] = { "nom","altitude", "latitude","longitude","codetypestation" };

    size_t arraySize = sizeof(nameParameterArray)/sizeof(std::string);
    for (size_t i =0;i<arraySize;i++)
        names.push_back(nameParameterArray[i]);
    //load_data(connection, "station", "Code", id, names, parameters);
    load_data(connection, "station", "codestation", id, names, parameters);
}

void ParametersReader::load_type_soil(
        const std::string& id,
        //pqxx::connection& connection,
        PGconn* connection,
        samara::ModelParameters& parameters)
{
    std::vector < std::string > names;
    std::string nameParameterArray[] = {
        //        "HumFC", "HumPF", "HumSat", "PercolationMax", "PourcRuiss",
        //        "SeuilRuiss", "PEvap"
        "humfc", "humpf", "humsat", "percolationmax", "pourcruiss",
        "seuilruiss", "pevap","ru","humcr"
    };

    size_t arraySize = sizeof(nameParameterArray)/sizeof(std::string);
    for (size_t i =0;i<arraySize;i++)
        names.push_back(nameParameterArray[i]);
    //load_data(connection, "typeSoil", "Id", id, names, parameters);
    load_data(connection, "typesol", "codetypesol", id, names, parameters);
}

} // namespace utils
