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

	// --- VEC3 --- //
	py::class_<math::vec3>(m, "vec3")
		.def(py::init<>())
		.def(py::init<float, float, float>(),
			py::arg("x"), py::arg("y"), py::arg("z"))
		.def_readwrite("x", &math::vec3::x)
		.def_readwrite("y", &math::vec3::y)
		.def_readwrite("z", &math::vec3::z)
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
		.def("dot",
			(float (math::vec3::*)(const math::vec3 & other)) & math::vec3::dot,
			py::arg("other"))
		.def("length",
			(float (math::vec3::*)() const) & math::vec3::length)
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


	// ---- VEC4 ---- //
	py::class_<math::vec4>(m, "vec4")
		.def(py::init<>())
		.def(py::init<const math::vec3&, float>(),
			py::arg("v"), py::arg("w"))
		.def(py::init<float, float, float, float>(),
			py::arg("x"), py::arg("y"), py::arg("z"), py::arg("w"))
		.def_readwrite("x", &math::vec4::x)
		.def_readwrite("y", &math::vec4::y)
		.def_readwrite("z", &math::vec4::z)
		.def_readwrite("w", &math::vec4::w)
		.def("add",
			(math::vec4 & (math::vec4::*)(float f)) & math::vec4::add,
			py::arg("f"))
		.def("subtract",
			(math::vec4 & (math::vec4::*)(float f)) & math::vec4::subtract,
			py::arg("f"))
		.def("multiply",
			(math::vec4 & (math::vec4::*)(float f)) & math::vec4::multiply,
			py::arg("f"))
		.def("divide",
			(math::vec4 & (math::vec4::*)(float f)) & math::vec4::divide,
			py::arg("f"))
		.def("add",
			(math::vec4 & (math::vec4::*)(const math::vec4 & other)) & math::vec4::add,
			py::arg("other"))
		.def("subtract",
			(math::vec4 & (math::vec4::*)(const math::vec4 & other)) & math::vec4::subtract,
			py::arg("other"))
		.def("multiply",
			(math::vec4 & (math::vec4::*)(const math::vec4 & other)) & math::vec4::multiply,
			py::arg("other"))
		.def("divide",
			(math::vec4 & (math::vec4::*)(const math::vec4 & other)) & math::vec4::divide,
			py::arg("other"))
		.def("dot",
			(float (math::vec4::*)(const math::vec4 & other)) & math::vec4::dot,
			py::arg("other"))
		.def("length",
			(float (math::vec4::*)() const) & math::vec4::length)
		.def_static("normalize",
			&math::vec4::normalize,
			py::arg("other"))
		// ---- vec4 + - * / FLOAT 
		.def("__add__",
			[](math::vec4 left, float right) { return left + right; },
			py::is_operator())
		.def("__sub__",
			[](math::vec4 left, float right) { return left - right; },
			py::is_operator())
		.def("__mul__",
			[](math::vec4 left, float right) { return left * right; },
			py::is_operator())
		.def("__floordiv__",
			[](math::vec4 left, float right) { return left / right; },
			py::is_operator())
		// ---- vec4 + - * / vec4 
		.def("__add__",
			[](math::vec4 left, const math::vec4& right) { return left + right; },
			py::is_operator())
		.def("__sub__",
			[](math::vec4 left, const math::vec4& right) { return left - right; },
			py::is_operator())
		.def("__mul__",
			[](math::vec4 left, const math::vec4& right) { return left * right; },
			py::is_operator())
		.def("__floordiv__",
			[](math::vec4 left, const math::vec4& right) { return left / right; },
			py::is_operator())
		// ---- EQUAL OPERATORS
		.def("__eq__",
			[](math::vec4& left, const math::vec4& other) { return left == other; },
			py::is_operator())
		.def("__ne__",
			[](math::vec4& left, const math::vec4& other) { return left != other; },
			py::is_operator());


	// ---- MAT4 ---- //
	py::class_<math::mat4>(m, "mat4")
		.def(py::init<>())
		.def(py::init<float>(),
			py::arg("diagonal"))
		.def("getColumn",
			&math::mat4::getColumn,
			py::arg("index"))
		.def_static("identity",
			&math::mat4::identity)
		.def("multiply",
			(math::mat4(math::mat4::*)(math::mat4& other))& math::mat4::multiply,
			py::arg("other"))
		.def("multiply",
			(math::vec4(math::mat4::*)(math::vec4& other))& math::mat4::multiply,
			py::arg("other"))
		.def_static("orthographic",
			&math::mat4::orthographic,
			py::arg("left"), py::arg("right"), py::arg("top"), py::arg("bottom"), py::arg("near"), py::arg("far"))
		.def_static("perspective",
			&math::mat4::perspective,
			py::arg("fov"), py::arg("aspectRatio"), py::arg("near"), py::arg("far"))
		.def_static("lookAt",
			&math::mat4::lookAt,
			py::arg("eye"), py::arg("center"), py::arg("y"))
		.def_static("translation",
			&math::mat4::translation,
			py::arg("trans"))
		.def_static("rotation",
			&math::mat4::rotation,
			py::arg("angle"), py::arg("axis"))
		.def_static("scale",
			&math::mat4::scale,
			py::arg("scal"))
		.def_static("inverse",
			&math::mat4::inverse,
			py::arg("m"))
		.def("__mul__",
			[](math::mat4 left, const math::mat4& right) { return left * right; },
			py::is_operator())
		.def("__mul__",
			[](math::mat4 left, const math::vec4& right) { return left * right; },
			py::is_operator());


	// ---- BASIC ---- //
	py::class_<math::basic>(m, "basic")
		.def_static("square",
			&math::basic::square,
			py::arg("val"));


	// --- TRIG ---- //
	py::class_<math::Trig>(m, "trig")
		.def_static("toRadians",
			&math::Trig::toRadians,
			py::arg("degrees"))
		.def_static("toDegrees",
			&math::Trig::toDegrees,
			py::arg("radians"))
		.def_static("sine",
			&math::Trig::sine,
			py::arg("radians"))
		.def_static("tangent",
			&math::Trig::tangent,
			py::arg("radians"))
		.def_static("cosine",
			&math::Trig::cosine,
			py::arg("radians"));
}


#endif // !MAR_ENGINE_SCRIPTING_EMBED_MATHS_H