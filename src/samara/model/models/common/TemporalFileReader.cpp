/**
 * @file TemporalFileReader.cpp
 * @author The Samara Development Team
 * See the AUTHORS file
 */

/*
 * Copyright (C) 2013-2017 ULCO http://www.univ-littoral.fr
 * Copyright (C) 2013-2017 Cirad http://www.cirad.fr
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

#include <samara/model/models/common/TemporalFileReader.hpp>

using namespace BOOST_SPIRIT;

namespace model { namespace models { namespace common {

TemporalFileReader::TemporalFileReader(
    const model::models::ModelParameters& params)
{
    std::string date = params.get < std::string > ("begin_date");

    if (not date.empty()) {
        mBeginTime = utils::DateTime::toJulianDayNumber(date);
    }
    mFile.open(getFilePath().c_str());
    if (not mFile) {
        throw utils::FileError(
            boost::format("Cannot open file : %1%") % mFilePath);
    }

    mDelay = params.get < double >("delay");
}

void TemporalFileReader::init(double time)
{
    if (mBeginTime != 0) {
        mDeltaTime = mBeginTime - time;
    }

    list_csv_item = not((confix_p('\"', *c_escape_ch_p, '\"')
                         | longest_d[real_p | int_p] | +alnum_p));
    list_csv = list_p(list_csv_item[push_back_a(mColumnValues)],
                      +chset_p(columnSeparator().c_str()));
}

bool TemporalFileReader::isGoodLine(double time)
{
    unsigned int _day = 0;
    unsigned int _week = 0;
    unsigned int _month = 0;
    unsigned int _year = 0;

    if (dayColumn() >= 0) {
        try {
            _day = boost::lexical_cast < unsigned int >(
                mColumnValues[dayColumn()]);
        } catch (...) {
            throw utils::ParseError("Bad format or bad index for day column");
        }
    }
    if (monthColumn() >= 0) {
        try {
            _month = boost::lexical_cast < unsigned int >(
                mColumnValues[monthColumn()]);
        } catch (...) {
            throw utils::ParseError("Bad format or bad index for month column");
        }
    }
    if (yearColumn() >= 0) {
        try {
            _year = boost::lexical_cast < unsigned int >(
                mColumnValues[yearColumn()]);
        } catch (...) {
            throw utils::ParseError("Bad format or bad index for year column");
        }
    }
    return (_year == 0 or utils::DateTime::year(time + mDelay) == _year)
        and (_month == 0 or utils::DateTime::month(time + mDelay) == _month)
        and (_week == 0 or utils::DateTime::weekOfYear(time + mDelay) == _week)
        and (_day == 0 or utils::DateTime::dayOfMonth(time + mDelay) == _day);
}

void TemporalFileReader::nextLine(double time)
{
    std::string line;
    bool found = false;
    bool stop = false;

    do {
        getline(mFile, line);
        if (not line.empty()) {
            if (parseLine(line)) {
                found = isGoodLine(time);
            } else {
                stop = true;
            }
        }
    } while (not stop and not found and not mFile.eof() and mFile.good());
    if (not found) {
        throw utils::ParseError(
            boost::format("Invalid date %1% or invalid data")
            % utils::DateTime::toJulianDay(time + mDelay));
    }
}

bool TemporalFileReader::parseLine(std::string& line)
{
    mColumnValues.clear();
    boost::trim(line);
    parse_info<> result = parse(line.c_str(), list_csv);
    if (not result.hit) {
        mColumnValues.clear();
    }
    return result.hit;
}

void TemporalFileReader::parseColumnHeader(const std::vector<std::string>& fields,
                              const std::string& colName,
                              const std::string& name)
{
    std::vector<std::string>::const_iterator it = find(fields.begin(),
                                                       fields.end(), colName);
    if (it != fields.end()) {
        addColumn(name, (int) distance(fields.begin(), it));
    } else {
        throw utils::ParseError(boost::format("Column %1% missing")
                                % colName);
    }
}

void TemporalFileReader::parseHeader(std::vector<std::string>& fields)
{
    std::string header;

    getline(getFile(), header);
    parseLine(fields, header);
}

void TemporalFileReader::parseLine(std::vector < std::string >& fields,
                                  std::string& line)
{
    boost::trim(line);
    rule<> list_csv, list_csv_item;
    list_csv_item = not(confix_p('\"', *c_escape_ch_p, '\"')
                        | +(alnum_p | ch_p('_') | ch_p('-'))
                        | longest_d[real_p | int_p]);
    list_csv = list_p(list_csv_item[push_back_a(fields)],
                      +chset_p(columnSeparator().c_str()));
    parse(line.c_str(), list_csv);
    for (std::vector<std::string>::iterator it = fields.begin();
         it != fields.end(); ++it) {
        if ((*it)[0] == '\"' and (*it)[it->size() - 1] == '\"') {
            *it = it->substr(1, it->size() - 2);
        }
    }
}

void TemporalFileReader::readLine(double time)
{
    nextLine(time);
    if (not mColumnValues.empty()) {
        for (std::map<std::string, int>::const_iterator it = mColumns.begin();
             it != mColumns.end(); ++it) {
            setVariable(it->first,
                        boost::lexical_cast<double>(
                            mColumnValues.at(it->second)));
        }
    }
}

} } }
