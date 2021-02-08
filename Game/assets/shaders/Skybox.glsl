#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normals;

layout (std140) uniform SceneData {
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

uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

out vec3 v_Position;
out vec3 v_Normals;
out vec3 v_TexCoord;

void main() {
	v_Normals = u_NormalMatrix * a_Normals;
	v_TexCoord = a_Position;
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout (std140) uniform SceneData {
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

layout (std140) uniform MaterialData {
	vec3 u_Ambient;
	vec3 u_Diffuse;
	vec3 u_Specular;
	float u_Shininess;
};

uniform samplerCube u_Texture;

in vec3 v_Position;
in vec3 v_Normals;
in vec3 v_TexCoord;

out vec4 color;

void main() {
	vec3 a = v_Normals * 0.00000001f;
    color = texture(u_Texture, v_TexCoord + a);
}