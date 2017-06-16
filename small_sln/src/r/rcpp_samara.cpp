/**
 * @file rcpp_samara.cpp
 * @author See the AUTHORS file
 */

/*
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

#include <Rcpp.h>
#include <Rinternals.h>
#include <cmath>
#include "rsamara_types.hpp"


using namespace Rcpp;


XPtr < Context > init_simulation_from_psql(Rcpp::String name) {}
XPtr < Context > init_simulation_from_folder(Rcpp::String path) {}

//load_simulation(idsimulation);
//load_variety(parameters->getString("idvariete"));
//load_station(parameters->getString("codestation"));
//load_plot(parameters->getString("idparcelle"));
//load_itinerary(parameters->getString("iditinerairetechnique"));
//load_meteo(parameters->getString("codestation"),
//           parameters->getString("datedebut"),
//           parameters->getString("datefin"));

List getDBSimulationDF(Rcpp::String idsimulation) {}
List getDBVarietyDF(Rcpp::String idvariete) {}
List getDBVarietyDFfromSim(Rcpp::String idsimulation) {}
List getDBItineraryDF(Rcpp::String iditinerairetechnique) {}
List getDBItineraryDFfromSim(Rcpp::String idsimulation) {}
List getDBStationDF(Rcpp::String codestation) {}
List getDBStationDFfromSim(Rcpp::String idsimulation) {}
List getDBPlotDF(Rcpp::String idparcelle) {}
List getDBPlotDFfromSim(Rcpp::String idsimulation) {}
List getDBMeteoDF(Rcpp::String codestation, Rcpp::String beginDate, Rcpp::String endDate) {}
List getDBMeteoDFfromSim(Rcpp::String idsimulation) {}
List getFileParameters(Rcpp::String filepath, Rcpp::String id){}

//// [[Rcpp::export]]
//List run(Rcpp::String from_date, Rcpp::String to_date, List meteo, List plot, List crop, List itinerary, List result_variables, int every_steps)
//{
//    model::models::ModelParameters parameters;

//    CharacterVector names = dfParameters[0];
//    CharacterVector values = dfParameters[1];
//    for (int i = 0; i < names.size(); ++i) {
//      //std::cout << names(i) << " " << values(i) << std::endl;
//        parameters.getRawParameters()->insert(
//            std::pair<std::string,std::string>(
//                Rcpp::as<std::string>(names(i)),
//                Rcpp::as<std::string>(values(i))
//            )
//            );
//    }

//    NumericVector TMax = dfMeteo[0];
//    NumericVector TMin = dfMeteo[1];
//    NumericVector TMoy = dfMeteo[2];
//    NumericVector HMax = dfMeteo[3];
//    NumericVector HMin = dfMeteo[4];
//    NumericVector HMoy = dfMeteo[5];
//    NumericVector Vt = dfMeteo[6];
//    NumericVector Ins = dfMeteo[7];
//    NumericVector Rg = dfMeteo[8];
//    NumericVector ETP = dfMeteo[9];
//    NumericVector Rain = dfMeteo[10];

//    for (int i = 0; i < TMax.size(); ++i) {
//      model::models::Climate c(TMax(i), TMin(i), TMoy(i), HMax(i), HMin(i), HMoy(i), Vt(i), Ins(i), Rg(i), Rain(i));
//      parameters.meteoValues.push_back(c);
//    }

//    /*std::cout << parameters.get<std::string>("datedebut") << "\n"
//                << parameters.get<std::string>("datefin") << std::endl;*/
//    double begin = utils::DateTime::toJulianDayNumber(
//      parameters.get<std::string>("datedebut"));
//    double end = utils::DateTime::toJulianDayNumber(
//      parameters.get<std::string>("datefin"));
//    model::kernel::Simulator * simulator = new model::kernel::Simulator(model, globalParameters);
//    simulator->attachView("global", new model::observer::GlobalView());
//    simulator->init(begin, parameters);
//    simulator->run(begin, end);


