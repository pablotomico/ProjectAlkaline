@echo off
setlocal enabledelayedexpansion

REM Default values
set CONFIG=Release
set DO_CLEAN=false

REM Parse arguments
:parse_args
if "%~1"=="" goto end_args

if "%~1"=="-d" (
    set CONFIG=Debug
)
if "%~1"=="-c" (
    set DO_CLEAN=true
)

shift
goto parse_args

:end_args

echo Configuration: %CONFIG%
echo Clean build: %DO_CLEAN%

REM Run premake
build\vendor\premake\premake5.exe vs2022 --file=premake5.lua

REM Optionally clean
if "%DO_CLEAN%"=="true" (
    echo Running clean...
    msbuild ./build/projects/Alkaline.sln /t:Clean /p:Configuration=%CONFIG%
)

REM Build
echo Building...
msbuild ./build/projects/Alkaline.sln /m /p:Configuration=%CONFIG%