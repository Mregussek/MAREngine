/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FILE_SYSTEM_H 
#define MAR_ENGINE_FILE_SYSTEM_H

#include "../mar.h"
#include "../Debug/Log.h"
#include "../Layers/MeshLayer.h"


namespace mar {
	namespace filesystem {

	
		class fnc {
			static std::vector<std::string> s_marfiles;
			static std::vector<std::string> s_texturefiles;
			static int s_mesh_count;
			static int s_scene_count;
			static std::vector<graphics::SceneType> s_scene_type;
			static std::vector<graphics::MeshType> s_mesh_type;
			static std::vector<std::vector<Ref<graphics::Shape>>> s_shapes;
			static std::vector<std::vector<glm::vec3>> s_centers;
			static std::vector<std::vector<glm::vec3>> s_angles;
			static std::vector<std::vector<glm::vec3>> s_scales;
			static std::vector<std::vector<glm::vec3>> s_colors;
			static std::vector<std::vector<std::string>> s_textures;
			static std::vector<std::vector<std::string>> s_objs;

		public:
			static void updateMarFiles(const char* path);
			static void updateMarTextures(const char* path);

			static void saveSceneToFile(const char* path, const std::vector<graphics::Mesh*>& meshes);
			static void loadSceneFromFile(std::string path);
			
			// --- GET METHODS --- //
			static std::vector<std::string>& getMarFiles() { return s_marfiles; }
			static std::vector<std::string>& getMarTextures() { return s_texturefiles; }

			inline static int& getMeshCount() { return s_mesh_count; }
			inline static int& getSceneCount() { return s_scene_count; }
			inline static std::vector<graphics::SceneType>& getSceneTypes() { return s_scene_type; }
			inline static std::vector<graphics::MeshType>& getMeshTypes() { return s_mesh_type; }
			inline static std::vector<std::vector<Ref<graphics::Shape>>>& getShapes() { return s_shapes; }
			inline static std::vector<std::vector<glm::vec3>>& getCenters() { return s_centers; }
			inline static std::vector<std::vector<glm::vec3>>& getAngles() { return s_angles; }
			inline static std::vector<std::vector<glm::vec3>>& getScales() { return s_scales; }
			inline static std::vector<std::vector<glm::vec3>>& getColors() { return s_colors; }
			inline static std::vector<std::vector<std::string>>& getTextures() { return s_textures; }
			inline static std::vector<std::vector<std::string>>& getObjs() { return s_objs; }

			static bool shouldLoadMeshes() { return s_mesh_count != -1; }
			static bool shouldLoadScene() { return s_scene_count != -1; }
		};

	}
}						 


#endif // !MAR_ENGINE_FILE_SYSTEM_H