//    for(auto it = parameters.getRawParameters()->begin(); it != parameters.getRawParameters()->end(); ++it) {
//      std::cout << it->first << " " << it->second << "\n";
//    }

//    const model::observer::Observer& observer = simulator->observer();
//    const model::observer::Observer::Views& views = observer.views();
//    Rcpp::List gresult(views.size());
//    Rcpp::CharacterVector gnames;
//    unsigned int gindex = 0;

//    for (model::observer::Observer::Views::const_iterator it = views.begin();
//         it != views.end(); ++it) {
//        Rcpp::List result(it->second->values().size() + 1);
//        Rcpp::CharacterVector names;
//        model::observer::View::Values values = it->second->values();
//        double begin = it->second->begin();
//        double end = it->second->end();

//        gnames.push_back(it->first);
//        // write header
//        names.push_back("time");
//        for (model::observer::View::Values::const_iterator
//             itv = values.begin(); itv != values.end(); ++itv) {
//            names.push_back(itv->first);
//        }
//        // write dates
//        {
//            Rcpp::CharacterVector values;

//            for (double t = begin; t <= end; ++t) {
//                values.push_back(utils::DateTime::toJulianDay(t));
//            }
//            result[0] = values;
//        }
//        // write values
//        unsigned int index = 1;

//        for (model::observer::View::Values::const_iterator itv =
//             values.begin(); itv != values.end(); ++itv) {
//            model::observer::View::Value::const_iterator itp =
//                    itv->second.begin();
//            Rcpp::NumericVector values;

//            for (double t = begin; t <= end; ++t) {
//                while (itp != itv->second.end() and itp->first < t) {
//                    ++itp;
//                }
//                if (itp != itv->second.end()) {
//                    values.push_back(
//                                boost::lexical_cast < double >(itp->second));
//                } else {
//                    values.push_back(NumericVector::get_na());
//                }
//            }
//            result[index] = values;
//            ++index;
//        }
//        DataFrame out(result);

//        out.attr("names") = names;
//        gresult[gindex] = out;
//        ++gindex;
//    }

//    gresult.attr("names") = gnames;
//    return gresult;
////return 0;
//}

//// [[Rcpp::export]]
//List getParameters_from_database(Rcpp::String name)
//{
//    model::models::ModelParameters parameters;
//    utils::ParametersReader reader;
//    reader.loadFromDatabase(name, parameters);

//    std::map < std::string, std::string > * paramMap = parameters.getRawParameters();
//    Rcpp::List result(paramMap->size());
//    Rcpp::CharacterVector names;
//    Rcpp::CharacterVector values;
//    for(auto const& it: *paramMap){
//        std::string key = it.first;
//        std::string value = it.second;
//        names.push_back(key);

//        /*try
//        {
//          double val = round(parameters.get<double>(key), 4);
//          char buffer[32];
//          snprintf(buffer, sizeof(buffer), "%g", val);
//          values.push_back(buffer);
//          std::cout << key << " " << buffer << std::endl;
//        }
//        catch(...) {
//          std::string v = parameters.get<std::string>(key);
//          values.push_back(value);
//        }*/
//        values.push_back(value);

//    }

//    DataFrame df = DataFrame::create(Named("Name")=names,Named("Values")=values);
//    return df;
//}


//// [[Rcpp::export]]
//List getMeteo_from_database(Rcpp::String name)
//{
//    model::models::ModelParameters parameters;
//    utils::ParametersReader reader;
//    reader.loadFromDatabase(name, parameters);

//    std::vector < model::models::Climate > meteoValues = parameters.getMeteoValues();
//    Rcpp::List result(meteoValues.size());
//    CharacterVector names =  CharacterVector::create("TMax", "TMin", "TMoy", "HMax", "HMin", "HMoy", "Vt", "Ins", "Rg", "ETP", "Rain");
//    NumericVector TMax, TMin, TMoy, HMax, HMin, HMoy, Vt, Ins, Rg, ETP, Rain;

