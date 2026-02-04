#version 460 core
//Corresponds to OpenGL version 460 -> 4.6

layout (location = 0) in vec3 aPos;	
layout (location = 1) in vec2 aTexCoord;
//Location of the input variable (in pipeline). 

uniform mat4 matrix;
out vec2 TexCoord;

void main() {

	gl_Position = matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//gl_Position, a predefined buffer to contain the output
	TexCoord = aTexCoord;

}