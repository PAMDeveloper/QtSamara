/**
 * @file ModelParameters.hpp
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

#ifndef MODELPARAMETERS_HPP
#define MODELPARAMETERS_HPP 1

//#define OPTIM_NO_LEXCAST

#include <map>
#include <string>
#include <vector>
#include <iso646.h>
#include <iostream>
#include <sstream>

namespace samara {

struct Climate {
  double TMax;
  double TMin;
  double TMoy;
  double HMax;
  double HMin;
  double HMoy;
  double Vt;
  double Ins;
  double Rg;
  double ETP;
  double Rain;

  Climate(double TMax, double TMin, double TMoy, double HMax, double HMin,
          double HMoy, double Vt, double Ins, double Rg,
          double Rain) :
    TMax(TMax), TMin(TMin), TMoy(TMoy), HMax(HMax), HMin(HMin),
    HMoy(HMoy), Vt(Vt), Ins(Ins), Rg(Rg), ETP(-999), Rain(Rain)
  { }
};


class ModelParameters {
public:
  /**
   * Default constructor.
   */
  ModelParameters()
  { }

  /**
   * Destructor.
   */
  virtual ~ModelParameters()
  { }

  /**
   * Get the requested parameter.
   * @param[in] paramName The name of the needed parameter.
   * @return T The data if readable for this type, an empty data otherwise.
   */
  template < typename T >
  T get(const std::string &paramName) const {
    std::map < std::string, std::string >::const_iterator it;
    it = mParams.find(paramName);

    if (it == mParams.end())
      std::cout << "Warning: no value for " << paramName << std::endl;

    return (T)((it == mParams.end()) ? "0" : it->second);
  }

     template <>
     double get( const std::string &paramName ) const
     {
        std::map < std::string, std::string >::const_iterator it;
        it = mParams.find( paramName );

        if( it == mParams.end() )
           std::cout << "Warning: no value for " << paramName << std::endl;

  	  return (it == mParams.end()) ? 0. : std::stod(it->second.c_str());
     }

     template <>
     int get( const std::string &paramName ) const
     {
        std::map < std::string, std::string >::const_iterator it;
        it = mParams.find( paramName );

        if( it == mParams.end() )
           std::cout << "Warning: no value for " << paramName << std::endl;

        return ( it == mParams.end() ) ? 0 : atoi( it->second.c_str() );
     }

	 template <>
	 std::string get(const std::string &paramName) const
	 {
		 std::map < std::string, std::string >::const_iterator it;
		 it = mParams.find(paramName);

		 if (it == mParams.end())
			 std::cout << "Warning: no value for " << paramName << std::endl;

		 return (it == mParams.end()) ? "0" : it->second.c_str();
	 }

  Climate get(double time) const {
    return meteoValues[time - beginDate];
  }

  /**
   * Set (update or create) the given parameter/
   * @param[in] key   The parameter name.
   * @param[in] value The value of this parameter.
   * @return void.
   */
  template < typename T >
  inline void set(const std::string &key, const T &value) {
    mParams[key] = (std::string)(value);
  }

  template <>
  inline void set(const std::string &key, const  std::string &value) {
	  mParams[key] = value;
  }

  template <>
  inline void set(const std::string &key, const double &value) {
	  std::ostringstream strs;
	  strs << value;
	  std::string str = strs.str();
	  mParams[key] = str;
  }
  template < typename T >
  inline void set(const std::string &key, const int &value) {
	  std::ostringstream strs;
	  strs << value;
	  std::string str = strs.str();
	  mParams[key] = str;
  }
  /**
   * Removes all parameters.
   * @param void.
   * @return void.
   */
  inline void clear() {
    mParams.clear();
  }


  std::vector < Climate > meteoValues;
public:
  std::map < std::string, std::string > *getRawParameters() { return &mParams; }
  std::vector < Climate > *getMeteoValues() { return &meteoValues; }
  double beginDate;
private:
  std::map < std::string, std::string > mParams;//!< Represent the parameters.
};

}

#endif
