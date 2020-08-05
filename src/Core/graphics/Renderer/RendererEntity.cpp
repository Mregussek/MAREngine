/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "RendererEntity.h"
#include "Renderer.h"
#include "../GraphicsLogs.h"


namespace mar {
	namespace graphics {


		RendererStatistics RendererEntity::s_stats;

		void RendererEntity::initialize() {
			m_renColor.initialize();
			m_ren2D.initialize(SHADER_ENTITY_TEXTURE2D);
			m_renCube.initialize(SHADER_ENTITY_CUBEMAP);
		}

		void RendererEntity::close() {
			m_renColor.close();
			m_ren2D.close();
			m_renCube.close();

			GRAPHICS_INFO("RENDERERENTITY: closed!");
		}

		void RendererEntity::update(ecs::Scene* scene) {
			auto& colors = scene->getColorsStorage();
			auto& cubemaps = scene->getCubemapStorage();
			auto& textures = scene->getTexturesStorage();

			if (!colors.vertices.empty()) {
				s_stats.drawCallsCount += 1;

				m_renColor.draw(colors,
					 scene->getRenderCamera(),
					 scene->getLightStorage()
				);
			}

			if(!cubemaps.vertices.empty()) {
				s_stats.drawCallsCount += 1;

				m_renCube.draw(cubemaps,
					 scene->getRenderCamera(),
					 scene->getLightStorage(), 
					 scene->getTextureInstance(),
					 GL_TEXTURE_CUBE_MAP
				);
			}

			if (!textures.vertices.empty()) {
				s_stats.drawCallsCount += 1;

				m_ren2D.draw(textures,
					 scene->getRenderCamera(),
					 scene->getLightStorage(), 
					 scene->getTextureInstance(),
					 GL_TEXTURE_2D
				);
			}

			s_stats.verticesCount += colors.vertices.size() + cubemaps.vertices.size() + textures.vertices.size();
			s_stats.indicesCount += colors.indices.size() + cubemaps.indices.size() + textures.indices.size();

			s_stats.trianglesCount = s_stats.indicesCount / 3;

			GRAPHICS_INFO("RENDERERENTITY: Draw calls finished for this scene!");
		}

		RendererStatistics& RendererEntity::getStatistics() { 
			return s_stats; 
		}

		void RendererEntity::clearStatistics() {
			s_stats.resetStatistics();

			GRAPHICS_INFO("RENDERERENTITY: reseting renderer statistics!");
		}


} }
