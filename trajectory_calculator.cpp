#include "trajectory_calculator.h"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <utility>

const double TrajectoryCalculator::angle_step = 0.01; // 初始化

std::string TrajectoryCalculator::findOptimalAngles(double targetDistance, double targetY, double n, double k, int distanceType,
const std::pair<int,int>& lowRange, const std::pair<int,int>& highRange)
{
    //计算前的检查
    double x;
    if (distanceType == 1) {
        x = targetDistance;
        if (x < k + 2) throw std::invalid_argument("距离过近");
    } else if (distanceType == 2) {
        double r = targetDistance;
        x = std::sqrt(std::pow(r, 2) - std::pow(targetY, 2));
        if (x <= k) throw std::invalid_argument("计算出的水平距离必须大于炮座前部长度");
    } else {
        throw std::invalid_argument("未知的距离类型");
    }

    if (n <= 0 || k < 0) {
        throw std::invalid_argument("装药必须大于0且炮座前部长度不能小于0");
    }

    //开始计算
    const double precision_threshold = 3; // 可接受的误差
    std::string results;
    // const std::pair<int, int> ranges[2] = {{-30, 30}, {30, 60}};
    std::pair<int,int> ranges[2] = {lowRange, highRange};
    const char* descriptions[2] = {"低弹道", "高弹道"};
    for (const auto& angle_range : ranges) {
        double min_diff = std::numeric_limits<double>::max();
        double best_t = -100;
        for (int t = angle_range.first * 100; t <= angle_range.second * 100; ++t) {
            try {
                double y = calculateY(x, t * angle_step, n, k);
                double diff = std::fabs(y - targetY);
                if (diff < min_diff) {
                    min_diff = diff;
                    best_t = t * angle_step;
                }
            } catch (std::exception& e) {
                continue;
            }
        }
        if (min_diff > precision_threshold) {
            best_t = -100; // 如果精度未达到阈值，重置best_t
        }

        if (best_t != -100) {
            std::string description = (angle_range.first == -30) ? "低弹道" : "高弹道";
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << best_t;
            results += std::string(description) + ": " + ss.str() + "\n";
        } else {
            std::string description = (angle_range.first == -30) ? "低弹道" : "高弹道";
            results += std::string(description) + ": ?\n";
        }
    }

    return results;
}

// 计算
double TrajectoryCalculator::calculateY(double x, double t, double n, double k)
{
    double radian_t = t * M_PI / 180.0; //计算弧度
    return ((5 / std::cos(radian_t)) / (2 * n) + std::tan(radian_t)) * x +
           500 * std::log(1 - (x / std::cos(radian_t) - k) / (200 * n)) -
           (5 * k / (2 * n)) + 2;
}
