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

#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <samara/utils/ParametersReader.hpp>
#include <samara/utils/DateTime.hpp>

#include <iostream>

namespace utils {

void ParametersReader::loadFromDatabase(const std::string& id,
                                        model::models::ModelParameters& parameters)
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
    load_variety(parameters.get < std::string >("idvariete"), connection, parameters);
    load_itineraire_technique( parameters.get < std::string >("iditinerairetechnique"), connection, parameters);
    load_plot(parameters.get < std::string >("idparcelle"), connection, parameters);
    load_station(parameters.get < std::string >("codestationmeteo"), connection, parameters);
    load_meteo(connection, parameters);
    std::cout << "Simulation " << id << ":" << std::endl;
    std::cout << " - begin = " << parameters.get < std::string >("datedebut")
              << std::endl;
    std::cout << " - end = " << parameters.get < std::string >("datefin")
              << std::endl;
    std::cout << " - variety = " << parameters.get < std::string >("idvariete")
              << std::endl;
    std::cout << " - itineraire technique = "
              << parameters.get < std::string >("iditinerairetechnique")
              << std::endl;
    std::cout << " - date semis = "
              << parameters.get < std::string >("datesemis")
              << std::endl;
    std::cout << " - plot = " << parameters.get < std::string >("idparcelle")
              << std::endl;
    std::cout << " - site = " << parameters.get < std::string >("idsite")
              << std::endl;
    std::cout << " - station = "
              << parameters.get < std::string >("codestationmeteo")
              << std::endl;
    std::cout << " - type_soil = "
              << parameters.get < std::string >("codetypesol")
              << std::endl;
    std::cout << " - model = "
              << parameters.get < std::string >("idmodele")
              << std::endl;
    std::cout << " - meteo = "
              << parameters.meteoValues.size() << " values"
              << std::endl;

}

void ParametersReader::load_meteo(PQConnector connection, model::models::ModelParameters &parameters){
    std::string beginDate;
    std::string endDate;
    double begin;
    double end;
    unsigned int beginYear;
    unsigned int endYear;
    int nbLigneResultMeteo,nbLigneResultPluie,compteurLigneResult;

    begin = utils::DateTime::toJulianDayNumber(parameters.get < std::string >("datedebut")); //beginDate
    end = utils::DateTime::toJulianDayNumber(parameters.get < std::string >("datefin")); //endDate
    beginYear = utils::DateTime::year(begin);
    endYear = utils::DateTime::year(end);

    std::ostringstream convertisseurAnnee;
    std::string yearString;

    for (unsigned int year = beginYear; year <= endYear; year++) {
        convertisseurAnnee<<year;
        yearString = convertisseurAnnee.str();
        std::string requestMeteorology = "SELECT * FROM meteo WHERE codestation='" +
                parameters.get < std::string >("codestationmeteo") + "' AND jour >='" + yearString
                + "-01-01' AND jour <='" + yearString + "-12-31' ORDER BY jour";

        PGresult *resultMeteorology = PQexec(connection,requestMeteorology.c_str());
        if (PQresultStatus(resultMeteorology) != PGRES_TUPLES_OK){
            std::cout << "Error: " << PQerrorMessage(connection) << std::endl;
        }

        std::string requestRainfall = "SELECT * FROM precipitation WHERE codestation='" +
                parameters.get < std::string >("codestationpluie") +"' AND jour >='" + yearString
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

                try {
                    day = PQgetvalue(resultMeteorology,compteurLigneResult,1);
                }
                catch (boost::bad_lexical_cast const& e){
                    std::cout << "Error meteo init recup jour: " << e.what() << "\n";
                }

                t = utils::DateTime::toJulianDayNumber(day);
                if (t >= begin && t <= end) {
                    try {
//                        std::cout << "*********** day " << day << "*************" << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,2) << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,3) << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,4) << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,5) << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,6) << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,7) << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,8) << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,9) << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,10) << std::endl;
//                        std::cout << PQgetvalue(resultMeteorology,compteurLigneResult,11) << std::endl;
//                        std::cout << PQgetvalue(resultRainfall,compteurLigneResult,2) << std::endl;
                        parameters.meteoValues.push_back(
                                    model::models::Climate(
                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,2)),
                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,3)),
                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,4)),
                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,5)),
                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,6)),
                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,7)),
                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,8)),
                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,9)),
                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,10)),
