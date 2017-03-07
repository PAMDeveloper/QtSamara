/**
 * @file samara/utils/ParametersReader.cpp
 * @author The Samara Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2013-2017 ULCO http://www.univ-littoral.fr
 * Copyright (C) 2013-2017 INRA http://www.inra.fr
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

namespace utils {

void ParametersReader::loadFromDatabase(
        const std::string& id, model::models::ModelParameters& parameters)
{
#if 1
    PQConnector connection =
            PQconnectdb("port='5432' hostaddr='127.0.0.1' dbname='samara' user='user_samara' password='toto'");
#else
    PQConnector connection(
                utils::Connections::connection(
                    "samara", "dbname=samara user=user_samara password=toto"));
#endif

    //Eric Ramat deprecated version
//    load_simulation(id, connection, parameters);
//    load_variety(parameters.get < std::string >("IdVariety"), connection, parameters);
//    load_itineraire_technique(parameters.get < std::string >("IdItineraireTechnique"), connection, parameters);
//    load_plot(parameters.get < std::string >("IdPlot"), connection, parameters);
//    load_site(parameters.get < std::string >("IdSite"), connection, parameters);
//    load_station(parameters.get < std::string >("CodeStationMeteo"), connection, parameters);
//    load_type_soil(parameters.get < std::string >("IdTypeSol"), connection, parameters);

    //Gregory Aguilar version
    load_simulation(id, connection, parameters);
    load_variety(parameters.getStringVal("idvariete"), connection, parameters);
    load_itineraire_technique(parameters.getStringVal("iditinerairetechnique"), connection,parameters);
    load_plot(parameters.getStringVal("idparcelle"), connection, parameters);
    load_station(parameters.getStringVal("codestationmeteo"), connection, parameters);


    /*    std::cout << "Simulation " << id << ":" << std::endl;
    std::cout << " - begin = " << parameters.get < std::string >("BeginDate")
              << std::endl;
    std::cout << " - end = " << parameters.get < std::string >("EndDate")
              << std::endl;
    std::cout << " - variety = " << parameters.get < std::string >("IdVariety")
              << std::endl;
    std::cout << " - itineraire technique = "
              << parameters.get < std::string >("IdItineraireTechnique")
              << std::endl;
    std::cout << " - date semis = "
              << parameters.get < std::string >("DateSemis")
              << std::endl;
    std::cout << " - plot = " << parameters.get < std::string >("IdPlot")
              << std::endl;
    std::cout << " - site = " << parameters.get < std::string >("IdSite")
              << std::endl;
    std::cout << " - station = "
              << parameters.get < std::string >("CodeStationMeteo")
              << std::endl;
    std::cout << " - type_soil = "
              << parameters.get < std::string >("IdTypeSol")
              << std::endl;
    std::cout << " - model = "
              << parameters.get < std::string >("IdModele")
              << std::endl; */

}

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

void load_meteo(PQConnector connection, model::models::ModelParameters &parameters){
#if 1
    std::string beginDate;
    std::string endDate;
    double begin;
    double end;
    unsigned int beginYear;
    unsigned int endYear;

    utils::DateTime::format_date(parameters.get < std::string >("BeginDate"),
                                 beginDate);
    utils::DateTime::format_date(parameters.get < std::string >("EndDate"),
                                 endDate);
    begin = utils::DateTime::toJulianDayNumber(beginDate);
    end = utils::DateTime::toJulianDayNumber(endDate);
    beginYear = utils::DateTime::year(begin);
    endYear = utils::DateTime::year(end);


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


    for (unsigned int year = beginYear; year <= endYear; year++) {
        std::string requestMeteorology =
                (boost::format("SELECT * FROM \"meteorology\" "         \
                               "WHERE \"CodeStation\"='%1%' AND "       \
                               "\"Jour\" LIKE \'%%%2%%%\'") %
                 parameters.get < std::string >("CodeStationMeteo") %
                 year).str();

        PGresult* resultMeteorology = PQexec(connection, requestMeteorology.c_str());
        if (PQresultStatus(resultMeteorology) != PGRES_TUPLES_OK) std::cout << "Error: " << PQerrorMessage(connection) << std::endl;

        std::string requestRainfall =
                (boost::format("SELECT * FROM \"rainfall\" "            \
                               "WHERE \"CodeStation\"='%1%' AND "       \
                               "\"Jour\" LIKE \'%%%2%%%\'") %
                 parameters.get < std::string >("CodeStationPluie") %
                 year).str();

        PGresult* resultRainfall = PQexec(connection, requestRainfall.c_str());
        if (PQresultStatus(resultRainfall) != PGRES_TUPLES_OK) std::cout << "Error: " << PQerrorMessage(connection) << std::endl;

        std::string requestPersonalData =
                (boost::format("SELECT * FROM \"personalData\" "        \
                               "WHERE \"CodeStation\"='%1%' AND "       \
                               "\"Jour\" LIKE \'%%%2%%%\'") %
                 parameters.get < std::string >("CodeStationMeteo") %
                 year).str();
        PGresult* resultPersonalData = PQexec(connection, requestPersonalData.c_str());
        if (PQresultStatus(resultPersonalData) != PGRES_TUPLES_OK) std::cout << "Error: " << PQerrorMessage(connection) << std::endl;

        if(PQntuples(resultMeteorology) > 0 && PQntuples(resultRainfall) > 0
                && PQntuples(resultPersonalData) > 0) {
            //        if (not resultMeteorology.empty() and not resultRainfall.empty()
            //                and not resultPersonalData.empty()) {

            //            pqxx::result::const_iterator itMeteorology =
            //                    resultMeteorology.begin();
            //            pqxx::result::const_iterator itRainfall =
            //                    resultRainfall.begin();
            //            pqxx::result::const_iterator itPersonalData =
            //                    resultPersonalData.begin();
            //            bool finished = false;

            for (int i = 0; i < PQntuples(resultMeteorology); i++) {
                std::string day;
                double t;

                //utils::DateTime::format_date(boost::lexical_cast < std::string >(PQgetvalue(result, i, 0)), day);
                //                utils::DateTime::format_date(
                //                            boost::lexical_cast <std::string >(
                //                                itMeteorology->at(1)), day);
                //                t = utils::DateTime::toJulianDayNumber(day);

                t = utils::DateTime::toJulianDayNumber(
                            boost::lexical_cast < std::string >(PQgetvalue(
                                                                    resultMeteorology, i, 0)));

                if (t >= begin && t <= end) {
                    parameters.meteoValues.push_back(
                                model::models::Climate(
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultMeteorology, i, 2)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultMeteorology, i, 3)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultMeteorology, i, 4)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultMeteorology, i, 5)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultMeteorology, i, 6)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultMeteorology, i, 7)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultMeteorology, i, 8)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultMeteorology, i, 9)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultMeteorology, i, 10)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultPersonalData, i, 2)),
                                    boost::lexical_cast < double >(
                                        PQgetvalue(resultRainfall, i, 2))));
                }
            }
        }
    }

