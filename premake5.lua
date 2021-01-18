outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
includeDir = {}
includeDir["spdlog"] = "Lynx/vendor/spdlog/include"

workspace "Lynx"
    architecture "x64"
    language "C++"
    cppdialect "C++17"
    configurations {"Debug", "Release"}
    startproject "Sandbox"

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"

    filter {}

    targetdir ("build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("build/obj/" .. outputDir .. "/%{prj.name}")

project "Lynx"
    location "Lynx"
    kind "StaticLib"
    staticruntime "on"

    files "Lynx/src/**"

    includes = {
        "Lynx/src",
        "%{includeDir.spdlog}"
    }

    includedirs {includes}

    function useLynx()
        includedirs {includes}
        links "Lynx"
    end

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    staticruntime "on"

    files "Sandbox/src/**"
 
    useLynx()

    filter "system:windows"
        systemversion "latest"
        defines {
            "_CRT_SECURE_NO_WARNINGS",
        }