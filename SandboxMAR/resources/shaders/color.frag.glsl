
#version 450

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_lightNormal;
layout(location = 2) in vec2 v_texCoords2D;
layout(location = 4) in flat int v_shapeIndex;

layout(location = 0) out vec4 outColor;

struct LightMaterial {
	vec4 position;								// 4 * 4 = 16		offset = 0
	vec4 ambient;								// 4 * 4 = 16		offset = 16
	vec4 diffuse;								// 4 * 4 = 16		offset = 32
	vec4 specular;								// 4 * 4 = 16		offset = 48

	float constant;								// 1 * 4 = 4		offset = 52
	float linear;								// 1 * 4 = 4		offset = 56
	float quadratic;							// 1 * 4 = 4		offset = 60
	float shininess;							// 1 * 4 = 4		offset = 64
}; 

layout(std430, binding = 2) buffer Material {
	LightMaterial u_material[32];
	int u_lightSize;
} material;

layout(std430, binding = 3) buffer TextureSamplers {
	vec4 u_Color[32];
} samplers;

vec4 setProperColor(int index);
vec4 computeAllLights(vec4 batchColor);

void main() {
	vec4 batchColor = setProperColor(v_shapeIndex);
	vec4 lightColor = computeAllLights(batchColor);

	outColor = batchColor * lightColor;
}

vec4 setProperColor(int index) {
	if(index <= 31) { 
		return samplers.u_Color[index];
	} 
	else {
		return vec4(0.5f, 0.5f, 0.5f, 1.0f);
	}
}

vec4 calculateLight(LightMaterial lightMaterial, vec3 batchedColor) {
	vec3 position = lightMaterial.position.xyz;
	vec3 ambient = lightMaterial.ambient.xyz;
	vec3 diffuse = lightMaterial.diffuse.xyz;

	// AMBIENT
	vec3 ambientResult = batchedColor * ambient;

	// DIFFUSE
	vec3 norm = normalize(v_lightNormal);
	vec3 lightDir = normalize(position - v_Position);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuseResult = batchedColor * (diff * diffuse);

	// ATTENUATION
	float distance = length(position - v_Position);
	float attenuation = 1.0f / (lightMaterial.constant + lightMaterial.linear * distance + lightMaterial.quadratic * (distance * distance));
		
	ambientResult *= attenuation;
	diffuseResult *= attenuation;

	return vec4(ambientResult + diffuseResult, 1.0f);
}

vec4 computeAllLights(vec4 batchColor) {
	vec4 lightColor = calculateLight(material.u_material[0], batchColor.xyz);

	for (int i = 1; i < 32; i++) {
		if (i >= material.u_lightSize) break;
		lightColor += calculateLight(material.u_material[i], batchColor.xyz);
	}

	return lightColor;
}
