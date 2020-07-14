/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "mar_includes.h"

/*!
	mar is a main namespace for MAREngine. Below you can find all classes,
	which are written for proper work of MAREngine. The most needed class is Window,
	cause it initializes OpenGL stuff. 
*/
namespace mar {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	struct TexturePaths;

	namespace window {
		/* namespace callbacks in Window.h */

		class Input;
		class Window;
	}

	namespace devices {
		class SerialPortMonitor;
	}

	namespace editor {
		class filesystem;

		struct GUIData;
		class GUI;
	}

	namespace layers {
		class Layer;
		class GUILayer;
		class MeshLayer;
		class CameraLayer;
		class LayerStack;
	}

	namespace graphics {
		struct CameraData;
		class Camera;

		namespace objects {
			class ObjectLoader;
		}

		struct RendererStatistics;
		class Renderer;
		class RendererFactory;
		class RendererOpenGLFactory;

		class Mesh;
		class MeshCreator;
		class Light;
		class Scene;

		class Shape;
		class ShapeManipulator;
		class Cube;
		class Pyramid;
		class Surface;
		class Wall;

		class VertexBuffer;
		class VertexBufferOpenGL;
		class VertexArray;
		class VertexArrayOpenGL;
		class VertexBufferLayout;
		class VertexBufferLayoutOpenGL;
		class ElementBuffer;
		class ElementBufferOpenGL;
		class FragmentBuffer;
		class FragmentBufferOpenGL;
		class Texture;
		class TextureOpenGL;
		class Shader;
		class ShaderOpenGL;
	}

	namespace engine {
		class MAREngine;
	}

#ifndef MAR_ENGINE_SETTINGS
#define MAR_ENGINE_SETTINGS

	namespace settings {
		inline const char name[10] = "MAREngine";
		inline const int width{ 1500 };
		inline const int height{ 900 };
		inline const char portName[10] = "\\\\.\\COM7";
		inline const char* glsl_version = "#version 330";
	}

	namespace constants {
		inline const size_t maxCount = 100000;
		inline const size_t maxVertexCount = maxCount * 50;
		inline const size_t maxIndexCount = maxCount * 50;
		inline const size_t maxObjectsInScene = 32;
	}

	namespace storage {
		inline Ref<graphics::RendererFactory> factory;
		inline bool usegui;
	}

#endif // !MAR_ENGINE_SETTINGS
}