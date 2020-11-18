
#version 450

layout(location = 1) in vec3 v_lightNormal;
layout(location = 2) in vec2 v_texCoords2D;
layout(location = 3) in float v_shapeIndex;
layout(location = 4) in vec3 v_Position;
layout(location = 5) in vec3 v_texCoords3D;
layout(location = 6) in float v_samplerType;
layout(location = 7) in vec3 v_CameraPos;

layout(location = 0) out vec4 outColor;

struct LightMaterial {
	float posX, posY, posZ;						// 3 * 4 = 12		offset = 0
	float ambientX, ambientY, ambientZ;			// 3 * 4 = 12		offset = 12
	float diffuseX, diffuseY, diffuseZ;			// 3 * 4 = 12		offset = 24
	float specularX, specularY, specularZ;		// 3 * 4 = 12		offset = 36

	float constant;								// 1 * 4 = 4		offset = 40
	float linear;								// 1 * 4 = 4		offset = 44
	float quadratic;							// 1 * 4 = 4		offset = 48
	float shininess;							// 1 * 4 = 4		offset = 52
}; 

layout(std430, binding = 2) buffer Material {
	LightMaterial u_material[32];

	int materialSize;
} material;

layout(std430, binding = 3) buffer TextureSamplers {
	vec3 u_Color[32];
} samplers;					

layout(binding = 4) uniform sampler2D u_2D[32];

vec4 setProperColor(float index);
vec4 setProperTexture2D(float index);
vec4 setProperTextureCubemap(float index);
vec4 computeAllLights(vec4 batchColor);

void main() {
	vec4 batchColor;

	if(v_samplerType <= 0.0f) {
		batchColor = setProperColor(v_shapeIndex);
	}
	else if(v_samplerType <= 1.0f)	{
		batchColor = vec4(0.f, 1.f, 1.0f, 1.0f);
		//batchColor = setProperTexture2D(v_shapeIndex);
	}	
	else if(v_samplerType <= 2.0f)	{
		batchColor = vec4(1.f, 1.f, 0.f, 1.0f);
		//batchColor = setProperTextureCubemap(v_shapeIndex);
	}
	else {
		batchColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	}

	vec4 lightColor = computeAllLights(batchColor);

	outColor = batchColor * lightColor;
}

