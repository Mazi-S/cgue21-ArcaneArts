#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normals;

layout (std140) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
};

uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

out vec3 v_Position;
out vec3 v_Normals;
out vec2 v_TexCoord;

void main() {
	v_Normals = u_NormalMatrix * a_Normals;
	v_TexCoord = a_TexCoord;
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout (std140) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
};

layout (std140) uniform MaterialData {
	vec3 u_Ambient;
	vec3 u_Diffuse;
	vec3 u_Specular;
	float u_Shininess;
};

in vec3 v_Position;
in vec3 v_Normals;
in vec2 v_TexCoord;

out vec4 color;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcPointLight(vec3 normal, vec3 viewDir);

void main() {
	// normalize normal
	vec3 normal = normalize(v_Normals);
	vec3 viewDir = normalize(u_CameraPosition - v_Position);

	// ambient
	vec3 resultAmbient = u_Ambient * vec3(1.0f, 1.0f, 1.0f);

	vec3 result = resultAmbient;

	// directional light
    result += CalcDirLight(normal, viewDir);

	// point light
    result += CalcPointLight(normal, viewDir);
		
	// final color
	color = vec4(result, 1.0f);
}

vec3 CalcDirLight(vec3 normal, vec3 viewDir) 
{
	// hardcoded stuff
	vec3 lightDir = vec3(0.0f, -1.0f, -1.0f);
	vec3 lightColor = vec3(0.8f, 0.8f, 0.8f);

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
	// hardcoded stuff
	vec3 lightPos = vec3(0.0f, 0.0f, 0.0f);;
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	float constant = 1.0f;
	float linear = 0.4f;
	float quadratic = 0.1f;

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