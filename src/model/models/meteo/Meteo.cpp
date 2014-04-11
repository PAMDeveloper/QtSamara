/**
 * @file model/models/meteo/Meteo.cpp
 * @author The TNT Development Team
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

#include <model/models/meteo/Meteo.hpp>

#include <utils/Connections.hpp>

namespace model { namespace models { namespace meteo {

Meteo::Meteo()
{
}

void Meteo::compute(double /* t */)
{
    if (it == values.end()) {
        it = values.begin();
    } else {
        ++it;
    }
}

void Meteo::init(const model::models::ModelParameters& parameters)
{
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
            std::string request =
                (boost::format("SELECT * FROM \"meteorology\" "         \
                               "WHERE \"CodeStation\"='%1%' and "       \
                               "\"Jour\" like \'%%%2%%%\' order by "    \
                               "\"Jour\" asc") %
                 parameters.get < std::string >("CodeStationMeteo") %
                 year).str();
            pqxx::result result = action.exec(request);

            if (not result.empty()) {
                for (pqxx::result::const_iterator it = result.begin();
                     it != result.end(); ++it) {
                    std::string day;
                    double t;

                    utils::DateTime::format_date(
                        boost::lexical_cast < std::string >(it->at(1)), day);
                    t = utils::DateTime::toJulianDayNumber(day);
                    if (t >= begin and t <= end) {
                        values.push_back(
                            Climate(
                                boost::lexical_cast < double >(it->at(2)),
                                boost::lexical_cast < double >(it->at(3)),
                                boost::lexical_cast < double >(it->at(4)),
                                boost::lexical_cast < double >(it->at(5)),
                                boost::lexical_cast < double >(it->at(6)),
                                boost::lexical_cast < double >(it->at(7)),
                                boost::lexical_cast < double >(it->at(8)),
                                boost::lexical_cast < double >(it->at(9)),
                                boost::lexical_cast < double >(it->at(10))));
                    }
                }
            }
        }
    } catch (pqxx::sql_error e) {
        std::cout << "Error: " << e.query() << std::endl;
    }
    it = values.end();
}

} } }

