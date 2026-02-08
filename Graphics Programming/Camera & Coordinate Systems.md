`Important!` Read this page top bottom when it all is a part of a large picture instead of different subjects and themes.

How a camera system is made and how it can be simulated can be seen here [Camera](#Camera).



Matrices can be used for linear transformations which works amazingly with vertices.

OpenGL expects all the vertices that want to become **visisble** ti be in normalized device coordinates (NDC) after each vertex shader run.

NDC: (Normalized Device Coorodinates), Coordinates between **-1.0** and **1.0** on a normal cartesian coordinate system. OpenGL requires visible vertices to be in this range to display them, this applies for all the x,y and z axes.

**Important!** NDC coordinates contains the z-component as we already know. This matters because of the OpenGLs inbuilt depth buffer (Z-buffer). This is a measure in which layer of the screen the object displays its vertices, not in the regular world-coordinate z-axis. 



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



# Camera

OpenGL by itself is not familiar with the concept of a camera, but we can try to simulate one by moving all objects in the scene in the reverse direction, giving the ilusion that we are moving.

To **define** a camera we needs its position in world space, the direction it's looking at and vectors pointing right and upwards from the camera. This will correspond to a coordinate systrem with 3 perpendicular (right angle ) unit axes with the camera's position as the origin. This is also known as the `Gram-Schmidt` process in linear algebra.

-  Setting the **camera position** is the same as defining a vector in $R^3$. Remember that the camera is stearing down the negative z-axis, so to move it backwards we would define the position as for example `vec3(0.0f, 0.0f, 3.0f)`.

- **Camera direction** is the vector corresponding to what direction we are pointing at.  This can be obtained by getting the difference (with different words, subtracting) our camera position vector and for example the origin of our scene (world). By intuition we would write that as `direction = target - cameraPos`.  With keeping in mind that the camera looks down the negative z-axis, we want to negate this direction to point backwards of us, for the sake of convenient calculations. Therefore, we would define the direction as following `direction = cameraPos - target`.  (It becomes a vector pointing from target to cameraPos)

* The **right axis** (of camera) represents the positive x-axis of the camera space. This is obtained by getting the cross product of the up-axis (in world space) with the cameras direction. (Opposite cross product $cameraDirection \times up$ we would get a vector that points in the negative x-axis).

- **Up axis** (of the camera) can be simply optained by taking the cross product of the direction vector and right vector.

![](Pasted%20image%2020260207170543.png)

Using these camera vectors we can now create a `LookAt` matrix that proves useful for creating a camera. 

**LookAt matrix** (a view matrix that looks at a given target)
- Using LookAt matrix as our view matrix effectively transforms the world in the opposite direction of where we want the camera to move.
- When we have defined a coordinate space using 3 perpendicular axes we can now create a matrix suitable for camera operations.
- We can set up the vectors in a 4x4 with a translation vector and we can transform any vector to that coordinate space by multiplying it with this matrix. 
$$ LookAt = \begin{bmatrix} \color{red}{R_x} & \color{red}{R_y} & \color{red}{R_z} & 0 \\ \color{green}{U_x} & \color{green}{U_y} & \color{green}{U_z} & 0 \\ \color{blue}{D_x} & \color{blue}{D_y} & \color{blue}{D_z} & 0 \\ 0 & 0 & 0 & 1 \end{bmatrix} * \begin{bmatrix} 1 & 0 & 0 & -\color{purple}{P_x} \\ 0 & 1 & 0 & -\color{purple}{P_y} \\ 0 & 0 & 1 & -\color{purple}{P_z} \\ 0 & 0 & 0 & 1 \end{bmatrix} $$
R = right  vector, U = up vector,     D /F= direction/forward vector ,     P = camera's position vector
**Transformations applied right to left!**

**Note**: These matrices are inverted (transposed (rotation) and negated (translation) respectively for the rotation and translation). This is to effectively transform the world in the opposite direction of where we want the camera to move. Keep in mind that **the transponding a rotation matrix is the same as the inverse of that matrix**. 

Its really important to understand that the first matrix - the rotation matrix. This matrix is ALREADY transponded (in this formulation), meaning that the rotation will be applied in reverse. It's an inverse version of the `camera transformation`, a matrix that describes the camera in world-space itself. 

Nonetheless since we want the world to translate opposite from where we want to move, the camera position is negated. After the two matrices are multiplied, we get dot products as `-dot(coordinate, camCoordinate)`. (Since the multiplication affects the whole row in the rotation matrix, it becomes an basis axis product). This creates the result of the world getting transformed and the camera becoming an origin.

While working with OpenGL it's important to remember that it reads data as column-major as default.  Note that column-major layout and transponding aren't the same operations! More about this can be found at [Learning from failure](Learning%20from%20failure.md).

Important sources - https://graphicscompendium.com/opengl/18-lookat-matrix, My linalg.cpp

# Camera - In practice

For **walk around** movement we specify world space coordinates for our camera, target (often z = -1, meaning in front) and camera up. These are vector variables that we can control with using inputs and simple operations with scalars and `deltaTime` but also cross products for left and right and the predefined camera up.



**Look around** is as expected dependent on the direction the camera is looking - meaning some trigonometry will be necessary to describe this.

**Euler angles** - 3 values that can represent any rotation in 3D, by **Leonhart Euler**. There are 3 Euler angles, `pitch, yaw and roll`. Each angle is represented by a single value and with the combination of all 3 of them we can calculate any rotation vector in 3D.

- **Pitch**: The angle that depicts how much we're looking up or down.
- **Yaw**: The magnitude we're looking to the left or to the right.
- **<del>Roll</del>**: How much we roll/twist (mostly used in space-flight cameras).

**Theory**:
Imagine right triangles with $hypotenuse = 1$. We can formulate some expressions:
- $sin(yaw) = y/h = y/1 = y = sin(yaw)$
- $cos(yaw) = x/h = x/1 = x = cos(yaw)$

This triangle can be imagined looking down the y-axis with the adjacent and opposite sides being parallell to the scene's x and z-axis.

If we visualize the yaw angle to be counter-clockwise angle from the x-axis, we can create two distinct relations:
- $x = cos(yaw)$
-  $z = sin(yaw)$

The  `pitch` angle can be obtained in a similar manner. Imagine now a triangle vertically from the x-z-plane. This creates new relations:
- $y = sin(pitch)$
- $x/z-plane = cos(pitch)$

Since x and z also are dependent on the pitch value, the total formulation for the pitch and yaw angle will be:
$$
yaw = \theta, pitch = \omega
$$ $$
\begin{bmatrix}
	cos(\theta) * cos(\omega) \\
	sin(\omega) \\
		sin(\theta) * cos(\omega)
\end{bmatrix}
$$


Since everything in the world-space is positioned in the direction of the negative z-axis, we will set the value of `yaw = -90.0f`.
Continuing on, GLFW has a mouse callback for cursor position. Here we have to:
1. Calculate the mouse's offset since the last frame.
2. Add the offset values to the camera's yaw and pitch values.
3. Add some constraints to the minimum/maximum pitch values. LookAt matrix flip when (` camera direction == world up vector (-89f, 89f)`)
4. Calculate the direction vector.
5. Set camera front to normalized direction vector.