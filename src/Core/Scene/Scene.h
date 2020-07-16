/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SCENE_H
#define SCENE_H

#include "../../mar.h"
#include "../Mesh/MeshCreator.h"

namespace mar {
	namespace graphics {


		enum class SceneType {
			DEFAULT,
			CUBEMAPS,
			OBJECTS
		};

		/*!
			Scene

			Scene contains everything needed to create basic and default scene
			on start of MAREngine.
		*/
		class Scene {
			std::vector<Ref<Shape>> m_shapes;
			std::vector<maths::vec3> m_centers;
			std::vector<maths::vec3> m_angles;
			std::vector<maths::vec3> m_scales;
			std::vector<const char*> m_textures;
			std::vector<const char*> m_obj;

		public:
			Scene() = default;
			Scene(SceneType type) { initializeScene(type); }

			void initializeScene(SceneType type) {
				switch (type) {
				case SceneType::DEFAULT:  
					createDefault();					break;
				case SceneType::CUBEMAPS: 	
					createSecondMesh();					break;	
				case SceneType::OBJECTS:
					createObjects();					break;
				default:
					createDefault();
				}
			}

		private:
			void createDefault();
			void createSecondMesh();
			void createObjects();

		public:
			inline std::vector<Ref<Shape>> getShapes() { return m_shapes; }
			inline std::vector<maths::vec3> getCenters() { return m_centers; }
			inline std::vector<maths::vec3> getAngles() { return m_angles; }
			inline std::vector<maths::vec3> getScales() { return m_scales; }
			inline std::vector<const char*> getTextures() { return m_textures; }
			inline std::vector<const char*> getObjPaths() { return m_obj; }

			inline const uint32_t getShapesNumber() { return m_shapes.size(); }

			inline Ref<Shape>& getShape(const uint32_t& index) { return m_shapes[index]; }
			inline maths::vec3& getCenter(const uint32_t& index) { return m_centers[index]; }
			inline maths::vec3& getAngle(const uint32_t& index) { return m_angles[index]; }
			inline maths::vec3& getScale(const uint32_t& index) { return m_scales[index]; }
			inline const char* getTexture(const uint32_t& index) { return m_textures[index]; }
			inline const char* getObjPath(const uint32_t& index) { return m_obj[index]; }
		};


} }


#define DEFAULT_SCENE ::mar::graphics::SceneType::DEFAULT
#define CUBEMAPS_SCENE ::mar::graphics::SceneType::CUBEMAPS
#define OBJECTS_SCENE ::mar::graphics::SceneType::OBJECTS


#endif // !SCENE_H