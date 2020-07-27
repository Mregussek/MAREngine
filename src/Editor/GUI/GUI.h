/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GUI_H
#define MAR_ENGINE_GUI_H

#include "../FileSystem/filesystem.h"
#include "../../mar.h"
#include "../../Debug/Log.h"
#include "../../MAREngine.h"
#include "../../Window/Window.h"
#include "../../Core/Mesh/MeshCreator.h"
#include "../../Core/Light/Light.h"
#include "../../Core/Renderer/RendererOpenGL.h"
#include "../../Core/Renderer/Buffers/FrameBuffer/FrameBuffer.h"
#include "../../Core/Renderer/Buffers/FrameBuffer/FrameBufferOpenGL.h"


namespace mar {
	namespace editor {

		struct scene {
			static std::vector<const char*> files;
			static int selected;
		};

		struct texture {
			static std::vector<const char*> files;
			static int selected;
		};
		
		struct GUIData {
			float colors[4];
			maths::mat4 rotation;
			maths::mat4 translate;

			GUIData() = default;
		};

		class GUI {
			// --- Attributes for sliders && input
			maths::vec3 m_sceneTranslation{ 0.0f, 0.0f, 0.0f };
			maths::vec3 m_sceneAngle{ 0.0f, 0.0f, 0.0f };
			float m_sceneColors[4];
			float m_inputCenter[3];
			float m_generalScale{ 1.f };
			char m_inputStr[20];
			// --- Run-time GUI attributes
			std::vector<graphics::Mesh*> m_meshes;
			std::vector<const char*> m_meshesNames;
			graphics::Light* m_light;
			static GUIData s_guiData;
			int m_meshIndex;
			int m_shapeIndex;
			int m_pushMeshIndex;
			// --- Dockspace
			graphics::FrameBuffer<graphics::FrameBufferOpenGL> m_framebuffer;
			static bool s_dockspaceOpen;
			static bool s_fullscreenPersisant;
			bool m_infoWindow{ false };
			bool m_instructionWindow{ false };
			bool m_fileOpenWindow{ false };
			bool m_fileSaveWindow{ false };

		public:
			GUI() = default;

			void initialize(const char* glsl_version);
			void shutdown();

			void prepareNewFrame();
			void display();

			void submit(graphics::Mesh* mesh) { 
				m_meshes.push_back(mesh);
				m_meshesNames.push_back(mesh->getMeshName());
			}

			// --- SET METHODS --- //
			void set(const graphics::FrameBuffer<graphics::FrameBufferOpenGL>& framebuffer) { m_framebuffer = framebuffer; }
			void set(graphics::Light* light) { m_light = light; }

			// --- GET METHODS --- //
			const maths::mat4 getTranslationMatrix() const;
			const maths::mat4 getRotationMatrix() const;
			inline const static GUIData& getGUIData() { return s_guiData; }

		private:
			void Setup_Theme();

			void File_Open();
			void File_Save();

			void Display_ViewPort();
			void Menu_ModifyScene();
			void Menu_ModifyShape();
			void Menu_SelectShape();
			void Menu_PushShapeToScene();
			void Menu_Environment();
			void Menu_Statistics();
			void Menu_Info();
			void Menu_Instruction();

			bool checkCharsEnding(const char* withwhat, const char* what) {
				static int l1;
				static int l2;

				l1 = strlen(withwhat);
				l2 = strlen(what);

				if (l1 > l2)
					return false;

				return std::strcmp(withwhat, what + (l2 - l1)) == 0;
			}

			bool checkCharsStart(const char* withwhat, const char* what) {
				const char* check = strstr(what, withwhat);

				if (what == check) return true;
				else return false;
			}

		};


} }

#endif // !MAR_ENGINE_GUI_H