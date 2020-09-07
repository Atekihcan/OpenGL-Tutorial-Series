@ECHO OFF

:: Get current directory
set CURRENT_DIR=%CD%

:: Set build and install directories
set BUILD_DIR=%CURRENT_DIR%\_build_windows
set INSTALL_DIR=%CURRENT_DIR%\_out

:: Set build configurations
set BUILD_TYPE=Debug

:: Select CMake compiler
set CMAKE_GENERATOR="Visual Studio 16 2019"

:: If installation directory doesn't exist, create it
if not exist %INSTALL_DIR% (
    mkdir %INSTALL_DIR%
)

:: If build directory doesn't exist, create and issue CMake generator command
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
    cmake -H. -B%BUILD_DIR% -G%CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX:PATH=%INSTALL_DIR%
)

:: Build and install
cmake --build %BUILD_DIR% --config %BUILD_TYPE%
cmake --install %BUILD_DIR% --config %BUILD_TYPE%
