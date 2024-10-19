MacOSVersion = MacOSVersion or "14.5"

project "Pulse"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "Off" -- For LLVM

	architecture "x86_64"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	-- Note: VS2022/Make only need the pchheader filename
	pchheader "plpch.h"
	pchsource "Pulse/Pulse/src/Pulse/plpch.cpp"

	files
	{
		"Pulse/Pulse/src/Pulse/**.h",
		"Pulse/Pulse/src/Pulse/**.hpp",
		"Pulse/Pulse/src/Pulse/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"Pulse/Pulse/src",
		"Pulse/Pulse/src/Pulse"
	}

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		systemversion "latest"

	filter "system:macosx"
		systemversion(MacOSVersion)

		-- Note: XCode only needs the full pchheader path
		pchheader "Pulse/Pulse/src/Pulse/plpch.h"

		-- Note: If we don't add the header files to the externalincludedirs
		-- we can't use <angled> brackets to include files.
		externalincludedirs
		{
			"Pulse/Pulse/src",
			"Pulse/Pulse/src/Pulse"
		}

	filter "configurations:Debug"
		runtime "Release" -- For LLVM's binaries
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
