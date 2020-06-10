/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_H
#define MAR_ENGINE_MESH_H

#include "../../mar.h"
#include "Shapes/Shapes.h"
#include "Shapes/DefaultShapes/Cube.h"
#include "Shapes/DefaultShapes/Wall.h"
#include "Shapes/DefaultShapes/Surface.h"
#include "Shapes/DefaultShapes/Pyramid.h"
#include "../Renderer.h"
#include "Light/Light.h"


namespace mar {
    namespace graphics {


		class Mesh {
			Ref<Texture> m_texture;

			std::vector<Ref<Shape>> m_shapes;
			std::vector<float> m_vertices;
			std::vector<unsigned int> m_indices;
			std::vector<int> m_samplers;
			std::vector<glm::mat4> m_translationMats;
			std::vector<glm::mat4> m_rotationMats;

			Light m_light;

			float m_availableShapeID;
			float m_availableTextureID;
			int m_indicesMaxValue;

		public:
			Mesh() = default;
			virtual ~Mesh();

			void createMesh(const Ref<RendererFactory>& factory);
			void loadScene(Scene* scene);

			void submitShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::string& texture);
			void flushShape(const unsigned int& index);

			void clearBuffers();
			void clearMatrices();

			void update();

		public:
			void pushShape(Ref<Shape>& new_shape);
			void popShape(const unsigned int& index);

			void pushMatrices(const glm::vec3& center, const glm::vec3& angle);
			void pushTexture(Ref<Shape>& new_shape, const std::string& texture);

			/// --- GET METHODS --- ///
			const unsigned int& getShapesCount() const { return m_shapes.size(); }
			const std::string& getName(const unsigned int& index) const { return m_shapes[index]->getName(); }

			const std::vector<float>& getVertices() const { return m_vertices; }
			const unsigned int& getVerticesSize() const { return m_vertices.size(); }

			const std::vector<unsigned int>& getIndices() const { return m_indices; }
			const unsigned int& getIndicesSize() const { return m_indices.size(); }
			const std::vector<int>& getSamplers() const { return m_samplers; }
			const int& getSamplerID(const unsigned int& index) { return m_samplers[index]; }
			const unsigned int& getSamplersSize() const { return m_samplers.size(); }

			const std::vector<glm::mat4>& getTranslationMatrices() const { return m_translationMats; }
			const std::vector<glm::mat4>& getRotationMatrices() const { return m_rotationMats; }
			
			const std::vector<unsigned int>& getLayout() const { return m_shapes[0]->getLayoutVector(); }
			const unsigned int& getLayout(const unsigned int& index) const { return m_shapes[0]->getLayout(index); }
			const unsigned int& getLayoutSize() const { return m_shapes[0]->getLayoutSize(); }

			Light& getLight() { return m_light; }

			glm::vec3& getCenter(const unsigned int& index) { return m_shapes[index]->getCenter(); }
			glm::vec3& getAngle(const unsigned int& index) { return m_shapes[index]->getAngle(); }

			// --- SET METHODS --- //
			void setCenter(const unsigned int& index, const glm::vec3& new_center) { m_shapes[index]->setCenter(new_center); }
			void setAngle(const unsigned int& index, const glm::vec3& new_angle) { m_shapes[index]->setAngle(new_angle); }
		};


} }

#endif // !MAR_ENGINE_MESH_H

