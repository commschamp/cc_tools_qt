rem Input
rem BUILD_DIR - Main build directory
rem GENERATOR - CMake generator
rem PLATFORM - CMake generator platform
rem QTDIR - Path to Qt installation
rem EXTERNALS_DIR - (Optional) Directory where externals need to be located
rem COMMS_REPO - (Optional) Repository of the COMMS library
rem COMMS_TAG - (Optional) Tag of the COMMS library
rem COMMON_INSTALL_DIR - (Optional) Common directory to perform installations
rem COMMON_BUILD_TYPE - (Optional) CMake build type
rem COMMON_CXX_STANDARD - (Optional) CMake C++ standard

rem -----------------------------------------------------

if [%BUILD_DIR%] == [] echo "BUILD_DIR hasn't been specified" & exit /b 1

if NOT [%GENERATOR%] == [] set GENERATOR_PARAM=-G %GENERATOR%

if NOT [%PLATFORM%] == [] set PLATFORM_PARAM=-A %PLATFORM%

if [%EXTERNALS_DIR%] == [] set EXTERNALS_DIR=%BUILD_DIR%/externals

if [%COMMS_REPO%] == [] set COMMS_REPO="https://github.com/commschamp/comms.git"

if [%COMMS_TAG%] == [] set COMMS_TAG="master"

if [%COMMON_BUILD_TYPE%] == [] set COMMON_BUILD_TYPE=Debug

set COMMS_SRC_DIR=%EXTERNALS_DIR%/comms
set COMMS_BUILD_DIR=%BUILD_DIR%/externals/comms/build
set COMMS_INSTALL_DIR=%COMMS_BUILD_DIR%/install
if NOT [%COMMON_INSTALL_DIR%] == [] set COMMS_INSTALL_DIR=%COMMON_INSTALL_DIR%

rem ----------------------------------------------------

mkdir "%EXTERNALS_DIR%"
if exist %COMMS_SRC_DIR%/.git (
    echo "Updating COMMS library..."
    cd "%COMMS_SRC_DIR%"
    git fetch --all
    git checkout .
    git checkout %COMMS_TAG%
    git pull --all
    if %errorlevel% neq 0 exit /b %errorlevel%
) else (
    echo "Cloning COMMS library..."
    git clone -b %COMMS_TAG% %COMMS_REPO% %COMMS_SRC_DIR%
    if %errorlevel% neq 0 exit /b %errorlevel%
)

echo "Building COMMS library..."
mkdir "%COMMS_BUILD_DIR%"
cd %COMMS_BUILD_DIR%
cmake %GENERATOR_PARAM% %PLATFORM_PARAM% -S %COMMS_SRC_DIR% -B %COMMS_BUILD_DIR% ^
    -DCMAKE_INSTALL_PREFIX=%COMMS_INSTALL_DIR% -DCMAKE_BUILD_TYPE=%COMMON_BUILD_TYPE% ^
    -DCMAKE_CXX_STANDARD=%COMMON_CXX_STANDARD%
if %errorlevel% neq 0 exit /b %errorlevel%
cmake --build %COMMS_BUILD_DIR% --config %COMMON_BUILD_TYPE% --target install
if %errorlevel% neq 0 exit /b %errorlevel%


