#type vertex
#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 420 core

in vec2 v_TexCoord;

layout(binding = 0) uniform sampler2D u_Image;

uniform bool u_Horizontal;
uniform float u_Weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

out vec4 color;

void main()
{
	vec2 tex_offset = 1.0 / textureSize(u_Image, 0);
	vec3 result = texture(u_Image, v_TexCoord).rgb * u_Weight[0];

	if(u_Horizontal)
	{
		for(int i = 1; i < 5; ++i)
		{
			result += texture(u_Image, v_TexCoord + vec2(tex_offset.x * i, 0.0)).rgb * u_Weight[i];
			result += texture(u_Image, v_TexCoord - vec2(tex_offset.x * i, 0.0)).rgb * u_Weight[i];
		}
	}
	else
	{
		for(int i = 1; i < 5; ++i)
		{
			result += texture(u_Image, v_TexCoord + vec2(0.0, tex_offset.y * i)).rgb * u_Weight[i];
			result += texture(u_Image, v_TexCoord - vec2(0.0, tex_offset.y * i)).rgb * u_Weight[i];
		}
	}

	color = vec4(result, 1.0);
}
