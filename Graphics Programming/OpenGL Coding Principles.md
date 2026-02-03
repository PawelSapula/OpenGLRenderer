
1. Linking GLFW as the window system and GLAD for extracting driver pointers to OpenGL functions.

2. Creating a window with GLFW, this allows us to easy create an OpenGL context for the window.

3. Eventual window callbacks to sync with OpenGL

4. Shader creation (for the necessary ones), compilation  and program creation. Remember to call glUseProgram(shaderID) before use!

5. Creation of the VAO's and VBO's for sooner use.

6. Main loop with input processing, callback handeling, drawing and swapping frame.

Definitions:
- VBO (Vertex Buffer Object): Specialized object to store and effectively send CPU vertex data to the GPU. This allows us also to manage where on the GPU it should be stored (long term, static, dynamical and changing etc.). Specifies also which data gets send to which input on the shaders.

 - VAO (Vertex Array Object): Allows us to create configurations for VBO and EBO (the different attributes). Automatically links EBO and VBO together. It automatically unbinds when enabling destined attribute, but for precaution sake, we can unbind it with glBindVertexArray(0);

+ EBO (Element buffer objects): A buffer just like a VBO that stores the indices (multiple of indexes) that OpenGL uses to decide what vertices to draw. This lets us create rectangles for example with just 4 vertices instead of 6. 