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
		"dependencies/tinyobjloader/**.cpp",
		"dependencies/physx/include/**.h",
	}

	includedirs {
		"%{wks.location}/Engine/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.tinyobjloader}",
		"%{IncludeDir.physx}"
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib",
		"PhysXCommon_64.lib",
		"PhysX_64.lib",
		"PhysXFoundation_64.lib",
		"PhysXExtensions_static_64.lib",
		"PhysXCooking_64.lib"
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
		libdirs { "%{LibDir.physx_debug}" }

	filter "configurations:Release"
		defines	{
			"ENGINE_RELEASE",
			"NDEBUG"
		}
		runtime "Release"
		optimize "on"
		libdirs { "%{LibDir.physx_release}" }
