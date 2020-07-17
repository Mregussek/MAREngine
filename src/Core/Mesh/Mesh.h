/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_H
#define MAR_ENGINE_MESH_H

#include "../../mar.h"
#include "../Renderer/RendererFactory.h"
#include "Texture/Texture.h"
#include "../Light/Light.h"


namespace mar {
    namespace graphics {


		enum class MeshType {
			NORMAL,
			CUBEMAPS,
			OBJECTS,
			ALPHA_CHANNEL
		};

#define NORMAL_MESH_TYPE ::mar::graphics::MeshType::NORMAL
#define CUBEMAPS_MESH_TYPE ::mar::graphics::MeshType::CUBEMAPS
#define OBJECTS_MESH_TYPE ::mar::graphics::MeshType::OBJECTS
#define ALPHA_CHANNEL_MESH_TYPE ::mar::graphics::MeshType::ALPHA_CHANNEL


		class Mesh {
			const char* m_meshName;
			static uint32_t s_existingInstance;
			// --- One needed buffer
			Ref<Texture> m_texture;
			// --- Run-Time data for mesh
			std::vector<Ref<Shape>> m_shapes;
			std::vector<float> m_vertices;
			std::vector<uint32_t> m_indices;
			std::vector<int> m_samplers;
			std::vector<maths::mat4> m_translationMats;
			std::vector<maths::mat4> m_rotationMats;
			std::vector<maths::mat4> m_scaleMats;
			std::vector<maths::vec3> m_colors;
			// --- Light object
			Light m_light;
			// --- State information 
			MeshType m_type;
			float m_availableTextureID{ 1.f };
			float m_availableShapeID{ 0.f };
			int m_indicesMaxValue{ 0 };
			uint32_t m_shapesCount{ 0 };
			bool m_onlyCubeMaps = false;

		public:
			Mesh() = default;
			virtual ~Mesh();

			void create();
			void loadScene(Scene* scene, MeshType type);

			void tryReuseShape(Ref<Shape>& new_shape, const maths::vec3& center, const maths::vec3& angle, const maths::vec3& scale, const char* texture);
			void submitShape(Ref<Shape>& new_shape, const maths::vec3& center, const maths::vec3& angle, const maths::vec3& scale, const char* texture);
			void flushShape(const uint32_t& index);

			void update();

			void clearBuffers();
			void updateBuffers();

			void clearMatrices();

			void pushMatrices(const Ref<Shape>& new_shape);

		private:
			void prepareShape(Ref<Shape>& new_shape);
			void pushShape(const Ref<Shape>& new_shape);
			void pushTexture(Ref<Shape>& new_shape, const char* texture);

			void popShape(const uint32_t& index);
			void PopHelper_newTextureID(const uint32_t& index);
			int32_t PopHelper_newIndicesMaxVal(const uint32_t& index);
			void popMatrices(const uint32_t& index);

			bool canPushShape(const Ref<Shape>& new_shape);

			bool checkEnding(const char* withwhat, const char* what);

		public:
			/// --- GET METHODS --- ///
			inline const MeshType& getMeshType() const { return m_type; }
			inline const char* getMeshName() const { return m_meshName; }

			inline const Ref<Shape>& getShape(const uint32_t& index) const { return m_shapes[index]; }
			inline const uint32_t& getShapesCount() const { return m_shapesCount; }

			inline const Ref<Texture>& getTexture() const { return m_texture; }

			inline const std::vector<float>& getVertices() const { return m_vertices; }
			inline const std::vector<uint32_t>& getIndices() const { return m_indices; }
			inline const std::vector<int>& getSamplers() const { return m_samplers; }
			inline const std::vector<uint32_t>& getLayout() const { return m_shapes[0]->getLayout(); }

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
				case ALPHA_CHANNEL_MESH_TYPE: m_meshName = "AlphaChannel"; break;
				}
			}
		};


} }



#endif // !MAR_ENGINE_MESH_H

