/**
 * @file TemporalFileReader.hpp
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

#ifndef SAMARA_CLIMATE_FILE_READER_HPP
#define SAMARA_CLIMATE_FILE_READER_HPP

#include <samara/utils/DateTime.hpp>

#include <fstream>
#include <map>

#if BOOST_VERSION < 103600

#include <boost/spirit/utility/chset.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/spirit/utility/escape_char.hpp>

namespace BOOST_SPIRIT = boost::spirit;

#else

#include <boost/spirit/include/classic_chset.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_confix.hpp>
#include <boost/spirit/include/classic_lists.hpp>
#include <boost/spirit/include/classic_escape_char.hpp>

namespace BOOST_SPIRIT = BOOST_SPIRIT_CLASSIC_NS;

#endif

#include <samara/model/models/ModelParameters.hpp>
// #include <model/models/IModel.hpp>
// #include <utils/Exception.hpp>

namespace model { namespace models { namespace common {

class TemporalFileReader
{
public:
    TemporalFileReader(const model::models::ModelParameters& parameters);

    virtual ~TemporalFileReader()
    {
        if (mFile) {
            mFile.close();
        }
    }

    virtual void compute(double time)
    { readLine(time + mDelay); }

    /**
     * @fn getValue
     * Get the requested value.
     *
     * @param[in] name The name of the needed value.
     * @return The needed value.
     */
    double getValue(const std::string& name) const
    { return mValues.at(name); }

    virtual void init(double time);

protected:
    typedef std::map < std::string, double > values_t;
    typedef std::map < std::string, int > columns_t;
    typedef std::vector < std::string > columnValues_t;

    void addColumn(const std::string& name, int index)
    { mColumns[name] = index; }

    void createVariables()
    {
        for (columns_t::const_iterator it = mColumns.begin();
             it != mColumns.end(); ++it) {
            createVariable(it->first);
        }
    }

    double getBeginTime() const
    { return mBeginTime; }

    std::ifstream& getFile()
    { return mFile; }

    const std::string& getFilePath() const
    { return mFilePath; }

    void jumpLines(unsigned int n)
    {
        std::string line;

        for (boost::uint32_t i = 0; i < n; ++i) {
            getline(getFile(), line);
        }
    }

    void parseColumnHeader(const std::vector<std::string>& fields,
                           const std::string& colName,
                           const std::string& name);

    void parseHeader(std::vector<std::string>& fields);

    /*  - - - - - - - - - - - - - --ooOoo-- - - - - - - - - - - -  */

    virtual std::string columnSeparator() const
    { return " "; }

    virtual std::string dateFormat() const
    { return ""; }

    virtual int yearColumn() const
    { return -1; }

    virtual int monthColumn() const
    { return -1; }

    virtual int weekColumn() const
    { return -1; }

    virtual int dayColumn() const
    { return -1; }

private:
    void createVariable(const std::string& name)
    { mValues[name] = 0.0; }

    void setVariable(const std::string& name, double value)
    { mValues[name] = value; }

    bool isGoodLine(double time);

    void nextLine(double time);
    bool parseLine(std::string& line);
    void parseLine(std::vector<std::string>& fields, std::string& line);
    void readLine(double time);

// parameters
    double      mBeginTime;
    double      mDeltaTime;
    double      mDelay;
    std::string mFilePath;

    std::ifstream  mFile;
    columns_t      mColumns;
    columnValues_t mColumnValues;
    values_t       mValues;

    BOOST_SPIRIT::rule<> list_csv;
    BOOST_SPIRIT::rule<> list_csv_item;
};

} } }

#endif
