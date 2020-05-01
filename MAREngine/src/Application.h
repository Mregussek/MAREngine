
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
#include "GUI/GUI.h"
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
			// FRONT M 
			// x    // y   // z
			-0.7f,  -0.5f,  0.5f, // vertex 0
			-0.35f, -0.5f,  0.5f, // vertex 1
			-0.2f,   0.5f,  0.5f, // vertex 2
			-0.55f,  0.5f,  0.5f, // vertex 3

			 0.7f,  -0.5f,  0.5f, // vertex 4
			 0.35f, -0.5f,  0.5f, // vertex 5
			 0.2f,   0.5f,  0.5f, // vertex 6
			 0.55f,  0.5f,  0.5f, // vertex 7

			-0.01f,  0.25f, 0.5f, // vertex 8
			 0.01f,  0.25f, 0.5f, // vertex 9
			-0.01f, -0.15f, 0.5f, // vertex 10
			 0.01f, -0.15f, 0.5f, // vertex 11

			 // BACKWARD M
			-0.7f,  -0.5f,  -0.5f, // vertex 12
			-0.35f, -0.5f,  -0.5f, // vertex 13
			-0.2f,   0.5f,  -0.5f, // vertex 14
			-0.55f,  0.5f,  -0.5f, // vertex 15

			 0.7f,  -0.5f,  -0.5f, // vertex 16
			 0.35f, -0.5f,  -0.5f, // vertex 17
			 0.2f,   0.5f,  -0.5f, // vertex 18
			 0.55f,  0.5f,  -0.5f, // vertex 19

			-0.01f,  0.25f, -0.5f, // vertex 21
			 0.01f,  0.25f, -0.5f, // vertex 22
			-0.01f, -0.15f, -0.5f, // vertex 23
			 0.01f, -0.15f, -0.5f, // vertex 24
	};

	unsigned int indices[] = {
		// Front M indices
		0, 1, 2, // LeftArm first triangle
		2, 3, 0, // LeftArm second triangle

		4, 5, 6, // RightArm first triangle
		4, 6, 7, // RightArm second triangle

		3, 2, 10, // LeftCenterArm first triangle
		2, 8, 10, // LefCentertArm second triangle

		9, 6, 11, // RightCenterArm first triangle
		11, 6, 7, // RightCentertArm second triangle

		8, 9, 11, // Center first
		8, 10, 11, // Center second

		// Backward M indices
		12, 13, 14, // LeftArm first triangle
		14, 15, 12, // LeftArm second triangle

		16, 17, 18, // RightArm first triangle
		16, 18, 19, // RightArm second triangle

		14, 13, 23, // LeftCenterArm first triangle
		14, 21, 23, // LefCentertArm second triangle

		22, 18, 24, // RightCenterArm first triangle
		24, 18, 19, // RightCentertArm second triangle

		21, 22, 24, // Center first
		21, 23, 24, // Center second

		// Other Indices
		0, 12, 3, // Left Arm
		0, 12, 15,

		4, 7, 16, // Right Arm
		4, 16, 19,
	};
	*/ //