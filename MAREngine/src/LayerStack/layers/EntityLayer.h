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
#include "../../Core/graphics/Renderer/RendererBatch.h"
#include "../../Core/graphics/Renderer/RenderPipeline.h"


namespace mar {
	namespace graphics { class RendererBatch; }

	namespace layers {


		class EntityLayer : public Layer {
			const char* m_debugName;
			ecs::SceneManager m_sceneManager;
			graphics::RendererBatch m_renderer;

		public:
			EntityLayer(const char* debugname);

			void initialize(ecs::Scene* scene = nullptr);

			// --- OVERLOADED METHODS --- //

			void update() override;

			void closeLayer() override;

			// --- GET METHODS --- //

			graphics::RendererBatch* getRenderer();
			ecs::SceneManager* getSceneManager();
		};



} }


#endif // !MAR_ENGINE_ENTITY_LAYER_H
