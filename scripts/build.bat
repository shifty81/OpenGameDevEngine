@echo off
REM OpenGameDevEngine Build Script for Windows
REM This script automates the build process with extensive logging

setlocal enabledelayedexpansion

REM ============================================================================
REM Configuration
REM ============================================================================
set BUILD_TYPE=Release
set CMAKE_GENERATOR=Visual Studio 17 2022
set SCRIPT_DIR=%~dp0
set ROOT_DIR=%SCRIPT_DIR%..
set BUILD_DIR=%ROOT_DIR%\build
set LOG_DIR=%ROOT_DIR%\logs
set TIMESTAMP=%date:~10,4%-%date:~4,2%-%date:~7,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%
set TIMESTAMP=%TIMESTAMP: =0%
set LOG_FILE=%LOG_DIR%\build_%TIMESTAMP%.log

REM Create logs directory if it doesn't exist
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"

REM ============================================================================
REM Banner
REM ============================================================================
echo.
echo ===============================================================
echo          OpenGameDevEngine Build System
echo          Windows Native DirectX Game Engine
echo ===============================================================
echo.

call :Log "Build script started"

REM ============================================================================
REM Check Prerequisites
REM ============================================================================
call :Log "Checking prerequisites..."

REM Check CMake
cmake --version >nul 2>&1
if errorlevel 1 (
    call :LogError "CMake not found! Please install CMake 3.15 or higher."
    call :LogError "Download from: https://cmake.org/download/"
    goto :Error
)
call :LogSuccess "CMake found"

REM Check for Visual Studio
set "VS_PATH="
for %%V in (2022 2019 2017) do (
    if exist "C:\Program Files\Microsoft Visual Studio\%%V" (
        set "VS_PATH=C:\Program Files\Microsoft Visual Studio\%%V"
        call :LogSuccess "Visual Studio %%V found"
        goto :VSFound
    )
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\%%V" (
        set "VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\%%V"
        call :LogSuccess "Visual Studio %%V found"
        goto :VSFound
    )
)
call :LogWarning "Visual Studio not detected. Make sure you have MSVC installed."
:VSFound

REM Check Git
git --version >nul 2>&1
if errorlevel 1 (
    call :LogWarning "Git not found. Some features may not work."
) else (
    call :LogSuccess "Git found"
)

call :Log "Prerequisites check complete"

REM ============================================================================
REM Parse Command Line Arguments
REM ============================================================================
:ParseArgs
if "%~1"=="" goto :DoneParsing
if /i "%~1"=="--clean" (
    set CLEAN_BUILD=1
    shift
    goto :ParseArgs
)
if /i "%~1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto :ParseArgs
)
if /i "%~1"=="--release" (
    set BUILD_TYPE=Release
    shift
    goto :ParseArgs
)
if /i "%~1"=="--run-examples" (
    set RUN_EXAMPLES=1
    shift
    goto :ParseArgs
)
if /i "%~1"=="--run-tests" (
    set RUN_TESTS=1
    shift
    goto :ParseArgs
)
if /i "%~1"=="--help" (
    call :ShowHelp
    exit /b 0
)
shift
goto :ParseArgs
:DoneParsing

REM ============================================================================
REM Clean Build Directory (if requested)
REM ============================================================================
if defined CLEAN_BUILD (
    call :Log "Cleaning build directory..."
    if exist "%BUILD_DIR%" (
        rmdir /s /q "%BUILD_DIR%"
        call :LogSuccess "Build directory cleaned"
    )
)

REM ============================================================================
REM Configure CMake
REM ============================================================================
call :Log "Configuring CMake..."
call :Log "Build Type: %BUILD_TYPE%"
call :Log "Generator: %CMAKE_GENERATOR%"

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cd /d "%BUILD_DIR%"

call :Log "Running CMake configuration..."
cmake .. ^
    -G "%CMAKE_GENERATOR%" ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DOGDE_BUILD_EXAMPLES=ON ^
    -DOGDE_BUILD_TESTS=ON ^
    -DOGDE_BUILD_TOOLS=ON ^
    >> "%LOG_FILE%" 2>&1

