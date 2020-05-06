/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXARRAY_OPENGL_H
#define VERTEXARRAY_OPENGL_H

#include "../../../mar.h"
#include "VertexArray.h"
#include "../VertexBuffer/VertexBufferLayout.h"

namespace mar {
	class VertexArrayOpenGL : public VertexArray, std::enable_shared_from_this<VertexArrayOpenGL> {
		unsigned int _rendererId;
		size_t _howMany;

	public:
		VertexArrayOpenGL() = default;
		~VertexArrayOpenGL() override;

		void initializeArrayBuffer() override;
		void addBuffer(const std::shared_ptr<VertexBufferLayout>& layout) override;

		void bind() const override;
		void unbind() const override;
	};
}

#endif // !VERTEXARRAY_OPENGL_H
