#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position; 

void main() 
{
	gl_Position = vec4(a_Position, 1.0);
}

#type geometry
#version 330 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 u_ViewProjection;
uniform vec3 u_CameraPos;
uniform float u_BillboardSize;

out vec2 v_TexCoord;

void main()
{
	vec3 Pos = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(u_CameraPos - Pos);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(toCamera, up) * u_BillboardSize;

	Pos -= right;
	gl_Position = u_ViewProjection * vec4(Pos, 1.0);
	v_TexCoord = vec2(0.0, 0.0);
	EmitVertex();

	Pos.y += u_BillboardSize;
	gl_Position = u_ViewProjection * vec4(Pos, 1.0);
	v_TexCoord = vec2(0.0, 1.0);
	EmitVertex();

	Pos.y -= u_BillboardSize;
	Pos += right;
	gl_Position = u_ViewProjection * vec4(Pos, 1.0);
	v_TexCoord = vec2(1.0, 0.0);
	EmitVertex();

	Pos.y += u_BillboardSize;
	gl_Position = u_ViewProjection * vec4(Pos, 1.0);
	v_TexCoord = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive(); 
} 

#type fragment
#version 330 core

uniform sampler2D u_ColorMap;

in vec2 v_TexCoord;
out vec4 FragColor;

void main()
{ 
	FragColor = texture2D(u_ColorMap, v_TexCoord);

	if (FragColor.r >= 0.9 && FragColor.g >= 0.9 && FragColor.b >= 0.9) {
		discard;
	}
}