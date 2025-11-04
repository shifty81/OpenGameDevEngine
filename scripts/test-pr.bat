@echo off
REM OpenGameDevEngine PR Testing Script for Windows
REM This script automates pulling and testing pull requests

setlocal enabledelayedexpansion

REM ============================================================================
REM Configuration
REM ============================================================================
set SCRIPT_DIR=%~dp0
set ROOT_DIR=%SCRIPT_DIR%..
set LOG_DIR=%ROOT_DIR%\logs
set TIMESTAMP=%date:~10,4%-%date:~4,2%-%date:~7,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%
set TIMESTAMP=%TIMESTAMP: =0%
set LOG_FILE=%LOG_DIR%\test-pr_%TIMESTAMP%.log
set MODE=in-place
set PR_NUMBER=
set AUTO_BUILD=1
set WORKSPACE_DIR=%ROOT_DIR%\pr-workspace

REM Create logs directory if it doesn't exist
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"

REM ============================================================================
REM Banner
REM ============================================================================
echo.
echo ===============================================================
echo          OpenGameDevEngine PR Testing Tool
echo          Automated Pull Request Testing
echo ===============================================================
echo.

call :Log "PR testing script started"

REM ============================================================================
REM Check Prerequisites
REM ============================================================================
call :Log "Checking prerequisites..."

REM Check Git
git --version >nul 2>&1
if errorlevel 1 (
    call :LogError "Git not found! Please install Git."
    call :LogError "Download from: https://git-scm.com/download/win"
    goto :Error
)
call :LogSuccess "Git found"

REM Check if we're in a git repository
cd /d "%ROOT_DIR%"
git rev-parse --git-dir >nul 2>&1
if errorlevel 1 (
    call :LogError "Not in a git repository!"
    goto :Error
)
call :LogSuccess "Git repository detected"

REM Get remote URL
for /f "tokens=*" %%i in ('git remote get-url origin 2^>nul') do set REMOTE_URL=%%i
if "%REMOTE_URL%"=="" (
    call :LogError "No git remote 'origin' found!"
    goto :Error
)
call :Log "Repository: %REMOTE_URL%"

REM ============================================================================
REM Parse Command Line Arguments
REM ============================================================================
:ParseArgs
if "%~1"=="" goto :DoneParsing
if /i "%~1"=="--multi-version" (
    set MODE=multi-version
    shift
    goto :ParseArgs
)
if /i "%~1"=="--in-place" (
    set MODE=in-place
    shift
    goto :ParseArgs
)
if /i "%~1"=="--pr" (
    if "%~2"=="" (
        call :LogError "--pr requires a PR number argument"
        goto :Error
    )
    set PR_NUMBER=%~2
    shift
    shift
    goto :ParseArgs
)
if /i "%~1"=="--no-build" (
    set AUTO_BUILD=0
    shift
    goto :ParseArgs
)
if /i "%~1"=="--workspace" (
    if "%~2"=="" (
        call :LogError "--workspace requires a directory path argument"
        goto :Error
    )
    set WORKSPACE_DIR=%~2
    shift
    shift
    goto :ParseArgs
)
if /i "%~1"=="--help" (
    call :ShowHelp
    exit /b 0
)
call :LogWarning "Unknown option: %~1"
shift
goto :ParseArgs
:DoneParsing

REM ============================================================================
REM Validate PR Number or Get Next PR
REM ============================================================================
if "%PR_NUMBER%"=="" (
    call :Log "Fetching next open PR..."
    
    REM Try to get PR list using gh CLI if available
    gh --version >nul 2>&1
    if not errorlevel 1 (
        for /f "tokens=1" %%i in ('gh pr list --state open --limit 1 --json number --jq ".[0].number" 2^>nul') do set PR_NUMBER=%%i
    )
    
    if "!PR_NUMBER!"=="" (
        call :LogError "Could not automatically fetch PR number."
        call :LogError "Please specify a PR number using --pr <number>"
        call :LogError "Or install GitHub CLI (gh) for automatic PR detection."
        goto :Error
    )
    call :LogSuccess "Found PR #!PR_NUMBER!"
) else (
    call :Log "Using specified PR #%PR_NUMBER%"
)

