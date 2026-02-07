
# First mistake
While writing the linalg project class, i totally couldnt get close to writing my calculations for example on the perspective projection method. This might also be the cause of me totally failing creating matrix operations (yuh, im a cry for this one). This is tied to how OpenGL reads arrays, which is again historically tied to hardware and math conventions - it makes matrix-vector multiplication faster and simpler for the GPU.

![](Pasted%20image%2020260206011556.png)

Thats why also uniform setters have inbuilt a transpose method, f.ex. `glUniformMatrix4fv(location, amount, TRANSPOSE, data)`. 

This same problem also took me around 10 hours while creating the `LookAt` view-matrix dependant on camera positions. In my own mathematics library i ended up making a `val_ptr` function that calls transpose (to effectively convert to column-major) and then returns the first address of matrix instance `&m[0][0]`.

**Important!** Transposed/column interpreted data changes how it's read, without touching any actual data/computation of the matrices. It effectively does the following:
- $M(row)^T = M(col) \xrightarrow{OpenGL} M(row)$
- $M(row) = \xrightarrow{OpenGL} M(row)^T = M(col)$ <-- Here logical data really changes, matrix is transponded. 

Here's an actual example of this. First view matrix is logged and sent as is (row-major order, as we programmed it). Even so, it appears to be totally wrong against our expectations (GPU sees this as the transponded matrix).

Second view matrix is logged and sent as the same row-major matrix but just transposed. Even when the matrix is transponded and in theory should create a different transformation, due to the GPU reading matrices in column-major it will appear and operate as the standard row-major matrix from our perspective.

![](Pasted%20image%2020260207170949.png)