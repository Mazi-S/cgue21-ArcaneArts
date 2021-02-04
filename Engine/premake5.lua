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
		"dependencies/glm/glm/**.hpp",
		"dependencies/glm/glm/**.inl",
		"dependencies/stb_image/**.h",
		"dependencies/stb_image/**.cpp",
		"dependencies/tinyobjloader/**.h",
		"dependencies/tinyobjloader/**.cpp"
	}

	includedirs {
		"%{wks.location}/Engine/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.tinyobjloader}"
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
