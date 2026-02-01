#version 330 core //Corresponds to OpenGL version 330 -> 3.3
layout (location = 0) in vec3 aPos;	//Location of the input variable (in pipeline). 

void main() {

	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); //gl_Position, a predefined buffer to contain the output

}