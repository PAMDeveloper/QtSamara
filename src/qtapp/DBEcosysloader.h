#ifndef PARADOX_LOADER_H
#define PARADOX_LOADER_H

#include <iostream>
#include <libpq-fe.h>
#include <QtSql/QtSql>
#include "simulationloader.h"
#include "../parameters.h"
#include "julianconverter.h"

#define PSQLTYPE_STRING 1043
#define PSQLTYPE_DATE 1082


using namespace std;


class DBEcosysLoader : public AbstractSimulationLoader
{
public:
    QSqlDatabase dbPlanteSol;
    QSqlDatabase dbSimulation;
    QSqlDatabase dbGeoClimato;

    void openDb(QSqlDatabase * db, QString path, QString name) {
        *db = QSqlDatabase::addDatabase("QODBC", name);
        QString pdxFolder = path + "/" + name;
        QString conStr = "Driver={Microsoft Paradox Driver (*.db )};DriverID=538;Fil=Paradox 5.X;" \
                          "DefaultDir=" + pdxFolder + ";" \
                          "Dbq=" + pdxFolder + ";" \
                          "CollatingSequence=ASCII;";
        db->setDatabaseName(conStr);
        if(db->open())
            qDebug() << "DB opened" << db->connectionName() << db->tables();
        else
            qDebug() <<"Error: " << db->lastError ();
    }

    DBEcosysLoader(SamaraParameters * params, QString pdxFolder)
        : AbstractSimulationLoader(params) {
        openDb(&dbPlanteSol, pdxFolder, "DBPlanteSol");
        openDb(&dbSimulation, pdxFolder, "DBSimulation");
        openDb(&dbGeoClimato, pdxFolder, "DBGeoClimato");
    }

    ~DBEcosysLoader() {
        dbPlanteSol.close();
        dbSimulation.close();
        dbGeoClimato.close();
    }

    QString query(QString table, QString key, QString value) {
        return "SELECT * FROM " + table + " WHERE " + key + "='" + value + "'";
    }

    QString list_query(QString table, QString key) {
        return "SELECT DISTINCT " + key + " FROM " + table;
    }

    vector<string> load_list(const QSqlDatabase & db, QString query) {
        QSqlQuery result(query, db);
        if(!result.lastError().text().isEmpty())
            qDebug() << query << " > " << result.lastError().text();
        vector<string> list;
        result.first();
        do {
            QString id = result.value(0).toString();
            list.push_back(id.toStdString());
        } while(result.next());
        std::sort (list.begin(), list.end());
        return list;
    }

    void load_records(const QSqlDatabase & db, QString query) {
        QSqlQuery result(query, db);
        if(!result.lastError().text().isEmpty())
            qDebug() << query << " >\n " << result.lastError().text();
        result.first();
        do {
            parameters->climatics.push_back(
                        Climate(
                            result.value(1).toDate().toJulianDay(),
                            result.value(2).toDouble(),
                            result.value(3).toDouble(),
                            result.value(4).toDouble(),
                            result.value(5).toDouble(),
                            result.value(6).toDouble(),
                            result.value(7).toDouble(),
                            result.value(8).toDouble(),
                            result.value(9).toDouble(),
                            result.value(10).toDouble(),
                            0,
                            result.value(11).toDouble()
                            )
                        );
        } while(result.next());
    }

    void load_record(const QSqlDatabase & db, QString table, QString key, QString value) {
        load_record(db, query(table,key,value), table);
    }

