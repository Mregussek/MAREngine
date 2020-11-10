
#version 450

in vec3 v_Position;
in vec3 v_lightNormal;
in float v_shapeIndex;
in vec2 v_texCoords2D;
in vec3 v_texCoords3D;
in float v_samplerType;

out vec4 outColor;

uniform struct Material {
	vec3 lightPos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float shininess;
} u_material[32];

uniform int u_materialSize;
uniform vec3 u_CameraPos;
uniform vec3 u_SamplersColor[32];
uniform sampler2D u_Samplers2D[32];
uniform samplerCube u_SamplersCube[32];

vec4 setProperColor(float index);
vec4 setProperTexture2D(float index);
vec4 setProperTextureCubemap(float index);
vec4 calculateLight(Material passed_material, vec3 passed_color_light);
vec4 computeAllLights(vec4 batchColor);

void main() {
	vec4 batchColor;

	if(v_samplerType <= 0.0f)
		batchColor = setProperColor(v_shapeIndex);
	else if(v_samplerType <= 1.0f)	
		batchColor = setProperTexture2D(v_shapeIndex);
	else if(v_samplerType <= 2.0f)	
		batchColor = setProperTextureCubemap(v_shapeIndex);
	else 
		batchColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);

	vec4 lightColor = computeAllLights(batchColor);

	outColor = batchColor * lightColor;
};

