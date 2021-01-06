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


namespace marengine {

	class FMeshBatchStaticColor;
	class FMeshBatchStaticTexture2D;
<<<<<<< HEAD
	class FPointLightBatch;
=======
>>>>>>> renderissue


	class RendererBatch {
	public:

		void initialize();
		void close();
<<<<<<< HEAD
		void draw(const FMeshBatchStaticColor& staticColorBatch, const FPointLightBatch& pointLightBatch) const;
		void draw(const FMeshBatchStaticTexture2D& staticTexture2DBatch, const FPointLightBatch& pointLightBatch) const;

	private:

		ShaderOpenGL m_shaderColors;
		ShaderOpenGL m_shaderTexture2D;
=======
		void draw() const;

	private:

		void drawColors(const FMeshBatchStaticColor& batches) const;
		void drawTextures2D(const FMeshBatchStaticTexture2D& batches) const;

		void setupShaders();


		ShaderOpenGL m_shaderColors;
		ShaderOpenGL m_shader2D;
>>>>>>> renderissue

	};


}

#endif // !MAR_ENGINE_GRAPHICS_RENDERER_BATCH_H
