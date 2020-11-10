
#version 450

layout(location = 1) in vec3 v_lightNormal;
layout(location = 2) in vec2 v_texCoords2D;
layout(location = 3) in float v_shapeIndex;
layout(location = 4) in vec3 v_Position;
layout(location = 5) in vec3 v_texCoords3D;
layout(location = 6) in float v_samplerType;
layout(location = 7) in vec3 v_CameraPos;

layout(location = 0) out vec4 outColor;

struct Material {
	vec3 lightPos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float shininess;
};

layout(std140, binding = 2) uniform MaterialInfo {
	Material u_material[32];
} materialInfo;

layout(std140, binding = 3) uniform MeshInfo {
	int u_materialSize;
} info;

layout(std140, binding = 4) uniform TextureSamplers {
	vec3 u_Color[32];
} samplers;					

layout(binding = 5) uniform sampler2D u_2D[32];
//layout(binding = 6) uniform samplerCube u_Cube[32];

vec4 setProperColor(float index);
vec4 setProperTexture2D(float index);
vec4 setProperTextureCubemap(float index);
vec4 calculateLight(Material passed_material, vec3 passed_color_light);
vec4 computeAllLights(vec4 batchColor);

void main() {
	vec4 batchColor;

	if(v_samplerType <= 0.0f) {
		batchColor = setProperColor(v_shapeIndex);
	}
	else if(v_samplerType <= 1.0f)	{
		batchColor = setProperTexture2D(v_shapeIndex);
	}	
	//else if(v_samplerType <= 2.0f)	{
	//	batchColor = setProperTextureCubemap(v_shapeIndex);
	//}
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
/*
vec4 setProperTextureCubemap(float index) {
	if      (index <= 0.5f)  return texture(u_Cube[0], v_texCoords3D);
	else if (index <= 1.5f)  return texture(u_Cube[1], v_texCoords3D);
	else if (index <= 2.5f)  return texture(u_Cube[2], v_texCoords3D);
	else if (index <= 3.5f)  return texture(u_Cube[3], v_texCoords3D);
	else if (index <= 4.5f)  return texture(u_Cube[4], v_texCoords3D);
	else if (index <= 5.5f)  return texture(u_Cube[5], v_texCoords3D);
	else if (index <= 6.5f)  return texture(u_Cube[6], v_texCoords3D);
	else if (index <= 7.5f)  return texture(u_Cube[7], v_texCoords3D);
	else if (index <= 8.5f)  return texture(u_Cube[8], v_texCoords3D);
	else if (index <= 9.5f)  return texture(u_Cube[9], v_texCoords3D);
	else if (index <= 10.5f) return texture(u_Cube[10], v_texCoords3D);
	else if (index <= 11.5f) return texture(u_Cube[11], v_texCoords3D);
	else if (index <= 12.5f) return texture(u_Cube[12], v_texCoords3D);
	else if (index <= 13.5f) return texture(u_Cube[13], v_texCoords3D);
	else if (index <= 14.5f) return texture(u_Cube[14], v_texCoords3D);
	else if (index <= 15.5f) return texture(u_Cube[15], v_texCoords3D);
	else if (index <= 16.5f) return texture(u_Cube[16], v_texCoords3D);
	else if (index <= 17.5f) return texture(u_Cube[17], v_texCoords3D);
	else if (index <= 18.5f) return texture(u_Cube[18], v_texCoords3D);
	else if (index <= 19.5f) return texture(u_Cube[19], v_texCoords3D);
	else if (index <= 20.5f) return texture(u_Cube[20], v_texCoords3D);
	else if (index <= 21.5f) return texture(u_Cube[21], v_texCoords3D);
	else if (index <= 22.5f) return texture(u_Cube[22], v_texCoords3D);
	else if (index <= 23.5f) return texture(u_Cube[23], v_texCoords3D);
	else if (index <= 24.5f) return texture(u_Cube[24], v_texCoords3D);
	else if (index <= 25.5f) return texture(u_Cube[25], v_texCoords3D);
	else if (index <= 26.5f) return texture(u_Cube[26], v_texCoords3D);
	else if (index <= 27.5f) return texture(u_Cube[27], v_texCoords3D);
	else if (index <= 28.5f) return texture(u_Cube[28], v_texCoords3D);
	else if (index <= 29.5f) return texture(u_Cube[29], v_texCoords3D);
	else if (index <= 30.5f) return texture(u_Cube[30], v_texCoords3D);
	else if (index <= 31.5f) return texture(u_Cube[31], v_texCoords3D);
	else return vec4(0.5f, 0.5f, 0.5f, 1.0f);
}		
*/
vec4 calculateLight(Material passed_material, vec3 passed_color_light) {
	// AMBIENT
	vec3 ambient = passed_color_light * passed_material.ambient;

	// DIFFUSE
	vec3 norm = normalize(v_lightNormal);
	vec3 lightDir = normalize(passed_material.lightPos - v_Position);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = passed_color_light * (diff * passed_material.diffuse);

	// SPECULAR
	vec3 viewDir = normalize(v_CameraPos - v_Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), passed_material.shininess);
	vec3 specular = passed_color_light * (spec * passed_material.specular);

	// ATTENUATION
	float distance = length(passed_material.lightPos - v_Position);
	float attenuation = 1.0f / (passed_material.constant + passed_material.linear * distance +
		passed_material.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return(vec4(ambient + diffuse + specular, 1.0f));
}

vec4 computeAllLights(vec4 batchColor) {
	vec4 lightColor = calculateLight(materialInfo.u_material[0], batchColor.xyz);

	for (int i = 1; i < 32; i++) {
		if (i >= info.u_materialSize) break;
		lightColor = lightColor + calculateLight(materialInfo.u_material[i], batchColor.xyz);
	}

	return lightColor;
}
