/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_H
#define MAR_ENGINE_MESH_H

#include "../../mar.h"
#include "../RendererFactory.h"
#include "Texture/Texture.h"
#include "../Light/Light.h"


namespace mar {
    namespace graphics {


		enum class MeshType {
			NORMAL,
			CUBEMAPS,
			OBJECTS
		};


		class Mesh {
			// --- One needed buffer
			Ref<Texture> m_texture;
			// --- Run-Time data for mesh
			std::vector<Ref<Shape>> m_shapes;
			std::vector<float> m_vertices;
			std::vector<unsigned int> m_indices;
			std::vector<int> m_samplers;
			std::vector<glm::mat4> m_translationMats;
			std::vector<glm::mat4> m_rotationMats;
			std::vector<glm::vec3> m_colors;
			// --- Light object
			Light m_light;
			// --- State information 
			MeshType m_type;
			static float s_availableTextureID;
			float m_availableShapeID;
			int m_indicesMaxValue;
			unsigned int m_shapesCount;
			bool m_onlyCubeMaps = false;

		public:
			Mesh() = default;
			virtual ~Mesh();

			void createMesh(const Ref<RendererFactory>& factory);
			void loadScene(Scene* scene, MeshType type);

			void tryReuseShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const char* texture);
			void submitShape(Ref<Shape>& new_shape, const glm::vec3& center, const glm::vec3& angle, const char* texture);
			void flushShape(const unsigned int& index);

			void update();

			void clearBuffers();
			void clearMatrices();

			void pushMatrices(const glm::vec3& center, const glm::vec3& angle);

		private:
			void pushShape(Ref<Shape>& new_shape);
			void pushTexture(Ref<Shape>& new_shape, const char* texture);

			void popShape(const unsigned int& index);
			void popMatrices(const unsigned int& index);

			bool checkIfCubemap(const char* withwhat, const char* what) {
				int l1 = strlen(withwhat);
				int l2 = strlen(what);

				if (l1 > l2)
					return false;

				return std::strcmp(withwhat, what + (l2 - l1)) == 0;
			}

		public:
			/// --- GET METHODS --- ///
			inline const MeshType& getMeshType() const { return m_type; }

			inline const Ref<Shape>& getShape(const unsigned int& index) const { return m_shapes[index]; }
			inline const unsigned int& getShapesCount() const { return m_shapesCount; }

			inline const Ref<Texture>& getTexture() const { return m_texture; }

			inline const std::vector<float>& getVertices() const { return m_vertices; }
			inline const std::vector<unsigned int>& getIndices() const { return m_indices; }
			inline const std::vector<int>& getSamplers() const { return m_samplers; }
			inline const std::vector<unsigned int>& getLayout() const { return m_shapes[0]->getLayoutVector(); }

			inline const std::vector<glm::mat4>& getTranslationMatrices() const { return m_translationMats; }
			inline const std::vector<glm::mat4>& getRotationMatrices() const { return m_rotationMats; }
			inline const std::vector<glm::vec3>& getColors() const { return m_colors; }

			inline Light& getLight() { return m_light; }

			// --- SET METHODS --- //
			void setType(const MeshType type) { m_type = type; }
		};


} }


#define NORMAL_MESH_TYPE ::mar::graphics::MeshType::NORMAL
#define CUBEMAPS_MESH_TYPE ::mar::graphics::MeshType::CUBEMAPS
#define OBJECTS_MESH_TYPE ::mar::graphics::MeshType::OBJECTS


#endif // !MAR_ENGINE_MESH_H

