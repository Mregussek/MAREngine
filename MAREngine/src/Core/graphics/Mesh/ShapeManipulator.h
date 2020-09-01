/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GRAPHICS_SHAPEMANIPULATOR_H
#define MAR_ENGINE_GRAPHICS_SHAPEMANIPULATOR_H

#include "../../../mar.h"
#include "../GraphicsLogs.h"

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

            /*
            Method changes the Shape ID value in verticesVector of any shape. It works every time, when ID
            is on the last position of data.

                \param stride - stride to find id in vertices
                \param vertices - vector of vertices
                \param newid - m_id value for shape
            */
            static void extendShapeID(std::vector<float>& vertices, uint32_t stride, float newid);

            /*
            Method extends indices, in order to batch render things.

                \param indices - indices which will be extended
                \param extension - factor for increasing indices
            */
            static void extendIndices(std::vector<uint32_t>& indices, uint32_t extension);

            /*
            Method extends indices, in order to batch render things.

                \param indices - indices which will be extended
                \param start - from which indice should we start
                \param end - at which indice should extension end
                \param extension - factor for increasing indices
            */
            static void extendIndices(std::vector<uint32_t>& indices, uint32_t start, uint32_t end, uint32_t extension);

            /*
            Calculates vertex normals for set of vertices. Make sure that mesh is triangulated (indices.size() divisible by 3!).
            Also correct stride is needed, in order for correct calculation! (If wrong there will be seg fault!)
                
                \param vertices - vector of vertices (non-const in order of modification)
                \param indices - vector of indices
                \param stride - number, where new vertex begins
            */
            static void calculateNormals(std::vector<float>& vertices, const std::vector<uint32_t>& indices, const int32_t stride);
        
        };


} }

#endif // !MAR_ENGINE_SHAPEMANIPULATOR_H