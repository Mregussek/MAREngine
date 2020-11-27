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


#ifndef MAR_ENGINE_GRAPHICS_RENDER_CONTAINER_H
#define MAR_ENGINE_GRAPHICS_RENDER_CONTAINER_H


#include "../../../mar.h"
#include "../Mesh/Vertex.h"
#include "RenderContainerDefinitions.h"


namespace mar::graphics {


	class RenderContainer {

		friend class RenderPipeline;
		friend class RenderEvents;

	public:

		RenderContainer() = default;
	
		void reset();

		// ---- GET methods ---- //

		const VertexVector& getVertices() const { return m_vertices; }
		const IndicesVector& getIndices() const { return m_indices; }
		const Mat4Vector& getTransforms() const { return m_transforms; }
		const ColorVector& getColors() const { return m_colors; }
		const TextureVector& getTexture2D() const { return m_tex2D; }
		const TextureVector& getTextureCubemap() const { return m_cubes; }
		
		const FloatVector& getSamplerTypes() const { return m_samplerTypes; }

		static uint32_t getStride() { return m_stride; }

	private:

		VertexVector m_vertices;
		IndicesVector m_indices;
		float m_shapeID{ 0.f };
		uint32_t m_indicesMax{ 0 };
		static const uint32_t m_stride{ 3 + 3 + 2 + 1 };

		Mat4Vector m_transforms;

		ColorVector m_colors;
		TextureVector m_tex2D;
		TextureVector m_cubes;
		FloatVector m_samplerTypes;

	};


	struct LightMaterial {
		maths::vec4 position{ 0.f, 0.f, 0.f, 1.f };
		maths::vec4 ambient{ 0.5f, 0.5f, 0.5f, 1.f };
		maths::vec4 diffuse{ 0.9f, 0.9f, 0.9f, 1.f };
		maths::vec4 specular{ 0.5f, 0.5f, 0.5f, 1.f };

		float constant{ 1.0f };
		float linear{ 0.045f };
		float quadratic{ 0.0075f };
		float shininess{ 64.0f };
	};

	class LightContainer {
	private:

		friend class RenderPipeline;
		friend class RenderEvents;

	public:

		LightContainer() = default;

		const std::vector<LightMaterial>& getLightMaterials() const { return m_lightMaterials; }

		void reset() {
			m_lightMaterials.clear();
		}

	private:

		std::vector<LightMaterial> m_lightMaterials;

	};


}


#endif // !MAR_ENGINE_GRAPHICS_RENDER_CONTAINER_H
