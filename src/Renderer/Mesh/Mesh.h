/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_H
#define MAR_ENGINE_MESH_H

#include "../../mar.h"
#include "../Renderer.h"
#include "Light/Light.h"


namespace mar {
    namespace graphics {


		class Mesh {
			std::shared_ptr<Texture> _texture;

			std::vector<std::shared_ptr<Shape>> _shapes;
			std::vector<float> m_vertices;
			std::vector<unsigned int> m_indices;
			std::vector<int> _samplers;
			std::vector<glm::mat4> m_translationMats;
			std::vector<glm::mat4> m_rotationMats;
			Light _light;
			std::vector<std::string> _names;

			float m_availableShapeID;
			float m_availableTextureID;
			unsigned int m_indicesMaxValue;

		public:
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
			void popMatrices(const unsigned int& index);

			void pushTexture(std::shared_ptr<Shape>& new_shape, const std::string& texture);
			void popTexture(const unsigned int& index);

			/// --- GET METHODS --- ///
			const unsigned int& getShapesCount() const { return _shapes.size(); }
			const std::vector<std::string>& getNames() const { return _names; }

			const std::vector<float>& getVertices() const { return m_vertices; }
			const unsigned int& getVerticesSize() const { return m_vertices.size(); }

			const std::vector<unsigned int>& getIndices() const { return m_indices; }
			const unsigned int& getIndicesSize() const { return m_indices.size(); }
			const std::vector<int>& getSamplers() const { return _samplers; }
			const int& getSamplerID(const unsigned int& index) { return _samplers[index]; }
			const unsigned int& getSamplersSize() const { return _samplers.size(); }

			const std::vector<glm::mat4>& getTranslationMatrices() const { return m_translationMats; }
			const std::vector<glm::mat4>& getRotationMatrices() const { return m_rotationMats; }
			
			const std::vector<unsigned int>& getLayout() const { return _shapes[0]->getLayoutVector();}
			const unsigned int& getLayout(const unsigned int& index) const { return _shapes[0]->getLayout(index); }
			const unsigned int& getLayoutSize() const { return _shapes[0]->getLayoutSize(); }

			const glm::vec3& getLightPosition() const { return _light.getPosition(); }
		};


} }

#endif // !MAR_ENGINE_MESH_H