#else
    pqxx::connection& connection(
                utils::Connections::connection(
                    "samara", "dbname=samara user=user_samara password=toto"));

    std::string beginDate;
    std::string endDate;
    double begin;
    double end;
    unsigned int beginYear;
    unsigned int endYear;

    utils::DateTime::format_date(parameters.get < std::string >("BeginDate"),
                                 beginDate);
    utils::DateTime::format_date(parameters.get < std::string >("EndDate"),
                                 endDate);
    begin = utils::DateTime::toJulianDayNumber(beginDate);
    end = utils::DateTime::toJulianDayNumber(endDate);
    beginYear = utils::DateTime::year(begin);
    endYear = utils::DateTime::year(end);
    try {
        pqxx::work action(connection);

        for (unsigned int year = beginYear; year <= endYear; year++) {
            std::string requestMeteorology =
                    (boost::format("SELECT * FROM \"meteorology\" "         \
                                   "WHERE \"CodeStation\"='%1%' AND "       \
                                   "\"Jour\" LIKE \'%%%2%%%\'") %
                     parameters.get < std::string >("CodeStationMeteo") %
                     year).str();

            pqxx::result resultMeteorology = action.exec(requestMeteorology);

            std::string requestRainfall =
                    (boost::format("SELECT * FROM \"rainfall\" "            \
                                   "WHERE \"CodeStation\"='%1%' AND "       \
                                   "\"Jour\" LIKE \'%%%2%%%\'") %
                     parameters.get < std::string >("CodeStationPluie") %
                     year).str();

            pqxx::result resultRainfall = action.exec(requestRainfall);

            std::string requestPersonalData =
                    (boost::format("SELECT * FROM \"personalData\" "        \
                                   "WHERE \"CodeStation\"='%1%' AND "       \
                                   "\"Jour\" LIKE \'%%%2%%%\'") %
                     parameters.get < std::string >("CodeStationMeteo") %
                     year).str();

            pqxx::result resultPersonalData = action.exec(requestPersonalData);

            if (not resultMeteorology.empty() and not resultRainfall.empty()
                    and not resultPersonalData.empty()) {
                pqxx::result::const_iterator itMeteorology =
                        resultMeteorology.begin();
                pqxx::result::const_iterator itRainfall =
                        resultRainfall.begin();
                pqxx::result::const_iterator itPersonalData =
                        resultPersonalData.begin();
                bool finished = false;

                while (not finished) {
                    std::string day;
                    double t;

                    utils::DateTime::format_date(
                                boost::lexical_cast <std::string >(
                                    itMeteorology->at(1)), day);
                    t = utils::DateTime::toJulianDayNumber(day);

                    if (t >= begin and t <= end) {
                        values.push_back(
                                    Climate(
                                        boost::lexical_cast < double >(
                                            itMeteorology->at(2)),
                                        boost::lexical_cast < double >(
                                            itMeteorology->at(3)),
                                        boost::lexical_cast < double >(
                                            itMeteorology->at(4)),
                                        boost::lexical_cast < double >(
                                            itMeteorology->at(5)),
                                        boost::lexical_cast < double >(
                                            itMeteorology->at(6)),
                                        boost::lexical_cast < double >(
                                            itMeteorology->at(7)),
                                        boost::lexical_cast < double >(
                                            itMeteorology->at(8)),
                                        boost::lexical_cast < double >(
                                            itMeteorology->at(9)),
                                        boost::lexical_cast < double >(
                                            itMeteorology->at(10)),
                                        boost::lexical_cast < double >(
                                            itPersonalData->at(2)),
                                        boost::lexical_cast < double >(
                                            itRainfall->at(2))));
                    }
                    if (++itMeteorology == resultMeteorology.end() or
                            ++itRainfall == resultRainfall.end()
                            or ++itPersonalData == resultPersonalData.end()) {
                        finished = true;
                    }
                }
            }
        }
    } catch (pqxx::sql_error e) {
        std::cout << "Error: " << e.query() << std::endl;
    }