//    for(auto const& it: meteoValues){
//        TMax.push_back(it.TMax);
//        TMin.push_back(it.TMin);
//        TMoy.push_back(it.TMoy);
//        HMax.push_back(it.HMax);
//        HMin.push_back(it.HMin);
//        HMoy.push_back(it.HMoy);
//        Vt.push_back(it.Vt);
//        Ins.push_back(it.Ins);
//        Rg.push_back(it.Rg);
//        ETP.push_back(it.ETP);
//        Rain.push_back(it.Rain);
//    }

//    DataFrame df = DataFrame::create(
//                Named("TMax")=TMax,
//                Named("TMin")=TMin,
//                Named("TMoy")=TMoy,
//                Named("HMax")=HMax,
//                Named("HMin")=HMin,
//                Named("HMoy")=HMoy,
//                Named("Vt")=Vt,
//                Named("Ins")=Ins,
//                Named("Rg")=Rg,
//                Named("ETP")=ETP,
//                Named("Rain")=Rain
//            );
//    return df;
//}

//// [[Rcpp::export]]
//XPtr < Context > rcpp_init_from_database(Rcpp::String name)
//{
//    Context* context = new Context;
//    samara::GlobalParameters globalParameters;
//    model::kernel::KernelModel* model = new model::kernel::KernelModel;
//    model::models::ModelParameters parameters;
//    utils::ParametersReader reader;
//    std::string begin;
//    std::string end;

//    reader.loadFromDatabase(name, parameters);
//    begin = parameters.get<std::string>("datedebut");
//    end = parameters.get<std::string>("datefin");
//    globalParameters.modelVersion = parameters.get < std::string >("idmodele");

//    context->begin = utils::DateTime::toJulianDayNumber(begin);
//    context->end = utils::DateTime::toJulianDayNumber(end);
//    context->simulator = new model::kernel::Simulator(model, globalParameters);

//    context->simulator->attachView("global", new model::observer::GlobalView());
//    context->simulator->init(context->begin, parameters);
//    return XPtr < Context >(context, true);
//}


////// [[Rcpp::export]]
////XPtr < Context > rcpp_init_from_json(Rcpp::String json)
////{
////    Context* context = new Context;
////    samara::GlobalParameters globalParameters;
////    model::kernel::KernelModel* model = new model::kernel::KernelModel;
////    model::models::ModelParameters parameters;
////    utils::ParametersReader reader;
////    std::string begin;
////    std::string end;

////    reader.loadFromJSON(json, parameters);
////    format_dates(parameters, begin, end);
////    globalParameters.modelVersion = parameters.get < std::string >("IdModele");

////    context->begin = utils::DateTime::toJulianDayNumber(begin);
////    context->end = utils::DateTime::toJulianDayNumber(end);
////    context->simulator = new model::kernel::Simulator(model, globalParameters);

////    context->simulator->attachView("global", new model::observer::GlobalView);
////    context->simulator->init(context->begin, parameters);
////    return XPtr < Context >(context, true);
////}

////// [[Rcpp::export]]
////XPtr < Context > rcpp_init_from_dataframe(Rcpp::List data)
////{
////    Context* context = new Context;
////    samara::GlobalParameters globalParameters;
////    model::kernel::KernelModel* model = new model::kernel::KernelModel;
////    model::models::ModelParameters parameters;
////    utils::ParametersReader reader;
////    std::string begin;
////    std::string end;
////    boost::property_tree::ptree tree;
////    CharacterVector names = data.attr("names");

////    for (unsigned int i = 0; i < names.size(); ++i) {
////        std::string key = Rcpp::as < std::string >(names[i]);
////        SEXP s = data[i];

