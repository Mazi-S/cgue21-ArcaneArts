#type vertex
#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normals;

layout (std140, binding = 0) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
	vec3 u_DirectionalLight_Direction;
	vec3 u_DirectionalLight_Color;
	vec3 u_PointLight_Position;
	vec3 u_PointLight_Color;
	float u_PointLight_Constant;
	float u_PointLight_Linear;
	float u_PointLight_Quadratic;
};

layout (std140, binding = 1) uniform ModelData {
	mat4 u_Transform;
	mat3 u_NormalMatrix;
};

layout (std140, binding = 6) uniform LightSpace
{
	mat4 u_LightSpaceMatrix;
};

out vec3 v_Position;
out vec3 v_Normals;
out vec2 v_TexCoord;

void main() {
	v_Normals = u_NormalMatrix * a_Normals;
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 420 core

// illumination multiplier
uniform float u_Brightness = 1.0;

layout (std140, binding = 0) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
	vec3 u_DirectionalLight_Direction;
	vec3 u_DirectionalLight_Color;
	vec3 u_PointLight_Position;
	vec3 u_PointLight_Color;
	float u_PointLight_Constant;
	float u_PointLight_Linear;
	float u_PointLight_Quadratic;
};

layout (std140, binding = 2) uniform MaterialData {
	vec3 u_Ambient;
	vec3 u_Diffuse;
	vec3 u_Specular;
	float u_Shininess;
};

layout(binding = 0) uniform sampler2DShadow u_ShadowMap;

in vec3 v_Position;
in vec3 v_Normals;

out vec4 color;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcPointLight(vec3 normal, vec3 viewDir); 

void main() {
	// normalize normal
	vec3 normal = normalize(v_Normals);
	vec3 viewDir = normalize(u_CameraPosition - v_Position);

	// ambient
	vec3 result = u_Ambient;

	// directional light
	result += CalcDirLight(normal, viewDir);

	// point light
	result += CalcPointLight(normal, viewDir);

	// final color
	color = vec4(result * u_Brightness, 1.0);
}

vec3 CalcDirLight(vec3 normal, vec3 viewDir) 
{
	vec3 lightDir = u_DirectionalLight_Direction;
	vec3 lightColor = u_DirectionalLight_Color;

	vec3 newLightDir = normalize(-lightDir);

	// diffuse
	float diffuse = clamp(dot(newLightDir, normal), 0, 1);
	vec3 resultDiffuse = u_Diffuse * diffuse;

	// specular
	vec3 reflectDir = normalize(reflect(-newLightDir, normal));
	float specular = pow(max(dot(viewDir, reflectDir), 0), u_Shininess);
	vec3 resultSpecular = u_Specular * specular;

	// result
	return(lightColor  * (resultDiffuse * u_Ambient + resultSpecular));
}

vec3 CalcPointLight(vec3 normal, vec3 viewDir) 
{
	vec3 lightPos = u_PointLight_Position;
	vec3 lightColor = u_PointLight_Color;
	float constant = u_PointLight_Constant;
	float linear = u_PointLight_Linear;
	float quadratic = u_PointLight_Quadratic;

	vec3 lightDir = normalize(lightPos - v_Position);

	// diffuse
	float diffuse = clamp(dot(lightDir, normal), 0, 1);
	vec3 resultDiffuse = u_Diffuse * diffuse;

	// specular
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float specular = pow(max(dot(viewDir, reflectDir), 0), u_Shininess);
	vec3 resultSpecular = u_Specular * specular;

	// calculate attenuation
	float distance = length(lightPos - v_Position);
	float attenuation = 1.0f / (constant + distance * linear + (distance * distance) * quadratic);

	// result
	return(lightColor * attenuation * (resultDiffuse * u_Ambient + resultSpecular));
}