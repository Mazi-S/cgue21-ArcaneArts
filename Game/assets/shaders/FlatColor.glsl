#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normals;

layout (std140) uniform SceneData {
	mat4 u_ViewProjection;
};

uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

out vec3 v_Normals;
out vec2 v_TexCoord;

void main() {
	v_Normals = u_NormalMatrix * a_Normals;
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout (std140) uniform MaterialData {
	vec3 u_Ambient;
	vec3 u_Diffuse;
	vec3 u_Specular;
	float u_Shininess;
};

in vec3 v_Normals;
in vec2 v_TexCoord;

out vec4 color;

void main() {
	vec3 n = normalize(v_Normals);
	
	color = vec4(u_Ambient, 1.0) + n.x * 0.01;
}