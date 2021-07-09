workspace "Roguelike"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Roguelike"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"src/Engine/**.h",
		"src/Engine/**.cpp"
	}
	
	includedirs
	{
		"src/",
		"src/Engine/"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RL_RELEASE"
		optimize "On"