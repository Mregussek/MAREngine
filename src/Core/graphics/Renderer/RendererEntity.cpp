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

		void RendererEntity::update(ecs::SceneManager* scene) {
			auto& storage = scene->getStorage();

			if (!storage.colors_storage.vertices.empty()) {
				s_stats.drawCallsCount += 1;

				m_renColor.draw(storage.colors_storage,
					 scene->getScene()->getRenderCamera(),
					 storage.light_storage
				);
			}

			if(!storage.cubemap_storage.vertices.empty()) {
				s_stats.drawCallsCount += 1;

				m_renCube.draw(storage.cubemap_storage,
					scene->getScene()->getRenderCamera(),
					storage.light_storage,
					GL_TEXTURE_CUBE_MAP
				);
			}

			if (!storage.texture_storage.vertices.empty()) {
				s_stats.drawCallsCount += 1;

				m_ren2D.draw(storage.texture_storage,
					scene->getScene()->getRenderCamera(),
					storage.light_storage,
					GL_TEXTURE_2D
				);
			}

			s_stats.verticesCount += 
				storage.colors_storage.vertices.size() +
				storage.cubemap_storage.vertices.size() +
				storage.texture_storage.vertices.size();
			s_stats.indicesCount += 
				storage.colors_storage.indices.size() +
				storage.cubemap_storage.indices.size() +
				storage.texture_storage.indices.size();

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
