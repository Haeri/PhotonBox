workspace "PhotonBox"
	architecture "x64"
	startproject "Runtime"

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
	kind "SharedLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"
	defines "PB_EXPORT"

	targetdir ("%{prj.name}/bin/" .. output_dir)
	objdir ("%{prj.name}/bin/int/" .. output_dir)


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
		stb_dir			.. "/include",
		zlib_dir		.. "/include",
		physx_dir		.. "/include/PhysX",
	}

	libdirs 
	{
		freetype_dir	.. "/lib",
		glfw_dir		.. "/lib",
		zlib_dir		.. "/lib",
	}

	filter "system:windows"
		defines "PB_PLATFORM_WIN"
		systemversion "latest"
		links 
		{
			"opengl32",
			"glfw3",
			"zlib",
			"freetype",
		}

	filter "system:Linux"
		defines "PB_PLATFORM_NIX"
		linkoptions 
		{
			 "-Wl,-rpath=.",
			 "-no-pie" 
		}
		links 
		{
			"GL",
			"glfw3",
			"X11",
			"Xxf86vm",
			"Xrandr",
			"Xinerama",
			"Xcursor",
			"pthread",
			"dl",
			"zlib",
			"freetype",
		}


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


	filter "configurations:Debug or configurations:Mem-Debug"
		runtime "Debug"
		symbols "On"
		libdirs (physx_dir .. "/lib/debug")
		links
		{
			"PxPvdSDKDEBUG_x64",
			"PhysX3ExtensionsDEBUG",
			"PxFoundationDEBUG_x64",
			"PhysX3CommonDEBUG_x64",
			"PhysX3DEBUG_x64",
		}


	filter "configurations:Release or configurations:Dist"
		defines { "PB_RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "On"
		libdirs (physx_dir .. "/lib/release")
		links
		{
			"PxPvdSDK_x64",
			"PhysX3Extensions",
			"PxFoundation_x64",
			"PhysX3Common_x64",
			"PhysX3_x64",
		}



project "Runtime"
	location "Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"
	dependson 
	{
		"PhotonBox",
		"Game",
	}

	targetdir ("%{prj.name}/bin/" .. output_dir)
	objdir ("%{prj.name}/bin/int/" .. output_dir)

	files
	{
		"%{prj.name}/src/main.cpp"
	}

	includedirs
	{
		"PhotonBox/include",

		freetype_dir	.. "/include",
		glad_dir		.. "/include",
		glfw_dir		.. "/include",
		imgui_dir		.. "/include",
		khr_dir			.. "/include",
		stb_dir			.. "/include",
		zlib_dir		.. "/include",
		physx_dir		.. "/include/PhysX",
	}

	libdirs 
	{
		freetype_dir	.. "/lib",
		glfw_dir		.. "/lib",
		zlib_dir		.. "/lib",
	}


	-- PLATTFORM FILTER

	filter "system:windows"
		defines "PB_PLATFORM_WIN"
		systemversion "latest"
		links 
		{
			"opengl32",
			"PhotonBox",
			"glfw3",
			"zlib",
			"freetype",
		}

	filter "system:Linux"
		defines "PB_PLATFORM_NIX"
		linkoptions 
		{
			 "-Wl,-rpath=.",
			 "-no-pie" 
		}
		links 
		{
			"GL",
			"glfw3",
			"X11",
			"Xxf86vm",
			"Xrandr",
			"Xinerama",
			"Xcursor",
			"pthread",
			"dl",
			"PhotonBox",
			"zlib",
			"freetype",
		}

	-- MODE FILTER

	filter "configurations:Debug"
		defines { "PB_DEBUG", "_DEBUG" }

	filter "configurations:Mem-Debug"
		defines { "PB_MEM_DEBUG", "_DEBUG" }


	filter "configurations:Debug or configurations:Mem-Debug"
		runtime "Debug"
		symbols "On"
		libdirs (physx_dir .. "/lib/debug")
		links
		{
			"PxPvdSDKDEBUG_x64",
			"PhysX3ExtensionsDEBUG",
			"PxFoundationDEBUG_x64",
			"PhysX3CommonDEBUG_x64",
			"PhysX3DEBUG_x64",
		}


	filter "configurations:Release or configurations:Dist"
		defines { "PB_RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "On"
		libdirs (physx_dir .. "/lib/release")
		links
		{
			"PxPvdSDK_x64",
			"PhysX3Extensions",
			"PxFoundation_x64",
			"PhysX3Common_x64",
			"PhysX3_x64",
		}

	filter {"system:windows", "configurations:Debug or configurations:Mem-Debug"}
		postbuildcommands 
		{
			"{COPY} ../PhotonBox/" .. output_dir .. "/PhotonBox.dll ../Runtime/bin/" .. output_dir,
			"{COPY} ../PhotonBox/" .. output_dir .. "/PhotonBox.pdb ../Runtime/bin/" .. output_dir,

			"{COPY} ../" .. physx_dir .. "/bin/debug/*.dll ../Runtime/bin/" .. output_dir,
		}
	filter {"system:windows", "configurations:Release or configurations:Dist"}
		postbuildcommands 
		{
			"{COPY} ../PhotonBox/" .. output_dir .. "/PhotonBox.dll ../Runtime/bin/" .. output_dir,

			"{COPY} ../" .. physx_dir .. "/bin/release/*.dll ../Runtime/bin/" .. output_dir,
		}

	filter {"system:Linux", "configurations:Debug or configurations:Mem-Debug"}
		libdirs (physx_dir .. "/bin/debug")
		postbuildcommands 
		{
			"{COPY} ../" .. physx_dir .. "/bin/debug/*.so ../%{prj.name}",
		}

	filter {"system:Linux", "configurations:Release or configurations:Dist"}
		libdirs (physx_dir .. "/bin/release")
		postbuildcommands 
		{
			"{COPY} ../" .. physx_dir .. "/bin/release/*.so ../%{prj.name}",
		}


-- PROJECT GAME

project "Game"
	location "Game"
	kind "SharedLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "off"

	targetdir ("%{prj.name}/bin/" .. output_dir)
	objdir ("%{prj.name}/bin/int/" .. output_dir)

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		glad_dir 	.. "/include/glad/**.c",
	}

	includedirs
	{
		"PhotonBox/include",

		freetype_dir	.. "/include",
		glad_dir		.. "/include",
		glfw_dir		.. "/include",
		imgui_dir		.. "/include",
		khr_dir			.. "/include",
		stb_dir			.. "/include",
		zlib_dir		.. "/include",
		physx_dir		.. "/include/PhysX",
	}

	libdirs 
	{
		freetype_dir	.. "/lib",
		glfw_dir		.. "/lib",
		zlib_dir		.. "/lib",
	}


	-- PLATTFORM FILTER

	filter "system:windows"
		defines "PB_PLATFORM_WIN"
		systemversion "latest"
		links 
		{
			"opengl32",
  			"PhotonBox",
			"glfw3",
			"zlib",
			"freetype",
		}

	filter "system:Linux"
		defines "PB_PLATFORM_NIX"
		linkoptions 
		{
			 "-Wl,-rpath=.",
			 "-no-pie" 
		}
		links 
		{
			"GL",
			"glfw3",
			"X11",
			"Xxf86vm",
			"Xrandr",
			"Xinerama",
			"Xcursor",
			"pthread",
			"dl",
			"PhotonBox",
			"zlib",
			"freetype",
		}

	-- MODE FILTER

	filter "configurations:Debug"
		defines { "PB_DEBUG", "_DEBUG" }

	filter "configurations:Mem-Debug"
		defines { "PB_MEM_DEBUG", "_DEBUG" }


	filter "configurations:Debug or configurations:Mem-Debug"
		runtime "Debug"
		symbols "On"
--		libdirs (physx_dir .. "/lib/debug")
--		links
--		{
--			"PxPvdSDKDEBUG_x64",
--			"PhysX3ExtensionsDEBUG",
--			"PxFoundationDEBUG_x64",
--			"PhysX3CommonDEBUG_x64",
--			"PhysX3DEBUG_x64",
--		}


	filter "configurations:Release or configurations:Dist"
		defines { "PB_RELEASE", "NDEBUG" }
		runtime "Release"
		optimize "On"
--		libdirs (physx_dir .. "/lib/release")
--		links
--		{
--			"PxPvdSDK_x64",
--			"PhysX3Extensions",
--			"PxFoundation_x64",
--			"PhysX3Common_x64",
--			"PhysX3_x64",
--		}


	filter {"system:windows", "configurations:Debug or configurations:Mem-Debug"}
		postbuildcommands 
		{
			"{COPY} ../Game/bin/" .. output_dir .. "/Game.dll ../Runtime/bin/" .. output_dir,
			"{COPY} ../Game/bin/" .. output_dir .. "/Game.pdb ../Runtime/bin/" .. output_dir,
		}
	filter {"system:windows", "configurations:Release or configurations:Dist"}
		postbuildcommands 
		{
			"{COPY} ../Game/bin/" .. output_dir .. "/Game.dll ../Runtime/bin/" .. output_dir,
		}

	filter {"system:Linux", "configurations:Debug or configurations:Mem-Debug"}
		postbuildcommands 
		{
			"{COPY} ../Game/bin/" .. output_dir .. "/Game.so ../Runtime/bin/" .. output_dir,
			"{COPY} ../Game/bin/" .. output_dir .. "/Game.pdb ../Runtime/bin/" .. output_dir,
		}

	filter {"system:Linux", "configurations:Release or configurations:Dist"}
		postbuildcommands 
		{
			"{COPY} ../Game/bin/" .. output_dir .. "/Game.so ../Runtime/bin/" .. output_dir,
		}