outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
includeDir = {}
includeDir["spdlog"] = "Lynx/vendor/spdlog/include"
includeDir["glfw"] = "Lynx/vendor/GLFW/include"
includeDir["glad"] = "Lynx/vendor/GLAD/include"
includeDir["glm"] = "Lynx/vendor/glm"

workspace "Lynx"
    architecture "x64"
    language "C++"
    cppdialect "C++17"
    configurations {"Debug", "Release"}
    startproject "Sandbox"

    targetdir ("build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("build/obj/" .. outputDir .. "/%{prj.name}")

    group "Dependencies"
        include "Lynx/vendor/GLFW"
        include "Lynx/vendor/GLAD"
    group ""

project "Lynx"
    location "Lynx"
    kind "StaticLib"
    staticruntime "on"

    pchheader "Lynxpch.h"
    pchsource "Lynx/src/Lynxpch.cpp"
    --forceincludes  "Lynxpch.h"

    files {
        "Lynx/src/**",
        "%{prj.name}/vendor/glm//**.hpp",
		"%{prj.name}/vendor/glm//**.inl"
    }

    includedirs {
        "Lynx/src",
        "%{includeDir.spdlog}",
        "%{includeDir.glfw}",
        "%{includeDir.glad}",
        "%{includeDir.glm}"
    }

    links {
        "GLFW",
        "GLAD",
        "opengl32"
    }

    function useLynx()
        includedirs {
            "Lynx/src",
            "%{includeDir.spdlog}",
            "%{includeDir.glm}",
        }
        links "Lynx"
    end

    filter "system:windows"
    systemversion "latest"
    defines {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "configurations:Debug"
        defines "LX_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LX_RELEASE"
        runtime "Release"
        optimize "on"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    staticruntime "on"

    files "Sandbox/src/**"
 
    useLynx()
    
    filter "configurations:Debug"
        defines "LX_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LX_RELEASE"
        runtime "Release"
        optimize "on"
