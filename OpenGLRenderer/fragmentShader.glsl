#version 460 core

out vec4 FragColor; 
//Declaring an output variable

uniform float color[4];

void main() {
	FragColor = vec4(color[0], color[1], color[2], color[3]); 
	// R,G,B,A
}