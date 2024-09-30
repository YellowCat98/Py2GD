#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/pytypes.h>
#include <pybind11/buffer_info.h>
#include <pybind11/embed.h>  // For embedding Python
#include <pybind11/numpy.h>      // For NumPy support
#include <pybind11/embed.h>      // For embedding Python
#include <pybind11/pybind11.h>  // Low-level detail, if needed
#include <pybind11/attr.h>       // For attributes

#define defineEnumVal(name, enumName) value(#name, enumName::name)

#define execPyWithTryCatch(code)                             \
try {                                                        \
	py::exec(code);                                          \
} catch (const py::error_already_set& e) {                   \
	log::info("{}", e.what());                               \
} catch (const std::invalid_argument& e) {                   \
	log::info("{}", e.what());                               \
} catch (const std::exception& e) {                          \
	log::info("{}", e.what());                               \
}                                                            \

namespace Py2GD {
    extern pybind11::scoped_interpreter* interpreter;
    extern pybind11::module m;
    extern std::string pyScript;

    bool initInterpreterAndBindings();
    void terminateInterpreter();

    void bindRobTop();
    void bindEnums();
    void bindCocos();
    void bindGeode();
}