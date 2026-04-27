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
#include <utility>   // for std::pair
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
    void inputCoordinates(); // 坐标输入
    void onRangeButtonClicked();    // 俯仰角范围按钮的响应函数

private:
    Ui::MainWindow *ui;
    TrajectoryCalculator calculator;
    std::pair<int, int> lowRange;   // 低弹道范围 {起始, 结束}
    std::pair<int, int> highRange;  // 高弹道范围 {起始, 结束}
};

#endif // MAINWINDOW_H
