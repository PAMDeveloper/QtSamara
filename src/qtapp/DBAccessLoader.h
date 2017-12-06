#ifndef PARADOX_LOADER_H
#define PARADOX_LOADER_H

#include <iostream>
#include <libpq-fe.h>
#include <QtSql/QtSql>
#include <utils/simulationloader.h>
#include <parameters.h>
#include <utils/juliancalculator.h>
#include <QDebug>

using namespace std;


class DBAccessLoader : public AbstractSimulationLoader
{
public:
    QSqlDatabase db;

    DBAccessLoader(SamaraParameters * params) : AbstractSimulationLoader(params) {}

    ~DBAccessLoader() {
        QSqlDatabase::removeDatabase(db.connectionName());
    }

    void openDb(QString path) {
        if(db.isValid())
            QSqlDatabase::removeDatabase(db.connectionName());

        QString conStr = "Driver={Microsoft Access Driver (*.mdb, *.accdb)}; Dbq=" + path + ";";
        db = QSqlDatabase::addDatabase("QODBC", "SamaraDB");
        db.setDatabaseName(conStr);

        if(!db.open())
            qDebug() <<"Error: " << db.lastError() << "\n" << conStr;
    }

    double toJulianDay(QString date) {
        if(date.contains("T"))
            date = date.split("T")[0];
        if(date.contains("/"))
            date = date.replace("/","-");
//        qDebug() << "***********" << date;
        return JulianCalculator::toJulianDay(date.toStdString(), JulianCalculator::YMD, '-');
    }

    double toJulianDay(string date) {
        QString qDate = QString::fromStdString(date);
        if(qDate.contains("T"))
            qDate = qDate.split("T")[0];
        if(qDate.contains("/"))
            qDate = qDate.replace("/","-");
        return JulianCalculator::toJulianDay(qDate.toStdString(), JulianCalculator::YMD, '-');
    }

    string fromJulianDay(double date) {
        return JulianCalculator::toStringDate(date, JulianCalculator::YMD, '-');
    }





    QString query(QString table, QString key, QString value) {
        return "SELECT * FROM " + table + " WHERE " + key + "='" + value + "'";
    }

    QString list_query(QString table, QString key) {
        return "SELECT DISTINCT " + key + " FROM " + table;
    }

