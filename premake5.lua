workspace "ArcaneArts"
	architecture "x64"
	startproject "Game"

	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Engine/dependencies/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/dependencies/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Engine/dependencies/Glad/include"
IncludeDir["glm"] = "%{wks.location}/Engine/dependencies/glm"
IncludeDir["stb_image"] = "%{wks.location}/Engine/dependencies/stb_image"
IncludeDir["entt"] = "%{wks.location}/Engine/dependencies/entt/include"

group "Dependencies"
	include "Engine/dependencies/GLFW"
	include "Engine/dependencies/Glad"

group ""
	include "Engine"
	include "Game"
