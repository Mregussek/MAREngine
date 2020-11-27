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


#ifndef MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H
#define MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H


#include "../../../mar.h"
#include "../../../Platform/OpenGL/ShaderOpenGL.h"
#include "../../../Platform/OpenGL/PipelineOpenGL.h"
#include "RenderContainerDefinitions.h"


namespace mar::graphics {

	class RenderPipeline;
	class RenderContainer;
	class RenderCamera;
	class LightContainer;
	struct LightMaterial;


	class RendererBatch {
	public:

		void initialize();
		void close();
		void draw(const RenderPipeline& renderPipeline);

	private:

		void setupSSBOs(platforms::ShaderOpenGL& shader);

		void drawWithShader(const platforms::ShaderOpenGL& shader, const std::vector<LightContainer>& lights,
			const RenderCamera& camera, const std::vector<RenderContainer>& containers) const;

		void drawContainer(const RenderContainer& container) const;

		void passTransformsToShader(const platforms::ShaderOpenGL& shader, const RenderContainer& container) const;
		void passColorsToShader(const platforms::ShaderOpenGL& shader, const ColorVector& colors) const;
		void passTexturesToShader(const platforms::ShaderOpenGL& shader, const TextureVector& textures) const;
		void passCubemapsToShader(const platforms::ShaderOpenGL& shader, const TextureVector& cubemaps) const;
		void passLightToShader(const platforms::ShaderOpenGL& shader, const std::vector<LightMaterial>& lightMaterials) const;
		void passCameraToShader(const platforms::ShaderOpenGL& shader, const RenderCamera& camera) const;

		platforms::PipelineOpenGL m_buffers;
		platforms::ShaderOpenGL m_shader2D;
		platforms::ShaderOpenGL m_shaderCubemap;

	};


}

#endif // !MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H
