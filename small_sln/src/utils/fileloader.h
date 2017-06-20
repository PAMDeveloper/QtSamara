#ifndef FILELOADER_H
#define FILELOADER_H

#include <fstream>
#include <sstream>
#include <string>

#include "../parameters.h"

using namespace std;

void load_file_parameters(string file_path, string id) {
    std::ifstream infile(file_path);
    std::string line;
    std::getline(infile, line); //config


    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        int a, b;
        if (!(iss >> a >> b)) { break; } // error

        // process pair (a,b)
    }
}


#endif // FILELOADER_H
