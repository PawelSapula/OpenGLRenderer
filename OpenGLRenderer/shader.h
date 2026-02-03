#pragma once // C++20 version of a include guard or header guard to prevent a class from being included multiple
// times in a single compilation unit. This is a modern version of the #ifndef and #define/endif macro.

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum UniformType {
	MATRIX4FV,
	F1V3
};

class Shader {
public:

	//Program ID
	unsigned int ID;

	//Constructor that reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader method
	void use();
	//utility uniform functions (since the native support for overloading is gone)
	void setUniform(UniformType type, const std::string& name, void* value) const; //const makes the function read-only and can't modify the state of object.
};
