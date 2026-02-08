# OpenGLRenderer
With biggest interest in computer graphics and low-level programming,i decided to work on a hobby project that picks up advanced OpenGL topics and C/C++. 

## How to run

 **Windows only**: Using Visual Studio, set the confiuration of Include and Library directories with the corresponding folders of this source, namely `Include` and `Libs`. <br> Additionally, go to the linker and add `glfw3.lib`, `opengl32.lib` as additional dependencies if they aren't set out the box.<br><br> Microsoft SDK comes already with OpenGL support, so don't be confused from not finding `opengl32.lib` in `Libs`. This should allow you to use the GLM, GLAD and GLFW libraries. <br><br> Furthermore, many source files are compiled or implemented manually from header files, so make sure to add to the solution explorer every .cpp (and glad.c) file from `OpenGLRenderer` aswell as `OpenGLRenderer/imgui` and `OpenGLRenderer/imgui/backends` before you compile.

<br><br>

**Multi-platform support**:  CMake is a cross platform build tool used for automating building applications. It works pretty similarly to VS, where the project properties are stored in a `CMakeLists.txt` file where all compiler-independent instructions can be described. <br><br> In the Releases tab, i provide a configuration file along the source code that sets all the necessary executables, libraries and include directories meant to work with the current release. Put `CMakeLists.txt` in the home/source directory of `../OpenGLRenderer` and build the project.

<br>

 > [!TIP]
> Linux users have to often install the GNU C++ Compiler, CMake, GLFW, OpenGL on their own. This can be done for Debian/Ubuntu systems with `sudo apt get`.

> [!WARNING]
> Running as a CMake Application in most IDE's changes normally working directory to the ´cmake-build-xxx´ folder on program execution attempt. The configuration file has properties to fix this, but most IDE's won't take precautions to this. Remember to set this attribute (recommended) or provide full directories for the shaders.
