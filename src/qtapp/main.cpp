#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


//    MainWindow ww;
//    ww.show();
//    reader.loadParametersFromFiles("d:/Output", parameters);
//    std::cout << "Simulation loaded. Starting." << std::endl;
//    ww.run(simulation, parameters, 0);
//    std::cout << "Simulation done." << std::endl;

//    MainWindow www;
//    www.show();
//    reader.loadParametersFromProstgresql(simulation, parameters);
//    std::cout << "Simulation loaded. Starting." << std::endl;
//    www.run(simulation, parameters, 0);
//    std::cout << "Simulation done." << std::endl;

    return a.exec();
}
