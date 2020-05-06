/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFER_OPENGL_H
#define VERTEXBUFFER_OPENGL_H

#include "VertexBuffer.h"

namespace mar {
	class VertexBufferOpenGL : public VertexBuffer, std::enable_shared_from_this<VertexBufferOpenGL> {
		unsigned int _id;
		unsigned int _size;
		unsigned int _allocatedMemory;
		bool _initialized = false;

	public:
		VertexBufferOpenGL() = default;

		void initializeVertex(unsigned int allocationMemory) override;

		void bind() const override;
		void updateDynamically(const std::vector<float>& vertices) const override;
		void unbind() const override;
		void close() override;

		unsigned int getSize() const override;
	};
}

#endif // VERTEXBUFFER_OPENGL_H