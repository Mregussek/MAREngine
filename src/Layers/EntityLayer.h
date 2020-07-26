/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ENTITY_LAYER_H
#define MAR_ENGINE_ENTITY_LAYER_H

#include "../mar.h"
#include "Layer.h"
#include "../Core/Scene/Scene.h"
#include "../Core/Scene/Entity/Entity.h"
#include "../Core/Renderer/Buffers/FrameBuffer/FrameBuffer.h"
#include "../Core/Renderer/Buffers/FrameBuffer/FrameBufferOpenGL.h"
#include "../Core/Shader/Shader.h"


namespace mar {
	namespace layers {


		class EntityLayer : public Layer {
			const char* m_debugName;
			ecs::Scene m_scene;
			graphics::RendererOpenGL* m_renderer;
			graphics::FrameBuffer<graphics::FrameBufferOpenGL> m_framebuffer;

		public:
			EntityLayer(const char* debugname)
				: m_debugName(debugname)
			{}

			void initialize(const std::vector<uint32_t>& layout);

			void prepareFrame() override;
			void update() override;
			void endFrame() override;

			void closeLayer() override;

			void set(const graphics::FrameBuffer<graphics::FrameBufferOpenGL>& framebuffer) {
				m_framebuffer = framebuffer;
			}

			inline ecs::Scene& getScene() { return m_scene; }
		};



} }


#endif // !MAR_ENGINE_ENTITY_LAYER_H
