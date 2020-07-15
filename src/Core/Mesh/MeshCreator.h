/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_CREATOR_H
#define MAR_ENGINE_MESH_CREATOR_H


#include "../../mar.h"
#include "Shapes/ShapeManipulator.h"
#include "Texture/TexturePaths.h"


namespace mar {
    namespace graphics {


        class MeshCreator {
        public:
            static Ref<Shape> createEmptyShape() {
                return PointerFactory<Shape>::makeRef();
            }

            static Ref<Shape> createCube() {
                return PointerFactory<Cube>::makeRef();
            }

            static Ref<Shape> createWall() {
                return PointerFactory<Wall>::makeRef();
            }

            static Ref<Shape> createSurface() {
                return PointerFactory<Surface>::makeRef();
            }

            static Ref<Shape> createPyramid() {
                return PointerFactory<Pyramid>::makeRef();
            }

            static void moveShape(Ref<Shape>& ref_shape, Ref<Shape>& shape_to_move) {
                ref_shape->setName(shape_to_move->getName());
                ref_shape->setID(shape_to_move->getID());
                ref_shape->setTextureID(shape_to_move->getTextureID());
                ref_shape->setUsedTexture(shape_to_move->getUsedTexture());
                ref_shape->setUsedObj(shape_to_move->getUsedObj());
                ref_shape->setCenter(shape_to_move->getCenter());
                ref_shape->setAngle(shape_to_move->getAngle());
                ref_shape->setScaleVec(shape_to_move->getScaleVec());
                ref_shape->setVertices(shape_to_move->getVertices());
                ref_shape->setIndices(shape_to_move->getIndices());
            }

        };


} }


#endif // !MAR_ENGINE_MESH_CREATOR_H