call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

cmake ^
    -S . ^
    -B build/Win32 ^
    -G "Visual Studio 17 2022" ^
    -A Win32 ^
    -T host=x64 ^
    -DCMAKE_SYSTEM_VERSION="10.0.19041.0"
cmake ^
    --build build/Win32 ^
    --config Debug
cmake ^
    --build build/Win32 ^
    --config RelWithDebInfo

cmake ^
    -S . ^
    -B build/x64 ^
    -G "Visual Studio 17 2022" ^
    -A x64 ^
    -T host=x64 ^
    -DCMAKE_SYSTEM_VERSION="10.0.19041.0"
cmake ^
    --build build/x64 ^
    --config Debug
cmake ^
    --build build/x64 ^
    --config RelWithDebInfo

if not exist pkg (
    mkdir pkg
)
nuget pack src\RenderLibrary\RenderLibrary.nuspec ^
    -OutputDirectory pkg
