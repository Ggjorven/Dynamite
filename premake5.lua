------------------------------------------------------------------------------
-- Dependencies
------------------------------------------------------------------------------
MacOSVersion = "14.5"

Dependencies = 
{
	-- Libraries
	Pulse = 
	{
		IncludeDir = "%{wks.location}/vendor/Pulse/Pulse/src",
		LibName = "Pulse"
	},

	-- Includedirs
	spdlog =
	{
		IncludeDir = "%{wks.location}/vendor/spdlog/include"
	}
}
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Solution
------------------------------------------------------------------------------
outputdir = "%{cfg.buildcfg}-%{cfg.system}"

workspace "Dynamite"
	architecture "x86_64"
	startproject "Dynamite"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

group "Dependencies"
	include "vendor/Pulse/Pulse"
group ""

include "Dynamite"
------------------------------------------------------------------------------
