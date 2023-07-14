#!/bin/bash
mkdir build
cd build
cmake .. -DBUILD_MEX_FILE=True -DMatlab_MEX_EXTENSION="mexmaca64" -DMatlab_ROOT_DIR=/Applications/MATLAB_R2022b_beta.app/
make -j 4 f2cdecimate_mex
cd ..
cp build/mex/f2cdecimate_mex.* ./
