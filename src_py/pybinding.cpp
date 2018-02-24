#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>

namespace py = pybind11;
PYBIND11_MODULE(extrinsic, m) {
    m.doc() = R"(as a test)";

    m.def("add_any", [](py::EigenDRef<Eigen::MatrixXd> x, int r, int c, double v) { x(r,c) += v; });
}
