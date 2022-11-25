#include "SceneImGuiDemo.h"

#include "imgui/imgui.h"
#include "Renderer.h"

namespace scene {
	SceneImGuiDemo::SceneImGuiDemo(GLFWwindow*& window) : 
		Scene::Scene(window)
	{
	}

	SceneImGuiDemo::~SceneImGuiDemo() {

	}

	void SceneImGuiDemo::OnUpdate(float deltaTime) {

	}

	void SceneImGuiDemo::OnRender() {
	}

	void SceneImGuiDemo::OnImGuiRender() {
		ImGui::ShowDemoWindow();
	}

}