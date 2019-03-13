workspace "PhotonBox"
	architecture "x64"
	startproject "Game"

	configurations
	{
		"Debug",
		"Mem-Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.architecture}/%{cfg.buildcfg}"

project "PhotonBox"
	location "PhotonBox"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{prj.name}/bin/" .. outputdir)
	objdir ("%{prj.name}/bin/int/" .. outputdir)

	--pchheader "pbpch.h"
	--pchsource "PhotonBox/src/pbpch.cpp"

	files
	{
		"%{prj.name}/include/PhotonBox/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/include/glad/glad.c",
		"%{prj.name}/include/imgui/**.cpp",	
	}

	includedirs
	{
		"%{prj.name}/include",
		"%{prj.name}/include/PhysX"
	}

	filter "system:windows"
		defines "PB_PLATFORM_WIN"
		cppdialect "C++11"
		systemversion "latest"

	filter "configurations:Debug"
		defines { "PB_DEBUG", "_DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Mem-Debug"
		defines { "PB_MEM_DEBUG", "_DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "PB_RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines { "PB_RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "On"
		postbuildcommands ("python \"../Tools/build_incrementer.py\"")

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("%{prj.name}/bin/" .. outputdir)
	objdir ("%{prj.name}/bin/int/" .. outputdir)

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"PhotonBox/src",
		"PhotonBox/include",
		"PhotonBox/include/PhotonBox",
		"PhotonBox/include/PhysX",
	}

	libdirs 
	{
	 	"PhotonBox/lib" 
	}

	links
	{
		"PhotonBox",
		"glfw3",
		"zlibstat",
		"freetype271MT",
	}

	filter "system:windows"
		defines "PB_PLATFORM_WIN"
		cppdialect "C++17"
		systemversion "latest"
		links "opengl32"

	filter "system:linux"
		defines "PB_PLATFORM_LNX"
		links "GL"		

	filter "configurations:Debug"
		defines { "PB_DEBUG", "_DEBUG" }
		runtime "Debug"
		symbols "On"
		links
		{
			"PhysX3DEBUG_x64",
			"PhysX3CommonDEBUG_x64",
			"PhysX3ExtensionsDEBUG",
			"PxFoundationDEBUG_x64",
		}
		postbuildcommands ("{COPY} \"../Redist/Debug\" \"bin/" .. outputdir .. "\"")

	filter "configurations:Mem-Debug"
		defines { "PB_MEM_DEBUG", "_DEBUG" }
		runtime "Debug"
		symbols "On"
		links
		{
			"PhysX3DEBUG_x64",
			"PhysX3CommonDEBUG_x64",
			"PhysX3ExtensionsDEBUG",
			"PxFoundationDEBUG_x64",
		}
		postbuildcommands ("{COPY} \"../Redist/Debug\" \"bin/" .. outputdir .. "\"")

	filter "configurations:Release"
		defines { "PB_RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "On"
		links
		{
			"PhysX3_x64",
			"PhysX3Common_x64",
			"PhysX3Extensions",
			"PxFoundation_x64",
		}
		postbuildcommands ("{COPY} \"../Redist/Release\" \"bin/" .. outputdir .. "\"")

	filter "configurations:Dist"
		defines { "PB_RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "On"
		links
		{
			"PhysX3_x64",
			"PhysX3Common_x64",
			"PhysX3Extensions",
			"PxFoundation_x64",
		}
		postbuildcommands ("{COPY} \"../Redist/Release\" \"bin/" .. outputdir .. "\"")