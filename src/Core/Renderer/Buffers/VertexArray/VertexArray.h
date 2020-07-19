/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "../../../../mar.h"
#include "../VertexBuffer/VertexBufferLayout.h"
#include "../VertexBuffer/VertexBufferLayoutOpenGL.h"
#include "../../../../Debug/Log.h"


namespace mar {
	namespace graphics {


		/*!
		VertexArray

			VertexArray is a object needed for storing state for supplying vertex data. Should contain
			format of vertex data, by providing vertex arrays.
			This is just base class for other implementations of VertexArray.

			Example Usage:

			VertexArray<VertexArrayOpenGL> m_vao;
		*/
		template<typename VAO>
		class VertexArray {
			VAO m_vao;

		public:
			//! Default constructor. For initialization use initializeArrayBuffer() method.
			VertexArray() = default;

			//! Method should generates and binds VAO. Method must be overloaded!
			void initializeArrayBuffer() { 
				m_vao.initializeArrayBuffer();
			}

			//! Method should deletes all data associated with VAO. Must be overloaded!
			void closeArrayBuffer() {
				m_vao.closeArrayBuffer();
			}

			/*
			Method should defines array of generic vertex attribute data and enables it
			Must be overloaded!

				\param layout - specifies, how data should be formatted
			*/
			void addBuffer(const VertexBufferLayout<VertexBufferLayoutOpenGL>& layout) {
				m_vao.addBuffer(layout);
			}

			// Method should bind class m_id member to target, which is VAO. Must be overloaded!
			void bind() const { 
				m_vao.bind();
			}

			// Method should unbind currently used VAO, break the existing VAO binding. Must be overloaded!
			void unbind() const { 
				m_vao.unbind();
			}

			/*
			Returns VAO in order to access original VAO.
			
				\return VAO - specified implementation of VAO (template)
			*/
			VAO& getInstance() {
				return m_vao;
			}
		};


} }


#endif // !VERTEXARRAY_H
