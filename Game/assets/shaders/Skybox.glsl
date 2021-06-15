#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 v_Texture;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	v_Texture = a_Position;
	vec4 pos = u_Projection * u_View * vec4(a_Position, 1.0);
	gl_Position = pos.xyww;
}

#type fragment
#version 330 core

// illumination multiplier
uniform float u_Brightness = 1.0;

uniform samplerCube u_Skybox;

in vec3 v_Texture;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 color_bright;

void main()
{
	color = texture(u_Skybox, -v_Texture);
	color.xyz *= u_Brightness;

	color_bright = vec4(0.0, 0.0, 0.0, 1.0);
}