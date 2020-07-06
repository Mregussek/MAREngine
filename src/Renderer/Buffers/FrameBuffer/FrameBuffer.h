/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FRAME_BUFFER_H
#define MAR_ENGINE_FRAME_BUFFER_H

#include "../../../mar.h"
#include "../../../Debug/Log.h"


namespace mar {
	namespace graphics {


		struct FrameBufferSpecification {
			float width{ 0.f };
			float height{ 0.f };
			unsigned int samples = 1;

			bool swapChainTarget = false;
			
			FrameBufferSpecification() = default;
			FrameBufferSpecification(float w, float h) : width(w), height(h) { }
		};


		class FrameBuffer : std::enable_shared_from_this<FrameBuffer> {
		public:
			FrameBuffer() = default;

			virtual void initialize(const FrameBufferSpecification& spec) { }

			virtual void bind() const { }

			virtual void unbind() const { }

			virtual void clear() const  { }

			virtual void close() { }

			virtual const unsigned int& getColorAttach() const { return unsigned int(0); }

			virtual const unsigned int& getDepthAttach() const { return unsigned int(0); }

			virtual const FrameBufferSpecification& getSpecification() const { return FrameBufferSpecification(); }
		};


} }


#endif	// !MAR_ENGINE_FRAME_BUFFER_OPENGL_H
