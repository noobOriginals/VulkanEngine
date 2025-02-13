# **NOTE**

### To build this project, you currently need **[CMake](https://cmake.org/)**, **[GLFW](https://www.glfw.org/)** and the **[Vulkan SDK](https://vulkan.lunarg.com/)**.  

## CMake

Go to the **[CMake website download page](https://cmake.org/download/)** and install the CMake GUI.  

## Vulkan SDK

Get the Vulkan SDK installer for your platform from **[LunarG's website](https://vulkan.lunarg.com/)**.  
Then install it with all components (main interest is GLM, for later math transformations).  
With it installed, create a new folder on your computer named "**vulkan**". Then copy the folders from the "**Include**" folder of the SDK there. Also copy the contents of the "**Lib**" folder to your "**vulkan**" folder.  

## GLFW

Get the GLFW library from **[their website](https://www.glfw.org/)** or by **[cloning it on github](https://github.com/glfw/glfw)**.  
Then follow **[these steps](#building-with-the-cmake-gui)** to build the GLFW library with CMake.  
**Make sure to build it with the "Release" config.**    
In the build directory provided to CMake, after you build the files, you should have a folder named "**src**".
Navigate to "**src/Release**", and copy the "**glfw3.lib**" to your "**vulkan**" folder.  
Then go back to your GLFW source folder and copy the contents of the "**include**" folder (the "**GLFW**" folder) to your "**vulkan**" folder.

## Next

After completing **[theese three steps](#cmake)**, you should have a folder named "**vulkan**" that has theese files: (this is if you installed all components of the VulkanSDK, and there's also **[Assimp](#assimp)** and **[stb_image](#stb_image)** here as I have them already installed and placed in this folder. But don't worry about **[these](#aditional-libraries)** as I'll not be using them very soon) 
```bash
$ ls
GLFW                     SPIRV-Tools-optd.lib     glslang-default-resource-limits.lib   spirv-cross-cored.lib
GenericCodeGen.lib       SPIRV-Tools-reduce.lib   glslang-default-resource-limitsd.lib  spirv-cross-cpp.lib
GenericCodeGend.lib      SPIRV-Tools-reduced.lib  glslang.lib                           spirv-cross-cppd.lib
MachineIndependent.lib   SPIRV-Tools-shared.lib   glslangd.lib                          spirv-cross-glsl.lib
MachineIndependentd.lib  SPIRV-Tools-sharedd.lib  shaderc                               spirv-cross-glsld.lib
OSDependent.lib          SPIRV-Tools.lib          shaderc.lib                           spirv-cross-hlsl.lib
OSDependentd.lib         SPIRV-Toolsd.lib         shaderc_combined.lib                  spirv-cross-hlsld.lib
SDL2                     SPIRV.lib                shaderc_combinedd.lib                 spirv-cross-msl.lib
SDL2-static.lib          SPIRVd.lib               shaderc_shared.lib                    spirv-cross-msld.lib
SDL2-staticd.lib         SPVRemapper.lib          shaderc_sharedd.lib                   spirv-cross-reflect.lib
SDL2.lib                 SPVRemapperd.lib         shaderc_util.lib                      spirv-cross-reflectd.lib
SDL2_test.lib            Volk                     shaderc_utild.lib                     spirv-cross-util.lib
SDL2_testd.lib           assimp                   shadercd.lib                          spirv-cross-utild.lib
SDL2d.lib                assimp-vc143-mtd.dll     slang                                 spirv-headers
SDL2main.lib             assimp-vc143-mtd.lib     slang-rt.lib                          spirv-tools
SDL2maind.lib            dxc                      slang-rtd.lib                         spirv_cross
SPIRV-Tools-diff.lib     dxcompiler.lib           slang.lib                             stb_image
SPIRV-Tools-diffd.lib    dxcompilerd.lib          slangd.lib                            vk_video
SPIRV-Tools-link.lib     gfx.lib                  spirv-cross-c-shared.lib              vma
SPIRV-Tools-linkd.lib    gfxd.lib                 spirv-cross-c-sharedd.lib             volk.lib
SPIRV-Tools-lint.lib     glfw3.lib                spirv-cross-c.lib                     volkd.lib
SPIRV-Tools-lintd.lib    glm                      spirv-cross-cd.lib                    vulkan
SPIRV-Tools-opt.lib      glslang                  spirv-cross-core.lib                  vulkan-1.lib  
```
You should be interested in having these specific files:  
```bash
./GLFW/
./vulkan-1.lib
./vulkan/
./glm/
./glfw3.lib
```
## VULKAN_PATH Env Variable

Now you have to create an **environment variable** with the name "**VULKAN_PATH**" and the value equal to the path to your "**vulkan**" folder that you just created.    

On Windows, you edit the system env variables in the menu which you can search for in your taskbar (search for "**environment variable**").  
On Linux or Mac, you add that to your env paths in the bash or zsh config (Linux), or by command line, with  
```bash
$ VULKAN_PATH="path/to/vulkan"
$ export $VULKAN_PATH
```

## Building with the CMake GUI
Open the CMake GUI, then select the source code directory.  
Now select a build directory (you have to create it yorself first).  
Now click "**Configure**", and choose your generator (I recommend **Ninja** for Mac and Linux, and **Visual Studio** for Windows). Then click "**Configure**" again to save the settings.  
Then click "**Generate**", to generate the build files.
Go to your build folder, and:  
- If you're on windows and you're using VS, open the generated "**.sln**" file and build the solution inside VS.
- If you're using Ninja, open the build folder in a terminal tab and run the `ninja` command in that folder.  

You can also use cmake to build you files, like this:  
```bash
$ cmake --build <build folder> --confing <configuration (usually 'Debug' or 'Release')> --target <if generator is Ninja: 'all'; if generator is VS: 'ALL_BUILD'>  
```

And CMake will also tell you which is the binary output directory. For example:  
```bash
$ cmake --build build --config Debug --target ALL_BUILD
MSBuild version 17.12.12+1cce77968 for .NET Framework

    Checking File Globs
    Copying resources into birnay output folder
    Engine-test.vcxproj -> .\bin\Debug\Engine-test.exe
```
  
Now you should see the binary files that were generated by building in the binary output folder.  

## Building the project
Follow **[these steps](#building-with-the-cmake-gui)** again, with the project folder as the source, and a build folder created inside the project folder as build.  
Then you can run the generated binary `/bin/<config>/Engine-test.exe` to see the result of this project.  

## Aditional Libraries

## Assimp

Clone the **[GitHub Repo](https://github.com/assimp/assimp)** and follow **[these steps](#building-with-the-cmake-gui)** to build the library.  
**Make sure to build it with the "Release" config.**  
Then: place the "**assimp-vc143-mtd.lib**" found in "**./lib/Release**" in your "**vulkan**" folder.  
Then: place the contents of the "**./include**" folder in your "**vulkan**" folder.

## stb_image

Download the raw file **[stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)** and place it in you "**vulkan**" folder in a folder named "**stb_image**".  
**Whenever you use it, include it like this:**
```cpp
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
```
