#type vertex
#version 330 core

out VertexData 
{
	vec3 v_Position;
	vec2 v_TexCoord;
	vec3 v_Normals;
} vert;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normals;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	vert.v_Position = vec3(u_Transform * vec4(a_Position, 1));
	vert.v_Normals = a_Normals * mat3(transpose(inverse(u_Transform)));
	vert.v_TexCoord = a_TexCoord;
}

#type fragment
#version 330 core

in VertexData 
{
	vec3 v_Position;
	vec2 v_TexCoord; 
	vec3 v_Normals; 
} vert;

out vec4 color;
uniform sampler2D u_Texture;

void main() {
	color = texture(u_Texture, vert.v_TexCoord).rgba;
}