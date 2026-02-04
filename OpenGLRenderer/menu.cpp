#include "menu.h"

#include <string>

#include "imgui/imgui.h"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

MVP::MVP() {
	int iteration = 0;
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			float val = row == column ? 1.0f : 0.0f;
			M[iteration] = val;
			V[iteration] = val;
			P[iteration] = val;
			iteration++;
		}
	}
}

void Menu::init(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true); // Second argument will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init("#version 460");
}

void Menu::frameSetup() { //Goes below glfwPollEvents

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Renderer settings");
	ImGui::ColorEdit3("Fragment shader color", color);
	ImGui::Checkbox("Play rotation animation", &rotationY);
	createMatrixTable4x4("Model Matrix", MVPMatrix.M);
	createMatrixTable4x4("View Matrix", MVPMatrix.V);
	createMatrixTable4x4("Projection Matrix", MVPMatrix.P);
	ImGui::End();


	//ImGui::ShowDemoWindow();

}

void Menu::createMatrixTable4x4(const char* label, float* matrix) {
	if (ImGui::TreeNode(label)) {
		if (ImGui::BeginTable(label, 4)) {

			int index = 0;

			for (int row = 0; row < 4; row++) {
				ImGui::TableNextRow	();
				for (int column = 0; column < 4; column++) {

					ImGui::TableSetColumnIndex(column);
					ImGui::InputFloat(std::to_string(index).c_str(), &matrix[index]); //c_str is a pointer to immutable, null terminated char array (C-style string)

					index++;
				}
			}

			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void Menu::render() { // Goes over swapBuffers
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Menu::shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}