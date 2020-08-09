/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_SCRIPTING_EMBED_MATHS_H
#define MAR_ENGINE_SCRIPTING_EMBED_MATHS_H

#include "../ecs/Scene.h"
#include "../ecs/Entity.h"
#include "EmbedEngineHelpers.h"


namespace py = pybind11;
namespace math = mar::maths;
namespace ecs = mar::ecs;
namespace scripting = mar::scripting;


PYBIND11_MODULE(MAREnginePy, m) {

	m.doc() = "MAREngine Python Module";

	// -----------------------------------------------------------------------------------
	// MATHS
	// -----------------------------------------------------------------------------------

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
			(math::mat4(math::mat4::*)(math::mat4 & other)) & math::mat4::multiply,
			py::arg("other"))
		.def("multiply",
			(math::vec4(math::mat4::*)(math::vec4 & other)) & math::mat4::multiply,
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

	// -----------------------------------------------------------------------------------
	// Entity Component System
	// -----------------------------------------------------------------------------------

	// --- COMPONENTS --- //
	py::class_<scripting::Transform>(m, "Transform")
		.def(py::init<>())
		.def_readwrite("scale", &scripting::Transform::scale)
		.def_readwrite("center", &scripting::Transform::center)
		.def_readwrite("angles", &scripting::Transform::angles);

	py::class_<scripting::Color>(m, "Color")
		.def(py::init<>())
		.def_readwrite("texture", &scripting::Color::texture);

	py::class_<scripting::Camera>(m, "Camera")
		.def(py::init<>())
		.def_readwrite("p_fov", &scripting::Camera::p_fov)
		.def_readwrite("p_aspectRation", &scripting::Camera::p_aspectRatio)
		.def_readwrite("p_near", &scripting::Camera::p_near)
		.def_readwrite("p_far", &scripting::Camera::p_far)
		.def_readwrite("o_left", &scripting::Camera::o_left)
		.def_readwrite("o_right", &scripting::Camera::o_right)
		.def_readwrite("o_top", &scripting::Camera::o_top)
		.def_readwrite("o_bottom", &scripting::Camera::o_bottom)
		.def_readwrite("o_near", &scripting::Camera::o_near)
		.def_readwrite("o_far", &scripting::Camera::o_far);

	py::class_<scripting::Light>(m, "Light")
		.def(py::init<>())
		.def_readwrite("ambient", &scripting::Light::ambient)
		.def_readwrite("diffuse", &scripting::Light::diffuse)
		.def_readwrite("specular", &scripting::Light::specular)
		.def_readwrite("constant", &scripting::Light::constant)
		.def_readwrite("linear", &scripting::Light::linear)
		.def_readwrite("quadratic", &scripting::Light::quadratic)
		.def_readwrite("shininess", &scripting::Light::shininess);

	// ---- ENTITY ---- //
	py::class_<scripting::PyEntity, scripting::PyTrampoline>(m, "Entity")
		.def(py::init<>())
		.def_readwrite("transform", &scripting::PyEntity::transform)
		.def_readwrite("light", &scripting::PyEntity::light)
		.def_readwrite("camera", &scripting::PyEntity::camera)
		.def_readwrite("color", &scripting::PyEntity::color)
		.def("start",
			&scripting::PyEntity::start)
		.def("update",
			&scripting::PyEntity::update);

	// -----------------------------------------------------------------------------------
	// Helper methods
	// -----------------------------------------------------------------------------------

	m.def("getTime",
		[]() { return glfwGetTime(); });

	m.def("printf",
		[](float f) {std::cout << f << "\n"; },
		py::arg("f"));

	m.def("print3",
		[](math::vec3 v) { std::cout << v.x << " " << v.y << " " << v.z << "\n"; },
		py::arg("v"));

	m.def("print4",
		[](math::vec4 v) { std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << "\n"; },
		py::arg("v"));
	m.def("printm",
		[](const math::mat4& matrix) {
			std::cout << "mat4:"
				<< " | " << matrix[0 + 0 * 4] << " " << matrix[0 + 1 * 4] << " " << matrix[0 + 2 * 4] << " " << matrix[0 + 3 * 4] << " |\n"
				<< "      | " << matrix[1 + 0 * 4] << " " << matrix[1 + 1 * 4] << " " << matrix[1 + 2 * 4] << " " << matrix[1 + 3 * 4] << " |\n"
				<< "      | " << matrix[2 + 0 * 4] << " " << matrix[2 + 1 * 4] << " " << matrix[2 + 2 * 4] << " " << matrix[2 + 3 * 4] << " |\n"
				<< "      | " << matrix[3 + 0 * 4] << " " << matrix[3 + 1 * 4] << " " << matrix[3 + 2 * 4] << " " << matrix[3 + 3 * 4] << " |\n";
		}, py::arg("matrix"));
}


#endif // !MAR_ENGINE_SCRIPTING_EMBED_MATHS_H