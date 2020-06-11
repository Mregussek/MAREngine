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
			SECOND_MESH,
			EMPTY
		};

		//! Scene
		/*!
			Scene contains everything needed to create basic and default scene
			on start of MAREngine.
		*/
		class Scene {
			std::vector<Ref<Shape>> m_shapes;
			std::vector<glm::vec3> m_centers;
			std::vector<glm::vec3> m_angles;
			std::vector<std::string> m_textures;

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
				case SceneType::SECOND_MESH: 	
					createOnlyColouredElements();		break;	
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
			void createOnlyColouredElements();
			void createEmpty();

		public:
			std::vector<Ref<Shape>> getShapes() { return m_shapes; }
			std::vector<glm::vec3> getCenters() { return m_centers; }
			std::vector<glm::vec3> getAngles() { return m_angles; }
			std::vector<std::string> getTextures() { return m_textures; }

			const unsigned int getShapesNumber() { return m_shapes.size(); }

			Ref<Shape>& getShape(unsigned int index) { return m_shapes[index]; }
			glm::vec3& getCenter(unsigned int index) { return m_centers[index]; }
			glm::vec3& getAngle(unsigned int index) { return m_angles[index]; }
			std::string& getTexture(unsigned int index) { return m_textures[index]; }
		};


} }

#endif // !SCENE_H