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

group "Dependencies"
	include "Engine/dependencies/GLFW"
	include "Engine/dependencies/Glad"

group ""
	include "Engine"
	include "Game"
