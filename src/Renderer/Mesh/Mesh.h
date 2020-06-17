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
#include "Texture/Texture.h"
#include "Light/Light.h"


namespace mar {
    namespace graphics {


		enum class MeshType {
			NORMAL,
			CUBEMAPS,
			OBJECTS
		};


		class Mesh {
			MeshType m_type;

			Ref<Texture> m_texture;

			std::vector<Ref<Shape>> m_shapes;
			std::vector<float> m_vertices;
			std::vector<unsigned int> m_indices;
			std::vector<int> m_samplers;
			std::vector<glm::mat4> m_translationMats;
			std::vector<glm::mat4> m_rotationMats;
			std::vector<glm::vec3> m_colors;

			Light m_light;

			float m_availableShapeID;
			int m_indicesMaxValue;
			static float s_availableTextureID;

			unsigned int m_shapesCount;
			unsigned int m_shapesDrawn;
			bool m_onlyCubeMaps = false;

		public:
			Mesh() = default;
			virtual ~Mesh();

			void createMesh(const Ref<RendererFactory>& factory);
			void loadScene(Scene* scene, MeshType type);

			void tryReuseShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::string& texture);
			void tryReuseShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::vector<std::string>& faces);

			void submitShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::string& texture);
			void submitShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const std::vector<std::string>& faces);

			void flushShape(const unsigned int& index);

			void resetDraw();
			void update();

			void clearBuffers();
			void clearMatrices();
			void clearColors();

			void pushShape(Ref<Shape>& new_shape);
			void popShape(const unsigned int& index);

			void pushMatrices(const glm::vec3& center, const glm::vec3& angle);
			void popMatrices(const unsigned int& index);

			void pushTexture(Ref<Shape>& new_shape, const std::string& texture);
			void pushCubeMap(Ref<Shape>& new_shape, const std::vector<std::string>& faces);

			/// --- GET METHODS --- ///
			inline const MeshType& getMeshType() const { return m_type; }

			inline const unsigned int& getShapesDrawn() const { return m_shapesDrawn; }

			inline const unsigned int& getTextureID(const unsigned int& index) const { return m_texture->getID(index); }
			inline const std::string& getShapesTexture(const unsigned int& index) const { return m_texture->getPath(index); }
			inline const unsigned int& getShapesCount() const { return m_shapesCount; }
			inline const std::string& getName(const unsigned int& index) const { return m_shapes[index]->getName(); }
	
			inline const std::vector<float>& getVertices() const { return m_vertices; }
			inline const unsigned int& getVerticesSize() const { return m_vertices.size(); }

			inline const std::vector<unsigned int>& getIndices() const { return m_indices; }
			inline const unsigned int& getIndicesSize() const { return m_indices.size(); }
			inline const std::vector<int>& getSamplers() const { return m_samplers; }
			inline const int& getSamplerID(const unsigned int& index) { return m_samplers[index]; }
			inline const unsigned int& getSamplersSize() const { return m_samplers.size(); }

			inline const std::vector<glm::mat4>& getTranslationMatrices() const { return m_translationMats; }
			inline const std::vector<glm::mat4>& getRotationMatrices() const { return m_rotationMats; }
			inline const std::vector<glm::vec3>& getColors() const { return m_colors; }
			inline glm::vec3& getColor(const unsigned int& index) { return m_colors[index]; }

			inline const std::vector<unsigned int>& getLayout() const { return m_shapes[0]->getLayoutVector(); }
			inline const unsigned int& getLayout(const unsigned int& index) const { return m_shapes[0]->getLayout(index); }
			inline const unsigned int& getLayoutSize() const { return m_shapes[0]->getLayoutSize(); }
	
			inline Light& getLight() { return m_light; }

			inline glm::vec3& getCenter(const unsigned int& index) { return m_shapes[index]->getCenter(); }
			inline glm::vec3& getAngle(const unsigned int& index) { return m_shapes[index]->getAngle(); }

			// --- SET METHODS --- //
			void setCenter(const unsigned int& index, const glm::vec3& new_center) { m_shapes[index]->setCenter(new_center); }
			void setAngle(const unsigned int& index, const glm::vec3& new_angle) { m_shapes[index]->setAngle(new_angle); }
			void setColor(const unsigned int& index, const glm::vec3& new_color) { m_colors[index] = new_color; m_shapes[index]->setDefaultColor(new_color); }
		};


} }


#define NORMAL_MESH_TYPE ::mar::graphics::MeshType::NORMAL
#define CUBEMAPS_MESH_TYPE ::mar::graphics::MeshType::CUBEMAPS
#define OBJECTS_MESH_TYPE ::mar::graphics::MeshType::OBJECTS


#endif // !MAR_ENGINE_MESH_H

