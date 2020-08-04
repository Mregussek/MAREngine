/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_SCRIPTING_EMBED_MATHS_H
#define MAR_ENGINE_SCRIPTING_EMBED_MATHS_H

#if __has_include(<pybind11/pybind11.h>)
	#include <pybind11/pybind11.h>
	#define MARMathPythonModule_PYBIND11_IMPORTED
#else
	#error "MARMathPythonModule: Cannot import pybind11/pybind11.h!"
#endif

#if __has_include("MARMaths.h")
	#include "MARMaths.h"
	#define MARMathPythonModule_LIB_IMPORTED
#else
	#error "MARMathPythonModule: Cannot import MARMaths.h!"
#endif

namespace py = pybind11;
namespace math = mar::maths;

PYBIND11_MODULE(MARMathPythonModule, m) {

	m.doc() = "MARMathPythonModule";

	py::class_<math::vec3>(m, "vec3")
		.def(py::init<>())
		.def(py::init<float, float, float>(),
			py::arg("x"), py::arg("y"), py::arg("z"))
		.def("add",
			(math::vec3 & (math::vec3::*)(float f)) & math::vec3::add,
			py::arg("f"))
		.def("subtract",
			(math::vec3 & (math::vec3::*)(float f)) & math::vec3::subtract,
			py::arg("f"))
		.def("multiply",
			(math::vec3 & (math::vec3::*)(float f)) & math::vec3::multiply,
			py::arg("f"))
		.def("divide",
			(math::vec3 & (math::vec3::*)(float f)) & math::vec3::divide,
			py::arg("f"))
		.def("add",
			(math::vec3 & (math::vec3::*)(const math::vec3 & other)) & math::vec3::add,
			py::arg("other"))
		.def("subtract",
			(math::vec3 & (math::vec3::*)(const math::vec3 & other)) & math::vec3::subtract,
			py::arg("other"))
		.def("multiply",
			(math::vec3 & (math::vec3::*)(const math::vec3 & other)) & math::vec3::multiply,
			py::arg("other"))
		.def("divide",
			(math::vec3 & (math::vec3::*)(const math::vec3 & other)) & math::vec3::divide,
			py::arg("other"))
		.def("cross",
			(math::vec3(math::vec3::*)(const math::vec3 & other)) & math::vec3::cross,
			py::arg("other"))
		.def_static("cross",
			[](const math::vec3& x, const math::vec3& y) { return math::vec3::cross(x, y); },
			py::arg("x"), py::arg("y"))
		.def("dot",
			(float (math::vec3::*)(const math::vec3 & other)) & math::vec3::dot,
			py::arg("other"))
		.def_static("dot",
			[](const math::vec3& left, const math::vec3& right) { return math::vec3::dot(left, right); },
			py::arg("left"), py::arg("right"))
		.def("length",
			(float (math::vec3::*)() const) & math::vec3::length)
		.def_static("length",
			[](math::vec3& v) { return math::vec3::length(v); },
			py::arg("v"))
		.def_static("normalize",
			&math::vec3::normalize,
			py::arg("other"))
		// ---- VEC3 + - * / FLOAT 
		.def("__add__",
			[](math::vec3 left, float right) { return left + right; },
			py::is_operator())
		.def("__sub__",
			[](math::vec3 left, float right) { return left - right; },
			py::is_operator())
		.def("__mul__",
			[](math::vec3 left, float right) { return left * right; },
			py::is_operator())
		.def("__floordiv__",
			[](math::vec3 left, float right) { return left / right; },
			py::is_operator())
		// ---- VEC3 + - * / VEC3 
		.def("__add__",
			[](math::vec3 left, const math::vec3& right) { return left + right; },
			py::is_operator())
		.def("__sub__",
			[](math::vec3 left, const math::vec3& right) { return left - right; },
			py::is_operator())
		.def("__mul__",
			[](math::vec3 left, const math::vec3& right) { return left * right; },
			py::is_operator())
		.def("__floordiv__",
			[](math::vec3 left, const math::vec3& right) { return left / right; },
			py::is_operator())
		// ---- EQUAL OPERATORS
		.def("__eq__",
			[](math::vec3& left, const math::vec3& other) { return left == other; },
			py::is_operator())
		.def("__ne__",
			[](math::vec3& left, const math::vec3& other) { return left != other; },
			py::is_operator());
}


#endif // !MAR_ENGINE_SCRIPTING_EMBED_MATHS_H