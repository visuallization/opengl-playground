#include "Scene.h"
#include "imgui/imgui.h"

namespace scene {
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