#include "DiffTrussSupport.hpp"
#include <fstream>
#include <cassert>

DiffTrussSupport::DiffTrussSupport(const std::string& path)
{
    //path: file path to supp data
    std::cout << path << std::endl;
    supppath = path;

    std::ifstream ifnode("D:"
                         "\\source\\PrusaSlicer\\build\\src\\Release\\resourc"
                         "es\\shapes\\trusssupp\\node.txt",
                         std::ios::binary);
    std::ifstream ifradius("D:"
                         "\\source\\PrusaSlicer\\build\\src\\Release\\resourc"
                         "es\\shapes\\trusssupp\\radius.txt",
                         std::ios::binary);
    std::ifstream ifstrut("D:"
                         "\\source\\PrusaSlicer\\build\\src\\Release\\resourc"
                         "es\\shapes\\trusssupp\\strut.txt",
                         std::ios::binary);
    assert(ifnode.is_open() && ifradius.is_open() && ifstrut.is_open());

    double node;
    while (!ifnode.eof()) {
        ifnode >> node;
        nodes.push_back(node);
    }
    nodes.pop_back();
    nNode = nodes.size() / 3;

    double rad;
    while (!ifradius.eof()) {
        ifradius >> rad;
        radius.push_back(rad);
    }
    radius.pop_back();

    int strut;
    while (!ifstrut.eof()) {
        ifstrut >> strut;
        struts.push_back(strut);
    }
    struts.pop_back();
    nStrut = struts.size() / 2;

    ifnode.close();
    ifradius.close();
    ifstrut.close();
}

DiffTrussSupport::~DiffTrussSupport()
{

}

double DiffTrussSupport::Length(const int& i) const {
    return sqrt(
        pow(nodes[ix3(struts[iy2(i)])] - nodes[ix3(struts[ix2(i)])], 2) +
        pow(nodes[iy3(struts[iy2(i)])] - nodes[iy3(struts[ix2(i)])], 2) +
        pow(nodes[iz3(struts[iy2(i)])] - nodes[iz3(struts[ix2(i)])], 2));
}

double DiffTrussSupport::Radius(const int& i) const
{
    return radius[i];
}

double DiffTrussSupport::ZAngle(const int &i) const
{
    // orinet: node1 to node2
    Eigen::Vector3d ei(nodes[ix3(struts[iy2(i)])] - nodes[ix3(struts[ix2(i)])],
        nodes[iy3(struts[iy2(i)])] - nodes[iy3(struts[ix2(i)])],
        nodes[iz3(struts[iy2(i)])] -
        nodes[iz3(struts[ix2(i)])]);
    ei.normalize();
    return acos(ei.dot(Eigen::Vector3d(0.0, 0.0, 1.0)));
}

Eigen::Matrix<double, 3, 1, 2> DiffTrussSupport::RotAxis(const int &i) const
{
    Eigen::Vector3d ei(nodes[ix3(struts[iy2(i)])] - nodes[ix3(struts[ix2(i)])],
        nodes[iy3(struts[iy2(i)])] - nodes[iy3(struts[ix2(i)])],
        0.0);
    Eigen::Matrix3d xyrot;
    xyrot << 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;
    Eigen::Vector3d axis = xyrot * ei;
    if (axis.squaredNorm() < 1e-5)
        return Eigen::Vector3d(0.0, 0.0, 1.0);

    return axis;
}

Eigen::Matrix<double, 3, 1, 2> DiffTrussSupport::Translate(const int &i) const
{
    return Eigen::Vector3d(nodes[ix3(struts[ix2(i)])],
        nodes[iy3(struts[ix2(i)])],
        nodes[iz3(struts[ix2(i)])]);
}