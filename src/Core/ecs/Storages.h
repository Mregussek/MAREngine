/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include <vector>
#include "ECS/Components.h"


namespace mar {
	namespace ecs {


		template<typename T>
		struct SceneStorage {
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


} }
