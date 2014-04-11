/**
 * @file utils/Connections.hpp
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

#ifndef UTILS_CONNECTIONS_HPP
#define UTILS_CONNECTIONS_HPP

#include <pqxx/pqxx>

namespace utils {

class Connections
{
    typedef std::map < std::string, pqxx::connection* > connections_t;

public:
    inline static pqxx::connection& connection(const std::string& database,
                                               const std::string& request)
    {
        connections_t::const_iterator it =
            mConnections.connections.find(database);

        if (it == mConnections.connections.end()) {
            mConnections.connections[database] = new pqxx::connection(request);
            it = mConnections.connections.find(database);
        }
        return *(it->second);
    }

private:
    Connections();

    virtual ~Connections()
    {
        for (connections_t::const_iterator it =
                 mConnections.connections.begin();
             it != mConnections.connections.end(); ++it) {
            delete it->second;
        }
        mConnections.connections.clear();
    }

    static Connections mConnections;

    connections_t connections;
};

} // namespace utils

#endif
