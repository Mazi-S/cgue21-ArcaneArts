#type vertex
#version 420 core
layout (location = 0) in vec3 a_Position;

layout (std140, binding = 6) uniform LightSpace
{
	mat4 u_LightSpaceMatrix;
};

void main()
{
	gl_Position = u_LightSpaceMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
layout(location = 0) out float fragmentdepth;

void main()
{
}