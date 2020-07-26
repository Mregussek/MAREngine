/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_SCENE_H
#define MAR_ENGINE_SCENE_H

#include "../../mar.h"
#include "Component/Components.h"


namespace mar {
	namespace ecs {

		class Entity;


		class Scene {
			friend class Entity;

			entt::registry m_registry;

			std::vector<Entity> m_entities;

			std::vector<float> m_vertices;
			std::vector<uint32_t> m_indices;
			std::vector<int32_t> m_samplers;
			std::vector<maths::mat4> m_transforms;
			std::vector<maths::vec3> m_colors;

		public:
			Scene();
			~Scene();

			void update();

			Entity createEntity();

			// --- GET METHODS --- //
			inline const std::vector<float>& getVertices() const { return m_vertices; }
			inline const std::vector<uint32_t>& getIndices() const { return m_indices; }
			inline const std::vector<maths::mat4>& getTransforms() const { return m_transforms; }
			inline const std::vector<int32_t>& getSamplers() const { return m_samplers; }
			inline const std::vector<maths::vec3>& getColors() const { return m_colors; }

			inline const uint32_t& getEntitiesCount() const { return (uint32_t) m_registry.size(); }
		};


} }


#endif // !MAR_ENGINE_SCENE_H