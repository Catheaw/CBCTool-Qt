#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QDoubleValidator>
#include <QMessageBox>
#include <QDialog>
#include "trajectory_calculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void calculateAngles();
    void inputCoordinates();

private:
    Ui::MainWindow *ui;
    TrajectoryCalculator calculator;
};

#endif // MAINWINDOW_H
