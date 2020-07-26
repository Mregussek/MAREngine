/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHAPEMANIPULATOR_H
#define SHAPEMANIPULATOR_H

#include "../../../mar.h"
#include "DefaultShapes/Cube.h"

namespace mar {
    namespace graphics {


		/*
        ShapeManipulator

		 ShapeManipulator class is a static class, which contains everything associated scene configuration.
		 Implementations of rotating object, changing indices for specified format are
		 very useful and this is why I have written Mesh class.
		*/
        class ShapeManipulator {
        public:
            //! Default constructor. Actually not used
            ShapeManipulator() = default;

            /*
            Method changes the Shape ID value in verticesVector of any shape. It works every time, when ID
            is on the last position of data. 
            
                \param shape - pointer to shape, which m_id must be changed
                \param nextID - m_id value for shape
            */
            static void extendShapeID(Ref<Shape>& shape, const float& nextID);

            /*
            Method changes the Shape ID value in verticesVector of any shape. It works every time, when ID
            is on the last position of data.

                \param stride - stride to find id in vertices
                \param vertices - vector of vertices
                \param newid - m_id value for shape
            */
            static void extendShapeID(std::vector<float>& vertices, const uint32_t& stride, const float& newid);

            /*
             Method changes the Texture ID value in verticesVector of any shape. It works every time, when ID
             is on the next to last position of data. 
            
                \param shape - pointer to shape, which m_id must be changed
                \param nextID - m_id value for shape
            */
            static void extendTextureID(Ref<Shape>& shape, const float& nextID);

            /*
            Method changes the Texture ID value in verticesVector of any shape. It works every time, when ID
            is on the next to last position of data.

                \param stride - stride to find texture id in vertices
                \param vertices - vector of vertices
                \param newid - new texture id
            */
            static void extendTextureID(std::vector<float>& vertices, const uint32_t& stride, const float& newid);

            /*
            Method changes both ids in vertices vector - shape id and texture id.

                \param stride - stride to find texture id in vertices
                \param vertices - vector of vertices
                \param newid - new texture id
            */
            static void extendBothIDs(std::vector<float>& vertices, const uint32_t& stride, const float& newid);

            /*
            Method extends indices, in order to batch render things.

                \param indices - indices which will be extended
                \param extension - factor for increasing indices
            */
            static void extendIndices(std::vector<uint32_t>& indices, const uint32_t& extension);

            /* 
            Method is just easier implementation of extending indices method. For more information check the method below.
            
                \param shape - pointer to shape
                \param max_value - max value of indices in current indices vector of renderer
            */
            static void changeIndicesFormat(Ref<Shape>& shape, int& max_value);

            /*
            If we want batch rendering, we need to change indices values for every shape. Default ones begin with 0.
            There is need to change begin value to new, in case of proper work of batch renderer.
            
                \param size - size of indicesVector
                \param max_value - max value of indices in current indices vector of renderer
                \param passedValue - passed indicesVector, its values are gonna be used to calculate new ones
                \return returnValue - vector containing new, extended, indices
            */
            static std::vector<uint32_t> changeIndicesFormat(const uint32_t& size, int& max_value,
                const std::vector<uint32_t>& passedValue);
        };


} }

#endif // !SHAPEMANIPULATOR_H