#endif


}

// Database
#if 1
void ParametersReader::load_data(PQConnector connection,
                                 const std::string &table,
                                 const std::string& key,
                                 const std::string& value,
                                 const std::vector < std::string > &names,
                                 model::models::ModelParameters &parameters)
{

    PGresult* result = PQexec(
                connection,
                (boost::format("SELECT * FROM \"%1%\" WHERE \"%2%\"=\"%3%\"") %
                 table % key % value).str().c_str()
                );

    if (PQresultStatus(result) != PGRES_TUPLES_OK)
        std::cout << "Error: " << PQerrorMessage(connection) << std::endl;


    for (int i = 0; i < PQnfields(result); i++) {
        std::vector < std::string >::const_iterator itv = std::find(names.begin(), names.end(),
                                                                    PQfname(result, i));

        if (itv != names.end()) {
            if ((PQftype(result, i) == 1043) || (PQftype(result, i) == 1082))
                parameters.set <std::string>(*itv, PQgetvalue(result, 0, i));

            else
                parameters.set<double>(*itv, boost::lexical_cast<double>(PQgetvalue(result, 0, i)));

        }
        else
            parameters.set < double >(PQfname(result, i), 0.);
    }
}
#else
void ParametersReader::load_data(PQConnector connection,
                                 const std::string& table,
                                 const std::string& key,
                                 const std::string& value,
                                 const std::vector < std::string >& names,
                                 model::models::ModelParameters& parameters)
{
    try {
        pqxx::work action(connection);
        pqxx::result result = action.exec(
                    (boost::format("SELECT * FROM \"%1%\" WHERE \"%2%\"='%3%'") %
                     table % key % value).str());

        if (not result.empty()) {
            for (pqxx::result::tuple::const_iterator it = result[0].begin();
                 it != result[0].end(); ++it) {
                std::vector < std::string >::const_iterator itv =
                        std::find(names.begin(), names.end(), it->name());

                if (itv != names.end()) {
                    if (it->type() == 1043) {
                        parameters.set < std::string >(
                                    *itv,  boost::lexical_cast < std::string >(*it));
                    } else {
                        parameters.set < double >(
                                    *itv, boost::lexical_cast < double >(*it));
                    }
                } else {
                    parameters.set < double >(it->name(), 0.);
                }
            }
        }
    } catch (pqxx::sql_error e) {
        std::cout << "Error: " << e.query() << std::endl;
    }
}
#endif

void ParametersReader::load_variety(
        const std::string& id,
        PQConnector connection,
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

    load_data(connection, "variety", "Id", id, names, parameters);
}

void ParametersReader::load_itineraire_technique(
        const std::string& id,
        PQConnector connection,
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

    load_data(connection, "itineraireTechnique", "Id", id, names, parameters);
}

void ParametersReader::load_plot(
        const std::string& id,
        PQConnector connection,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = {
        "Ca", "EpaisseurProf", "EpaisseurSurf", "StockIniProf", "StockIniSurf",
        "IdTypeSol"
    };

    load_data(connection, "plot", "Id", id, names, parameters);
}

void ParametersReader::load_simulation(
        const std::string& id,
        PQConnector connection,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = { "IdSite","IdPlot", "IdVariety",
                                          "IdItineraireTechnique", "IdModele",
                                          "BeginDate", "EndDate" };

    load_data(connection, "simulation", "Id", id, names, parameters);
}

void ParametersReader::load_site(
        const std::string& id,
        PQConnector connection,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = { "SeuilPluie", "Kpar",
                                          "CodeStationMeteo",
                                          "CodeStationPluie" };

    load_data(connection, "site", "Id", id, names, parameters);
}

void ParametersReader::load_station(
        const std::string& id,
        PQConnector connection,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = { "Altitude", "Latitude" };

    load_data(connection, "station", "Code", id, names, parameters);
}

void ParametersReader::load_type_soil(
        const std::string& id,
        PQConnector connection,
        model::models::ModelParameters& parameters)
{
    std::vector < std::string > names = {
        "HumFC", "HumPF", "HumSat", "PercolationMax", "PourcRuiss",
        "SeuilRuiss", "PEvap"
    };

    load_data(connection, "typeSoil", "Id", id, names, parameters);
}

// JSON

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
