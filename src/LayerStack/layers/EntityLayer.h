/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ENTITY_LAYER_H
#define MAR_ENGINE_ENTITY_LAYER_H

#include "../../mar.h"
#include "../Layer.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/SceneManager.h"


namespace mar {
	namespace graphics { class RendererEntity; }

	namespace layers {


		class EntityLayer : public Layer {
			const char* m_debugName;
			ecs::SceneManager m_sceneManager;
			graphics::RendererEntity* m_renderer;

		public:
			EntityLayer(const char* debugname);

			void initialize(ecs::Scene* scene = nullptr);

			// --- OVERLOADED METHODS --- //

			void update() override;

			void closeLayer() override;

			// --- GET METHODS --- //

			graphics::RendererEntity* getRenderer();
			ecs::SceneManager* getSceneManager();
		};



} }


#endif // !MAR_ENGINE_ENTITY_LAYER_H
