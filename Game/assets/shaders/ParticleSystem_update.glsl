#type vertex
#version 330 core

layout (location = 0) in float a_Type;
layout (location = 1) in vec3 a_Position;
layout (location = 2) in vec3 a_Velocity;
layout (location = 3) in float a_Age;

out float v_Type0;
out vec3 v_Position0;
out vec3 v_Velocity0;
out float v_Age0;

void main()
{
	v_Type0 = a_Type;
	v_Position0 = a_Position;
	v_Velocity0 = a_Velocity;
	v_Age0 = a_Age;
}

#type geometry
#version 330 core

layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;

in float v_Type0[];
in vec3 v_Position0[];
in vec3 v_Velocity0[];
in float v_Age0[];

out float Type1;
out vec3 Position1;
out vec3 Velocity1;
out float Age1;

uniform float u_DeltaTimeMillis;
uniform float u_Time;
uniform sampler1D u_RandomTexture;
uniform float u_LauncherLifetime;
uniform float u_ShellLifetime;
uniform float u_SecondaryShellLifetime;

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

vec3 GetRandomDir(float TexCoord)
{
	vec3 Dir = texture(u_RandomTexture, TexCoord).xyz;
	Dir -= vec3(0.5, 0.5, 0.5);
	return Dir;
}

void main()
{
	float Age = v_Age0[0] + u_DeltaTimeMillis;

	if (v_Type0[0] == PARTICLE_TYPE_LAUNCHER) {
		if (Age >= u_LauncherLifetime) {
			Type1 = PARTICLE_TYPE_SHELL;
			Position1 = v_Position0[0];
			vec3 Dir = GetRandomDir(u_Time/1000.0);
			Dir.y = max(Dir.y, 0.5);
			Velocity1 = normalize(Dir) / 20.0;
			Age1 = 0.0;
			EmitVertex();
			EndPrimitive();
			Age = 0.0;
		}

		Type1 = PARTICLE_TYPE_LAUNCHER;
		Position1 = v_Position0[0];
		Velocity1 = v_Velocity0[0];
		Age1 = Age;
		EmitVertex();
		EndPrimitive();
	}
	else {
		float DeltaTimeSecs = u_DeltaTimeMillis / 1000.0f;
		float t1 = v_Age0[0] / 1000.0;
		float t2 = Age / 1000.0;
		vec3 DeltaP = DeltaTimeSecs * v_Velocity0[0];
		vec3 DeltaV = vec3(DeltaTimeSecs) * (0.0, -9.81, 0.0);

		if (v_Type0[0] == PARTICLE_TYPE_SHELL)  {
			if (Age < u_ShellLifetime) {
				Type1 = PARTICLE_TYPE_SHELL;
				Position1 = v_Position0[0] + DeltaP;
				Velocity1 = v_Velocity0[0] + DeltaV;
				Age1 = Age;
				EmitVertex();
				EndPrimitive();
			}
			else {
				for (int i = 0 ; i < 10 ; i++) {
					 Type1 = PARTICLE_TYPE_SECONDARY_SHELL;
					 Position1 = v_Position0[0];
					 vec3 Dir = GetRandomDir((u_Time + i)/1000.0);
					 Velocity1 = normalize(Dir) / 20.0;
					 Age1 = 0.0f;
					 EmitVertex();
					 EndPrimitive();
				}
			}
		}
		else {
			if (Age < u_SecondaryShellLifetime) {
				Type1 = PARTICLE_TYPE_SECONDARY_SHELL;
				Position1 = v_Position0[0] + DeltaP;
				Velocity1 = v_Velocity0[0] + DeltaV;
				Age1 = Age;
				EmitVertex();
				EndPrimitive();
			}
		}
	}
}

