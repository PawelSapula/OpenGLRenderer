#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int initShader();

const unsigned int W_WIDTH = 800;
const unsigned int W_HEIGHT = 600;

const float vertices[] = { // Triangle, even so z coordinate is depth. (NDC)
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f, 0.5f + 0.1667, 0.0f
};

const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 matrix;\n"
"void main() {\n"
"	gl_Position = matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // 6
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGLRenderer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Register callback function to scale with the window.

	unsigned int shaderProgram = initShader(); // Shader assembly and initialization



	unsigned int VAO; // Required, Vertex Array Object to more effectively manage configurations (vertex, binding w shaders)
	glGenVertexArrays(1, &VAO); // Generates a VAO with a VAO id.

	glBindVertexArray(VAO); // Bind our VAO and configure VBO and unbind for later use.

	unsigned int VBO; // Vertex buffer objects, can store a large number of vertices since its slow to constantly send data from CPU to GPU
	glGenBuffers(1, &VBO); // Generate a buffer with a buffer ID for our VBO.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER is the buffer type for standard vertices.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //4th parameter: Defines how the set data should be managed by the GPU, more https://learnopengl.com/Getting-started/Hello-Triangle

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// 1. Which attribute (location = ?) in shader we want to configure.
	// 2. Size of the vertex attribute. Its vec3 so its composed of 3 values. (keep in mind our float array is 9)
	// 3. Specifies the type of data we send.
	// 4. True/false if the if we want the data normalized. 
	// 5. Stride, the space or size between consecusive vertex attributes. Can be 0 when tightly packed (same array objects).
	// 6. Weird cast, but offset of where relevant data begins in the buffer.

	glEnableVertexAttribArray(0); // Enable the destination in shader to really accept data. Disabled normally.
	// Unbinds VAO automatically. But line under for precation.

	glBindVertexArray(0); // Unbind VAO

	while (!glfwWindowShouldClose(window)) { //Render loop


		/////////////////// TEST

		double angle = glfwGetTime();

		float identity[] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		float rotateY3d[] = {
			cos(angle),	 0.0f,	sin(angle),		0.0f,
			0.0f,		 1.0f,	0.0f,			0.0f,
			-sin(angle), 0.0f,	cos(angle),		0.0f,

			0.0f,		 0.0f,	0.0f,			1.0f
		};

		float rotateY2d[] = {
			cos(angle),	 -sin(angle),   0,				0.0f,
			sin(angle),	 cos(angle)	,	0.0f,			0.0f,
			0,			 0.0f,			1,				0.0f,

			0.0f,		 0.0f,			0.0f,			1.0f
		};

		glProgramUniformMatrix4fv(shaderProgram, glGetUniformLocation(shaderProgram, "matrix"), 1, GL_FALSE, rotateY3d);

		///////////////////////

		processInput(window);

		glClearColor(0.1f, 0.3f, 0.3f, 1.0f); // Configuration for glClear
		glClear(GL_COLOR_BUFFER_BIT); // Clear color buffer to ensure that previous frame results dont linger.

		//glUseProgram(shaderProgram) Already enabled
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); //Draws after primitive choice, starting index and final count.

		glfwPollEvents(); //Keyboard presses, window changes
		glfwSwapBuffers(window); // Frame swap
	}

	glfwTerminate();
	return 0;

}

unsigned int initShader() {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a shader object referenced by an id
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // Creates a program and returns the ID reference
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // link the shaders together with the shader program

	glUseProgram(shaderProgram); //applies for every rendering call after this call

	glDeleteShader(vertexShader); // Unnecessary objects. Can delete.
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); //Telling OpenGL to respect the glfwWindow's dimensions.
	// Remember that NDC coordinates in OpenGL are [-1, 1]
}