`Important!` Read this page top bottom when it all is a part of a large picture instead of different subjects and themes.



Matrices can be used for linear transformations which works amazingly with vertices.

OpenGL expects all the vertices that want to become **visisble** ti be in normalized device coordinates (NDC) after each vertex shader run.

NDC: (Normalized Device Coorodinates), Coordinates between **-1.0** and **1.0** on a normal cartesian coordinate system. OpenGL requires visible vertices to be in this range to display them on the x,y and z axes.



Transforming coordinates to NDC is usually a accomplished in several linear transformations where we transform an object's vertices to several coordinate systems before really turning them to NDC. The advantage of having several **intermediate coordinate systems** is that some operations/calculations are easier in certain coordinate systems.

There are a total of 5 different coordinate systems that are of importance to us:
- `Local space` or object space
- `World space`
- `View space` or eye space
- `Clip space`
- `Screen space`

To transform coordinates from one space to the next ocordinate space we'll use several transformation matrices of which the most important are the model, view and projection matrix (the `MVP` matrix). We could do all this in one transformation, but that leaves us with less flexibility (matrix multiplication still results into one final transformation either way).

**The transformation pipeline**

![](transformation_pipeline.png)


1. `Local Coordinates` - local coordinates of an object relative relative to its local origin. They're the coordinates your object begins in. 

2. `World Coordinates (World space)` - Transforming local space coordinates to coordinates in respect of a larger world.  These coordinates are relative to some global origin of the world, together with many other objects also placed relative to this world's origin. 

3. `View-space coordinates (View space)` - Transforming world coordinates in such a way that each coordinate is as seen from the camera or viewer's point of view. 

4. `Clip Coordinates (Clip Space)` - Projection of view space coordinates to clip coordinates. Clip coordinates are processed to the -1.0, 1.0 range and determine which vertices will end up on the screen. Projection to clip-space coordinates can add perspective if using perspective projection.

	Even with large similarities to the viewport transform, it's more of a logical space for visibility and giving objects perspective than an practical transformation. Coordinates here are still homogenous and have to happen here to assure that all the final computations into the viewport will go as desired.

5. `Viewport transform` -  Transformation of clip coordinates to screen coordinates that transforms the coordinates from -1.0 and 1.0 to the coordiante range defined by `glViewport`. The resulting coordinates are then sent to the rasterizer to turn them into fragments.



Further description of the different coordinates systems (spaces)

`Local space` is the coordinate space that is local to your object, i.e. where your object begins in. Think about how a model in blender is positioned around (0,0,0) and built out from there, but the model will still have a differnet location in the final application.  All the vertices of your model are therefore in local space: they are akk kicak to your object.

The vertices of the container (HelloWorld triangle and first releasae on github rectangle) we've been using were specified as coordinates between -0.5 and 0.5, with 0.0 as its origin. These are local coordinates.


`World space` - If we would import all of our models directly in the application they would all be somwhere positioned inside each other at the world's origin of (0,0,0). We want to define a position each object to position them inside a larger world.  This creates a context that gives sense to real coordinates we can describe in a three dimentional space.

- `Model Matrix`: A transformation matrix that translates, scales and/or rotates your object to place it in the world at a location/orientation they belong to.

`View space` - Is what people usually refer to as the camera of OpenGL (`camera space`, `eye space`). It's the result of transforming your world-space coordinate sto coordinates that re in front of the user's view; This the space as seen from the cameras point of view.
Accomplished with a combination of translations and rotations to translate/rotate the world/scene so that certain items are tranformed to the front of  the camera. These combined transformations are genereally stored instide a `View Matrix`.

`Clip Space` - At the end of each vertex shader run, OpenGL expects the coordinates to be within a specific range and any coordinate that falls outside this range is clipped/discarded.  The remaining coordinates will end up as fragments visible on your screen. Thats that for the `clip` trivia.

As earlier mentioned, because of specifying the visible coordinates to be within the range -1.0 and 1.0 isn't really intuitive, we specify our own coordinate set to work in and convert those back to NDC as OpenGL expects them.

-  `Projection Matrix` - Specifies a range of coordinates in each dimention. At the end we convert the specified range to NDC with a step called `Perspective Division`.  All of coordinates then mapped outside -1.0 and 1.0 will be therefore clipped.

	`Frustum`- The viewing box a projection matrix creates. Each coordinate that ends up inside this frustum will end up on the user's screen. Usually takes two different forms, we can eigther create an  [Orthographic Projection](#Orthographic%20Projection) matrix or a [Perspective Projection](#Perspective%20Projection) matrix.
	
	`Projection` - the total process to convert coordinates within a specified range to NDC that can easily mapped to 2D view-space coordinates is called a **projection** since the projection matrix **projects** 3D coordinates to the easay-to-map-to -2D NDC.
	
	 `Perspective division` - A process where we divide the x, y and z components of the position vectors by the vector's hopmogenous w component. Perspective division is what transforms the 4D clip space coordinates to 3D NDC. **This step is performed automatically at the end of the vertex shader step.**

- `Quick sidenote` - in case of singular primitives is outside the clipping volume, OpenGL will reconstruct the traingle asa one or more triangles to fit inside the clipping range.

`Viewport transform` is the stage where the resulting coordinates are mapped to screen coordinates (using the settings of `glViewport`) and turned into fragments. This happens automatically as we can see with the triangle example $v \in R^3$ .


### Orthographic Projection

- Cube like frustum that defines the clipping space where each vertex outside this box is clipped.
- Orthographgic projection matrix is specified with width, height and length of the visible frustum.
- Frustum defines the visible coordinates and is specified by a width, a height and a near and far plane. Any coordinate in front of the near plane is clipped and the same applies to coordinates behind the far plane. 
- Orhographic frustum directly maps all coordinates inside the frustum to NDC without any special side effects since it won't touch the w component of the transformed vector; if the w component remains equal to 1.0 perspective division won't change the coordinates.
- Direct projection produces unrealistic results since the projection doesn't take perspective into account. Perspective projection fixes that.

`glm::ortho(left , right, bottom, top, near, far)` to define an orthographic projection matrix. Or manually with a formula.
### Perspective Projection

- Projection with perspective (things far away appear to be much smaller)
- The projection matrix mas a given frustrum range to clip space, but also manipulates the w value of each vertex coordinates in such a way that the furthey away a vertex coordinate is from the viewer, the higher this w component becomes. Once the coordinates are transformed to clip space they are in the range -w to w. OpenGL requires that the visible coordinates fall between the range -1.0 and 1.0 as the final vertex shader output, thus once the coordinates are in clip space, perspective division is applied to the clip space coordinates.
- Perspective division divides each vertex coordinate by its w component, giving smaller vertex coordinates further awaay from the viewer.
- How orthographic and projection matrices are really calculated: https://www.songho.ca/opengl/gl_projectionmatrix.html

`glm::perspective(FoV, aspect (width/height or constant), near, far)`; or manual (see source over).