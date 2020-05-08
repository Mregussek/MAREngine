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
		//! Default constructor. For initialization use initializeArrayBuffer() method.
		VertexArrayOpenGL() = default;

		//! Generates and binds VAO
		void initializeArrayBuffer() override;

		//! Deletes VAO
		void closeArrayBuffer() override;

		//! Defines array of generic vertex attribute data and enables it
		/*
			\param layout - specifies, how data should be formatted
		*/
		void addBuffer(const std::shared_ptr<VertexBufferLayout>& layout) override;

		//! Binds class _id member to target, which is VAO
		void bind() const override;

		//! Unbind currently used VAO, break the existing VAO binding
		void unbind() const override;
	};
}

#endif // !VERTEXARRAY_OPENGL_H