//                                        boost::lexical_cast < double >(PQgetvalue(resultMeteorology,compteurLigneResult,11)),
                                        boost::lexical_cast < double >(PQgetvalue(resultRainfall,compteurLigneResult,2))
                                        ));

                    }
                    catch(boost::bad_lexical_cast const& e)
                    {
                        std::cout << "Error meteo init recup valeur: " << e.source_type().name() << "\n";
                        std::cout << "Error meteo init recup valeur: " << e.target_type().name() << "\n";
                        std::cout << "Error meteo init recup valeur: " << e.what() << "\n";
                    }
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
                                 model::models::ModelParameters& parameters)
{
    //    try {
    //pqxx::work action(connection);
    //pqxx::result result = action.exec(
    PGresult* result = PQexec(connection,
                              (boost::format("SELECT * FROM \"%1%\" WHERE \"%2%\"='%3%'") %
                               table % key % value).str().c_str());
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
                    parameters.set < std::string >( //affecte a la map parameters la cle nomdeparam avec sa valeur
                                                    //                            *itv,  boost::lexical_cast < std::string >(*it));
                                                    *itv,  std::string(PQgetvalue(result,0,compteur)));
                }
                else {
                    parameters.set < double >(
                                //                            *itv, boost::lexical_cast < double >(*it));
                                *itv, atof(PQgetvalue(result,0,compteur)));
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
        model::models::ModelParameters& parameters)
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
        model::models::ModelParameters& parameters)
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
        model::models::ModelParameters& parameters)
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

    load_site(parameters.get < std::string >("idsite"), connection, parameters);

    load_type_soil(parameters.get < std::string >("codetypesol"), connection,parameters);
}

void ParametersReader::load_simulation(
        const std::string& id,
        //pqxx::connection& connection,
        PGconn* connection,
        model::models::ModelParameters& parameters)
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
        model::models::ModelParameters& parameters)
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
        model::models::ModelParameters& parameters)
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
        model::models::ModelParameters& parameters)
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

// JSON
void ParametersReader::loadFromTree(
        const boost::property_tree::ptree& tree,
        model::models::ModelParameters& parameters)
{
    using boost::property_tree::ptree;

    ptree::const_iterator it_simulation = tree.end();
    ptree::const_iterator it_variety = tree.end();
    ptree::const_iterator it_itineraire_technique = tree.end();
    ptree::const_iterator it_plot = tree.end();
    ptree::const_iterator it_site = tree.end();
    ptree::const_iterator it_station = tree.end();
    ptree::const_iterator it_type_soil = tree.end();

    for (ptree::const_iterator it = tree.begin(); it != tree.end(); ++it) {
        if (it->first == "simulation") {
            it_simulation = it;
        } else if (it->first == "variety") {
            it_variety = it;
        } else if (it->first == "itineraire_technique") {
            it_itineraire_technique = it;
        } else if (it->first == "plot") {
            it_plot = it;
        } else if (it->first == "site") {
            it_site = it;
        } else if (it->first == "station") {
            it_station = it;
        } else if (it->first == "type_soil") {
            it_type_soil = it;
        }
    }
    load_simulation(it_simulation, parameters);
    load_variety(it_variety, parameters);
    load_itineraire_technique(it_itineraire_technique, parameters);
    load_plot(it_plot, parameters);
    load_site(it_site, parameters);
    load_station(it_station, parameters);
    load_type_soil(it_type_soil, parameters);


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


    load_meteo(connection, parameters);
//    std::cout << "Simulation " << id << ":" << std::endl;
    std::cout << " - begin = " << parameters.get < std::string >("datedebut")
              << std::endl;
    std::cout << " - end = " << parameters.get < std::string >("datefin")
              << std::endl;
    std::cout << " - variety = " << parameters.get < std::string >("idvariete")
              << std::endl;
    std::cout << " - itineraire technique = "
              << parameters.get < std::string >("iditinerairetechnique")
              << std::endl;
    std::cout << " - date semis = "
              << parameters.get < std::string >("datesemis")
              << std::endl;
    std::cout << " - plot = " << parameters.get < std::string >("idparcelle")
              << std::endl;
    std::cout << " - site = " << parameters.get < std::string >("idsite")
              << std::endl;
    std::cout << " - station = "
              << parameters.get < std::string >("codestationmeteo")
              << std::endl;
    std::cout << " - type_soil = "
              << parameters.get < std::string >("codetypesol")
              << std::endl;
    std::cout << " - model = "
              << parameters.get < std::string >("idmodele")
              << std::endl;
    std::cout << " - meteo = "
              << parameters.meteoValues.size() << " values"
              << std::endl;
}

