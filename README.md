# Decimate returning Fine to Coarse Map

Repository wrapping `decimate` function of [libigl](http://libigl.github.io/libigl/) to additionally compute a fine to coarse map for triangles and vertices.
With your full resolution mesh be `V, F` and your decimated mesh be `Vd, Fd` then you obtain maps `I: V -> Vd` and `J: F -> Fd` (contrary to decimate function in libigl which returns maps `I: Vd -> V` and `J: Fd -> F`).

Note: this algorithm might be very slow for large shapes since the `igl:decimate` function is called `250` times assuming you reduce the shape by `500` triangles.

## ⚙️ Installation

This repo comes with c++ code, python code and a matlab mex wrapper 💡

Note: Code only tested on unix systems. Expect issues with Windows 🪟.

🐍 Python

Simply download the repo and `python setup.py install` (you need working c++ compiler + cmake installed on your machine).

🔺 Matlab

See example file `BUILD_MEX.sh` on how to compile matlab mex wrapper. ⚠️ Do not simply run the `.sh` file as you need to adjust the variables `Matlab_MEX_EXTENSION` and `Matlab_ROOT_DIR` depending on your system.

🚀 Cpp

Simply include the singlemost header `decimate.hpp` file of this project into your project.

## ✨ Usage
🐍 Python
```python
import fine2coarsedec as f2cd
target_num_faces = 100
use_qslim = False # choose different decimation algorithm
vred, fred, ff2c, vf2c = f2cd.decimate(v, f, target_num_faces, use_qslim)
```

🔺 Matlab
```matlab
target_num_faces = 100
use_qslim = false
[vred, fred, ff2c, vf2c] = f2cdecimate(v, f, target_num_faces, use_qslim);
```

🚀 Cpp
```c++
#include "include/decimate.hpp"
Eigen::VectorXi Vfine2coarse, Ffine2coarse;
const bool useQslim = true;
const int targetNumFaces = 100;
f2cdec::decimator(V, F, targetNumFaces, Vfine2coarse, Ffine2coarse, useqslim);
```

## Attribution 🎓
When using this code for your own projects please cite the followig:

```bibtex
@misc{decimatec2f,
  title = {Decimate Coarse to Fine},
  author = {Paul Roetzer and others},
  note = {https://github.com/paul0noah/c2fdecimate},
  year = {2023},
}
```

## License 🚀
This repo is licensed under MIT licence.
