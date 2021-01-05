
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 lightNormal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float shapeIndex;

layout(location = 0) out vec3 v_Position;
layout(location = 1) out vec3 v_lightNormal;
layout(location = 2) out vec2 v_texCoords2D;
layout(location = 4) out flat int v_shapeIndex;


layout(std430, binding = 0) buffer CameraSSBO {
	mat4 MVP;
} Camera;

layout(std430, binding = 5) buffer TransformSSBO {
	mat4 Transform[32];
} Transforms;

void main() {
	// Calculate all transformations
	int intShapeIndex = int(shapeIndex);
	vec4 vertexComputed = Transforms.Transform[intShapeIndex] * vec4(position, 1.f);
	gl_Position = Camera.MVP * vertexComputed;

	// Pass values to fragment shader
	v_Position = vertexComputed.xyz;
	v_lightNormal = lightNormal;
	v_texCoords2D = texCoord;
	v_shapeIndex = intShapeIndex;
	
}