void ParametersReader::loadFromJSON(
        const std::string& json, model::models::ModelParameters& parameters)
{
    using boost::property_tree::ptree;

    ptree tree;
    std::stringstream ss;

    ss << json;
    read_json(ss, tree);
    loadFromTree(tree, parameters);
}

void ParametersReader::load_data(
        boost::property_tree::ptree::const_iterator& it,
        const std::vector < std::string >& names,
        model::models::ModelParameters& parameters)
{
    using boost::property_tree::ptree;

    for (ptree::const_iterator it2 = it->second.begin();
         it2 != it->second.end(); ++it2) {
        std::vector < std::string >::const_iterator itv =
                std::find(names.begin(), names.end(), it2->first);

        if (itv != names.end()) {
            try {
                parameters.set < double >(
                            *itv, it2->second.get_value < double >());
            } catch (...) {
                parameters.set < std::string >(
                            *itv, it2->second.get_value < std::string >());
            }
        }


    }
}

void ParametersReader::load_variety(
        boost::property_tree::ptree::const_iterator& it,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = {
        "ASScstr",
        "AttenMitch",
        "Ca",
        "CO2Cp",
        "CO2Exp",
        "CO2SlopeTr",
        "CoeffAssimSla",
        "CoefficientQ10",
        "CoeffInternodeMass",
        "CoeffInternodeNum",
        "CoeffLeafDeath",
        "CoeffLeafWLRatio",
        "CoeffPanicleMass",
        "CoeffPanSinkPop",
        "CoeffResCapacityInternode",
        "CoeffReserveSink",
        "CoeffRootMassPerVolMax",
        "CoeffTillerDeath",
        "DEVcstr",
        "ExcessAssimToRoot",
        "FTSWIrrig",
        "HaunCritTillering",
        "IcTillering",
        "InternodeLengthMax",
        "IrrigAutoResume",
        "IrrigAutoStop",
        "KcMax",
        "KCritSterCold1",
        "KCritSterCold2",
        "KCritSterFtsw1",
        "KCritSterFtsw2",
        "KCritSterHeat1",
        "KCritSterHeat2",
        "KCritStressCold1",
        "KCritStressCold2",
        "Kdf",
        "KRespInternode",
        "KRespMaintLeaf",
        "KRespMaintRoot",
        "KRespMaintSheath",
        "KRespPanicle",
        "KTempMaint",
        "LeafLengthMax",
        "PanStructMassMax",
        "PFactor",
        "Phyllo",
        "PoidsSecGrain",
        "PPCrit",
        "PPExp",
        "PPSens",
        "PriorityPan",
        "RankLongestLeaf",
        "RelMobiliInternodeMax",
        "RelPhylloPhaseStemElong",
        "RollingBase",
        "RollingSens",
        "RootCstr",
        "RootFrontMax",
        "RootPartitMax",
        "SDJBVP",
        "SDJLevee",
        "SDJMatu1",
        "SDJMatu2",
        "SDJRPR",
        "SeuilPP",
        "SlaMax",
        "SlaMin",
        "TBase",
        "TempSla",
        "TilAbility",
        "TLet",
        "TOpt1",
        "TOpt2",
        "TransplantingDepth",
        "TxAssimBVP",
        "TxAssimMatu1",
        "TxAssimMatu2",
        "TxConversion",
        "TxResGrain",
        "TxRuSurfGermi",
        "VRacBVP",
        "VRacLevee",
        "VRacMatu1",
        "VRacMatu2",
        "VRacPSP",
        "VRacRPR",
        "WaterLoggingSens",
        "WtRatioLeafSheath"
    };

    load_data(it, names, parameters);
}

void ParametersReader::load_itineraire_technique(
        boost::property_tree::ptree::const_iterator& it,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = {
        "BundHeight",
        "CoeffTransplantingShock",
        "DateSemis",
        "DensityField",
        "DensityNursery",
        "DurationNursery",
        "FTSWIrrig",
        "IrrigAuto",
        "IrrigAutoResume",
        "IrrigAutoStop",
        "IrrigAutoTarget",
        "LifeSavingDrainage",
        "Mulch",
        "PlantsPerHill",
        "PlotDrainageDAF",
        "ProfRacIni",
        "Transplanting",
        "TransplantingDepth"
    };

    load_data(it, names, parameters);
}

void ParametersReader::load_plot(
        boost::property_tree::ptree::const_iterator& it,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = {
        "Ca", "EpaisseurProf", "EpaisseurSurf", "StockIniProf", "StockIniSurf",
        "IdTypeSol"
    };

    load_data(it, names, parameters);
}

