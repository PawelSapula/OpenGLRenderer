An method for applying detail to our objects without creating coutnless vertices and storing color information in them aswell.

A `texture` is (most often) a 2D image used to add detail to an object. Think of it as a piece of paper that u wrap around your object.

**Texture coordinate**:  Textures coordinates (0,0)<->(1,1) asigned to each vertex from where it should take a sample from our texture. These are float numbers and not dependent on resolution. Fragment interpolation than does the rest for the other fragments [Fragment Interpolation](GLSL.md).

**Texel**: Texture pixel.

**Sampling**: Process of retrieving texture color using texture coordinates. Mapping a texture on a object can be done in many different way, thus our job is to tell OpenGL how to sample it according to our object and wanted effect.

**Texture wrapping**: A parameter used to specify the behaviour of the wrapped texture if the texture coordinates are outside the standard range. OpenGL offer a set of options:
- `GL_REPEAT`: The default behaviour for textures. Repeats the texture image.
- `GL_MIRRORED REPEAT`: Same as GL_REPEAT but mirrors the image with each repeat.
- `GL_CLAMP_TO_EDGE`: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a streched edge pattern.
- `GL_CLAMP_TO_BORDER`: Coordinates outside the range are now given a user-specified border color.

  Each of these can be set per coordinate axis (here s,t,r corresponsing to x,y,z) using `glTexParameter*` function.

**Texture Filtering**: How the texels should behave on an object. This specifically applies on large objects and a low resolution textures. OpenGL has options for this:
- `GL_NEAREST`: Also known as nearest neighbor or point filtering. It is the deafult texture filtering method of OpenGL. OpenGL selects the Texel that center is closest to the texture coordinate. 
- `GL_LINEAR`: Also known as (bi)linear filtering. Takes an interpolated value from the texture coordinates neighboring texels, approximating a color between the texels.

  Texture filtering can be set for magnifying and minifying operations (when scaling up or downwards) meaning we can choose the different options depending on the favoured look.

**Mipmaps**: A concept that is basaically a collection of texture images where each subsequent texture is twice as small compared to the previous one. This makes up for more accurate sampling of texels, instead of using a high resolution texture on objects far from the viewer where the resolution difference won't be noticable. Saves also cache memory. OpenGL is able to do all the hard work for us, providing the call to `glGenerateMipmap` after creating the texture. Provides new filtering solutions:

- `GL_NEAREST_MIPMAP_NEAREST`: Takes nearest mipmap to object size and use of neighbor interpolation for sampling.

-  `GL_LINEAR_MIPMAP_NEAREST`: Takes nearest mipmap to object size and use of linear interpolation for sampling.

-  `GL_NEAREST_MIPMAP_LINEAR`: Linearly interpolates between two minmaps closest to the objects size and use of neighbor interpolation for sampling.

-  `GL_LINEAR_MIPMAP_LINEAAR`: Linearly interpolates between two minmaps closest to the objects size and use of linear interpolation for sampling.

   **Important!** Mipmaps are used for downscaling, so for magnifying OpenGL will throw an exception.

How to make a picture a texture? Parse it to your program.  Recommended to use stb-image, Sean Barrett, https://github.com/nothings/stb/, a library for parsing the most picture types.

**Texture unit**: The memory location of a texture. The main purpouse of these are to allow us to use multiple textures at the same time in an fragment shader. Calling `glActiveTexture(GL_TEXTURE'0-15')` (atleast 16) enables us to activate the different texture units.

`IMPORTANT!` Two samplers are still needed to get two textures, just that they have asigned numeric values asigned to the texture unit. You can't lighten the process of initialization. The only thing that happens is that even when calling `glBindTexture`, it wont override our first texture but put hold it in the unit.

1. Activate a texture unit
2. Bind texture

The default texture unit for a texture is 0, which OpenGL automatically enables.  Not all graphics drivers do this unfortunately.