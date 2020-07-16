/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FRAME_BUFFER_H
#define MAR_ENGINE_FRAME_BUFFER_H

#include "../../../../mar.h"
#include "../../../../Debug/Log.h"


namespace mar {
	namespace graphics {


		struct FrameBufferSpecification {
			float width{ 0.f };
			float height{ 0.f };
			uint32_t samples = 1;

			bool swapChainTarget = false;
			
			FrameBufferSpecification() = default;
			FrameBufferSpecification(float w, float h) : width(w), height(h) { }
		};


		class FrameBuffer : std::enable_shared_from_this<FrameBuffer> {
			FrameBufferSpecification m_spec;

		public:
			FrameBuffer() = default;

			virtual void initialize(const FrameBufferSpecification& spec) { }

			virtual void bind() const { }

			virtual void unbind() const { }

			virtual void clear() const  { }

			virtual void close() { }

			virtual const uint32_t& getColorAttach() const { return 0; }

			virtual const uint32_t& getDepthAttach() const { return 0; }

			virtual const FrameBufferSpecification& getSpecification() const { return m_spec; }
		};


} }


#endif	// !MAR_ENGINE_FRAME_BUFFER_OPENGL_H
