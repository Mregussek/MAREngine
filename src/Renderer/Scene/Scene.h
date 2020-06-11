/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SCENE_H
#define SCENE_H

#include "../../mar.h"
#include "../RendererFactory.h"

namespace mar {
	namespace graphics {


		enum class SceneType {
			DEFAULT,
			SURFACE,
			WITH_COLOURED_ELEMS,
			CUBEMAPS,
			EMPTY
		};

		/*!
			Scene

			Scene contains everything needed to create basic and default scene
			on start of MAREngine.
		*/
		class Scene {
			std::vector<Ref<Shape>> m_shapes;
			std::vector<glm::vec3> m_centers;
			std::vector<glm::vec3> m_angles;
			std::vector<std::string> m_textures;
			std::vector<std::vector<std::string>> m_faces;

		public:
			Scene() = default;
			Scene(SceneType type = SceneType::DEFAULT) { initializeScene(type); }

			void initializeScene(SceneType type) {
				switch (type) {
				case SceneType::DEFAULT:  
					createDefault();					break;
				case SceneType::SURFACE: 
					createSurface();					break;
				case SceneType::WITH_COLOURED_ELEMS: 
					createWithColouredElements();		break;	
				case SceneType::CUBEMAPS: 	
					createSecondMesh();					break;	
				case SceneType::EMPTY: 
					createEmpty();						break;
				default:
					createDefault();
				}
			}

		private:
			void createDefault();
			void createSurface();
			void createWithColouredElements();
			void createSecondMesh();
			void createEmpty();

		public:
			inline std::vector<Ref<Shape>> getShapes() { return m_shapes; }
			inline std::vector<glm::vec3> getCenters() { return m_centers; }
			inline std::vector<glm::vec3> getAngles() { return m_angles; }
			inline std::vector<std::string> getTextures() { return m_textures; }
			inline std::vector<std::vector<std::string>> getFaces() { return m_faces; }

			inline const unsigned int getShapesNumber() { return m_shapes.size(); }

			inline Ref<Shape>& getShape(const unsigned int& index) { return m_shapes[index]; }
			inline glm::vec3& getCenter(const unsigned int& index) { return m_centers[index]; }
			inline glm::vec3& getAngle(const unsigned int& index) { return m_angles[index]; }
			inline std::string& getTexture(const unsigned int& index) { return m_textures[index]; }
			inline std::vector<std::string> getFace(const unsigned int& index) { return m_faces[index]; }
		};


} }

#endif // !SCENE_H