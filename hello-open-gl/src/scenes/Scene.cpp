#include "Scene.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace scene {
	void Scene::OnImGuiRender() {
		ImGui::Checkbox("Draw wireframe", &m_Debug);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	}

	void Scene::OnRender() {
		if (m_Debug) {
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		} else {
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		}
	}

	SceneMenu::SceneMenu(Scene*& currentScene): m_CurrentScene(currentScene) {

	}

	void SceneMenu::OnImGuiRender() {
		for (auto& scene : m_Scenes) {
			if (ImGui::Button(scene.first.c_str())) {
				m_CurrentScene = scene.second();
			}
		}
	}
}