#include "gl.h"
#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		//Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		//Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close file handlers
		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "Error shader file reading.\n" << std::endl;
	}

	//Convert to C-type strings
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER); // Create a shader object referenced by an id
	glShaderSource(vertex, 1, &vShaderCode, NULL); // A long string, so size = 1 and NULL for full length (maybe).
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "Vertex shader compilation failed.\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "Fragment shader compilation failed.\n" << infoLog << std::endl;
	}

	ID = glCreateProgram(); // Creates a program and returns the ID reference
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID); // link the shaders together with the shader program

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Shader program linking failed.\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex); // Unnecessary objects. Can delete.
	glDeleteShader(fragment);

}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setUniform(UniformType type, const std::string& name, void* value) const {
	if (type == F1V3) {
		glProgramUniform1fv(ID, glGetUniformLocation(ID, name.c_str()), 3, (float*)value);
	}
	if (type == MATRIX4FV) {
		glProgramUniformMatrix4fv(ID, glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, (float*)value);
	}
}
