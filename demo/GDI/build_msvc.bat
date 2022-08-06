@echo OFF 
if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
)
msbuild microui-gdi-impl.sln -target:Build
move "x64\Release\microui-gdi-impl.exe" "microui-gdi-impl.exe"
rmdir /s /Q x64
echo build done!