////        if (TYPEOF(s) == 14) {
////            tree.put(key, Rcpp::as < double >(data[i]));
////        } else if (TYPEOF(s) == 16) {
////            tree.put(key, Rcpp::as < std::string >(data[i]));
////        } else if (TYPEOF(s) == 19) { // VECSEXP
////            List subdata = data[i];
////            CharacterVector subnames = subdata.attr("names");
////            boost::property_tree::ptree subtree;

////            for (unsigned int j = 0; j < subnames.size(); ++j) {
////                std::string subkey = Rcpp::as < std::string >(subnames[j]);
////                SEXP t = subdata[j];

////                if (TYPEOF(t) == 14) {
////                    subtree.put(subkey, Rcpp::as < double >(subdata[j]));
////                } else if (TYPEOF(t) == 16) {
////                    subtree.put(subkey, Rcpp::as < std::string >(subdata[j]));
////                }
////            }
////            tree.add_child(key, subtree);
////        }
////    }
////    reader.loadFromTree(tree, parameters);
////    format_dates(parameters, begin, end);
////    globalParameters.modelVersion = parameters.get < std::string >("IdModele");

////    context->begin = utils::DateTime::toJulianDayNumber(begin);
////    context->end = utils::DateTime::toJulianDayNumber(end);
////    context->simulator = new model::kernel::Simulator(model, globalParameters);
////    context->simulator->attachView("global", new model::observer::GlobalView);
////    context->simulator->init(context->begin, parameters);
////    return XPtr < Context >(context, true);
////}

//// [[Rcpp::export]]
//List rcpp_run(SEXP handle)
//{
//    XPtr < Context > context(handle);

//    context->simulator->run(context->begin, context->end);

//    const model::observer::Observer& observer = context->simulator->observer();
//    const model::observer::Observer::Views& views = observer.views();
//    Rcpp::List gresult(views.size());
//    Rcpp::CharacterVector gnames;
//    unsigned int gindex = 0;

//    for (model::observer::Observer::Views::const_iterator it = views.begin();
//         it != views.end(); ++it) {
//        Rcpp::List result(it->second->values().size() + 1);
//        Rcpp::CharacterVector names;
//        model::observer::View::Values values = it->second->values();
//        double begin = it->second->begin();
//        double end = it->second->end();

//        gnames.push_back(it->first);
//        // write header
//        names.push_back("time");
//        for (model::observer::View::Values::const_iterator
//             itv = values.begin(); itv != values.end(); ++itv) {
//            names.push_back(itv->first);
//        }
//        // write dates
//        {
//            Rcpp::CharacterVector values;

//            for (double t = begin; t <= end; ++t) {
//                values.push_back(utils::DateTime::toJulianDay(t));
//            }
//            result[0] = values;
//        }
//        // write values
//        unsigned int index = 1;

//        for (model::observer::View::Values::const_iterator itv =
//             values.begin(); itv != values.end(); ++itv) {
//            model::observer::View::Value::const_iterator itp =
//                    itv->second.begin();
//            Rcpp::NumericVector values;

//            for (double t = begin; t <= end; ++t) {
//                while (itp != itv->second.end() and itp->first < t) {
//                    ++itp;
//                }
//                if (itp != itv->second.end()) {
//                    values.push_back(
//                                boost::lexical_cast < double >(itp->second));
//                } else {
//                    values.push_back(NumericVector::get_na());
//                }
//            }
//            result[index] = values;
//            ++index;
//        }
//        DataFrame out(result);

//        out.attr("names") = names;
//        gresult[gindex] = out;
//        ++gindex;
//    }

//    gresult.attr("names") = gnames;
//    return gresult;
//}

//// [[Rcpp::export]]
//List rcpp_run_from_dataframe(List dfParameters, List dfMeteo)
//{

//    samara::GlobalParameters globalParameters;
//    model::kernel::KernelModel* model = new model::kernel::KernelModel;
//    model::models::ModelParameters parameters;

