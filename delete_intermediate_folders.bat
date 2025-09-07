@echo off
setlocal

:: Clean Unreal Engine Project Cache Script
:: Deletes Intermediate, Binaries, and DerivedDataCache folders
:: Run this script from your project root directory

echo ================================================
echo    Unreal Engine Project Cache Cleaner
echo ================================================
echo.

:: Check if we're in a project directory
if not exist "*.uproject" (
    echo ERROR: No .uproject file found in current directory
    echo Please run this script from your Unreal project root folder
    echo.
    pause
    exit /b 1
)

:: Get the project name
for %%f in (*.uproject) do set PROJECT_NAME=%%~nf
echo Found project: %PROJECT_NAME%
echo Current directory: %CD%
echo.

:: Delete Intermediate folder
echo Checking for Intermediate folder...
if exist "Intermediate" (
    echo Deleting Intermediate folder...
    rmdir /s /q "Intermediate" 2>nul
    if exist "Intermediate" (
        echo WARNING: Could not delete Intermediate - folder may be in use
    ) else (
        echo SUCCESS: Intermediate deleted
    )
) else (
    echo Intermediate folder not found - skipping
)
echo.

:: Delete Binaries folder
echo Checking for Binaries folder...
if exist "Binaries" (
    echo Deleting Binaries folder...
    rmdir /s /q "Binaries" 2>nul
    if exist "Binaries" (
        echo WARNING: Could not delete Binaries - folder may be in use
    ) else (
        echo SUCCESS: Binaries deleted
    )
) else (
    echo Binaries folder not found - skipping
)
echo.

:: Delete DerivedDataCache folder
echo Checking for DerivedDataCache folder...
if exist "DerivedDataCache" (
    echo Deleting DerivedDataCache folder...
    rmdir /s /q "DerivedDataCache" 2>nul
    if exist "DerivedDataCache" (
        echo WARNING: Could not delete DerivedDataCache - folder may be in use
    ) else (
        echo SUCCESS: DerivedDataCache deleted
    )
) else (
    echo DerivedDataCache folder not found - skipping
)
echo.

pause