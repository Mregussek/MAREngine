/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MARENGINE_SHADERSTORAGEBUFFEROPENGL_H
#define MARENGINE_SHADERSTORAGEBUFFEROPENGL_H


#include "../../mar.h"
#include "../GLSL/ShaderUniforms.h"


namespace marengine {

	class ShaderBufferStorage;


	class FShaderStorageBufferOpenGL {

		friend class ShaderBufferStorage;

	public:

		void initialize(const UniformBuffer& uniformBuffer, const std::vector<UniformItem>& items);
		void close();

		void bind() const;
		void unbind() const;

		template<typename T>
		void update(uint32_t offset, uint32_t memory, const std::vector<T>& data) const;

		template<typename T>
		void update(uint32_t offset, uint32_t memory, const T* data) const;

		void reset() const;

	private:

		UniformBuffer m_uniformBuffer{ "Null", 0, 0 };
		std::vector<UniformItem> m_uniformItems;
		uint32_t m_ssbo{ 0 };

	};


}


#include "ShaderStorageBufferOpenGL.inl"


#endif // !MARENGINE_SHADERSTORAGEBUFFEROPENGL_H
