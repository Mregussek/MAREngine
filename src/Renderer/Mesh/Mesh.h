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
			std::shared_ptr<Texture> m_texture;

			std::vector<std::shared_ptr<Shape>> m_shapes;
			std::vector<std::string> m_names;
			std::vector<float> m_vertices;
			std::vector<unsigned int> m_indices;
			std::vector<int> m_samplers;
			std::vector<glm::mat4> m_translationMats;
			std::vector<glm::mat4> m_rotationMats;

			Light _light;

			float m_availableShapeID;
			float m_availableTextureID;
			int m_indicesMaxValue;

		public:
			Mesh() = default;
			virtual ~Mesh();

			void createMesh(const std::shared_ptr<RendererFactory>& factory);
			void loadScene(Scene* scene);

			void submitShape(std::shared_ptr<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::string& texture);
			void flushShape(const unsigned int& index);

			void clearBuffers();
			void clearMatrices();

			void update();

		public:
			void pushShape(std::shared_ptr<Shape>& new_shape);
			void popShape(const unsigned int& index);

			void pushMatrices(const glm::vec3& center, const glm::vec3& angle);
			void pushTexture(std::shared_ptr<Shape>& new_shape, const std::string& texture);

			/// --- GET METHODS --- ///
			const unsigned int& getShapesCount() const { return m_shapes.size(); }
			const std::vector<std::string>& getNames() const { return m_names; }
			const std::string& getName(const unsigned int& index) const { return m_names[index]; }

			const std::vector<float>& getVertices() const { return m_vertices; }
			const unsigned int& getVerticesSize() const { return m_vertices.size(); }

			const std::vector<unsigned int>& getIndices() const { return m_indices; }
			const unsigned int& getIndicesSize() const { return m_indices.size(); }
			const std::vector<int>& getSamplers() const { return m_samplers; }
			const int& getSamplerID(const unsigned int& index) { return m_samplers[index]; }
			const unsigned int& getSamplersSize() const { return m_samplers.size(); }

			const std::vector<glm::mat4>& getTranslationMatrices() const { return m_translationMats; }
			const std::vector<glm::mat4>& getRotationMatrices() const { return m_rotationMats; }
			
			const std::vector<unsigned int>& getLayout() const { return m_shapes[0]->getLayoutVector();}
			const unsigned int& getLayout(const unsigned int& index) const { return m_shapes[0]->getLayout(index); }
			const unsigned int& getLayoutSize() const { return m_shapes[0]->getLayoutSize(); }

			const glm::vec3& getLightPosition() const { return _light.getPosition(); }
		};


} }

#endif // !MAR_ENGINE_MESH_H

