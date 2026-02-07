
# First mistake
While writing the linalg project class, i totally couldnt get close to writing my calculations for example on the perspective projection method. This might also be the cause of me totally failing creating matrix operations (yuh, im a cry for this one). This is tied to how OpenGL reads arrays, which is again historically tied to hardware and math conventions - it makes matrix-vector multiplication faster and simpler for the GPU.

![](Pasted%20image%2020260206011556.png)

Thats why also uniform setters have inbuilt a transpose method, f.ex. `glUniformMatrix4fv(location, amount, TRANSPOSE, data)`. 

This same problem also took me around 10 hours while creating the `LookAt` view-matrix dependant on camera positions. In my own mathematics library i ended up making a `val_ptr` function that calls transpose (to effectively convert to column-major) and then returns the first address of matrix instance `&m[0][0]`.