//    CharacterVector names = dfParameters[0];
//    CharacterVector values = dfParameters[1];
//    for (int i = 0; i < names.size(); ++i) {
//      //std::cout << names(i) << " " << values(i) << std::endl;
//        parameters.getRawParameters()->insert(
//            std::pair<std::string,std::string>(
//                Rcpp::as<std::string>(names(i)),
//                Rcpp::as<std::string>(values(i))
//            )
//            );
//    }

//    NumericVector TMax = dfMeteo[0];
//    NumericVector TMin = dfMeteo[1];
//    NumericVector TMoy = dfMeteo[2];
//    NumericVector HMax = dfMeteo[3];
//    NumericVector HMin = dfMeteo[4];
//    NumericVector HMoy = dfMeteo[5];
//    NumericVector Vt = dfMeteo[6];
//    NumericVector Ins = dfMeteo[7];
//    NumericVector Rg = dfMeteo[8];
//    NumericVector ETP = dfMeteo[9];
//    NumericVector Rain = dfMeteo[10];

//    for (int i = 0; i < TMax.size(); ++i) {
//      model::models::Climate c(TMax(i), TMin(i), TMoy(i), HMax(i), HMin(i), HMoy(i), Vt(i), Ins(i), Rg(i), Rain(i));
//      parameters.meteoValues.push_back(c);
//    }

//    /*std::cout << parameters.get<std::string>("datedebut") << "\n"
//                << parameters.get<std::string>("datefin") << std::endl;*/
//    double begin = utils::DateTime::toJulianDayNumber(
//      parameters.get<std::string>("datedebut"));
//    double end = utils::DateTime::toJulianDayNumber(
//      parameters.get<std::string>("datefin"));
//    model::kernel::Simulator * simulator = new model::kernel::Simulator(model, globalParameters);
//    simulator->attachView("global", new model::observer::GlobalView());
//    simulator->init(begin, parameters);
//    simulator->run(begin, end);


//    for(auto it = parameters.getRawParameters()->begin(); it != parameters.getRawParameters()->end(); ++it) {
//      std::cout << it->first << " " << it->second << "\n";
//    }

//    const model::observer::Observer& observer = simulator->observer();
//    const model::observer::Observer::Views& views = observer.views();
//    Rcpp::List gresult(views.size());
//    Rcpp::CharacterVector gnames;
//    unsigned int gindex = 0;

//    for (model::observer::Observer::Views::const_iterator it = views.begin();
//         it != views.end(); ++it) {
//        Rcpp::List result(it->second->values().size() + 1);
//        Rcpp::CharacterVector names;
//        model::observer::View::Values values = it->second->values();
//        double begin = it->second->begin();
//        double end = it->second->end();

//        gnames.push_back(it->first);
//        // write header
//        names.push_back("time");
//        for (model::observer::View::Values::const_iterator
//             itv = values.begin(); itv != values.end(); ++itv) {
//            names.push_back(itv->first);
//        }
//        // write dates
//        {
//            Rcpp::CharacterVector values;

//            for (double t = begin; t <= end; ++t) {
//                values.push_back(utils::DateTime::toJulianDay(t));
//            }
//            result[0] = values;
//        }
//        // write values
//        unsigned int index = 1;

//        for (model::observer::View::Values::const_iterator itv =
//             values.begin(); itv != values.end(); ++itv) {
//            model::observer::View::Value::const_iterator itp =
//                    itv->second.begin();
//            Rcpp::NumericVector values;

//            for (double t = begin; t <= end; ++t) {
//                while (itp != itv->second.end() and itp->first < t) {
//                    ++itp;
//                }
//                if (itp != itv->second.end()) {
//                    values.push_back(
//                                boost::lexical_cast < double >(itp->second));
//                } else {
//                    values.push_back(NumericVector::get_na());
//                }
//            }
//            result[index] = values;
//            ++index;
//        }
//        DataFrame out(result);

//        out.attr("names") = names;
//        gresult[gindex] = out;
//        ++gindex;
//    }

//    gresult.attr("names") = gnames;
//    return gresult;
////return 0;
//}
