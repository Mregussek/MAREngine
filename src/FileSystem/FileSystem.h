/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_FILE_SYSTEM_H 
#define MAR_ENGINE_FILE_SYSTEM_H

#include "../mar.h"
#include "../Debug/Log.h"
#include "../Layers/MeshLayer.h"
#include "../Renderer/RendererFactory.h"


namespace mar {


	namespace filesystem {

		class Storage {
			static Storage s_instance;

		public:
			Ref<graphics::RendererFactory> factory;
			bool usegui;

		public:
			Storage() = default;

			void setup(const Ref<graphics::RendererFactory>& f, const bool& u) {
				factory = f;
				usegui = u;
			}

			inline static Storage* getInstance() { return &s_instance; }
		};

		class fnc {
			static std::vector<std::string> s_storage;

		public:
			static void updateMarFiles();

			static void saveSceneToFile(const char* path, const std::vector<graphics::Mesh*>& meshes);
			static std::vector<layers::MeshLayer*> loadSceneFromFile(std::string path);
		
			static std::vector<std::string>& getMarFiles() { return s_storage; }
		};
		

} }


#endif // !MAR_ENGINE_FILE_SYSTEM_H
