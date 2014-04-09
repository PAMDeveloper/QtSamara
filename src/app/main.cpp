/**
 * @file app/main.cpp
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

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <samara/version.hpp>

#include <model/kernel/Model.hpp>
#include <model/kernel/Simulator.hpp>

#include <model/models/ModelParameters.hpp>

#include <utils/DateTime.hpp>

#include <iostream>

using namespace model;

namespace po = boost::program_options;

typedef std::vector < std::string > CmdArgs;

static void load_parameters(model::models::ModelParameters& parameters)
{
    parameters.set < double >("ASScstr", 0.);
    parameters.set < double >("AttenMitch", 0.);
    parameters.set < double >("BundHeight", 0.);
    parameters.set < double >("Ca", 0.);
    parameters.set < double >("CO2Cp", 0.);
    parameters.set < double >("CO2Exp", 0.);
    parameters.set < double >("CoeffAssimSla", 0.);
    parameters.set < double >("CoeffCO2Assim", 0.);
    parameters.set < double >("CoefficientQ10", 0.);
    parameters.set < double >("CoeffInternodeMass", 0.);
    parameters.set < double >("CoeffInternodeNum", 0.);
    parameters.set < double >("CoeffLeafDeath", 0.);
    parameters.set < double >("CoeffLeafWLRatio", 0.);
    parameters.set < double >("CoeffPanicleMass", 0.);
    parameters.set < double >("CoeffPanSinkPop", 0.);
    parameters.set < double >("CoeffResCapacityInternode", 0.);
    parameters.set < double >("CoeffReserveSink", 0.);
    parameters.set < double >("CoeffRootMassPerVolMax", 0.);
    parameters.set < double >("CoeffTillerDeath", 0.);
    parameters.set < double >("CoeffTransplantingShock", 0.);
    parameters.set < double >("DensityField", 0.);
    parameters.set < double >("DensityNursery", 0.);
    parameters.set < double >("DEVcstr", 0.);
    parameters.set < double >("DurationNursery", 0.);
    parameters.set < double >("EpaisseurProf", 0.);
    parameters.set < double >("EpaisseurSurf", 0.);
    parameters.set < double >("ExcessAssimToRoot", 0.);
    parameters.set < double >("FTSWIrrig", 0.);
    parameters.set < double >("HaunCritTillering", 0.);
    parameters.set < double >("HumFC", 0.);
    parameters.set < double >("HumPF", 0.);
    parameters.set < double >("HumSat", 0.);
    parameters.set < double >("IcTillering", 0.);
    parameters.set < double >("InternodeLengthMax", 0.);
    parameters.set < double >("IrrigAutoResume", 0.);
    parameters.set < double >("IrrigAutoStop", 0.);
    parameters.set < double >("IrrigAutoTarget", 0.);
    parameters.set < double >("KcMax", 0.);
    parameters.set < double >("KCritSterCold1", 0.);
    parameters.set < double >("KCritSterCold2", 0.);
    parameters.set < double >("KCritSterFtsw1", 0.);
    parameters.set < double >("KCritSterFtsw2", 0.);
    parameters.set < double >("KCritSterHeat1", 0.);
    parameters.set < double >("KCritSterHeat2", 0.);
    parameters.set < double >("KCritStressCold1", 0.);
    parameters.set < double >("KCritStressCold2", 0.);
    parameters.set < double >("Kdf", 0.);
    parameters.set < double >("kRespInternode", 0.);
    parameters.set < double >("kRespMaintLeaf", 0.);
    parameters.set < double >("kRespMaintRoot", 0.);
    parameters.set < double >("kRespMaintSheath", 0.);
    parameters.set < double >("kRespPanicle", 0.);
    parameters.set < double >("kTempMaint", 0.);
    parameters.set < double >("LeafLengthMax", 0.);
    parameters.set < double >("LifeSavingDrainage", 0.);
    parameters.set < double >("Mulch", 0.);
    parameters.set < double >("PanStructMassMax", 0.);
    parameters.set < double >("PercolationMax", 0.);
    parameters.set < double >("PFactor", 0.);
    parameters.set < double >("Phyllo", 0.);
    parameters.set < double >("PlantsPerHill", 0.);
    parameters.set < double >("PlotDrainageDAF", 0.);
    parameters.set < double >("PoidsSecGrain", 0.);
    parameters.set < double >("PourcRuiss", 0.);
    parameters.set < double >("PPCrit", 0.);
    parameters.set < double >("PPExp", 0.);
    parameters.set < double >("PriorityPan", 0.);
    parameters.set < double >("ProfRacIni", 0.);
    parameters.set < double >("RankLongestLeaf", 0.);
    parameters.set < double >("RelMobiliInternodeMax", 0.);
    parameters.set < double >("RelPotLeafLength", 0.);
    parameters.set < double >("RollingBase", 0.);
    parameters.set < double >("RollingSens", 0.);
    parameters.set < double >("RootCstr", 0.);
    parameters.set < double >("RootFrontMax", 0.);
    parameters.set < double >("RootPartitMax", 0.);
    parameters.set < double >("SDJBVP", 0.);
    parameters.set < double >("SDJLevee", 0.);
    parameters.set < double >("SDJMatu1", 0.);
    parameters.set < double >("SDJMatu2", 0.);
    parameters.set < double >("SDJRPR", 0.);
    parameters.set < double >("SeuilPP", 0.);
    parameters.set < double >("SeuilRuiss", 0.);
    parameters.set < double >("SlaMax", 0.);
    parameters.set < double >("SlaMin", 0.);
    parameters.set < double >("StockIniProf", 0.);
    parameters.set < double >("StockIniSurf", 0.);
    parameters.set < double >("TBase", 0.);
    parameters.set < double >("TempSla", 0.);
    parameters.set < double >("TilAbility", 0.);
    parameters.set < double >("TLet", 0.);
    parameters.set < double >("TMax", 0.);
    parameters.set < double >("TMin", 0.);
    parameters.set < double >("TOpt1", 0.);
    parameters.set < double >("TOpt2", 0.);
    parameters.set < double >("TransplantingDepth", 0.);
    parameters.set < double >("TxAssimBVP", 0.);
    parameters.set < double >("TxAssimMatu1", 0.);
    parameters.set < double >("TxAssimMatu2", 0.);
    parameters.set < double >("TxConversion", 0.);
    parameters.set < double >("TxResGrain", 0.);
    parameters.set < double >("VRacBVP", 0.);
    parameters.set < double >("VRacLevee", 0.);
    parameters.set < double >("VRacMatu1", 0.);
    parameters.set < double >("VRacMatu2", 0.);
    parameters.set < double >("VRacPSP", 0.);
    parameters.set < double >("VRacRPR", 0.);
    parameters.set < double >("WaterLoggingSens", 0.);
    parameters.set < double >("WtRatioLeafSheath", 0.);
    parameters.set < int >("IrrigAuto", 0);
    parameters.set < int >("Transplanting", 0);
}

static void run(const std::string& path, int verbose)
{
    kernel::Model* model = new kernel::Model;
    kernel::Simulator simulator(model);
    model::models::ModelParameters parameters;

    load_parameters(parameters);
    simulator.init(parameters);
    simulator.run(utils::DateTime::toJulianDayNumber("2000/01/01"),
                  utils::DateTime::toJulianDayNumber("2000/01/01") + 300);
}

static int show_infos()
{
    std::cout << boost::format(
        "Samara - %1%\n"
        "Copyright (C) 2013 - 2014 The Samara Development Team.\n"
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
