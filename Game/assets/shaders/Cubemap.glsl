#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_Position;

uniform mat4 u_Model;
uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    gl_Position = u_Projection * u_View * vec4(v_Position, 1.0);
}  

#type fragment
#version 330 core
out vec4 color;

in vec3 v_Normal;
in vec3 v_Position;

uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;

void main()
{    
    vec3 I = normalize(v_Position - u_CameraPos);
    vec3 R = reflect(I, normalize(v_Normal));
    color = vec4(texture(u_Skybox, R).rgb, 1.0);
}