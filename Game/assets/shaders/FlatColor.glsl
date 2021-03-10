#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normals;

layout (std140) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
	vec3 u_DirectionalLight_Direction;
	vec3 u_DirectionalLight_Color;
	vec3 u_PointLight_Position;
	vec3 u_PointLight_Color;
	float u_PointLight_Constant;
	float u_PointLight_Linear;
	float u_PointLight_Quadratic;
};

uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

uniform mat4 u_LightSpaceMatrix;

out vec3 v_Position;
out vec3 v_Normals;
out vec2 v_TexCoord;
out vec4 v_FragPosLightSpace;

void main() {
	v_Normals = u_NormalMatrix * a_Normals;
	v_TexCoord = a_TexCoord;
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	v_FragPosLightSpace = u_LightSpaceMatrix * vec4(a_Position, 1.0);
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout (std140) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
	vec3 u_DirectionalLight_Direction;
	vec3 u_DirectionalLight_Color;
	vec3 u_PointLight_Position;
	vec3 u_PointLight_Color;
	float u_PointLight_Constant;
	float u_PointLight_Linear;
	float u_PointLight_Quadratic;
};

layout (std140) uniform MaterialData {
	vec3 u_Ambient;
	vec3 u_Diffuse;
	vec3 u_Specular;
	float u_Shininess;
};

uniform sampler2D u_ShadowMap;

in vec3 v_Position;
in vec3 v_Normals;
in vec2 v_TexCoord;
in vec4 v_FragPosLightSpace;

out vec4 color;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcPointLight(vec3 normal, vec3 viewDir);

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(normal, -lightDir)), 0.0005);
	// PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	if(projCoords.z > 1.0)
		shadow = 0.0;

	return shadow;
}

void main() {
	// normalize normal
	vec3 normal = normalize(v_Normals);
	vec3 viewDir = normalize(u_CameraPosition - v_Position);

	// shadow
	float shadow = ShadowCalculation(v_FragPosLightSpace, normal, u_DirectionalLight_Direction);

	// ambient
	vec3 resultAmbient = u_Ambient;

	vec3 result = resultAmbient;

	// directional light
    result += (1.0 - shadow) * CalcDirLight(normal, viewDir);

	// point light
    result += CalcPointLight(normal, viewDir);
		
	// final color
	color = vec4(result, 1.0);
}

vec3 CalcDirLight(vec3 normal, vec3 viewDir) 
{
	vec3 lightDir = u_DirectionalLight_Direction;
	vec3 lightColor = u_DirectionalLight_Color;

	vec3 newLightDir = normalize(-lightDir);

	// diffuse
	float diffuse = clamp(dot(newLightDir, normal), 0, 1);
	vec3 resultDiffuse = u_Diffuse * diffuse;

	// specular
	vec3 reflectDir = normalize(reflect(-newLightDir, normal));
	float specular = pow(max(dot(viewDir, reflectDir), 0), u_Shininess);
	vec3 resultSpecular = u_Specular * specular;

	// result
	return(lightColor  * (resultDiffuse * u_Ambient + resultSpecular));
}

vec3 CalcPointLight(vec3 normal, vec3 viewDir) 
{
	vec3 lightPos = u_PointLight_Position;
	vec3 lightColor = u_PointLight_Color;
	float constant = u_PointLight_Constant;
	float linear = u_PointLight_Linear;
	float quadratic = u_PointLight_Quadratic;

	vec3 lightDir = normalize(lightPos - v_Position);

	// diffuse
	float diffuse = clamp(dot(lightDir, normal), 0, 1);
	vec3 resultDiffuse = u_Diffuse * diffuse;

	// specular
	vec3 reflectDir = normalize(reflect(-lightDir, normal));
	float specular = pow(max(dot(viewDir, reflectDir), 0), u_Shininess);
	vec3 resultSpecular = u_Specular * specular;

	// calculate attenuation
	float distance = length(lightPos - v_Position);
	float attenuation = 1.0f / (constant + distance * linear + (distance * distance) * quadratic);

	// result
	return(lightColor * attenuation * (resultDiffuse * u_Ambient + resultSpecular));
}