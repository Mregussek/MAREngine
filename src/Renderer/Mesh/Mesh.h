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

#define NORMAL_MESH_TYPE ::mar::graphics::MeshType::NORMAL
#define CUBEMAPS_MESH_TYPE ::mar::graphics::MeshType::CUBEMAPS
#define OBJECTS_MESH_TYPE ::mar::graphics::MeshType::OBJECTS


		class Mesh {
			const char* m_meshName;
			static unsigned int s_existingInstance;
			// --- One needed buffer
			Ref<Texture> m_texture;
			// --- Run-Time data for mesh
			std::vector<Ref<Shape>> m_shapes;
			std::vector<float> m_vertices;
			std::vector<unsigned int> m_indices;
			std::vector<int> m_samplers;
			std::vector<maths::mat4> m_translationMats;
			std::vector<maths::mat4> m_rotationMats;
			std::vector<maths::mat4> m_scaleMats;
			std::vector<maths::vec3> m_colors;
			// --- Light object
			Light m_light;
			// --- State information 
			MeshType m_type;
			static float s_availableTextureID;
			float m_availableShapeID{ 0.f };
			int m_indicesMaxValue{ 0 };
			unsigned int m_shapesCount{ 0 };
			bool m_onlyCubeMaps = false;

		public:
			Mesh() = default;
			virtual ~Mesh();

			void create();
			void loadScene(Scene* scene, MeshType type);

			void tryReuseShape(Ref<Shape>& new_shape, const maths::vec3& center, const maths::vec3& angle, const maths::vec3& scale, const char* texture);
			void submitShape(Ref<Shape>& new_shape, const maths::vec3& center, const maths::vec3& angle, const maths::vec3& scale, const char* texture);
			void flushShape(const unsigned int& index);

			void update();

			void clearBuffers();
			void clearMatrices();

			void pushMatrices(const maths::vec3& center, const maths::vec3& angle, const maths::vec3& scale);

		private:
			void prepareShape(Ref<Shape>& new_shape);
			void pushShape(const Ref<Shape>& new_shape);
			void pushTexture(Ref<Shape>& new_shape, const char* texture);

			void popShape(const unsigned int& index);
			void popMatrices(const unsigned int& index);

			bool canPushShape(const Ref<Shape>& new_shape) {
				unsigned int currentVerticesSize = m_vertices.size() + new_shape->getVertices().size();
				unsigned int currentIndicesSize = m_indices.size() + new_shape->getIndices().size();

				if (currentVerticesSize >= constants::maxVertexCount) {
					MAR_CORE_ERROR("To much vertices in vector!");
					return false;
				}

				if (currentIndicesSize >= constants::maxIndexCount) {
					MAR_CORE_ERROR("To much indices in vector!");
					return false;
				}

				if (m_shapesCount == constants::maxObjectsInScene - 1) {
					MAR_CORE_ERROR("Cannot push more objects!");
					return false;
				}

				return true;
			}

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
			inline const char* getMeshName() const { return m_meshName; }

			inline const Ref<Shape>& getShape(const unsigned int& index) const { return m_shapes[index]; }
			inline const unsigned int& getShapesCount() const { return m_shapesCount; }

			inline const Ref<Texture>& getTexture() const { return m_texture; }

			inline const std::vector<float>& getVertices() const { return m_vertices; }
			inline const std::vector<unsigned int>& getIndices() const { return m_indices; }
			inline const std::vector<int>& getSamplers() const { return m_samplers; }
			inline const std::vector<unsigned int>& getLayout() const { return m_shapes[0]->getLayout(); }

			inline const std::vector<maths::mat4>& getTranslationMatrices() const { return m_translationMats; }
			inline const std::vector<maths::mat4>& getRotationMatrices() const { return m_rotationMats; }
			inline const std::vector<maths::mat4>& getScaleMatrices() const { return m_scaleMats; }
			inline const std::vector<maths::vec3>& getColors() const { return m_colors; }

			inline Light& getLight() { return m_light; }

			// --- SET METHODS --- //
			void setMeshType(const MeshType type) {
				m_type = type;

				switch (type) {
				case NORMAL_MESH_TYPE: m_meshName = "Normal"; break;
				case CUBEMAPS_MESH_TYPE: m_meshName = "Cubemap"; break;
				case OBJECTS_MESH_TYPE: m_meshName = "Objects"; break;
				}
			}
		};


} }



#endif // !MAR_ENGINE_MESH_H