void ParametersReader::load_simulation(
        boost::property_tree::ptree::const_iterator& it,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = { "IdSite","IdPlot", "IdVariety",
                                          "IdItineraireTechnique", "IdModele",
                                          "BeginDate", "EndDate" };

    load_data(it, names, parameters);
}

void ParametersReader::load_site(
        boost::property_tree::ptree::const_iterator& it,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = { "SeuilPluie", "Kpar",
                                          "CodeStationMeteo",
                                          "CodeStationPluie" };

    load_data(it, names, parameters);
}

void ParametersReader::load_station(
        boost::property_tree::ptree::const_iterator& it,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = { "Altitude", "Latitude" };

    load_data(it, names, parameters);
}

void ParametersReader::load_type_soil(
        boost::property_tree::ptree::const_iterator& it,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = {
        "HumFC", "HumPF", "HumSat", "PercolationMax", "PourcRuiss",
        "SeuilRuiss", "PEvap"
    };

    load_data(it, names, parameters);
}


} // namespace utils

///**
// * @file samara/utils/ParametersReader.cpp
// * @author The Samara Development Team
// * See the AUTHORS file
// */

///*
// * Copyright (C) 2013-2017 ULCO http://www.univ-littoral.fr
// * Copyright (C) 2013-2017 INRA http://www.inra.fr
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program.  If not, see <http://www.gnu.org/licenses/>.
// */

//#include <boost/format.hpp>
//#include <boost/property_tree/json_parser.hpp>
//#include <iostream>
//#include <samara/utils/ParametersReader.hpp>
//#include <samara/utils/DateTime.hpp>

//namespace utils {

//void ParametersReader::loadFromDatabase(
//        const std::string& id, model::models::ModelParameters& parameters)
//{
//#if 1
//    PQConnector connection =
//            PQconnectdb("port='5432' hostaddr='127.0.0.1' dbname='samara' user='user_samara' password='toto'");
//#else
//    PQConnector connection(
//                utils::Connections::connection(
//                    "samara", "dbname=samara user=user_samara password=toto"));
//#endif

//    //Eric Ramat deprecated version
////    load_simulation(id, connection, parameters);
////    load_variety(parameters.get < std::string >("IdVariety"), connection, parameters);
////    load_itineraire_technique(parameters.get < std::string >("IdItineraireTechnique"), connection, parameters);
////    load_plot(parameters.get < std::string >("IdPlot"), connection, parameters);
////    load_site(parameters.get < std::string >("IdSite"), connection, parameters);
////    load_station(parameters.get < std::string >("CodeStationMeteo"), connection, parameters);
////    load_type_soil(parameters.get < std::string >("IdTypeSol"), connection, parameters);

//    //Gregory Aguilar version
//    load_simulation(id, connection, parameters);
//    load_variety(parameters.getStringVal("idvariete"), connection, parameters);
//    load_itineraire_technique(parameters.getStringVal("iditinerairetechnique"), connection,parameters);
//    load_plot(parameters.getStringVal("idparcelle"), connection, parameters);
//    load_station(parameters.getStringVal("codestationmeteo"), connection, parameters);


//    /*    std::cout << "Simulation " << id << ":" << std::endl;
//    std::cout << " - begin = " << parameters.get < std::string >("BeginDate")
//              << std::endl;
//    std::cout << " - end = " << parameters.get < std::string >("EndDate")
//              << std::endl;
//    std::cout << " - variety = " << parameters.get < std::string >("IdVariety")
//              << std::endl;
//    std::cout << " - itineraire technique = "
//              << parameters.get < std::string >("IdItineraireTechnique")
//              << std::endl;
//    std::cout << " - date semis = "
//              << parameters.get < std::string >("DateSemis")
//              << std::endl;
//    std::cout << " - plot = " << parameters.get < std::string >("IdPlot")
//              << std::endl;
//    std::cout << " - site = " << parameters.get < std::string >("IdSite")
//              << std::endl;
//    std::cout << " - station = "
//              << parameters.get < std::string >("CodeStationMeteo")
//              << std::endl;
//    std::cout << " - type_soil = "
//              << parameters.get < std::string >("IdTypeSol")
//              << std::endl;
//    std::cout << " - model = "
//              << parameters.get < std::string >("IdModele")
//              << std::endl; */

//}