vec4 setProperColor(float index) {
	if      (index <= 0.5f)  return vec4(samplers.u_Color[0], 1.0f);
	else if (index <= 1.5f)  return vec4(samplers.u_Color[1], 1.0f);
	else if (index <= 2.5f)  return vec4(samplers.u_Color[2], 1.0f);
	else if (index <= 3.5f)  return vec4(samplers.u_Color[3], 1.0f);
	else if (index <= 4.5f)  return vec4(samplers.u_Color[4], 1.0f);
	else if (index <= 5.5f)  return vec4(samplers.u_Color[5], 1.0f);
	else if (index <= 6.5f)  return vec4(samplers.u_Color[6], 1.0f);
	else if (index <= 7.5f)  return vec4(samplers.u_Color[7], 1.0f);
	else if (index <= 8.5f)  return vec4(samplers.u_Color[8], 1.0f);
	else if (index <= 9.5f)  return vec4(samplers.u_Color[9], 1.0f);
	else if (index <= 10.5f) return vec4(samplers.u_Color[10], 1.0f);
	else if (index <= 11.5f) return vec4(samplers.u_Color[11], 1.0f);
	else if (index <= 12.5f) return vec4(samplers.u_Color[12], 1.0f);
	else if (index <= 13.5f) return vec4(samplers.u_Color[13], 1.0f);
	else if (index <= 14.5f) return vec4(samplers.u_Color[14], 1.0f);
	else if (index <= 15.5f) return vec4(samplers.u_Color[15], 1.0f);
	else if (index <= 16.5f) return vec4(samplers.u_Color[16], 1.0f);
	else if (index <= 17.5f) return vec4(samplers.u_Color[17], 1.0f);
	else if (index <= 18.5f) return vec4(samplers.u_Color[18], 1.0f);
	else if (index <= 19.5f) return vec4(samplers.u_Color[19], 1.0f);
	else if (index <= 20.5f) return vec4(samplers.u_Color[20], 1.0f);
	else if (index <= 21.5f) return vec4(samplers.u_Color[21], 1.0f);
	else if (index <= 22.5f) return vec4(samplers.u_Color[22], 1.0f);
	else if (index <= 23.5f) return vec4(samplers.u_Color[23], 1.0f);
	else if (index <= 24.5f) return vec4(samplers.u_Color[24], 1.0f);
	else if (index <= 25.5f) return vec4(samplers.u_Color[25], 1.0f);
	else if (index <= 26.5f) return vec4(samplers.u_Color[26], 1.0f);
	else if (index <= 27.5f) return vec4(samplers.u_Color[27], 1.0f);
	else if (index <= 28.5f) return vec4(samplers.u_Color[28], 1.0f);
	else if (index <= 29.5f) return vec4(samplers.u_Color[29], 1.0f);
	else if (index <= 30.5f) return vec4(samplers.u_Color[30], 1.0f);
	else if (index <= 31.5f) return vec4(samplers.u_Color[31], 1.0f);
	else return vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

vec4 setProperTexture2D(float index) {
	if      (index <= 0.5f)  return texture(u_2D[0], v_texCoords2D);
	else if (index <= 1.5f)  return texture(u_2D[1], v_texCoords2D);
	else if (index <= 2.5f)  return texture(u_2D[2], v_texCoords2D);
	else if (index <= 3.5f)  return texture(u_2D[3], v_texCoords2D);
	else if (index <= 4.5f)  return texture(u_2D[4], v_texCoords2D);
	else if (index <= 5.5f)  return texture(u_2D[5], v_texCoords2D);
	else if (index <= 6.5f)  return texture(u_2D[6], v_texCoords2D);
	else if (index <= 7.5f)  return texture(u_2D[7], v_texCoords2D);
	else if (index <= 8.5f)  return texture(u_2D[8], v_texCoords2D);
	else if (index <= 9.5f)  return texture(u_2D[9], v_texCoords2D);
	else if (index <= 10.5f) return texture(u_2D[10], v_texCoords2D);
	else if (index <= 11.5f) return texture(u_2D[11], v_texCoords2D);
	else if (index <= 12.5f) return texture(u_2D[12], v_texCoords2D);
	else if (index <= 13.5f) return texture(u_2D[13], v_texCoords2D);
	else if (index <= 14.5f) return texture(u_2D[14], v_texCoords2D);
	else if (index <= 15.5f) return texture(u_2D[15], v_texCoords2D);
	else if (index <= 16.5f) return texture(u_2D[16], v_texCoords2D);
	else if (index <= 17.5f) return texture(u_2D[17], v_texCoords2D);
	else if (index <= 18.5f) return texture(u_2D[18], v_texCoords2D);
	else if (index <= 19.5f) return texture(u_2D[19], v_texCoords2D);
	else if (index <= 20.5f) return texture(u_2D[20], v_texCoords2D);
	else if (index <= 21.5f) return texture(u_2D[21], v_texCoords2D);
	else if (index <= 22.5f) return texture(u_2D[22], v_texCoords2D);
	else if (index <= 23.5f) return texture(u_2D[23], v_texCoords2D);
	else if (index <= 24.5f) return texture(u_2D[24], v_texCoords2D);
	else if (index <= 25.5f) return texture(u_2D[25], v_texCoords2D);
	else if (index <= 26.5f) return texture(u_2D[26], v_texCoords2D);
	else if (index <= 27.5f) return texture(u_2D[27], v_texCoords2D);
	else if (index <= 28.5f) return texture(u_2D[28], v_texCoords2D);
	else if (index <= 29.5f) return texture(u_2D[29], v_texCoords2D);
	else if (index <= 30.5f) return texture(u_2D[30], v_texCoords2D);
	else if (index <= 31.5f) return texture(u_2D[31], v_texCoords2D);
	else return vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

vec4 calculateLight(LightMaterial lightMaterial, vec3 batchedColor) {
	vec3 position = vec3(lightMaterial.posX, lightMaterial.posY, lightMaterial.posZ);

	// AMBIENT
	vec3 ambientResult = batchedColor * vec3(lightMaterial.ambientX, lightMaterial.ambientY, lightMaterial.ambientZ);

	// DIFFUSE
	vec3 norm = normalize(v_lightNormal);
	vec3 lightDir = normalize(position - v_Position);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuseResult = batchedColor * (diff * vec3(lightMaterial.diffuseX, lightMaterial.diffuseY, lightMaterial.diffuseZ));

	// SPECULAR
	vec3 viewDir = normalize(v_CameraPos - v_Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), lightMaterial.shininess);
	vec3 specularResult = batchedColor * (spec * vec3(lightMaterial.specularX, lightMaterial.specularY, lightMaterial.specularZ));

	// ATTENUATION
	float distance = length(position - v_Position);
	float attenuation = 1.0f / (lightMaterial.constant + lightMaterial.linear * distance + lightMaterial.quadratic * (distance * distance));
		
	ambientResult *= attenuation;
	diffuseResult *= attenuation;
	specularResult *= attenuation;

	return vec4(ambientResult + diffuseResult + specularResult, 1.0f);
}

vec4 computeAllLights(vec4 batchColor) {
	vec4 lightColor = calculateLight(material.u_material[0], batchColor.xyz);

	for (int i = 1; i < 32; i++) {
		if (i >= material.materialSize) break;
		vec4 lightColor = calculateLight(material.u_material[i], batchColor.xyz);
	}

	return lightColor;
}