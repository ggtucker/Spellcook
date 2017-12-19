function IncludeGlfw()
    includedirs { "Libraries/glfw/Include" }
end
function LinkGlfw()
    filter { "platforms:Win32" }
        syslibdirs { "Libraries/glfw/Lib/x86" }
    filter { "platforms:Win64" }
        syslibdirs { "Libraries/glfw/Lib/x64" }

    filter { "kind:not StaticLib" }
        links { "glfw3" }
    filter {}
end

function UseRenderLib()
    links "Render"
    LinkGlfw()
end

workspace "Spellcook"
    configurations { "Debug", "Release" }
    platforms { "Win32", "Win64" }
    location "build"
    startproject "Spellcook"

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

    filter { "system:windows" }
        links { "OpenGL32" }
    filter { "system:not windows" }
        links { "GL" }

    filter {}

project "Spellcook"
    kind "ConsoleApp"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { "Source" }
    links { "Core", "Encosys", "Game" }
    files { "Source/Spellcook/**.h", "Source/Spellcook/**.cpp" }
    UseRenderLib()

project "Game"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { "Source" }
    links { "Core" }
    files { "Source/Game/**.h", "Source/Game/**.cpp" }

project "Render"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { "Source", "Libraries/glad/Include" }
    links { "Core" }
    files { "Source/Render/**.h", "Source/Render/**.cpp", "Libraries/glad/Source/glad.c" }
    IncludeGlfw()

project "Math"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { "Source" }
    links { "Core" }
    files { "Source/Math/**.h", "Source/Math/**.cpp" }

project "Core"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    includedirs { "Source" }
    files { "Source/Core/**.h", "Source/Core/**.cpp" }

project "Encosys"
    kind "StaticLib"
    language "C++"
    location "build/"
    targetdir "build/%{cfg.buildcfg}"
    files { "Source/Encosys/**.h", "Source/Encosys/**.cpp" }