//void load_meteo(PQConnector connection, model::models::ModelParameters &parameters){
//#if 1
//    std::string beginDate;
//    std::string endDate;
//    double begin;
//    double end;
//    unsigned int beginYear;
//    unsigned int endYear;

//    utils::DateTime::format_date(parameters.get < std::string >("BeginDate"),
//                                 beginDate);
//    utils::DateTime::format_date(parameters.get < std::string >("EndDate"),
//                                 endDate);
//    begin = utils::DateTime::toJulianDayNumber(beginDate);
//    end = utils::DateTime::toJulianDayNumber(endDate);
//    beginYear = utils::DateTime::year(begin);
//    endYear = utils::DateTime::year(end);


//    //    PGresult* result = PQexec(
//    //                connection,
//    //                (boost::format("SELECT * FROM \"%1%\" WHERE \"%2%\"=\"%3%\"") %
//    //                 table % key % value).str().c_str()
//    //                );
//    //    if (PQresultStatus(result) != PGRES_TUPLES_OK)
//    //        std::cout << "Error: " << PQerrorMessage(connection) << std::endl;


//    //    for (int i = 0; i < PQnfields(result); i++) {
//    //        std::vector < std::string >::const_iterator itv = std::find(names.begin(), names.end(),
//    //                                                                    PQfname(result, i));

//    //        if (itv != names.end()) {
//    //            if ((PQftype(result, i) == 1043) || (PQftype(result, i) == 1082))
//    //                parameters.set <std::string>(*itv, PQgetvalue(result, 0, i));

//    //            else
//    //                parameters.set<double>(*itv, boost::lexical_cast<double>(PQgetvalue(result, 0, i)));

//    //        }
//    //        else
//    //            parameters.set < double >(PQfname(result, i), 0.);
//    //    }


//    for (unsigned int year = beginYear; year <= endYear; year++) {
//        std::string requestMeteorology =
//                (boost::format("SELECT * FROM \"meteorology\" "         \
//                               "WHERE \"CodeStation\"='%1%' AND "       \
//                               "\"Jour\" LIKE \'%%%2%%%\'") %
//                 parameters.get < std::string >("CodeStationMeteo") %
//                 year).str();

//        PGresult* resultMeteorology = PQexec(connection, requestMeteorology.c_str());
//        if (PQresultStatus(resultMeteorology) != PGRES_TUPLES_OK) std::cout << "Error: " << PQerrorMessage(connection) << std::endl;

//        std::string requestRainfall =
//                (boost::format("SELECT * FROM \"rainfall\" "            \
//                               "WHERE \"CodeStation\"='%1%' AND "       \
//                               "\"Jour\" LIKE \'%%%2%%%\'") %
//                 parameters.get < std::string >("CodeStationPluie") %
//                 year).str();

//        PGresult* resultRainfall = PQexec(connection, requestRainfall.c_str());
//        if (PQresultStatus(resultRainfall) != PGRES_TUPLES_OK) std::cout << "Error: " << PQerrorMessage(connection) << std::endl;

//        std::string requestPersonalData =
//                (boost::format("SELECT * FROM \"personalData\" "        \
//                               "WHERE \"CodeStation\"='%1%' AND "       \
//                               "\"Jour\" LIKE \'%%%2%%%\'") %
//                 parameters.get < std::string >("CodeStationMeteo") %
//                 year).str();
//        PGresult* resultPersonalData = PQexec(connection, requestPersonalData.c_str());
//        if (PQresultStatus(resultPersonalData) != PGRES_TUPLES_OK) std::cout << "Error: " << PQerrorMessage(connection) << std::endl;

//        if(PQntuples(resultMeteorology) > 0 && PQntuples(resultRainfall) > 0
//                && PQntuples(resultPersonalData) > 0) {
//            //        if (not resultMeteorology.empty() and not resultRainfall.empty()
//            //                and not resultPersonalData.empty()) {

//            //            pqxx::result::const_iterator itMeteorology =
//            //                    resultMeteorology.begin();
//            //            pqxx::result::const_iterator itRainfall =
//            //                    resultRainfall.begin();
//            //            pqxx::result::const_iterator itPersonalData =
//            //                    resultPersonalData.begin();
//            //            bool finished = false;

//            for (int i = 0; i < PQntuples(resultMeteorology); i++) {
//                std::string day;
//                double t;

//                //utils::DateTime::format_date(boost::lexical_cast < std::string >(PQgetvalue(result, i, 0)), day);
//                //                utils::DateTime::format_date(
//                //                            boost::lexical_cast <std::string >(
//                //                                itMeteorology->at(1)), day);
//                //                t = utils::DateTime::toJulianDayNumber(day);

