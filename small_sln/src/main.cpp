/**
 * @file app/main.cpp
 * @author The Ecomeristem Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2005-2017 Cirad http://www.cirad.fr
 * Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
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

#include "delphirunner.h"
#include "ParametersReader.hpp"
#include <chrono>
#include <time.h>
#include <ctime>

struct GlobalParameters
{ };


int main(int argc, char *argv[]) {
    GlobalParameters globalParameters;
    samara::ModelParameters parameters;
    utils::ParametersReader reader;
    std::string id = "06SB15-fev13-D1_SV21";
    parameters.set< std::string>("idsimulation", id);
    reader.loadFromDatabase(id, parameters);

   /* QDate start = QDate::fromString(QString::fromStdString(parameters.get < std::string >("datedebut")), "yyyy-MM-dd");
    QDate end = QDate::fromString(QString::fromStdString(parameters.get < std::string >("datefin")), "yyyy-MM-dd");*/
	parameters.beginDate = 655565; // start.toJulianDay();

    std::chrono::time_point<std::chrono::system_clock> startC, endC;
    startC = std::chrono::system_clock::now();

    for (int i = 0; i < 10; ++i) {
       // run_samara_21(start.toJulianDay(), end.toJulianDay(), parameters);
    }

    endC = std::chrono::system_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
            (endC-startC).count() / 10;
    std::time_t end_time = std::chrono::system_clock::to_time_t(endC);
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds << "ms\n";
    exit(0);
}
