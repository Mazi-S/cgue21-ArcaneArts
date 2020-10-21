project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "egpch.h"
	pchsource "src/egpch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
	}

	includedirs {
		"%{wks.location}/Engine/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	defines	{
		"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		runtime "Release"
		optimize "on"
