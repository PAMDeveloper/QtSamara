/**
 * @file app/main.cpp
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

#include <artis/observer/Output.hpp>

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <samara/version.hpp>

#include <samara/model/kernel/Model.hpp>
#include <samara/model/kernel/Simulator.hpp>
#include <samara/model/models/ModelParameters.hpp>
#include <samara/model/observer/GlobalView.hpp>

#include <samara/utils/DateTime.hpp>
#include <samara/utils/ParametersReader.hpp>

#include <iostream>

using namespace model;

namespace po = boost::program_options;

typedef std::vector < std::string > CmdArgs;

static void format_dates(const model::models::ModelParameters& parameters,
                         std::string& begin, std::string& end)
{
    utils::DateTime::format_date(parameters.get < std::string >("BeginDate"),
                                 begin);
    utils::DateTime::format_date(parameters.get < std::string >("EndDate"),
                                 end);
}

typedef artis::observer::Output < artis::utils::DoubleTime,
                                  model::models::ModelParameters > AnOutput;

static void run(const std::string& /* path */, int /* verbose */)
{
    samara::GlobalParameters globalParameters;
    model::kernel::Model* model = new model::kernel::Model;
    model::models::ModelParameters parameters;
    utils::ParametersReader reader;
    std::string begin;
    std::string end;

    reader.load("06SB15-fev13-D1_SV21", parameters);
    format_dates(parameters, begin, end);

    globalParameters.modelVersion = parameters.get < std::string >("IdModele");
    kernel::Simulator simulator(model, globalParameters);

    simulator.attachView("global", new model::observer::GlobalView);
    simulator.init(utils::DateTime::toJulianDayNumber(begin), parameters);
    simulator.run(utils::DateTime::toJulianDayNumber(begin),
                  utils::DateTime::toJulianDayNumber(end));

    AnOutput output(simulator.observer());

    output();
}

static int show_infos()
{
    std::cout << boost::format(
        "Samara - %1%\n"
        "Copyright (C) 2013 - 2017 The Samara Development Team.\n"
        "Samara comes with ABSOLUTELY NO WARRANTY.\n"
        "You may redistribute copies of Samara\n"
        "under the terms of the GNU General Public License.\n"
        "For more information about these matters, see the file named "
        "COPYING.\n") % SAMARA_NAME_COMPLETE << std::endl;

    return EXIT_SUCCESS;
}

static int show_help(const po::options_description &desc)
{
    std::cout << desc << std::endl;

    return EXIT_SUCCESS;
}

static int show_version()
{
    std::cout << boost::format("%1%\n") % SAMARA_NAME_COMPLETE << std::endl;

    return EXIT_SUCCESS;
}

enum ProgramOptionsCode
{
    PROGRAM_OPTIONS_FAILURE = -1,
    PROGRAM_OPTIONS_END = 0,
    PROGRAM_OPTIONS_PATH = 1
};

struct ProgramOptions
{
    ProgramOptions(std::string* path, int* verbose, CmdArgs* args)
        : generic("Allowed options"), path(path), args(args)
    {
        generic.add_options()
            ("help,h", "Produce help message")
            ("version,v", "Print version string")
            ("verbose,V", po::value < int >(verbose),
             "Choose the verbose level for logs outputs")
            ("infos", "Informations of Samara")
            ("path,P", po::value < std::string >(path),
             "Select working path,\n  path name")
            ;

        desc.add(generic);
    }

    virtual ~ProgramOptions()
    { }

    int run(int argc, char *argv[])
    {
        po::positional_options_description p;
        p.add("input", -1);

        try {
            po::store(po::command_line_parser(argc,
                        argv).options(desc).positional(p).run(), vm);
            po::notify(vm);

            if (vm.count("input"))
                *args = vm["input"].as < CmdArgs >();

            if (vm.count("help"))
                return show_help(generic);

            if (vm.count("version"))
                return show_version();

            if (vm.count("infos"))
                return show_infos();

            if (vm.count("path"))
                return PROGRAM_OPTIONS_PATH;

        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;

            return PROGRAM_OPTIONS_FAILURE;
        }

        std::cerr << "Nothing to do. See the help.\n";

        return PROGRAM_OPTIONS_END;
    }

    po::options_description desc, generic;
    po::variables_map vm;
    std::string *path;
    int* verbose;
    CmdArgs *args;
};

int main(int argc, char** argv)
{
    std::string path;
    int verbose;
    CmdArgs args;

    setlocale(LC_ALL, "C");

    {
        ProgramOptions prgs(&path, &verbose, &args);
        int ret = prgs.run(argc, argv);

        if (ret == PROGRAM_OPTIONS_FAILURE) {
            return EXIT_FAILURE;
        } else if (ret == PROGRAM_OPTIONS_END) {
            return EXIT_SUCCESS;
        }
    }

    run(path, verbose);
    return 0;
}
