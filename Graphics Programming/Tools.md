
CMake - Tool used for generating project/solution files from a collection of pre-defined CMake scripts.

Linking - To let the project know its dependencies, we use linking of libraries and includes to use them in our project. Standard practice is placing a **Libs** abd **Include** folder in our solution folder.,

OpenGL - Comes naturally with Microsoft SDK. Linux more work necessary.

GLFW - Window creator that interprets OpenGL drivers to make the context available to use.

GLAD - Tool for mapping OpenGL
- since OpenGL is only a standard/specification it is up to the driver manuifacturer to implement the specification to a driver that the specific graphics card supports.
- Since there are many different versions of OpenGL drivers, its functions are not known at compile time and have to be retrieved at runtime.
- Its developers task to retrieve the locations of the functions and store them in pointers for later use.
- Found by version at https://glad.dav1d.de/

GLM - Open**GL** **M**athematics. Header-only library which means we only have we only have to include proper header files and we're done. https://glm.g-truc.net/0.9.8/index.html