//                t = utils::DateTime::toJulianDayNumber(
//                            boost::lexical_cast < std::string >(PQgetvalue(
//                                                                    resultMeteorology, i, 0)));

//                if (t >= begin && t <= end) {
//                    parameters.meteoValues.push_back(
//                                model::models::Climate(
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultMeteorology, i, 2)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultMeteorology, i, 3)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultMeteorology, i, 4)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultMeteorology, i, 5)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultMeteorology, i, 6)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultMeteorology, i, 7)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultMeteorology, i, 8)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultMeteorology, i, 9)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultMeteorology, i, 10)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultPersonalData, i, 2)),
//                                    boost::lexical_cast < double >(
//                                        PQgetvalue(resultRainfall, i, 2))));
//                }
//            }
//        }
//    }

//#else
//    pqxx::connection& connection(
//                utils::Connections::connection(
//                    "samara", "dbname=samara user=user_samara password=toto"));

//    std::string beginDate;
//    std::string endDate;
//    double begin;
//    double end;
//    unsigned int beginYear;
//    unsigned int endYear;

//    utils::DateTime::format_date(parameters.get < std::string >("BeginDate"),
//                                 beginDate);
//    utils::DateTime::format_date(parameters.get < std::string >("EndDate"),
//                                 endDate);
//    begin = utils::DateTime::toJulianDayNumber(beginDate);
//    end = utils::DateTime::toJulianDayNumber(endDate);
//    beginYear = utils::DateTime::year(begin);
//    endYear = utils::DateTime::year(end);
//    try {
//        pqxx::work action(connection);

//        for (unsigned int year = beginYear; year <= endYear; year++) {
//            std::string requestMeteorology =
//                    (boost::format("SELECT * FROM \"meteorology\" "         \
//                                   "WHERE \"CodeStation\"='%1%' AND "       \
//                                   "\"Jour\" LIKE \'%%%2%%%\'") %
//                     parameters.get < std::string >("CodeStationMeteo") %
//                     year).str();

//            pqxx::result resultMeteorology = action.exec(requestMeteorology);

//            std::string requestRainfall =
//                    (boost::format("SELECT * FROM \"rainfall\" "            \
//                                   "WHERE \"CodeStation\"='%1%' AND "       \
//                                   "\"Jour\" LIKE \'%%%2%%%\'") %
//                     parameters.get < std::string >("CodeStationPluie") %
//                     year).str();

//            pqxx::result resultRainfall = action.exec(requestRainfall);

//            std::string requestPersonalData =
//                    (boost::format("SELECT * FROM \"personalData\" "        \
//                                   "WHERE \"CodeStation\"='%1%' AND "       \
//                                   "\"Jour\" LIKE \'%%%2%%%\'") %
//                     parameters.get < std::string >("CodeStationMeteo") %
//                     year).str();

//            pqxx::result resultPersonalData = action.exec(requestPersonalData);

//            if (not resultMeteorology.empty() and not resultRainfall.empty()
//                    and not resultPersonalData.empty()) {
//                pqxx::result::const_iterator itMeteorology =
//                        resultMeteorology.begin();
//                pqxx::result::const_iterator itRainfall =
//                        resultRainfall.begin();
//                pqxx::result::const_iterator itPersonalData =
//                        resultPersonalData.begin();
//                bool finished = false;

//                while (not finished) {
//                    std::string day;
//                    double t;

//                    utils::DateTime::format_date(
//                                boost::lexical_cast <std::string >(
//                                    itMeteorology->at(1)), day);
//                    t = utils::DateTime::toJulianDayNumber(day);

//                    if (t >= begin and t <= end) {
//                        values.push_back(
//                                    Climate(
//                                        boost::lexical_cast < double >(
//                                            itMeteorology->at(2)),
//                                        boost::lexical_cast < double >(
//                                            itMeteorology->at(3)),
//                                        boost::lexical_cast < double >(
//                                            itMeteorology->at(4)),
//                                        boost::lexical_cast < double >(
//                                            itMeteorology->at(5)),
//                                        boost::lexical_cast < double >(
//                                            itMeteorology->at(6)),
//                                        boost::lexical_cast < double >(
//                                            itMeteorology->at(7)),
//                                        boost::lexical_cast < double >(
//                                            itMeteorology->at(8)),
//                                        boost::lexical_cast < double >(
//                                            itMeteorology->at(9)),
//                                        boost::lexical_cast < double >(
//                                            itMeteorology->at(10)),
//                                        boost::lexical_cast < double >(
//                                            itPersonalData->at(2)),
//                                        boost::lexical_cast < double >(
//                                            itRainfall->at(2))));
//                    }
//                    if (++itMeteorology == resultMeteorology.end() or
//                            ++itRainfall == resultRainfall.end()
//                            or ++itPersonalData == resultPersonalData.end()) {
//                        finished = true;
//                    }
//                }
//            }
//        }
//    } catch (pqxx::sql_error e) {
//        std::cout << "Error: " << e.query() << std::endl;
//    }
//#endif


