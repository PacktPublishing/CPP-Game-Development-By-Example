@echo off
echo compiling glsl shaders to spirv 
for /r %%i in (*.vert;*.frag) do %VULKAN_SDK%\Bin32\glslangValidator.exe -V "%%i" -o  "%%~dpiSPIRV\%%~nxi".spv

