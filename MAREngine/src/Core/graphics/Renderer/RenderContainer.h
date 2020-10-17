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
#include "../../ecs/Components/LightComponents.h"
#include "../Mesh/Vertex.h"


namespace mar::graphics {


	typedef std::pair<float, maths::vec3> ColorPair;
	typedef std::pair<float, std::string> TexturePair;
	typedef std::pair<maths::vec3, ecs::LightComponent> LightPair;

	typedef std::vector<Vertex> VertexVector;
	typedef std::vector<uint32_t> IndicesVector;
	typedef std::vector<maths::mat4> Mat4Vector;
	typedef std::vector<ColorPair> ColorVector;
	typedef std::vector<TexturePair> TextureVector;
	typedef std::vector<LightPair> LightVector;
	typedef std::vector<float> FloatVector;


	class RenderContainer {

		friend class RenderPipeline;

	public:

		RenderContainer() = default;
	
		void reset();

		// ---- GETTERS ---- //

		const VertexVector& getVertices() const { return m_vertices; }
		const IndicesVector& getIndices() const { return m_indices; }
		const Mat4Vector& getTransforms() const { return m_transforms; }
		const ColorVector& getColors() const { return m_colors; }
		const TextureVector& getTexture2D() const { return m_tex2D; }
		const TextureVector& getTextureCubemap() const { return m_cubes; }
		const LightVector& getLights() const { return m_lights; }
		const FloatVector& getSamplerTypes() const { return m_samplerTypes; }

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

		LightVector m_lights;

	};


}


#endif // !MAR_ENGINE_GRAPHICS_RENDER_CONTAINER_H
