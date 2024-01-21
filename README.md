### Current issues
- Missing exact solution gradient in Ethier Steinman problem
- Lack of converge to an acceptable precision due to missing preconditioner
- Large L2 error in Ethier Steinman problem (possibly caused by low precision in solver)
- Missing 3D cylinder mesh files
- 3D Flow past a cylinder was not tested (waiting for mesh files)
- Boundary tags for 3D flow past a cylinder are placeholders (waiting for mesh files)
- Missing computation of the lift and drag coefficients
- Check if serial computations (e.g. initialization of the triangulation) or dense matrices (e.g. in assembly) cause problems for performance/memory usage, remove them is necessary

### Compiling
To build the executable, make sure you have loaded the needed modules with
```bash
$ module load gcc-glibc dealii
```
Then run the following commands:
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
The executable will be created into `build`, and can be executed through
```bash
$ ./navier_stokes
```