REM ============================================================================
REM Execute Based on Mode
REM ============================================================================
if "%MODE%"=="in-place" (
    call :InPlaceMode
) else (
    call :MultiVersionMode
)

if errorlevel 1 goto :Error

REM ============================================================================
REM Build the Project (if requested)
REM ============================================================================
if "%AUTO_BUILD%"=="1" (
    call :Log "Building project..."
    
    if "%MODE%"=="multi-version" (
        set "BUILD_ROOT=%WORKSPACE_DIR%\pr-%PR_NUMBER%"
    ) else (
        set "BUILD_ROOT=%ROOT_DIR%"
    )
    
    call :Log "Build directory: !BUILD_ROOT!"
    
    cd /d "!BUILD_ROOT!"
    
    REM Create and clean build directory
    if exist "build" (
        call :Log "Cleaning existing build directory..."
        rmdir /s /q "build"
    )
    mkdir build
    cd build
    
    REM Run CMake configuration
    call :Log "Running CMake..."
    cmake .. >> "%LOG_FILE%" 2>&1
    if errorlevel 1 (
        call :LogWarning "CMake configuration failed! Check log for details."
    ) else (
        REM Build the project
        call :Log "Building project..."
        cmake --build . --config Release >> "%LOG_FILE%" 2>&1
        if errorlevel 1 (
            call :LogWarning "Build failed! Check log for details."
        ) else (
            call :LogSuccess "Build completed successfully"
        )
    )
    
    cd /d "%SCRIPT_DIR%"
)

REM ============================================================================
REM Success Summary
REM ============================================================================
call :Log "============================================================"
call :LogSuccess "PR #%PR_NUMBER% ready for testing!"
call :Log "============================================================"

if "%MODE%"=="multi-version" (
    call :Log "PR Location: %WORKSPACE_DIR%\pr-%PR_NUMBER%"
    echo.
    call :Log "To test the PR, navigate to:"
    call :LogSuccess "  cd %WORKSPACE_DIR%\pr-%PR_NUMBER%"
) else (
    call :Log "PR checked out in current directory"
)

echo.
call :Log "To test another PR, run:"
call :LogSuccess "  scripts\test-pr.bat --pr <number>"
echo.

call :Log "PR testing script completed successfully"
exit /b 0

REM ============================================================================
REM Functions
REM ============================================================================

:InPlaceMode
call :Log "Mode: In-Place (updating current directory)"
call :Log "============================================================"

REM Save current branch
for /f "tokens=*" %%i in ('git rev-parse --abbrev-ref HEAD 2^>nul') do set CURRENT_BRANCH=%%i
call :Log "Current branch: %CURRENT_BRANCH%"

REM Check for uncommitted changes
git diff-index --quiet HEAD -- >nul 2>&1
if errorlevel 1 (
    call :LogWarning "You have uncommitted changes!"
    set /p CONTINUE="Continue and discard changes? (y/N): "
    if /i not "!CONTINUE!"=="y" (
        call :Log "Cancelled by user"
        exit /b 1
    )
    call :Log "Discarding changes..."
    git reset --hard HEAD
)

REM Fetch latest changes
call :Log "Fetching latest changes from remote..."
git fetch origin >> "%LOG_FILE%" 2>&1
if errorlevel 1 (
    call :LogError "Failed to fetch from remote"
    exit /b 1
)

REM Checkout the PR
call :Log "Checking out PR #%PR_NUMBER%..."
git fetch origin pull/%PR_NUMBER%/head:pr-%PR_NUMBER% >> "%LOG_FILE%" 2>&1
if errorlevel 1 (
    call :LogError "Failed to fetch PR #%PR_NUMBER%"
    call :LogError "Make sure the PR number is valid"
    exit /b 1
)

