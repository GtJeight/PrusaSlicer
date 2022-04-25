#include "DiffTrussSupport.hpp"
#include <fstream>
#include <cassert>

DiffTrussSupport::DiffTrussSupport(const std::string& path)
{
    //path: file path to supp data
    std::cout << path << std::endl;
    supppath = path;

    std::ifstream ifnode(path + "\\nodes.txt", std::ios::binary);
    std::ifstream ifradius(path + "\\radius.txt", std::ios::binary);
    std::ifstream ifnoderadius(path + "\\noderadius.txt", std::ios::binary);
    std::ifstream ifstrut(path + "\\beams.txt", std::ios::binary);
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

    double nrad;
    while (!ifnoderadius.eof()) {
        ifnoderadius >> nrad;
        node_radius.push_back(nrad);
    }
    node_radius.pop_back();

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

double DiffTrussSupport::NRadius(const int& i) const
{
    return node_radius[i];
}

double DiffTrussSupport::ZAngle(const int &i) const
{
    // orinet: node1 to node2
    Eigen::Vector3d ei(nodes[ix3(struts[iy2(i)])] - nodes[ix3(struts[ix2(i)])],
        nodes[iy3(struts[iy2(i)])] - nodes[iy3(struts[ix2(i)])],
        nodes[iz3(struts[iy2(i)])] -
        nodes[iz3(struts[ix2(i)])]);
    ei.normalize();
    double theta = acos(ei.dot(Eigen::Vector3d(0.0, 0.0, 1.0)));
    //std::cout << theta / 3.1415926 * 180 << std::endl;
    return theta;
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
        return Eigen::Vector3d(1.0, 0.0, 0.0);

    axis.normalize();

    return axis;
}

Eigen::Matrix<double, 3, 1, 2> DiffTrussSupport::TranslateS(const int &i) const
{
    return Eigen::Vector3d(nodes[ix3(struts[ix2(i)])],
        nodes[iy3(struts[ix2(i)])],
        nodes[iz3(struts[ix2(i)])]);
}

Eigen::Matrix<double, 3, 1, 2> DiffTrussSupport::TranslateN(const int& i) const
{
    return Eigen::Vector3d(nodes[ix3(i)],
        nodes[iy3(i)],
        nodes[iz3(i)]);
}