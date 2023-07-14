#include <mex.h>

#include <igl/matlab/parse_rhs.h>
#include <iostream>

#include "include/decimate.hpp"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[]) {
    const float tol = 1e-5;

    // check and read mex input
    if (nrhs != 4 || nlhs > 2) {
        mexErrMsgTxt("Usage: [Vred, Fred, Jf2c, If2c] = f2cdecimate_mex(V, F, num_faces, useQslim)");
    }

    // parse inputs
    Eigen::MatrixXd V;
    igl::matlab::parse_rhs_double(&prhs[0], V);

    Eigen::MatrixXd Fd;
    igl::matlab::parse_rhs_double(&prhs[1], Fd);
    Eigen::MatrixXi F = Fd.cast<int>();
    F = F.array() - 1; // matlab to c++

    const int numFaces = (int) *mxGetPr(prhs[2]);

    const bool useQslim = (bool) *mxGetPr(prhs[3]);

    Eigen::VectorXi Vfine2coarse, Ffine2coarse;
    f2cdec::decimator(V, F, 100, Ffine2coarse, Vfine2coarse, useQslim);

    plhs[0] = mxCreateDoubleMatrix(V.rows(), V.cols(), mxREAL);
    std::copy(V.data(), V.data() + V.size(), mxGetPr(plhs[0]));

    //plhs[1] = mxCreateDoubleMatrix(F.rows(), F.cols(), mxREAL);
    plhs[1] = mxCreateNumericMatrix(F.rows(), F.cols(), mxINT32_CLASS, mxREAL);
    std::copy(F.data(), F.data() + F.size(), mxGetPr(plhs[1]));

    //plhs[2] = mxCreateDoubleMatrix(Ffine2coarse.rows(), Ffine2coarse.cols(), mxREAL);
    plhs[2] = mxCreateNumericMatrix(Ffine2coarse.rows(), Ffine2coarse.cols(), mxINT32_CLASS, mxREAL);
    std::copy(Ffine2coarse.data(), Ffine2coarse.data() + Ffine2coarse.size(), mxGetPr(plhs[2]));

    //plhs[3] = mxCreateDoubleMatrix(Vfine2coarse.rows(), Vfine2coarse.cols(), mxREAL);
    plhs[3] = mxCreateNumericMatrix(Vfine2coarse.rows(), Vfine2coarse.cols(), mxINT32_CLASS, mxREAL);
    std::copy(Vfine2coarse.data(), Vfine2coarse.data() + Vfine2coarse.size(), mxGetPr(plhs[3]));
}
