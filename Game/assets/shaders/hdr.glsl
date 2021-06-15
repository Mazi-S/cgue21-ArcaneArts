#type vertex
#version 420 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main() {
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 420 core

in vec2 v_TexCoord;

out vec4 color;

layout(binding = 0) uniform sampler2D u_HDRBuffer;
layout(binding = 1) uniform sampler2D u_BloomBuffer;

uniform bool u_Bloom;
uniform bool u_HDR;
uniform bool u_GammaCorrection;
uniform float u_Exposure;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(u_HDRBuffer, v_TexCoord).rgb;
	vec3 bloomColor = texture(u_BloomBuffer, v_TexCoord).rgb;

	if(u_Bloom)
	{
		hdrColor += bloomColor; // additive blending
	}

	if(u_HDR)
	{
		hdrColor = vec3(1.0) - exp(-hdrColor * u_Exposure);
	}

	if(u_GammaCorrection)
	{
		hdrColor = pow(hdrColor, vec3(1.0 / gamma));
	}

	color = vec4(hdrColor, 1.0);
}
