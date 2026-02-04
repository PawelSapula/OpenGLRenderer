#version 460 core

out vec4 FragColor; 
//Declaring an output variable
in vec2 TexCoord;

uniform float color[3];
uniform sampler2D ourTexture;

void main() {
	//FragColor = vec4(color[0], color[1], color[2], color[3]); 
	// R,G,B,A
	FragColor = texture(ourTexture, TexCoord) * vec4(color[0], color[1], color[2], 1.0f);
}