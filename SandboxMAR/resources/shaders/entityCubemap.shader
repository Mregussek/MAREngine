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

layout(location = 0) out vec4 color;

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

vec4 calculateLight(Material passed_material, vec3 passed_color_light);

void main() {
	int index = int(v_shapeIndex);

	vec4 batchColor = texture(u_SeparateColor[index], v_texCoords);
	vec4 lightColor = calculateLight(u_material[0], batchColor.xyz);

	for (int i = 1; i < 15; i++) {
		if (i >= u_materialSize) break;
		lightColor = lightColor + calculateLight(u_material[i], batchColor.xyz);
	}

	color = batchColor * lightColor;
};

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