#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDoubleValidator>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <cmath>  // 用于 std::sqrt, std::pow

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , calculator(TrajectoryCalculator())
{
    ui->setupUi(this);

    // 设置输入框的验证器，限制只能输入浮点数
    QDoubleValidator *validator = new QDoubleValidator(this);
    ui->distanceedit->setValidator(validator);
    ui->yedit->setValidator(validator);
    ui->nedit->setValidator(validator);
    ui->kedit->setValidator(validator);

    // 连接按钮点击事件
    connect(ui->startbutton, &QPushButton::clicked, this, &MainWindow::calculateAngles, Qt::QueuedConnection);
    connect(ui->coordButton, &QPushButton::clicked, this, &MainWindow::inputCoordinates, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculateAngles()
{
    try {
        bool ok;

        // 安全地解析输入，检查转换是否成功
        double targetDistance = ui->distanceedit->text().toDouble(&ok);
        if (!ok) {
            throw std::invalid_argument("目标距离输入无效，请输入一个有效的数字。");
        }

        double targetY = ui->yedit->text().toDouble(&ok);
        if (!ok) {
            throw std::invalid_argument("目标高度 Y 输入无效，请输入一个有效的数字。");
        }

        double n = ui->nedit->text().toDouble(&ok);
        if (!ok) {
            throw std::invalid_argument("参数 n 输入无效，请输入一个有效的数字。");
        }

        double k = ui->kedit->text().toDouble(&ok);
        if (!ok) {
            throw std::invalid_argument("参数 k 输入无效，请输入一个有效的数字。");
        }

        // 获取距离类型（水平距离或直线距离）
        int distanceType = ui->distancerb->isChecked() ? 1 : 2;

        // 验证数值合理性
        if (targetDistance <= 0) {
            throw std::invalid_argument("目标距离必须大于 0。");
        }
        if (n <= 0) {
            throw std::invalid_argument("参数 n 必须大于 0。");
        }
        if (k < 0) {
            throw std::invalid_argument("参数 k 不能为负数。");
        }

        ui->startbutton->setEnabled(false); // 禁用按钮防止重复点击

        // 调用计算器计算最优角度
        std::string resultText = calculator.findOptimalAngles(targetDistance, targetY, n, k, distanceType);
        ui->resultlabel->setText(QString::fromStdString(resultText).replace("\n", "<br>"));

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "输入错误", QString::fromStdString(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "未知错误", "发生未知异常。");
    }

    ui->startbutton->setEnabled(true); // 无论成功或失败，恢复按钮状态
}

void MainWindow::inputCoordinates()
{
    QDialog dialog(this);
    dialog.setWindowTitle("输入坐标");

    QFormLayout *layout = new QFormLayout(&dialog);

    QLineEdit *selfXEdit = new QLineEdit(&dialog);
    QLineEdit *selfYEdit = new QLineEdit(&dialog);
    QLineEdit *selfZEdit = new QLineEdit(&dialog);
    QLineEdit *targetXEdit = new QLineEdit(&dialog);
    QLineEdit *targetYEdit = new QLineEdit(&dialog);
    QLineEdit *targetZEdit = new QLineEdit(&dialog);

    layout->addRow(new QLabel("自身 X 坐标:"), selfXEdit);
    layout->addRow(new QLabel("自身 Y 坐标:"), selfYEdit);
    layout->addRow(new QLabel("自身 Z 坐标:"), selfZEdit);
    layout->addRow(new QLabel("目标 X 坐标:"), targetXEdit);
    layout->addRow(new QLabel("目标 Y 坐标:"), targetYEdit);
    layout->addRow(new QLabel("目标 Z 坐标:"), targetZEdit);

    QPushButton *okButton = new QPushButton("确定", &dialog);
    QPushButton *cancelButton = new QPushButton("取消", &dialog);

    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    // Lambda 捕获所有 by reference，并处理坐标转换
    connect(okButton, &QPushButton::clicked, this, [this, &dialog, selfXEdit, selfYEdit, selfZEdit, targetXEdit, targetYEdit, targetZEdit]() {
        bool ok;
        double selfX = 0.0, selfY = 0.0, selfZ = 0.0;
        double targetX = 0.0, targetY = 0.0, targetZ = 0.0;

        // 解析自身坐标
        if (!selfXEdit->text().isEmpty()) {
            selfX = selfXEdit->text().toDouble(&ok);
            if (!ok) selfX = 0.0;
        }
        if (!selfYEdit->text().isEmpty()) {
            selfY = selfYEdit->text().toDouble(&ok);
            if (!ok) selfY = 0.0;
        }
        if (!selfZEdit->text().isEmpty()) {
            selfZ = selfZEdit->text().toDouble(&ok);
            if (!ok) selfZ = 0.0;
        }

        // 解析目标坐标
        if (!targetXEdit->text().isEmpty()) {
            targetX = targetXEdit->text().toDouble(&ok);
            if (!ok) targetX = 0.0;
        }
        if (!targetYEdit->text().isEmpty()) {
            targetY = targetYEdit->text().toDouble(&ok);
            if (!ok) targetY = 0.0;
        }
        if (!targetZEdit->text().isEmpty()) {
            targetZ = targetZEdit->text().toDouble(&ok);
            if (!ok) targetZ = 0.0;
        }

        // 计算水平距离和相对高度
        double distance = std::sqrt(std::pow(targetX - selfX, 2) + std::pow(targetZ - selfZ, 2));
        double height = targetY - selfY;

        // 更新主界面字段
        ui->distanceedit->setText(QString::number(distance, 'f', 6)); // 保留6位小数
        ui->yedit->setText(QString::number(height, 'f', 6));
        ui->distancerb->setChecked(true); // 切换为“水平距离”模式

        dialog.accept(); // 关闭对话框
    });

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.exec(); // 使用模态方式运行
}