//}

//// Database
//#if 1
//void ParametersReader::load_data(PQConnector connection,
//                                 const std::string &table,
//                                 const std::string& key,
//                                 const std::string& value,
//                                 const std::vector < std::string > &names,
//                                 model::models::ModelParameters &parameters)
//{

//    PGresult* result = PQexec(
//                connection,
//                (boost::format("SELECT * FROM \"%1%\" WHERE \"%2%\"=\"%3%\"") %
//                 table % key % value).str().c_str()
//                );

//    if (PQresultStatus(result) != PGRES_TUPLES_OK)
//        std::cout << "Error: " << PQerrorMessage(connection) << std::endl;


//    for (int i = 0; i < PQnfields(result); i++) {
//        std::vector < std::string >::const_iterator itv = std::find(names.begin(), names.end(),
//                                                                    PQfname(result, i));

//        if (itv != names.end()) {
//            if ((PQftype(result, i) == 1043) || (PQftype(result, i) == 1082))
//                parameters.set <std::string>(*itv, PQgetvalue(result, 0, i));

//            else
//                parameters.set<double>(*itv, boost::lexical_cast<double>(PQgetvalue(result, 0, i)));

//        }
//        else
//            parameters.set < double >(PQfname(result, i), 0.);
//    }
//}
//#else
//void ParametersReader::load_data(PQConnector connection,
//                                 const std::string& table,
//                                 const std::string& key,
//                                 const std::string& value,
//                                 const std::vector < std::string >& names,
//                                 model::models::ModelParameters& parameters)
//{
//    try {
//        pqxx::work action(connection);
//        pqxx::result result = action.exec(
//                    (boost::format("SELECT * FROM \"%1%\" WHERE \"%2%\"='%3%'") %
//                     table % key % value).str());

//        if (not result.empty()) {
//            for (pqxx::result::tuple::const_iterator it = result[0].begin();
//                 it != result[0].end(); ++it) {
//                std::vector < std::string >::const_iterator itv =
//                        std::find(names.begin(), names.end(), it->name());

//                if (itv != names.end()) {
//                    if (it->type() == 1043) {
//                        parameters.set < std::string >(
//                                    *itv,  boost::lexical_cast < std::string >(*it));
//                    } else {
//                        parameters.set < double >(
//                                    *itv, boost::lexical_cast < double >(*it));
//                    }
//                } else {
//                    parameters.set < double >(it->name(), 0.);
//                }
//            }
//        }
//    } catch (pqxx::sql_error e) {
//        std::cout << "Error: " << e.query() << std::endl;
//    }
//}
//#endif

