#include <igl/decimate.h>
#include <igl/read_triangle_mesh.h>
#include <igl/find.h>
#include <igl/colon.h>
#include <igl/triangle_triangle_adjacency.h>
#include <Eigen/Dense>
#include "include/decimate.hpp"


int main(int argc, char *argv[])
{
  // Inline mesh of a cube
  Eigen::MatrixXd V= (Eigen::MatrixXd(8,3)<<
    0.0,0.0,0.0,
    0.0,0.0,1.0,
    0.0,1.0,0.0,
    0.0,1.0,1.0,
    1.0,0.0,0.0,
    1.0,0.0,1.0,
    1.0,1.0,0.0,
    1.0,1.0,1.0).finished();
  Eigen::MatrixXi F = (Eigen::MatrixXi(12,3)<<
    0,6,4,
    0,2,6,
    0,3,2,
    0,1,3,
    2,7,6,
    2,3,7,
    4,6,7,
    4,7,5,
    0,4,5,
    0,5,1,
    1,5,7,
    1,7,3).finished();

    Eigen::VectorXi Vfine2coarse, Ffine2coarse;
    bool useqslim = true;
    f2cdec::decimator(V, F, 100, Vfine2coarse, Ffine2coarse, useqslim);
    std::cout << Vfine2coarse << Ffine2coarse << std::endl;
}
