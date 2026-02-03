#version 460 core 
//Corresponds to OpenGL version 460 -> 4.6

layout (location = 0) in vec3 aPos;	
//Location of the input variable (in pipeline). 

uniform mat4 matrix;

void main() {

	gl_Position = matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//gl_Position, a predefined buffer to contain the output

}