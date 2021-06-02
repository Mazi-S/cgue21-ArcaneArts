#type vertex
#version 420 core

struct PointLight {
	vec3 Position;
	vec3 Color;
	float Constant;
	float Linear;
	float Quadratic;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normals;

layout (std140, binding = 0) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
	vec3 u_DirectionalLight_Direction;
	vec3 u_DirectionalLight_Color;
	int u_PointLightCount;
	PointLight[5] u_PointLight;
};

layout (std140, binding = 6) uniform LightSpace
{
	mat4 u_LightSpaceMatrix;
};

uniform mat4 u_Transform;
uniform mat3 u_NormalMatrix;

out vec3 v_Position;
out vec3 v_Normals;
out vec2 v_TexCoord;
out vec4 v_FragPosLightSpace;

void main() {
	v_Normals = u_NormalMatrix * a_Normals;
	v_TexCoord = a_TexCoord;
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	v_FragPosLightSpace = u_LightSpaceMatrix * u_Transform * vec4(a_Position, 1.0);
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 420 core

struct PointLight {
	vec3 Position;
	vec3 Color;
	float Constant;
	float Linear;
	float Quadratic;
};

// illumination multiplier
uniform float u_Brightness = 1.0;

layout (std140, binding = 0) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
	vec3 u_DirectionalLight_Direction;
	vec3 u_DirectionalLight_Color;
	int u_PointLightCount;
	PointLight[5] u_PointLight;
};

layout (std140, binding = 1) uniform MaterialData {
	vec3 u_Ambient;
	vec3 u_Diffuse;
	vec3 u_Specular;
	float u_Shininess;
};

layout(binding = 0) uniform sampler2DShadow u_ShadowMap;

in vec3 v_Position;
in vec3 v_Normals;
in vec2 v_TexCoord;
in vec4 v_FragPosLightSpace;

out vec4 color;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcPointLight(vec3 normal, vec3 viewDir, vec3 lightPos, vec3 lightColor, float constant, float linear, float quadratic);

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(normal, -lightDir)), 0.0005);
	// PCF
	float shadow = 0.0;
	
	vec2 poissonDisk[16] = vec2[]( 
		vec2( -0.94201624, -0.39906216 ), 
		vec2( 0.94558609, -0.76890725 ), 
		vec2( -0.094184101, -0.92938870 ), 
		vec2( 0.34495938, 0.29387760 ), 
		vec2( -0.91588581, 0.45771432 ), 
		vec2( -0.81544232, -0.87912464 ), 
		vec2( -0.38277543, 0.27676845 ), 
		vec2( 0.97484398, 0.75648379 ), 
		vec2( 0.44323325, -0.97511554 ), 
		vec2( 0.53742981, -0.47373420 ), 
		vec2( -0.26496911, -0.41893023 ), 
		vec2( 0.79197514, 0.19090188 ), 
		vec2( -0.24188840, 0.99706507 ), 
		vec2( -0.81409955, 0.91437590 ), 
		vec2( 0.19984126, 0.78641367 ), 
		vec2( 0.14383161, -0.14100790 ) 
	);

	for (int i = 0; i < 16; i++)
	{
		vec3 coord = projCoords;
		coord.xy += poissonDisk[i] / 3000.0;
		float depth = texture( u_ShadowMap, coord, bias );
		shadow += 1.0 - depth;
	}
	shadow /= 16.0;

	// TODO: use other ShadowMap
	if(projCoords.z > 1.0)
		shadow = 0.0;

	return min(shadow, 0.8);
	//return shadow;
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
	for(int i = 0; i < u_PointLightCount; i++)
	{
		result += CalcPointLight(normal, viewDir, u_PointLight[i].Position, u_PointLight[i].Color, u_PointLight[i].Constant, u_PointLight[i].Linear, u_PointLight[i].Quadratic);
	}
		
	// final color
	color = vec4(result * u_Brightness, 1.0);
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
	return(lightColor  * (resultDiffuse + resultSpecular));
}

vec3 CalcPointLight(vec3 normal, vec3 viewDir, vec3 lightPos, vec3 lightColor, float constant, float linear, float quadratic)
{
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
	return(lightColor * attenuation * (resultDiffuse + resultSpecular));
}