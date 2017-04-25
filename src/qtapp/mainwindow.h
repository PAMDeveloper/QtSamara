#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QGridLayout>

#include <artis/utils/DateTime.hpp>
#include <ModelParameters.hpp>
#include <observer/GlobalView.hpp>
#include <utils/ParametersReader.hpp>

#include <QMouseEvent>
#include <QDate>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayData(observer::GlobalView * view, QString refFile,
                     samara::ModelParameters * parameters,
                     QString begin, QString end);
    void addChart(int row, int col, QLineSeries * series, QLineSeries *refseries,
                  QGridLayout * lay, QString name);

     QLineSeries * getSeries(QString fileName, QDate endDate);

     void show_trace();

 private slots:
     void on_lineEdit_textChanged(const QString &arg1);
     void on_lineEdit_2_textChanged(const QString &arg1);
     void on_lineEdit_3_textChanged(const QString &arg1);
     void on_lineEdit_4_textChanged(const QString &arg1);

 private:
     Ui::MainWindow *ui;
     QString _date;
     QString _model_name;
     QString _var_name;
     int _type;

private:
    samara::ModelParameters parameters;
    utils::ParametersReader reader;
    std::string simulation;
    QString refFolder;
    QDate currentDate;
    QDate startDate;
};

#endif // MAINWINDOW_H
