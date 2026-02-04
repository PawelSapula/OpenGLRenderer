#include "gl.h"

#include <iostream>
#include <cmath>

#include "shader.h"
#include "menu.h"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int initShader();

const unsigned int W_WIDTH = 800;
const unsigned int W_HEIGHT = 600;

const float vertices[] = {
	-0.5f, -0.5f, 0.0f,			0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,			0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,			1.0f, 0.0f,
	 0.5f, 0.5f, 0.0f,			1.0f, 1.0f
};

const unsigned int indices[] = {
	0, 1, 2,
	1, 2, 3
};

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); 
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

	Menu::init(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Register callback function to scale with the window.

	Shader shader("vertexShader.glsl", "fragmentShader.glsl");
	shader.use();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode.
	stbi_set_flip_vertically_on_load(true);

	unsigned int texture;
	glGenTextures(1, &texture); // Takes in how many textures we want to create and stores it in a unsigned int array. (our case 1 unsigned int)
	glBindTexture(GL_TEXTURE_2D, texture); // Binding texture to set properties

	// Sets the wrapping mode for our texture. arg1: texture type, arg2: which axis, arg3: wrapping mode.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Setting the texture filtering attributes.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels; // Color channels
	unsigned char* data = stbi_load("resources/gojo_sukuna.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// 1: Target
		// 2: Default mipmap level. Full image -> 0
		// 3: Desired texture format
		// 4, 5: width and height
		// 6: Legacy stuff, always 0
		// 7: Source image format -> loaded with RGB values
		// 8: Datatype of picture (loaded as chars, (bytes))
		// 9. Actual image data

		glGenerateMipmap(GL_TEXTURE_2D); // Generate Mipmaps :D
	}
	else {
		std::cout << "Failed to load texture." << std::endl;
	}

	stbi_image_free(data); // Free image from memory


	unsigned int VBO; // Vertex buffer objects, can store a large number of vertices since its slow to constantly send data from CPU to GPU
	unsigned int VAO; // Required, Vertex Array Object to more effectively manage configurations (vertex, binding w shaders)
	unsigned int EBO; // Element Buffer Object

	glGenVertexArrays(1, &VAO); // Generates a VAO with a VAO id. (Binds it with our address specified over.)
	glGenBuffers(1, &EBO); // Generates a EBO with EBO id.
	glGenBuffers(1, &VBO); // Generate a buffer with a buffer ID for our VBO.

	glBindVertexArray(VAO); // Bind our VAO and configure VBO and unbind for later use.
	 
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER is the buffer type for standard vertices.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //4th parameter: Defines how the set data should be managed by the GPU, more https://learnopengl.com/Getting-started/Hello-Triangle

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind EBO with the element array buffer.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Store our indices

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //Coordinates
	// 1. Which attribute (location = ?) in shader we want to configure.
	// 2. Size of the vertex attribute. Its vec3 so its composed of 3 values. (keep in mind our float array is 9)
	// 3. Specifies the type of data we send.
	// 4. True/false if the if we want the data normalized. 
	// 5. Stride, the space or size between consecusive vertex attributes. Can be 0 when tightly packed (same array objects).
	// 6. Weird cast, but offset of where relevant data begins in the buffer.

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float))); //Texture coords

	glEnableVertexAttribArray(0); // Enable the destination in shader to really accept data. Disabled normally.
	glEnableVertexAttribArray(1);
	// Unbinds VAO automatically. But line under for precation.

	glBindVertexArray(0); // Unbind VAO

	while (!glfwWindowShouldClose(window)) { //Render loop

		glfwPollEvents(); //Keyboard presses, window changes
		Menu::frameSetup();

		/////////////////// TEST

		double angle = glfwGetTime();

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

		if (Menu::rotationY) {
			Menu::MVPMatrix.M[0] = rotateY3d[0];
			Menu::MVPMatrix.M[2] = rotateY3d[2];
			Menu::MVPMatrix.M[5] = rotateY3d[5];
			Menu::MVPMatrix.M[8] = rotateY3d[8];
			Menu::MVPMatrix.M[10] = rotateY3d[10];
			Menu::MVPMatrix.M[15] = rotateY3d[15];
		}

		shader.setUniform(MATRIX4FV, "matrix", Menu::MVPMatrix.M);
		shader.setUniform(F1V3, "color", Menu::color);

		///////////////////////

		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Configuration for glClear
		glClear(GL_COLOR_BUFFER_BIT); // Clear color buffer to ensure that previous frame results dont linger.

		//glUseProgram(shaderProgram) Already enabled
		//glBindTexture(GL_TEXTURE_2D, texture); Already enabled
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // arg 2: 6 indices -> 6 vertices in total, arg 3: type, arg 4: offset in EBO
			

		//VBO only example under
		//glDrawArrays(GL_TRIANGLES, 0, 3); //Draws after primitive choice, starting index and final count.

		Menu::render();
		glfwSwapBuffers(window); // Frame swap
	}

	Menu::shutdown();
	glfwTerminate();
	return 0;

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