workspace "ArcaneArts"
	architecture "x64"
	startproject "Game"

	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Engine/dependencies/GLFW/include"

group "Dependencies"
	include "Engine/dependencies/GLFW"

group ""
	include "Engine"
	include "Game"
