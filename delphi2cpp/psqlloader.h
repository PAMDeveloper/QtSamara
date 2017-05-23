#ifndef PSQLLOADER_H
#define PSQLLOADER_H
#include <libpq-fe.h>
#include "parameters.h"

using namespace std;

class PSQLLoader
{
public:
    PGconn * db;
    SamaraParameters * parameters;

    PSQLLoader() {
        string connection_string = "host=localhost port=5432 dbname=samara user=user_samara password=samarapassword";
        db = PQconnectdb(connection_string.c_str());
        parameters = new SamaraParameters();
    }

    void load_simulation(string id);
    void load_variety(string id);
    void load_plot(string id);
    void load_itinerary(string id);
    void load_meteo(string id, string start, string end);

    void get_record(string query, string category) {
        PGresult* result = PQexec(db, query.c_str());
        if (PQresultStatus(result) != PGRES_TUPLES_OK){
            cout << "Error: " << PQerrorMessage(db) << endl;
        }

        for (int col = 0; col < PQnfields(result); ++col) {
            string key = string(PQfname(result,col));
            if((PQftype(result,col) == 1043) || (PQftype(result,col) == 1082)){
                parameters->strings[key] =  pair <string, string> (string(PQgetvalue(result,0,col)), category);
            }
            else {
                parameters->doubles[key] =  pair <double, string> ( atof(PQgetvalue(result,0,col)), category);
            }
        }
    }
};

#endif // PSQLLOADER_H
