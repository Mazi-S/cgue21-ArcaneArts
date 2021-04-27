workspace "ArcaneArts"
	architecture "x64"
	startproject "Game"

	configurations {
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

LibDir = {}
LibDir["physx_debug"] = "%{wks.location}/Engine/dependencies/physx/bin/debug"
LibDir["physx_release"] = "%{wks.location}/Engine/dependencies/physx/bin/release"
LibDir["irrklang"] = "%{wks.location}/Engine/dependencies/irrklang/bin"

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Engine/dependencies/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/dependencies/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Engine/dependencies/Glad/include"
IncludeDir["glm"] = "%{wks.location}/Engine/dependencies/glm"
IncludeDir["stb_image"] = "%{wks.location}/Engine/dependencies/stb_image"
IncludeDir["entt"] = "%{wks.location}/Engine/dependencies/entt/include"
IncludeDir["tinyobjloader"] = "%{wks.location}/Engine/dependencies/tinyobjloader"
IncludeDir["physx"] = "%{wks.location}/Engine/dependencies/physx/include"
IncludeDir["irrklang"] = "%{wks.location}/Engine/dependencies/irrklang"
IncludeDir["yaml_cpp"] = "%{wks.location}/Engine/dependencies/yaml-cpp/include"
IncludeDir["ImGui"] = "%{wks.location}/Engine/dependencies/imgui"

group "Dependencies"
	include "Engine/dependencies/GLFW"
	include "Engine/dependencies/Glad"
	include "Engine/dependencies/yaml-cpp"
	include "Engine/dependencies/imgui"

group ""
	include "Engine"
	include "Game"
