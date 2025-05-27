workspace "Alkaline"
   architecture "x64"
   startproject "Alkaline"
   location "build/projects"
   configurations { "Debug", "Release" }

-- outputpath = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
outputpath = "%{cfg.buildcfg}"
targetpath = "bin/" .. outputpath .. "/%{prj.name}"
objpath = "bin-int/" .. outputpath .. "/%{prj.name}"

-- ImGui + rlImGui static library
project "imgui_rl"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"

   targetdir(targetpath)
   objdir(objpath)

   files {
      "external/imgui/*.cpp",
      "external/imgui/*.h",
      "external/rlImGui/rlImGui.cpp",
      "external/rlImGui/rlImGui.h"
   }

   includedirs {
      "external",
      "external/imgui",
      "external/rlImGui",
      "external/raylib",
      "external/sol"
   }

   filter "system:windows"
      systemversion "latest"

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      runtime "Release"
      optimize "on"

-- Main Project
project "Alkaline"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"

   targetdir(targetpath)
   objdir(objpath)

   files {
      "src/**.cpp",
      "external/tracy/public/TracyClient.cpp"
   }

   includedirs {
      "src",
      "external",
      "external/imgui",
      "external/rlImGui",
      "external/raylib",
      "external/sol",
      "external/lua",
      "external/tracy/public/tracy"
   }

   links {
      "imgui_rl",
      "raylib",
      "lua54",
      "winmm"
   }

   libdirs {
      "lib"
   }

   filter "system:windows"
      systemversion "latest"
      staticruntime "on"  -- /MDd
      linkoptions { "/NODEFAULTLIB:LIBCMT" }

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"
      defines { "DEBUG_BUILD", "TRACY_ENABLE" }

   filter "configurations:Release"
      runtime "Release"
      optimize "on"
