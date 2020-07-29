/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ENTITY_LAYER_H
#define MAR_ENGINE_ENTITY_LAYER_H

#include "../../mar.h"
#include "../Layer.h"
#include "../../Core/Scene/Scene.h"


namespace mar {
	namespace graphics { class RendererEntity; }

	namespace layers {


		class EntityLayer : public Layer {
			const char* m_debugName;
			ecs::Scene m_scene;
			graphics::RendererEntity* m_renderer;

		public:
			EntityLayer(const char* debugname);

			void initialize();

			// --- OVERLOADED METHODS --- //

			void update() override;

			void closeLayer() override;

			// --- GET METHODS --- //

			graphics::RendererEntity* getRenderer();
			ecs::Scene* getScene();
		};



} }


#endif // !MAR_ENGINE_ENTITY_LAYER_H