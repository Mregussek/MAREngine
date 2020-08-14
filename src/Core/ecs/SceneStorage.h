/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_SCENE_STORAGE_H
#define MAR_ENGINE_ECS_SCENE_STORAGE_H

#include <vector>
#include "ECS/Components.h"


namespace mar {
	namespace ecs {


		template<typename T>
		struct BufferStorage {
			std::vector<float> vertices;
			std::vector<uint32_t> indices;
			std::vector<maths::mat4> transforms;
			std::vector<std::string> paths; // only for textures!
			std::vector<T> samplers;
			int32_t counter = 0;
			uint32_t indicesMax = 0;
			static const int32_t stride = 3 + 3 + 2 + 1;
		};

		struct LightStorage {
			std::vector<maths::vec3> positions;
			std::vector<LightComponent> components;
		};

		struct PlayStorage {
			std::vector<TransformComponent> transforms;
			std::vector<ColorComponent> colors;
			std::vector<LightComponent> lights;
		};


		// -------------------------------------------------------------
		// SCENE STORAGE
		// -------------------------------------------------------------

		struct SceneStorage {
			BufferStorage<maths::vec3> colors_storage;
			BufferStorage<int32_t> texture_storage;
			BufferStorage<int32_t> cubemap_storage;
			LightStorage light_storage;
			PlayStorage play_storage;

			void resetAll() {
				reset(colors_storage);
				reset(texture_storage);
				reset(cubemap_storage);
				reset(light_storage);
				reset(play_storage);
			}

			template<typename T>
			void reset(BufferStorage<T>& storage) {
				storage.vertices.clear();
				storage.indices.clear();
				storage.transforms.clear();
				storage.paths.clear();
				storage.samplers.clear();
				storage.counter = 0;
				storage.indicesMax = 0;
			}

			void reset(LightStorage& light) {
				light.components.clear();
				light.positions.clear();
			}

			void reset(PlayStorage& play) {
				play.transforms.clear();
				play.colors.clear();
				play.lights.clear();
			}

			void pushLight(maths::vec3& center, LightComponent& light) {
				light_storage.positions.push_back(center);
				light_storage.components.push_back(light);
			}
		};



} }



#endif // !MAR_ENGINE_ECS_SCENE_STORAGE_H
