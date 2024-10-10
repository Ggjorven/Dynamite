MacOSVersion = MacOSVersion or "14.5"

project "Dynamite"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	architecture "x86_64"

	debugdir ("%{wks.location}")
	debugargs { "Dynamite/dynamite/main.dy --I=IncludeDir1 --Verbose" }

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	-- Note: VS2022/Make only need the pchheader filename
	pchheader "dypch.h"
	pchsource "src/Dynamite/dypch.cpp"

	--------------------------------------
	-- Files & Options
	--------------------------------------
	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		"src/Dynamite",

		"%{Dependencies.Pulse.IncludeDir}",
		"%{Dependencies.spdlog.IncludeDir}",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
	}

	links
	{
		"%{Dependencies.Pulse.LibName}",
	}

	--------------------------------------
	-- Platforms
	--------------------------------------
	filter "system:windows"
		defines "DY_PLATFORM_WINDOWS"
		systemversion "latest"
		staticruntime "on"

	filter "system:linux"
		defines "DY_PLATFORM_LINUX"
		systemversion "latest"
		staticruntime "on"

    filter "system:macosx"
		defines "DY_PLATFORM_MACOS"
		systemversion(MacOSVersion)
		staticruntime "on"

	filter "action:xcode*"
		-- Note: XCode needs the full pch header path
		pchheader "src/Dynamite/dypch.h"

		-- Note: If we don't add the header files to the externalincludedirs
		-- we can't use <angled> brackets to include files.
		externalincludedirs
		{
			"src",
			"src/Dynamite",

			"%{Dependencies.Pulse.IncludeDir}",
			"%{Dependencies.spdlog.IncludeDir}",
		}

	--------------------------------------
	-- Configurations
	--------------------------------------
	filter "configurations:Debug"
		defines "DY_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DY_CONFIG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DY_CONFIG_DIST"
		runtime "Release"
		optimize "Full"