    void load_record(const QSqlDatabase & db, QString query, QString category) {
        QSqlQuery result(query, db);
        QSqlRecord rec = result.record();
        if(!result.lastError().text().isEmpty())
            qDebug() << query << " >\n " << result.lastError().text();
;        result.first();
        do {
            for (int col = 0; col < rec.count(); ++col) {
                QVariant::Type type = rec.field(col).type();
                QString key = rec.fieldName(col);
                if(category == "variete") {
                    if(     key == "IrrigAutoStop" ||
                            key == "IrrigAutoResume" ||
                            key == "FTSWIrrig" ||
                            key == "TransplantingDepth" ||
                            key == "Ca")
                        continue;
                }
                if(parameters->doubles.find(key.toLower().toStdString()) != parameters->doubles.end() && key != "Edit") {
                    qDebug() << key << "already exists !!!!" ;
                    qDebug() << "from" << parameters->doubles[key.toLower().toStdString()].first << "to" << result.value(col).toString();
                }
                if(type == QVariant::String) {
                    parameters->strings[key.toLower().toStdString()] =
                            pair <string, string> (
                                result.value(col).toString().toStdString(), category.toStdString());
                } else if (type == QVariant::Date || type == QVariant::DateTime) {
//                    qDebug() << key << result.value(col).toString() << QVariant::typeToName(type);
                    parameters->strings[key.toLower().toStdString()] =
                            pair <string, string> (
                                result.value(col).toString().toStdString(), category.toStdString());
//                    qDebug() << QString::fromStdString(parameters->strings[key.toStdString()].first);
                    parameters->doubles[key.toLower().toStdString()] =
                            pair <double, string> (
                                result.value(col).toDate().toJulianDay(), category.toStdString());
                } else {
//                    qDebug() << key << result.value(col) << QVariant::typeToName(type);
                    parameters->doubles[key.toLower().toStdString()] =
                            pair <double, string> (
                                result.value(col).toDouble(), category.toStdString());
                }

            }
        } while (result.next());

//        PGresult* result = PQexec(db, query.c_str());
//        if (PQresultStatus(result) != PGRES_TUPLES_OK){
//            cout << "Error: " << PQerrorMessage(db) << endl;
//        }

//        for (int col = 0; col < PQnfields(result); ++col) {
//            string key = string(PQfname(result,col));
//            if( PQftype(result,col) == PSQLTYPE_STRING ){
//                parameters->strings[key] =  pair <string, string> (string(PQgetvalue(result,0,col)), category);
//            } else if( PQftype(result,col) == PSQLTYPE_DATE ){
//                parameters->strings[key] =  pair <string, string> (string(PQgetvalue(result,0,col)), category);
//                parameters->doubles[key] =  pair <double, string> (JulianDayConverter::toJulianDayNumber(string(PQgetvalue(result,0,col))), category);

//            } else {
//                parameters->doubles[key] =  pair <double, string> ( atof(PQgetvalue(result,0,col)), category);
//            }
//        }
    }


    void load_complete_simulation(string idsimulation) {
        load_simulation(idsimulation);
        for(auto pair: parameters->strings) {
            qDebug() << QString::fromStdString(pair.first) << QString::fromStdString(pair.second.first);
        }
        load_variety(parameters->getString("idvariete"));
        load_station(parameters->getString("idsite"));
        load_plot(parameters->getString("idparcelle"));
        load_itinerary(parameters->getString("idItinerairetechnique"));
        load_meteo(parameters->getString("codestation"),
                   parameters->getDouble("debutsimul"),
                   parameters->getDouble("finsimul"));
    }

    vector<string> load_simulation_list() {
        return load_list(dbSimulation, list_query("Simule","Id"));
    }

    vector<string>  load_variety_list() {
        return load_list(dbPlanteSol, list_query("Variete","Id"));
    }

    vector<string>  load_station_list() {
        return load_list(dbSimulation, list_query("Site","Id")); //Nom
    }

    vector<string> load_plot_list() {
        return load_list(dbSimulation, list_query("Parcelle","Id"));
    }

    vector<string>  load_itinerary_list() {
        return load_list(dbSimulation, list_query("ItineraireTechnique","Id"));
    }

    //@TODO bouger KPAR dans la station ou ailleurs
    void load_simulation(string idsimulation) {
        parameters->clearParameters("simulation");
        load_record(dbSimulation, query("Simule","Id",QString::fromStdString(idsimulation)), "simulation");
    }

    void load_variety(string idvariete) {
        parameters->clearParameters("variete");
        load_record(dbPlanteSol, query("Variete","Id",QString::fromStdString(idvariete)), "variete");
    }

