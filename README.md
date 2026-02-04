# OpenGLRenderer
With high interest in computer graphics and low-level programming,i decided to do a hobby project that picks up advanced OpenGL topics and low C/C++. 

## How to run

 **Windows**: Using Visual Studio, set the linker to include additional includes and libraries with the corresponding folders of this source, namely `Include` and `Libs`. This should allow you to include GLAD and GLFW libraries **See below**.
 If this would for some reason not work, go ahead and download your glad dependencies with from [here](https://glad.dav1d.de/). Change our folders for these new ones. Then compile GLFW with CMake and slide it along.
 If you read this, you probably have the Microsoft SDK which automatically contains OpenGL. If not, u will install Visual Studio and get it.
 
 <code>#include <glad/glad.h> 
#include <GLFW/glfw3.h></code>


**Linux**: Use JetBrains CLion IDE if you don't wanna shoot yourself in the foot with blasaphemous things like VSCode or other goofy IDEs for this system (Not if its Git's Nano tho). This will automatically detect for you to use CMake as a build manager and generate a `CMakeLists.txt` file. Call here to find the GLFW and OpenGL packages and then link target libraries.

 > [!TIP]
> Linux users have to often install the GNU C++ Compiler, CMake, GLFW, OpenGL on their own. This can be done for Debian/Ubuntu systems with `sudo apt get`.
> <br><br> For ImGUI compilation for both systems. For VS, add the .cpp files to the Project Source and simply build the solution. For CMake as your build manager, simply Edit `CMakeLists.txt` and add include directories pointing to the `OpenGLRenderer\imgui` and put all the .cpp files asa executables.

> [!WARNING]
> Running as a CMake Application changes normally working directory to the ´cmake-build-xxx´ folder. Remember to set this attribute (recommended) or provide full directories for the shader.
