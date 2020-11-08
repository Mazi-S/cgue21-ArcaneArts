#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normals;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform vec4 u_Color;

out vec4 v_Color;
void main() {
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	v_Color = u_Color;
}

#type fragment
#version 330 core

out vec4 color;
in vec4 v_Color;
void main() {
	color = v_Color;
}