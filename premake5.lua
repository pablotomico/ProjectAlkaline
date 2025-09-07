---@diagnostic disable: undefined-global
workspace "Alkaline"
   architecture "x64"
   startproject "AlkalineClient"
   configurations { "Debug", "Release" }

   filter "action:vs*"
      buildoptions { "/MP" }

-- outputpath = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
outputpath = "%{cfg.buildcfg}"
targetpath = "bin/" .. outputpath .. "/%{prj.name}"
objpath = "bin-int/" .. outputpath .. "/%{prj.name}"

-- ImGui + rlImGui static library
group "Vendor"
project "imgui_rl"
   location "build/projects"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   -- staticruntime "on"

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
   }

   filter "system:windows"
      systemversion "latest"

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

   filter "configurations:Release"
      runtime "Release"
      optimize "on"


group "Core"
project "AlkalineCore"
   location "build/projects"
   -- location "AlkalineCore"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   -- staticruntime "on"

   targetdir(targetpath)
   objdir(objpath)

   files {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp",
      -- "external/tracy/public/TracyClient.cpp"
   }

   includedirs {
      "%{prj.name}/src",
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
      -- "raylib",
      "lua54",
      "winmm"
   }

   libdirs {
      "lib"
   }

   filter "system:windows"
      systemversion "latest"
      linkoptions { "/NODEFAULTLIB:LIBCMT" }

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"
      defines { "ALK_DEBUG" }
      -- buildoptions { "/Bt+", "/d2cgsummary" }

   filter "configurations:Release"
      runtime "Release"
      optimize "on"
      defines { "ALK_DEBUG" }

group "Game"
project "AlkalineGame"
   location "build/projects"
   -- location "AlkalineGame"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   -- staticruntime "on"

   targetdir(targetpath)
   objdir(objpath)

   files {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp",
      -- "external/tracy/public/TracyClient.cpp"
   }

   includedirs {
      "%{prj.name}/src",
      "AlkalineCore/src",
      "external",
      "external/imgui",
      "external/rlImGui",
      "external/raylib",
      "external/sol",
      "external/lua",
      "external/tracy/public/tracy"
   }

   links {
      "AlkalineCore",
   }

   filter "system:windows"
      systemversion "latest"
      -- linkoptions { "/NODEFAULTLIB:LIBCMT" }

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"
      defines { "ALK_DEBUG", "TRACY_ENABLE" }
      
   filter "configurations:Release"
      runtime "Release"
      optimize "on"
      defines { "ALK_DEBUG", "TRACY_ENABLE" }

group "Applications"
project "AlkalineClient"
   location "build/projects"
   -- location "AlkalineClient"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   -- staticruntime "off"

   targetdir(targetpath)
   objdir(objpath)

   files {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp",
      -- "external/tracy/public/TracyClient.cpp"
   }

   includedirs {
      "%{prj.name}/src",
      "AlkalineCore/src",
      "AlkalineGame/src",
      "external",
      "external/sol",
      "external/lua",
      "external/raylib",
   }

   links {
      "imgui_rl",
      "raylib",
      "AlkalineCore",
      "AlkalineGame",
   }

   libdirs {
      "lib"
   }

   filter "system:windows"
      systemversion "latest"
      linkoptions { "/NODEFAULTLIB:LIBCMT" }
      postbuildcommands { "{COPYDIR} %[AlkalineGame/config/] %[" .. targetpath .. "/]" }

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"
      linkoptions { "/WHOLEARCHIVE:AlkalineGame" }
      defines { "ALK_DEBUG", "TRACY_ENABLE" }
      
   filter "configurations:Release"
      runtime "Release"
      optimize "on"
      defines { "ALK_DEBUG", "TRACY_ENABLE" }

project "AlkalineEditor"
   location "build/projects"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   -- staticruntime "off"

   targetdir(targetpath)
   objdir(objpath)

   files {
      "%{prj.name}/src/**.h",
      "%{prj.name}/src/**.cpp",
      -- "external/tracy/public/TracyClient.cpp"
   }

   includedirs {
      "%{prj.name}/src",
      "AlkalineCore/src",
      "AlkalineGame/src",
      "external",
      "external/lua",
   }

   links {
      "imgui_rl",
      "raylib",
      "AlkalineCore",
      "AlkalineGame",
   }

   libdirs {
      "lib"
   }

   filter "system:windows"
      systemversion "latest"
      linkoptions { "/NODEFAULTLIB:LIBCMT" }
      postbuildcommands { "{COPYDIR} %[AlkalineGame/config/] %[" .. targetpath .. "/]" }

   filter "configurations:Debug"
      runtime "Debug"
      symbols "on"
      linkoptions { "/WHOLEARCHIVE:AlkalineGame" }
      defines { "ALK_DEBUG", "TRACY_ENABLE" }

   filter "configurations:Release"
      runtime "Release"
      linkoptions { "/WHOLEARCHIVE:AlkalineGame" }
      defines { "ALK_DEBUG", "TRACY_ENABLE" }

