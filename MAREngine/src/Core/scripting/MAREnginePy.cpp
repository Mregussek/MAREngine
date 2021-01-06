/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef MAR_ENGINE_SCRIPTING_EMBED_MARENGINE_H
#define MAR_ENGINE_SCRIPTING_EMBED_MARENGINE_H


#include "MAREnginePy_Trampoline.h"


namespace py = pybind11;


PYBIND11_EMBEDDED_MODULE(MAREnginePy, m) {

	using namespace marengine;
	using namespace marengine::maths;

	m.doc() = "MAREngine Python Module";

	// -----------------------------------------------------------------------------------
	// MATHS
	// -----------------------------------------------------------------------------------

	// --- VEC3 --- //
	py::class_<vec3>(m, "vec3")
		.def(py::init<>())
		.def(py::init<float, float, float>(),
			py::arg("x"), py::arg("y"), py::arg("z"))
		.def_readwrite("x", &vec3::x)
		.def_readwrite("y", &vec3::y)
		.def_readwrite("z", &vec3::z)
		.def("add",
			(vec3 & (vec3::*)(float f)) & vec3::add,
			py::arg("f"))
		.def("subtract",
			(vec3 & (vec3::*)(float f)) & vec3::subtract,
			py::arg("f"))
		.def("multiply",
			(vec3 & (vec3::*)(float f)) & vec3::multiply,
			py::arg("f"))
		.def("divide",
			(vec3 & (vec3::*)(float f)) & vec3::divide,
			py::arg("f"))
		.def("add",
			(vec3 & (vec3::*)(const vec3 & other)) & vec3::add,
			py::arg("other"))
		.def("subtract",
			(vec3 & (vec3::*)(const vec3 & other)) & vec3::subtract,
			py::arg("other"))
		.def("multiply",
			(vec3 & (vec3::*)(const vec3 & other)) & vec3::multiply,
			py::arg("other"))
		.def("divide",
			(vec3 & (vec3::*)(const vec3 & other)) & vec3::divide,
			py::arg("other"))
		.def("cross",
			(vec3(vec3::*)(const vec3 & other) const) & vec3::cross,
			py::arg("other"))
		.def("dot",
			(float (vec3::*)(const vec3 & other) const) & vec3::dot,
			py::arg("other"))
		.def("length",
			(float (vec3::*)() const) & vec3::length)
		.def_static("normalize",
			&vec3::normalize,
			py::arg("other"))
		// ---- VEC3 + - * / FLOAT 
		.def("__add__",
			[](vec3 left, float right) { return left + right; },
			py::is_operator())
		.def("__sub__",
			[](vec3 left, float right) { return left - right; },
			py::is_operator())
		.def("__mul__",
			[](vec3 left, float right) { return left * right; },
			py::is_operator())
		.def("__floordiv__",
			[](vec3 left, float right) { return left / right; },
			py::is_operator())
		// ---- VEC3 + - * / VEC3 
		.def("__add__",
			[](vec3 left, const vec3& right) { return left + right; },
			py::is_operator())
		.def("__sub__",
			[](vec3 left, const vec3& right) { return left - right; },
			py::is_operator())
		.def("__mul__",
			[](vec3 left, const vec3& right) { return left * right; },
			py::is_operator())
		.def("__floordiv__",
			[](vec3 left, const vec3& right) { return left / right; },
			py::is_operator())
		// ---- EQUAL OPERATORS
		.def("__eq__",
			[](vec3& left, const vec3& other) { return left == other; },
			py::is_operator())
		.def("__ne__",
			[](vec3& left, const vec3& other) { return left != other; },
			py::is_operator());


	// ---- VEC4 ---- //
	py::class_<vec4>(m, "vec4")
		.def(py::init<>())
		.def(py::init<const vec3&, float>(),
			py::arg("v"), py::arg("w"))
		.def(py::init<float, float, float, float>(),
			py::arg("x"), py::arg("y"), py::arg("z"), py::arg("w"))
		.def_readwrite("x", &vec4::x)
		.def_readwrite("y", &vec4::y)
		.def_readwrite("z", &vec4::z)
		.def_readwrite("w", &vec4::w)
		.def("add",
			(vec4 & (vec4::*)(float f)) & vec4::add,
			py::arg("f"))
		.def("subtract",
			(vec4 & (vec4::*)(float f)) & vec4::subtract,
			py::arg("f"))
		.def("multiply",
			(vec4 & (vec4::*)(float f)) & vec4::multiply,
			py::arg("f"))
		.def("divide",
			(vec4 & (vec4::*)(float f)) & vec4::divide,
			py::arg("f"))
		.def("add",
			(vec4 & (vec4::*)(const vec4 & other)) & vec4::add,
			py::arg("other"))
		.def("subtract",
			(vec4 & (vec4::*)(const vec4 & other)) & vec4::subtract,
			py::arg("other"))
		.def("multiply",
			(vec4 & (vec4::*)(const vec4 & other)) & vec4::multiply,
			py::arg("other"))
		.def("divide",
			(vec4 & (vec4::*)(const vec4 & other)) & vec4::divide,
			py::arg("other"))
		.def("dot",
			(float (vec4::*)(const vec4 & other) const) & vec4::dot,
			py::arg("other"))
		.def("length",
			(float (vec4::*)() const) & vec4::length)
		.def_static("normalize",
			&vec4::normalize,
			py::arg("other"))
		// ---- vec4 + - * / FLOAT 
		.def("__add__",
			[](vec4 left, float right) { return left + right; },
			py::is_operator())
		.def("__sub__",
			[](vec4 left, float right) { return left - right; },
			py::is_operator())
		.def("__mul__",
			[](vec4 left, float right) { return left * right; },
			py::is_operator())
		.def("__floordiv__",
			[](vec4 left, float right) { return left / right; },
			py::is_operator())
		// ---- vec4 + - * / vec4 
		.def("__add__",
			[](vec4 left, const vec4& right) { return left + right; },
			py::is_operator())
		.def("__sub__",
			[](vec4 left, const vec4& right) { return left - right; },
			py::is_operator())
		.def("__mul__",
			[](vec4 left, const vec4& right) { return left * right; },
			py::is_operator())
		.def("__floordiv__",
			[](vec4 left, const vec4& right) { return left / right; },
			py::is_operator())
		// ---- EQUAL OPERATORS
		.def("__eq__",
			[](vec4& left, const vec4& other) { return left == other; },
			py::is_operator())
		.def("__ne__",
			[](vec4& left, const vec4& other) { return left != other; },
			py::is_operator());


	// ---- MAT4 ---- //
	py::class_<mat4>(m, "mat4")
		.def(py::init<>())
		.def(py::init<float>(),
			py::arg("diagonal"))
		.def("getColumn4",
			&mat4::getColumn4,
			py::arg("index"))
		.def_static("identity",
			&mat4::identity)
		.def("multiply",
			(mat4(mat4::*)(const mat4 & other) const) & mat4::multiply,
			py::arg("other"))
		.def("multiply",
			(vec4(mat4::*)(const vec4 & other) const) & mat4::multiply,
			py::arg("other"))
		.def_static("orthographic",
			&mat4::orthographic,
			py::arg("left"), py::arg("right"), py::arg("top"), py::arg("bottom"), py::arg("near"), py::arg("far"))
		.def_static("perspective",
			&mat4::perspective,
			py::arg("fov"), py::arg("aspectRatio"), py::arg("near"), py::arg("far"))
		.def_static("lookAt",
			&mat4::lookAt,
			py::arg("eye"), py::arg("center"), py::arg("y"))
		.def_static("translation",
			&mat4::translation,
			py::arg("trans"))
		.def_static("rotation",
			&mat4::rotation,
			py::arg("angle"), py::arg("axis"))
		.def_static("scale",
			&mat4::scale,
			py::arg("scal"))
		.def_static("inverse",
			&mat4::inverse,
			py::arg("m"))
		.def("__mul__",
			[](mat4 left, const mat4& right) { return left * right; },
			py::is_operator())
		.def("__mul__",
			[](mat4 left, const vec4& right) { return left * right; },
			py::is_operator());


	// ---- BASIC ---- //
	py::class_<basic>(m, "basic")
		.def_static("square",
			&basic::square,
			py::arg("val"));


	// --- TRIG ---- //
	py::class_<trig>(m, "trig")
		.def_static("toRadians",
			&trig::toRadians,
			py::arg("degrees"))
		.def_static("toDegrees",
			&trig::toDegrees,
			py::arg("radians"))
		.def_static("sine",
			&trig::sine,
			py::arg("radians"))
		.def_static("tangent",
			&trig::tangent,
			py::arg("radians"))
		.def_static("cosine",
			&trig::cosine,
			py::arg("radians"));

	// -----------------------------------------------------------------------------------
	// Entity Component System
	// -----------------------------------------------------------------------------------

	py::class_<TransformComponent>(m, "Transform")
		.def(py::init<>())
		.def_readwrite("scale", &TransformComponent::scale)
		.def_readwrite("center", &TransformComponent::center)
		.def_readwrite("angles", &TransformComponent::angles);

	py::class_<ColorComponent>(m, "Color")
		.def(py::init<>())
		.def_readwrite("texture", &ColorComponent::texture);

	py::class_<CameraComponent>(m, "Camera")
		.def(py::init<>())
		.def_readwrite("p_fov",				&CameraComponent::p_fov)
		.def_readwrite("p_aspectRatio",	&CameraComponent::p_aspectRatio)
		.def_readwrite("p_near",			&CameraComponent::p_near)
		.def_readwrite("p_far",				&CameraComponent::p_far)
		.def_readwrite("o_left",			&CameraComponent::o_left)
		.def_readwrite("o_right",			&CameraComponent::o_right)
		.def_readwrite("o_top",				&CameraComponent::o_top)
		.def_readwrite("o_bottom",			&CameraComponent::o_bottom)
		.def_readwrite("o_near",			&CameraComponent::o_near)
		.def_readwrite("o_far",				&CameraComponent::o_far);

	py::class_<FPointLight>(m, "Light")
		.def(py::init<>())
		.def_readwrite("position",	&FPointLight::position)
		.def_readwrite("ambient",	&FPointLight::ambient)
		.def_readwrite("diffuse",	&FPointLight::diffuse)
		.def_readwrite("specular",	&FPointLight::specular)
		.def_readwrite("constant",	&FPointLight::constant)
		.def_readwrite("linear",	&FPointLight::linear)
		.def_readwrite("quadratic", &FPointLight::quadratic)
		.def_readwrite("shininess", &FPointLight::shininess);

	// ---- ENTITY ---- //
	py::class_<PyEntity, PyTrampoline>(m, "Entity")
		.def(py::init<>())
		.def("start",				&PyEntity::start)
		.def("update",				&PyEntity::update)
		.def_readwrite("transform", &PyEntity::transform)
		.def_readwrite("light",		&PyEntity::light)
		.def_readwrite("camera",	&PyEntity::camera)
		.def_readwrite("color",		&PyEntity::color);

	// -----------------------------------------------------------------------------------
	// Helper methods
	// -----------------------------------------------------------------------------------

	m.def("getTime",
		[]() { return glfwGetTime(); });

	m.def("printf",
		[](float f) {std::cout << f << "\n"; },
		py::arg("f"));

	m.def("print3",
		[](vec3 v) { std::cout << v.x << " " << v.y << " " << v.z << "\n"; },
		py::arg("v"));

	m.def("print4",
		[](vec4 v) { std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << "\n"; },
		py::arg("v"));
	m.def("printm",
		[](const mat4& matrix) {
			std::cout << "mat4:"
				<< " | " << matrix[0 + 0 * 4] << " " << matrix[0 + 1 * 4] << " " << matrix[0 + 2 * 4] << " " << matrix[0 + 3 * 4] << " |\n"
				<< "      | " << matrix[1 + 0 * 4] << " " << matrix[1 + 1 * 4] << " " << matrix[1 + 2 * 4] << " " << matrix[1 + 3 * 4] << " |\n"
				<< "      | " << matrix[2 + 0 * 4] << " " << matrix[2 + 1 * 4] << " " << matrix[2 + 2 * 4] << " " << matrix[2 + 3 * 4] << " |\n"
				<< "      | " << matrix[3 + 0 * 4] << " " << matrix[3 + 1 * 4] << " " << matrix[3 + 2 * 4] << " " << matrix[3 + 3 * 4] << " |\n";
		}, py::arg("matrix"));
}


#endif // !MAR_ENGINE_SCRIPTING_EMBED_MARENGINE_H