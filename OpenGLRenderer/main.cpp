#include "gl.h"

#include <iostream>

#include <cmath>
#include <numbers>

#include "shader.h"
#include "linalg.h"
#include "menu.h"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float fov = 45.0f;
float lastX = 400, lastY = 300; // Middle of the screen at 800x600
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = true;

const unsigned int W_WIDTH = 800;
const unsigned int W_HEIGHT = 600;

float box[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

const float vertices[] = {
	-0.5f, -0.5f, 0.0f,			0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,			0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,			1.0f, 0.0f,
	 0.5f, 0.5f, 0.0f,			1.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

const unsigned int indices[] = {
	0, 1, 2,
	1, 2, 3
};

struct Camera {
	Vec3 pos;
	Vec3 front;
	Vec3 up;

	Camera() = default;
	Camera(Vec3 pos, Vec3 cameraFront, Vec3 cameraUp);
};

Camera::Camera(Vec3 pos, Vec3 front, Vec3 up) {
	this->pos = pos;
	this->front = front;
	this->up = up;
}

Camera camera(Vec3(0.0f, 0.0f, 3.0f),
	Vec3(0.0f, 0.0f, -1.0f),  // Target in front of camera. Target is defined in world space coordinates.
	Vec3(0.0f, 1.0f, 0.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	glfwSetCursorPosCallback(window, mouse_callback); // Mouse callback
	glfwSetScrollCallback(window, scroll_callback); // Mouse scroll callback

	Shader shader("vertexShader.glsl", "fragmentShader.glsl");
	shader.use();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode.
	glEnable(GL_DEPTH_TEST); // depth testing via the depth buffer (z coordinate of a fragment). GLFW automatically creates this buffer.
	stbi_set_flip_vertically_on_load(true); // Flip to normal position. Happens because texture coords LeftBottom - (0,0) while pictures are normally LeftTop (0,0)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Capturing the cursor

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW); //4th parameter: Defines how the set data should be managed by the GPU, more https://learnopengl.com/Getting-started/Hello-Triangle

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

		Menu::frameSetup();

		/////////////////// TEST

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		Matrix4 view;
		Matrix4 projection;

		//model = LinAlg::rotate(model, glm::radians(0.0f), Vec3(1.0f, 1.0f, 0.0f));
	
		projection = LinAlg::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
		view = LinAlg::lookAt(
			camera.pos,
			Vec3::add(camera.pos, camera.front), // The camera keeps looking in forwards. The target is in front of us and moves with the position.
			camera.up
		);

		LinAlg::setMatrix2D1D(Menu::MVPMatrix.V, &view.matrix[0][0], sizeof(Menu::MVPMatrix.M));
		LinAlg::setMatrix2D1D(Menu::MVPMatrix.P, &projection.matrix[0][0], sizeof(Menu::MVPMatrix.P));
		
		shader.setUniform(MATRIX4FV, "V", LinAlg::val_ptr(view));
		shader.setUniform(MATRIX4FV, "P", LinAlg::val_ptr(projection));
		
		shader.setUniform(F1V3, "color", Menu::color);

		std::cout << std::to_string(camera.pos.x) << " " << std::to_string(camera.pos.y) << " " << std::to_string(camera.pos.z) << std::endl;

		///////////////////////

		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Configuration for glClear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color buffer to ensure that previous frame results dont linger.

		//glUseProgram(shaderProgram) Already enabled
		//glBindTexture(GL_TEXTURE_2D, texture); Already enabled
		glBindVertexArray(VAO);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // arg 2: 6 indices -> 6 vertices in total, arg 3: type, arg 4: offset in EBO
			
		for (int i = 0; i < 10; i++) {

			Matrix4 model(1.0f);
			model = LinAlg::translate(model, Vec3(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z));
			model = LinAlg::scalar(model, 4);
			LinAlg::setMatrix2D1D(Menu::MVPMatrix.M, &model.matrix[0][0], sizeof(Menu::MVPMatrix.M));
			shader.setUniform(MATRIX4FV, "M", model.matrix);

			//VBO only example under
			glDrawArrays(GL_TRIANGLES, 0, 36); //Draws after primitive choice, starting index and final count.
		}

		Menu::render();
		glfwSwapBuffers(window); // Frame swap
		glfwPollEvents(); //Callbacks, IO events, etc.
	}

	Menu::shutdown();
	glfwTerminate();
	return 0;

}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	const float cameraSpeed = 2.0f*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.pos = Vec3::add(camera.pos, Vec3::scalar(camera.front, cameraSpeed)); // Go forward in z axis -> camera object - z
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.pos = Vec3::subtract(camera.pos, Vec3::scalar(camera.front, cameraSpeed));  // Go back in z axis -> camera object + z
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.pos = Vec3::subtract(camera.pos, Vec3::scalar(Vec3::normalize(Vec3::cross(camera.front, camera.up)), cameraSpeed)); // Go negative x -> world right
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.pos = Vec3::add(camera.pos, Vec3::scalar(Vec3::normalize(Vec3::cross(camera.front, camera.up)), cameraSpeed)); // Go positive x -> world left
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.pos = Vec3::add(camera.pos, Vec3::scalar(camera.up, cameraSpeed)); // normal
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.pos = Vec3::subtract(camera.pos, Vec3::scalar(camera.up, cameraSpeed)); // normal
	}

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset;
	if (fov < 1.0f) fov = 1.0f;
	if (fov > 60.0f) fov = 60.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) { // In case user clicks in to the window to limit the movement jump.
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX; 
	float yoffset = lastY - ypos; //reversed to make new ypos (ypos<lastY) positive when going up the screen
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // Sensitivity variable to scale with, 
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset; // Add sensitivity offset. Can directly add because proportional to screen up-down, left-right.
	pitch += yoffset;

	//Limit the viewing angles because of a weird flip in LookAt when camDir = worldUp.
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	Vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.front = Vec3::normalize(direction);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); //Telling OpenGL to respect the glfwWindow's dimensions.
	// Remember that NDC coordinates in OpenGL are [-1, 1]
}