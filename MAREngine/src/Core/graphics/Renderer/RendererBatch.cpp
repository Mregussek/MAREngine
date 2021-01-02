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


#include "RendererBatch.h"
#include "ShaderBufferStorage.h"
#include "RenderMemorizer.h"
#include "PipelineStorage.h"
#include "../GraphicsLogs.h"
#include "../GraphicLimits.h"
#include "../RenderAPI/RenderPipeline.h"
#include "../../events/RenderEvents.h"
#include "../../../Platform/OpenGL/DrawingOpenGL.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"
#include "../../../Platform/GLSL/ShaderUniforms.h"

#include "../Mesh/MeshBatchStaticColor.h"
#include "../Lightning/PointLightBatch.h"


namespace marengine {


	void RendererBatch::initialize() {
		GRAPHICS_INFO("RENDERER_BATCH: going to initialize!");

		setupSSBOs();
		setupShaders();
	}

	void RendererBatch::setupSSBOs() {
		GRAPHICS_TRACE("RENDERER_BATCH: going to setup shader storage buffers...");

		{ // setup Camera SSBO
			const std::vector<UniformItem> cameraItems{ GLSL_SSBOs::ut_u_MVP };

			auto& camera = ShaderBufferStorage::Instance->createShaderBufferStorage();
			RenderMemorizer::Instance->cameraSSBO = ShaderBufferStorage::Instance->getSSBOs().size() - 1;
			camera.initialize(GLSL_SSBOs::ub_Camera, cameraItems);
		}

		{ // setup EntityCmp SSBO
			auto& entityCmp = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<UniformItem> entitycmpItems{ GLSL_SSBOs::ut_u_SeparateTransform };

			entityCmp.initialize(GLSL_SSBOs::ub_EntityCmp, std::move(entitycmpItems));
		}

		{ // setup Material SSBO
			auto& material = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<UniformItem> materialItems{ 
				GLSL_SSBOs::ut_u_material, GLSL_SSBOs::ut_u_lightSize
			};

			material.initialize(GLSL_SSBOs::ub_Material, std::move(materialItems));
		}

		{ // setup TextureSmaplers SSBO
			auto& textureSamplers = ShaderBufferStorage::Instance->createShaderBufferStorage();
			std::vector<UniformItem> textureSamplersItems{ GLSL_SSBOs::ut_u_Color };

			textureSamplers.initialize(GLSL_SSBOs::ub_TextureSamplers, std::move(textureSamplersItems));
		}
	}

	void RendererBatch::setupShaders() {
		GRAPHICS_TRACE("RENDERER_BATCH: going to setup shaders...");

		const char* vert = "resources/shaders/batcher.vert.glsl";
		{
			const char* frag = "resources/shaders/texture2d.frag.glsl";
			const ShaderPaths shaderPaths(vert, frag, nullptr);

			m_shader2D.initialize(shaderPaths);
			m_shader2D.setupShaderUniforms(GLSL_SSBOs::u_2D);
		}
	}

	void RendererBatch::close() {
		GRAPHICS_INFO("RENDERER_BATCH: going to close!");

		m_shader2D.shutdown();
		TextureOpenGL::Instance()->shutdown();

		GRAPHICS_INFO("RENDERER_BATCH: closed!");
	}

	void RendererBatch::draw(const FMeshBatchStaticColor& staticColorBatch, const FPointLightBatch& pointLightBatch) const {
		m_shader2D.bind();

		{ // pass camera
			const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
			cameraSSBO.bind();
		}
		{ // pass transforms
			const FTransformsArray& transforms{ staticColorBatch.getTransforms() };
			const auto& entityShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSL_SSBOs::ub_EntityCmp);

			entityShaderBuffer.bind();
			entityShaderBuffer.update<float>(GLSL_SSBOs::ut_u_SeparateTransform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
		}
		{ // pass colors
			const FColorsArray& colors{ staticColorBatch.getColors() };
			const auto& textureShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSL_SSBOs::ub_TextureSamplers);
			textureShaderBuffer.bind();

			textureShaderBuffer.update<float>(GLSL_SSBOs::ut_u_Color.offset, colors.size() * sizeof(maths::vec4), maths::vec4::value_ptr(colors));
		}
		{ // pass lights
			const FPointLightsArray& pointLights{ pointLightBatch.getLights() };
			const auto lightSize = (int32_t)pointLights.size();
			const auto& lightShaderBuffer = ShaderBufferStorage::Instance->getCorrectShaderBuffer(GLSL_SSBOs::ub_Material);
			lightShaderBuffer.bind();

			lightShaderBuffer.update<float>(GLSL_SSBOs::ut_u_material.offset, sizeof(LightMaterial) * pointLights.size(), &pointLights[0].position.x);
			lightShaderBuffer.update<int32_t>(GLSL_SSBOs::ut_u_lightSize.offset, sizeof(int32_t), &lightSize);
		}
		
		DrawingOpenGL::drawTriangles(staticColorBatch.getIndices().size());
		RenderEvents::Instance().onDrawCall();
	}


}
