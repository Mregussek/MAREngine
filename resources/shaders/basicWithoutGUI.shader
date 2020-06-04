#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 basicColors;
layout(location = 2) in vec3 lightNormal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in float texIndex;
layout(location = 5) in float shapeIndex;

out vec2 v_TexCoord;
out float v_shapeIndex;
out float v_textureIndex;
out vec3 v_lightNormal;
out vec3 v_Position;
out vec3 v_basicColors;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_SeperateTranslate[32];
uniform mat4 u_SeperateRotation[32];

void main() {
	// Calculate all transformations
	int index = int(shapeIndex);
	mat4 renderTrans = u_SeperateTranslate[index] * u_SeperateRotation[index];

	mat4 mvp = u_Projection * u_View * u_Model;

	gl_Position = mvp * renderTrans * position;

	// Pass values to fragment shader
	v_TexCoord = texCoord;
	v_textureIndex = texIndex;
	v_shapeIndex = shapeIndex;
	v_lightNormal = mat3(u_Model) * lightNormal;
	v_Position = vec4(u_Model * position).xyz;
	v_basicColors = basicColors;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_shapeIndex;
in float v_textureIndex;
in vec3 v_lightNormal;
in vec3 v_Position;
in vec3 v_basicColors;

struct Material {
	vec3 ambientStrength;
	vec3 diffuseStrength;
	vec3 specularStrength;

	vec3 lightPos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float shininess;

	float constant;
	float linear;
	float quadratic;
};

uniform Material u_material;
uniform sampler2D u_Texture[32];
uniform vec3 u_CameraPos;

void main() {
	// Texture Setup
	int index = int(v_shapeIndex);
	vec4 texColor;

	if (v_textureIndex != 0.0f)
		texColor = texture(u_Texture[index], v_TexCoord);
	else
		texColor = vec4(v_basicColors, 1.0f);

	// AMBIENT
	vec3 ambient = u_material.ambientStrength * u_material.ambient;

	// DIFFUSE
	vec3 norm = normalize(v_lightNormal);
	vec3 lightDir = normalize(u_material.lightPos - v_Position);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = u_material.diffuseStrength * (diff * u_material.diffuse);

	// SPECULAR
	vec3 viewDir = normalize(u_CameraPos - v_Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	vec3 specular = u_material.specularStrength * (spec * u_material.specular);

	// ATTENUATION
	float distance = length(u_material.lightPos - v_Position);
	float attenuation = 1.0f / (u_material.constant + u_material.linear * distance +
		u_material.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// FINAL
	vec4 calculatedLight = vec4(ambient + diffuse + specular, 1.0f);

	color = texColor * calculatedLight;
};