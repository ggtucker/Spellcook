workspace "Spellcook"
    configurations { "Debug", "Release" }
    location "build"

project "Spellcook"
    kind "ConsoleApp"
    language "C++"
    location "build"
    targetdir "build/%{cfg.buildcfg}"

    files { "**.h", "**.cpp" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"