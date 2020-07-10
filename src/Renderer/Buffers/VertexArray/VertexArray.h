/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "../../../mar.h"
#include "../VertexBuffer/VertexBufferLayout.h"
#include "../../../Debug/Log.h"


namespace mar {
	namespace graphics {


		/*!
		VertexArray

			VertexArray is a object needed for storing state for supplying vertex data. Should contain
			format of vertex data, by providing vertex arrays.
			This is just base class for other implementations of VertexArray.
		*/
		class VertexArray : std::enable_shared_from_this<VertexArray> {
		public:
			//! Default constructor. For initialization use initializeArrayBuffer() method.
			VertexArray() = default;

			//! Method should generates and binds VAO. Method must be overloaded!
			virtual void initializeArrayBuffer() { }

			//! Method should deletes all data associated with VAO. Must be overloaded!
			virtual void closeArrayBuffer() { }

			/*
			Method should defines array of generic vertex attribute data and enables it
			Must be overloaded!

				\param layout - specifies, how data should be formatted
			*/
			virtual void addBuffer(const Ref<VertexBufferLayout>& layout) { }

			//! Method should bind class m_id member to target, which is VAO. Must be overloaded!
			virtual void bind() const { }

			//! Method should unbind currently used VAO, break the existing VAO binding
			//! Must be overloaded!
			virtual void unbind() const { }
		};


} }


#endif // !VERTEXARRAY_H
