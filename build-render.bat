call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

msbuild ShaderPlayground.Render.sln ^
    -t:Restore ^
    -p:RestorePackagesConfig=true

msbuild ShaderPlayground.Render.sln ^
    -p:Configuration=Debug ^
    -p:Platform=x86 ^
    -maxCpuCount ^
    -nologo ^
    -v:minimal
msbuild ShaderPlayground.Render.sln ^
    -p:Configuration=Release ^
    -p:Platform=x86 ^
    -maxCpuCount ^
    -nologo ^
    -v:minimal
msbuild ShaderPlayground.Render.sln ^
    -p:Configuration=Debug ^
    -p:Platform=x64 ^
    -maxCpuCount ^
    -nologo ^
    -v:minimal
msbuild ShaderPlayground.Render.sln ^
    -p:Configuration=Release ^
    -p:Platform=x64 ^
    -maxCpuCount ^
    -nologo ^
    -v:minimal

if not exist pkg (
    mkdir pkg
)
nuget pack src\RenderLibrary\RenderLibrary.nuspec ^
    -OutputDirectory pkg
