@echo off
REM OpenGameDevEngine Setup Script
REM Quick setup for first-time users

setlocal enabledelayedexpansion

echo.
echo ===============================================================
echo          OpenGameDevEngine - First Time Setup
echo          Windows Native DirectX Game Engine
echo ===============================================================
echo.
echo This script will help you set up the build environment.
echo.

REM Check CMake
echo Checking for CMake...
cmake --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake not found!
    echo.
    echo Please install CMake 3.15 or higher from:
    echo https://cmake.org/download/
    echo.
    echo Make sure to add CMake to your system PATH during installation.
    echo.
    pause
    exit /b 1
) else (
    echo [OK] CMake is installed
)

REM Check for Visual Studio
echo.
echo Checking for Visual Studio...
set "VS_FOUND=0"
for %%V in (2022 2019 2017) do (
    if exist "C:\Program Files\Microsoft Visual Studio\%%V" (
        echo [OK] Visual Studio %%V found
        set "VS_FOUND=1"
        goto :VSCheckDone
    )
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\%%V" (
        echo [OK] Visual Studio %%V found
        set "VS_FOUND=1"
        goto :VSCheckDone
    )
)
:VSCheckDone

if "%VS_FOUND%"=="0" (
    echo [WARNING] Visual Studio not detected
    echo.
    echo Please install Visual Studio 2017 or later with:
    echo   - Desktop development with C++
    echo   - Windows SDK
    echo.
    echo Download from:
    echo https://visualstudio.microsoft.com/downloads/
    echo.
)

REM Check Git
echo.
echo Checking for Git...
git --version >nul 2>&1
if errorlevel 1 (
    echo [WARNING] Git not found (optional)
    echo.
    echo Git is optional but recommended. Download from:
    echo https://git-scm.com/download/win
    echo.
) else (
    echo [OK] Git is installed
)

REM Summary
echo.
echo ===============================================================
echo Setup Check Complete
echo ===============================================================
echo.

if "%VS_FOUND%"=="0" (
    echo [!] You need to install Visual Studio before you can build.
    echo.
    pause
    exit /b 1
)

echo All required tools are installed!
echo.
echo Next steps:
echo   1. Run: scripts\build.bat
echo   2. Wait for the build to complete
echo   3. Run: build\bin\Release\BasicExample.exe
echo.
echo For more options, run: scripts\build.bat --help
echo.

pause
exit /b 0
