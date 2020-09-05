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
