#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 lightNormal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float shapeIndex;

out vec3 v_Position;
out vec3 v_lightNormal;
out float v_shapeIndex;
out vec3 v_texCoords;

uniform mat4 u_Model;
uniform mat4 u_MVP;
uniform mat4 u_SeparateTransform[32];

void main() {
	// Calculate all transformations
	int index = int(shapeIndex);
	vec4 verter_transformed = u_SeparateTransform[index] * position;
	gl_Position = u_MVP * verter_transformed;

	// Pass values to fragment shader
	v_shapeIndex = shapeIndex;
	v_lightNormal = mat3(u_Model) * lightNormal;
	v_Position = vec4(u_Model * verter_transformed).xyz;
	v_texCoords = position.xyz;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 out_Color;

in vec3 v_Position;
in vec3 v_lightNormal;
in float v_shapeIndex;
in vec3 v_texCoords;

uniform struct Material {
	vec3 lightPos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float shininess;
} u_material[15];

uniform int u_materialSize;
uniform vec3 u_CameraPos;
uniform samplerCube u_SeparateColor[32];

vec4 setProperTexture(float index);
vec4 calculateLight(Material passed_material, vec3 passed_color_light);
vec4 computeAllLights(vec4 batchColor);

void main() {
	vec4 batchColor = setProperTexture(v_shapeIndex);
	vec4 lightColor = computeAllLights(batchColor);

	out_Color = batchColor * lightColor;
};

vec4 setProperTexture(float index) {
	vec4 color;

	if      (index <= 0.0f) color = texture(u_SeparateColor[0], v_texCoords);
	else if (index <= 1.0f) color = texture(u_SeparateColor[1], v_texCoords);
	else if (index <= 2.0f) color = texture(u_SeparateColor[2], v_texCoords);
	else if (index <= 3.0f) color = texture(u_SeparateColor[3], v_texCoords);
	else if (index <= 4.0f) color = texture(u_SeparateColor[4], v_texCoords);
	else if (index <= 5.0f) color = texture(u_SeparateColor[5], v_texCoords);
	else if (index <= 6.0f) color = texture(u_SeparateColor[6], v_texCoords);
	else if (index <= 7.0f) color = texture(u_SeparateColor[7], v_texCoords);
	else if (index <= 8.0f) color = texture(u_SeparateColor[8], v_texCoords);
	else if (index <= 9.0f) color = texture(u_SeparateColor[9], v_texCoords);
	else if (index <= 10.0f) color = texture(u_SeparateColor[10], v_texCoords);
	else if (index <= 11.0f) color = texture(u_SeparateColor[11], v_texCoords);
	else if (index <= 12.0f) color = texture(u_SeparateColor[12], v_texCoords);
	else if (index <= 13.0f) color = texture(u_SeparateColor[13], v_texCoords);
	else if (index <= 14.0f) color = texture(u_SeparateColor[14], v_texCoords);
	else if (index <= 15.0f) color = texture(u_SeparateColor[15], v_texCoords);
	else if (index <= 16.0f) color = texture(u_SeparateColor[16], v_texCoords);
	else if (index <= 17.0f) color = texture(u_SeparateColor[17], v_texCoords);
	else if (index <= 18.0f) color = texture(u_SeparateColor[18], v_texCoords);
	else if (index <= 19.0f) color = texture(u_SeparateColor[19], v_texCoords);
	else if (index <= 20.0f) color = texture(u_SeparateColor[20], v_texCoords);
	else if (index <= 21.0f) color = texture(u_SeparateColor[21], v_texCoords);
	else if (index <= 22.0f) color = texture(u_SeparateColor[22], v_texCoords);
	else if (index <= 23.0f) color = texture(u_SeparateColor[23], v_texCoords);
	else if (index <= 24.0f) color = texture(u_SeparateColor[24], v_texCoords);
	else if (index <= 25.0f) color = texture(u_SeparateColor[25], v_texCoords);
	else if (index <= 26.0f) color = texture(u_SeparateColor[26], v_texCoords);
	else if (index <= 27.0f) color = texture(u_SeparateColor[27], v_texCoords);
	else if (index <= 28.0f) color = texture(u_SeparateColor[28], v_texCoords);
	else if (index <= 29.0f) color = texture(u_SeparateColor[29], v_texCoords);
	else if (index <= 30.0f) color = texture(u_SeparateColor[30], v_texCoords);
	else if (index <= 31.0f) color = texture(u_SeparateColor[31], v_texCoords);
				 
	return color;
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

	for (int i = 1; i < 15; i++) {
		if (i >= u_materialSize) break;
		lightColor = lightColor + calculateLight(u_material[i], batchColor.xyz);
	}

	return lightColor;
}