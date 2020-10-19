workspace "ArcaneArts"
	architecture "x64"
	startproject "Game"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}";

IncludeDir = {}

group ""
	include "Engine"
	include "Game"
