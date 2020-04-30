
// Vertices
#include "VertexBuffer/VertexBuffer.h"
#include "VertexBuffer/VertexBufferLayout.h"
#include "ElementBuffer/ElementBuffer.h"
#include "VertexArray/VertexArray.h"
// Rendering
#include "Shader/Shader.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"
#include "Camera/Camera.h"
// Window && Input
#include "Window/Window.h"
#include "Devices/SerialPortMonitor.h"

namespace mar {

	class Application {
	public:
		Application() = default;

		// --- MAIN METHOD --- //
		int run();

	private:
		void rgbColorsChange(float& r, float& g, float& b, float& rc, float& gc, float& bc);
	};

}

/* ----------------------- For M letter positions and indices!!!
	float positions[] = {
			// x    // y   // z
			-0.7f,  -0.5f,  0.0f, // vertex 0
			-0.35f, -0.5f,  0.0f, // vertex 1
			-0.2f,   0.5f,  0.0f, // vertex 2
			-0.55f,  0.5f,  0.0f, // vertex 3

			 0.7f,  -0.5f,  0.0f, // vertex 4
			 0.35f, -0.5f,  0.0f, // vertex 5
			 0.2f,   0.5f,  0.0f, // vertex 6
			 0.55f,  0.5f,  0.0f, // vertex 7

			-0.01f,  0.25f, 0.0f, // vertex 8
			 0.01f,  0.25f, 0.0f, // vertex 9
			-0.01f, -0.15f, 0.0f, // vertex 10
			 0.01f, -0.15f, 0.0f  // vertex 11
	};

	unsigned int indices[] = {
		0, 1, 2, // LeftArm first triangle
		2, 3, 0, // LeftArm second triangle

		4, 5, 6, // RightArm first triangle
		4, 6, 7, // RightArm second triangle

		3, 2, 10, // LeftCenterArm first triangle
		2, 8, 10, // LefCentertArm second triangle

		9, 6, 11, // RightCenterArm first triangle
		11, 6, 7, // RightCentertArm second triangle

		8, 9, 11, // Center first
		8, 10, 11 // Center second
	};
*/