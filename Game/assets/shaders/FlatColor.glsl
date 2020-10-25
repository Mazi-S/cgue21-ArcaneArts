#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform vec4 u_Color;

out vec4 v_Color;
void main() {
	gl_Position = vec4(a_Position, 1.0);
	v_Color = u_Color;
}

#type fragment
#version 330 core

out vec4 color;
in vec4 v_Color;
void main() {
	color = v_Color;
}