vec4 setProperColor(float index) {
	if      (index <= 0.5f)  return vec4(u_SamplersColor[0], 1.0f);
	else if (index <= 1.5f)  return vec4(u_SamplersColor[1], 1.0f);
	else if (index <= 2.5f)  return vec4(u_SamplersColor[2], 1.0f);
	else if (index <= 3.5f)  return vec4(u_SamplersColor[3], 1.0f);
	else if (index <= 4.5f)  return vec4(u_SamplersColor[4], 1.0f);
	else if (index <= 5.5f)  return vec4(u_SamplersColor[5], 1.0f);
	else if (index <= 6.5f)  return vec4(u_SamplersColor[6], 1.0f);
	else if (index <= 7.5f)  return vec4(u_SamplersColor[7], 1.0f);
	else if (index <= 8.5f)  return vec4(u_SamplersColor[8], 1.0f);
	else if (index <= 9.5f)  return vec4(u_SamplersColor[9], 1.0f);
	else if (index <= 10.5f) return vec4(u_SamplersColor[10], 1.0f);
	else if (index <= 11.5f) return vec4(u_SamplersColor[11], 1.0f);
	else if (index <= 12.5f) return vec4(u_SamplersColor[12], 1.0f);
	else if (index <= 13.5f) return vec4(u_SamplersColor[13], 1.0f);
	else if (index <= 14.5f) return vec4(u_SamplersColor[14], 1.0f);
	else if (index <= 15.5f) return vec4(u_SamplersColor[15], 1.0f);
	else if (index <= 16.5f) return vec4(u_SamplersColor[16], 1.0f);
	else if (index <= 17.5f) return vec4(u_SamplersColor[17], 1.0f);
	else if (index <= 18.5f) return vec4(u_SamplersColor[18], 1.0f);
	else if (index <= 19.5f) return vec4(u_SamplersColor[19], 1.0f);
	else if (index <= 20.5f) return vec4(u_SamplersColor[20], 1.0f);
	else if (index <= 21.5f) return vec4(u_SamplersColor[21], 1.0f);
	else if (index <= 22.5f) return vec4(u_SamplersColor[22], 1.0f);
	else if (index <= 23.5f) return vec4(u_SamplersColor[23], 1.0f);
	else if (index <= 24.5f) return vec4(u_SamplersColor[24], 1.0f);
	else if (index <= 25.5f) return vec4(u_SamplersColor[25], 1.0f);
	else if (index <= 26.5f) return vec4(u_SamplersColor[26], 1.0f);
	else if (index <= 27.5f) return vec4(u_SamplersColor[27], 1.0f);
	else if (index <= 28.5f) return vec4(u_SamplersColor[28], 1.0f);
	else if (index <= 29.5f) return vec4(u_SamplersColor[29], 1.0f);
	else if (index <= 30.5f) return vec4(u_SamplersColor[30], 1.0f);
	else if (index <= 31.5f) return vec4(u_SamplersColor[31], 1.0f);
	else return vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

vec4 setProperTexture2D(float index) {
	if      (index <= 0.5f)  return texture(u_Samplers2D[0], v_texCoords2D);
	else if (index <= 1.5f)  return texture(u_Samplers2D[1], v_texCoords2D);
	else if (index <= 2.5f)  return texture(u_Samplers2D[2], v_texCoords2D);
	else if (index <= 3.5f)  return texture(u_Samplers2D[3], v_texCoords2D);
	else if (index <= 4.5f)  return texture(u_Samplers2D[4], v_texCoords2D);
	else if (index <= 5.5f)  return texture(u_Samplers2D[5], v_texCoords2D);
	else if (index <= 6.5f)  return texture(u_Samplers2D[6], v_texCoords2D);
	else if (index <= 7.5f)  return texture(u_Samplers2D[7], v_texCoords2D);
	else if (index <= 8.5f)  return texture(u_Samplers2D[8], v_texCoords2D);
	else if (index <= 9.5f)  return texture(u_Samplers2D[9], v_texCoords2D);
	else if (index <= 10.5f) return texture(u_Samplers2D[10], v_texCoords2D);
	else if (index <= 11.5f) return texture(u_Samplers2D[11], v_texCoords2D);
	else if (index <= 12.5f) return texture(u_Samplers2D[12], v_texCoords2D);
	else if (index <= 13.5f) return texture(u_Samplers2D[13], v_texCoords2D);
	else if (index <= 14.5f) return texture(u_Samplers2D[14], v_texCoords2D);
	else if (index <= 15.5f) return texture(u_Samplers2D[15], v_texCoords2D);
	else if (index <= 16.5f) return texture(u_Samplers2D[16], v_texCoords2D);
	else if (index <= 17.5f) return texture(u_Samplers2D[17], v_texCoords2D);
	else if (index <= 18.5f) return texture(u_Samplers2D[18], v_texCoords2D);
	else if (index <= 19.5f) return texture(u_Samplers2D[19], v_texCoords2D);
	else if (index <= 20.5f) return texture(u_Samplers2D[20], v_texCoords2D);
	else if (index <= 21.5f) return texture(u_Samplers2D[21], v_texCoords2D);
	else if (index <= 22.5f) return texture(u_Samplers2D[22], v_texCoords2D);
	else if (index <= 23.5f) return texture(u_Samplers2D[23], v_texCoords2D);
	else if (index <= 24.5f) return texture(u_Samplers2D[24], v_texCoords2D);
	else if (index <= 25.5f) return texture(u_Samplers2D[25], v_texCoords2D);
	else if (index <= 26.5f) return texture(u_Samplers2D[26], v_texCoords2D);
	else if (index <= 27.5f) return texture(u_Samplers2D[27], v_texCoords2D);
	else if (index <= 28.5f) return texture(u_Samplers2D[28], v_texCoords2D);
	else if (index <= 29.5f) return texture(u_Samplers2D[29], v_texCoords2D);
	else if (index <= 30.5f) return texture(u_Samplers2D[30], v_texCoords2D);
	else if (index <= 31.5f) return texture(u_Samplers2D[31], v_texCoords2D);
	else return vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

vec4 setProperTextureCubemap(float index) {
	if      (index <= 0.5f)  return texture(u_SamplersCube[0], v_texCoords3D);
	else if (index <= 1.5f)  return texture(u_SamplersCube[1], v_texCoords3D);
	else if (index <= 2.5f)  return texture(u_SamplersCube[2], v_texCoords3D);
	else if (index <= 3.5f)  return texture(u_SamplersCube[3], v_texCoords3D);
	else if (index <= 4.5f)  return texture(u_SamplersCube[4], v_texCoords3D);
	else if (index <= 5.5f)  return texture(u_SamplersCube[5], v_texCoords3D);
	else if (index <= 6.5f)  return texture(u_SamplersCube[6], v_texCoords3D);
	else if (index <= 7.5f)  return texture(u_SamplersCube[7], v_texCoords3D);
	else if (index <= 8.5f)  return texture(u_SamplersCube[8], v_texCoords3D);
	else if (index <= 9.5f)  return texture(u_SamplersCube[9], v_texCoords3D);
	else if (index <= 10.5f) return texture(u_SamplersCube[10], v_texCoords3D);
	else if (index <= 11.5f) return texture(u_SamplersCube[11], v_texCoords3D);
	else if (index <= 12.5f) return texture(u_SamplersCube[12], v_texCoords3D);
	else if (index <= 13.5f) return texture(u_SamplersCube[13], v_texCoords3D);
	else if (index <= 14.5f) return texture(u_SamplersCube[14], v_texCoords3D);
	else if (index <= 15.5f) return texture(u_SamplersCube[15], v_texCoords3D);
	else if (index <= 16.5f) return texture(u_SamplersCube[16], v_texCoords3D);
	else if (index <= 17.5f) return texture(u_SamplersCube[17], v_texCoords3D);
	else if (index <= 18.5f) return texture(u_SamplersCube[18], v_texCoords3D);
	else if (index <= 19.5f) return texture(u_SamplersCube[19], v_texCoords3D);
	else if (index <= 20.5f) return texture(u_SamplersCube[20], v_texCoords3D);
	else if (index <= 21.5f) return texture(u_SamplersCube[21], v_texCoords3D);
	else if (index <= 22.5f) return texture(u_SamplersCube[22], v_texCoords3D);
	else if (index <= 23.5f) return texture(u_SamplersCube[23], v_texCoords3D);
	else if (index <= 24.5f) return texture(u_SamplersCube[24], v_texCoords3D);
	else if (index <= 25.5f) return texture(u_SamplersCube[25], v_texCoords3D);
	else if (index <= 26.5f) return texture(u_SamplersCube[26], v_texCoords3D);
	else if (index <= 27.5f) return texture(u_SamplersCube[27], v_texCoords3D);
	else if (index <= 28.5f) return texture(u_SamplersCube[28], v_texCoords3D);
	else if (index <= 29.5f) return texture(u_SamplersCube[29], v_texCoords3D);
	else if (index <= 30.5f) return texture(u_SamplersCube[30], v_texCoords3D);
	else if (index <= 31.5f) return texture(u_SamplersCube[31], v_texCoords3D);
	else return vec4(0.5f, 0.5f, 0.5f, 1.0f);
}		

vec4 calculateLight(Material passed_material, vec3 passed_color_light) {
	// AMBIENT
	vec3 ambient = passed_color_light * passed_material.ambient;

	// DIFFUSE
	vec3 norm = normalize(v_lightNormal);
	vec3 lightDir = normalize(passed_material.lightPos - v_Position);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = passed_color_light * (diff * passed_material.diffuse);

	// SPECULAR
	vec3 viewDir = normalize(u_CameraPos - v_Position);
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
	vec4 lightColor = calculateLight(u_material[0], batchColor.xyz);

	for (int i = 1; i < 32; i++) {
		if (i >= u_materialSize) break;
		lightColor = lightColor + calculateLight(u_material[i], batchColor.xyz);
	}

	return lightColor;
}
