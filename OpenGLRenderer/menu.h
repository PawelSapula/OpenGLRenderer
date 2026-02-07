#pragma once
#include <GLFW/glfw3.h>


//TODO: Alternative for MVP since everything got ported into linalg.h/.cpp

struct MVP {
	float M[16];
	float V[16];
	float P[16];

	MVP(); // Constructor for initializing the matrices as identity matrices.
};

namespace Menu {

	inline float color[3] = {1,1,1}; // Inline for header file initialization, so compiler knows that this is the owner between multiple source files (check for better definition)
	inline MVP MVPMatrix;
	inline bool rotationY = false;

	void init(GLFWwindow* window);
	void createMatrixTable4x4(const char* label, float* matrix);
	void frameSetup();
	void render();
	void shutdown();

}
