local function prequire(m) 
    local ok, err = pcall(require, m) 
    if not ok then return nil, err end
    return err
end

local config = prequire "premake_config"
if not config then
    print("Error: could not find premake_config.lua. See README.md for setup information.")
    os.exit()
end
print("Loaded premake_config module")

workspace "Spellcook"
    configurations { "Debug", "Release" }
    platforms { "Win32", "Win64" }
    location "build"
    startproject "Spellcook"

project "Spellcook"
    kind "ConsoleApp"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { config.includedirs, "Source/" }
    links { "Core", "Encosys", "Game", "Render" }

    files { "Source/Spellcook/**.h", "Source/Spellcook/**.cpp" }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG" }

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"
        syslibdirs { config.syslibdirs32 }

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x64"
        syslibdirs { config.syslibdirs64 }

project "Game"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { config.includedirs, "Source/" }
    links { "Core" }

    files { "Source/Game/**.h", "Source/Game/**.cpp" }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG" }

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"
        syslibdirs { config.syslibdirs32 }

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x64"
        syslibdirs { config.syslibdirs64 }

project "Render"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { config.includedirs, "Source/" }
    links { "Core" }

    files { "Source/Render/**.h", "Source/Render/**.cpp" }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG", "SFML_STATIC" }
        links { "sfml-graphics-s-d", "sfml-window-s-d", "sfml-system-s-d", "opengl32", "freetype", "jpeg", "winmm", "gdi32" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG", "SFML_STATIC" }
        links { "sfml-graphics-s", "sfml-window-s", "sfml-system-s", "opengl32", "freetype", "jpeg", "winmm", "gdi32" }

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"
        syslibdirs { config.syslibdirs32 }

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x64"
        syslibdirs { config.syslibdirs64 }

project "Math"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { config.includedirs, "Source/" }
    links { "Core" }

    files { "Source/Math/**.h", "Source/Math/**.cpp" }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG" }

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"
        syslibdirs { config.syslibdirs32 }

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x64"
        syslibdirs { config.syslibdirs64 }

project "Core"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { config.includedirs, "Source/" }

    files { "Source/Core/**.h", "Source/Core/**.cpp" }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG" }

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"
        syslibdirs { config.syslibdirs32 }

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x64"
        syslibdirs { config.syslibdirs64 }

project "Encosys"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"

    files { "Source/Encosys/**.h", "Source/Encosys/**.cpp" }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG" }

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x32"

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x64"
