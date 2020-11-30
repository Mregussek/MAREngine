
#version 450

struct Vertex {
	float posX, posY, posZ;
	float norX, norY, norZ;
	float texX, texY;
};

layout(binding = 0) buffer Vertices {
	
	Vertex vertices[];

};

layout(location = 0) out vec4 v_Color;

void main() {
	Vertex v = vertices[gl_VertexIndex];
	vec3 position = vec3(v.posX, v.posY, v.posZ);
	vec3 normal = vec3(v.norX, v.norY, v.norZ);
	vec2 texCoord = vec2(v.texX, v.texY);

	gl_Position = vec4(position + vec3(0.f, 0.f, 1.f), 1.f);

	v_Color = vec4(normal * 0.5f + vec3(0.5f), 1.f);
}
