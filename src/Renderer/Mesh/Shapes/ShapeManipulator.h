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
            static void extendShapeID(std::shared_ptr<Shape>& shape, const float& nextID);

            //! Method changes the Texture ID value in verticesVector of any shape. It works every time, when ID
            //! is on the next to last position of data. 
            /*!
                \param shape - pointer to shape, which id must be changed
                \param nextID - id value for shape
            */
            static void extendTextureID(std::shared_ptr<Shape>& shape, const float& nextID);

            //! Method is just easier implementation of shape rotation method. We need to pass only 2 values,
            //! instead of 5. For more information check the method below.
            /*!
                \param shape - pointer to shape, which will be rotated
                \param angle - vec3 contains angles for x, y and z axis.
            */
            static void rotateObject(std::shared_ptr<Shape>& shape, const glm::vec3& angle);

            //! Method returns new verticesVector for shape after rotation process. It rotates around all axis,
            //! x, y and z. For proper working you have to pass a few arguments.
            /*!
                \param size - size of verticesVector
                \param stride - stride of verticesVector
                \param angle - vec3 contains angles all axis (angle.x is angle for x axis and so on)
                \param center - point of rotation
                \param passedValue - passed verticesVector, we are gonna use this values for rotation
                \return returnValue - vector containing new, rotated, verticesVector
            */
            static std::vector<float> rotateObject(const unsigned int& size, const unsigned int& stride,
                const glm::vec3& angle, const glm::vec3& center, const std::vector<float>& passedValue);

            //! Method is just easier implementation of shape new position method. We need to pass only 2 values,
            //! instead of 4. For more information check the method below.
            /*!
                \param shape - pointer to shape
                \param center - new center point for shape
            */
            static void changeCenterOfObject(std::shared_ptr<Shape>& shape, const glm::vec3& center);

            //! Method returns new verticesVector for shape after changing center point process. 
            //! For proper working you have to pass a few arguments.
            /*!
                \param size - size of verticesVector
                \param stride - stride of verticesVector
                \param center - point of rotation
                \param passedValue - passed verticesVector, we are gonna use this values for changing center
                \return returnValue - vector containing new, translated, verticesVector
            */
            static std::vector<float> changeCenterOfObject(const unsigned int& size, const unsigned int& stride,
                const glm::vec3& center, const std::vector<float>& passedValue);

            //! Method is just easier implementation of extending indices method. For more information check the method below.
            /*!
                \param shape - pointer to shape
                \param max_value - max value of indices in current indices vector of renderer
            */
            static void changeIndicesFormat(std::shared_ptr<Shape>& shape, unsigned int& max_value);

            //! If we want batch rendering, we need to change indices values for every shape. Default ones begin with 0.
            //! There is need to change begin value to new, in case of proper work of batch renderer.
            /*!
                \param size - size of indicesVector
                \param max_value - max value of indices in current indices vector of renderer
                \param passedValue - passed indicesVector, its values are gonna be used to calculate new ones
                \return returnValue - vector containing new, extended, indices
            */
            static std::vector<unsigned int> changeIndicesFormat(const unsigned int& size, unsigned int& max_value,
                const std::vector<unsigned int>& passedValue);

            static void changeDefaultColor(std::shared_ptr<Shape>& shape, glm::vec3& new_color);
            static std::vector<float> changeDefaultColor(const unsigned int& size, const unsigned int& stride,
                glm::vec3& new_color, const std::vector<float>& passedValue);

            //! Returns rotation matrix of all axis (x, y, z)
            /*!
                \param center - point of rotation
                \param angle - vec3 contains angles all axis (angle.x is angle for x axis and so on)
                \return matrix4x4 - structure containing rotation matrix
            */
            static glm::mat4 getRotationMatrix(const glm::vec3& center, const glm::vec3& angle);

            //! Returns rotation matrix of all axis (x, y, z)
            /*!
                \param center - point of rotation
                \param angle - just angle in degrees
                \return matrix4x4 - structure containing rotation matrix
            */
            static glm::mat4 getRotationMatrix(const glm::vec3& center, const float& angle);
        };


} }

#endif // !SHAPEMANIPULATOR_H