git checkout pr-%PR_NUMBER% >> "%LOG_FILE%" 2>&1
if errorlevel 1 (
    call :LogError "Failed to checkout PR #%PR_NUMBER%"
    exit /b 1
)

call :LogSuccess "Successfully checked out PR #%PR_NUMBER%"
exit /b 0

:MultiVersionMode
call :Log "Mode: Multi-Version (creating separate directory)"
call :Log "============================================================"

REM Create workspace directory if it doesn't exist
if not exist "%WORKSPACE_DIR%" (
    call :Log "Creating workspace directory: %WORKSPACE_DIR%"
    mkdir "%WORKSPACE_DIR%"
)

set "PR_DIR=%WORKSPACE_DIR%\pr-%PR_NUMBER%"

REM Check if PR directory already exists
if exist "%PR_DIR%" (
    call :LogWarning "Directory already exists: %PR_DIR%"
    set /p OVERWRITE="Overwrite existing directory? (y/N): "
    if /i not "!OVERWRITE!"=="y" (
        call :Log "Using existing directory"
        exit /b 0
    )
    call :Log "Removing existing directory..."
    rmdir /s /q "%PR_DIR%"
)

REM Clone the repository
call :Log "Cloning repository to %PR_DIR%..."
git clone "%REMOTE_URL%" "%PR_DIR%" >> "%LOG_FILE%" 2>&1
if errorlevel 1 (
    call :LogError "Failed to clone repository"
    exit /b 1
)

REM Fetch and checkout the PR
cd /d "%PR_DIR%"
call :Log "Fetching PR #%PR_NUMBER%..."
git fetch origin pull/%PR_NUMBER%/head:pr-%PR_NUMBER% >> "%LOG_FILE%" 2>&1
if errorlevel 1 (
    call :LogError "Failed to fetch PR #%PR_NUMBER%"
    call :LogError "Make sure the PR number is valid"
    exit /b 1
)

git checkout pr-%PR_NUMBER% >> "%LOG_FILE%" 2>&1
if errorlevel 1 (
    call :LogError "Failed to checkout PR #%PR_NUMBER%"
    exit /b 1
)

call :LogSuccess "Successfully created PR workspace at %PR_DIR%"
cd /d "%SCRIPT_DIR%"
exit /b 0

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
echo Usage: test-pr.bat [options]
echo.
echo Description:
echo   Automate pulling and testing pull requests from the current repository.
echo.
echo Modes:
echo   --in-place        Update current directory with PR (default)
echo   --multi-version   Create separate directory for each PR
echo.
echo Options:
echo   --pr NUMBER       Specify PR number to test (required if gh CLI not installed)
echo   --no-build        Skip automatic build after checkout
echo   --workspace PATH  Custom workspace directory for multi-version mode
echo                     (default: ./pr-workspace)
echo   --help            Show this help message
echo.
echo Examples:
echo   test-pr.bat --pr 42
echo     Test PR #42 in current directory
echo.
echo   test-pr.bat --pr 42 --multi-version
echo     Test PR #42 in separate directory (./pr-workspace/pr-42)
echo.
echo   test-pr.bat --pr 42 --multi-version --no-build
echo     Checkout PR #42 without building
echo.
echo   test-pr.bat
echo     Automatically fetch and test next open PR (requires gh CLI)
echo.
echo Requirements:
echo   - Git must be installed
echo   - GitHub CLI (gh) is optional but recommended for auto-detection
echo.
echo Multi-Version Mode:
echo   Creates separate directories for each PR, allowing you to test
echo   multiple PRs side-by-side without switching branches:
echo     pr-workspace/
echo       pr-42/    (PR #42)
echo       pr-43/    (PR #43)
echo       pr-44/    (PR #44)
exit /b 0

:Error
call :LogError "PR testing failed! Check the log file for details:"
call :LogError "  %LOG_FILE%"
echo.
pause
exit /b 1
