#type vertex
#version 420 core

layout (location = 0) in float a_Type;
layout (location = 1) in vec3 a_Position;
layout (location = 2) in vec3 a_Velocity;
layout (location = 3) in float a_Power;

out float v_Type;
out vec3 v_Position;
out vec3 v_Velocity;
out float v_Power;

void main()
{
	v_Type = a_Type;
	v_Position = a_Position;
	v_Velocity = a_Velocity;
	v_Power = a_Power;
}

#type geometry
#version 420 core

#define TYPE_EMITTER 0.0f
#define TYPE_PARTICLE 1.0f

layout(points) in;
layout(points) out;
layout(max_vertices = 500) out;

in float v_Type[];
in vec3 v_Position[];
in vec3 v_Velocity[];
in float v_Power[];

out float Type;
out vec3 Position;
out vec3 Velocity;
out float Power;

// DeltaTime in milliseconds
uniform float u_DeltaTime_ms;
uniform float u_Time;

layout(binding = 0) uniform sampler1D u_RandomTexture;

uniform vec3 u_EmitterPosition;
uniform float u_EmitPower;
uniform float u_Cooling;

vec3 GetRandomDir(float TexCoord)
{
	vec3 direction = texture(u_RandomTexture, TexCoord).xyz;
	direction -= vec3(0.5, 0.5, 0.5);
	return direction;
}

void main()
{
	float deltaTime_s = u_DeltaTime_ms / 1000.0;
	float deltaPower = deltaTime_s * u_Cooling;
	
	if (v_Type[0] == TYPE_EMITTER)
	{
		float power = v_Power[0] + deltaPower;

		if (power > u_EmitPower)
		{
			for (int i = 1 ; i < 10 ; i++)
			{
				vec3 direction = GetRandomDir(u_Time/ (1001.0 * i));
				direction.y = max(direction.y, 0.5);
				Velocity = normalize(direction) / 20.0;
				Type = TYPE_PARTICLE;
				Position = u_EmitterPosition;
				Power = 1.0;
				EmitVertex();
				EndPrimitive();
			}
			power = 0;
		}

		Type = TYPE_EMITTER;
		Position = u_EmitterPosition;
		Velocity = v_Velocity[0];
		Power = power;
		EmitVertex();
		EndPrimitive();
	}
	else
	{
		float power = v_Power[0] - deltaPower;

		vec3 DeltaPosition = deltaTime_s * v_Velocity[0];
		vec3 DeltaVelocity = vec3(deltaTime_s) * (0.0, -9.81, 0.0);

		if (power > 0)
		{
			Type = TYPE_PARTICLE;
			Position = v_Position[0] + DeltaPosition;
			Velocity = v_Velocity[0] + DeltaVelocity;
			Power = power;
			EmitVertex();
			EndPrimitive();
		}
	}
}

