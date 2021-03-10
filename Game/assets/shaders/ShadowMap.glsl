#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_DepthMVP;

void main()
{
	gl_Position = u_DepthMVP * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
layout(location = 0) out float fragmentdepth;

void main()
{
}