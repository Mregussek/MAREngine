/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include "../../../../mar.h"
#include "../../../../Debug/Log.h"


namespace mar {
	namespace graphics {


		/*!
		VertexBufferElement

			VertexBufferElement stores data, which will be put in GPU. Helps set up
			VertexAttribPointer.
		*/
		struct VertexBufferElement {
			uint32_t _type;
			uint32_t _count;
			unsigned char _normalized;
		};

		/*!
		PushBuffer

			PushBuffer helps in deciding what type of value
			we are pushing to buffer.
		*/
		enum class PushBuffer {
			PUSH_FLOAT,
			PUSH_UNSIGNED_INT,
			PUSH_UNSIGNED_BYTE
		};

		/*!
		VertexBufferLayout

			VertexBufferLayout is used for collecting format of vertices. Main point of this class is for
			automating creating new objects.
			This is just base class for other implementations of VertexBufferLayout.
		*/
		class VertexBufferLayout {
			std::vector<VertexBufferElement> m_return;

		public:
			//! Constructor, which should initialize stride counter with 0
			VertexBufferLayout() = default;

			/*
			Should returns all elements, which help the data to be formatted properly

				\return _elements - vector of VertexBufferElement
			*/
			virtual const std::vector<VertexBufferElement>& getElements() const { return m_return; }

			/*
			 Method should returns stride of data

				\return _stride - calculated stride for proper data format
			*/
			virtual uint32_t getStride() const { return 0; }

			/*
			Method should pushData new element to layout by specifying its type and count

				\param count - value of next Vertex Attribute
				\param what - specify what value you pushData (float[PUSH_FLOAT], uint32_t[PUSH_uint32_t] or char[PUSH_UNSIGNED_BYTE])
			*/
			virtual void push(uint32_t count, PushBuffer what) { }
		};


} }

#endif // VERTEXBUFFERLAYOUT_H