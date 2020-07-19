/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "filesystem.h"


namespace mar {
	namespace editor {


		std::optional<std::vector<layers::MeshLayer*>> filesystem::assignLoadedLayers(graphics::FrameBuffer<graphics::FrameBufferOpenGL>& framebuffer) {
			if (shouldLoadMeshes()) {
				if (checkMeshEquality()) {
					MAR_CORE_ERROR("Mesh Types size is not equal to mesh_count!");
					return std::nullopt;
				}

				return loadMesh(framebuffer);
			}

			if (shouldLoadScene()) {
				if (checkSceneEquality()) {
					MAR_CORE_ERROR("Loaded types of mesh are not equal to the size of scene types size!");
					return std::nullopt;
				}

				return loadScene(framebuffer);
			}

			MAR_CORE_ERROR("Unsupported Data!");
			return std::nullopt;
		}

		const std::vector<layers::MeshLayer*> filesystem::loadMesh(graphics::FrameBuffer<graphics::FrameBufferOpenGL>& framebuffer) {
			std::vector<layers::MeshLayer*> lay;

			for (size_t i = 0; i < getMeshTypes().size(); i++) {
				auto layer = new layers::MeshLayer("Mesh Layer");
				layer->initialize();
				layer->getMesh()->setMeshType(getMeshTypes()[i]);

				for (uint32_t j = 0; j < getShapes()[i].size(); j++) {
					auto shape =  getShapes()[i][j];
					auto center = getCenters()[i][j];
					auto angle =  getAngles()[i][j];
					auto scale =  getScales()[i][j];
					auto color =  getColors()[i][j];
					auto texture = getTextures()[i][j];

					if (layer->getMesh()->getMeshType() == OBJECTS_MESH_TYPE) {
						auto obj = getObjs()[i][j];
						shape->assignDataFromFile(obj.c_str());
					}

					layer->getMesh()->submitShape(shape, center, angle, scale, texture.c_str());
				}

				layer->load();
				layer->set(framebuffer);
				lay.push_back(layer);
			}

			MAR_CORE_INFO("Scene is loaded from meshes!");

			return lay;
		}

		const std::vector<layers::MeshLayer*> filesystem::loadScene(graphics::FrameBuffer<graphics::FrameBufferOpenGL>& framebuffer) {
			std::vector<layers::MeshLayer*> lay;

			for (uint32_t i = 0; i < getMeshTypes().size(); i++) {
				auto layer = new layers::MeshLayer("Mesh Layer");
				layer->initialize();
				layer->scene(getSceneTypes()[i], getMeshTypes()[i]);
				layer->set(framebuffer);
				lay.push_back(layer);
			}

			MAR_CORE_INFO("Default scene is being loaded!");

			return lay;
		}


} }