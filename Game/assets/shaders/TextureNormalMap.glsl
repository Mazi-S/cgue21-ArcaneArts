#type vertex
#version 420 core

struct DirectionalLight {
	vec3 Direction;
	vec3 Color;
};

struct PointLight {
	vec3 Position;
	vec3 Color;
	float Constant;
	float Linear;
	float Quadratic;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

// Scene Data
layout (std140, binding = 0) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
	float u_Time;
	int u_PointLightCount;
	vec2 placeholder;
	DirectionalLight u_DirectionalLight;
	PointLight[5] u_PointLight;
};

layout (std140, binding = 1) uniform ModelData {
	mat4 u_Transform;
	mat3 u_NormalMatrix;
};

layout (std140, binding = 6) uniform LightSpace
{
	mat4 u_LightSpaceMatrix;
};

out vec3 v_Position;
out vec2 v_TexCoord;
out vec4 v_FragPosLightSpace;
out mat3 v_TBN;

void main() {
	vec3 T = normalize(vec3(u_Transform * vec4(a_Tangent,   0.0)));
	vec3 B = normalize(vec3(u_Transform * vec4(a_Bitangent, 0.0)));
	vec3 N = normalize(vec3(u_Transform * vec4(a_Normal,    0.0)));
	v_TBN = mat3(T, B, N);
	
	v_TexCoord = a_TexCoord;
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	v_FragPosLightSpace = u_LightSpaceMatrix * u_Transform * vec4(a_Position, 1.0);

	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 420 core

struct DirectionalLight {
	vec3 Direction;
	vec3 Color;
};

struct PointLight {
	vec3 Position;
	vec3 Color;
	float Constant;
	float Linear;
	float Quadratic;
};

// illumination multiplier
uniform float u_Brightness = 1.0;

// Scene Data
layout (std140, binding = 0) uniform SceneData {
	mat4 u_ViewProjection;
	vec3 u_CameraPosition;
	float u_Time;
	int u_PointLightCount;
	vec2 placeholder;
	DirectionalLight u_DirectionalLight;
	PointLight[5] u_PointLight;
};

layout (std140, binding = 2) uniform MaterialData {
	vec3 u_Ambient;
	vec3 u_Diffuse;
	vec3 u_Specular;
	float u_Shininess;
};

layout(binding = 0) uniform sampler2DShadow u_ShadowMap;
layout(binding = 1) uniform sampler2D u_ColorTexture;
layout(binding = 2) uniform sampler2D u_NormalTexture;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec4 v_FragPosLightSpace;
in mat3 v_TBN;

uniform mat3 u_NormalMatrix;

out vec4 color;

vec3 CalcDirLight(vec4 color, vec3 diffuse, vec3 specular, float shininess, vec3 normal, vec3 viewDir, DirectionalLight directionalLight);
vec3 CalcPointLight(vec4 color, vec3 diffuse, vec3 specular, float shininess, vec3 position, vec3 normal, vec3 viewDir, PointLight pointLight);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main() {
	vec3 viewDir = normalize(u_CameraPosition - v_Position);
	vec4 fragmentColor = texture(u_ColorTexture, v_TexCoord);
	vec3 normal = texture(u_NormalTexture, v_TexCoord).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(v_TBN * normal);
	//vec3 normal = normalize(u_NormalMatrix * texture(u_NormalTexture, v_TexCoord).xyz);

	// shadow
	float shadow = ShadowCalculation(v_FragPosLightSpace, normal, u_DirectionalLight.Direction);

	// ambient
	vec3 result = u_Ambient * fragmentColor.rgb;

	// directional light
	result += (1.0 - shadow) * CalcDirLight(fragmentColor, u_Diffuse, u_Specular, u_Shininess, normal, viewDir, u_DirectionalLight);

	// point light
	for(int i = 0; i < u_PointLightCount; i++)
	{
		result += CalcPointLight(fragmentColor, u_Diffuse, u_Specular, u_Shininess, v_Position, normal, viewDir, u_PointLight[i]);
	}

	// final color
	color = vec4(result * u_Brightness, 1.0);
}



vec3 CalcDirLight(vec4 color, vec3 diffuse, vec3 specular, float shininess, vec3 normal, vec3 viewDir, DirectionalLight directionalLight)
{
	vec3 newLightDir = normalize(-directionalLight.Direction);
	vec3 reflectDir = normalize(reflect(-newLightDir, normal));

	// diffuse
	vec3 resultDiffuse = diffuse * clamp(dot(newLightDir, normal), 0, 1);

	// specular
	vec3 resultSpecular = specular * pow(max(dot(viewDir, reflectDir), 0), u_Shininess);

	// result
	return(directionalLight.Color  * (resultDiffuse * color.rgb + resultSpecular));
}

// Blinn-Phong shading
vec3 CalcPointLight(vec4 color, vec3 diffuse, vec3 specular, float shininess, vec3 position, vec3 normal, vec3 viewDir, PointLight pointLight)
{
	vec3 lightDir = normalize(pointLight.Position - position);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	// diffuse
	vec3 resultDiffuse = diffuse * clamp(dot(lightDir, normal), 0, 1);

	// specular
	vec3 resultSpecular = specular * pow(max(dot(normal, halfwayDir), 0.0), shininess);

	// calculate attenuation
	float distance = length(pointLight.Position - v_Position);
	float attenuation = 1.0 / (pointLight.Constant + distance * pointLight.Linear + (distance * distance) * pointLight.Quadratic);

	// result
	return(pointLight.Color * attenuation * (resultDiffuse * color.rgb + resultSpecular));
}

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