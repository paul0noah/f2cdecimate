#include "include/decimate.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>


namespace py = pybind11;
using namespace pybind11::literals;  // NOLINT


std::tuple<Eigen::MatrixXd, Eigen::MatrixXi, Eigen::VectorXi, Eigen::VectorXi>
decimate_wrapper(Eigen::MatrixXd& V,
                 Eigen::MatrixXi& F,
                 const int numFaces,
                 const bool useQslim) {
    Eigen::MatrixXd Vcoarse = V;
    Eigen::MatrixXi Fcoarse = F;
    Eigen::VectorXi Vfine2coarse;
    Eigen::VectorXi Ffine2coarse;

    f2cdec::decimator(Vcoarse, Fcoarse, numFaces, Ffine2coarse, Vfine2coarse, useQslim);

    return std::make_tuple(Vcoarse, Fcoarse, Ffine2coarse, Vfine2coarse);
}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXi>
decimate_wrapper_no_map(Eigen::MatrixXd& V,
                 Eigen::MatrixXi& F,
                 const int numFaces,
                 const bool useQslim) {

    Eigen::MatrixXd U;
    Eigen::MatrixXi G;
    Eigen::VectorXi J;
    Eigen::VectorXi I;
    if (useQslim) {
        igl::qslim(V, F, numFaces, U, G, J, I);
    }
    else {
        igl::decimate(V, F, numFaces, U, G, J, I);
    }

    return std::make_tuple(U, G);
}

PYBIND11_MODULE(fine2coarsedec, handle) {
    handle.doc() = "Fine2Coarse Decimator python bindings";
    handle.def("decimate", &decimate_wrapper);
    handle.def("decimate_no_map", &decimate_wrapper_no_map);
}
