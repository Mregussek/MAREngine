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
#include "../RenderAPI/RenderContainerDefinitions.h"


namespace marengine {

	struct GLSL_SSBOs; 
	class TextureOpenGL;
	class RenderPipeline;
	class RenderContainer;
	class LightContainer;
	struct LightMaterial;


	class RendererBatch {

		typedef ShaderOpenGL ShaderGL;
		typedef TextureOpenGL TextureGL;

	public:

		void initialize();
		void close();
		void draw(const RenderPipeline& renderPipeline);

	private:

		void setupSSBOs();
		void setupShaders();

		void drawWithShader(const ShaderGL& shader, const std::vector<LightContainer>& lights,
			const std::vector<RenderContainer>& containers) const;

		void passTransformsToSSBO(const RenderContainer& container) const;
		void passColorsToSSBO(const ColorVector& colors) const;
		void passTexturesToShader(const ShaderGL& shader, const TextureVector& textures) const;
		void passCubemapsToShader(const ShaderGL& shader, const TextureVector& cubemaps) const;
		void passLightToSSBO(const std::vector<LightMaterial>& lightMaterials) const;


		ShaderOpenGL m_shader2D;

	};


}

#endif // !MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H
