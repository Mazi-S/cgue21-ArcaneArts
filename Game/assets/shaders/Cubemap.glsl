#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 2) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_Position;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_Normal = mat3(transpose(inverse(u_View * u_Model))) * a_Normal;
    v_Position = vec3(u_View * u_Model * vec4(a_Position, 1.0));
    gl_Position = u_Projection * vec4(v_Position, 1.0);
}  

#type fragment
#version 330 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 color_bright; 

in vec3 v_Normal;
in vec3 v_Position;

uniform mat4 u_View;
uniform samplerCube u_Skybox;

void main()
{    
    vec3 I = normalize(v_Position);
    vec3 viewR = reflect(I, normalize(v_Normal));
    vec3 worldR = inverse(mat3(u_View)) * viewR;
    color = vec4(texture(u_Skybox, -worldR).rgb, 1.0);

	color_bright = vec4(0.0, 0.0, 0.0, 1.0);
}