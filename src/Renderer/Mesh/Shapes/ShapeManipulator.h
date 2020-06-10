/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHAPEMANIPULATOR_H
#define SHAPEMANIPULATOR_H

#include "../../../mar.h"
#include "Shapes.h"

namespace mar {
    namespace graphics {


        //! ShapeManipulator
		/*!
		 ShapeManipulator class is a static class, which contains everything associated scene configuration.
		 Implementations of rotating object, changing indices for specified format are
		 very useful and this is why I have written Mesh class.
		*/
        class ShapeManipulator {
        public:
            //! Default constructor. Actually not used
            ShapeManipulator() = default;

            //! Method changes the Shape ID value in verticesVector of any shape. It works every time, when ID
            //! is on the last position of data. 
            /*!
                \param shape - pointer to shape, which id must be changed
                \param nextID - id value for shape
            */
            static void extendShapeID(Ref<Shape>& shape, const float& nextID);

            //! Method changes the Texture ID value in verticesVector of any shape. It works every time, when ID
            //! is on the next to last position of data. 
            /*!
                \param shape - pointer to shape, which id must be changed
                \param nextID - id value for shape
            */
            static void extendTextureID(Ref<Shape>& shape, const float& nextID);

            //! Method is just easier implementation of extending indices method. For more information check the method below.
            /*!
                \param shape - pointer to shape
                \param max_value - max value of indices in current indices vector of renderer
            */
            static void changeIndicesFormat(Ref<Shape>& shape, int& max_value);

            //! If we want batch rendering, we need to change indices values for every shape. Default ones begin with 0.
            //! There is need to change begin value to new, in case of proper work of batch renderer.
            /*!
                \param size - size of indicesVector
                \param max_value - max value of indices in current indices vector of renderer
                \param passedValue - passed indicesVector, its values are gonna be used to calculate new ones
                \return returnValue - vector containing new, extended, indices
            */
            static std::vector<unsigned int> changeIndicesFormat(const unsigned int& size, int& max_value,
                const std::vector<unsigned int>& passedValue);
        };


} }

#endif // !SHAPEMANIPULATOR_H