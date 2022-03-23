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

    int ix2(const int &i) const { return 2 * i; }
    int iy2(const int &i) const { return 2 * i + 1; }
    int ix3(const int &i) const { return 3 * i; }
    int iy3(const int &i) const { return 3 * i + 1; }
    int iz3(const int &i) const { return 3 * i + 2; }

    //Eigen::Matrix<double, 3, 1, 2> Orient(const int &i) const;
    double Length(const int &i) const;
    double Radius(const int& i) const;
    double ZAngle(const int &i) const;
    Eigen::Matrix<double, 3, 1, 2> RotAxis(const int &i) const;
    Eigen::Matrix<double, 3, 1, 2> Translate(const int &i) const;

public:
    int nNode;
    int nStrut;

private:
    std::string supppath;
    std::vector<double> nodes;
    std::vector<double> radius;
    std::vector<double> node_radius;
    std::vector<int>    struts;
};