if errorlevel 1 (
    call :LogError "CMake configuration failed!"
    cd /d "%SCRIPT_DIR%"
    goto :Error
)

call :LogSuccess "CMake configuration complete"

REM ============================================================================
REM Build Project
REM ============================================================================
call :Log "Building project..."
call :Log "This may take several minutes..."

cmake --build . --config %BUILD_TYPE% >> "%LOG_FILE%" 2>&1

if errorlevel 1 (
    call :LogError "Build failed!"
    cd /d "%SCRIPT_DIR%"
    goto :Error
)

call :LogSuccess "Build complete"
cd /d "%SCRIPT_DIR%"

REM ============================================================================
REM Run Examples (if requested)
REM ============================================================================
if defined RUN_EXAMPLES (
    call :Log "Running examples..."
    
    set "EXAMPLES_DIR=%BUILD_DIR%\bin\%BUILD_TYPE%"
    if not exist "!EXAMPLES_DIR!" set "EXAMPLES_DIR=%BUILD_DIR%\bin"
    
    if exist "!EXAMPLES_DIR!" (
        for %%F in ("!EXAMPLES_DIR!\*Example.exe") do (
            call :Log "Running: %%~nxF"
            "%%F"
        )
    ) else (
        call :LogWarning "Examples directory not found"
    )
)

REM ============================================================================
REM Run Tests (if requested)
REM ============================================================================
if defined RUN_TESTS (
    call :Log "Running tests..."
    cd /d "%BUILD_DIR%"
    ctest -C %BUILD_TYPE% --output-on-failure >> "%LOG_FILE%" 2>&1
    if errorlevel 1 (
        call :LogWarning "Some tests failed"
    ) else (
        call :LogSuccess "All tests passed"
    )
    cd /d "%SCRIPT_DIR%"
)

REM ============================================================================
REM Build Summary
REM ============================================================================
call :Log "============================================================"
call :Log "Build Summary:"
call :Log "  Build Type: %BUILD_TYPE%"
call :Log "  Build Directory: %BUILD_DIR%"
call :Log "  Log File: %LOG_FILE%"

REM Count executables
set EXE_COUNT=0
if exist "%BUILD_DIR%\bin" (
    for /r "%BUILD_DIR%\bin" %%F in (*.exe) do set /a EXE_COUNT+=1
)
call :Log "  Executables built: %EXE_COUNT%"

call :Log "============================================================"
call :LogSuccess "Build completed successfully!"
echo.
call :Log "To run the basic example:"
call :LogSuccess "  %BUILD_DIR%\bin\%BUILD_TYPE%\BasicExample.exe"
echo.
call :Log "Build script completed successfully"

exit /b 0

REM ============================================================================
REM Functions
REM ============================================================================

:Log
echo [%date% %time%] [INFO] %~1
echo [%date% %time%] [INFO] %~1 >> "%LOG_FILE%"
exit /b 0

:LogSuccess
echo [%date% %time%] [SUCCESS] %~1
echo [%date% %time%] [SUCCESS] %~1 >> "%LOG_FILE%"
exit /b 0

:LogWarning
echo [%date% %time%] [WARNING] %~1
echo [%date% %time%] [WARNING] %~1 >> "%LOG_FILE%"
exit /b 0

:LogError
echo [%date% %time%] [ERROR] %~1
echo [%date% %time%] [ERROR] %~1 >> "%LOG_FILE%"
exit /b 0

:ShowHelp
echo Usage: build.bat [options]
echo.
echo Options:
echo   --clean         Clean build directory before building
echo   --debug         Build in Debug mode (default: Release)
echo   --release       Build in Release mode
echo   --run-examples  Run example programs after building
echo   --run-tests     Run tests after building
echo   --help          Show this help message
echo.
echo Examples:
echo   build.bat                          Build in Release mode
echo   build.bat --clean --debug          Clean build in Debug mode
echo   build.bat --release --run-examples Build and run examples
exit /b 0

:Error
call :LogError "Build failed! Check the log file for details:"
call :LogError "  %LOG_FILE%"
echo.
pause
exit /b 1
