/**
 * @file Exception.hpp
 * @author The TNT Development Team
 * See the AUTHORS or Authors.txt file
 */

/*
 * Copyright (C) 2012-2013 ULCO http://www.univ-littoral.fr
 * Copyright (C) 2012-2013 INRA http://www.inra.fr
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

#ifndef TNT_EXCEPTION_HPP
#define TNT_EXCEPTION_HPP 1

#include <boost/format.hpp>
#include <stdexcept>

namespace utils {

class BaseError : public std::runtime_error
{
public:
    explicit BaseError(const std::string& argv = std::string())
        : std::runtime_error(argv)
    { }

    explicit BaseError(const boost::format& argv)
        : std::runtime_error(argv.str())
    { }
};

class FileError : public BaseError
{
public:
    explicit FileError(const std::string& argv = std::string())
        : BaseError(argv)
    { }

    explicit FileError(const boost::format& argv)
        : BaseError(argv)
    { }
};

class ParseError : public BaseError
{
public:
    explicit ParseError(const std::string& argv = std::string())
        : BaseError(argv)
    { }

    explicit ParseError(const boost::format& argv)
        : BaseError(argv)
    { }
};

class ReadError : public BaseError
{
public:
    explicit ReadError(const std::string& argv = std::string())
        : BaseError(argv)
    { }

    explicit ReadError(const boost::format& argv)
        : BaseError(argv)
    { }
};

class InvalidFileFormat : public BaseError
{
public:
    explicit InvalidFileFormat(const std::string& argv = std::string())
        : BaseError(argv)
    { }

    explicit InvalidFileFormat(const boost::format& argv)
        : BaseError(argv)
    { }
};

class ErrorInContextFile : public BaseError
{
public:
    explicit ErrorInContextFile(const std::string& argv = std::string())
        : BaseError(argv)
    { }

    explicit ErrorInContextFile(const boost::format& argv)
        : BaseError(argv)
    { }
};

}

#endif