    vector<string> load_list(const QSqlDatabase & db, QString query) {
        QSqlQuery result(query, db);
        if(result.lastError().isValid())
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
        if(result.lastError().isValid())
            qDebug() << query << " >\n " << result.lastError().text();
        result.first();
        do {
            parameters->climatics.push_back(
                        Climate(
//                            result.value(1).toDate().toJulianDay(),
                            result.value(3).toDouble(),
                            result.value(2).toDouble(),
                            result.value(4).toDouble(),
                            result.value(6).toDouble(),
                            result.value(5).toDouble(),
                            result.value(7).toDouble(),
                            result.value(9).toDouble(),
                            result.value(11).toDouble(),
                            result.value(10).toDouble(),
                            result.value(8).toDouble(),
                            result.value(12).toDouble()
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
        if(result.lastError().isValid())
            qDebug() << query << " >\n " << result.lastError().text();
;        result.first();
        do {
            for (int col = 0; col < rec.count(); ++col) {
                QVariant::Type type = rec.field(col).type();
                QString key = rec.fieldName(col);
//                if(category == "variete") {
//                    if(     key == "IrrigAutoStop" ||
//                            key == "IrrigAutoResume" ||
//                            key == "FTSWIrrig" ||
//                            key == "TransplantingDepth" ||
//                            key == "Ca")
//                        continue;
//                }
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
                    QString val = result.value(col).toDate().toString("yyyy-MM-dd");
                    parameters->strings[key.toLower().toStdString()] =
                            pair <string, string> (
                                val.toStdString(), category.toStdString());
//                    qDebug() << result.value(col).toString();
                    parameters->doubles[key.toLower().toStdString()] =
                            pair <double, string> (
                                toJulianDay(val), category.toStdString());
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
//        for(auto pair: parameters->strings) {
//            qDebug() << QString::fromStdString(pair.first) << QString::fromStdString(pair.second.first);
//        }
        load_variety(parameters->getString("variety"));
        load_station(parameters->getString("wscode"));
        load_plot(parameters->getString("plotcode"));
        load_itinerary(parameters->getString("itkcode"));
        load_meteo(parameters->getString("wscode"),
                   parameters->getDouble("startingdate"),
                   parameters->getDouble("endingdate"));
    }

    vector<string> load_simulation_list() {
        return load_list(db, list_query("simulation","simcode"));
    }

    vector<string>  load_variety_list() {
        return load_list(db, list_query("variety","variety"));
    }

    vector<string>  load_station_list() {
        return load_list(db, list_query("ws","wscode")); //Nom
    }

    vector<string> load_plot_list() {
        return load_list(db, list_query("plotsim","plotcode"));
    }

    vector<string>  load_itinerary_list() {
        return load_list(db, list_query("itksim","itkcode"));
    }

    //@TODO bouger KPAR dans la station ou ailleurs
    void load_simulation(string idsimulation) {
        parameters->clearParameters("simulation");
        load_record(db, query("simulation","simcode",QString::fromStdString(idsimulation)), "simulation");
        parameters->doubles["kpar"] = pair <double, string> (0.5,"simulation");
    }

    void load_variety(string idvariete) {
        parameters->clearParameters("variete");
        load_record(db, query("variety","variety",QString::fromStdString(idvariete)), "variete");
    }

    void load_station(string idsite) {
//        parameters->clearParameters("site");
        parameters->clearParameters("station");
//        load_record(db, query("Site","Id",QString::fromStdString(idsite)), "site");
        load_record(db, query("ws","wscode",QString::fromStdString(parameters->getString("wscode"))), "station");
    }

    void load_plot(string idparcelle) {
        parameters->clearParameters("parcelle");
        parameters->clearParameters("soil");
        load_record(db, query("plotsim","plotcode",QString::fromStdString(idparcelle)), "parcelle");
        load_record(db, query("genesoil","soilcode",QString::fromStdString(parameters->getString("soilcode"))), "soil");
    }

    void load_itinerary(string iditinerairetechnique) {
        parameters->clearParameters("itinerairetechnique");
        load_record(db, query("itksim","itkcode",QString::fromStdString(iditinerairetechnique)), "itinerairetechnique");
    }

    void load_meteo(string codestation, double startJulian, double endJulian) {
        parameters->clearMeteo();
//        QString codeStationQuery = query("Site", "Id", QString::fromStdString(site));
//        QSqlQuery result(codeStationQuery, db);
//        result.first();
//        QString codestation = result.value("CodeStationMeteo").toString();

        string start = fromJulianDay(startJulian);
        string end = fromJulianDay(endJulian);

        std::string meteo_query = query("wdataday","wscode", QString::fromStdString(codestation)).toStdString() +
                " AND weatherdate >= (#" + start + "#)-1 AND weatherdate <= (#" +
                end + "#) ORDER BY weatherdate ASC";

        load_records(db, QString::fromStdString(meteo_query));
    }


    map<string, vector<double>> load_obs(string idsimulation) {
//        load_simulation(idsimulation);
        return load_obs(parameters->getString("plotcode"),
                        parameters->getString("variety"),
                        parameters->getString("itkcode"),
                        parameters->getString("wscode"),
                        parameters->getDouble("startingdate"),
                        parameters->getDouble("endingdate"));
    }

    map<string, vector<double>> load_obs(string plot, string variety, string itinerary, string codestation, double startJulian, double endJulian) {
        QString gQuery = "SELECT DISTINCT variety, varcode FROM plotcrop WHERE variety = '"
                + QString::fromStdString(variety) + "'";
        QSqlQuery gResult(gQuery, db);
        if(gResult.lastError().isValid())
            qDebug() << gQuery << " >\n " << gResult.lastError().text();
        gResult.first();
        map<string, vector<double>> resultMap;
        QString varCode = gResult.value(1).toString();
        QString trialCode = QString::fromStdString(itinerary) + varCode;
        QString start = QString::fromStdString(fromJulianDay(startJulian));
        QString end = QString::fromStdString(fromJulianDay(endJulian));
        QString query =
                "SELECT Observation_resultat.Id as id, Observation_resultat.obsplantdate as Jour, " \
                "Observation_resultat.DAP as NbJas, [Observation_resultat]![Pl_Height]*10 AS PlantHeight, " \
                "Observation_resultat.lai as Lai, Observation_resultat.GrainYield as GrainYieldPop, " \
                "Observation_resultat.AppLeaves as AppLeaves, Observation_resultat.AppTill as CulmsPerPlant, " \
                "[Observation_resultat]![GrainYield]/[Observation_resultat]![TotBiom] AS HaunIndex, " \
                "Observation_resultat.PanGrainNb as PanGrainNb,  [Observation_resultat]![StemDM]*10+" \
                "[Observation_resultat]![leafDM]*10+IIf(IsNull([Observation_resultat]![PanicleDM]*10),0," \
                "[Observation_resultat]![PanicleDM]*10) AS DryMatAboveGroundPop, [Observation_resultat]" \
                "![lai]/([Observation_resultat]![leafDM]*10) AS Sla, [Observation_resultat.leafDM]*10 as DryMatStructLeafPop, " \
                "[Observation_resultat.StemDM]*10 as DryMatStructStemPop, " \
                "[Observation_resultat.PanicleDM]*10 as DryMatStructPaniclePop FROM Observation_resultat " \
                " WHERE id='" + trialCode + "'" \
                " AND Observation_resultat.obsplantdate > (#" + start + "#)-1" +
                " AND Observation_resultat.obsplantdate < (#" + end + "#)";

        QSqlQuery result(query, db);
        QSqlRecord rec = result.record();
        if(result.lastError().isValid())
            qDebug() << query << " >\n " << result.lastError().text();
        result.first();
        for (int col = 0; col < rec.count(); ++col) {
            QString key = rec.fieldName(col);
            vector<double> values;
            resultMap.insert(pair<string, vector<double>>(key.toStdString(),values));
        }

        do {
            for (int col = 0; col < rec.count(); ++col) {
                QVariant::Type type = rec.field(col).type();
                string key = rec.fieldName(col).toStdString();
                if(type == QVariant::String) {
                    if(!result.value(col).isNull())
                        resultMap[key].push_back(result.value(col).toDouble());
                    else
                        resultMap[key].push_back(-999);
                } else if (type == QVariant::Date || type == QVariant::DateTime) {
                   resultMap[key].push_back(toJulianDay(result.value(col).toString()));
                } else {
                    if(!result.value(col).isNull())
                        resultMap[key].push_back(result.value(col).toDouble());
                    else
                        resultMap[key].push_back(-999);
                }
            }
        } while (result.next());
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