    void load_station(string idsite) {
        parameters->clearParameters("site");
        parameters->clearParameters("station");
        load_record(dbSimulation, query("Site","Id",QString::fromStdString(idsite)), "site");
        load_record(dbGeoClimato, query("Station","Code",QString::fromStdString(parameters->getString("codestationmeteo"))), "station");
    }

    void load_plot(string idparcelle) {
        parameters->clearParameters("parcelle");
        parameters->clearParameters("soil");
        load_record(dbSimulation, query("Parcelle","Id",QString::fromStdString(idparcelle)), "parcelle");
        load_record(dbPlanteSol, query("TypeSol","Id",QString::fromStdString(parameters->getString("idtypesol"))), "soil");
    }

    void load_itinerary(string iditinerairetechnique) {
        parameters->clearParameters("itinerairetechnique");
        load_record(dbSimulation, query("ItineraireTechnique","Id",QString::fromStdString(iditinerairetechnique)), "itinerairetechnique");
    }


    //Paradox dateType
    //{d 'value'} yyyy-mm-dd
    //{t 'value'} hh:mm:ss
    //{ts 'value'} yyyy-mm-dd hh:mm:ss
    void load_meteo(string site, double startJulian, double endJulian) {
        parameters->clearMeteo();
        QString codeStationQuery = query("Site", "Id", QString::fromStdString(site));
        QSqlQuery result(codeStationQuery, dbSimulation);
        result.first();
        QString codestation = result.value("CodeStationMeteo").toString();

        string start = QDate::fromJulianDay(startJulian-1).toString("yyyy-MM-dd").toStdString();
        string end = QDate::fromJulianDay(endJulian).toString("yyyy-MM-dd").toStdString();

        std::string meteo_query = query("Meteorologie","CodeStation", codestation).toStdString() +
                " AND Jour >= {d '" + start + "'} AND Jour <= { d'" +
                end + "'} ORDER BY Jour ASC";

        load_records(dbGeoClimato, QString::fromStdString(meteo_query));
    }


    map<string, vector<double>> load_obs(string idsimulation) {
        load_simulation(idsimulation);
        return load_obs(parameters->getString("idparcelle"), parameters->getString("idvariete"), parameters->getString("iditinerairetechnique"),
                        parameters->getString("codestation"), parameters->getDouble("datedebut"), parameters->getDouble("datefin"));
    }

    map<string, vector<double>> load_obs(string plot, string variety, string itinerary, string codestation, double startJulian, double endJulian) {
        string start = JulianDayConverter::toJulianDayFmt( startJulian - 1, DATE_FORMAT_YMD);
        string end = JulianDayConverter::toJulianDayFmt( endJulian, DATE_FORMAT_YMD );
        std::string obs_query = "SELECT * FROM observer WHERE idparcelle='" + plot +
                "' AND idvariete='" + variety +
                "' AND iditinerairetechnique='" + itinerary +
                "' AND codestation='" + codestation +
                "' AND jour >= '" + start + "' AND jour <= '" +
                end + "' ORDER BY jour ASC";
        std::cout << obs_query;
        map<string, vector<double>> resultMap;
//        PGresult* result = PQexec(db, obs_query.c_str());
//        if (PQresultStatus(result) != PGRES_TUPLES_OK){
//            cout << "Error: " << PQerrorMessage(db) << endl;
//        }

//        for (int col = 0; col < PQnfields(result); ++col) {
//            if( PQftype(result,col) == PSQLTYPE_STRING ){
//                //Nothing
//            } else if( PQftype(result,col) == PSQLTYPE_DATE ){
//                //Nothing ?
//            } else {
//                string key = string(PQfname(result,col));
//                vector<double> colData;
//                for (int row = 0; row < PQntuples(result); ++row) {
//                    colData.push_back(atof(PQgetvalue(result,row,col)));
//                }
//                resultMap.insert(pair<string, vector<double>>(key, colData));
//            }
//        }
        return resultMap;
    }

};

#endif // PARADOX_LOADER_H



//#ifndef FILELOADER_H
//#define FILELOADER_H

