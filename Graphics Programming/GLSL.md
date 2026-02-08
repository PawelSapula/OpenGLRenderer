The OpenGL Shading Language. Its a C-like language and is used to write shader programs for our GPU.

Vertex attributes: Input variables that have the input from the different buffers. Maximum number is limited by hardware. OpenGL guarantees there is always at least 16, 4-component vertex attributes available. Can be checked with `GL_MAX_VERTEX_ATTRIBUTES`.

Vertex shader is an exception; It's input comes straight from vertex data. To define how the vertex data is organized, we specify the input variables with location metadata so we can configure the vertex attributes on the CPU. This is done by initializing variables in the following way `(location=0) in vec3 aPos`.

**Important!** - If types and names equal on both sides of two shaders, OpenGL will link those variables together and it is possible to send data between shaders.

**Uniforms** - are another way to pass data from our application on the CPU to shaders on the GPU. Uniforms are `global` - unique per shader program object (can be accessed by any shader stage). 
Since OpenGL is in its core a C library, it does not have native support for function overloading. OpenGL defines new functions for each type with a postfix.

List of possible postfixes (... some of them):
- `f`: the function expects a `float` as its value.
- `i`: the function expects an `int` as its value.
- `ui`: the function expects an `unsigned int` as its value.
- `3f`: the function expects `3 floats` as its value.
- `fv`: the function expects an `float vector/array` as its value.

Structure
1. Starts with a version definition `#version version_number` that corresponds to the OpenGL version.
2. Have in and out variables. This is the only way shaders can communicate between each other.
3. Uniforms where variables from the CPU can be used in the shaders.
4. `void main()` as the entry poinbt and where the logic happens.

Variable Types
- Basic types from languages like C: `int, float, double, uint & bool`.
- Vectors
	- `vecn` - the default vector of `n` floats.
	- `bvecn` - a vector of `n` booleans.
	- `bvecn` - a vector of `n` integers.
	- `ivecn` - a vector of `n` integers.
	- `uvecn` - a vector of `n` unsigned integers.
	- `dvecn` - a vector of `n` double components.
	- Vector as `rgba` for colors.
	- Vector as `stpq` for texture coordinates.

	- Swizzling: allows vector syntax like this:

<code>vec2 someVec; <br>
vec4 differentVec = someVec.xyxx;
</code> 

Fragment interpolation: Blending of colors. When we have for example different colors on our vertices, after the rasaterization process, our pixel will blend the colors depending on their location from the "original" color vertices.

