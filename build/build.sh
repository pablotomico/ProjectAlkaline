#!/bin/bash

options="-std=c++20"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch"
libs="-Llib -lmsvcrt -llua54 -lraylib -lOpenGL32 -lGdi32 -lWinMM -lkernel32 -lshell32 -lUser32 -Xlinker /NODEFAULTLIB:libcmt"
includes="-Isrc/include/sol -Isrc/include/imgui -Isrc/include -Isrc/ -Iexternal/ -Iexternal/rlImGui"

clang++ $options -g src/main.cpp external/rlImGui/*.cpp external/imgui/*.cpp -o bin/alkaline.exe $warnings $includes $libs

if [ "$?" -ne 0 ] 
then
    echo "----------------------------------------------"
    echo "================ BUILD FAILED ================"
    echo "----------------------------------------------"
    exit 1
fi

echo "Compilation Succeeded"
echo "-----------------------------------------------"

mkdir -p ./docs/
rm -rf ./docs/*
echo "Generating XML Docs.."
doxygen.exe ./build/Doxyfile
echo "Successfully generated XML Docs"

echo "-----------------------------------------------"

mkdir -p ./scripts/Meta/
rm -rf ./scripts/Meta/*
echo "Generating Lua Globals.."
python ./build/GenerateLuaGlobals.py -quiet
echo "Successfully generated globals.lua"

echo "-----------------------------------------------"
echo "================ BUILD SUCCESS ================"
echo "-----------------------------------------------"

exit 0