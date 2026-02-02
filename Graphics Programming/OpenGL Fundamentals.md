Refer to [[OpenGL Coding Principles]] and [[Tools]] for more practical documentation (skipping most theory) .

- Developed usually by the graphic card manufacturers for different versions
- Ported to Linux etc. by community
- **Core-profile** is the indepth OpenGL. **Immediate Mode** allowed simple drawing but is now depreacated.

- OpenGL is by itself a **large state machine**
	- A ciollection of variables
	- **Context** - what we refer to asa the OpenGL state

- OpenGL libraries are written in C
	- Derived often to other languages such as LWJGL
	- Since of its low-level nature, OpenGL hasa developed an abstraction layer like **objects**

- **Objects** - struct like entities
- Often seen workflow in OpenGL:
	1. Create an object and store a reference to it as an id
	2. Bind the object to the taraget location of the context
	3. Set options 
	4. Unbind object that hasa a copy of the data

- GLFW Callbacks - Callback functions (events) that enables configuration of window changes (ex. scaling, keypresses).

- Graphics Pipeline
	- Can be divided into two large parts:
		1. Transforming 3D into 2D coords
		2. 2D coords into actual colored pixels

	- Shaders
		- Small programs that can be run in parallell and specify in a singular, small task.
		- Have input and output for the pipeline.
		- Shaders are divided onto the cores of the GPU. Spares CPU of enormous computing.
		- Written in GLSL (OpenGL Shading Language)
		- Gives the developer much more control over specific parts of the pipeline.

	 - Stages of the graphics pipeline:
		 1. Vertex Shader (configurable, required)
		 2. Geometry Shader (configurable)
		 3. Shape Assembly
		 4. Rasterization
		 5. Fragment Shader (configurable, required)
		 6. Tests and blending
	
	- Vertex Shader
		- Vertices:  Collection of data per 3D coordinate (data is represented using vertex attributes, for simplicity sake position and color)
		- Vertex: List of vertices
		- **Task**: Transformation

	- Geometry Shader
		- Input from vertex shader
		- **Task**: Forms a primitve or has the ability to generate other shapes by emitting new vertices to form new or other primitives.

	- Assembly stage
		- **Task**: Assembles the points in the primitive shape into triangles

	- Rasterization stage
		- **Task**: Maps the results of assembly stage to the corresponding pixels on the final screen.
		- **Clipping is performed to discard everything that is outside the view, increasing performance**

	- Fragment Shader
		- **Task**: Calculate the final color of a pixel. All advanced OpenGL effects occur here. Contains the data about the 3D scene to calculate the final pixel color.
	
	- Blending stage
		- **Task**: Depth check, opacity.
