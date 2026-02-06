
# First mistake
While writing the linalg project class, i totally couldnt get close to writing my calculations for example on the perspective projection method. This is tied to how OpenGL reads arrays, which is again historically tied to hardware and math conventions - it makes matrix-vector multiplication faster and simpler for the GPU.

![](Pasted%20image%2020260206011556.png)

Thats why also uniform setters have inbuilt a transpose method, f.ex. `glUniformMatrix4fv(location, amount, TRANSPOSE, data)`. 