//void ParametersReader::load_variety(
//        const std::string& id,
//        PQConnector connection,
//        model::models::ModelParameters& parameters)
//{
//    std::vector < std::string > names = {
//        "ASScstr",
//        "AttenMitch",
//        "Ca",
//        "CO2Cp",
//        "CO2Exp",
//        "CO2SlopeTr",
//        "CoeffAssimSla",
//        "CoefficientQ10",
//        "CoeffInternodeMass",
//        "CoeffInternodeNum",
//        "CoeffLeafDeath",
//        "CoeffLeafWLRatio",
//        "CoeffPanicleMass",
//        "CoeffPanSinkPop",
//        "CoeffResCapacityInternode",
//        "CoeffReserveSink",
//        "CoeffRootMassPerVolMax",
//        "CoeffTillerDeath",
//        "DEVcstr",
//        "ExcessAssimToRoot",
//        "FTSWIrrig",
//        "HaunCritTillering",
//        "IcTillering",
//        "InternodeLengthMax",
//        "IrrigAutoResume",
//        "IrrigAutoStop",
//        "KcMax",
//        "KCritSterCold1",
//        "KCritSterCold2",
//        "KCritSterFtsw1",
//        "KCritSterFtsw2",
//        "KCritSterHeat1",
//        "KCritSterHeat2",
//        "KCritStressCold1",
//        "KCritStressCold2",
//        "Kdf",
//        "KRespInternode",
//        "KRespMaintLeaf",
//        "KRespMaintRoot",
//        "KRespMaintSheath",
//        "KRespPanicle",
//        "KTempMaint",
//        "LeafLengthMax",
//        "PanStructMassMax",
//        "PFactor",
//        "Phyllo",
//        "PoidsSecGrain",
//        "PPCrit",
//        "PPExp",
//        "PPSens",
//        "PriorityPan",
//        "RankLongestLeaf",
//        "RelMobiliInternodeMax",
//        "RelPhylloPhaseStemElong",
//        "RollingBase",
//        "RollingSens",
//        "RootCstr",
//        "RootFrontMax",
//        "RootPartitMax",
//        "SDJBVP",
//        "SDJLevee",
//        "SDJMatu1",
//        "SDJMatu2",
//        "SDJRPR",
//        "SeuilPP",
//        "SlaMax",
//        "SlaMin",
//        "TBase",
//        "TempSla",
//        "TilAbility",
//        "TLet",
//        "TOpt1",
//        "TOpt2",
//        "TransplantingDepth",
//        "TxAssimBVP",
//        "TxAssimMatu1",
//        "TxAssimMatu2",
//        "TxConversion",
//        "TxResGrain",
//        "TxRuSurfGermi",
//        "VRacBVP",
//        "VRacLevee",
//        "VRacMatu1",
//        "VRacMatu2",
//        "VRacPSP",
//        "VRacRPR",
//        "WaterLoggingSens",
//        "WtRatioLeafSheath"
//    };

//    load_data(connection, "variety", "Id", id, names, parameters);
//}

//void ParametersReader::load_itineraire_technique(
//        const std::string& id,
//        PQConnector connection,
//        model::models::ModelParameters& parameters)
//{
//    std::vector < std::string > names = {
//        "BundHeight",
//        "CoeffTransplantingShock",
//        "DateSemis",
//        "DensityField",
//        "DensityNursery",
//        "DurationNursery",
//        "FTSWIrrig",
//        "IrrigAuto",
//        "IrrigAutoResume",
//        "IrrigAutoStop",
//        "IrrigAutoTarget",
//        "LifeSavingDrainage",
//        "Mulch",
//        "PlantsPerHill",
//        "PlotDrainageDAF",
//        "ProfRacIni",
//        "Transplanting",
//        "TransplantingDepth"
//    };

//    load_data(connection, "itineraireTechnique", "Id", id, names, parameters);
//}

//void ParametersReader::load_plot(
//        const std::string& id,
//        PQConnector connection,
//        model::models::ModelParameters& parameters)
//{
//    std::vector < std::string > names = {
//        "Ca", "EpaisseurProf", "EpaisseurSurf", "StockIniProf", "StockIniSurf",
//        "IdTypeSol"
//    };

//    load_data(connection, "plot", "Id", id, names, parameters);
//}

//void ParametersReader::load_simulation(
//        const std::string& id,
//        PQConnector connection,
//        model::models::ModelParameters& parameters)
//{
//    std::vector < std::string > names = { "IdSite","IdPlot", "IdVariety",
//                                          "IdItineraireTechnique", "IdModele",
//                                          "BeginDate", "EndDate" };

//    load_data(connection, "simulation", "Id", id, names, parameters);
//}

//void ParametersReader::load_site(
//        const std::string& id,
//        PQConnector connection,
//        model::models::ModelParameters& parameters)
//{
//    std::vector < std::string > names = { "SeuilPluie", "Kpar",
//                                          "CodeStationMeteo",
//                                          "CodeStationPluie" };

//    load_data(connection, "site", "Id", id, names, parameters);
//}

//void ParametersReader::load_station(
//        const std::string& id,
//        PQConnector connection,
//        model::models::ModelParameters& parameters)
//{
//    std::vector < std::string > names = { "Altitude", "Latitude" };

//    load_data(connection, "station", "Code", id, names, parameters);
//}

//void ParametersReader::load_type_soil(
//        const std::string& id,
//        PQConnector connection,
//        model::models::ModelParameters& parameters)
//{
//    std::vector < std::string > names = {
//        "HumFC", "HumPF", "HumSat", "PercolationMax", "PourcRuiss",
//        "SeuilRuiss", "PEvap"
//    };

//    load_data(connection, "typeSoil", "Id", id, names, parameters);
//}


//} // namespace utils
