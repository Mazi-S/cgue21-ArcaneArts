#type vertex
#version 420 core

layout (location = 0) in vec3 a_Position; 
layout (location = 1) in float a_Power;

uniform mat4 u_Offset;

out float g_Power;

void main() 
{

	gl_Position = u_Offset * vec4(a_Position, 1.0);
	g_Power = a_Power;
}

#type geometry
#version 420 core

in float g_Power[];

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 u_ViewProjection;
uniform vec3 u_CameraPos;
uniform float u_BillboardSize;

out vec2 v_TexCoord;
out float v_Power;

void main()
{
	vec3 Pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(u_CameraPos - Pos);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(toCamera, up) * u_BillboardSize;

	Pos -= right;
	gl_Position = u_ViewProjection * vec4(Pos, 1.0);
	v_TexCoord = vec2(0.0, 0.0);
	v_Power = g_Power[0];
	EmitVertex();

	Pos.y += u_BillboardSize;
	gl_Position = u_ViewProjection * vec4(Pos, 1.0);
	v_TexCoord = vec2(0.0, 1.0);
	v_Power = g_Power[0];
	EmitVertex();

	Pos.y -= u_BillboardSize;
	Pos += right;
	gl_Position = u_ViewProjection * vec4(Pos, 1.0);
	v_TexCoord = vec2(1.0, 0.0);
	v_Power = g_Power[0];
	EmitVertex();

	Pos.y += u_BillboardSize;
	gl_Position = u_ViewProjection * vec4(Pos, 1.0);
	v_TexCoord = vec2(1.0, 1.0);
	v_Power = g_Power[0];
	EmitVertex();

	EndPrimitive(); 
} 

#type fragment
#version 420 core

// illumination multiplier
uniform float u_Brightness = 1.0;

layout(binding = 0) uniform sampler2D u_ColorMap;
uniform vec4 u_ColorStart;
uniform vec4 u_ColorEnd;

in vec2 v_TexCoord;
in float v_Power;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 color_bright; 

void main()
{ 
	vec4 color_T = u_ColorStart * v_Power + u_ColorEnd * (1.0 - v_Power);
	color = color_T * texture2D(u_ColorMap, v_TexCoord);
	color.xyz *= u_Brightness;

	color_bright = color;
	color_bright.w *= 2.5;
}