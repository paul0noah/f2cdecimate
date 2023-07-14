#include <igl/decimate.h>
#include <igl/qslim.h>
#include <igl/find.h>
#include <igl/colon.h>
#include <igl/triangle_triangle_adjacency.h>
#include <Eigen/Dense>

namespace f2cdec {


Eigen::MatrixXi findChildTriangles(Eigen::MatrixXi& F, const int missingTriangle1Idx, const int missingTriangle2Idx, const int centerVertex, const long nVertices) {

    Eigen::MatrixXi childIdcs(2, 1);
    childIdcs << -1, -1;

    Eigen::VectorXi VF, NI;
    Eigen::MatrixXi TT, TTi;
    igl::triangle_triangle_adjacency(F, TT,TTi);
    for (int i = 0; i < 3; i++) {
        if ((TT(missingTriangle1Idx, i) != missingTriangle2Idx) && childIdcs(0) == -1) {
            for (int j = 0; j < 3; j++) {
                if (F(TT(missingTriangle1Idx, i), j) == centerVertex) {
                    childIdcs(0) = TT(missingTriangle1Idx, i);
                    break;
                }
            }
        }

        if ((TT(missingTriangle2Idx, i) != missingTriangle1Idx) && childIdcs(1) == -1) {
            for (int j = 0; j < 3; j++) {
                if (F(TT(missingTriangle2Idx, i), j) == centerVertex) {
                    childIdcs(1) = TT(missingTriangle2Idx, i);
                    break;
                }
            }
        }
    }

    return childIdcs;
}


void decimator(Eigen::MatrixXd& V,
               Eigen::MatrixXi& F,
               const int num_faces,
               Eigen::VectorXi& Ffine2coarse,
               Eigen::VectorXi& Vfine2coarse,
               const bool useQslim) {

    const bool verbose = false;

    Vfine2coarse = igl::colon<int>(0, V.rows() - 1);
    Ffine2coarse = igl::colon<int>(0, F.rows() - 1);

    Eigen::MatrixXd U;
    Eigen::MatrixXi G;
    Eigen::VectorXi J;
    Eigen::VectorXi I;

    size_t current_num_faces = F.rows();

    const size_t maxIter = current_num_faces - num_faces;
    size_t iter = 0;
    Eigen::ArrayXi findMissingVertexHelper(V.rows(), 1);
    Eigen::ArrayXi findMissingTriangleHelper(F.rows(), 1);
    while (current_num_faces > num_faces) {
        if (verbose) std::cout << "Iter: " << iter << std::endl;
        /*
         if v1 non-existent in reduced shape this means that
         triangles A and B collaps this means for the fine-to-coarse mapping:
         A -> C
         B -> D
         and
         v1 -> v2

         fine:                  coarse:
         v1
          ----------                         -
         / \  B   / \                       / \
        /   \    /   \                     /   \
       /  A  \  / D   \                   / D   \
      /----------------\        -----------------\
      \       / v2              \       / v2
       \  C  /                   \  C  /
        \   /                     \   /
         \ /                       \ /
          -                         -
         */


        Eigen::VectorXi currentVf2c = igl::colon<int>(0, V.rows() - 1);
        Eigen::VectorXi currentFf2c = igl::colon<int>(0, F.rows() - 1);

        current_num_faces--;
        if (useQslim) {
            igl::qslim(V, F, current_num_faces, U, G, J, I);
        }
        else {
            igl::decimate(V, F, current_num_faces, U, G, J, I);
        }

        // find missing triangle and vertex
        findMissingTriangleHelper.setOnes();
        findMissingTriangleHelper(J) -= 1;
        findMissingVertexHelper.setOnes();
        findMissingVertexHelper(I) -= 1;
        Eigen::VectorXi O, P;
        Eigen::VectorXi Q;
        igl::find(findMissingVertexHelper, O, P, Q);
        int missingVertexIdx = O(0);
        if (O.rows() > 1) {
            std::cout << "Error: To many vertices missing. Cannot continue!" << std::endl;
            exit(-1);
        }
        igl::find(findMissingTriangleHelper, O, P, Q);
        int missingTriangle1Idx = O(0);
        int missingTriangle2Idx = O(1);
        if (O.rows() > 2) {
            std::cout << "Error: To many triangles missing. Cannot continue!" << std::endl;
            exit(-1);
        }


        // find shared edge of missing triangles
        int sharedVertexIdx = -1;
        for (int i = 0; i < 3; i++) {
            if (F(missingTriangle1Idx, i) != missingVertexIdx) {
                for (int j = 0; j < 3; j ++) {
                    if (F(missingTriangle1Idx, i) == F(missingTriangle2Idx, j)) {
                        sharedVertexIdx = F(missingTriangle1Idx, i);
                        break;
                    }
                }
            }
            if (sharedVertexIdx != -1) break;
        }

        Eigen::MatrixXi childIdx = findChildTriangles(F, missingTriangle1Idx, missingTriangle2Idx, sharedVertexIdx, V.rows());


        // update maps fine to coarse
        currentVf2c(missingVertexIdx) = sharedVertexIdx;
        //currentVf2c(I) = igl::colon<int>(0, I.rows() - 1);
        for (int i = 0; i < currentVf2c.rows(); i++) {
            if (currentVf2c(i) >= missingVertexIdx)
                currentVf2c(i) = currentVf2c(i) - 1;
        }
        currentFf2c(missingTriangle1Idx) = childIdx(0);
        currentFf2c(missingTriangle2Idx) = childIdx(1);
        //currentVf2c(J) = igl::colon<int>(0, J.rows() - 1);
        for (int i = 0; i < currentFf2c.rows(); i++) {
            if (currentFf2c(i) >= missingTriangle1Idx) {
                currentFf2c(i) = currentFf2c(i) - 1;
            }
            if (currentFf2c(i) >= missingTriangle2Idx) {
                currentFf2c(i) = currentFf2c(i) - 1;
            }
        }


        // update vars
        V = U;
        F = G;
        Vfine2coarse = currentVf2c(Vfine2coarse);
        Ffine2coarse = currentFf2c(Ffine2coarse);
        findMissingVertexHelper.conservativeResize(V.rows(), 1);
        findMissingTriangleHelper.conservativeResize(F.rows(), 1);
        iter++;
        if (iter > maxIter) {
            break;
        }
        current_num_faces = F.rows();
        //std::cout << "Frows " << V.rows() << std::endl;
        //std::cout << I << std::endl;
    }

    //std::cout << "Vfine2coarse" << std::endl << Vfine2coarse << std::endl;
    //std::cout << "Ffine2coarse" << std::endl << Ffine2coarse << std::endl;

    //std::cout << F << std::endl;
}


} // namespace f2cdec
