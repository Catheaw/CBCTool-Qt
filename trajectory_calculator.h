#ifndef TRAJECTORY_CALCULATOR_H
#define TRAJECTORY_CALCULATOR_H

#include <string>

class TrajectoryCalculator
{
public:
    static const double angle_step; // 初始化

    std::string findOptimalAngles(double targetDistance, double targetY, double n, double k, int distanceType,
    const std::pair<int,int>& lowRange, const std::pair<int,int>& highRange);

private:
    double calculateY(double x, double t, double n, double k);
};

#endif // TRAJECTORY_CALCULATOR_H
