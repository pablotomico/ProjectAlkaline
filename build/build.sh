#!/bin/bash

start_time=$(date +%s)
start_timestamp=$(date +"%Y-%m-%d %H:%M:%S")
echo "Build started at $start_timestamp"
echo "-----------------------------------------------"

options="-std=c++20 -DTRACY_ENABLE"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch -Wno-microsoft-cast"
libs="-Llib -lmsvcrt -llua54 -lraylib -lOpenGL32 -lGdi32 -lWinMM -lkernel32 -lshell32 -lUser32 -Xlinker /NODEFAULTLIB:libcmt -ldbghelp"
includes="-Isrc/include/sol -Isrc/include/imgui -Isrc/include -Isrc/ -Iexternal/ -Iexternal/rlImGui -Iexternal/tracy/public"

external="external/rlImGui/*.cpp \
        external/imgui/*.cpp \
        external/tracy/public/TracyClient.cpp \
        "
source="$(find src/ -name "*.cpp")"

clang++ -DDEBUG_BUILD=1 $options -g $source $external -o bin/alkaline.exe $warnings $includes $libs

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

end_time=$(date +%s)
end_timestamp=$(date +"%Y-%m-%d %H:%M:%S")
duration=$((end_time - start_time))

echo "Build finished at $end_timestamp"
echo "Total build time: ${duration} seconds"

exit 0