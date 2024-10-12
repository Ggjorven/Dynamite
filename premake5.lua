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
	LLVM = 
	{
		IncludeDir = "%{wks.location}/vendor/llvm/LLVM/include",
		LibName = "LLVM"
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
	include "vendor/llvm"
group ""

include "Dynamite"
------------------------------------------------------------------------------
