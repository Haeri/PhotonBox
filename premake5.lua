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


	-- VARIABLES

	output_dir		= "%{cfg.architecture}/%{cfg.buildcfg}"

	freetype_dir 	= "PhotonBox/vendor/freetype"
	glad_dir 		= "PhotonBox/vendor/glad"
	glfw_dir 		= "PhotonBox/vendor/GLFW"
	imgui_dir		= "PhotonBox/vendor/imgui"
	khr_dir			= "PhotonBox/vendor/KHR"
	physx_dir 		= "PhotonBox/vendor/PhysX"
	stb_dir 		= "PhotonBox/vendor/STB"
	zlib_dir		= "PhotonBox/vendor/zlib"


-- PROJECT PHOTONBOX

project "PhotonBox"
	location "PhotonBox"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"

	targetdir ("%{prj.name}/bin/" .. output_dir)
	objdir ("%{prj.name}/bin/int/" .. output_dir)

	--pchheader "pbpch.h"
	--pchsource "PhotonBox/src/pbpch.cpp"

	files
	{
		"%{prj.name}/include/PhotonBox/**.h",
		"%{prj.name}/src/**.cpp",

		glad_dir 	.. "/include/glad/**.c",
		imgui_dir 	.. "/include/imgui/**.cpp",	
	}

	includedirs
	{
		"%{prj.name}/include",
		freetype_dir	.. "/include",
		glad_dir		.. "/include",
		glfw_dir		.. "/include",
		imgui_dir		.. "/include",
		khr_dir			.. "/include",
		physx_dir		.. "/include",
		stb_dir			.. "/include",
		zlib_dir		.. "/include",
	}

	filter "system:windows"
		defines "PB_PLATFORM_WIN"
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


-- PROJECT GAME

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"

	targetdir ("%{prj.name}/bin/" .. output_dir)
	objdir ("%{prj.name}/bin/int/" .. output_dir)

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"PhotonBox/include",
		"PhotonBox/include/PhotonBox",

		freetype_dir	.. "/include",
		glad_dir		.. "/include",
		glfw_dir		.. "/include",
		imgui_dir		.. "/include",
		khr_dir			.. "/include",
		physx_dir		.. "/include",
		stb_dir			.. "/include",
		zlib_dir		.. "/include",
	}

	libdirs 
	{
		freetype_dir	.. "/lib",
		glad_dir		.. "/lib",
		glfw_dir		.. "/lib",
		imgui_dir		.. "/lib",
		khr_dir			.. "/lib",
		stb_dir			.. "/lib",
		zlib_dir		.. "/lib",
	}

	links
	{
		"PhotonBox",
		"glfw3",
		"zlib",
		"freetype271MT",
	}


	-- PLATTFORM FILTER

	filter "system:windows"
		defines "PB_PLATFORM_WIN"
		systemversion "latest"
		links "opengl32"

	filter "system:Linux"
		defines "PB_PLATFORM_NIX"
		links {
			"GL",
			"GLU",
			"X11",
			"Xxf86vm",
			"Xrandr",
			"pthread",
			"Xi",
			"dl",
		}


	-- MODE FILTER

	filter "configurations:Debug"
		defines { "PB_DEBUG", "_DEBUG" }
		runtime "Debug"
		symbols "On"
		libdirs (physx_dir .. "/lib/debug")
		links
		{
			"PhysX3DEBUG_x64",
			"PhysX3CommonDEBUG_x64",
			"PhysX3ExtensionsDEBUG",
			"PxFoundationDEBUG_x64",
		}

		postbuildcommands 
		{
			"{COPY} ../" .. freetype_dir 	.. "/bin/ bin/" 		.. output_dir,
			"{COPY} ../" .. physx_dir 		.. "/bin/debug/ bin/"	.. output_dir,
		}

	filter "configurations:Mem-Debug"
		defines { "PB_MEM_DEBUG", "_DEBUG" }
		runtime "Debug"
		symbols "On"
		libdirs (physx_dir .. "/lib/debug")
		links
		{
			"PhysX3DEBUG_x64",
			"PhysX3CommonDEBUG_x64",
			"PhysX3ExtensionsDEBUG",
			"PxFoundationDEBUG_x64",
		}

		postbuildcommands 
		{
			"{COPY} ../" .. freetype_dir 	.. "/bin/ bin/" 		.. output_dir,
			"{COPY} ../" .. physx_dir 		.. "/bin/debug/ bin/"	.. output_dir,
		}

	filter "configurations:Release"
		defines { "PB_RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "On"
		libdirs (physx_dir .. "/lib/release")
		links
		{
			"PhysX3_x64",
			"PhysX3Common_x64",
			"PhysX3Extensions",
			"PxFoundation_x64",
		}

		postbuildcommands 
		{
			"{COPY} ../" .. freetype_dir 	.. "/bin/ bin/" 		.. output_dir,
			"{COPY} ../" .. physx_dir 		.. "/bin/release/ bin/"	.. output_dir,
		}

	filter "configurations:Dist"
		defines { "PB_RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "On"
		libdirs (physx_dir .. "/lib/release")
		links
		{
			"PhysX3_x64",
			"PhysX3Common_x64",
			"PhysX3Extensions",
			"PxFoundation_x64",
		}

		postbuildcommands 
		{
			"{COPY} ../" .. freetype_dir 	.. "/bin/ bin/" 		.. output_dir,
			"{COPY} ../" .. physx_dir 		.. "/bin/release/ bin/"	.. output_dir,
		}