//#include <fstream>
//#include <sstream>
//#include <string>
//#include <algorithm>
//#include <iterator>
//#include "simulationloader.h"
//#include "../parameters.h"

//#include <QDebug>
//using namespace std;


//class ParadoxExportLoader : public AbstractSimulationLoader {
//public:
//    string folder;
//    ParadoxExportLoader(string folderPath, SamaraParameters * params)
//        : AbstractSimulationLoader(params), folder(folderPath){

//    }

//    template<typename Out>
//    void split(const std::string &s, char delim, Out result) {
//        std::stringstream ss;
//        ss.str(s);
//        std::string item;
//        while (std::getline(ss, item, delim)) {
//            *(result++) = item;
//        }
//    }


//    std::vector<std::string> split(const std::string &s, char delim) {
//        std::vector<std::string> elems;
//        split(s, delim, std::back_inserter(elems));
//        return elems;
//    }

//    void load_file_parameters(string file_path, string id) {
//        std::ifstream infile(file_path);
//        std::string line;
//        std::getline(infile, line); //config

//        std::getline(infile, line); //headers
//        vector<string> headers;
//        istringstream iss(line);
//        copy(istream_iterator<string>(iss),
//             istream_iterator<string>(),
//             back_inserter(headers));

//        foreach (string s, headers) {
//            qDebug() << QString::fromStdString(s);
//        }

//        while (std::getline(infile, line))
//        {
//            vector<string> data = split(line, '\t');
//            foreach (string s, data) {
//                qDebug() << QString::fromStdString(s);
//            }
//        }
//    }



//    void load_complete_simulation(string idsimulation){
//        load_simulation(idsimulation);
//        load_variety(parameters->getString("idvariete"));
//        load_station(parameters->getString("codestation"));
//        load_plot(parameters->getString("idparcelle"));
//        load_itinerary(parameters->getString("iditinerairetechnique"));
//        load_meteo(parameters->getString("codestation"),
//                   parameters->getString("datedebut"),
//                   parameters->getString("datefin"));
//    }

////    void load_simulation(string idsimulation) {
////        load_record(query("Simule","Id",idsimulation), "simulation");
////    }

////    void load_variety(string idvariete) {
////        load_record(query("Variete","Id",idvariete), "variete");
////    }

////    void load_station(string codestation) {
////        load_record(query("Site","Id",codestation), "station");
////    }

////    void load_plot(string idparcelle) {
////        load_record(query("Parcelle","Id",idparcelle), "parcelle");
////        load_record(query("TypeSol","Id",parameters->getString("codetypesol")), "parcelle");
////    }

////    void load_itinerary(string iditinerairetechnique) {
////        load_record(query("ItineraireTechnique","Id",iditinerairetechnique), "itinerairetechnique");
////    }

////    void load_meteo(string codestation, string start, string end) {
////        parameters->clearMeteo();
////        start = JulianDayConverter::toJulianDay( JulianDayConverter::toJulianDayNumber(start) - 1);
////        std::string meteo_query = query("meteo","codestation",codestation) +
////                " AND jour >= '" + start + "' AND jour <= '" +
////                end + "' ORDER BY jour ASC";
////        load_records(meteo_query);
////        std::cout << meteo_query << std::endl;
////    }

////    void load_meteo(string codestation, double startJulian, double endJulian) {
////        parameters->clearMeteo();
////        string start = JulianDayConverter::toJulianDayFmt( startJulian - 1, DATE_FORMAT_YMD);
////        string end = JulianDayConverter::toJulianDayFmt( endJulian, DATE_FORMAT_YMD );
////        std::string meteo_query = query("meteo","codestation",codestation) +
////                " AND jour >= '" + start + "' AND jour <= '" +
////                end + "' ORDER BY jour ASC";
////        load_records(meteo_query);
////        std::cout << meteo_query << std::endl;
////    }


//    vector<string> load_simulation_list(){}
//    vector<string>  load_variety_list(){}
//    vector<string>  load_station_list(){}
//    vector<string> load_plot_list(){}
//    vector<string>  load_itinerary_list(){}
//};


//#endif // FILELOADER_H
