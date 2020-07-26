/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FILE_SYSTEM_H 
#define MAR_ENGINE_FILE_SYSTEM_H

#include "../../mar.h"
#include "../../Debug/Log.h"
#include "../../Layers/MeshLayer.h"
#include "../../Core/Mesh/Mesh.h"
#include "../../Core/Renderer/Buffers/FrameBuffer/FrameBuffer.h"
#include "../../Core/Renderer/Buffers/FrameBuffer/FrameBufferOpenGL.h"


namespace mar {

	namespace editor {

	
		class filesystem {
			// --- Updating files in directories
			static std::vector<std::string> s_marfiles;
			static std::vector<std::string> s_texturefiles;
			// --- Loading scene from file
			static std::vector<graphics::MeshType> s_mesh_type;
			static std::vector<std::vector<Ref<graphics::Shape>>> s_shapes;
			static std::vector<std::vector<maths::vec3>> s_centers;
			static std::vector<std::vector<maths::vec3>> s_angles;
			static std::vector<std::vector<maths::vec3>> s_scales;
			static std::vector<std::vector<maths::vec3>> s_colors;
			static std::vector<std::vector<std::string>> s_textures;
			static std::vector<std::vector<std::string>> s_objs;
			static int s_mesh_count;
			static int s_scene_count;

		public:
			static void updateMarFiles(const char* path);
			static void updateMarTextures(const char* path);

			static void saveSceneToFile(const std::string& path, const std::vector<graphics::Mesh*>& meshes);
			static void loadSceneFromFile(std::string path);
			
			static std::optional<std::vector<layers::MeshLayer*>> assignLoadedLayers();
			static const std::vector<layers::MeshLayer*> loadMesh();

			// --- GET METHODS --- //
			inline static std::vector<std::string>& getMarFiles() { return s_marfiles; }
			inline static std::vector<std::string>& getMarTextures() { return s_texturefiles; }

			inline static int& getMeshCount() { return s_mesh_count; }
			inline static int& getSceneCount() { return s_scene_count; }
			inline static std::vector<graphics::MeshType>& getMeshTypes() { return s_mesh_type; }
			inline static std::vector<std::vector<Ref<graphics::Shape>>>& getShapes() { return s_shapes; }
			inline static std::vector<std::vector<maths::vec3>>& getCenters() { return s_centers; }
			inline static std::vector<std::vector<maths::vec3>>& getAngles() { return s_angles; }
			inline static std::vector<std::vector<maths::vec3>>& getScales() { return s_scales; }
			inline static std::vector<std::vector<maths::vec3>>& getColors() { return s_colors; }
			inline static std::vector<std::vector<std::string>>& getTextures() { return s_textures; }
			inline static std::vector<std::vector<std::string>>& getObjs() { return s_objs; }

			inline static bool shouldLoadMeshes() { return s_mesh_count != -1; }
			inline static bool checkMeshEquality() { return getMeshCount() + 1 != getMeshTypes().size(); }
		};

	}
}						 



#endif // !MAR_ENGINE_FILE_SYSTEM_H
