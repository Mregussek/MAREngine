/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_VERTEXARRAY_OPENGL_H
#define MAR_ENGINE_VERTEXARRAY_OPENGL_H

#include "../../../../../mar.h"
#include "VertexArray.h"


namespace mar {
	namespace graphics {


		/*!
		VertexArrayOpenGL

			VertexArrayOpenGL is a object needed for storing state for supplying vertex data. Should contain
			format of vertex data, by providing vertex arrays.
			This is overloaded class of base one - VertexArrayOpenGL, which implements solution to that problem with OpenGL.
		*/
		class VertexArrayOpenGL {
			uint32_t _rendererId;

		public:
			//! Default constructor. For initialization use initialize() method.
			VertexArrayOpenGL() = default;

			//! Generates and binds VAO
			void initialize();

			//! Deletes VAO
			void close();

			/*
			Defines array of generic vertex attribute data and enables it

				\param layout - specifies, how data should be formatted
			*/
			void addBuffer(const VertexBufferLayoutOpenGL& layout);

			//! Binds class m_id member to target, which is VAO
			void bind() const;

			//! Unbind currently used VAO, break the existing VAO binding
			void unbind() const;
		};


} }

#endif // !MAR_ENGINE_VERTEXARRAY_OPENGL_H
