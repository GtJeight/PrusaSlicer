#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>

class DiffTrussSupport
{
public:
    DiffTrussSupport(const std::string &path);
    ~DiffTrussSupport();

private:
    std::string supppath;
    std::vector<Eigen::Vector3d> nodes;

};