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

#include <qtapp/mainwindow.h>
#include <QApplication>
#include <QDebug>
#include <QDate>

#include <defines.hpp>
#include <observer/GlobalView.hpp>
#include <models/Model2_1.hpp>
#include <utils/ParametersReader.hpp>
#include "../delphi2cpp/delphirunner.h"

void runDelphiTrad(double start, double end, const samara::ModelParameters &parameters);


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    GlobalParameters globalParameters;
    samara::ModelParameters parameters;
    utils::ParametersReader reader;
    std::string id = "06SB15-fev13-D1_SV21";
    parameters.set< std::string>("idsimulation", id);
    reader.loadFromDatabase(id, parameters);

    //    parameters.set < std::string >("datefin", "2011-10-21");

    QDate start = QDate::fromString(QString::fromStdString(parameters.get < std::string >("datedebut")),
                                    "yyyy-MM-dd");
    QDate end = QDate::fromString(QString::fromStdString(parameters.get < std::string >("datefin")),
                                  "yyyy-MM-dd");
    parameters.beginDate = start.toJulianDay();
    qDebug() << parameters.beginDate << end.toJulianDay();
    SamaraContext context(start.toJulianDay(), end.toJulianDay());

    ::Trace::trace().clear();
    SamaraSimulator simulator(new SamaraModel2_1, globalParameters);
    observer::GlobalView *view = new observer::GlobalView();
    simulator.attachView("plant", view);
    simulator.init(start.toJulianDay(), parameters);
    simulator.run(context);
//    w.show_trace();


    std::chrono::time_point<std::chrono::system_clock> startC, endC;
    startC = std::chrono::system_clock::now();

    for (int i = 0; i < 1000; ++i) {
        run_samara_21(start.toJulianDay(), end.toJulianDay(), parameters);
    }

    endC = std::chrono::system_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::microseconds>
            (endC-startC).count() / 1000;
    std::time_t end_time = std::chrono::system_clock::to_time_t(endC);
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds << "microsec\n";
//    exit(0);


    std::string dirName = "D:/PAMStudio_dev/data/samara/06SB15-fev13-D1_SV21.txt";
    w.displayData(view, QString::fromStdString(dirName), &parameters,
                  QString::fromStdString(parameters.get < std::string >("datedebut")),
                  QString::fromStdString(parameters.get < std::string >("datefin")));
    return a.exec();
}
