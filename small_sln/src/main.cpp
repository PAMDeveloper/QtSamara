#include <chrono>
#include <ctime>
#include <iostream>

#include <utils/psqlloader.h>
#include <samara.h>

int main(int /*argc*/, char */*argv[]*/) {

    std::chrono::time_point<std::chrono::system_clock> startC, endC;
    startC = std::chrono::system_clock::now();
    int nb_runs = 10000;

    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader loader(parameters);
    loader.load_complete_simulation("06SB15-fev13-D1_SV21");
    init_parameters(parameters);
    for (int i = 0; i < nb_runs; ++i) {
       run_samara_2_1(parameters);
    }

    endC = std::chrono::system_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::microseconds>
            (endC-startC).count() / nb_runs;
    std::time_t end_time = std::chrono::system_clock::to_time_t(endC);
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds << "microsec\n";
	std::cin.get();
	exit(0);
}
