#ifndef PSQLLOADER_H
#define PSQLLOADER_H

#include <libpq-fe.h>
#include "../parameters.h"
#include "../utils/DateTime.h"

#define PSQLTYPE_STRING 1043
#define PSQLTYPE_DATE 1082


using namespace std;

class PSQLLoader
{
public:
    PGconn * db;
    SamaraParameters * parameters;

    PSQLLoader(SamaraParameters * params) {
        string connection_string = "host=localhost port=5432 dbname=samara user=user_samara password=samarapassword";
        db = PQconnectdb(connection_string.c_str());
        parameters = params;
    }

    string query(string table, string key, string value) {
        return "SELECT * FROM " + table + " WHERE " + key + " = '" + value + "'";
    }

    void load_complete_simulation(string idsimulation) {
        load_simulation(idsimulation);
        load_variety(parameters->getString("idvariete"));
        load_station(parameters->getString("codestation"));
        load_plot(parameters->getString("idparcelle"));
        load_itinerary(parameters->getString("iditinerairetechnique"));
        load_meteo(parameters->getString("codestation"),
                   parameters->getString("datedebut"),
                   parameters->getString("datefin"));
    }

    //@TODO bouger KPAR dans la station ou ailleurs
    void load_simulation(string idsimulation) {
        load_record(query("simulation","idsimulation",idsimulation), "simulation");
    }

    void load_variety(string idvariete) {
        load_record(query("variete","idvariete",idvariete), "variete");
    }

    void load_station(string codestation) {
        load_record(query("station","codestation",codestation), "station");
    }

    void load_plot(string idparcelle) {
        load_record(query("parcelle","idparcelle",idparcelle), "parcelle");
        load_record(query("typesol","codetypesol",parameters->getString("codetypesol")), "parcelle");
    }

    void load_itinerary(string iditinerairetechnique) {
        load_record(query("itinerairetechnique","iditinerairetechnique",iditinerairetechnique), "itinerairetechnique");
    }

    void load_meteo(string codestation, string start, string end) {
        std::string meteo_query = query("meteo","codestation",codestation) +
                " AND jour >= '" + start + "' AND jour <= '" +
                end + "' ORDER BY jour ASC";
        load_records(meteo_query);
    }

    void load_records(string query) {
        PGresult* result = PQexec(db, query.c_str());
        if (PQresultStatus(result) != PGRES_TUPLES_OK){
            cout << "Error: " << PQerrorMessage(db) << endl;
        }

        for (int row = 0; row < PQntuples(result); ++row) {
            parameters->climatics.push_back(
                        Climate(
                            atof(PQgetvalue(result, row, 2)),
                            atof(PQgetvalue(result, row, 3)),
                            atof(PQgetvalue(result, row, 4)),
                            atof(PQgetvalue(result, row, 5)),
                            atof(PQgetvalue(result, row, 6)),
                            atof(PQgetvalue(result, row, 7)),
                            atof(PQgetvalue(result, row, 8)),
                            atof(PQgetvalue(result, row, 9)),
                            atof(PQgetvalue(result, row, 10)),
                            atof(PQgetvalue(result, row, 11)),
                            atof(PQgetvalue(result, row, 12))
                            )
                        );
        }
    }

    void load_record(string table, string key, string value) {
        load_record(query(table,key,value), table);
    }

    void load_record(string query, string category) {
        PGresult* result = PQexec(db, query.c_str());
        if (PQresultStatus(result) != PGRES_TUPLES_OK){
            cout << "Error: " << PQerrorMessage(db) << endl;
        }

        for (int col = 0; col < PQnfields(result); ++col) {
            string key = string(PQfname(result,col));
            if( PQftype(result,col) == PSQLTYPE_STRING ){
                parameters->strings[key] =  pair <string, string> (string(PQgetvalue(result,0,col)), category);
            } else if( PQftype(result,col) == PSQLTYPE_DATE ){
                parameters->strings[key] =  pair <string, string> (string(PQgetvalue(result,0,col)), category);
                parameters->doubles[key] =  pair <double, string> (DateTime::toJulianDayNumber(string(PQgetvalue(result,0,col))), category);

            } else {
                parameters->doubles[key] =  pair <double, string> ( atof(PQgetvalue(result,0,col)), category);
            }
        }
    }
};

#endif // PSQLLOADER_H
