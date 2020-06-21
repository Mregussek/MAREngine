/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include "../../../mar.h"

namespace mar {
	namespace graphics {


		/*!
		VertexBufferElement

			VertexBufferElement stores data, which will be put in GPU. Helps set up
			VertexAttribPointer.
		*/
		struct VertexBufferElement {
			unsigned int _type;
			unsigned int _count;
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
		public:
			//! Constructor, which should initialize stride counter with 0
			VertexBufferLayout() = default;

			/*
			Should returns all elements, which help the data to be formatted properly

				\return _elements - vector of VertexBufferElement
			*/
			virtual const std::vector<VertexBufferElement>& getElements() const { return std::vector<VertexBufferElement>(); }

			/*
			 Method should returns stride of data

				\return _stride - calculated stride for proper data format
			*/
			virtual unsigned int getStride() const { return 0; }

			/*
			Method should pushData new element to layout by specifying its type and count

				\param count - value of next Vertex Attribute
				\param what - specify what value you pushData (float[PUSH_FLOAT], unsigned int[PUSH_UNSIGNED INT] or char[PUSH_UNSIGNED_BYTE])
			*/
			virtual void push(unsigned int count, PushBuffer what) { }
		};


} }

#endif // VERTEXBUFFERLAYOUT_H